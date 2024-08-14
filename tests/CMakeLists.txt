# tests/CMakeLists.txt

# Include the main source directory to access headers
include_directories(${PROJECT_SOURCE_DIR}/)

# Find the testing framework
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Specify test directory 
set(TEST_FILES_DIRECTORY ${PROJECT_SOURCE_DIR}/tests)

# Add main.cpp to all tests
set(TEST_MAIN ${TEST_FILES_DIRECTORY}/main.cpp)

# Recursively get all .cpp files in tests/
file(GLOB_RECURSE TEST_FILES ${TEST_FILES_DIRECTORY} *.cpp)

# Remove main.cpp from TEST_FILES if it's there
list(REMOVE_ITEM TEST_FILES ${TEST_MAIN})

# Iterate over each file
foreach(TEST_FILE ${TEST_FILES})
    # Get the file name without extension
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    
    # Create an executable for each test
    add_executable(${TEST_NAME} ${TEST_MAIN} ${TEST_FILE})
    
    # Link the GTest and pthread libraries
    target_link_libraries(${TEST_NAME} ${GTEST_LIBRARIES} pthread kruskaldb)
    
    # Add a test with the same name as the executable
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    
endforeach()