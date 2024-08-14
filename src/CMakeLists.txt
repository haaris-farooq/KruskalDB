# src/CMakeLists.txt

# Specify source directory
set(SOURCE_FILES_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Explicitly list the directories
set(SUBDIRECTORIES
    core
    storage
    cache_dir  # Rename this to avoid potential conflicts
)

# Recursively get all .cpp files in src/
foreach(DIR ${SUBDIRECTORIES})
    file(GLOB_RECURSE DIR_SOURCES ${SOURCE_FILES_DIRECTORY}/${DIR}/*.cpp)
    list(APPEND SOURCE_FILES ${DIR_SOURCES})
endforeach()

# Create a library target
add_library(kruskaldb STATIC ${SOURCE_FILES})

# Include directories
target_include_directories(kruskaldb PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)

# Set C++ standard for the library
set_target_properties(kruskaldb PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# If you have any dependencies, link them here
# target_link_libraries(kruskaldb PRIVATE some_dependency)