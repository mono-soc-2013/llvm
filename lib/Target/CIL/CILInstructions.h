//===-- CILInstructions.h - Instructions for the CIL ------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//===----------------------------------------------------------------------===//
#ifndef CILINSTRUCTIONS_H
#define CILINSTRUCTIONS_H

#include "CILTypes.h"
#include "CILMetadata.h"
#include "llvm/Support/DataTypes.h"
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
class LoadArgAddr : public Instruction
{
  uint16_t Index;

public:
  LoadArgAddr(uint16_t Index);
};

////////////////////////////////////////////////////////////////////////////////
class LoadLocal : public Instruction
{
  uint16_t Index;

public:
  LoadLocal(uint16_t Index);
};

////////////////////////////////////////////////////////////////////////////////
class LoadLocalAddr : public Instruction
{
  uint16_t Index;

public:
  LoadLocalAddr(uint16_t Index);
};

////////////////////////////////////////////////////////////////////////////////
class LoadStaticField : public Instruction
{
public:
  LoadStaticField(const Type *Ty, StringRef Name);
};

////////////////////////////////////////////////////////////////////////////////
class LoadStaticFieldAddr : public Instruction
{
public:
  LoadStaticFieldAddr(const Type *Ty, StringRef Name);
};

////////////////////////////////////////////////////////////////////////////////
class LoadFunction : public Instruction
{
public:
  LoadFunction(const FunctionType *Ty, StringRef Name);
};

////////////////////////////////////////////////////////////////////////////////
class LoadIndirect : public Instruction
{
  TypeID TyID;

public:
  LoadIndirect(TypeID TyID);
};

////////////////////////////////////////////////////////////////////////////////
class StoreLocal : public Instruction
{
  uint16_t Index;

public:
  StoreLocal(uint16_t Index);
};

////////////////////////////////////////////////////////////////////////////////
class StoreIndirect : public Instruction
{
  TypeID TyID;

public:
  StoreIndirect(TypeID TyID);
};

////////////////////////////////////////////////////////////////////////////////
class Ret : public Instruction
{
public:
  Ret();
};

////////////////////////////////////////////////////////////////////////////////
class Branch : public Instruction
{
  StringRef Label;

public:
  Branch(StringRef Label);
};

////////////////////////////////////////////////////////////////////////////////
class BranchCond : public Instruction
{
  StringRef Label;

public:
  BranchCond(bool Cond, StringRef Label);
};

////////////////////////////////////////////////////////////////////////////////
class Call : public Instruction
{
public:
  Call(const FunctionType *Ty, const CallConv &CC, StringRef Name);
};

////////////////////////////////////////////////////////////////////////////////
class CallIndirect : public Instruction
{
public:
  CallIndirect(const FunctionType *Ty, const CallConv &CC);
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
class And : public Instruction
{
public:
  And();
};

////////////////////////////////////////////////////////////////////////////////
class Or : public Instruction
{
public:
  Or();
};

////////////////////////////////////////////////////////////////////////////////
class Xor : public Instruction
{
public:
  Xor();
};

////////////////////////////////////////////////////////////////////////////////
class Cmp : public Instruction
{
public:
  enum Predicate {
    EQ,
    GT,
    LT
  };

public:
  Cmp(Predicate Predicate, bool Un = false);
};

////////////////////////////////////////////////////////////////////////////////
class Conv : public Instruction
{
  TypeID TyID;

public:
  Conv(TypeID TyID, bool Overflow = false, bool Unsigned = false);
};

////////////////////////////////////////////////////////////////////////////////
class Dup : public Instruction
{
public:
  Dup();
};

////////////////////////////////////////////////////////////////////////////////
class Pop : public Instruction
{
public:
  Pop();
};

}

inline raw_ostream &operator<<(raw_ostream &O, const cil::Instruction &Inst)
{
  Inst.print(O);
  return O;
}

}

#endif
