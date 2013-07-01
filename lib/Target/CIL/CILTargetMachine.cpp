//===-- CILTargetMachine.cpp - TargetMachine for the CIL backend -*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===-----------------------------------------------------------------------===//
//
// This file declares the TargetMachine that is used by the CIL backend.
//
//===-----------------------------------------------------------------------===//

#include "CILTargetMachine.h"
#include "llvm/PassSupport.h"
#include "llvm/IR/ValueSymbolTable.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Analysis/ConstantsScanner.h"
#include "llvm/Support/CallSite.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/CodeGen/Passes.h"
#include <iostream>
#include <string>

using namespace llvm;

CILTargetMachine::CILTargetMachine(const Target &T, StringRef TT,
                                   StringRef CPU, StringRef FS,
                                   const TargetOptions &Options,
                                   Reloc::Model RM, CodeModel::Model CM,
                                   CodeGenOpt::Level OL)
  : TargetMachine(T, TT, CPU, FS, Options)
{
}

const DataLayout *CILTargetMachine::getDataLayout() const
{
  return NULL;
}

bool CILTargetMachine::addPassesToEmitFile(PassManagerBase &PM,
                                           formatted_raw_ostream &O,
                                           CodeGenFileType FileType,
                                           bool DisableVerify,
                                           AnalysisID StartAfter,
                                           AnalysisID StopAfter)
{
  if (FileType != TargetMachine::CGFT_AssemblyFile)
    return true;

  O << "addPassesToEmitFile\n";

  return false;
}
