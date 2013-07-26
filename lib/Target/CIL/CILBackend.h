//===-- CILBackend.h - TargetMachine for the CIL --------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===--------------------------------------------------------------------===//
//
// This file declares the CILWriter that is used by the CIL.
//
//===--------------------------------------------------------------------===//
#ifndef CILBACKEND_H
#define CILBACKEND_H

#include "CILTypes.h"
#include "CILInstructions.h"
#include "llvm/ADT/MapVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/ValueMap.h"
#include "llvm/Analysis/FindUsedTypes.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/CodeGen/IntrinsicLowering.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/PassManager.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/GetElementPtrTypeIterator.h"
#include "llvm/Target/TargetMachine.h"
#include <vector>

namespace llvm {
void initializeCILWriterPass(PassRegistry &);

class CILWriter : public ModulePass
{
public:
  static char ID;

private:
  formatted_raw_ostream *Out;

public:
  CILWriter()
    : ModulePass(ID)
  {
    initializeCILWriterPass(*PassRegistry::getPassRegistry());
  }

  CILWriter(formatted_raw_ostream &Out)
    : CILWriter()
  {
    this->Out = &Out;
  }

  virtual void getAnalysisUsage(AnalysisUsage &AU) const
  {
    AU.addRequired<DataLayout>();
    AU.addRequired<LoopInfo>();
    AU.addRequired<FindUsedTypes>();
  }

  virtual const char *getPassName() const
  {
    return "CIL Writer";
  }

public:
  virtual bool doInitialization(Module &Mod);
  virtual bool doFinalization(Module &Mod);
  virtual bool runOnModule(Module &Mod);

protected:
  static CILType *convertType(const Type *T);

protected:
  void makeNames(Function &Fun) const;

private:
  DenseMap<const Value *, uint16_t> Locals;
protected:
  void makeLocals(const Function &Fun);
  bool isLocal(const Value *Val) const;
  uint16_t getLocalIndex(const Value *Val) const;
  void printLocals() const;

private:
  std::vector<const cil::Instruction *> Instructions;
protected:
  void emitInstruction(const cil::Instruction *Inst);
  void loadValue(const Value *Val);
  void processInstruction(const Instruction *Inst);
  void makeInstructions(const Function &Fun);
  void printInstructions() const;

protected:
  void printModule(Module &Mod);
  void printVariable(GlobalVariable &Var);
  void printFunction(Function &Fun);
};

}

#endif

