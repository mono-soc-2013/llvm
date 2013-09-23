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
#include <list>
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
  static cil::Type *convertType(const Type *T);

  static bool isReachable(const BasicBlock *Target,
                          std::list<const BasicBlock *> &Sources);

private:
  DenseMap<GlobalVariable *, const cil::Type *> Globals;
protected:
  const cil::Type *getGlobalType(const GlobalVariable *Global);

private:
  DenseMap<AllocaInst *, uint16_t> RegLocals;
  DenseMap<AllocaInst *, uint16_t> IndLocals;
  DenseMap<Instruction *, uint16_t> TmpLocals;
protected:
  void makeLocals(Function &Fun);
  bool isRegLocal(const Value *Val) const;
  bool isIndLocal(const Value *Val) const;
  bool isTmpLocal(const Value *Val) const;
  uint16_t getRegLocalIndex(const Value *Val) const;
  uint16_t getIndLocalIndex(const Value *Val) const;
  uint16_t getTmpLocalIndex(const Value *Val) const;
  void printLocals() const;

protected:
  void makeNames(Function &Fun) const;

private:
  std::list<const cil::Instruction *> Instructions;
protected:
  void emitInstruction(const cil::Instruction *Inst);
  void loadValue(const Value *Val);
  void loadValueAddr(const Value *Val);
  void processInstruction(const Instruction *Inst);
  void makeInstructions(const BasicBlock &BB);
  void printInstructions() const;

protected:
  void printDataItem(const Constant *Init);
  void printDataBody(const Constant *Init);
  void printVariableDeclaration(GlobalVariable &Var);
  void printVariable(GlobalVariable &Var);

  void printFunctionDeclaration(Function &Fun);
  void printFunction(Function &Fun);

  void printModule(Module &Mod);
};

}

#endif

