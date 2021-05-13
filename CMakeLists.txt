#TODO: Edit the "Project Working Directory" below
#TODO: Edit "EMSDK" to point to your emsdk file location
set(PWD "D:/git/Gaffney-Orthotics-Capstone-Project")
set(EMSDK "C:/git/emsdk")
cmake_minimum_required(VERSION 3.17.5)

# Project Name
project(Gaffney-Orthotics-Capstone-Project)

# Configure CMake using Emscripten's .cmake file which makes it use the emcc and em++ compilers
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_TOOLCHAIN_FILE=${EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake)

# Configure emcc/em++ arguments use \ to escape quotations "

set(FUNCTIONS "\"_import_file\",\"_main\",\"_is_ready\",\"_import_model\",\"_set_camera\",\"_export_model\",\"_print_hello\",\"_scale\",\"_get_export_strlen\",\"_on_mouse_up\",\"_translate_vertex\",\"_set_size\"")
set(OPTIONS "--post-js ${PWD}/frontend/wrapper.js -g -s ALLOW_MEMORY_GROWTH=1 -s INITIAL_MEMORY=1900MB -s MAXIMUM_MEMORY=4GB -s TOTAL_STACK=1GB -s SAFE_HEAP -s FORCE_FILESYSTEM=1 -s MAX_WEBGL_VERSION=2 -s FULL_ES3=1 -s EXPORTED_FUNCTIONS=[${FUNCTIONS}] -s EXPORTED_RUNTIME_METHODS=[\"ccall\",\"cwrap\",\"allocate\",\"intArrayFromString\"]")

# Tell CMake where to look for #include pre-processor directives
include_directories(${EMSDK}/upstream/emscripten/system/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} lib/assimp/include)

# Tell CMake to use the CMakeLists.txt inside the assimp library when compiling it
add_subdirectory(lib/assimp)

# Define sources (variable) to add to executable
file(GLOB_RECURSE sources ${PWD}/backend/src/core/*.cpp)
file(GLOB_RECURSE headers ${PWD}/backend/src/core/*.h)
file(GLOB_RECURSE engine-sources ${PWD}/backend/src/engine/*.cpp)
file(GLOB_RECURSE engine-headers ${PWD}/backend/src/engine/*.h)

# Define the executable using the sources
add_executable(backend ${sources} ${engine-sources} ${headers} ${engine-headers} backend/src/core/CylinderString.h backend/src/core/ArrowString.h)

# Link the built library to the project
target_link_libraries(backend assimp)

# Set the linker flags to use when compiling the projects
set_target_properties(backend PROPERTIES LINK_FLAGS "-s DEMANGLE_SUPPORT=1 ${OPTIONS}") # add '-s LLD_REPORT_UNDEFINED' for dbg info
set_target_properties(assimp PROPERTIES LINKER_LANGUAGE CXX) # help CMake determine what language assimp is written in