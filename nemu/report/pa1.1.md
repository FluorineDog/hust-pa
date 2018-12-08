# PA1.1
## 实现正确的 CPU_State 结构
根据 x86 手册, CPU 内的通用寄存器有8个, 可以根据其位次访问gpr\[i\], 也可以使用其别名访问.  
因此, 我们需要正确实现union结构, 保证两种访问方式的是等价的
```c++
typedef union {
  union {
    uint32_t _32;
    uint16_t _16;
    uint8_t _8[2];
  } gpr[8];

  /* Do NOT change the order of the GPRs' definitions. */

  /* In NEMU, rtlreg_t is exactly uint32_t. This makes RTL instructions
   * in PA2 able to directly access these registers.
   */
  struct {
      rtlreg_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
      vaddr_t eip;
  };
} CPU_state;
```

## RTFSC

在CLion中调试跟踪源码, 理清了程序的执行流
- 程序调用init_monitor 进行初始化步骤
  - parse_args 解析命令行参数
  - init_log 打开log 文件作为调试输出
  - reg_test 测试CPU_State 是否正确实现
  - load_img 加载镜像, 如果命令行参数没有给出, 
  加载一个只有mov和halt指令的小镜像到ENTRY_START处
  - restart 将EIP 设置为ENTRY_START, 也就是默认的镜像入口点
  - init_regex 初始化正则引擎
  - init_wp_pool 初始化watchpoint 的链表
  - init_device 暂时为空, 等待后期实现IO_Exception 
  - init_difftest 等待后期实现diff测试
- 接下来, 进入了ui_mainloop, 也就是NEMU 图形界面的主要消息循环
  - 在batch_mode 下, 跳过交互代码, 直接执行cpu_exec, 不然进入NEMU命令行模式, 详解如下:
    - 使用readline 读取NEMU 命令
    - 在cmd_table 查找执行对应的handler
        - help 打印帮助信息
        - q 退出
        - c 执行cpu_exec
  - 上一步最终都要进入cpu_exec, 以下详述cpu_exec
    - 判断nemu_state, 在ABORT/END 等状态终止执行
    - 设置必要符号后, 切入exec_wrapper
      - decoding 为全局状态变量, 首先设置seq_eip 为 EIP
      - 切入exec_real中, 分析指令的第一个字节, 设置对应的位宽, opcode, 然后切入idex中
        - 分别执行指令的decode 和execute 函数, 
        完成指令的解码执行
      - 打印调试信息
      - 更新EIP
    - 用户程序指令计数器喜+1
    - 检查watchpoint 
    - 打印若干调试信息
    - 循环执行, 直到NEMU状态不再是RUNNING时退出
  - 命令行模式读取下一条指令, 或者在batch 模式下直接退出

本模拟器的核心在与decode 和execute 函数, 
他们根据指令的不同, 形态迥异. 目前仅有mov被实现了, 
其具体细节暂时作为黑箱处理.

## 单步执行
解析需要执行的步数, 调用cpu_exec 即可

## 打印寄存器
仿照gcc 的格式, 将8个寄存器按照GPR 的顺序输出

## 扫描内存
根据内存首地址, 将连续N 个16进制数输出即可

## Q&A
> 假设你在Windows中使用Docker安装了一个GNU/Linux container, 
> 然后在container中完成PA, 通过NEMU运行Hello World程序. 在这样的情况下, 尝试画出相应的层次图.

|      层次图             |
|:----------------------:|
| "Hello World" program  |
| Simulated x86 hardware |
| NEMU                   |
| Docker                 |
| Windows                |
| Computer hardware      |

> 如果没有寄存器, 计算机还可以工作吗? 如果可以, 这会对硬件提供的编程模型有什么影响呢?

寄存器是计算机储存体系结构的最顶层. 如果没有寄存器, 那么所有的信息都必须来自内存. 
所有的指令都必须直接在内存上运行. 由于内存空间很大, 所有的指令可能都需要带上很长的位段, 描述从内存何处取值,
或者划分内存的一段极小的区域作为"指令高速区域"进行操作, 
但是前者会极大增大指令体积, 后者实际上和寄存器方法没有太大区别. 

> 我们知道, 时序逻辑电路里面有"状态"的概念. 
> 那么, 对于TRM来说, 是不是也有这样的概念呢? 
> 具体地, 什么东西表征了TRM的状态? 在状态模型中, 执行指令和执行程序, 其本质分别是什么?

其状态包含存储器, PC, 寄存器的值. 
执行指令, 等价于以储存器中的值作为输入, 依照对应的指令进行一次状态的变换. 
执行程序就是这一系列变换过程的总和. 

> 回忆程序设计课的内容, 一个程序从哪里开始执行呢?

程序拥有EFI 结构体, 其中有个entry_point参数决定了程序执行的入口地址. 

> 阅读reg_test()的代码, 思考代码中的assert()条件是根据什么写出来的.

第一部分, 是根据x86的小端序结构, 8bit读取被写入的32bit得到了低字节  
第二部分测试别名. 保证aliasing 的变量等价
> 在cmd_c()函数中, 调用cpu_exec()的时候传入了参数-1, 你知道这是什么意思吗?

cpu_exec的参数表示指令执行的最大条数, 其类型是uint64, 输入-1被转换为UINT64_MAX, 
表示无限执行下去直到halt 

> "调用cpu_exec()的时候传入了参数-1", 这一做法属于未定义行为吗? 请查阅C11手册确认你的想法.

根据手册6.3.1.3, 这是安全的. 
```
1 When a value with integer type is converted to another integer type other than _Bool, if
the value can be represented by the new type, it is unchanged.
2 Otherwise, if the new type is unsigned, the value is converted by repeatedly adding or
subtracting one more than the maximum value that can be represented in the new type
until the value is in the range of the new type. 
```

> opcode_table到底是个什么类型的数组? 

它是以结构体opcode_entry 为元素的数组, 
opcode_entry包含了解码函数, 执行函数, 指令位宽.

> 但你是否怀疑过, 凭什么程序执行到main()函数的返回处就结束了?

main函数的返回想当于调用exit函数, 会执行一下清理工作, 比如已经被atexit注册好的函数

> 对于GNU/Linux上的一个程序, 怎么样才算开始? 怎么样才算是结束? 
> 对于在NEMU中运行的程序, 问题的答案又是什么呢?  
> 与此相关的问题还有: NEMU中为什么要有nemu_trap? 为什么要有monitor?

TODO: 二周目回答

> 如何测试字符串处理函数?

可以随机你生成若干个字符串, 用1到多个空格分隔开来. 使用args 解析后, 能够恢复原数组即可. 

