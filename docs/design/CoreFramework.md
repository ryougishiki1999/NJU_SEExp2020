- [核心框架设计](#核心框架设计)
  - [本框架是如何工作的？](#本框架是如何工作的)
    - [设计](#设计)
    - [架构](#架构)
  - [相关模块文档](#相关模块文档)
    - [StaticAnalyzer](#staticanalyzer)
    - [CSADriver](#csadriver)
      - [CSACheckerRegistry](#csacheckerregistry)
      - [CSAAction](#csaaction)
      - [CSAFactory](#csafactory)
      - [CSADiagnostics](#csadiagnostics)
    - [ResultCollector](#resultcollector)
      - [Diagnostic](#diagnostic)
      - [Stats](#stats)
    - [AnalyzerOption](#analyzeroption)
    - [CommandLineParser](#commandlineparser)
    - [Event](#event)
    - [HiSAInstance](#hisainstance)
## 核心框架设计

以下简称Clang Static Analyzer为CSA

### 本框架是如何工作的？

#### 设计 
本框架设计目标：
+ 将CSA作为本项目的库进行使用，而不是将本项目作为CSA的插件使用
+ 尽可能封装CSA运行时内部API，以降低Clang整体学习成本，使得后续开发可以专注于缺陷检测算法逻辑，而非Clang运行时实现细节
+ Make Life Easier

本框架负责：
+ 构造完整Clang Instance
+ 本项目与CSA接口间适配
+ 解决`Checker`依赖
+ 由本软件运行时陷入CSA以及从CSA逃逸至本软件运行时

本框架不负责：
+ 封装CSA符号执行引擎相关类（如上下文环境、程序状态、约束等）
+ 提供任何CSA不提供的分析能力

#### 架构
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
+----------------------+           +----------------------+   +----------+
|                      |  Checkers |                      |   | Analyzer |
|   CSA  Frontend      |<----------|       CSADriver      |<--| Options  |
|                      |  Options  |                      |   |          |
+----------------------+           +----------------------+   +----------+
           |       ^                    ^         |                 ^           +----------------------+
           |       |       Diagnostics  |         |                 |           |                      |
           |       |       +------------+         +-----------------|---------->|   Result Collector   |
           |       |       |                                        |           |                      |
           |       +-------|-------------------+                    |           +----------------------+
           v               |                   |                    |
+----------------------+   |       +----------------------+    +---------+
|                      |   |       |                      |    | Command |
|   CSA  Core Engine   |---+       |   Clang  Instance    |    | Line    |
|                      |           |                      |    | Parser  |
+----------------------+           +----------------------+    +---------+
                                               ^                    |
                                               | CompileOptions <---+
                                               | SourcePaths <------+
                                   +----------------------+
                                   |                      |
                                   |    StaticAnalyzer    |
                                   |                      |
                                   +----------------------+
```

### 相关模块文档
#### StaticAnalyzer

`StaticAnalyzer`负责完成整个缺陷检测的驱动，具体而言，其需要：
1. 正确构造符合`libTooling`API规范的`ClangTool`
2. 根据用户配置，正确的传递编译选项和待处理文件给`ClangTool`
3. 将用户关于本软件的配置选项传递给`CSADriver`
4. 将准备完成的`CSADriver`注册为`ClangTool`的运行行为
5. 运行`ClangTool`，使软件陷入Clang运行时环境

#### CSADriver

`CSADriver`为本项目接口对Clang内部API的适配器，负责`Checker`的注册、将本软件配置注入CSA Framework和CSA符号执行引擎的驱动。

##### CSACheckerRegistry

单例类，负责注册、管理本项目`Checker`，并将`Checker`注册进CSA实例，移交管理权至CSA

##### CSAAction

负责创建CSA运行时、开启统计计时并将统计信息传给`ResultCollector`、将CSA的Checker管理器、注册器传递给`CSACheckerRegistry`并正确的启用和配置CSA执行选项

##### CSAFactory

`CSAAction`的工厂类，负责`AnalyzerOptions`的传递

##### CSADiagnostics

负责获取CSA的缺陷报告，并将其处理生成为`Diagnostic`传给`ResultCollector`

#### ResultCollector

`ResultCollector`作为单例类负责管理所有的缺陷报告信息及统计信息，是本框架对外输出的统一数据接口

##### Diagnostic

保存有缺陷报告相关信息的数据结构

##### Stats

保存有针对翻译单元的统计信息的数据结构

#### AnalyzerOption

负责管理用户对于缺陷检测的配置。

#### CommandLineParser

负责解析命令行。

#### Event

在运行时发生事件，用户可通过使用事件回调函数来获取静态分析器的内部状态

#### HiSAInstance

类似于ClangTool，提供统一访问接口