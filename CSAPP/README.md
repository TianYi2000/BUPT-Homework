本项目为《编译原理与技术》课程实验

作者：胡天翼

实验环境：

1.	硬件：学生个人电脑（x86-64）
2.	软件：Windows 10，VMware Workstation 15 Player，32位Linux-Ubuntu 16.04.1
3.	gcc-3.4编译环境
4.	GDB调试工具

---

#### 一、LINUX环境和GCC工具链

现有int型数组a[i]=i-50,b[i]=i+y，其中y取自于学生本人学号2018211x*y的个位。登录bupt1服务器，在linux环境下使用vi编辑器编写C语言源程序，完成数组a+b的功能，规定数组长度为100，函数名为madd（），数组a，b均定义在函数内，采用gcc编译该程序（不使用优化选项），

1. 使用objdump工具生成汇编程序，找到madd函数的汇编程序，给出截图；

2. 用gdb进行调试，练习如下gdb命令，给出截图；gdb、file、kill、quit、break、delete、clear、info break、run、continue、nexti、stepi、disassemble、list、print、x、info reg、watch

3. 找到a[i]+b[i]对应的汇编指令，指出a[i]和b[i]位于哪个寄存器中，给出截图；

4. 使用单步指令及gdb相关命令，显示a[xy]+b[xy]对应的汇编指令执行前后操作数寄存器十进制和十六进制的值，其中x，y取自于学生本人学号2018211x*y的百位和个位。

学号2018211999，a[99]+b[99]单步执行前后的参考截图如下（实际命令未显示出）：

![](https://s2.loli.net/2022/02/04/cnKsaWJlYu1E87w.png)

---

#### 二、拆解二进制炸弹

登录bupt1服务器，在home目录下可以找到Evil博士专门为你量身定制的一个bomb，当运行时，它会要求你输入一个字符串，如果正确，则进入下一关，继续要求你输入下一个字符串；否则，炸弹就会爆炸，输出一行提示信息并向计分服务器提交扣分信息。因此，本实验要求你必须通过反汇编和逆向工程对bomb执行文件进行分析，找到正确的字符串来解除这个的炸弹。

本实验通过要求使用课程所学知识拆除一个“binary bombs”来增强对程序的机器级表示、汇编语言、调试器和逆向工程等方面原理与技能的掌握。 “binary bombs”是一个Linux可执行程序，包含了5个阶段（或关卡）。炸弹运行的每个阶段要求你输入一个特定字符串，你的输入符合程序预期的输入，该阶段的炸弹就被拆除引信；否则炸弹“爆炸”，打印输出 “BOOM!!!”。炸弹的每个阶段考察了机器级程序语言的一个不同方面，难度逐级递增。 

为完成二进制炸弹拆除任务，需要使用gdb调试器和objdump来反汇编bomb文件，可以单步跟踪调试每一阶段的机器代码，也可以阅读反汇编代码，从中理解每一汇编语言代码的行为或作用，进而设法推断拆除炸弹所需的目标字符串。实验2的具体内容见实验2说明。

---

#### 三、The Attack Lab

This assignment involves generating a total of five attacks on two programs having different security vulnerabilities. Outcomes you will gain from this lab include:

- You will learn different ways that attackers can exploit security vulnerabilities when programs do not safeguard themselves well enough against buffer overflows.

- Through this, you will get a better understanding of how to write programs that are more secure, as well as some of the features provided by compilers and operating systems to make programs less vulnerable.

- You will gain a deeper understanding of the stack and parameter-passing mechanisms of x86-64 machine code.

- You will gain a deeper understanding of how x86-64 instructions are encoded.

- You will gain more experience with debugging tools such as GDB and OBJDUMP.

Note: In this lab, you will gain firsthand experience with methods used to exploit security weaknesses in operating systems and network servers. Our purpose is to help you learn about the runtime operation of programs and to understand the nature of these security weaknesses so that you can avoid them when you write system code. We do not condone the use of any other form of attack to gain unauthorized access to any system resources. You will want to study Sections 3.10.3 and 3.10.4 of the CS:APP3e book as reference material for this lab. 

---

#### 四、键盘驱动程序的分析与修改

1.理解I/O系统调用函数和C标准I/O函数的概念和区别；

2.建立内核空间I/O软件层次结构概念，即与设备无关的操作系统软件、设备驱动程序和中断服务程序；

3.了解Linux-0.11字符设备驱动程序及功能，初步理解控制台终端程序的工作原理；

4.通过阅读源代码，进一步提高C语言和汇编程序的编程技巧以及源代码分析能力；

5.锻炼和提高对复杂工程问题进行分析的能力，并根据需求进行设计和实现的能力。