//===-- CILMetadata.cpp - Metadata for the CIL ----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//===----------------------------------------------------------------------===//

#include "CILMetadata.h"
#include <algorithm>

using namespace llvm;

void cil::CallConv::print(raw_ostream &O) const
{
  switch (Attr) {
  case CallAttr::None:
    break;
  case CallAttr::Instance:
    O << "instance ";
    break;
  case CallAttr::InstanceExplicit:
    O << "instance explicit ";
    break;
  }
  switch (Kind) {
  case CallKind::Default:
    O << "default";
    break;
  case CallKind::Cdecl:
    O << "unmanaged cdecl";
    break;
  case CallKind::Fastcall:
    O << "unmanaged fastcall";
    break;
  case CallKind::Stdcall:
    O << "unmanaged stdcall";
    break;
  case CallKind::Thiscall:
    O << "unmanaged thiscall";
    break;
  case CallKind::Vararg:
    O << "vararg";
    break;
  }
}

////////////////////////////////////////////////////////////////////////////////
void cil::MethodRef::print(raw_ostream &O) const
{
  O << "method " << CC << " " << *RetTy << " " << Name << "(";
  auto Iter = Params.begin(), End = Params.end();
  unsigned int I = 1, Size = Params.size();
  while (Iter != End) {
    const Type *ParamTy = *Iter;
    O << *ParamTy << (I < Size ? ", " : ")");

    ++Iter;
    ++I;
  }
}
