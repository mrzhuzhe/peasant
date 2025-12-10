cmake_minimum_required(VERSION 3.20.0)
project(SimpleProject)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

link_directories(${LLVM_LIBRARY_DIR})

# LLVM is normally built without RTTI. Be consistent with that.
if(NOT LLVM_ENABLE_RTTI)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

# message(STATUS ${LLVM_INCLUDE_DIRS})

add_library(new SHARED 
  new.cpp
  LoopVectorizationLegality.cpp
  LoopVectorize.cpp
  new.cpp
  Vectorize.cpp
  VPlanAnalysis.cpp
  VPlanConstruction.cpp
  VPlan.cpp
  VPlanPredicator.cpp
  VPlanRecipes.cpp
  VPlanTransforms.cpp
  VPlanUnroll.cpp
  VPlanUtils.cpp
  VPlanVerifier.cpp
)
