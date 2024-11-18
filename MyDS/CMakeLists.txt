cmake_minimum_required(VERSION 3.10)
project(MyDS)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加头文件目录
include_directories(${PROJECT_SOURCE_DIR})

# 设置源文件目录
set(SOURCE_DIR ${PROJECT_SOURCE_DIR})
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR})

# 收集源文件
file(GLOB SOURCES 
    "${SOURCE_DIR}/*.cpp"
)

# 收集头文件
file(GLOB HEADERS 
    "${INCLUDE_DIR}/*.h"
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# # 如果要构建测试
# option(BUILD_TESTS "Build the tests" ON)
# if(BUILD_TESTS)
#     enable_testing()
#     add_subdirectory(tests)
# endif()
