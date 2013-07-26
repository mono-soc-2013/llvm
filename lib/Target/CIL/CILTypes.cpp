//===-- CILTypes.cpp - Types for the CIL -------------------*- C++ -*-===//
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

#include <cstdio>

using namespace llvm;

////////////////////////////////////////////////////////////////////////////////
CILType::CILType(TypeID ID)
  : ID(ID)
{
}

CILType::~CILType()
{
}

CILType *CILType::getVoidTy()
{
  static CILType T(VoidTyID);
  return &T;
}

CILType *CILType::getBoolTy()
{
  static CILType T(BoolTyID);
  return &T;
}

CILType *CILType::getInt8Ty()
{
  static CILType T(Int8TyID);
  return &T;
}

CILType *CILType::getUInt8Ty()
{
  static CILType T(UInt8TyID);
  return &T;
}

CILType *CILType::getInt16Ty()
{
  static CILType T(Int16TyID);
  return &T;
}

CILType *CILType::getUInt16Ty()
{
  static CILType T(UInt16TyID);
  return &T;
}

CILType *CILType::getInt32Ty()
{
  static CILType T(Int32TyID);
  return &T;
}

CILType *CILType::getUInt32Ty()
{
  static CILType T(UInt32TyID);
  return &T;
}

CILType *CILType::getInt64Ty()
{
  static CILType T(Int64TyID);
  return &T;
}

CILType *CILType::getUInt64Ty()
{
  static CILType T(UInt64TyID);
  return &T;
}

CILType *CILType::getIntTy()
{
  static CILType T(IntTyID);
  return &T;
}

CILType *CILType::getUIntTy()
{
  static CILType T(UIntTyID);
  return &T;
}

CILType *CILType::getFloat32Ty()
{
  static CILType T(Float32TyID);
  return &T;
}

CILType *CILType::getFloat64Ty()
{
  static CILType T(Float64TyID);
  return &T;
}

CILType *CILType::getTypedRefTy()
{
  static CILType T(TypedRefTyID);
  return &T;
}

CILType::TypeID CILType::getTypeID() const
{
  return ID;
}

bool CILType::isPrimitive() const
{
  return 0 <= ID && ID <= LastPrimitiveTyID;
}

bool CILType::isVoid() const
{
  return ID == VoidTyID;
}

void CILType::print(raw_ostream &O) const
{
  switch (ID)
    {
    case VoidTyID:
      O << "void";
      break;
    case BoolTyID:
      O << "bool";
      break;
    case Int8TyID:
      O << "int8";
      break;
    case UInt8TyID:
      O << "unsigned int8";
      break;
    case Int16TyID:
      O << "int16";
      break;
    case UInt16TyID:
      O << "unsigned int16";
      break;
    case Int32TyID:
      O << "int32";
      break;
    case UInt32TyID:
      O << "unsigned int32";
      break;
    case Int64TyID:
      O << "int64";
      break;
    case UInt64TyID:
      O << "unsigned int64";
      break;
    case IntTyID:
      O << "native int";
      break;
    case UIntTyID:
      O << "native unsigned int";
      break;
    case Float32TyID:
      O << "float32";
      break;
    case Float64TyID:
      O << "float64";
      break;
    case TypedRefTyID:
      O << "typedref";
      break;
    default:
      O << "WTF";
      break;
    }
}

bool CILType::operator==(CILType &Ty) const
{
  if (getTypeID() != Ty.getTypeID())
    return false;
  else if (isPrimitive())
    return true;
  else
    return false;
}

static inline raw_ostream &operator<<(raw_ostream &O, CILType &Ty)
{
  Ty.print(O);
  return O;
}

////////////////////////////////////////////////////////////////////////////////
CILCompositeType::CILCompositeType(TypeID ID, CILType *ElemTy)
  : CILType(ID)
  , ElemType(ElemTy)
{
}

CILCompositeType::~CILCompositeType()
{
  delete ElemType;
}

CILType *CILCompositeType::getElemType() const
{
  return ElemType;
}

bool CILCompositeType::operator==(CILType &Ty) const
{
  return getTypeID() == Ty.getTypeID()
    && getElemType() == static_cast<CILCompositeType &>(Ty).getElemType();
}

////////////////////////////////////////////////////////////////////////////////
CILPointerType::CILPointerType(CILType *ElemTy)
  : CILCompositeType(PointerTyID, ElemTy)
{
}

CILPointerType *CILPointerType::get(CILType *ElemTy)
{
  return new CILPointerType(ElemTy);
}

void CILPointerType::print(raw_ostream &O) const
{
  O << *getElemType() << "*";
}

////////////////////////////////////////////////////////////////////////////////
CILVectorType::CILVectorType(CILType *ElemTy)
  : CILCompositeType(VectorTyID, ElemTy)
{
}

CILVectorType *CILVectorType::get(CILType *ElemTy)
{
  return new CILVectorType(ElemTy);
}

void CILVectorType::print(raw_ostream &O) const
{
  O << *getElemType() << "[]";
}
