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

它是以结构体opcode_entry 为元素的数组,  opcode_entry包含了解码函数, 执行函数, 指令位宽.

> 但你是否怀疑过, 凭什么程序执行到main()函数的返回处就结束了?

main函数的返回想当于调用exit函数, 会执行一下清理工作, 比如已经被atexit注册好的函数

> 对于GNU/Linux上的一个程序, 怎么样才算开始? 怎么样才算是结束? 
> 对于在NEMU中运行的程序, 问题的答案又是什么呢?
> 与此相关的问题还有: NEMU中为什么要有nemu_trap? 为什么要有monitor?

TODO: 二周目回答

