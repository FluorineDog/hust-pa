add_library("cpu" OBJECT "intr.cpp" "reg.cpp")
add_subdirectory("decode")
add_subdirectory("exec")
