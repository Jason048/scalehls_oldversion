//===----------------------------------------------------------------------===//
//
// Copyright 2020-2021 The ScaleHLS Authors.
//
//===----------------------------------------------------------------------===//

#include "mlir/Analysis/LoopAnalysis.h"
#include "mlir/Pass/PassManager.h"
#include "mlir/Transforms/GreedyPatternRewriteDriver.h"
#include "mlir/Transforms/Passes.h"
#include "scalehls/Analysis/QoREstimation.h"
#include "scalehls/Transforms/Passes.h"
#include "llvm/Support/Debug.h"

#define DEBUG_TYPE "scalehls"

using namespace mlir;
using namespace scalehls;

//===----------------------------------------------------------------------===//
// Helper methods
//===----------------------------------------------------------------------===//

static int64_t getInnerParallelism(AffineForOp forOp) {
  int64_t count = 0;
  for (auto loop : forOp.getOps<AffineForOp>()) {
    auto innerCount = getInnerParallelism(loop);
    if (auto trip = getConstantTripCount(loop))
      count += trip.getValue() * innerCount;
    else
      count += innerCount;
  }

  // If the current loop is innermost loop, count should be one.
  return std::max(count, (int64_t)1);
}

//===----------------------------------------------------------------------===//
// Optimizer Class Declaration
//===----------------------------------------------------------------------===//

class HLSCppOptimizer : public HLSCppAnalysisBase {
public:
  explicit HLSCppOptimizer(FuncOp func, LatencyMap &latencyMap, int64_t numDSP)
      : HLSCppAnalysisBase(OpBuilder(func)), func(func), latencyMap(latencyMap),
        numDSP(numDSP) {
    // TODO: only insert affine-related patterns.
    OwningRewritePatternList owningPatterns;
    for (auto *op : func.getContext()->getRegisteredOperations())
      op->getCanonicalizationPatterns(owningPatterns, func.getContext());
    patterns = std::move(owningPatterns);
  }

  using TileSizes = SmallVector<unsigned, 8>;

  enum LoopState { HOT = 0, COLD = 1, FROZEN = 2 };
  using BandState = SmallVector<LoopState, 8>;

  bool loopBandIsFrozen(BandState bandState) {
    for (auto loopState : bandState)
      if (loopState != LoopState::FROZEN)
        return false;
    return true;
  }

  bool loopBandIsColdOrFrozen(BandState bandState) {
    for (auto loopState : bandState)
      if (loopState == LoopState::HOT)
        return false;
    return true;
  }

  bool loopBandIsOneHot(BandState bandState) {
    unsigned hotNum = 0;
    for (auto loopState : bandState)
      if (loopState == LoopState::HOT)
        hotNum++;

    if (hotNum == 1)
      return true;
    else
      return false;
  }

  void emitDebugInfo(FuncOp targetFunc, StringRef message);
  bool applyLoopTilingStrategy(FuncOp targetFunc,
                               ArrayRef<TileSizes> tileSizesList);

  bool incrTileSizeAtLoc(TileSizes &tileSizes, const TileSizes &tripCounts,
                         unsigned &loc);

  /// This is a temporary approach that does not scale.
  void applyMultipleLevelDSE();

  FuncOp func;
  LatencyMap &latencyMap;
  int64_t numDSP;
  FrozenRewritePatternList patterns;
};

//===----------------------------------------------------------------------===//
// Optimizer Class Definition
//===----------------------------------------------------------------------===//

void HLSCppOptimizer::emitDebugInfo(FuncOp targetFunc, StringRef message) {
  LLVM_DEBUG(auto latency = getIntAttrValue(targetFunc, "latency");
             auto dsp = getIntAttrValue(targetFunc, "dsp");

             llvm::dbgs() << message << "\n";
             llvm::dbgs() << "Current latency is " << Twine(latency)
                          << ", DSP utilization is " << Twine(dsp) << ".\n\n";);
}

bool HLSCppOptimizer::applyLoopTilingStrategy(
    FuncOp targetFunc, ArrayRef<TileSizes> tileSizesList) {
  AffineLoopBands targetBands;
  getLoopBands(targetFunc.front(), targetBands);

  // Apply loop tiling.
  SmallVector<AffineForOp, 4> pipelineLoops;
  unsigned idx = 0;
  for (auto &band : targetBands)
    if (auto loop =
            applyPartialAffineLoopTiling(band, builder, tileSizesList[idx++]))
      pipelineLoops.push_back(loop);
    else
      return false;
  applyPatternsAndFoldGreedily(targetFunc, patterns);

  // Apply loop pipelining.
  for (auto loop : pipelineLoops)
    applyLoopPipelining(loop, builder);
  applyPatternsAndFoldGreedily(targetFunc, patterns);

  // Apply general optimizations and array partition.
  PassManager passManager(targetFunc.getContext(), "func");
  passManager.addPass(createSimplifyAffineIfPass());
  passManager.addPass(createAffineStoreForwardPass());
  passManager.addPass(createSimplifyMemrefAccessPass());
  passManager.addPass(createCSEPass());
  passManager.addPass(createArrayPartitionPass());

  if (failed(passManager.run(targetFunc)))
    return false;
  applyPatternsAndFoldGreedily(targetFunc, patterns);

  // Estimate performance and resource utilization.
  HLSCppEstimator(targetFunc, latencyMap).estimateFunc();
  LLVM_DEBUG(llvm::dbgs() << "Current tiling strategy:\n";
             for (unsigned idx = 0; idx < targetBands.size(); ++idx) {
               auto tileSizes = tileSizesList[idx];
               auto loop = pipelineLoops[idx];
               llvm::dbgs() << "Loop band " << Twine(idx) << ":";

               llvm::dbgs()
                   << " ResII=" << getIntAttrValue(loop, "resource_ii")
                   << " DepII=" << getIntAttrValue(loop, "dependence_ii")
                   << " IterLatency=" << getIntAttrValue(loop, "iter_latency")
                   << " DSP=" << getIntAttrValue(loop, "dsp") << " TileVector=";

               for (auto size : tileSizes)
                 llvm::dbgs() << " " << Twine(size);
               llvm::dbgs() << "\n";
             });

  emitDebugInfo(targetFunc, "Apply loop tiling and pipelining, generic IR "
                            "opts, and array partition.");
  return true;
}

bool HLSCppOptimizer::incrTileSizeAtLoc(TileSizes &tileSizes,
                                        const TileSizes &tripCounts,
                                        unsigned &loc) {
  auto size = tileSizes[loc];
  auto tripCount = tripCounts[loc];

  if (size >= tripCount || tripCount % size != 0)
    return false;

  // Fine the minimum factor that can be applied.
  unsigned factor = 2;
  while (tripCount % (size * factor) != 0)
    factor++;

  // Increase and update tile size.
  size *= factor;
  tileSizes[loc] = size;
  return true;
}

/// This is a temporary approach that does not scale.
void HLSCppOptimizer::applyMultipleLevelDSE() {
  applyPatternsAndFoldGreedily(func, patterns);
  HLSCppEstimator(func, latencyMap).estimateFunc();
  if (getIntAttrValue(func, "dsp") > numDSP)
    return;
  emitDebugInfo(func, "Start multiple level design space exploration.");

  //===--------------------------------------------------------------------===//
  // STAGE 1: Simplify Loop Nests Structure
  //===--------------------------------------------------------------------===//

  // Simplify loop nests by pipelining. If we take the following loops as
  // example, where each nodes represents one sequential loop nests (LN). In the
  // simplification, we'll first try to pipeline LN1 and LN6. Suppose pipelining
  // LN6 meets the resource constaints while pipelining LN1 not, we'll pipeline
  // LN6 (fully unroll LN7 and LN8) and keep LN1 untouched. In the next step,
  // we'll look into LN1 and check whether LN2 can be pipelined. Suppose
  // pipelining LN2 meets the resource constraints, we'll pipeling LN2 (fully
  // unroll LN7 and LN8). Note that in this simplification, all LNs that don't
  // contain any LNs will not be pipelined, such as LN5. Their optimization will
  // be explored later. This procedure will recursively applied to inner LNs
  // until no eligible LN exists.
  //
  //     LN1      LN6
  //      |        |
  //     / \      / \
  //   LN2 LN5  LN7 LN8
  //    |
  //   / \
  // LN3 LN4
  //
  // After the simplification, the loop becomes the following one, where LN1 has
  // been proved untouchable as loop pipelining is the primary optimization that
  // consumes the least extra resources. Formally, in the simplified function,
  // all non-leaf LNs is untouchable (LN1) and only leaf LNs can be further
  // optimized (LN2, LN5, and LN6).
  //
  //     LN1      LN6
  //      |
  //     / \
  //   LN2 LN5
  //
  // TODO: there is a large design space in this simplification.

  auto funcForOps = func.getOps<AffineForOp>();
  auto targetLoops =
      SmallVector<AffineForOp, 8>(funcForOps.begin(), funcForOps.end());

  while (!targetLoops.empty()) {
    SmallVector<AffineForOp, 8> candidateLoops;
    llvm::SmallDenseMap<Operation *, int64_t, 8> parallelismMap;

    // Collect all candidate loops. Here, only loops whose innermost loop has
    // more than one inner loops will be considered as a candidate.
    for (auto target : targetLoops) {
      AffineLoopBand loopBand;
      auto innermostLoop = getLoopBandFromRoot(target, loopBand);

      // Calculate the overall introduced parallelism if the innermost loop of
      // the current loop band is pipelined.
      auto parallelism = getInnerParallelism(innermostLoop);

      // Collect all candidate loops into an ordered vector. The loop indicating
      // the largest parallelism will show in the front.
      if (parallelism > 1) {
        parallelismMap[innermostLoop] = parallelism;

        for (auto it = candidateLoops.begin(); it <= candidateLoops.end(); ++it)
          if (it == candidateLoops.end()) {
            candidateLoops.push_back(innermostLoop);
            break;
          } else if (parallelism < parallelismMap[*it]) {
            candidateLoops.insert(it, innermostLoop);
            break;
          }
      }
    }

    // Since all target loops have been handled, clear the targetLoops vector.
    targetLoops.clear();

    // Traverse all candidates to check whether applying loop pipelining has
    // violation with the resource constraints. If so, add all inner loops into
    // targetLoops. Otherwise, pipeline the candidate.
    for (auto &candidate : candidateLoops) {
      // Create a temporary function.
      setAttrValue(candidate, "opt_flag", true);
      auto tmpFunc = func.clone();

      // Find the candidate loop in the temporary function and apply loop
      // pipelining to it.
      tmpFunc.walk([&](AffineForOp loop) {
        if (getIntAttrValue(loop, "opt_flag")) {
          applyLoopPipelining(loop, builder);
          return;
        }
      });

      // Estimate the temporary function.
      HLSCppEstimator(tmpFunc, latencyMap).estimateFunc();

      // Pipeline the candidate loop or delve into child loops.
      if (getIntAttrValue(tmpFunc, "dsp") <= numDSP)
        applyLoopPipelining(candidate, builder);
      else {
        auto childForOps = candidate.getOps<AffineForOp>();
        targetLoops.append(childForOps.begin(), childForOps.end());
      }

      candidate.removeAttr("opt_flag");
    }
  }

  HLSCppEstimator(func, latencyMap).estimateFunc();
  if (getIntAttrValue(func, "dsp") > numDSP)
    return;
  emitDebugInfo(func, "1. Simplify loop nests structure.");

  //===--------------------------------------------------------------------===//
  // STAGE 2: Loop Bands Optimization
  //===--------------------------------------------------------------------===//

  // Optimize leaf loop nests. Different optimization conbinations will be
  // applied to each leaf LNs, and the best one which meets the resource
  // constraints will be picked as the final solution.
  // TODO: better handle variable bound kernels.
  AffineLoopBands targetBands;
  getLoopBands(func.front(), targetBands);

  // Loop perfection, remove variable bound, and loop order optimization are
  // always applied for the convenience of polyhedral optimizations.
  for (auto &band : targetBands) {
    applyAffineLoopPerfection(band.back(), builder);
    applyAffineLoopOrderOpt(band);
    applyRemoveVariableBound(band.front(), builder);
  }

  // Estimate the current latency.
  HLSCppEstimator(func, latencyMap).estimateFunc();
  if (getIntAttrValue(func, "dsp") > numDSP)
    return;
  emitDebugInfo(func, "2. Apply loop perfection, remove variable bound, and "
                      "loop order opt.");

  //===--------------------------------------------------------------------===//
  // STAGE 3: Loop Bands Tiling and Finalization
  //===--------------------------------------------------------------------===//

  // Hold trip counts of all loops in each loop band, this can also be
  // considered as maxTileSizesList.
  std::vector<TileSizes> tripCountsList;
  // Hold the loop number in each loop band.
  SmallVector<unsigned, 8> loopNumList;

  // Hold the current tiling sizes of each loop band. This is the main design
  // vector which will evolve in the procedure of DSE.
  std::vector<TileSizes> tileSizesList;
  // Hold the DSE status of all loops in each loop band.
  std::vector<BandState> BandStateList;

  // Hold the current loop tiling location in each loop band.
  SmallVector<unsigned, 8> headLocList;

  // Initialize all lists.
  for (auto band : targetBands) {
    TileSizes tripCounts;
    for (auto loop : band)
      tripCounts.push_back(getIntAttrValue(loop, "trip_count"));

    // These two lists will not be modified in the DSE.
    tripCountsList.push_back(tripCounts);
    loopNumList.push_back(band.size());

    // These two lists will evolve in the DSE.
    tileSizesList.push_back(TileSizes(band.size(), 1));
    BandStateList.push_back(BandState(band.size(), LoopState::COLD));

    headLocList.push_back(0);
  }

  // Try and record the none tiling performance.
  auto nonTileFunc = func.clone();
  applyLoopTilingStrategy(nonTileFunc, tileSizesList);
  unsigned minLatency = getIntAttrValue(nonTileFunc, "latency");

  if (getIntAttrValue(nonTileFunc, "dsp") > numDSP)
    return;
  nonTileFunc.erase();
  LLVM_DEBUG(llvm::dbgs() << "3. Search for the best tiling strategy.\n";);

  unsigned targetNum = targetBands.size();
  unsigned iteration = 0;
  // Main loop for design space exploration.
  // for (unsigned i = 0; i < targetNum; ++i) {
  //   while (true) {
  //     auto &tileSizes = tileSizesList[i];
  //     for (unsigned loc = 0; loc < loopNumList[i]; ++loc) {
  //       auto &tileSize = tileSizes[loc];
  //       if (loc == 0)
  //         tileSize *= 2;
  //       else if (tileSizes[loc - 1] == tripCountsList[i][loc - 1])
  //         tileSize *= 2;
  //     }

  //     auto lastLoc = loopNumList[i] - 1;
  //     if (tileSizes[lastLoc] == tripCountsList[i][lastLoc]) {
  //       for (unsigned loc = 0; loc < loopNumList[i]; ++loc)
  //         tileSizes[loc] = 1;
  //       break;
  //     }

  //     for (unsigned loc = 0; loc < loopNumList[i]; ++loc) {
  //       auto &tileSize = tileSizes[loc];
  //       if (tileSize == tripCountsList[i][loc])
  //         tileSize = 1;
  //     }

  //     auto tmpFunc = func.clone();
  //     applyLoopTilingStrategy(tmpFunc, tileSizesList);
  //   }
  // }

  while (true) {
    LLVM_DEBUG(llvm::dbgs() << "Iteration " << iteration++ << ":\n\n";);
    bool isAllFrozen = true;
    // Walk through each target loop band.
    for (unsigned i = 0; i < targetNum; ++i) {
      auto &bandState = BandStateList[i];

      // Update state of the current loop band.
      for (unsigned loc = 0; loc < loopNumList[i]; ++loc)
        if (tileSizesList[i][loc] >= tripCountsList[i][loc])
          bandState[loc] = LoopState::FROZEN;

      // If all loop in the current loop band are frozen, continue and visit
      // next loop band.
      if (loopBandIsFrozen(bandState))
        continue;
      isAllFrozen = false;

      // If all loop in the current loop band are cold or frozen, walk through
      // all loop levels and heat the best one to hot state.
      if (loopBandIsColdOrFrozen(bandState)) {
        unsigned bestLoc = 0;
        unsigned bestLatency = UINT_MAX;

        for (unsigned loc = 0; loc < loopNumList[i]; ++loc) {
          if (bandState[loc] == LoopState::FROZEN)
            continue;

          // Increase the tile size of current location.
          auto tmpTileSizesList = tileSizesList;
          if (incrTileSizeAtLoc(tmpTileSizesList[i], tripCountsList[i], loc)) {
            // Try to apply the new tile size.
            auto tmpFunc = func.clone();
            if (applyLoopTilingStrategy(tmpFunc, tmpTileSizesList)) {
              auto latency = getIntAttrValue(tmpFunc, "latency");
              auto dsp = getIntAttrValue(tmpFunc, "dsp");

              if (dsp < numDSP && latency < bestLatency * 0.95) {
                bestLoc = loc;
                bestLatency = latency;
              }
              // Move to the next location.
              continue;
            }
          }

          // If the current loop cannot be further tiled, set it as frozen.
          bandState[loc] = LoopState::FROZEN;
        }

        if (bestLatency != UINT_MAX) {
          // Heat the best loop location. If the best latency is already better
          // than the minimum found latency, apply it. Otherwise, only heat the
          // location.
          bandState[bestLoc] = LoopState::HOT;
          if (bestLatency < minLatency * 0.95) {
            incrTileSizeAtLoc(tileSizesList[i], tripCountsList[i], bestLoc);
            minLatency = bestLatency;
          }
        } else {
          // If cannot find a proper tiling strategy for the current loop band,
          // frozen all loops.
          for (unsigned loc = 0; loc < loopNumList[i]; ++loc)
            bandState[loc] = LoopState::FROZEN;
        }
        // Move to the next DSE iteration.
        continue;
      }

      // For now, there should only one loop locations are in HOT state.
      if (loopBandIsOneHot(bandState)) {
        unsigned hotLoc = 0;
        for (unsigned loc = 0; loc < loopNumList[i]; ++loc)
          if (bandState[loc] == LoopState::HOT)
            hotLoc = loc;

        unsigned lastLatency = minLatency;
        unsigned tolerantCounter = 0;

        // Increase the tile size of current location until the latency is
        // improved or tile size cannot be further increased.
        auto tmpTileSizesList = tileSizesList;
        while (true) {
          // If the latency has not been improved for more than a certain
          // number of iterations, stop to increase tile size.
          if (tolerantCounter > 1) {
            bandState[hotLoc] = LoopState::FROZEN;
            break;
          }

          // Try to increase the tile size.
          if (incrTileSizeAtLoc(tmpTileSizesList[i], tripCountsList[i],
                                hotLoc)) {
            // Try to apply the new tile size.
            auto tmpFunc = func.clone();
            if (applyLoopTilingStrategy(tmpFunc, tmpTileSizesList)) {
              auto latency = getIntAttrValue(tmpFunc, "latency");
              auto dsp = getIntAttrValue(tmpFunc, "dsp");

              if (dsp < numDSP && latency < minLatency * 0.95) {
                // If find a new minimum latency, apply it.
                tileSizesList = tmpTileSizesList;
                minLatency = latency;
                break;
              } else if (dsp < numDSP && latency < lastLatency * 0.95) {
                // If the latency is better than the last iteration, even if it
                // is not the minimum latency, continue to try on the hot loop
                // location.
                lastLatency = latency;
                tolerantCounter = 0;
                continue;
              } else {
                // If the latency is worse than the last iteration, increase the
                // tolerant counter by 1 and continue to
                lastLatency = latency;
                tolerantCounter++;
                continue;
              }
            }
          }

          // If the hot location cannot contribute to the improvement of
          // latency, set it as frozen.
          bandState[hotLoc] = LoopState::FROZEN;
          break;
        }
      }
    }
    if (isAllFrozen)
      break;
  }

  // Finally, we found the best tiling strategy.
  LLVM_DEBUG(llvm::dbgs() << "4. Apply the best tiling strategy.\n";);
  applyLoopTilingStrategy(func, tileSizesList);
}

namespace {
struct MultipleLevelDSE : public MultipleLevelDSEBase<MultipleLevelDSE> {
  void runOnOperation() override {
    auto module = getOperation();

    // Read configuration file.
    INIReader spec(targetSpec);
    if (spec.ParseError())
      emitError(module.getLoc(), "target spec file parse fail\n");

    // Collect profiling data, where default values are based on PYNQ-Z1
    // board.
    LatencyMap latencyMap;
    getLatencyMap(spec, latencyMap);
    int64_t numDSP = ceil(spec.GetInteger("specification", "dsp", 220) * 1.1);

    // Optimize the top function.
    for (auto func : module.getOps<FuncOp>())
      if (auto topFunction = func->getAttrOfType<BoolAttr>("top_function"))
        if (topFunction.getValue())
          HLSCppOptimizer(func, latencyMap, numDSP).applyMultipleLevelDSE();
  }
};
} // namespace

std::unique_ptr<Pass> scalehls::createMultipleLevelDSEPass() {
  return std::make_unique<MultipleLevelDSE>();
}
