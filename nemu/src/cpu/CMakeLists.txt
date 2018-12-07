add_library("cpu" OBJECT "reg.cpp" "intr.cpp")
add_subdirectory("decode")
add_subdirectory("exec")
