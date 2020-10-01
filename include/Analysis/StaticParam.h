//===------------------------------------------------------------*- C++ -*-===//
//
//===----------------------------------------------------------------------===//

#ifndef SCALEHLS_ANALYSIS_STATICPARAM_H
#define SCALEHLS_ANALYSIS_STATICPARAM_H

#include "mlir/IR/Operation.h"
#include "mlir/IR/Value.h"

namespace mlir {
namespace scalehls {

//===----------------------------------------------------------------------===//
// ParamBase class
//===----------------------------------------------------------------------===//

template <typename ParamKind, typename KeyType> class ParamBase {
public:
  void init(KeyType key) {
    for (unsigned i = 0; i < (unsigned)ParamKind::KindNum; ++i)
      Params[key].push_back(0);
  }

  unsigned get(KeyType key, ParamKind kind) {
    return Params[key][(unsigned)kind];
  }

  void set(KeyType key, ParamKind kind, unsigned param) {
    Params[key][(unsigned)kind] = param;
  }

  DenseMap<KeyType, SmallVector<unsigned, 16>> Params;
};

//===----------------------------------------------------------------------===//
// ProcParam and MemParam classes
//===----------------------------------------------------------------------===//

enum class ProcParamKind {
  // Process-related pragam configurations.
  EnablePipeline,
  UnrollFactor,

  // Process attributes.
  LowerBound,
  UpperBound,
  IterNumber,
  IsPerfect,

  // Performance parameters.
  InitInterval,
  IterLatency,
  PipeIterNumber,
  Latency,

  // Resource parameters.
  LUT,
  BRAM,
  DSP,

  KindNum = DSP + 1
};

enum class MemParamKind {
  // Pragma configurations.
  StorageType,
  StorageImpl,
  PartitionType,
  PartitionFactor,
  InterfaceMode,

  // Performance parameters.
  ReadNum,
  WriteNum,
  ReadPorts,
  WritePorts,
  DepdcyLatency,
  DepdcyDistance,

  // Resource parameters.
  LUT,
  BRAM,

  KindNum = BRAM + 1
};

/// This class includes all possible parameters kind for "processes" (function,
/// for/parallel loop, and if).
class ProcParam : public ParamBase<ProcParamKind, Operation *> {};

/// This class includes all possible parameters kind for memories (memref,
/// tensor, and vector).
class MemParam : public ParamBase<MemParamKind, Value> {};

} // namespace scalehls
} // namespace mlir

#endif // SCALEHLS_ANALYSIS_STATICPARAM_H
