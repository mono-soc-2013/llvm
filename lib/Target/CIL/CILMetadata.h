//===-- CILMetadata.h - Metadata for the CIL --------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//===----------------------------------------------------------------------===//

#ifndef CILMETADATA_H
#define CILMETADATA_H

#include "CILTypes.h"
#include "llvm/Support/DataTypes.h"
#include "llvm/Support/raw_ostream.h"
#include <tuple>
#include <vector>

namespace llvm {

namespace cil {

enum class CallKind : unsigned short {
  Default = 0,
  Cdecl,
  Fastcall,
  Stdcall,
  Thiscall,
  Vararg
};

enum class CallAttr : unsigned short {
  None = 0,
  Instance,
  InstanceExplicit
};

namespace {

static const unsigned short
  FirstCallKind = static_cast<unsigned short>(CallKind::Default),
  LastCallKind = static_cast<unsigned short>(CallKind::Vararg),
  NumCallKinds = LastCallKind - FirstCallKind;

static const unsigned short
  FirstCallAttr = static_cast<unsigned short>(CallAttr::None),
  LastCallAttr = static_cast<unsigned short>(CallAttr::InstanceExplicit),
  NumCallAttrs = LastCallAttr - FirstCallAttr;

}

class CallConv {
public:
  void print(raw_ostream &O) const;

  static CallConv &get(CallKind Kind = CallKind::Default,
                       CallAttr Attr = CallAttr::None)
  {
    static CallConv *CCs[NumCallKinds][NumCallAttrs] = {{ nullptr }};

    unsigned short KindIndex = static_cast<unsigned short>(Kind);
    unsigned short AttrIndex = static_cast<unsigned short>(Attr);
    CallConv *CCPtr = CCs[KindIndex][AttrIndex];
    if (CCPtr == nullptr)
      CCPtr = new CallConv(Kind, Attr);
    return *CCPtr;
  }

protected:
  CallConv(CallKind Kind, CallAttr Attr)
    : Kind(Kind)
    , Attr(Attr)
  {
  }

private:
  CallKind Kind;
  CallAttr Attr;
};

////////////////////////////////////////////////////////////////////////////////
class MethodRef {
public:
  MethodRef(const CallConv &CC,
            const Type *RetTy,
            StringRef Name,
            const std::vector<const Type *> &Params)
    : CC(CC)
    , RetTy(RetTy)
    , Name(Name)
    , Params(Params)
  {
  }

public:
  const CallConv &getCallConv() const
  {
    return CC;
  }

  const Type *getRetTy() const
  {
    return RetTy;
  }

  StringRef getName() const
  {
    return Name;
  }

  const std::vector<const Type *> &getParams() const
  {
    return Params;
  }

  void print(raw_ostream &O) const;

private:
  const CallConv &CC;
  const Type *RetTy;
  StringRef Name;
  const std::vector<const Type *> Params;
};

}

////////////////////////////////////////////////////////////////////////////////
inline raw_ostream &operator<<(raw_ostream &O, const cil::CallConv &CC)
{
  CC.print(O);
  return O;
}

inline raw_ostream &operator<<(raw_ostream &O, const cil::MethodRef &MR)
{
  MR.print(O);
  return O;
}

}

#endif /* CILMETADATA_H */
