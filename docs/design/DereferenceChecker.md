## DereferenceChekcer

DereferenceChecker相关代码由施超烜编写，如有问题，可通过scxzxczxxc@126.com联系

本Checker受`CSA DereferenceChecker`启发而来

###### 目标缺陷

+ Null Pointer Dereferenced

### 本Checker是如何工作的？

#### CSA相关知识

[CSA 内存模型](http://lcs.ios.ac.cn/~xzx/memmodel.pdf)  
[CSA Guide](https://github.com/haoNoQ/clang-analyzer-guide/releases/download/v0.1/clang-analyzer-guide-v0.1.pdf)

#### 设计

对于空指针解引用缺陷，我们需要跟踪的是

+ 对指针变量的读写操作
+ 使用指针解引用对引用变量的初始化绑定操作
+ 指针变量的赋值与算术操作

其中由于指针的赋值与算术操作等已经由`CSA`的符号执行引擎处理完备，因此我们只需要追踪

+ 对指针变量的读写操作
+ 使用指针解引用对引用变量的初始化绑定操作

##### 相关回调函数

###### checkLocation

本回调函数在对指针进行读写操作时被调用，`state`为当前程序状态，`loc`为当前指针的符号，通过语句`std::tie(not_null_state, null_state) = state->assume(loc);`调用符号执行引擎对`loc`所代表的值进行判断，并将非空和为空的情况之后的程序状态分别赋予`not_null_state`和`null_state`，如果只存在为空情况不存在非空情况，则报告一个空指针解引用错误，如果为空情况和非空情况同时存在，则报告一个可能隐形存在的空指针解引用错误。

###### checkBind

本回调函数在对符号进行绑定值的时候被调用，对于空指针解引用缺陷，只需要检查对引用变量进行初始化绑定的情况，因此需要对被绑定的符号进行类型的判断。经过判断过后，与`checkLocation`的情况类似，通过对进行绑定的值进行非空情况和为空情况的判断，并根据判断结果选择是否报告错误。

##### 相关缺陷报告函数

###### report_direct_dereference

对直接出现的，且一定会发生空指针解引用的缺陷进行报告，并根据错误语句的类型添加必要的描述信息和来源信息

##### 相关辅助函数

###### get_dereference_expr

根据传入的参数`stmt`获取解引用对应的表达式。

###### add_deref_source

根据传入的参数expr，state，lctx和loaded_from分析解引用的来源并将来源信息添加到参数os输入流和参数ranges中

###### is_decl_ref_expr_to_reference

当传入参数`expr`是一个对于引用变量的声明语句时，返回`true`。



