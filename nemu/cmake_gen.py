#!python3
import os, sys
print(sys.argv)
temp = r'add_library()'

def load_module(dir_path, name):
    file_list = set()
    module_list = set()
    for item in os.listdir(dir_path):
        if(item == "debug"):
            continue
        if os.path.isdir(dir_path+ "/" + item):
            module_list.add(item)
            load_module(dir_path+ "/" + item, item)
        elif item.endswith(".c"):
            file_list.add(item)
    # print(name, " with ", file_list," and ", module_list)
    text = ""
    if name == "main":
        text += "add_executable({0} {1})\n".format(name, " ".join(file_list))
    else:
        text += "add_library({0} {1})\n".format(name, " ".join(file_list))

    if len(module_list) > 0:
        for module in module_list:
            text += "add_subdirectory({0})\n".format(module)
        text += "target_link_libraries({0} {1} {1})\n".format(name, " ".join(module_list))
    if name == "main":
        text += "target_link_libraries(main readline)\n"
    print(text)
    with open(dir_path + "/CMakeLists.txt", "w") as file:
        file.write(text)

load_module(sys.argv[1], "main")