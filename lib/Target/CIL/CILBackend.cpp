//===-- CILBackend.cpp - Library for converting LLVM code to CIL --------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This library converts LLVM code to CIL code.
//
//===----------------------------------------------------------------------===//

#include "CILBackend.h"
#include "CILTargetMachine.h"
#include "llvm/PassSupport.h"
#include "llvm/IR/ValueSymbolTable.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Analysis/ConstantsScanner.h"
#include "llvm/Support/CallSite.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/CodeGen/Passes.h"
#include <sstream>

using namespace llvm;

extern "C" void LLVMInitializeCILTarget()
{
  RegisterTargetMachine<CILTargetMachine> X(TheCILTarget);
}
