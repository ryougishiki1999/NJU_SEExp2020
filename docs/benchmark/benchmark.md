# benchmark文档

撰写人：施超烜 1985045185@qq.com

## 相关信息

本benchmark文档包含以下信息：benchmark使用的项目的简要介绍与来源，项目的代码行数，通过应用clang static analyzer得到的分析结果与本项目程序得到的分析结果错误数目的对比。

注1：由于UninitObjChecker对应缺陷并未出现在缺陷名单的21项中，因此本次benchmark并未包含相关的统计信息。

注1：clang static analyzer中对于数组越界进行检查的ArrayBoundChecker处于Alpha包中，这意味着这是clang认为误报率/漏报率较高的checker，通常clang不会开启这个checker，而我们的程序在-g模式下之后执行clang默认开启的checker，如果想要得到与本次benchmark相似的结果，需要在/include/Checkers.td中将ArrayBoundChecker从alpha包移动到core包中，然后重新编译clang/llvm和hisa，使得clang和hisa的-g模式默认能够执行这个checker。

## 项目一：libtnet

来源：gitee上一个开源的小巧高性能C++网络库，地址：https://gitee.com/siddontang/libtnet.git

代码行数：约1wLoc，详见./libtnet/loc.md

分析时间：正常模式120s，-g模式143s

内存峰值：正常模式132M，-g模式136M

与clang static analyzer对比得到的错误数目对比表格如下：

| Checker            | 正常模式 | -g模式 |
| ------------------ | -------- | ------ |
| DivModZeroChecker  | 未出现   | 未出现 |
| DereferenceChecker | 未出现   | 未出现 |
| MemoryChecker      | 11       | 1      |
| ArrayBoundChecker  | 7        | 7      |

## 项目二：libjpeg

来源：github上开源代码libjepg，地址：https://github.com/libjpeg-turbo/libjpeg-turbo

代码行数：约4wLoc，详见./libjpeg/loc.md

分析时间：正常模式419s，-g模式477s

内存峰值：正常模式92M，-g模式99M

与clang static analyzer对比得到的错误数目对比表格如下：

| Checker            | 正常模式 | -g模式 |
| ------------------ | -------- | ------ |
| DivModZeroChecker  | 3        | 3      |
| DereferenceChecker | 25       | 23     |
| MemoryChecker      | 11       | 1      |
| ArrayBoundChecker  | 88       | 90     |

## 项目三：cpython

来源：github上开源代码cpython，地址：https://github.com/python/cpython

代码行数：约45wLoc，详见./cpython/loc.md

分析时间：正常模式2978s，-g模式3485s

内存峰值：正常模式185M，-g模式197M

与clang static analyzer对比得到的错误数目对比表格如下：

| Checker            | 正常模式 | -g模式 |
| ------------------ | -------- | ------ |
| DivModZeroChecker  | 3        | 2      |
| DereferenceChecker | 150      | 152    |
| MemoryChecker      | 5        | 1      |
| ArrayBoundChecker  | 1138     | 1139   |

## 项目四：mysql

来源：github上开源代码mysql，地址：https://github.com/mysql/mysql-server

代码行数：约160wLoc，详见./mysql/loc.md

分析时间：正常模式9679s，-g模式10777s

内存峰值：正常模式244M，-g模式267M

与clang static analyzer对比得到的错误数目表格如下：

| Checker            | 正常模式 | -g模式 |
| ------------------ | -------- | ------ |
| DivModZeroChecker  | 7        | 6      |
| DereferenceChecker | 267      | 180    |
| MemoryChecker      | 181      | 17     |
| ArrayBoundChecker  | 237      | 243    |

