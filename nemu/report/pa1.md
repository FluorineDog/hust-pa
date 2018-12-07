# PA1
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


|      层次图            |
|:---------------------:|
| "Hello World" program |
| Docker                |
| Windows               |
| Computer hardware     |

