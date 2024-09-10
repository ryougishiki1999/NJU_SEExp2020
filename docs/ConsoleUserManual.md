- [通过二进制文件安装hisa](#通过二进制文件安装hisa)
  - [Windows](#windows)
  - [Linux](#linux)
- [从源码构建hisa](#从源码构建hisa)
  - [Windows](#windows-1)
  - [Linux](#linux-1)
- [命令行](#命令行)
  - [命令列表](#命令列表)
  - [使用示例](#使用示例)
      - [我只想打印帮助信息或现有的检查器列表](#我只想打印帮助信息或现有的检查器列表)
      - [我想检测我的项目源码，它位于/proj/src](#我想检测我的项目源码它位于projsrc)
      - [但我不想检测所有的文件，只想检测其中几个源文件](#但我不想检测所有的文件只想检测其中几个源文件)
      - [我还希望能输出全面的信息](#我还希望能输出全面的信息)
      - [可我只想检测特定的缺陷](#可我只想检测特定的缺陷)
      - [我是Checker的开发者，我该如何debug我的Checker?](#我是checker的开发者我该如何debug我的checker)
      - [我想使用Clang Static Analyzer](#我想使用clang-static-analyzer)
- [交互式命令行环境REPL](#交互式命令行环境repl)
  - [命令列表](#命令列表-1)
  - [使用示例](#使用示例-1)
  - [行为约定](#行为约定)

## 通过二进制文件安装hisa

### Windows

在windows已经安装clang/llvm的前提下，下载二进制文件压缩包后，将exe可执行文件解压缩后即可运行。

### Linux

下载二进制文件压缩包后，解压缩后将hisa二进制文件移动至与clang同一目录下，如果是默认路径通常是/usr/local/bin，参考命令````sudo cp console/hisa /usr/local/bin````

## 从源码构建hisa

### Windows

1. 所需工具
   - Git
   - CMake
   - Visual Studio
2. 从源码构建Clang和LLVM，见[官方文档](http://clang.llvm.org/get_started.html)
   - cmake的`-G`选项需对应自身VS版本
   - 只需要编译clang libraries和llvm libraries即可
3. 在本项目下cmake -D LOCAL_LLVM_BUILDPATH=your_llvm_build_path

### Linux

1. 从源码构建Clang和LLVM，见[官方文档](http://clang.llvm.org/get_started.html) 

2. 编译hisa

   - 开始编译我们的项目hisa，同样从github/gitee下载一份我们的最新代码，解压后放在共享文件夹中，在移入虚拟机的桌面或其他文件夹。

   - 进入我们的项目SEExp2020目录，执行以下命令行指令：

     ```bash
     cd src
     cmake -D LOCAL_LLVM_BUILDPATH=/usr/local/lib/cmake -DCMAKE_BUILD_TYPE=Release .
     make -j4
     ```

   - 这时我们的hisa二进制文件应该已经位于src目录中了，但是由于clang tools的特性，我们的hisa文件必须与clang保持同一目录下，因此执行`sudo cp console/hisa /usr/local/bin`

   - 如果你是通过下载二进制文件获取我们程序的，只要直接将我们程序放置于clang同一目录下即可，如果按照默认构建过程，这个位置往往是```/usr/local/bin```.

3. compile_commands.json文件

   - 由于我们的软件是基于clang tools实现的，需要为我们的程序提供供于clang前端编译使用的compilation database。
   - compilation database是一个叫做``` compile_commands.json```的JSON格式文件，其中包含了项目中每个编译单元（一般为文件）编译所需要的结构化数据。
   - compilation database的生成可以参见文档：[如何生成Compilation Database](https://sarcasm.github.io/notes/dev/compilation-database.html#how-to-generate-a-json-compilation-database)
   - 下面列举几个工具的简单用法：
     - CMake：使用CMAKE_EXPORT_COMPILE_COMMANDS标志，你可以运行```cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON...```或者添加下面语句到你的CMakeLists.txt中：```set(CMAKE_EXPORT_COMPILE_COMMANDS ON)```之后compile_commands.json文件就会出现在你构建的路径中
     - [compiledb](https://github.com/nickdiego/compiledb)工具可以为基于makefile构建的项目创造compile_commands.json文件，使用方式为```compiledb make ...```
     - [Bear](https://github.com/rizsotto/Bear)/intercept-build from [scan-build](https://github.com/rizsotto/scan-build)可以通过接入make构建进程的方式生成compile_commands.json文件，使用方式为：```bear/intercept-build make ...```

4. 运行hisa

   - 这时我们就可以在任意目录下运行我们的hisa了，不过由于我们基于clang实现的特性，需要为我们的程序提供compile_commands.json文件，具体生成方式见4
   - 以我们提供的test为例，在CMakeList.txt所在目录下执行`cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .`即可在该目录下生成该项目的compile_commands.json，这是cmake项目生成compile_commands.json的例子。
   - 对于makefile构建的项目，在我们安装了compiledb后，可以使用compiledb插入make过程中以生成compile_commands.json文件，注意使用compiledb生成compile_commands.json时，应先将文件夹中已存在的compile_commands.json文件删去。对于makefile存在的文件夹，输入指令`compiledb make`即可生成compile_commands.json。由于cmake项目也是通过生成makefile来完成源码编译的，因此cmake项目同样可以使用这种方法构造compile_commands.json文件，同时可以避免一些文件会包含只有在make后才会生成的文件所导致的编译错误
   - 打开命令行界面并输入hisa以及必要的命令行参数，就可以执行我们的程序了。

## 命令行

### 命令列表

|                Command                |                                   Description                                   |
| ------------------------------------- | ------------------------------------------------------------------------------- |
| -help                                 | 打印帮助信息                                                                    |
| -g                                    | 开启god mode                                                                    |
| -debug                                | 开启debug mode                                                                  |
| -debug --output-path path             | 指定生成的ExplodedGraph的文件路径，仅debug模式下有效                            |
| -debug --trim-exploded-graph          | 只打印缺陷报告的路径，仅debug模式下有效                                         |
| -debug --show-analysis-progress       | 打印分析的进度，仅debug模式下有效                                               |
| -debug --exploded-graph               | 打印ExplodedGraph，仅debug模式下有效                                            |
| -input file1 file2 ...                | 指定输入文件，若留空则检查compile_commands.json内所有源文件                     |
| -compilation-database-path path       | 指定compile_commands.json路径                                                   |
| -output --path path                   | 指定输出路径                                                                    |
| -output --format format1 format2 ...  | 指定输出格式，目前支持json,console,text,html                                    |
| -output --info opt1 opt2 ...          | 指定输出包含的信息，目前支持time,checker,desc,stats，若指定time则默认添加-stats |
| -enable-checker checker1 checker2 ... | 指定开启的检查器，若留空则禁用所有检查                                          |
| -checker-list                         | 列举现有支持的检查器                                                            |

### 使用示例

##### 我只想打印帮助信息或现有的检查器列表

```
hisa -help //打印帮助信息
hisa -checker-list //打印现有的检查器列表
```

##### 我想检测我的项目源码，它位于/proj/src

在检测项目前，必须向HiSA指定其编译数据库以保证HiSA正确运行，假设compile_commands.json位于/proj/build目录下

```
hisa -compilation-database-path /proj/build
```
##### 但我不想检测所有的文件，只想检测其中几个源文件

可以使用`-input`，但请确保源文件存在于compile_commands.json中

```
hisa -compilation-database-path /proj/build -input /proj/src/a.cpp /proj/src/b.cpp ...
```

##### 我还希望能输出全面的信息

假设输出目录为/proj/output

```
hisa -compilation-database-path /proj/build -input /proj/src/a.cpp /proj/src/b.cpp -output --path /proj/output --format console text json html --info time checker desc stats
```

##### 可我只想检测特定的缺陷

以内存错误和除零错误为例

```
hisa -compilation-database-path /proj/build -input /proj/src/a.cpp /proj/src/b.cpp -output --path /proj/output --format console text json html --info time checker desc stats -enable-checker hisa.MemoryChecker hisa.DivZeroChecker
```

##### 我是Checker的开发者，我该如何debug我的Checker?

将ExplodedGraph保存至/proj/dbg_report.dot
```
hisa -compilation-database-path /proj/build -input /proj/src/a.cpp /proj/src/b.cpp -debug --output-path /proj/dbg_report.dot --trim-exploded-graph
```

##### 我想使用Clang Static Analyzer

```
hisa -compilation-database-path /proj/build -input /proj/src/a.cpp /proj/src/b.cpp -g
```

## 交互式命令行环境REPL

可以通过无参数的运行程序以进入交互式环境。

### 命令列表

| Command |           Sub  Command            |                          Description                           |
| ------- | --------------------------------- | -------------------------------------------------------------- |
| help    |                                   | 打印帮助信息                                                   |
| quit    |                                   | 退出程序                                                       |
| info    | help                              | 打印info命令的帮助信息                                         |
| info    | analyzer-opt                      | 获取Analyzer Options                                           |
| info    | output-opt                        | 获取Output Options                                             |
| info    | checker                           | 获取Checker List及其状态                                       |
| info    | src                               | 获取当前将会分析的文件列表                                     |
| info    | available-src                     | 获取当前可进行分析的文件列表                                   |
| set     | help                              | 打印set命令的帮助信息                                          |
| set     | debug-output-path path            | 设置生成的ExplodedGraph的文件路径，仅debug模式下有效           |
| set     | compilation-database path         | 设置compile_commands.json路径                                  |
| set     | output-format format1 format2 ... | 设置输出格式，目前支持json,console,text,html                   |
| set     | output-path path                  | 设置输出路径                                                   |
| enable  | help                              | 打印enable命令的帮助信息                                       |
| enable  | gmode                             | 开启God Mode                                                   |
| enable  | analyzer-stats                    | 开启分析器统计                                                 |
| enable  | debug                             | 开启Debug Mode                                                 |
| enable  | debug-analysis-progress           | 开启分析进度显示（默认开启Debug Mode）                         |
| enable  | debug-exploded-graph              | 开启Exploded Graph打印（默认开启Debug Mode）                   |
| enable  | debug-trim-exploded-graph         | 开启对Exploded Graph路径裁剪（默认开启Debug Mode）             |
| enable  | output-time                       | 开启时间信息输出                                               |
| enable  | output-checker                    | 开启Checker信息输出                                            |
| enable  | output-description                | 开启缺陷描述信息输出                                           |
| enable  | output-stats                      | 开启统计信息输出                                               |
| enable  | checkername                       | 开启对应Checker                                                |
| disable | help                              | 打印disable命令的帮助信息                                      |
| disable | gmode                             | 关闭God Mode                                                   |
| disable | analyzer-stats                    | 关闭分析器统计                                                 |
| disable | debug                             | 关闭Debug Mode                                                 |
| disable | debug-analysis-progress           | 关闭分析进度显示                                               |
| disable | debug-exploded-graph              | 关闭Exploded Graph打印                                         |
| disable | debug-trim-exploded-graph         | 关闭对Exploded Graph路径裁剪                                   |
| disable | output-time                       | 关闭时间信息输出                                               |
| disable | output-checker                    | 关闭Checker信息输出                                            |
| disable | output-description                | 关闭缺陷描述信息输出                                           |
| disable | output-stats                      | 关闭统计信息输出                                               |
| disable | checkername                       | 关闭对应Checker                                                |
| analyze | analyze \<filename\>              | 运行分析器，若指定文件名则分析指定文件，否则按文件列表进行分析 |
| reset   | help                              | 打印reset命令的帮助信息                                        |
| reset   | analyzer-opt                      | 重置Analyzer Options                                           |
| reset   | output-opt                        | 重置Output Options                                             |
| reset   | checker                           | 重置Checker状态                                                |
| reset   | src                               | 重置文件列表                                                   |
| addsrc  | filename                          | 将文件加入至文件列表                                           |
| delsrc  | filename                          | 从文件列表中移除对应文件                                       |
| show    | help                              | 打印show命令的帮助信息                                         |
| show    | summary                           | 打印上一次分析的总结报告                                       |
| show    | all                               | 打印上一次分析的分析报告，包含各个文件的分析和总结报告         |

### 使用示例

```
./hisa  #run program without argument
hisa>>> set compilation-database compile_commands.json
hisa>>> analyze
hisa>>> show all
hisa>>> quit
```

### 行为约定

1. 当不指定任何文件列表时，默认使用compilation database提供的文件列表
2. 当使用God Mode时，使用的为CSA内建分析器，此时默认开启所有CSA分析器
3. 默认开启所有Checker