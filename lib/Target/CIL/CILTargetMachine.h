//===-- CILTargetMachine.h - TargetMachine for the CIL backend -*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the TargetMachine that is used by the CIL backend.
//
//===----------------------------------------------------------------------===//

#ifndef CILTARGETMACHINE_H
#define CILTARGETMACHINE_H

#include "llvm/Target/TargetMachine.h"

namespace llvm
{
struct CILTargetMachine : public TargetMachine
{
  CILTargetMachine(const Target &T,
                   StringRef TargetTriple,
                   StringRef CPU,
                   StringRef FS,
                   const TargetOptions &Options,
                   Reloc::Model RM,
                   CodeModel::Model CM,
                   CodeGenOpt::Level OL)
    : TargetMachine(T, TargetTriple, CPU, FS, Options)
  {
  }

  virtual bool addPassesToEmitFile(PassManagerBase &PM,
                                   formatted_raw_ostream &Out,
                                   CodeGenFileType FileType,
                                   bool DisableVerify,
                                   AnalysisID StartAfter,
                                   AnalysisID StopAfter);
};

extern Target TheCILTarget;
}
#endif
