# PA 1.3
有了表达式引擎后, 我们就可以愉快地补充我们NEMU 命令行的功能了,
列表如下
|命令| 功能| 实现|
|---|---|---|
|help|打印帮助|x
|c|继续执行|x
|q|退出|x
|si [N]|单步执行N条指令|x
|info r|打印寄存器状态|x
|info w|打印watchpoint状态|x
|p EXPR|计算表达式| x
|x N EXPR|扫描内存| x
|w EXPR|设置watchpoint|x
|d N| 删除N号watchpoint|x
前5个功能已经在以前的阶段中实现, 我们只需要其他功能即可
## 计算表达式
计算表达式实际上只需要对表达式引擎生成的结果进行一次求值即可.

## 内存扫描
内存扫描实际上就是按照格式将内存数据打印出来即可. 
我们仿照gdb 的格式, 生成了如下效果
```
(nemu) x 16 $eip
0x00100000: 0x001234b8    0x0027b900    0x01890010    0x0441c766    
0x00100010: 0x02bb0001    0x66000000    0x009984c7    0x01ffffe0    
0x00100020: 0x0000b800    0x00d60000    0x00000000    0x00000000    
0x00100030: 0x00000000    0x00000000    0x00000000    0x00000000
```

## watchpoint
Watchpoint 在原先的框架中是一个限制了长度的链表. 我们将其删除, 改为std::map存放.
因此, 添加删除与遍历watchpoint 的方法就非常简单了, 只需要调用相应API即可. 


为了是的链表有一个全局唯一的编号, 我们定义了一个全局自增变量g_watch_count以供使用. 

WatchPoint 结构体中存放了`伪编译`好的表达式, 以及上一次更新时的表达式. 
在CPU_exec 中, 每当执行一条指令, 都需要更新watchpoint 信息. 如果发现其中有值发生了改变,
需要打印新旧值, 和检视点信息, 并停止执行. 

最后效果如下
```
(nemu) w 123
Added to watchpoint 0
(nemu) w 456
Added to watchpoint 1
(nemu) w 789
Added to watchpoint 2
(nemu) info w
id      value           expr
0       123             123
1       456             456
2       789             789
(nemu) d 1
Watchpoint 1 deleted
(nemu) w *($eip)
Added to watchpoint 3
(nemu) c
Software watchpoint 3: *($eip)
Old value = 1193144 [0x001234b8]
New value = 268445625 [0x100027b9]

(nemu) info w
id      value           expr
0       123             123
2       789             789
3       268445625       *($eip)
(nemu) 
```
