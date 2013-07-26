//===-- CILBackend.cpp - Library for converting LLVM code to CIL --------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This library converts LLVM code to CIL code.
//
//===----------------------------------------------------------------------===//

#include "CILBackend.h"
#include "CILTargetMachine.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/Analysis/ConstantsScanner.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/IR/ValueSymbolTable.h"
#include "llvm/PassSupport.h"
#include "llvm/Support/CallSite.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Transforms/Scalar.h"
#include <algorithm>

using namespace llvm;

extern "C" void LLVMInitializeCILTarget()
{
  RegisterTargetMachine<CILTargetMachine> X(TheCILTarget);
}

/******************************************************************************/
char CILWriter::ID = 0;
INITIALIZE_PASS_BEGIN(CILWriter,
                      "cil-writer",
                      "CIL Writer",
                      false,
                      false)
INITIALIZE_PASS_DEPENDENCY(DataLayout)
INITIALIZE_PASS_DEPENDENCY(LoopInfo)
INITIALIZE_PASS_DEPENDENCY(FindUsedTypes)
INITIALIZE_PASS_END(CILWriter,
                    "cil-writer",
                    "CIL Writer",
                    false,
                    false)

bool CILWriter::doInitialization(Module &Mod)
{
  return false;
}

bool CILWriter::doFinalization(Module &Mod)
{
  return false;
}

bool CILWriter::runOnModule(Module &Mod)
{
  const Function *Main = Mod.getFunction("main");
  if (Main == NULL)
    llvm_unreachable("Can't find main function");

  printModule(Mod);

  return true;
}

////////////////////////////////////////
CILType *CILWriter::convertType(const Type *T)
{
  switch (T->getTypeID()) {
  case Type::VoidTyID:    
    return CILType::getVoidTy();
  case Type::FloatTyID:
    return CILType::getFloat32Ty();
  case Type::DoubleTyID:
    return CILType::getFloat64Ty();
  case Type::IntegerTyID:
    switch (static_cast<const IntegerType *>(T)->getBitWidth()) {
    case 1:
      return CILType::getBoolTy();
    case 8:
      return CILType::getInt8Ty();
    case 16:
      return CILType::getInt16Ty();
    case 32:
      return CILType::getInt32Ty();
    case 64:
      return CILType::getInt64Ty();
    default:
      llvm_unreachable("Unsupported Integer width");
    }
  case Type::PointerTyID:
    {
      const Type *E = static_cast<const PointerType *>(T)->getElementType();
      return CILPointerType::get(convertType(E));
    }
  case Type::ArrayTyID:
    {
      const Type *E = static_cast<const ArrayType *>(T)->getElementType();
      return CILVectorType::get(convertType(E));
    }
  case Type::StructTyID:
  case Type::FunctionTyID:
    llvm_unreachable("Support for this type not yet implemented");
  default:
    llvm_unreachable("Unsupported Type");
  }
}

////////////////////////////////////////
void CILWriter::makeNames(Function &Fun) const
{
  uint16_t NextFresh = 1;
  for (inst_iterator I = inst_begin(Fun), E = inst_end(Fun); I != E; ++I) {
    Instruction &Inst = *I;
    if (Inst.hasNUsesOrMore(1) && !Inst.hasName()) {
      Inst.setName(Twine('_') + Twine(NextFresh));
      ++NextFresh;
    }
  }
}

////////////////////////////////////////
void CILWriter::makeLocals(const Function &Fun)
{
  Locals.clear();

  uint16_t Index = 0;
  for (const_inst_iterator I = inst_begin(Fun), E = inst_end(Fun);
       I != E;
       ++I) {
    const Value &Val = *I;
    if (Val.hasNUsesOrMore(1)) {
      Locals.insert(std::make_pair(&Val, Index));
      ++Index;
    }
  }
}

bool CILWriter::isLocal(const Value *Val) const
{
  return Locals.count(Val) > 0;
}

uint16_t CILWriter::getLocalIndex(const Value *Val) const
{
  return Locals.lookup(Val);
}

void CILWriter::printLocals() const
{
  std::vector<const Value *> Vals;

  for (const std::pair<const Value *, uint16_t> &Local : Locals)
    Vals.push_back(Local.first);

  std::sort(Vals.begin(), Vals.end(),
            [&](const Value *Val0, const Value *Val1) {
              return Locals.lookup(Val0) < Locals.lookup(Val1);
            });

  for (const Value *Val : Vals) {
    const CILType *Ty = convertType(Val->getType());
    *Out << "  .locals (";
    Ty->print(*Out);
    *Out << " " << Val->getName() << ")\n";
  }
}

////////////////////////////////////////
void CILWriter::emitInstruction(const cil::Instruction *Inst)
{
  Instructions.push_back(Inst);
}

void CILWriter::loadValue(const Value *Val)
{
  if (const Argument *Arg = dyn_cast<Argument>(Val))
    emitInstruction(new cil::LoadArg(Arg->getArgNo()));
  else if (isLocal(Val))
    emitInstruction(new cil::LoadLocal(Locals[Val]));
  else if (const ConstantInt *ConstInt = dyn_cast<ConstantInt>(Val)) {
    const CILType *Ty = convertType(ConstInt->getType());
    switch (Ty->getTypeID()) {
    case CILType::Int32TyID:
      emitInstruction(new cil::LoadConst_Int32(ConstInt->getSExtValue()));
      break;
    case CILType::Int64TyID:
      emitInstruction(new cil::LoadConst_Int64(ConstInt->getSExtValue()));
      break;
    default:
      llvm_unreachable("loadValue: ConstantInt invalid type");
    }
  }
}

void CILWriter::processInstruction(const Instruction *Inst)
{
  DataLayout &DL = getAnalysis<DataLayout>();

  DEBUG(*Out
        << "*** processInstruction: "
        << Inst->getOpcodeName() << " "
        << Inst->getOpcode() << "\n");

  if (const AllocaInst *_Inst = dyn_cast<AllocaInst>(Inst)) {
    int32_t Size = DL.getTypeAllocSize(_Inst->getAllocatedType());
    const Value *Count = _Inst->getArraySize();
    if (! isa<IntegerType>(Count->getType()))
      llvm_unreachable("AllocaInst.getArraySize() not an IntegerType");

    loadValue(Count);
    emitInstruction(new cil::LoadConst_Int32(Size));
    emitInstruction(new cil::Mul());
    emitInstruction(new cil::Alloca());
  }
  else if (const LoadInst *_Inst = dyn_cast<LoadInst>(Inst)) {
    
  }
  else if (const StoreInst *_Inst = dyn_cast<StoreInst>(Inst)) {
    
  }
  else if (const ReturnInst *_Inst = dyn_cast<ReturnInst>(Inst)) {
    if (const Value *Val = _Inst->getReturnValue())
      loadValue(Val);
    emitInstruction(new cil::Ret());
  }
  else if (const BinaryOperator *_Inst = dyn_cast<BinaryOperator>(Inst)) {
    loadValue(_Inst->getOperand(0));
    loadValue(_Inst->getOperand(1));
    switch (_Inst->getOpcode()) {
    case Instruction::Add:
      emitInstruction(new cil::Add());
      break;
    case Instruction::FAdd:
      emitInstruction(new cil::Add());
      break;
    case Instruction::Sub:
      emitInstruction(new cil::Sub());
      break;
    case Instruction::FSub:
      emitInstruction(new cil::Sub());
      break;
    case Instruction::Mul:
      emitInstruction(new cil::Mul());
      break;
    case Instruction::FMul:
      emitInstruction(new cil::Mul());
      break;
    case Instruction::UDiv:
      emitInstruction(new cil::Div(true));
      break;
    case Instruction::SDiv:
      emitInstruction(new cil::Div());
      break;
    case Instruction::FDiv:
      emitInstruction(new cil::Div());
      break;
    case Instruction::URem:
      emitInstruction(new cil::Rem(true));
      break;
    case Instruction::SRem:
      emitInstruction(new cil::Rem());
      break;
    case Instruction::FRem:
      emitInstruction(new cil::Rem());
      break;
    }
  }

  if (isLocal(Inst)) {
    emitInstruction(new cil::StoreLocal(Locals[Inst]));
  }
}

void CILWriter::makeInstructions(const Function &Fun)
{
  for (const_inst_iterator I = inst_begin(Fun), E = inst_end(Fun);
       I != E;
       ++I) {
    const Instruction *Inst = &*I;
    processInstruction(Inst);
  }
}

void CILWriter::printInstructions() const
{
  for (const cil::Instruction *Inst : Instructions) {
    *Out << "  ";
    Inst->print(*Out);
    *Out << "\n";
  }
}

////////////////////////////////////////
void CILWriter::printModule(Module &Mod)
{
  *Out << ".assembly extern mscorlib { }\n"
       << ".assembly CIL { }\n"
       << "\n.method static public int32 $CIL_Entry()\n"
       << "{\n"
       << "  .entrypoint\n"
       << "  call int32 main()\n"
       << "  ret\n"
       << "}\n";

  for (Module::global_iterator I = Mod.global_begin(), E = Mod.global_end();
       I != E;
       ++I) {
    GlobalVariable &Var = *I;
    if (! (Var.isDeclaration() || Var.hasAvailableExternallyLinkage()))
      printVariable(Var);
  }

  for (Function &Fun : Mod)
    if (! (Fun.isDeclaration() || Fun.hasAvailableExternallyLinkage()))
      printFunction(Fun);
}

void CILWriter::printVariable(GlobalVariable &Var)
{
}

void CILWriter::printFunction(Function &Fun)
{
  makeNames(Fun);
  makeLocals(Fun);
  makeInstructions(Fun);

  *Out << "\n.method static "
       << (Fun.hasLocalLinkage() ? "private " : "public ")
       << (Fun.isVarArg() ? "vararg " : "");
  const CILType *RetTy = convertType(Fun.getReturnType());
  RetTy->print(*Out);
  *Out << " " << Fun.getName() << "(";

  // Arguments
  for (Function::const_arg_iterator B = Fun.arg_begin(), E = Fun.arg_end(),
         I = B;
       I != E;
       ++I) {
    const Argument &Arg = *I;
    const CILType *Ty = convertType(Arg.getType());
    StringRef Name = Arg.getName();

    if (I != B)
      *Out << ", ";
    Ty->print(*Out);
    *Out << " " << Name;
  }

  *Out << ")\n{\n";
  printLocals();
  printInstructions();
  *Out << "}\n";
}

/******************************************************************************/
bool CILTargetMachine::addPassesToEmitFile(PassManagerBase &PM,
                                           formatted_raw_ostream &Out,
                                           CodeGenFileType FileType,
                                           bool DisableVerify,
                                           AnalysisID StartAfter,
                                           AnalysisID StopAfter)
{
  if (FileType != TargetMachine::CGFT_AssemblyFile)
    return true;

  PM.add(createGCLoweringPass());
  PM.add(createLowerSwitchPass());
  PM.add(createCFGSimplificationPass());

  CILWriter *Writer = new CILWriter(Out);
  PM.add(Writer);

  return false;
}
