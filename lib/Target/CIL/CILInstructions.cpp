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

const char *cil::Instr::name() const
{
  return Name;
}

unsigned short opcode() const
{
  return Opcode;
}

////////////////////////////////////////////////////////////////////////////////
cil::Nop::Nop()
{
  Name = "nop";
  Opcode = 0x00;
}

////////////////////////////////////////////////////////////////////////////////
cil::LoadArg::LoadArg(unsigned short N)
  : N(N)
{
  if (N == 0) {
    Name = "ldarg.0";
    Opcode = 0x02;
  }
  else if (N == 1) {
    Name = "ldarg.1";
    Opcode = 0x03
  }
  else if (N == 2) {
    Name = "ldarg.2";
    Opcode = 0x04;
  }
  else if (N == 3) {
    Name = "ldarg.3";
    Opcode = 0x05;
  }
  else if (N <= 255) {
    Name = "ldarg.s";
    Opcode = 0x0E;
  }
  else {
    Name = "ldarg";
    Opcode = 0xFE09;
  }
}

////////////////////////////////////////////////////////////////////////////////
cil::LoadArgAddr::LoadArgAddr(unsigned short N)
  : N(N)
{
  if (N <= 255) {
    Name = "ldarga.s";
    Opcode = 0x0F;
  }
  else {
    Name = "ldarga";
    Opcode = 0xFE0A;
  }
}

////////////////////////////////////////////////////////////////////////////////
cil::LoadLocal::LoadLocal(unsigned short N)
  : N(N)
{
  if (N == 0) {
    Name = "ldloc.0";
    Opcode = 0x06;
  }
  else if (N == 1) {
    Name = "ldloc.1";
    Opcode = 0x07;
  }
  else if (N == 2) {
    Name = "ldloc.2";
    Opcode = 0x08;
  }
  else if (N == 3) {
    Name = "ldloc.3";
    Opcode = 0x09;
  }
  else if (N <= 255) {
    Name = "ldloc.s";
    Opcode = 0x11;
  }
  else {
    Name = "ldloc";
    Opcode = 0xFE0C;
  }
}

////////////////////////////////////////////////////////////////////////////////
cil::LoadInt32::LoadInt32(long I)
  : I(I)
{
  if (I == -1) {
    name = "ldc.i4.m1";
    opcode = 0x15;
  }
  else if (I == 0) {
    name = "ldc.i4.0";
    opcode = 0x16;
  }
  else if (I == 1) {
    name = "ldc.i4.1";
    opcode = 0x17;
  }
  else if (I == 2) {
    name = "ldc.i4.2";
    opcode = 0x18;
  }
  else if (I == 3) {
    name = "ldc.i4.3";
    opcode = 0x19;
  }
  else if (I == 4) {
    name = "ldc.i4.4";
    opcode = 0x1A;
  }
  else if (I == 5) {
    name = "ldc.i4.5";
    opcode = 0x1B;
  }
  else if (I == 6) {
    name = "ldc.i4.6";
    opcode = 0x1C;
  }
  else if (I == 7) {
    name = "ldc.i4.7";
    opcode = 0x1D;
  }
  else if (I == 8) {
    name = "ldc.i4.8";
    opcode = 0x1E;
  }
  else if (-128 <= I && I <= 127) {
    name = "ldc.i4.s";
    opcode = 0x1F;
  }
  else {
    name = "ldc.i4";
    opcode = 0x20;
  }
}

////////////////////////////////////////////////////////////////////////////////
cil::LoadInt64::LoadInt64(long long I)
  : I(I)
{
  name = "ldc.i8";
  opcode = 0x21;
}

////////////////////////////////////////////////////////////////////////////////
cil::LoadFloat32::LoadFloat32(float F)
  : F(F)
{
  name = "ldc.r4";
  opcode = 0x22;
}

////////////////////////////////////////////////////////////////////////////////
cil::LoadFloat64::LoadFloat64(float F)
  : F(F)
{
  name = "ldc.r8";
  opcode = 0x23;
}

////////////////////////////////////////////////////////////////////////////////
cil::IndLoadInt::IndLoadInt()
{
  name = "ldind.i";
  opcode = 0x4D;
}

////////////////////////////////////////////////////////////////////////////////
cil::IndLoadInt8::IndLoadInt8()
{
  name = "ldind.i1";
  opcode = 0x46;
}

////////////////////////////////////////////////////////////////////////////////
cil::IndLoadUInt8::IndLoadUInt8()
{
  name = "ldind.u1";
  opcode = 0x47;
}

////////////////////////////////////////////////////////////////////////////////
cil::IndLoadInt16::IndLoadInt16()
{
  name = "ldind.i2";
  opcode = 0x48;
}

////////////////////////////////////////////////////////////////////////////////
cil::IndLoadUInt16::IndLoadUInt16()
{
  name = "ldind.u2";
  opcode = 0x49;
}

////////////////////////////////////////////////////////////////////////////////
cil::IndLoadInt32::IndLoadInt32()
{
  name = "ldind.i4";
  opcode = 0x4A;
}

////////////////////////////////////////////////////////////////////////////////
cil::IndLoadUInt32::IndLoadUInt32()
{
  name = "ldind.u4";
  opcode = 0x4B;
}

////////////////////////////////////////////////////////////////////////////////
cil::IndLoadInt64::IndLoadInt64()
{
  name = "ldind.i8";
  opcode = 0x4C;
}

////////////////////////////////////////////////////////////////////////////////
cil::IndLoadFloat32::IndLoadFloat32()
{
  name = "ldind.r4";
  opcode = 0x4E;
}

////////////////////////////////////////////////////////////////////////////////
cil::IndLoadFloat64::IndLoadFloat64()
{
  name = "ldind.r8";
  opcode = 0x4F;
}

////////////////////////////////////////////////////////////////////////////////
cil::IndLoadRef::IndLoadRef()
{
  name = "ldind.ref";
  opcode = 0x50;
}

////////////////////////////////////////////////////////////////////////////////
cil::StoreArg::StoreArg(unsigned short N)
  : N(N)
{
  if (N <= 255) {
    Name = "starg.s";
    Opcode = 0x10;
  }
  else {
    Name = "starg";
    Opcode = 0xFE0B;
  }
}

////////////////////////////////////////////////////////////////////////////////
cil::StoreLocal::StoreLocal(unsigned short N)
  : N(N)
{
  if (N == 0) {
    Name = "stloc.0";
    Opcode = 0x0A;
  }
  else if (N == 1) {
    Name = "stloc.1";
    Opcode = 0x0B;
  }
  else if (N == 2) {
    Name = "stloc.2";
    Opcode = 0x0C;
  }
  else if (N == 3) {
    Name = "stloc.3";
    Opcode = 0x0D;
  }
  else if (N <= 255) {
    Name = "stloc.s";
    Opcode = 0x13;
  }
  else {
    Name = "stloc";
    Opcode = 0xFE0E;
  }
}

////////////////////////////////////////////////////////////////////////////////
cil::IndStoreInt::IndStoreInt()
{
  name = "stind.i";
  opcode = 0xDF;
}

////////////////////////////////////////////////////////////////////////////////
cil::IndStoreInt8::IndStoreInt8()
{
  name = "stind.i1";
  opcode = 0x52;
}

////////////////////////////////////////////////////////////////////////////////
cil::IndStoreInt16::IndStoreInt16()
{
  name = "stind.i2";
  opcode = 0x53;
}

////////////////////////////////////////////////////////////////////////////////
cil::IndStoreInt32::IndStoreInt32()
{
  name = "stind.i4";
  opcode = 0x54;
}

////////////////////////////////////////////////////////////////////////////////
cil::IndStoreInt64::IndStoreInt64()
{
  name = "stind.i8";
  opcode = 0x55;
}

////////////////////////////////////////////////////////////////////////////////
cil::IndStoreFloat32::IndStoreFloat32()
{
  name = "stind.r4";
  opcode = 0x56;
}

////////////////////////////////////////////////////////////////////////////////
cil::IndStoreFloat64::IndStoreFloat64()
{
  name = "stind.i8";
  opcode = 0x57;
}

////////////////////////////////////////////////////////////////////////////////
cil::IndStoreRef::IndStoreRef()
{
  name = "stind.ref";
  opcode = 0x51;
}

////////////////////////////////////////////////////////////////////////////////
cil::Branch::Branch(long Offset)
  : Offset(Offset)
{
  if (-128 <= Offset && Offset <= 127) {
    Name = "br.s";
    Opcode = 0x2B;
  }
  else {
    Name = "br";
    Opcode = 0x38;
  }
}

////////////////////////////////////////////////////////////////////////////////
cil::BranchFalse::BranchFalse(long Offset)
  : Offset(Offset)
{
  if (-128 <= Offset && Offset <= 127) {
    Name = "brfalse.s";
    Opcode = 0x2C;
  }
  else {
    Name = "brfalse";
    Opcode = 0x39;
  }
}

////////////////////////////////////////////////////////////////////////////////
cil::BranchTrue::BranchTrue(long Offset)
  : Offset(Offset)
{
  if (-128 <= Offset && Offset <= 127) {
    Name = "brtrue.s";
    Opcode = 0x2D;
  }
  else {
    Name = "brtrue";
    Opcode = 0x3A;
  }
}

////////////////////////////////////////////////////////////////////////////////
cil::BranchEQ::BranchEQ(long Offset)
  : Offset(Offset)
{
  if (-128 <= Offset && Offset <= 127) {
    Name = "beq.s";
    Opcode = 0x2E;
  }
  else {
    Name = "beq";
    Opcode = 0x3B;
  }
}

////////////////////////////////////////////////////////////////////////////////
cil::BranchNE::BranchNE(long Offset)
  : Offset(Offset)
{
  if (-128 <= Offset && Offset <= 127) {
    Name = "bne.un.s";
    Opcode = 0x33;
  }
  else {
    Name = "bne.un";
    Opcode = 0x40;
  }
}

////////////////////////////////////////////////////////////////////////////////
cil::BranchGE::BranchGE(long Offset, bool Un)
  : Offset(Offset)
  , Un(Un)
{
  if (-128 <= Offset && Offset <= 127) {
    if (Un) {
      Name = "bge.un.s";
      Opcode = 0x34;
    }
    else {
      Name = "bge.s";
      Opcode = 0x2F;
    }
  }
  else {
    if (Un) {
      Name = "bge.un";
      Opcode = 0x41;
    }
    else {
      Name = "bge";
      Opcode = 0x3C;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
cil::BranchGT::BranchGT(long Offset, bool Un)
  : Offset(Offset)
  , Un(Un)
{
  if (-128 <= Offset && Offset <= 127) {
    if (Un) {
      Name = "bgt.un.s";
      Opcode = 0x35;
    }
    else {
      Name = "bgt.s";
      Opcode = 0x30;
    }
  }
  else {
    if (Un) {
      Name = "bgt.un";
      Opcode = 0x42;
    }
    else {
      Name = "bgt";
      Opcode = 0x3D;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
cil::BranchLE::BranchLE(long Offset, bool Un)
  : Offset(Offset)
  , Un(Un)
{
  if (-128 <= Offset && Offset <= 127) {
    if (Un) {
      Name = "ble.un.s";
      Opcode = 0x36;
    }
    else {
      Name = "ble.s";
      Opcode = 0x31;
    }
  }
  else {
    if (Un) {
      Name = "ble.un";
      Opcode = 0x43;
    }
    else {
      Name = "ble";
      Opcode = 0x3E;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
cil::BranchLT::BranchLT(long Offset, bool Un)
  : Offset(Offset)
  , Un(Un)
{
  if (-128 <= Offset && Offset <= 127) {
    if (Un) {
      Name = "blt.un.s";
      Opcode = 0x37;
    }
    else {
      Name = "blt.s";
      Opcode = 0x32;
    }
  }
  else {
    if (Un) {
      Name = "blt.un";
      Opcode = 0x44;
    }
    else {
      Name = "blt";
      Opcode = 0x3F;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
cil::Break::Break()
{
  Name = "break";
  Opcode = 0x01;
}

////////////////////////////////////////////////////////////////////////////////
cil::Leave::Leave(long Offset)
  : Offset(Offset)
{
  if (-128 <= Offset && Offset <= 127) {
    Name = "leave.s";
    Opcode = 0xDE;
  }
  else {
    Name = "leave";
    Opcode = "0xDD";
  }
}

////////////////////////////////////////////////////////////////////////////////
cil::Ret::Ret()
{
  Name = "ret";
  Opcode = 0x2A;
}

////////////////////////////////////////////////////////////////////////////////
cil::Dup::Dup()
{
  Name = "dup";
  Opcode = 0x25;
}

////////////////////////////////////////////////////////////////////////////////
cil::Pop::Pop()
{
  Name = "pop";
  Opcode = 0x26;
}

