- [UninitObjChecker](UninitObjChecker.md)
      - [目标缺陷](#目标缺陷)
  - [UninitObjChecker工作流程](#uninitobjchecker的工作流程)
    - [内存模型](#内存模型)
    - [设计](#设计)
    - [相关函数](#相关函数)
      - [checkEndFunction](#checkendfunction)
      - [checkDeadSymbol](#checkdeadsymbols)
    - [相关处理函数](#相关处理函数)
      - [will_obj_analyze_later](#will_obj_analyzed_later)
      - [get_constructed_region](#get_constructed_region)
      - [is_union_obj_uninit](#is_union_obj_uninit)
      - [is_non_union_obj_uninit](#is_non_union_obj_uninit)
      - [is_reference_obj_uninit](#is_reference_obj_uninit)
      - [is_primitive_obj_uninit](#is_primitive_obj_uninit)


## UninitObjChecker

如果对本Checker有任何问题, 可以通过171860634@smail.nju.edu.cn联系我

##### 模块文件

src/core/checker/UninitObjChecker/*

- UninitializedObjectChecker.h
- UninitializedObjectChecker.cpp
- UninitObjCheckerUtil.h
- UninitObjCheckerUtil.cpp

###### 目标缺陷

检测结构体中在执行构造函数后未初始化的成员
不仅限于最渐层的未初始化字段，分析字段的子子字段

### UninitObjChecker的工作流程

#### 内存模型

结构体下字段成员的内存区域为`FieldRegion`，我们可以链或树的形式来表示结构体下所有的内存区域，再分别检测不同类型的成员是否未初始化

#### 设计

`FieldNode`: 用这种抽象接口类来包装`FieldRegion`，存储关于`FiedlRegion`额外的信息

- `RegularFieldNode`: 继承于`FieldNode`, 表示基本类型的`FieldRegion`.
- `BaseClassNode`: 继承于`FieldNode`,并不用来包装`FieldRegion`，用来描述其他两种`FieldRegion`之间的关系
- `LocFieldNode`: 继承于`FieldNode`,存储`pointer`和`pointee`类型的`FieldRegion`
- `NeedsCastLocFieldNode`: 继承于`FieldNode`,存储需要重新转换成正确的动态类型
- `CyclicLocFieldNode`: 继承于`FieldNode`, 存储表示循环指针类型
  

`FieldListMgr`: 将检查的结构体域表示的`FieldNode`以链状表示，方便对结构体的成员字段的遍历和检测

`FindUninitializedFields`: 建立`FieldListMgr`, 判断各种类型字段的是否未初始化

将Analyzed region 看作 directed tree，成员对象看作树节点Node
节点可能是以下类型，所有树叶节点会是数组、原始数据类型、空指针或未定义指针
- a union
- a non-union record
- dereferenceable
- an array
- a primitive type


#### 相关函数

###### checkEndFunction

在结构体构造函数的路径执行完之后调用，根据上下文分析获取构造函数后的结构体内存区域进行分析

###### checkDeadSymbols

在分析函数调用时开始调用，在分析时去除在函数路径未被使用的内存域等同于去除未被调用的结构体成员字段

#### 相关处理函数

###### will_obj_analyzed_later

检查此字段成员域是否会存在在另一个对象里，避免重复检查

###### get_constructed_region

返回结构体成员字段声明的内存区域

###### is_union_obj_uninit

判断Union类型成员字段或其子字段是否未初始化

###### is_non_union_obj_uninit

判断non-union 类型成员字段或其子字段是否未初始化

##### is_reference_obj_uninit

判断引用类型或其指向的字段是否未初始化

##### is_primitive_obj_uninit

判断原始数据类型的字段是否未初始化

##### dereference

解引用`FieldRegion`, 并返回其指向的内存区域，并判断其是否需要转换成正确的类型







