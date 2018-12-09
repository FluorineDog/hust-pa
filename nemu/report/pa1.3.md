# PA 1.3
有了表达式引擎后, 我们就可以愉快地补充我们NEMU 命令行的功能了,
列表如下
|命令| 功能| 实现细节|
|---|---|---|
|help|打印帮助|x
|c|继续执行|x
|q|退出|x
|si [N]|单步执行N条指令|x
|info r|打印寄存器状态|x
|info w|打印watchpoint状态|
|p EXPR|计算表达式|_
|x N EXPR|扫描内存|
|w EXPR|设置watchpoint|
|d N| 删除N号watchpoint|
前5个功能已经在以前的阶段中实现, 我们只需要其他功能即可
## 计算表达式
从最简单的入手, 计算表达式实际上只需要对表达式引擎生成的结果进行一次求值即可.
## 