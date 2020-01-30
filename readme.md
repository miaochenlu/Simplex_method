group project for AOR course

ljy, mcl, whh, zfx

# **一．** 实验内容与要求

理解单纯形法和对偶单纯形法的原理，并编程实现。要求其能适用于较大规模（极限情况为200个决策变量，500个约束）的线性规划，并且保证解的误差小于等于$0.1^4$。程序需要考虑、并且能够处理退化、无解、无有限最优解等情况。

# 二. 程序输入输出格式

<img src="/Users/jones/Library/Application Support/typora-user-images/image-20200130174125561.png" alt="image-20200130174125561" style="zoom:50%;" />

其中n为决策变量个数，m为约束个数。

$c_1,c_2,\cdots,c_n $分别对应目标函数中决策变量$x_1,x_2,\cdots,x_n$ 的系数(极小化目标函数)。

$a_{i1}, a_{i2},\cdots, a_{in}, b_i,d_i$ 对应第i条约束，其中$a_{i1}, a_{i2},\cdots, a_{in}$分别对应$x_1,x_2,\cdots,x_n$ 的系数，$b_i$对应该约束的右端系数，$d_i=-1、0、1$分别对应小于等于、等于、大于等于三种约束。

* $e_i=-1$表示 $x_i$小于等于0
* $e_i=0$表示$x_i$无约束
* $e_i=1$表示$x_i$大于等于0

<img src="/Users/jones/Library/Application Support/typora-user-images/image-20200130175315633.png" alt="image-20200130175315633" style="zoom:50%;" />

* k 为-1 表示线性规划无解
* k 为 0 表示无有限最优解
* k=1 表示存在有限最优解。 

若 k=1，程序需要继续输出 z（算法求得的目标函数值）和$x_1,x_2,\cdots,x_n$对应的决策变量取 

值。



# 三. 程序使用说明

在 Mac 终端下可使用命令(`./simplex`), 直接运行可执行文件 simplex，然 后便可输入数据，程序返回结果。

或者直接输入命令`./simplex < data/data_1.txt `将测试文件(如 data_1.txt)输 入，程序直接返回结果。



**编译选项**

若要自己编译c++文件，则可直接在命令行下执行指令

```powershell
make run
```

清除生成的可执行文件

```
make clean
```



