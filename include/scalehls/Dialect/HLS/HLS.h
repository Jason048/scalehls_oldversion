//===----------------------------------------------------------------------===//
//
// Copyright 2020-2021 The ScaleHLS Authors.
//
//===----------------------------------------------------------------------===//

#ifndef SCALEHLS_DIALECT_HLS_HLS_H
#define SCALEHLS_DIALECT_HLS_HLS_H

#include "mlir/Dialect/Affine/IR/AffineOps.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/IR/Dialect.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/Interfaces/ControlFlowInterfaces.h"
#include "mlir/Interfaces/SideEffectInterfaces.h"
#include "scalehls/Dialect/HLS/HLSDialect.h.inc"
#include "scalehls/Dialect/HLS/HLSEnums.h.inc"

#define GET_TYPEDEF_CLASSES
#include "scalehls/Dialect/HLS/HLSTypes.h.inc"

#define GET_ATTRDEF_CLASSES
#include "scalehls/Dialect/HLS/HLSAttributes.h.inc"

namespace mlir {
namespace scalehls {
namespace hls {

#include "scalehls/Dialect/HLS/HLSInterfaces.h.inc"

enum class PartitionKind { CYCLIC, BLOCK, NONE };
enum class OperandKind { INPUT, OUTPUT, PARAM };
enum class MemoryKind {
  LUTRAM_1P,
  LUTRAM_S2P,
  LUTRAM_T2P,
  BRAM_1P,
  BRAM_S2P,
  BRAM_T2P,
  URAM_1P,
  URAM_S2P,
  URAM_T2P,
  DRAM
};

bool isRam1P(MemoryKind kind);
bool isRamS2P(MemoryKind kind);
bool isRamT2P(MemoryKind kind);
bool isDram(MemoryKind kind);

namespace StreamEffects {
struct Instantiate : public MemoryEffects::Effect::Base<Instantiate> {};
struct Push : public MemoryEffects::Effect::Base<Push> {};
struct Pop : public MemoryEffects::Effect::Base<Pop> {};
} // namespace StreamEffects

/// Timing attribute utils.
TimingAttr getTiming(Operation *op);
void setTiming(Operation *op, TimingAttr timing);
void setTiming(Operation *op, int64_t begin, int64_t end, int64_t latency,
               int64_t interval);

/// Resource attribute utils.
ResourceAttr getResource(Operation *op);
void setResource(Operation *op, ResourceAttr resource);
void setResource(Operation *op, int64_t lut, int64_t dsp, int64_t bram);

/// Loop information attribute utils.
LoopInfoAttr getLoopInfo(Operation *op);
void setLoopInfo(Operation *op, LoopInfoAttr loopInfo);
void setLoopInfo(Operation *op, int64_t flattenTripCount, int64_t iterLatency,
                 int64_t minII);

/// Loop directives attribute utils.
LoopDirectiveAttr getLoopDirective(Operation *op);
void setLoopDirective(Operation *op, LoopDirectiveAttr loopDirective);
void setLoopDirective(Operation *op, bool pipeline, int64_t targetII,
                      bool dataflow, bool flatten);

/// Parrallel and point loop attribute utils.
bool hasParallelAttr(Operation *op);
void setParallelAttr(Operation *op);
bool hasPointAttr(Operation *op);
void setPointAttr(Operation *op);

/// Function directives attribute utils.
FuncDirectiveAttr getFuncDirective(Operation *op);
void setFuncDirective(Operation *op, FuncDirectiveAttr FuncDirective);
void setFuncDirective(Operation *op, bool pipeline, int64_t targetInterval,
                      bool dataflow);

/// Top and runtime function attribute utils.
bool hasTopFuncAttr(Operation *op);
void setTopFuncAttr(Operation *op);
bool hasRuntimeAttr(Operation *op);
void setRuntimeAttr(Operation *op);

class NodeOp;

} // namespace hls
} // namespace scalehls
} // namespace mlir

namespace mlir {
namespace OpTrait {

using namespace scalehls::hls;

template <typename ConcreteType>
class DataflowBufferLike : public TraitBase<ConcreteType, DataflowBufferLike> {
public:
  static LogicalResult verifyTrait(Operation *op) {
    if (op->getNumResults() != 1 ||
        !op->getResult(0).getType().isa<StreamType, MemRefType>())
      return failure();
    return success();
  }
};

} // namespace OpTrait
} // namespace mlir

#define GET_OP_CLASSES
#include "scalehls/Dialect/HLS/HLS.h.inc"

#endif // SCALEHLS_DIALECT_HLS_HLS_H
