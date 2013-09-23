add_llvm_target(CIL
  CILUtil.cpp
  CILTypes.cpp
  CILMetadata.cpp
  CILInstructions.cpp
  CILBackend.cpp
  )

add_subdirectory(TargetInfo)
