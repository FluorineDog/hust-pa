# PA0
本节的主要目的为配置环境, 搭建好良好易用的基础设施.  

PA原本自带的手动编写的Makefile 已经不能满足现代编程的需求, 
因而我们选择将其更换为CMake, 这样一来我们就可以在CLion 等IDE 上进行开发.

与此同时, 为了专注与体系结构, 避免将宝贵的时间浪费于 regex/linked-list 等基础数据结构的实现上, 
我们将整个项目向C++ 17 上迁移, 以充分利用其丰富强大的标准库与语法糖.

## 配置环境
由于我选择使用原生的ArchLinux 环境, 无需折腾复杂的Docker 镜像

按照任务书使用 `git clone` 下载对应仓库后, 运行`init.sh` 脚本完成

## CMake 迁移
首先, 我们需要分析Makefile的功能   
1. 将src 下所有 .c 文件编译为 .o 文件
2. 引用include 下的头文件
3. 加入必要的编译选项
4. 在每次编译时都进行 `git commit` 操作

前3点都有对应的CMake 命令可以实现. 为此, 我编写了脚本 `cmake_gen.py` 自动化这一步骤

至于第四点, 对于不熟悉 `git` 的同学来说可能非常有价值, 但是对于我本人没有意义, 
反而对我使用 `git diff` 造成了障碍. 
因此这一点被我无视, 同时原版Makefile 的这一功能也被我注释掉. 

在迁移过程中, 我还发现了原版代码需要添加`-Wno-restrict` 编译选项来避免高版本的gcc的报错. 

## C++ 迁移
第一步, 当然是将.c 文件全部改成.cpp 文件
```
find . | grep -E "\.c" | xargs rename .c .cpp
```

接下来, 由于C++ 是一门强类型语言, -fpermissive 被默认启用且在高版本的编译器上不可关闭. 因此, 我们需要对 void* 指针的隐式转换进行相应的修改

```c++
// in include/common.h
template<typename To>
inline void BITCAST(To& dst, const void* src){
    dst = (To)src; 
}

// in some src file
// ...
  // ref_difftest_memcpy_from_dut = dlsym(handle, "difftest_memcpy_from_dut");
  BITCAST(ref_difftest_memcpy_from_dut, dlsym(handle, "difftest_memcpy_from_dut"));
// ...
```
C++ 不支持`non-trivial designated initializers`, 因此需要在`keyboard.cpp`下做出如下修改
```c++
// #define XX(k) [name_concat(SDL_SCANCODE_, k)] = name_concat(_KEY_, k),
// static uint32_t keymap[256] = {
//   _KEYS(XX)
// };
static uint32_t keymap[256] = {};
static void init_keymap(){
    #define XX(k) keymap[name_concat(SDL_SCANCODE_, k)] = name_concat(_KEY_, k);
    _KEYS(XX)
}
```
出于安全因素, C++ 不允许将常量字符串赋值给`char *`类型, 因此需要添加`const` 修饰符.

## Todo
观察代码发现, `dlsym` 处可能出现C/C++链接符号不同带来的问题. 此处暂时放下不管, 等待PA2再进行处理. 

