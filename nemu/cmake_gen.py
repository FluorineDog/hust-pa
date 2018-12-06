#!python3
import os, sys
print(sys.argv)
temp = r'add_library()'

def load_module(dir_path, name):
    file_list = set()
    module_list = set()
    delta_m = set()
    for it in os.listdir(dir_path):
        # if(item == "debug"):
        #     continue
        path = dir_path + '/' + it
        item = '"' + it + '"'
        if os.path.isdir(path):
            module_list.add(item)
            m = load_module(path, item)
            delta_m = delta_m.union(m)
        elif it.endswith(".c"):
            file_list.add(item)
    # print(name, " with ", file_list," and ", module_list)
    text = ""
    if name == "main":
        text += "add_executable({0} {1})\n".format(name, " ".join(file_list))
    else:
        text += "add_library({0} OBJECT {1})\n".format(name, " ".join(file_list))

    if len(module_list) > 0:
        for module in module_list:
            text += "add_subdirectory({0})\n".format(module)

    if name == "main":
        module_list = module_list.union(delta_m)
        text += "target_link_libraries({0} {1})\n".format(name, " ".join(module_list))
        text += "target_link_libraries(main readline)\n"

    print(text)
    with open(dir_path + "/CMakeLists.txt", "w") as file:
        file.write(text)

    return module_list.union(delta_m)

load_module(sys.argv[1], "main")