//===-- CILBackend.cpp - Library for converting LLVM code to CIL ----------===//
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
  for (Function &Fun : Mod)
    if (! (Fun.isDeclaration() || Fun.hasAvailableExternallyLinkage())) {
      switch (Fun.getCallingConv()) {
      case CallingConv::C:
        Fun.setCallingConv(CallingConv::CIL_Static);
        break;
      default:
        llvm_unreachable("Unsupported CC in function definition");
      }
    }

  const Function *Main = Mod.getFunction("main");
  if (Main == NULL)
    llvm_unreachable("Can't find main function");

  printModule(Mod);

  return true;
}

////////////////////////////////////////
cil::Type *CILWriter::convertType(const Type *T)
{
  switch (T->getTypeID()) {
  case Type::VoidTyID:
    return cil::Type::getVoidTy();
  case Type::FloatTyID:
    return cil::Type::getFloat32Ty();
  case Type::DoubleTyID:
    return cil::Type::getFloat64Ty();
  case Type::IntegerTyID:
    switch (cast<const IntegerType>(T)->getBitWidth()) {
    case 1:
      return cil::Type::getBoolTy();
    case 8:
      return cil::Type::getInt8Ty();
    case 16:
      return cil::Type::getInt16Ty();
    case 32:
      return cil::Type::getInt32Ty();
    case 64:
      return cil::Type::getInt64Ty();
    default:
      llvm_unreachable("Unsupported Integer width");
    }
  case Type::PointerTyID:
    {
      const Type *E = cast<const PointerType>(T)->getElementType();
      return cil::PointerType::get(convertType(E));
    }
  case Type::ArrayTyID:
    {
      const Type *E = cast<const ArrayType>(T)->getElementType();
      return cil::VectorType::get(convertType(E));
    }
  case Type::FunctionTyID:
    {
      const FunctionType *FunTy = cast<const FunctionType>(T);
      const cil::Type *RetTy = convertType(FunTy->getReturnType());
      std::vector<const cil::Type *> ParamTys;
      for (auto Iter = FunTy->param_begin(), End = FunTy->param_end();
           Iter != End;
           ++Iter)
        ParamTys.push_back(convertType(*Iter));
      return cil::FunctionType::get(RetTy, ParamTys);
    }
  case Type::StructTyID:
    llvm_unreachable("Support for this type not yet implemented");
  default:
    llvm_unreachable("Unsupported Type");
  }
}

////////////////////////////////////////
bool CILWriter::isReachable(const BasicBlock *Target,
                            std::list<const BasicBlock *> &Sources)
{
  std::set<const BasicBlock *> Visited;

  while (! Sources.empty()) {
    const BasicBlock *Src = Sources.front();
    if (Src == Target)
      return true;
    else if (Visited.count(Src) == 0) {
      for (auto I = succ_begin(Src), E = succ_end(Src); I != E; ++I)
        Sources.push_back(*I);
      Visited.insert(Src);
    }
    Sources.pop_front();
  }

  return false;
}

////////////////////////////////////////
const cil::Type *CILWriter::getGlobalType(const GlobalVariable *Global)
{
  return Globals.lookup((GlobalVariable *)Global);
}

////////////////////////////////////////
void CILWriter::makeLocals(Function &Fun)
{
  RegLocals.clear();
  IndLocals.clear();
  TmpLocals.clear();

  uint16_t Index = 0;
  for (auto I = inst_begin(Fun), E = inst_end(Fun); I != E; ++I) {
    Instruction *Inst = &*I;
    if (Inst->hasNUsesOrMore(1)) {
      if (AllocaInst *_Inst = dyn_cast<AllocaInst>(Inst)) {
        const BasicBlock *Target = _Inst->getParent();
        std::list<const BasicBlock *> Sources(succ_begin(Target),
                                              succ_end(Target));
        if (isReachable(Target, Sources)) {
          IndLocals.insert(std::make_pair(_Inst, Index));
          ++Index;
        }
        else {
          RegLocals.insert(std::make_pair(_Inst, Index));
          ++Index;
        }
      }
      else {
        TmpLocals.insert(std::make_pair(Inst, Index));
        ++Index;
      }
    }
  }
}

bool CILWriter::isRegLocal(const Value *Val) const
{
  if (AllocaInst *Inst = dyn_cast<AllocaInst>((Value *)Val))
    return (RegLocals.count(Inst) > 0);
  else
    return false;
}

bool CILWriter::isIndLocal(const Value *Val) const
{
  if (AllocaInst *Inst = dyn_cast<AllocaInst>((Value *)Val))
    return (IndLocals.count(Inst) > 0);
  else
    return false;
}

bool CILWriter::isTmpLocal(const Value *Val) const
{
  if (Instruction *Inst = dyn_cast<Instruction>((Value *)Val))
    return (TmpLocals.count(Inst) > 0);
  else
    return false;
}

uint16_t CILWriter::getRegLocalIndex(const Value *Val) const
{
  return RegLocals.lookup(cast<AllocaInst>((Value *)Val));
}

uint16_t CILWriter::getIndLocalIndex(const Value *Val) const
{
  return IndLocals.lookup(cast<AllocaInst>((Value *)Val));
}

uint16_t CILWriter::getTmpLocalIndex(const Value *Val) const
{
  return TmpLocals.lookup(cast<Instruction>((Value *)Val));
}

void CILWriter::printLocals() const
{
  *Out << "  //// RegLocals\n";
  {
    unsigned Size = RegLocals.size();
    if (0 < Size) {
      *Out << "  .locals (";
      auto Iter = RegLocals.begin(), End = RegLocals.end();
      unsigned int I = 1;
      while (Iter != End) {
        const std::pair<const AllocaInst *, uint16_t> &Pair = *Iter;
        const cil::Type *Ty = convertType(Pair.first->getAllocatedType());
        StringRef Name = Pair.first->getName();
        uint16_t Index = Pair.second;
        *Out << (I == 1 ? "[" : "           [") << Index << "] "
             << *Ty << ' ' << Name
             << (I < Size ? ',' : ')') << '\n';
        ++Iter;
        ++I;
      }
    }
  }

  *Out << "  //// IndLocals\n";
  {
    unsigned Size = IndLocals.size();
    if (0 < Size) {
      *Out << "  .locals (";
      auto Iter = IndLocals.begin(), End = IndLocals.end();
      unsigned int I = 1;
      while (Iter != End) {
        const std::pair<const AllocaInst *, uint16_t> &Pair = *Iter;
        const cil::Type *Ty = convertType(Pair.first->getType());
        StringRef Name = Pair.first->getName();
        uint16_t Index = Pair.second;
        *Out << (I == 1 ? "[" : "           [") << Index << "] "
             << *Ty << ' ' << Name
             << (I < Size ? ',' : ')') << '\n';
        ++Iter;
        ++I;
      }
    }
  }

  *Out << "  //// TmpLocals\n";
  {
    unsigned Size = TmpLocals.size();
    if (0 < Size) {
      *Out << "  .locals (";
      auto Iter = TmpLocals.begin(), End = TmpLocals.end();
      unsigned int I = 1;
      while (Iter != End) {
        const std::pair<const Instruction *, uint16_t> &Pair = *Iter;
        const cil::Type *Ty = convertType(Pair.first->getType());
        StringRef Name = Pair.first->getName();
        uint16_t Index = Pair.second;
        *Out << (I == 1 ? "[" : "           [") << Index << "] "
             << *Ty << ' ' << Name
             << (I < Size ? ',' : ')') << '\n';
        ++Iter;
        ++I;
      }
    }
  }
}

////////////////////////////////////////
void CILWriter::makeNames(Function &Fun) const
{
  {
    uint32_t NextLabel = 0;
    for (BasicBlock &BB : Fun) {
      if (! BB.hasName()) {
        BB.setName(Twine("BB_") + Twine(NextLabel));
        ++NextLabel;
      }
    }
  }

  for (const std::pair<AllocaInst *, uint16_t> &Pair : RegLocals) {
    AllocaInst *Inst = Pair.first;
    uint16_t Index = Pair.second;
    if (! Inst->hasName())
      Inst->setName(Twine("$REG_") + Twine(Index));
  }
  for (const std::pair<AllocaInst *, uint16_t> &Pair : IndLocals) {
    AllocaInst *Inst = Pair.first;
    uint16_t Index = Pair.second;
    if (! Inst->hasName())
      Inst->setName(Twine("$IND_") + Twine(Index));
  }
  for (const std::pair<Instruction *, uint16_t> &Pair : TmpLocals) {
    Instruction *Inst = Pair.first;
    uint16_t Index = Pair.second;
    if (! Inst->hasName())
      Inst->setName(Twine("$TMP_") + Twine(Index));
  }
}

////////////////////////////////////////
void CILWriter::emitInstruction(const cil::Instruction *Inst)
{
  Instructions.push_back(Inst);
}

void CILWriter::loadValue(const Value *Val)
{
  if (const ConstantInt *ConstInt = dyn_cast<ConstantInt>(Val)) {
    const cil::Type *Ty = convertType(ConstInt->getType());
    switch (Ty->getTypeID()) {
    case cil::TypeID::Int32:
      emitInstruction(new cil::LoadConst_Int32(ConstInt->getSExtValue()));
      break;
    case cil::TypeID::Int64:
      emitInstruction(new cil::LoadConst_Int64(ConstInt->getSExtValue()));
      break;
    default:
      llvm_unreachable("loadValue: ConstantInt invalid type");
    }
  }
  else if (const Argument *Arg = dyn_cast<Argument>(Val))
    emitInstruction(new cil::LoadArg(Arg->getArgNo()));
  else if (isRegLocal(Val))
    emitInstruction(new cil::LoadLocal(getRegLocalIndex(Val)));
  else if (isIndLocal(Val))
    emitInstruction(new cil::LoadLocal(getIndLocalIndex(Val)));
  else if (isTmpLocal(Val))
    emitInstruction(new cil::LoadLocal(getTmpLocalIndex(Val)));
  else if (const GlobalVariable *Var = dyn_cast<GlobalVariable>(Val)) {
    const cil::Type *Ty = getGlobalType(Var);
    emitInstruction(new cil::LoadStaticField(Ty, Var->getName()));
  }
}

void CILWriter::loadValueAddr(const Value *Val)
{
  if (const Argument *Arg = dyn_cast<Argument>(Val))
    emitInstruction(new cil::LoadArgAddr(Arg->getArgNo()));
  else if (isRegLocal(Val))
    emitInstruction(new cil::LoadLocalAddr(getRegLocalIndex(Val)));
  else if (isIndLocal(Val))
    emitInstruction(new cil::LoadLocalAddr(getIndLocalIndex(Val)));
  else if (isTmpLocal(Val))
    emitInstruction(new cil::LoadLocalAddr(getTmpLocalIndex(Val)));
  else if (const GlobalVariable *Var = dyn_cast<GlobalVariable>(Val)) {
    const cil::Type *Ty = getGlobalType(Var);
    emitInstruction(new cil::LoadStaticFieldAddr(Ty, Var->getName()));
  }
}

void CILWriter::processInstruction(const Instruction *Inst)
{
  DataLayout &DL = getAnalysis<DataLayout>();

  if (const AllocaInst *_Inst = dyn_cast<AllocaInst>(Inst)) {
    if (isIndLocal(_Inst) || isTmpLocal(_Inst)) {
      int32_t Size = DL.getTypeAllocSize(_Inst->getAllocatedType());
      const Value *Count = _Inst->getArraySize();
      if (! isa<IntegerType>(Count->getType()))
        llvm_unreachable("AllocaInst.getArraySize() not an IntegerType");
      loadValue(Count);
      emitInstruction(new cil::LoadConst_Int32(Size));
      emitInstruction(new cil::Mul());
      emitInstruction(new cil::Alloca());
    }
  }
  else if (const LoadInst *_Inst = dyn_cast<LoadInst>(Inst)) {
    const Value *Operand = _Inst->getPointerOperand();
    loadValue(Operand);
    if (isIndLocal(Operand) || isTmpLocal(Operand)) {
      const cil::Type *Ty = convertType(Operand->getType());
      emitInstruction(new cil::LoadIndirect(Ty->getTypeID()));
    }
  }
  else if (const StoreInst *_Inst = dyn_cast<StoreInst>(Inst)) {
    const Value *Loc = _Inst->getPointerOperand();
    const Value *Val = _Inst->getValueOperand();
    if (isRegLocal(Loc)) {
      loadValue(Val);
      emitInstruction(new cil::StoreLocal(getRegLocalIndex(Loc)));
    }
    else {
      loadValue(Loc);
      loadValue(Val);
      const cil::Type *Ty = convertType(Val->getType());
      emitInstruction(new cil::StoreIndirect(Ty->getTypeID()));
    }
  }
  else if (const GetElementPtrInst *_Inst = dyn_cast<GetElementPtrInst>(Inst)) {
    loadValueAddr(_Inst->getOperand(0));
    for (gep_type_iterator
           Iter = gep_type_begin(_Inst),
           End = gep_type_end(_Inst);
         Iter != End;
         ++Iter) {
      Type *Ty = Iter.getIndexedType();
      const Value *Index = Iter.getOperand();
      emitInstruction(new cil::LoadConst_Int32(DL.getTypeAllocSize(Ty)));
      emitInstruction(new cil::Conv(cil::TypeID::UInt));
      loadValue(Index);
      emitInstruction(new cil::Conv(cil::TypeID::UInt));
      emitInstruction(new cil::Mul());
      emitInstruction(new cil::Add());
    }
  }
  else if (const ReturnInst *_Inst = dyn_cast<ReturnInst>(Inst)) {
    if (const Value *Val = _Inst->getReturnValue())
      loadValue(Val);
    emitInstruction(new cil::Ret());
  }
  else if (const BranchInst *_Inst = dyn_cast<BranchInst>(Inst)) {
    if (_Inst->isConditional()) {
      StringRef TrueLabel = _Inst->getSuccessor(0)->getName();
      StringRef FalseLabel = _Inst->getSuccessor(1)->getName();
      loadValue(_Inst->getCondition());
      emitInstruction(new cil::Dup());
      emitInstruction(new cil::BranchCond(true, TrueLabel));
      emitInstruction(new cil::BranchCond(false, FalseLabel));
    }
    else
      emitInstruction(new cil::Branch(_Inst->getSuccessor(0)->getName()));
  }
  else if (const CallInst *_Inst = dyn_cast<CallInst>(Inst)) {
    for (auto B = _Inst->op_begin(), E = _Inst->op_end(), I = B; I != E; ++I)
      loadValue(I->get());

    if (const Function *Fun = _Inst->getCalledFunction()) {
      const cil::FunctionType *Ty
        = static_cast<const cil::FunctionType *>(convertType(Fun->getFunctionType()));
      const cil::CallConv &CC = cil::CallConv::get();
      StringRef Name = Fun->getName();
      emitInstruction(new cil::Call(Ty, CC, Name));
    }
    else if (const Value *Val = _Inst->getCalledValue()) {
      if (const PointerType *Ty = dyn_cast<PointerType>(Val->getType())) {
        if (const FunctionType *_Ty = dyn_cast<FunctionType>(Ty->getElementType())) {
          for (auto B = Fun->arg_begin(), E = Fun->arg_end(), I = B; I != E; ++I)
            loadValue(I);
          const cil::FunctionType *FunTy
            = static_cast<const cil::FunctionType *>(convertType(_Ty));
          const cil::CallConv &CC = cil::CallConv::get();
          loadValue(Val);
          emitInstruction(new cil::CallIndirect(FunTy, CC));
        }
        else
          llvm_unreachable("CalledValue is not a Function Pointer");
      }
      else
        llvm_unreachable("CalledValue is not a Pointer");
    }
    else
      llvm_unreachable("CallInst without CalledFunction or CalledValue");

    if (!_Inst->getType()->isVoidTy() && _Inst->getNumUses() == 0)
      emitInstruction(new cil::Pop());
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
  else if (const CmpInst *_Inst = dyn_cast<CmpInst>(Inst)) {
    switch (_Inst->getPredicate()) {
    case CmpInst::FCMP_TRUE:
      emitInstruction(new cil::LoadConst_Int32(1));
      break;
    case CmpInst::FCMP_FALSE:
      emitInstruction(new cil::LoadConst_Int32(0));
      break;
    case CmpInst::ICMP_EQ:
    case CmpInst::FCMP_OEQ:
      loadValue(_Inst->getOperand(0));
      loadValue(_Inst->getOperand(1));
      emitInstruction(new cil::Cmp(cil::Cmp::EQ));
      break;
    case CmpInst::ICMP_SGT:
    case CmpInst::FCMP_OGT:
      loadValue(_Inst->getOperand(0));
      loadValue(_Inst->getOperand(1));
      emitInstruction(new cil::Cmp(cil::Cmp::GT));
      break;
    case CmpInst::ICMP_SGE:
    case CmpInst::FCMP_OGE:
      loadValue(_Inst->getOperand(0));
      loadValue(_Inst->getOperand(1));
      emitInstruction(new cil::Cmp(cil::Cmp::LT));
      emitInstruction(new cil::LoadConst_Int32(1));
      emitInstruction(new cil::Xor());
      break;
    case CmpInst::ICMP_SLT:
    case CmpInst::FCMP_OLT:
      loadValue(_Inst->getOperand(0));
      loadValue(_Inst->getOperand(1));
      emitInstruction(new cil::Cmp(cil::Cmp::LT));
      break;
    case CmpInst::ICMP_SLE:
    case CmpInst::FCMP_OLE:
      loadValue(_Inst->getOperand(0));
      loadValue(_Inst->getOperand(1));
      emitInstruction(new cil::Cmp(cil::Cmp::GT));
      emitInstruction(new cil::LoadConst_Int32(1));
      emitInstruction(new cil::Xor());
      break;
    case CmpInst::ICMP_NE:
    case CmpInst::FCMP_ONE:
      loadValue(_Inst->getOperand(0));
      loadValue(_Inst->getOperand(1));
      emitInstruction(new cil::Cmp(cil::Cmp::EQ));
      emitInstruction(new cil::LoadConst_Int32(1));
      emitInstruction(new cil::Xor());
      break;
    case CmpInst::FCMP_ORD:
      llvm_unreachable("CmpInst: FCMP_ORD not implemented");
      break;
    case CmpInst::FCMP_UNO:
      llvm_unreachable("CmpInst: FCMP_UNO not implemented");
      break;
    case CmpInst::FCMP_UEQ:
      loadValue(_Inst->getOperand(0));
      loadValue(_Inst->getOperand(1));
      emitInstruction(new cil::Cmp(cil::Cmp::EQ, true));
      break;
    case CmpInst::ICMP_UGT:
    case CmpInst::FCMP_UGT:
      loadValue(_Inst->getOperand(0));
      loadValue(_Inst->getOperand(1));
      emitInstruction(new cil::Cmp(cil::Cmp::GT, true));
      break;
    case CmpInst::ICMP_UGE:
    case CmpInst::FCMP_UGE:
      loadValue(_Inst->getOperand(0));
      loadValue(_Inst->getOperand(1));
      emitInstruction(new cil::Cmp(cil::Cmp::LT, true));
      emitInstruction(new cil::LoadConst_Int32(1));
      emitInstruction(new cil::Xor());
      break;
    case CmpInst::ICMP_ULT:
    case CmpInst::FCMP_ULT:
      loadValue(_Inst->getOperand(0));
      loadValue(_Inst->getOperand(1));
      emitInstruction(new cil::Cmp(cil::Cmp::LT, true));
      break;
    case CmpInst::ICMP_ULE:
    case CmpInst::FCMP_ULE:
      loadValue(_Inst->getOperand(0));
      loadValue(_Inst->getOperand(1));
      emitInstruction(new cil::Cmp(cil::Cmp::GT, true));
      emitInstruction(new cil::LoadConst_Int32(1));
      emitInstruction(new cil::Xor());
      break;
    case CmpInst::FCMP_UNE:
      loadValue(_Inst->getOperand(0));
      loadValue(_Inst->getOperand(1));
      emitInstruction(new cil::Cmp(cil::Cmp::EQ, true));
      emitInstruction(new cil::LoadConst_Int32(1));
      emitInstruction(new cil::Xor());
      break;
    }
  }

  if (isIndLocal(Inst))
    emitInstruction(new cil::StoreLocal(getIndLocalIndex(Inst)));
  else if (isTmpLocal(Inst))
    emitInstruction(new cil::StoreLocal(getTmpLocalIndex(Inst)));
}

void CILWriter::makeInstructions(const BasicBlock &BB)
{
  Instructions.clear();

  for (const Instruction &Inst : BB)
    processInstruction(&Inst);
}

void CILWriter::printInstructions() const
{
  for (const cil::Instruction *Inst : Instructions)
    *Out << "  " << *Inst << "\n";
}

////////////////////////////////////////
void CILWriter::printDataItem(const Constant *Init)
{
  if (const ConstantInt *_Init = dyn_cast<ConstantInt>(Init)) {
    switch (_Init->getBitWidth()) {
    case 8:
      *Out << "int8(";
      break;
    case 16:
      *Out << "int16(";
      break;
    case 32:
      *Out << "int32(";
      break;
    case 64:
      *Out << "int64(";
      break;
    default:
      llvm_unreachable("Unsupported Integer width");
    }
    *Out << _Init->getValue() << ")";
  }
  else if (const ConstantFP *_Init = dyn_cast<ConstantFP>(Init)) {
    if (_Init->getType()->isFloatTy())
      *Out << "float32(" << _Init->getValueAPF().convertToFloat();
    else if (_Init->getType()->isDoubleTy())
      *Out << "float64(" << _Init->getValueAPF().convertToDouble();
    else
      llvm_unreachable("Unsupported Type");
    *Out << ")";
  }
  else
    llvm_unreachable("Unsupported Type");
}

void CILWriter::printDataBody(const Constant *Init)
{
  if (const ConstantDataArray *_Init = dyn_cast<ConstantDataArray>(Init)) {
    unsigned N = _Init->getNumElements();
    if (N > 0) {
      *Out << "\n  { ";
      printDataItem(_Init->getElementAsConstant(0));
      for (unsigned I = 1; I < N; ++I) {
        *Out << "\n  , ";
        printDataItem(_Init->getElementAsConstant(I));
      }
      *Out << "\n  }\n";
    }
    else
      *Out << " { }\n";
  }
  else {
    *Out << " ";
    printDataItem(Init);
    *Out << "\n";
  }
}

void CILWriter::printVariableDeclaration(GlobalVariable &Var)
{
  *Out << ".field static public "
       << *convertType(Var.getType()->getElementType()) << " "
       << Var.getName() << "\n\n";
}

void CILWriter::printVariable(GlobalVariable &Var)
{
  Type *Ty = Var.getType()->getElementType();
  const cil::Type *_Ty;
  std::string DataName = Var.getName().str() + "$data";

  if (dyn_cast<CompositeType>(Ty)) {
    DataLayout &DL = getAnalysis<DataLayout>();
    std::string *TypeName = new std::string(Var.getName().str() + "$type");
    *Out << ".class public explicit sealed value " << *TypeName
         << " { .size " << DL.getTypeAllocSize(Ty) << " }\n";
    _Ty = cil::ValueType::get(StringRef(*TypeName));
  }
  else
    _Ty = convertType(Ty);
  Globals.insert(std::make_pair(&Var, _Ty));

  *Out << ".field static public "
       << *_Ty << " " << Var.getName() << " at " << DataName << "\n";

  *Out << ".data " << DataName << " =";
  printDataBody(Var.getInitializer());
  *Out << "\n";
}

void CILWriter::printFunctionDeclaration(Function &Fun)
{
  if (Fun.isIntrinsic())
    return;

  const char *PInvokeAttrs;
  switch (Fun.getCallingConv()) {
  case CallingConv::CIL_Static:
    return;
  case CallingConv::C:
    PInvokeAttrs = " cdecl";
    break;
  case CallingConv::Fast:
    PInvokeAttrs = " fastcall";
    break;
  default:
    llvm_unreachable("Unsupported CC in function declaration");
  }
  const cil::Type *RetTy = convertType(Fun.getReturnType());

  *Out << ".method static "
       << (Fun.hasLocalLinkage() ? "private " : "public ")
       << "pinvokeimpl(\"msvcrt.dll\"" << PInvokeAttrs << ") "
       << (Fun.isVarArg() ? "vararg " : "")
       << *RetTy << " " << Fun.getName() << "(";

  for (auto B = Fun.arg_begin(), E = Fun.arg_end(), I = B; I != E; ++I) {
    const Argument &Arg = *I;
    const cil::Type *Ty = convertType(Arg.getType());
    *Out << (I == B ? "" : ", ") << *Ty;
  }
  *Out << ")\n{ }\n\n";

}

void CILWriter::printFunction(Function &Fun)
{
  if (Fun.hasAvailableExternallyLinkage())
    return;

  makeLocals(Fun);
  makeNames(Fun);

  {
    const cil::Type *RetTy = convertType(Fun.getReturnType());

    *Out << ".method static "
         << (Fun.hasLocalLinkage() ? "private " : "public ")
         << (Fun.isVarArg() ? "vararg " : "")
         << *RetTy << " " << Fun.getName() << "(";
  }

  for (auto B = Fun.arg_begin(), E = Fun.arg_end(), I = B; I != E; ++I) {
    const Argument &Arg = *I;
    const cil::Type *Ty = convertType(Arg.getType());
    StringRef Name = Arg.getName();
    *Out << (I == B ? "" : ", ") << *Ty << ' ' << Name;
  }
  *Out << ")\n{\n";

  printLocals();

  for (const BasicBlock &BB : Fun) {
    makeInstructions(BB);
    *Out << BB.getName() << ":\n";
    printInstructions();
  }
  *Out << "}\n\n";
}

void CILWriter::printModule(Module &Mod)
{
  *Out << ".assembly extern mscorlib { }\n"
       << ".assembly CIL { }\n\n";

  for (auto I = Mod.global_begin(), E = Mod.global_end(); I != E; ++I) {
    GlobalVariable &Var = *I;
    if (Var.isDeclaration())
      printVariableDeclaration(Var);
    else
      printVariable(Var);
  }

  for (Function &Fun : Mod)
    if (Fun.isDeclaration())
      printFunctionDeclaration(Fun);
    else
      printFunction(Fun);


  *Out << ".method static public int32 $CIL_Entry()\n"
       << "{\n"
       << "  .entrypoint\n"
       << "  call int32 main()\n"
       << "  ret\n"
       << "}\n";
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

  PM.add(new CILWriter(Out));

  return false;
}
