# Set the LOCAL_LLVM_BUILDPATH to your llvm build path.
set(LOCAL_LLVM_PATH ${LOCAL_LLVM_BUILDPATH}/llvm)
set(LOCAL_CLANG_PATH ${LOCAL_LLVM_BUILDPATH}/clang)

# get LLVM and Clang
find_package(LLVM REQUIRED CONFIG PATHS ${LOCAL_LLVM_PATH} NO_DEFAULT_PATH)
find_package(Clang REQUIRED CONFIG PATHS ${LOCAL_CLANG_PATH} NO_DEFAULT_PATH)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "LLVM include dir: ${LLVM_INCLUDE_DIRS}")
message(STATUS "Clang include dir: ${CLANG_INCLUDE_DIRS}")

# add llvm
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
# add clang
include_directories(${CLANG_INCLUDE_DIRS})

# build target
add_library(libhisa STATIC
 tool/Instance.cpp
 StaticAnalyzer.cpp
 CSADriver.cpp
 Logger.cpp
 CommandLineParser.cpp
 ResultCollector.cpp
 OutputManager.cpp
 Json/DiagJsonTransform.cpp
 Json/JsonProc.cpp
 HtmlPrinter.cpp
 Statistics/StatisticsHelper.cpp
 AnalyzerOptions.cpp
 OutputOptions.cpp
 Event.cpp
 checker/MemoryChecker.cpp
 checker/UninitObjChecker/UninitializedObjectChecker.cpp
 checker/UninitObjChecker/UninitObjCheckerUtil.cpp
 checker/DivModZeroChecker.cpp
 checker/DereferenceChecker.cpp
 checker/ArrayBoundChecker.cpp
)

# llvm libs import
llvm_map_components_to_libnames(llvm_libs support)

# link llvm and libclang
set(CLANG_LIBS
  clangTooling
  clangStaticAnalyzerFrontend
  clangStaticAnalyzerCheckers
  clangStaticAnalyzerCore
)

target_link_libraries(libhisa ${llvm_libs} ${CLANG_LIBS})