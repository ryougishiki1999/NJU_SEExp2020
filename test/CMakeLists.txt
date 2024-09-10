cmake_minimum_required(VERSION 3.4.3)
project(HiSATest)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

add_executable(HiSATest MemoryChecker/double_delete.cpp
 MemoryChecker/double_free.cpp
 MemoryChecker/memory_leak.cpp
 MemoryChecker/use_after_free.cpp
 DivModZeroChecker/div_and_mod_zero.cpp
 DereferenceChecker/direct.cpp
 DereferenceChecker/implicit.cpp
 DereferenceChecker/normal.cpp
 DereferenceChecker/undefine.cpp
 UninitObjChecker/baseClass_field.cpp
 UninitObjChecker/cyclic_loc_field.cpp
 UninitObjChecker/loc_field.cpp
 UninitObjChecker/needCast_loc_field.cpp
 UninitObjChecker/regular_field.cpp
 UninitObjChecker/remaining_test.cpp
 ArrayBoundChecker/bufferOverflows.cpp
 ArrayBoundChecker/bufferOverflows2.cpp
 ArrayBoundChecker/pointerOverflows.cpp
 ArrayBoundChecker/arrayBoundOverflows.cpp
 )