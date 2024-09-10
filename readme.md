## 从源码构建本项目

#### Windows
1. 所需工具
   - Git
   - CMake
   - Visual Studio

2. 从源码构建Clang和LLVM，见[官方文档](http://clang.llvm.org/get_started.html)
   - cmake的`-G`选项需对应自身VS版本
   - 只需要编译clang libraries和llvm libraries即可

3. 在本项目下cmake -D LOCAL_LLVM_BUILDPATH=your_llvm_build_path
#### Linux

1. 虚拟机配置

   - 虚拟机软件：Vmware

   - 操作系统：Ubuntu 18.04
   - 建议配置：内存 8GB 磁盘大小 60GB 处理器 4核

2. 从源码构建Clang和LLVM，见[官方文档](http://clang.llvm.org/get_started.html) ，参考流程如下：

   - 首先在自己的电脑上从github下载clang/llvm的最新源码，不要下载llvm10.0.0，github下载过慢可以通过国内gitee下载

   - 在虚拟机上安装Vmware Tools，并建立共享文件夹，将解压后的llvm-project放到共享文件夹中，然后将llvm-project从共享文件夹移到虚拟机的桌面或任意文件夹中，不要在共享文件夹编译clang/llvm，会有未知bug

   - 安装clang/llvm可能需要的依赖库：git，vim，cmake，autoconf，autogen，automake，python3.7-dev，python3-pip，zlib1g-dev，glib2.0，libpixman-1-dev，flex，bison。（我也不清楚是不是全需要，总之按上就完事了），具体命令行指令可参考如下：

     ```bash
     sudo apt-get update
     sudo apt-get upgrade
     sudo apt-get install -y git vim cmake autoconf autogen automake
     sudo apt-get install -y python3.7-dev python3-pip
     sudo apt-get install -y zlib1g-dev glib2.0 libpixman-1-dev libsdl-dev flex bison
     ```
   
   - 在llvm-project中通过cmake构建clang/llvm，具体命令行指令如下：
   
     ```bash
     mkdir build
     cd build
     cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS=clang ../llvm
     make -j4
     sudo make install
     ```
   
   - 编译完成后通过命令行指令`clang --version`判断是否编译成功，此时clang/llvm相关的文件，包括二进制文件，源代码，cmake文件应该都在/usr/local的下属目录中。
   
3.    编译hisa

   - 开始编译我们的项目hisa，同样从github/gitee下载一份我们的最新代码，解压后放在共享文件夹中，在移入虚拟机的桌面或其他文件夹。

   - 进入我们的项目SEExp2020目录，执行以下命令行指令：

     ```bash
     cd src
     cmake -D LOCAL_LLVM_BUILDPATH=/usr/local/lib/cmake -DCMAKE_BUILD_TYPE=Release .
     make -j4
     ```

   - 这时我们的hisa二进制文件应该已经位于src目录中了，但是由于clang tools的特性，我们的hisa文件必须与clang保持同一目录下，因此执行`sudo cp console/hisa /usr/local/bin`

4. compile_commands.json文件

   - 由于我们的软件时基于clang tools实现的，需要为我们的程序提供供于clang前端编译使用的compilation database。
   - compilation database是一个叫做``` compile_commands.json```的JSON格式文件，其中包含了项目中每个编译单元（一般为文件）编译所需要的结构化数据。
   - compilation database的生成可以参见文档：[如何生成Compilation Database](https://sarcasm.github.io/notes/dev/compilation-database.html#how-to-generate-a-json-compilation-database)
   - 下面列举几个工具的简单用法：
     - CMake：使用CMAKE_EXPORT_COMPILE_COMMANDS标志，你可以运行```cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON...```或者添加下面语句到你的CMakeLists.txt中：```set(CMAKE_EXPORT_COMPILE_COMMANDS ON)```之后compile_commands.json文件就会出现在你构建的路径中
     - [compiledb](https://github.com/nickdiego/compiledb)工具可以为基于makefile构建的项目创造compile_commands.json文件，使用方式为```compiledb make ...```
     - [Bear](https://github.com/rizsotto/Bear)/intercept-build from [scan-build](https://github.com/rizsotto/scan-build)可以通过接入make构建进程的方式生成compile_commands.json文件，使用方式为：```bear/intercept-build make ...```

5. 运行hisa

   - 这时我们就可以在任意目录下运行我们的hisa了，不过由于我们基于clang实现的特性，需要为我们的程序提供compile_commands.json文件，具体生成方式见4
   - 以我们提供的test为例，在CMakeList.txt所在目录下执行`cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .`即可在该目录下生成该项目的compile_commands.json，这是cmake项目生成compile_commands.json的例子。
   - 对于makefile构建的项目，在我们安装了compiledb后，可以使用compiledb插入make过程中以生成compile_commands.json文件，注意使用compiledb生成compile_commands.json时，应先将文件夹中已存在的compile_commands.json文件删去。对于makefile存在的文件夹，输入指令`compiledb make`即可生成compile_commands.json。由于cmake项目也是通过生成makefile来完成源码编译的，因此cmake项目同样可以使用这种方法构造compile_commands.json文件，同时可以避免一些文件会包含只有在make后才会生成的文件所导致的编译错误
   - 打开命令行界面并输入hisa以及必要的命令行参数，就可以执行我们的程序了。

## 其他

- 会议记录，工作安排，需求文档，分析文档，设计文档，测试文档等见docs文件
