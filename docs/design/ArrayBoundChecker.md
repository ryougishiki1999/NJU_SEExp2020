- [ArrayBoundChecker](#arrayboundchecker)
      - [模块文件](#模块文件)
        - [目标缺陷](#目标缺陷)
  - [ArrayBoundChecker的工作流程](#arrayboundchecker的工作流程)
    - [设计](#设计)
    - [相关知识](#相关知识)
    - [相关函数](#相关函数)
        - [checkLocation](#checklocation)

## ArrayBoundChecker

    如果对本Checker有任何问题，你可以通过171860634@smail.nju.edu.cn

##### 模块文件

    src/core/checker/ArrayBoundChecker.h
    src/core/checker/ArrayBoundChecker.cpp
    
###### 目标缺陷

+ 检查数组越界
+ 检查缓冲区溢出

### ArrayBoundChecker的工作流程

#### 设计

大体思路十分简单
对于数组的检查逻辑比较简单，判断获取的MemRegion是否是ElementRegion，然后获取当前所访问的ElementRegion的index是否处于ElementRegion的SuperRegion的index区间内

专门对结构体/类数组进行了判断，特殊判断获取的MemRegion是否是FieldRegion,在获取其SuperRegion，再进行普通检查逻辑的流程

#### 相关知识

CSA对于指针算术运算会使用当前指针指向的MemRegion作为SuperRegion,然后创建一个新的ElementRegion

CSA对数组到指针的转换是通过`ArrayToPointerDecay`的类型出现，`ArrayToPointerDecay`类型转换最后是通过生成新的ElementRegion来解决的。

检查逻辑基于"`&index < &length`",解决问题很表层，会出现一些情况的误报

由于CSA引擎的限制，过程间或约束求解无法判断

#### 相关函数

###### checkLocation

本checker以检查点`checkLocation()`作为核心，`checkLocation()`函数在对内存的读写进行模拟执行时被调用。

`checkLocation()`会被`evalLocation`调用; 判断当前的region类型是否为`ElementRegion`或`FieldRegion`，再获取其`SuperRegion`的element个数，判断获取的`index`是否处于合法区间。