//===-- CILBackend.h - TargetMachine for the CIL --------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the CILWriter that is used by the CIL.
//
//===----------------------------------------------------------------------===//
#ifndef CILBACKEND_H
#define CILBACKEND_H

#include "llvm/Pass.h"
#include "llvm/PassManager.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Analysis/FindUsedTypes.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/GetElementPtrTypeIterator.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/CodeGen/IntrinsicLowering.h"

namespace llvm
{
extern Target TheCILTarget;
}

#endif

