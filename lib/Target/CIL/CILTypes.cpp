//===-- cil::Types.cpp - Types for the CIL -------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===-----------------------------------------------------------------===//
//
//===-----------------------------------------------------------------===//

#include "CILTypes.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

void cil::Type::print(raw_ostream &O) const
{
  switch (ID)
    {
    case TypeID::Void:
      O << "void";
      break;
    case TypeID::Bool:
      O << "bool";
      break;
    case TypeID::Int8:
      O << "int8";
      break;
    case TypeID::UInt8:
      O << "unsigned int8";
      break;
    case TypeID::Int16:
      O << "int16";
      break;
    case TypeID::UInt16:
      O << "unsigned int16";
      break;
    case TypeID::Int32:
      O << "int32";
      break;
    case TypeID::UInt32:
      O << "unsigned int32";
      break;
    case TypeID::Int64:
      O << "int64";
      break;
    case TypeID::UInt64:
      O << "unsigned int64";
      break;
    case TypeID::Int:
      O << "native int";
      break;
    case TypeID::UInt:
      O << "native unsigned int";
      break;
    case TypeID::Float32:
      O << "float32";
      break;
    case TypeID::Float64:
      O << "float64";
      break;
    case TypeID::TypedRef:
      O << "typedref";
      break;
    default:
      llvm_unreachable("cil::Type::print(raw_ostream &) Unreachable");
    }
}

bool cil::Type::operator==(const Type &Ty) const
{
  if (getTypeID() != Ty.getTypeID())
    return false;
  else if (isPrimitive())
    return true;
  else
    return false;
}

////////////////////////////////////////////////////////////////////////////////
bool cil::CompositeType::operator==(const Type &Ty) const
{
  return getTypeID() == Ty.getTypeID()
    && getElemType() == static_cast<const CompositeType &>(Ty).getElemType();
}

////////////////////////////////////////////////////////////////////////////////
void cil::PointerType::print(raw_ostream &O) const
{
  O << *getElemType() << "*";
}

////////////////////////////////////////////////////////////////////////////////
void cil::VectorType::print(raw_ostream &O) const
{
  O << *getElemType() << "[]";
}

////////////////////////////////////////////////////////////////////////////////
void cil::ValueType::print(raw_ostream &O) const
{
  O << "valuetype " << getName();
}

bool cil::ValueType::operator==(const Type &Ty) const
{
  return getTypeID() == Ty.getTypeID()
    && getName() == static_cast<const ValueType &>(Ty).getName();
}
