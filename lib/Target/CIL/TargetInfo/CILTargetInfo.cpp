//===-- CILTargetInfo.cpp - CIL Target Implementation -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "CILBackend.h"
#include "CILTargetMachine.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

Target llvm::TheCILTarget;

extern "C" void LLVMInitializeCILTargetInfo()
{
  RegisterTarget<Triple::cil, /*HasJIT=*/false>
    X(TheCILTarget, "cil", "Common Intermediate Language");
}

extern "C" void LLVMInitializeCILTargetMC()
{
}
