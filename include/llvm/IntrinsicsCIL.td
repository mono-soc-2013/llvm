//==- IntrinsicsCIL.td - CIL intrinsics                       -*- tablegen -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines all of the CIL-specific intrinsics.
//
//===----------------------------------------------------------------------===//

let TargetPrefix = "cil" in {  // All intrinsics start with "llvm.cil.".
  // Load intrinsics
  def int_cil_ldstr : Intrinsic<[llvm_ptr_ty],[],[IntrNoMem]>;
  def int_cil_ldnull : Intrinsic<[llvm_ptr_ty],[],[IntrNoMem]>;

  // Object intrinsics
  def int_cil_newobj : Intrinsic<[llvm_ptr_ty],[],[IntrNoMem]>;
  def int_cil_newvalue : Intrinsic<[],[llvm_vararg_ty],[IntrNoMem]>;
  def int_cil_copyvalue : Intrinsic<[],[llvm_vararg_ty],[IntrNoMem]>;
  def int_cil_box : Intrinsic<[llvm_ptr_ty],[llvm_vararg_ty],[IntrNoMem]>;
  def int_cil_unbox : Intrinsic<[llvm_ptr_ty],[llvm_vararg_ty],[IntrNoMem]>;
  def int_cil_castclass : Intrinsic<[llvm_ptr_ty],[llvm_ptr_ty],[IntrNoMem]>;

  // Array intrinsics
  def int_cil_newarr : Intrinsic<[llvm_ptr_ty],[llvm_ptr_ty, llvm_vararg_ty],
    [IntrNoMem]>;
  def int_cil_ldarr : Intrinsic<[llvm_ptr_ty],[llvm_ptr_ty, llvm_vararg_ty],
    [IntrNoMem]>;
  def int_cil_starr : Intrinsic<[],[llvm_ptr_ty, llvm_vararg_ty],
    [IntrNoMem]>;
}
