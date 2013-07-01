#set(LLVM_TARGET_DEFINITIONS CIL.td)

#tablegen(LLVM CILGenInstrInfo.inc -gen-instr-info)

#add_public_tablegen_target(CILCommonTableGen)

add_llvm_target(CIL
  CILInstructions.cpp
  CILTargetMachine.cpp
  CILBackend.cpp
  )

add_subdirectory(TargetInfo)
