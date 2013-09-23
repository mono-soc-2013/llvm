//===-- CILInstructions.cpp - Instructions for the CIL --------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===--------------------------------------------------------------------------===//
//
//===--------------------------------------------------------------------------===//

#include "CILInstructions.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

unsigned cil::Instruction::getOpcode() const
{
  return Opcode;
}

StringRef cil::Instruction::getOpcodeName() const
{
  return StringRef(OpcodeName);
}

void cil::Instruction::print(raw_ostream &O) const
{
  Print(O);
}

////////////////////////////////////////////////////////////////////////////////
cil::Alloca::Alloca()
{
  Opcode = 0xFE0F;
  OpcodeName = "localloc";
  Print = [&](raw_ostream &O) { O << OpcodeName; };
}

////////////////////////////////////////////////////////////////////////////////
cil::LoadConst_Int32::LoadConst_Int32(int32_t X)
  : X(X)
{
  if (X == -1) {
    Opcode = 0x15;
    OpcodeName = "ldc.i4.m1";
    Print = [&](raw_ostream &O) { O << OpcodeName; };
  }
  else if (X == 0) {
    Opcode = 0x16;
    OpcodeName = "ldc.i4.0";
    Print = [&](raw_ostream &O) { O << OpcodeName; };
  }
  else if (X == 1) {
    Opcode = 0x17;
    OpcodeName = "ldc.i4.1";
    Print = [&](raw_ostream &O) { O << OpcodeName; };
  }
  else if (X == 2) {
    Opcode = 0x18;
    OpcodeName = "ldc.i4.2";
    Print = [&](raw_ostream &O) { O << OpcodeName; };
  }
  else if (X == 3) {
    Opcode = 0x19;
    OpcodeName = "ldc.i4.3";
    Print = [&](raw_ostream &O) { O << OpcodeName; };
  }
  else if (X == 4) {
    Opcode = 0x1A;
    OpcodeName = "ldc.i4.4";
    Print = [&](raw_ostream &O) { O << OpcodeName; };
  }
  else if (X == 5) {
    Opcode = 0x1B;
    OpcodeName = "ldc.i4.5";
    Print = [&](raw_ostream &O) { O << OpcodeName; };
  }
  else if (X == 6) {
    Opcode = 0x1C;
    OpcodeName = "ldc.i4.6";
    Print = [&](raw_ostream &O) { O << OpcodeName; };
  }
  else if (X == 7) {
    Opcode = 0x1D;
    OpcodeName = "ldc.i4.7";
    Print = [&](raw_ostream &O) { O << OpcodeName; };
  }
  else if (X == 8) {
    Opcode = 0x1E;
    OpcodeName = "ldc.i4.8";
    Print = [&](raw_ostream &O) { O << OpcodeName; };
  }
  else if (-128 <= X && X < 128) {
    Opcode = 0x1F;
    OpcodeName = "ldc.i4.s";
    Print = [&, X](raw_ostream &O) { O << OpcodeName << " " << X; };
  }
  else {
    Opcode = 0x20;
    OpcodeName = "ldc.i4";
    Print = [&, X](raw_ostream &O) { O << OpcodeName << " " << X; };
  }
}

////////////////////////////////////////////////////////////////////////////////
cil::LoadConst_Int64::LoadConst_Int64(int64_t X)
  : X(X)
{
  Opcode = 0x21;
  OpcodeName = "ldc.i8";
  Print = [&, X](raw_ostream &O) { O << OpcodeName << " " << X; };
}

////////////////////////////////////////////////////////////////////////////////
cil::LoadConst_Float32::LoadConst_Float32(float X)
  : X(X)
{
  Opcode = 0x22;
  OpcodeName = "ldc.r4";
  Print = [&, X](raw_ostream &O) { O << OpcodeName << " " << X; };
}

////////////////////////////////////////////////////////////////////////////////
cil::LoadConst_Float64::LoadConst_Float64(double X)
  : X(X)
{
  Opcode = 0x23;
  OpcodeName = "ldc.r8";
  Print = [&, X](raw_ostream &O) { O << OpcodeName << " " << X; };
}

////////////////////////////////////////////////////////////////////////////////
cil::LoadArg::LoadArg(uint16_t Index)
  : Index(Index)
{
  if (Index == 0) {
    Opcode = 0x02;
    OpcodeName = "ldarg.0";
    Print = [&](raw_ostream &O) { O << OpcodeName; };
  }
  else if (Index == 1) {
    Opcode = 0x03;
    OpcodeName = "ldarg.1";
    Print = [&](raw_ostream &O) { O << OpcodeName; };
  }
  else if (Index == 2) {
    Opcode = 0x04;
    OpcodeName = "ldarg.2";
    Print = [&](raw_ostream &O) { O << OpcodeName; };
  }
  else if (Index == 3) {
    Opcode = 0x05;
    OpcodeName = "ldarg.3";
    Print = [&](raw_ostream &O) { O << OpcodeName; };
  }
  else if (Index < 256) {
    Opcode = 0x0E;
    OpcodeName = "ldarg.s";
    Print = [&, Index](raw_ostream &O) { O << OpcodeName << " " << Index; };
  }
  else {
    Opcode = 0xFE09;
    OpcodeName = "ldarg";
    Print = [&, Index](raw_ostream &O) { O << OpcodeName << " " << Index; };
  }
}

////////////////////////////////////////////////////////////////////////////////
cil::LoadArgAddr::LoadArgAddr(uint16_t Index)
  : Index(Index)
{
  if (Index < 256) {
    Opcode = 0x0F;
    OpcodeName = "ldarga.s";
    Print = [&, Index](raw_ostream &O) { O << OpcodeName << " " << Index; };
  }
  else {
    Opcode = 0xFE0A;
    OpcodeName = "ldarga";
    Print = [&, Index](raw_ostream &O) { O << OpcodeName << " " << Index; };
  }
}

////////////////////////////////////////////////////////////////////////////////
cil::LoadLocal::LoadLocal(uint16_t Index)
  : Index(Index)
{
  if (Index == 0) {
    Opcode = 0x06;
    OpcodeName = "ldloc.0";
    Print = [&](raw_ostream &O) { O << OpcodeName; };
  }
  else if (Index == 1) {
    Opcode = 0x07;
    OpcodeName = "ldloc.1";
    Print = [&](raw_ostream &O) { O << OpcodeName; };
  }
  else if (Index == 2) {
    Opcode = 0x08;
    OpcodeName = "ldloc.2";
    Print = [&](raw_ostream &O) { O << OpcodeName; };
  }
  else if (Index == 3) {
    Opcode = 0x09;
    OpcodeName = "ldloc.3";
    Print = [&](raw_ostream &O) { O << OpcodeName; };
  }
  else if (Index < 256) {
    Opcode = 0x11;
    OpcodeName = "ldloc.s";
    Print = [&, Index](raw_ostream &O) { O << OpcodeName << " " << Index; };
  }
  else {
    Opcode = 0xFE0C;
    OpcodeName = "ldloc";
    Print = [&, Index](raw_ostream &O) { O << OpcodeName << " " << Index; };
  }
}

////////////////////////////////////////////////////////////////////////////////
cil::LoadLocalAddr::LoadLocalAddr(uint16_t Index)
  : Index(Index)
{
  if (Index < 256) {
    Opcode = 0x12;
    OpcodeName = "ldloca.s";
    Print = [&, Index](raw_ostream &O) { O << OpcodeName << " " << Index; };
  }
  else {
    Opcode = 0xFE0D;
    OpcodeName = "ldloca";
    Print = [&, Index](raw_ostream &O) { O << OpcodeName << " " << Index; };
  }
}

////////////////////////////////////////////////////////////////////////////////
cil::LoadStaticField::LoadStaticField(const Type *Ty, StringRef Name)
{
  Opcode = 0x7E;
  OpcodeName = "ldsfld";
  Print = [=](raw_ostream &O) {
    O << OpcodeName << " " << *Ty << " " << Name;
  };
}

////////////////////////////////////////////////////////////////////////////////
cil::LoadStaticFieldAddr::LoadStaticFieldAddr(const Type *Ty, StringRef Name)
{
  Opcode = 0x7F;
  OpcodeName = "ldsflda";
  Print = [=](raw_ostream &O) {
    O << OpcodeName << " " << *Ty << " " << Name;
  };
}

////////////////////////////////////////////////////////////////////////////////
cil::LoadFunction::LoadFunction(const FunctionType *Ty, StringRef Name)
{
  Opcode = 0xFE06;
  OpcodeName = "ldftn";
  Print = [=](raw_ostream &O) {
    O << OpcodeName << " " << *(Ty->getReturnType()) << " " << Name << "(";
    unsigned Max = Ty->getNumParams();
    if (0 < Max) {
      --Max;
      for (unsigned I = 0; I < Max; ++I)
        O << *(Ty->getParamType(I)) << ", ";
      O << *(Ty->getParamType(Max));
    }
    O << ")";
  };
}

////////////////////////////////////////////////////////////////////////////////
cil::LoadIndirect::LoadIndirect(TypeID TyID)
  : TyID(TyID)
{
  switch (TyID) {
  case TypeID::Int8:
    Opcode = 0x46;
    OpcodeName = "ldind.i1";
    break;
  case TypeID::UInt8:
    Opcode = 0x47;
    OpcodeName = "ldind.u1";
    break;
  case TypeID::Int16:
    Opcode = 0x48;
    OpcodeName = "ldind.i2";
    break;
  case TypeID::UInt16:
    Opcode = 0x49;
    OpcodeName = "ldind.u2";
    break;
  case TypeID::Int32:
    Opcode = 0x4A;
    OpcodeName = "ldind.i4";
    break;
  case TypeID::UInt32:
    Opcode = 0x4B;
    OpcodeName = "ldind.u4";
    break;
  case TypeID::Int64:
  case TypeID::UInt64:
    Opcode = 0x4C;
    OpcodeName = "ldind.i8";
    break;
  case TypeID::Int:
    Opcode = 0x4D;
    OpcodeName = "ldind.i";
    break;
  case TypeID::Float32:
    Opcode = 0x4E;
    OpcodeName = "ldind.r4";
    break;
  case TypeID::Float64:
    Opcode = 0x4F;
    OpcodeName = "ldind.r8";
    break;
  case TypeID::TypedRef:
    Opcode = 0x50;
    OpcodeName = "ldind.ref";
    break;
  default:
    llvm_unreachable("LoadIndirect: invalid type");
  }
  Print = [&](raw_ostream &O) { O << OpcodeName; };
}

////////////////////////////////////////////////////////////////////////////////
cil::StoreLocal::StoreLocal(uint16_t Index)
  : Index(Index)
{
  if (Index == 0) {
    Opcode = 0x0A;
    OpcodeName = "stloc.0";
    Print = [&](raw_ostream &O) { O << OpcodeName; };
  }
  else if (Index == 1) {
    Opcode = 0x0B;
    OpcodeName = "stloc.1";
    Print = [&](raw_ostream &O) { O << OpcodeName; };
  }
  else if (Index == 2) {
    Opcode = 0x0C;
    OpcodeName = "stloc.2";
    Print = [&](raw_ostream &O) { O << OpcodeName; };
  }
  else if (Index == 3) {
    Opcode = 0x0D;
    OpcodeName = "stloc.3";
    Print = [&](raw_ostream &O) { O << OpcodeName; };
  }
  else if (Index < 256) {
    Opcode = 0x13;
    OpcodeName = "stloc.s";
    Print = [&, Index](raw_ostream &O) { O << OpcodeName << " " << Index; };
  }
  else {
    Opcode = 0xFE0E;
    OpcodeName = "stloc";
    Print = [&, Index](raw_ostream &O) { O << OpcodeName << " " << Index; };
  }
}

////////////////////////////////////////////////////////////////////////////////
cil::StoreIndirect::StoreIndirect(TypeID TyID)
  : TyID(TyID)
{
  switch (TyID) {
  case TypeID::Int8:
  case TypeID::UInt8:
    Opcode = 0x52;
    OpcodeName = "stind.i1";
    break;
  case TypeID::Int16:
  case TypeID::UInt16:
    Opcode = 0x53;
    OpcodeName = "stind.i2";
    break;
  case TypeID::Int32:
  case TypeID::UInt32:
    Opcode = 0x54;
    OpcodeName = "stind.i4";
    break;
  case TypeID::Int64:
  case TypeID::UInt64:
    Opcode = 0x55;
    OpcodeName = "stind.i8";
    break;
  case TypeID::Int:
  case TypeID::UInt:
    Opcode = 0xDF;
    OpcodeName = "stind.i";
    break;
  case TypeID::Float32:
    Opcode = 0x56;
    OpcodeName = "stind.r4";
    break;
  case TypeID::Float64:
    Opcode = 0x57;
    OpcodeName = "stind.r8";
    break;
  case TypeID::TypedRef:
    Opcode = 0x51;
    OpcodeName = "stind.ref";
    break;
  default:
    llvm_unreachable("StoreIndirect: invalid type");
  }
  Print = [&](raw_ostream &O) { O << OpcodeName; };
}

////////////////////////////////////////////////////////////////////////////////
cil::Ret::Ret()
{
  Opcode = 0x2A;
  OpcodeName = "ret";
  Print = [&](raw_ostream &O) { O << OpcodeName; };
}

////////////////////////////////////////////////////////////////////////////////
cil::Branch::Branch(StringRef Label)
  : Label(Label)
{
  Opcode = 0x38;
  OpcodeName = "br";
  Print = [&, Label](raw_ostream &O) { O << OpcodeName << " " << Label; };
}

////////////////////////////////////////////////////////////////////////////////
cil::BranchCond::BranchCond(bool Cond, StringRef Label)
  : Label(Label)
{
  if (Cond) {
    Opcode = 0x3A;
    OpcodeName = "brtrue";
  }
  else {
    Opcode = 0x39;
    OpcodeName = "brfalse";
  }
  Print = [&, Label](raw_ostream &O) { O << OpcodeName << " " << Label; };
}

////////////////////////////////////////////////////////////////////////////////
cil::Call::Call(const FunctionType *Ty, const CallConv &CC, StringRef Name)
{
  Opcode = 0x28;
  OpcodeName = "call";
  Print = [=](raw_ostream &O) {
    O << OpcodeName << " "
      << CC << " "
      << *(Ty->getReturnType()) << " "
      << Name << "(";
    unsigned Max = Ty->getNumParams();
    if (0 < Max) {
      --Max;
      for (unsigned I = 0; I < Max; ++I)
        O << *(Ty->getParamType(I)) << ", ";
      O << *(Ty->getParamType(Max));
    }
    O << ")";
  };
}

////////////////////////////////////////////////////////////////////////////////
cil::CallIndirect::CallIndirect(const FunctionType *Ty, const CallConv &CC)
{
  Opcode = 0x29;
  OpcodeName = "calli";
  Print = [=](raw_ostream &O) {
    O << OpcodeName << " "
      << CC << " "
      << *(Ty->getReturnType()) << "(";
    unsigned Max = Ty->getNumParams();
    if (0 < Max) {
      --Max;
      for (unsigned I = 0; I < Max; ++I)
        O << *(Ty->getParamType(I)) << ", ";
      O << *(Ty->getParamType(Max));
    }
    O << ")";
  };
}

////////////////////////////////////////////////////////////////////////////////
cil::Add::Add(bool Overflow, bool Unsigned)
{
  if (Overflow)
    if (Unsigned) {
      Opcode = 0xD7;
      OpcodeName = "add.ovf.un";
    }
    else {
      Opcode = 0xD6;
      OpcodeName = "add.ovf";
    }
  else {
    Opcode = 0x58;
    OpcodeName = "add";
  }
  Print = [&](raw_ostream &O) { O << OpcodeName; };
}

////////////////////////////////////////////////////////////////////////////////
cil::Sub::Sub(bool Overflow, bool Unsigned)
{
  if (Overflow)
    if (Unsigned) {
      Opcode = 0xDB;
      OpcodeName = "sub.ovf.un";
    }
    else {
      Opcode = 0xDA;
      OpcodeName = "sub.ovf";
    }
  else {
    Opcode = 0x59;
    OpcodeName = "sub";
  }
  Print = [&](raw_ostream &O) { O << OpcodeName; };
}

////////////////////////////////////////////////////////////////////////////////
cil::Mul::Mul(bool Overflow, bool Unsigned)
{
  if (Overflow)
    if (Unsigned) {
      Opcode = 0xD9;
      OpcodeName = "mul.ovf.un";
    }
    else {
      Opcode = 0xD8;
      OpcodeName = "mul.ovf";
    }
  else {
    Opcode = 0x5A;
    OpcodeName = "mul";
  }
  Print = [&](raw_ostream &O) { O << OpcodeName; };
}

////////////////////////////////////////////////////////////////////////////////
cil::Div::Div(bool Unsigned)
{
  if (Unsigned) {
    Opcode = 0x5B;
    OpcodeName = "div.un";
  }
  else {
    Opcode = 0x5C;
    OpcodeName = "div";
  }
  Print = [&](raw_ostream &O) { O << OpcodeName; };
}

////////////////////////////////////////////////////////////////////////////////
cil::Rem::Rem(bool Unsigned)
{
  if (Unsigned) {
    Opcode = 0x5D;
    OpcodeName = "rem.un";
  }
  else {
    Opcode = 0x5E;
    OpcodeName = "rem";
  }
  Print = [&](raw_ostream &O) { O << OpcodeName; };
}

////////////////////////////////////////////////////////////////////////////////
cil::And::And()
{
  Opcode = 0x5F;
  OpcodeName = "and";
  Print = [&](raw_ostream &O) { O << OpcodeName; };
}

////////////////////////////////////////////////////////////////////////////////
cil::Or::Or()
{
  Opcode = 0x60;
  OpcodeName = "or";
  Print = [&](raw_ostream &O) { O << OpcodeName; };
}

////////////////////////////////////////////////////////////////////////////////
cil::Xor::Xor()
{
  Opcode = 0x61;
  OpcodeName = "xor";
  Print = [&](raw_ostream &O) { O << OpcodeName; };
}

////////////////////////////////////////////////////////////////////////////////
cil::Cmp::Cmp(Predicate Predicate, bool Un)
{
  switch (Predicate) {
  case EQ:
    Opcode = 0xFE01;
    OpcodeName = "ceq";
    break;
  case GT:
    if (Un) {
      Opcode = 0xFE03;
      OpcodeName = "cgt.un";
    }
    else {
      Opcode = 0xFE02;
      OpcodeName = "cgt";
    }
    break;
  case LT:
    if (Un) {
      Opcode = 0xFE05;
      OpcodeName = "clt.un";
    }
    else {
      Opcode = 0xFE04;
      OpcodeName = "clt";
    }
    break;
  }
  Print = [&](raw_ostream &O) { O << OpcodeName; };
}

////////////////////////////////////////////////////////////////////////////////
cil::Conv::Conv(TypeID TyID, bool Overflow, bool Unsigned)
  : TyID(TyID)
{
  switch (TyID) {
  case TypeID::Int8:
    if (Overflow) {
      if (Unsigned) {
        Opcode = 0x82;
        OpcodeName = "conv.ovf.i1.un";
      }
      else {
        Opcode = 0xB3;
        OpcodeName = "conv.ovf.i1";
      }
    }
    else {
      Opcode = 0x67;
      OpcodeName = "conv.i1";
    }
    break;
  case TypeID::UInt8:
    if (Overflow) {
      if (Unsigned) {
        Opcode = 0x86;
        OpcodeName = "conv.ovf.u1.un";
      }
      else {
        Opcode = 0xB4;
        OpcodeName = "conv.ovf.u1";
      }
    }
    else {
      Opcode = 0xD2;
      OpcodeName = "conv.u1";
    }
    break;
  case TypeID::Int16:
    if (Overflow) {
      if (Unsigned) {
        Opcode = 0x83;
        OpcodeName = "conv.ovf.i2.un";
      }
      else {
        Opcode = 0xB5;
        OpcodeName = "conv.ovf.i2";
      }
    }
    else {
      Opcode = 0x68;
      OpcodeName = "conv.i2";
    }
    break;
  case TypeID::UInt16:
    if (Overflow) {
      if (Unsigned) {
        Opcode = 0x87;
        OpcodeName = "conv.ovf.u2.un";
      }
      else {
        Opcode = 0xB6;
        OpcodeName = "conv.ovf.u2";
      }
    }
    else {
      Opcode = 0xD1;
      OpcodeName = "conv.u2";
    }
    break;
  case TypeID::Int32:
    if (Overflow) {
      if (Unsigned) {
        Opcode = 0x84;
        OpcodeName = "conv.ovf.i4.un";
      }
      else {
        Opcode = 0xB7;
        OpcodeName = "conv.ovf.i4";
      }
    }
    else {
      Opcode = 0x69;
      OpcodeName = "conv.i4";
    }
    break;
  case TypeID::UInt32:
    if (Overflow) {
      if (Unsigned) {
        Opcode = 0x88;
        OpcodeName = "conv.ovf.u4.un";
      }
      else {
        Opcode = 0xB8;
        OpcodeName = "conv.ovf.u4";
      }
    }
    else {
      Opcode = 0x6D;
      OpcodeName = "conv.u4";
    }
    break;
  case TypeID::Int64:
    if (Overflow) {
      if (Unsigned) {
        Opcode = 0x85;
        OpcodeName = "conv.ovf.i8.un";
      }
      else {
        Opcode = 0xB9;
        OpcodeName = "conv.ovf.i8";
      }
    }
    else {
      Opcode = 0x6A;
      OpcodeName = "conv.i8";
    }
    break;
  case TypeID::UInt64:
    if (Overflow) {
      if (Unsigned) {
        Opcode = 0x89;
        OpcodeName = "conv.ovf.u8.un";
      }
      else {
        Opcode = 0xBA;
        OpcodeName = "conv.ovf.u8";
      }
    }
    else {
      Opcode = 0x6E;
      OpcodeName = "conv.u8";
    }
    break;
  case TypeID::Int:
    if (Overflow) {
      if (Unsigned) {
        Opcode = 0x8A;
        OpcodeName = "conv.ovf.i.un";
      }
      else {
        Opcode = 0xD4;
        OpcodeName = "conv.ovf.i";
      }
    }
    else {
      Opcode = 0xD3;
      OpcodeName = "conv.i";
    }
    break;
  case TypeID::UInt:
    if (Overflow) {
      if (Unsigned) {
        Opcode = 0x8B;
        OpcodeName = "conv.ovf.u.un";
      }
      else {
        Opcode = 0xD5;
        OpcodeName = "conv.ovf.u";
      }
    }
    else {
      Opcode = 0xE0;
      OpcodeName = "conv.u";
    }
    break;
  case TypeID::Float32:
    Opcode = 0x6B;
    OpcodeName = "conv.r4";
    break;
  case TypeID::Float64:
    Opcode = 0x6C;
    OpcodeName = "conv.r8";
    break;
  }
  Print = [&](raw_ostream &O) { O << OpcodeName; };
}

////////////////////////////////////////////////////////////////////////////////
cil::Dup::Dup()
{
  Opcode = 0x25;
  OpcodeName = "dup";
  Print = [&](raw_ostream &O) { O << OpcodeName; };
}

////////////////////////////////////////////////////////////////////////////////
cil::Pop::Pop()
{
  Opcode = 0x26;
  OpcodeName = "pop";
  Print = [&](raw_ostream &O) { O << OpcodeName; };
}
