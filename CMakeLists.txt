# CMakeLists.txt in the project root
cmake_minimum_required(VERSION 3.10)
project(KruskalDB)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the include directory
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add your source directory
add_subdirectory(src)

# Add the tests
enable_testing()
add_subdirectory(tests)