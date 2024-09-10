## 项目文件结构

- [项目文件结构](#项目文件结构)
  - [项目源码结构](#项目源码结构)
    - [核心部分](#核心部分)
    - [命令行](#命令行)
    - [GUI](#gui)
  - [项目测试结构](#项目测试结构)
  - [项目文档结构](#项目文档结构)

+ /src 源代码
+ /test 测试样例
+ /docs 文档


### 项目源码结构

项目分为三大部分开发，具体文件说明见API文档

+ /src/core 核心部分，以静态库形式编译并提供
+ /src/console 命令行，以二进制程序形式编译并提供
+ /src/gui GUI，以二进制程序形式编译并提供
  

其中命令行依赖核心库，GUI依赖命令行

#### 核心部分

+ /src/core/checker 检测逻辑实现
+ /src/core CSA适配器
+ /src/core/Json JSON文件处理
+ /src/core/Statistics 结果统计
+ /src/core/tool 核心对外接口
+ /src/core/utility 实用工具

#### 命令行

+ /src/console/REPL 交互式命令行实现

#### GUI

- /src/gui/DirTreeDialog 项目文件树结构展示界面实现
- /src/gui/ExplanationDialog 项目功能解释对话框实现
- /src/gui/History 历史文件读写功能实现
- /src/gui/HistoryDialog 历史记录展示界面实现
- /src/gui/MainWindow 主窗口展示界面实现
- /src/gui/preprocessor 分析预处理器实现(找到项目文件夹中的 compile_commands.json文件并解析出相应的文件树结构) 
- /src/gui/ProcessDialog 项目分析进度展示界面实现

### 项目测试结构

+ /test/DereferenceChecker 空指针解引用相关样例
+ /test/DivModZeroChecker 除模零相关样例
+ /test/MemoryChecker 内存泄漏，Double-Free，Use-After-Free相关样例
+ /test/UninitObjChecker 未初始化对象域相关样例
+ /test/ArrayBoundChecker 数组越界相关样例

### 项目文档结构

+ /docs/design 设计文档
+ /docs/meeting 会议记录
+ /docs/benchmark 性能测试
+ /docs/***test_doc 代码测试