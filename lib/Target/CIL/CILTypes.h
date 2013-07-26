//===-- CILTypes.h - Types for the CIL ---------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===-----------------------------------------------------------------===//
//
//===-----------------------------------------------------------------===//
#ifndef CILTYPES_H
#define CILTYPES_H

#include "llvm/Support/FormattedStream.h"
#include <cstdint>

namespace llvm
{

////////////////////////////////////////////////////////////////////////////////
class CILType
{
public:
  enum TypeID {
    VoidTyID = 0,
    BoolTyID,
    Int8TyID,
    UInt8TyID,
    Int16TyID,
    UInt16TyID,
    Int32TyID,
    UInt32TyID,
    Int64TyID,
    UInt64TyID,
    IntTyID,
    UIntTyID,
    Float32TyID,
    Float64TyID,
    TypedRefTyID,

    PointerTyID,
    ManagedPointerTyID,
    FunctionTyID,
    VectorTyID
  };

private:
  static const TypeID LastPrimitiveTyID = TypedRefTyID;

private:
  TypeID ID;

protected:
  CILType(TypeID ID);
public:
  virtual ~CILType();

public:
  TypeID getTypeID() const;

  bool isPrimitive() const;
  bool isVoid() const;

  static CILType *getVoidTy();
  static CILType *getBoolTy();
  static CILType *getInt8Ty();
  static CILType *getUInt8Ty();
  static CILType *getInt16Ty();
  static CILType *getUInt16Ty();
  static CILType *getInt32Ty();
  static CILType *getUInt32Ty();
  static CILType *getInt64Ty();
  static CILType *getUInt64Ty();
  static CILType *getIntTy();
  static CILType *getUIntTy();
  static CILType *getFloat32Ty();
  static CILType *getFloat64Ty();
  static CILType *getTypedRefTy();

  virtual void print(raw_ostream &O) const;

  virtual bool operator==(CILType &T) const;
};

////////////////////////////////////////////////////////////////////////////////
class CILCompositeType : public CILType
{
private:
  CILType *ElemType;
public:
  virtual ~CILCompositeType();

protected:
  CILCompositeType(TypeID ID, CILType *ElemType);

public:
  CILType *getElemType() const;

  virtual bool operator==(CILType &T) const;
};

////////////////////////////////////////////////////////////////////////////////
class CILPointerType : public CILCompositeType
{
protected:
  CILPointerType(CILType *ElemType);

public:
  static CILPointerType *get(CILType *ElemType);

  virtual void print(raw_ostream &O) const;
};

////////////////////////////////////////////////////////////////////////////////
class CILVectorType : public CILCompositeType
{
protected:
  CILVectorType(CILType *ElemType);

public:
  static CILVectorType *get(CILType *ElemType);

  virtual void print(raw_ostream &O) const;
};

}

#endif
