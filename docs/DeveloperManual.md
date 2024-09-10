# 开发者手册
- [开发者手册](#开发者手册)
    - [框架](#框架)
      - [设计原则](#设计原则)
      - [整体架构](#整体架构)
      - [使用](#使用)
        - [创建新的Checker](#创建新的checker)
        - [创建新的命令行指令](#创建新的命令行指令)
        - [创建新的事件](#创建新的事件)
    - [实用工具](#实用工具)
        - [将HiSA作为库进行调用](#将hisa作为库进行调用)

以下简称Clang Static Analyzer为CSA，相关接口见API文档

### 框架

#### 设计原则
+ 屏蔽Clang繁琐的调用流程，使得在基于本框架开发时仅需要CSA相关知识
+ Make Life Easier

#### 整体架构
```
+----------------------+           +----------------------+
|                      |           |                      |
| CSA Builtin Checkers |           | Our Project Checkers |
|                      |           |                      |
+----------------------+           +----------------------+
           |                                   |
           X                                   |
           |                                   |
           v                                   v
+----------------------+           +----------------------+         +----------+
|                      |  Checkers |                      |         | Analyzer |
|   CSA  Frontend      |<----------|       CSADriver      |<--------| Options  |
|                      |  Options  |                      |         |          |
+----------------------+           +----------------------+<):Event +----------+
           |       ^                    ^         |                   ^         +----------------------+
           |       |       Diagnostics  |         |                   |         |                      |
           |       |       +------------+         +-------------------|-------->|   Result Collector   |
           |       |       |                                          |         |                      |
           |       +-------|-------------------+                      |         +----------------------+
           v               |                   |                      |
+----------------------+   |       +----------------------+      +---------+
|                      |   |       |                      |      | Command |
|   CSA  Core Engine   |---+       |   Clang  Instance    |      | Line    |
|                      |           |                      |      | Parser  |
+----------------------+           +----------------------+      +---------+
                                               ^                      |
                                               | CompileOptions <-----+
                                               | SourcePaths <--------+
                                   +----------------------+
                                   |                      |
                                   |    StaticAnalyzer    |
                                   |                      |
                                   +----------------------+
```
#### 使用

##### 创建新的Checker

若没有CSA相关知识，建议在创建Checker前了解[CSA 内存模型](http://lcs.ios.ac.cn/~xzx/memmodel.pdf)及[CSA Guide](https://github.com/haoNoQ/clang-analyzer-guide/releases/download/v0.1/clang-analyzer-guide-v0.1.pdf)
1. 按照CSA规范，编写自己的XXXChecker.cpp,XXXChecker.h
2. 在.cpp中使用`CSA_GENERATE_REGISTER_FUNCTION_DEF(yourcheckerclassname)`定义注册函数
3. 在.h中使用`CSA_GENERATE_REGISTER_FUNCTION_DECL(yourcheckerclassname)`声明注册函数
4. 在分析文件之前，调用`CSACheckerRegistry::register_checker()`来注册自己的`Checker`

```c++
   // MyChecker.h
   class MyChecker: public clang::ento::Checker<...>
   {
       ...
   };
   CSA_GENERATE_REGISTER_FUNCTION_DECL(MyChecker)

   // MyChecker.cpp

   MyChecker::... // implementation of MyChecker

   CSA_GENERATE_REGISTER_FUNCTION_DEF(MyChecker)

   // main.cpp
   // before analysis
   CSACheckerRegistry::instance().register_checker("MyChecker",CSA_REGISTER_FUNCTION_NAME(MyChecker),"my bug");
```

##### 创建新的命令行指令

1. 创建新指令对应的回调函数，其类型为`int (CommandLineParser &)`
2. 在`CommandLineParser::command_map`中添加映射，包含命令名、命令行为描述及回调函数

回调函数可以自由访问`CommandLineParser`，这意味着你可以通过添加数据成员到`CommandLineParser`类中，然后在回调函数中去填充该数据成员。同时，对于子命令或带参数的命令，在回调函数中可以通过`get_next_arg()`来获取当前命令的下一个子命令或参数，当当前命令处理完时，`get_next_arg()`返回空字符串。**注意子命令或参数不能与现有命令冲突**
```c++
   // CommandLineParser.cpp
    static int newcmd_callback(CommandLineParser &parser)
    {
        std::cout << "Just a new command.";
        return 0;
    }

    // add new command
    const std::unordered_map<std::string, std::pair<std::string, CommandLineParser::CommandCallbackType>>
    CommandLineParser::command_map = {
        ...
        {"-newcmd", {"a new command", newcmd_callback}}};
```

##### 创建新的事件

1. 在`Event`类中添加新的`EventType`
2. 编写自己新的`XXXEvent`类，并继承自`Event`，构造时使用自身的`EventType`
3. 在合适的位置，调用`Callback`

```c++
    // Event.h
    enum Event::EventType{
        ...
        MyEventType,
        ...
        EventCount,
    };
    // MyEvent.h
    class MyEvent : public Event
    {
    public:
        MyEvent(): Event(MyEventType){}
    };
    // somewhere, assume we have EventCallbackTable event_table.
    event_table.callback(std::make_unique<MyEvent>());
```

### 实用工具

##### 将HiSA作为库进行调用

我们提供了与ClangTool类似的接口HiSAInstance

1. 调用`HiSAInstance::global_init()`以进行全局数据初始化，包括Checker的注册
2. 推荐通过与命令行一致的方式，调用`HiSAInstance::run(int argc,char *argv[])`

```c++
    // main.cpp
    EventCallbackTable event_table;
    event_table.set_callback(...);// set your callback for specific event
    HiSAInstance instance(event_table);
    instance.run(argc, argv);
```