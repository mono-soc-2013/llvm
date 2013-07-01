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

#include "llvm/Support/FormattedStream.h"

namespace llvm
{

namespace cil
{

class Instr
{
protected:
  char *Name;
  unsigned short Opcode;

public:
  const char *name() const;
  unsigned short opcode() const;

  virtual void print(formatted_raw_ostream &Stream) const = 0;
};

////////////////////////////////////////////////////////////////////////////////
class Nop : public Instr
{
public:
  Nop();

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class LoadArg : public Instr
{
  unsigned short N;

public:
  LoadArg(unsigned short N);

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class LoadArgAddr : public Instr
{
  unsigned short N;

public:
  LoadArgAddr(unsigned short N);

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class LoadLocal : public Instr
{
  unsigned short N;

public:
  LoadLocal(unsigned short N);

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class LoadInt32 : public Instr
{
  long I;

public:
  LoadInt32(long I);

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class LoadInt64 : public Instr
{
  long long I;

public:
  LoadInt64(long long I);

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class LoadFloat32 : public Instr
{
  float F;

public:
  LoadFloat32(float I);

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class LoadFloat64 : public Instr
{
  double F;

public:
  LoadFloat64(double F);

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class IndLoadInt : public Instr
{
public:
  IndLoadInt();

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class IndLoadInt8 : public Instr
{
public:
  IndLoadInt8();

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class IndLoadUInt8 : public Instr
{
public:
  IndLoadUInt8();

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class IndLoadInt16 : public Instr
{
public:
  IndLoadInt16();

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class IndLoadUInt16 : public Instr
{
public:
  IndLoadUInt16();

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class IndLoadInt32 : public Instr
{
public:
  IndLoadInt32();

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class IndLoadUInt32 : public Instr
{
public:
  IndLoadUInt32();

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class IndLoadInt64 : public Instr
{
public:
  IndLoadInt64();

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class IndLoadFloat32 : public Instr
{
public:
  IndLoadFloat32();

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class IndLoadFloat64 : public Instr
{
public:
  IndLoadFloat64();

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class IndLoadRef : public Instr
{
public:
  IndLoadRef();

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class StoreArg : public Instr
{
  unsigned short N;

public:
  StoreArg(unsigned short N);

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class StoreLocal : public Instr
{
  unsigned short N;

public:
  StoreLocal(unsigned short N);

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class IndStoreInt : public Instr
{
public:
  IndStoreInt();

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class IndStoreInt8 : public Instr
{
public:
  IndStoreInt8();

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class IndStoreInt16 : public Instr
{
public:
  IndStoreInt16();

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class IndStoreInt32 : public Instr
{
public:
  IndStoreInt32();

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class IndStoreInt64 : public Instr
{
public:
  IndStoreInt64();

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class IndStoreFloat32 : public Instr
{
public:
  IndStoreFloat32();

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class IndStoreFloat64 : public Instr
{
public:
  IndStoreFloat64();

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class IndStoreRef : public Instr
{
public:
  IndStoreRef();

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class Branch : public Instr
{
  long Offset;

public:
  Branch(long Offset);

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class BranchFalse : public Instr
{
  long Offset;

public:
  BranchFalse(long Offset);

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class BranchTrue : public Instr
{
  long Offset;

public:
  BranchTrue(long Offset);

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class BranchEQ : public Instr
{
  long Offset;

public:
  BranchEQ(long Offset);

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class BranchNE : public Instr
{
  long Offset;

public:
  BranchNE(long Offset);

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class BranchGE : public Instr
{
  long Offset;
  bool Un;

public:
  BranchGE(long Offset, bool Un = false);

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class BranchGT : public Instr
{
  long Offset;
  bool Un;

public:
  BranchGT(long Offset, bool Un = false);

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class BranchLE : public Instr
{
  long Offset;
  bool Un;

public:
  BranchLE(long Offset, bool Un = false);

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class BranchLT : public Instr
{
  long Offset;
  bool Un;

public:
  BranchLT(long Offset, bool Un = false);

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class Break : public Instr
{
public:
  Break();

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class Leave : public Instr
{
public:
  Leave(long Offset);

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class Ret : public Instr
{
public:
  Ret();

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class Dup : public Instr
{
public:
  Dup();

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class Pop : public Instr
{
public:
  Pop();

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class Add : public Instr
{
public:
  Add(bool Ovf = false, bool Un = false);

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class Sub : public Instr
{
public:
  Sub(bool Ovf = false, bool Un = false);

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class Mul : public Instr
{
public:
  Mul(bool Ovf = false, bool Un = false);

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class Div : public Instr
{
public:
  Div(bool Un = false);

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class Rem : public Instr
{
public:
  Rem(bool Un = false);

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class Neg : public Instr
{
public:
  Neg();

  void print(formatted_raw_ostream &Stream) const;
};

////////////////////////////////////////////////////////////////////////////////
class Add : public Instr
{
public:
  Add();

  void print(formatted_raw_ostream &Stream) const;
};

}

}

#endif
