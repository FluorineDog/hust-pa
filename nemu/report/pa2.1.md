## RTFSC
在实现dummy的过程中, 我们阅读了源代码, 有以下一些值得注意的要点:
- 每一条指令都分为 解码阶段和执行阶段
  - 解码阶段按照指令格式, 可以处理多种指令格式的排列组合, 包括
    - opcode 低位编码
    - ModR/M
    - imm8/imm16/imm32
  - 这些格式排列组合后, 可以表示出
    - 通用寄存器
    - 内存地址
    - 立即数
    - debug/control register
    - eflags
    - j 系列指令的相对寻址
    - ds:si/es:ds
  - 解码函数完成后, 其值被临时储存于id_src, id_dest, id_src2 中
    - 读取时, 应该采用id_xxx->val 访问相应的值
    - 写入时, 应该利用operand_write 函数
  - 执行函数按照如上的约定存取数据, 改写EFLAGS等. 
- opcode_table 存储了指令解码函数(可以为空)和执行函数的查找表. idex_wrapper 读取相应的函数, 分别执行
- 所有的解码和执行函数都使用rtl 伪指令作为数据流传递的方法. 
- 为了保证后期可以成功完成各种JIT 的目标, 同时尽可能得减少BUG 出现的概率, 我们对RTL 伪指令的书写做出以下约定: 
  - 所有的RTL伪指令不允许改变任何寄存器状态, 如果需要临时寄存器,
  应该在函数定义域中临时声明. 
  - 所有伪指令都必须由基本RTL伪指令或其他伪指令组合而成, 不允许递归
  - 不允许直接读取rtlreg_t 中的数据, 只能借助RTL伪指令进行处理
- 因此, 每一条指令的实现, 分为以下几个步骤:
  - 找到对应的解码函数, 如果没有, 实现它
  - 实现对应的执行函数, 并在all-intr.h 中声明它
  - 在exec.cpp 文件里的optable_table 上注册相应的函数

## dummy
dummy需要实现的指令不多, 只需要实现
- call imm
- sub r/m imm8
- xor r r/m
- ret
这些指令实现成功后, 可以得到正确输出
```
(nemu) si 9
  100000:   bd 00 00 00 00                        movl $0x0,%ebp
  100005:   bc 00 7c 00 00                        movl $0x7c00,%esp
  10000a:   e8 01 00 00 00                        call 100010
  100010:   55                                    pushl %ebp
  100011:   89 e5                                 movl %esp,%ebp
  100013:   83 ec 08                              subl $0x8,%esp
  100016:   e8 05 00 00 00                        call 100020
  100020:   31 c0                                 xorl %eax,%eax
  100022:   c3                                    ret
(nemu) si 9
  10001b:   d6                                    nemu trap
nemu: HIT GOOD TRAP at eip = 0x0010001b

[/home/mike/workspace/x86/ics-pa/nemu/src/monitor/cpu-exec.cpp,23,monitor_statistic] total guest instructions = 10
(nemu)
```
