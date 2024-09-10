cmake_minimum_required(VERSION 3.4.3)
project(PREPROCESSOR_TEST)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_CXX_STANDARD 17)

# set(JSONCPP_INCLUDE_DIR jsoncpp/include)
# set(JSONCPP_LIB_DIR jsoncpp/lib)

# include_directories(${JSONCPP_INCLUDE_DIR})
# link_directories(${JSONCPP_LIB_DIR})
# link_libraries(jsoncpp)

# build target
add_executable(preprocessor_test
    ../../core/Json/JsonProc.cpp
    Preprocessor.cpp
    FileSystemTree.cpp
)

# target_link_libraries(preprocessor_test jsoncpp)
