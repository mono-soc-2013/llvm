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

#include "llvm/Support/raw_ostream.h"

namespace llvm {

namespace cil {

enum class TypeID : unsigned short {
  Void = 0,
  Bool,
  Int8,
  UInt8,
  Int16,
  UInt16,
  Int32,
  UInt32,
  Int64,
  UInt64,
  Int,
  UInt,
  Float32,
  Float64,
  TypedRef,

  Pointer,
  ManagedPointer,
  Vector,
  Function,

  ValueType
};

namespace {

const unsigned short
  FirstPrimitive = static_cast<unsigned short>(TypeID::Void),
  LastPrimitive = static_cast<unsigned short>(TypeID::TypedRef);

}

////////////////////////////////////////
class Type {
  TypeID ID;

protected:
  Type(TypeID ID)
    : ID(ID)
  {
  }

public:
  virtual ~Type()
  {
  }

public:
  TypeID getTypeID() const
  {
    return ID;
  }

  bool isPrimitive() const
  {
    unsigned short _ID = static_cast<unsigned short>(ID);
    return 0 <= _ID && _ID <= LastPrimitive;
  }

  bool isVoid() const
  {
    return ID == TypeID::Void;
  }

  static Type *getVoidTy()
  {
    static Type T(TypeID::Void);
    return &T;
  }

  static Type *getBoolTy()
  {
    static Type T(TypeID::Bool);
    return &T;
  }

  static Type *getInt8Ty()
  {
    static Type T(TypeID::Int8);
    return &T;
  }

  static Type *getUInt8Ty()
  {
    static Type T(TypeID::UInt8);
    return &T;
  }

  static Type *getInt16Ty()
  {
    static Type T(TypeID::Int16);
    return &T;
  }

  static Type *getUInt16Ty()
  {
    static Type T(TypeID::UInt16);
    return &T;
  }

  static Type *getInt32Ty()
  {
    static Type T(TypeID::Int32);
    return &T;
  }

  static Type *getUInt32Ty()
  {
    static Type T(TypeID::UInt32);
    return &T;
  }

  static Type *getInt64Ty()
  {
    static Type T(TypeID::Int64);
    return &T;
  }

  static Type *getUInt64Ty()
  {
    static Type T(TypeID::UInt64);
    return &T;
  }

  static Type *getIntTy()
  {
    static Type T(TypeID::Int);
    return &T;
  }

  static Type *getUIntTy()
  {
    static Type T(TypeID::UInt);
    return &T;
  }

  static Type *getFloat32Ty()
  {
    static Type T(TypeID::Float32);
    return &T;
  }

  static Type *getFloat64Ty()
  {
    static Type T(TypeID::Float64);
    return &T;
  }

  static Type *getTypedRefTy()
  {
    static Type T(TypeID::TypedRef);
    return &T;
  }

public:
  virtual void print(raw_ostream &O) const;

public:
  virtual bool operator==(const Type &T) const;
};

////////////////////////////////////////
class CompositeType : public Type {
  const Type *ElemTy;

protected:
  CompositeType(TypeID ID, const Type *ElemTy)
    : Type(ID)
    , ElemTy(ElemTy)
  {
  }
public:
  ~CompositeType()
  {
  }

public:
  const Type *getElemType() const
  {
    return ElemTy;
  }

  virtual bool operator==(const Type &T) const;
};

////////////////////////////////////////
class PointerType : public CompositeType {
protected:
  PointerType(const Type *ElemTy)
    : CompositeType(TypeID::Pointer, ElemTy)
  {
  }

public:
  static PointerType *get(const Type *ElemTy)
  {
    return new PointerType(ElemTy);
  }

  virtual void print(raw_ostream &O) const;
};

////////////////////////////////////////
class VectorType : public CompositeType {
protected:
  VectorType(const Type *ElemTy)
    : CompositeType(TypeID::Vector, ElemTy)
  {
  }

public:
  static VectorType *get(const Type *ElemTy)
  {
    return new VectorType(ElemTy);
  }

  virtual void print(raw_ostream &O) const;
};

////////////////////////////////////////
class FunctionType : public Type {
public:
  typedef std::vector<const Type *>::const_iterator const_param_iterator;

public:
  static FunctionType *get(const Type *RetTy,
                           const std::vector<const Type *> &ParamTys)
  {
    return new FunctionType(RetTy, ParamTys);
  }

  const Type *getReturnType() const
  {
    return RetTy;
  }

  const_param_iterator param_begin() const
  {
    return ParamTys.begin();
  }

  const_param_iterator param_end() const
  {
    return ParamTys.end();
  }

  const Type *getParamType(unsigned I) const
  {
    return ParamTys[I];
  }

  unsigned getNumParams() const
  {
    return ParamTys.size();
  }

protected:
  FunctionType(const Type *RetTy,
               const std::vector<const Type *> &ParamTys)
    : Type(TypeID::Function)
    , RetTy(RetTy)
    , ParamTys(ParamTys)
  {
  }

private:
  const Type *RetTy;
  const std::vector<const Type *>ParamTys;
};

////////////////////////////////////////
class ValueType : public Type {
  StringRef Name;

protected:
  ValueType(StringRef Name)
    : Type(TypeID::ValueType)
    , Name(Name)
  {
  }

public:
  ~ValueType()
  {
  }

  StringRef getName() const
  {
    return Name;
  }

  static ValueType *get(StringRef Name)
  {
    return new ValueType(Name);
  }

  virtual void print(raw_ostream &O) const;
  virtual bool operator==(const Type &T) const;
};

}

////////////////////////////////////////////////////////////////////////////////
inline raw_ostream &operator<<(raw_ostream &O, const cil::Type &Ty)
{
  Ty.print(O);
  return O;
}

}

#endif /* CILTYPES_H */
