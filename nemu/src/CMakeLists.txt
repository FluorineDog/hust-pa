add_executable(main "main.cpp")
add_subdirectory("device")
add_subdirectory("jit")
add_subdirectory("memory")
add_subdirectory("misc")
add_subdirectory("cpu")
add_subdirectory("monitor")
target_link_libraries(main "device" "jit" "diff-test" "memory" "misc" "cpu" "exec" "io" "decode" "debugg" "monitor")
target_link_libraries(main readline SDL2 dl)

option(LLVM_ENABLE_RTTI ON)
find_package(LLVM REQUIRED CONFIG)
option(LLVM_ENABLE_RTTI ON)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS} -DLLVM_ENABLE_RTTI=1)

# Now build our tools
# Find the libraries that correspond to the LLVM components
# that we wish to use

llvm_map_components_to_libnames(llvm_libs all)

# Link against LLVM libraries
target_link_libraries(main ${llvm_libs})
