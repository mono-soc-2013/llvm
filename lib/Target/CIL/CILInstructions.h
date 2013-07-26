//===-- CILInstructions.h - Instructions for the CIL --------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===------------------------------------------------------------------------===//
//
//===------------------------------------------------------------------------===//
#ifndef CILINSTRUCTIONS_H
#define CILINSTRUCTIONS_H

#include "llvm/Support/DataTypes.h"
#include "llvm/Support/raw_ostream.h"
#include <functional>

namespace llvm {
namespace cil {

class Instruction
{
protected:
  unsigned Opcode;
  const char *OpcodeName;

  std::function<void(raw_ostream &)> Print;

public:
  unsigned getOpcode() const;
  StringRef getOpcodeName() const;

  void print(raw_ostream &O) const;
};

////////////////////////////////////////////////////////////////////////////////
class Alloca : public Instruction
{
public:
  Alloca();
};

////////////////////////////////////////////////////////////////////////////////
class LoadConst_Int32 : public Instruction
{
  int32_t X;

public:
  LoadConst_Int32(int32_t X);
};

////////////////////////////////////////////////////////////////////////////////
class LoadConst_Int64 : public Instruction
{
  int64_t X;

public:
  LoadConst_Int64(int64_t X);
};

////////////////////////////////////////////////////////////////////////////////
class LoadConst_Float32 : public Instruction
{
  float X;

public:
  LoadConst_Float32(float X);
};

////////////////////////////////////////////////////////////////////////////////
class LoadConst_Float64 : public Instruction
{
  double X;

public:
  LoadConst_Float64(double X);
};

////////////////////////////////////////////////////////////////////////////////
class LoadArg : public Instruction
{
  uint16_t Index;

public:
  LoadArg(uint16_t Index);
};

////////////////////////////////////////////////////////////////////////////////
class LoadLocal : public Instruction
{
  uint16_t Index;

public:
  LoadLocal(uint16_t Index);
};

////////////////////////////////////////////////////////////////////////////////
class StoreLocal : public Instruction
{
  uint16_t Index;

public:
  StoreLocal(uint16_t Index);
};

////////////////////////////////////////////////////////////////////////////////
class Add : public Instruction
{
public:
  Add(bool Overflow = false, bool Unsigned = false);
};

////////////////////////////////////////////////////////////////////////////////
class Sub : public Instruction
{
public:
  Sub(bool Overflow = false, bool Unsigned = false);
};

////////////////////////////////////////////////////////////////////////////////
class Mul : public Instruction
{
public:
  Mul(bool Overflow = false, bool Unsigned = false);
};

////////////////////////////////////////////////////////////////////////////////
class Div : public Instruction
{
public:
  Div(bool Unsigned = false);
};

////////////////////////////////////////////////////////////////////////////////
class Rem : public Instruction
{
public:
  Rem(bool Unsigned = false);
};

////////////////////////////////////////////////////////////////////////////////
class Ret : public Instruction
{
public:
  Ret();
};

}

raw_ostream &operator<<(raw_ostream &O, cil::Instruction &Inst);

}

#endif
