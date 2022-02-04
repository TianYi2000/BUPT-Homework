本项目为《编译原理与技术》课程实验

作者：胡天翼

实验环境：

- 操作系统：Windos10
- IDE：Visual Studio 2019（C++17）
- GUI设计工具：Qt 5.14

---

#### 一、[词法分析](https://github.com/TianYi2000/BUPT-Homework/tree/main/Principle_of_Compiler/%E8%AF%8D%E6%B3%95%E5%88%86%E6%9E%90)

题目：C语言词法分析程序的设计与实现
实验内容及要求：
1. 可以识别出用C语言编写的源程序中的每个单词符号，并以记号的形式输出每个单词符号。
2. 可以识别并跳过源程序中的注释。
3. 可以统计源程序中的语句行数、各类单词的个数、以及字符总数，并输出统计结果。
4. 检查源程序中存在的词法错误，并报告错误所在的位置。
5. 对源程序中出现的错误进行适当的恢复，使词法分析可以继续进行，对源程序进行一次扫描，即可
检查并报告源程序中存在的所有词法错误。
实现方法要求：分别用以下两种方法实现：
方法1：采用C/C++作为实现语言，手工编写词法分析程序。（必做）
方法2：编写LEX源程序，利用LEX编译程序自动生成词法分析程序。

---

#### 二、[语法分析](https://github.com/TianYi2000/BUPT-Homework/tree/main/Principle_of_Compiler/%E8%AF%AD%E6%B3%95%E5%88%86%E6%9E%90)

编写语法分析程序，实现对算术表达式的语法分析。要求所分析算数表达式由如下的文法产生。

$$E\to E+T \quad|\quad E–T \quad|\quad T$$

$$T\to T*F \quad|\quad T/F \quad|\quad F$$

$$F\to (E) \quad|\quad num$$

