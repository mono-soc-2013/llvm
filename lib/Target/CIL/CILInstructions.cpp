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

raw_ostream &operator<<(raw_ostream &O, cil::Instruction &Inst)
{
  Inst.print(O);
  return O;
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
cil::Ret::Ret()
{
  Opcode = 0x2A;
  OpcodeName = "ret";
  Print = [&](raw_ostream &O) { O << OpcodeName; };
}
