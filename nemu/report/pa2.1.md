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
- opcode_table 存储了指令解码函数(可以为空)和执行函数的查找表. idex_wrapper 读取相应的函数, 分别执行
- 所有的解码和执行函数都使用, 