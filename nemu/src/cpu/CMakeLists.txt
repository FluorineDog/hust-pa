add_library("cpu" OBJECT "intr.cpp" "reg.cpp")
add_subdirectory("exec")
add_subdirectory("decode")
