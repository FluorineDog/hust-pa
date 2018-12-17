## 添加teset-diff支持
在开启DIFF_TEST 宏后运行, 发现程序assert 停止在diff_test处的TODO上. 
对于通用寄存器, 框架已经基本搭好, 只需要在在diff-test.cpp 处添加比较语句即可

对于EFlags, 比较麻烦, 