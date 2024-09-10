- [MemoryChecker](#memorychecker)
        - [目标缺陷](#目标缺陷)
  - [本Checker是如何工作的？](#本checker是如何工作的)
    - [所需要理解的CSA相关知识](#所需要理解的csa相关知识)
    - [设计](#设计)
    - [内存模型](#内存模型)
    - [相关函数](#相关函数)
        - [checkNewAllocator](#checknewallocator)
        - [checkLocation](#checklocation)
        - [checkDeadSymbols](#checkdeadsymbols)
        - [checkPreCall](#checkprecall)
        - [checkPostCall](#checkpostcall)
        - [checkPostStmt](#checkpoststmt)
        - [checkPreStmt](#checkprestmt)
        - [checkEndFunction](#checkendfunction)
        - [evalAssume](#evalassume)
    - [相关处理函数](#相关处理函数)
        - [handle_new](#handle_new)
        - [handle_allocation](#handle_allocation)
        - [handle_deallocation](#handle_deallocation)
        - [report_memory_leak](#report_memory_leak)
## MemoryChecker

本Checker为受`CSA MallocChecker`启发而来

###### 目标缺陷
+ Memory Leak
+ Double-free
+ Use-after-free

### 本Checker是如何工作的？

#### 所需要理解的CSA相关知识

[CSA 内存模型](http://lcs.ios.ac.cn/~xzx/memmodel.pdf)  
[CSA Guide](https://github.com/haoNoQ/clang-analyzer-guide/releases/download/v0.1/clang-analyzer-guide-v0.1.pdf)

#### 设计

对于内存相关缺陷，我们需要跟踪的是
+ 内存分配释放
+ 指针解引用、作为参数传递、作为返回值
+ 指针赋值、算术操作

其中由于指针赋值、算术操作等产生的别名已经由CSA的符号执行引擎处理好，故我们只需要跟踪
+ 内存分配释放
+ 指针解引用、作为返回值

#### 内存模型

在分析时，实际跟踪的是内存区域的状态，即通过对被分析指针当前指向的内存区域的状态来判断是否存在缺陷，这使得处理非常简单，指针只需要保存自己指向的内存区域的位置，在更新内存区域的状态时，不需要更新所有指向该区域的指针的状态。

#### 相关函数
###### checkNewAllocator
本回调对new分配完内存，调用构造函数前执行。其内部调用`handle_new()`来标记分配的内存为`allocated`状态

###### checkLocation
本回调在对指针使用的时候执行，若使用的指针指向的内存区域为`deallocated`，那么存在use-after-free缺陷

###### checkDeadSymbols
本回调在CSA Core Engine无法继续追踪某些符号时执行，在此函数中若死亡符号对应的内存区域为`allocated`状态，那么存在memory leak缺陷

###### checkPreCall
本回调在函数调用前执行，在本函数中，我们对除`deallocation`函数外的所有函数的参数进行检查，若参数对应符号对应的内存区域为`deallocated`状态，那么存在use-after-free缺陷，若函数调用是类方法调用，那么若`this`指针指向的内存区域为`deallocated`状态，则同样存在use-after-free缺陷

###### checkPostCall
本回调函数在函数调用后执行，在本函数中，我们对`allocation`族函数和`deallocation`族函数进行检查，对于`allocation`族函数，我们调用`handle_allocation()`来更新其内存状态，对于`deallocation`族函数，我们调用`handle_deallocation()`来更新其内存状态并检测double-free缺陷

###### checkPostStmt
本回调函数在语句执行后执行，在本函数中，对于`new_expr`我们调用`handle_new()`来更新其内存状态

###### checkPreStmt
本回调函数在语句执行前执行，在本函数中，对于`delete_expr`我们调用`handle_deallocation()`来更新其内存状态并检测double-free缺陷，对于`return`语句，我们认为返回`deallocated`的内存区域是use-after-free缺陷

###### checkEndFunction
同return语句处理

###### evalAssume
本回调函数用于condition时进行状态更新，在本函数中，我们移除对所有空指针的追踪

#### 相关处理函数

###### handle_new
本函数用于将符号对应的内存区域状态更新为`allocated`，符号本身应该是一个`Location`

###### handle_allocation
本函数创建一个代表内存区域的符号，并令其状态为`allocated`，该符号是与allocation site绑定的

###### handle_deallocation
本函数将符号对应内存区域由`allocated`状态更新为`deallocated`状态，若原先状态为`deallocated`，则此次是重复释放，存在double-free缺陷

###### report_memory_leak
memory leak在`deadsymbols`处被检测，需要对`ExplodedGraph`向前查找相应的`allocation site node`获取导致缺陷的指针并报告。