# 工作量统计

## 统计方式

+ 代码及非打印形式文档采用行数统计方式，统计工具为`cloc 1.86`
+ 打印形式文档按页进行统计
+ 空行不计入
+ 自动生成的文档不计入
+ 代码统计为采用统一格式化后的结果，格式化规则见`.clang-format`

#### 文档

+ 代码说明文档 45l(CK8,CHD37)
+ 需求分析说明书 48p(CK)
+ 系统测试报告 202l(CK138,SCX64)
+ 命令行用户手册 178l(CHD138,SCX40)
+ GUI用户手册 74l(CK)
+ 开发者手册 123l(CHD)
+ API文档 见代码中注释相关统计
+ 会议记录 249l(CK)
+ 设计文档 WXY26p,CHD171l,SCX32l,CK94l,ZRH204l
+ Benchmark 54l(SCX)
+ 项目研发报告 162l(SCX)

#### 代码

| 文件名                            | 代码行数 | 注释行数 | 编写者 |
| --------------------------------- | -------- | -------- | ------ |
| console/*                         | 593      | 13       | 陈昊东 |
| core/checker/UninitObjChecker/*   | 768      | 192      | 周若衡 |
| core/checker/ArrayBoundChecker.*  | 111      | 7        | 周若衡 |
| core/checker/DereferenceChecker.* | 194      | 72       | 施超烜 |
| core/checker/DivModZeroChecker.*  | 62       | 4        | 程珂   |
| core/checker/MemoryChecker.*      | 521      | 34       | 陈昊东 |
| core/Json/*                       | 714      | 215      | 吴晓阳 |
| core/Statistics/*                 | 463      | 227      | 周若衡 |
| core/tool/*                       | 107      | 34       | 陈昊东 |
| core/utility/*                    | 25       | 6        | 陈昊东 |
| core/AnalyzerOptions.*            | 49       | 8        | 陈昊东 |
| core/CommandLineParser.*          | 284      | 21       | 陈昊东 |
| core/CSADriver.*                  | 226      | 54       | 陈昊东 |
| core/Event.*                      | 62       | 16       | 陈昊东 |
| core/HtmlPrinter.*                | 1810     | 153      | 施超烜 |
| core/Logger.*                     | 54       | 10       | 陈昊东 |
| core/OutputManager.*              | 565      | 37       | 吴晓阳 |
| core/OutputOptions.*              | 56       | 13       | 陈昊东 |
| core/ResultCollector.*            | 82       | 31       | 陈昊东 |
| core/StaticAnalyzer.*             | 88       | 28       | 陈昊东 |
| gui/DirTreeDialog/*               | 355      | 39       | 程珂   |
| gui/ExplanationDialog/*           | 101      | 18       | 程珂   |
| gui/History/*                     | 102      | 12       | 程珂   |
| gui/HistoryDialog/*               | 391      | 43       | 程珂   |
| gui/Language/*                    | 4        | 0        | 程珂   |
| gui/MainWindow/*                  | 1225     | 110      | 程珂   |
| gui/preprocessor/*                | 540      | 262      | 周若衡 |
| gui/ProcessDialog/*               | 210      | 17       | 程珂   |
| gui/main.cpp                      | 10       | 6        | 程珂   |
| mainpage/mainpage.html            | 1053     | 0        | 吴晓阳 |

|  所属  | 代码行数 | 注释行数 | commit数量 |
| ------ | -------- | -------- | ---------- |
| 项目   | 10825   | 1682  | 441         |
| 施超烜 | 2004     | 225      | 53        |
| 周若衡 | 1882     | 688      | 60          |
| 程珂   | 2460    | 249 | 54         |
| 吴晓阳 | 2332     | 252      | 53         |
| 陈昊东 | 2147     | 268      | 115          |

#### DEBUG

| commit  |                                          说明                                          | 提交者 |                                            URL                                             |
| ------- | -------------------------------------------------------------------------------------- | ------ | ------------------------------------------------------------------------------------------ |
| c6c3fd0 | 修复< > &符号在输出的html文件中消失的问题                                              | 施超烜 | https://github.com/shinkunyachen/SEExp2020/commit/c6c3fd0bc0474f9299a9acf2806ee999eb150fa2 |
| 85cb042 | 修复&符号的转换导致程序死循环的问题                                                    | 施超烜 | https://github.com/shinkunyachen/SEExp2020/commit/85cb0423c5b5e2d18cd353cc5eafe872064fe7d6 |
| bb4af3d | 修复md格式的输出格式不正确的问题                                                       | 施超烜 | https://github.com/shinkunyachen/SEExp2020/commit/bb4af3d09ad6554b718d04001eeffcdff714ff75 |
| 3a8fbb5 | 修复空指针解引用检测对值的要求过于苛刻导致误报率过高的问题                             | 施超烜 | https://github.com/shinkunyachen/SEExp2020/commit/3a8fbb5f74afd555cb0481036b5bc8cd4c4540d1 |
| a7bfffb | 修复了生成的关于头文件的html会被相同名字的关于源代码文件的html覆盖的问题               | 施超烜 | https://github.com/shinkunyachen/SEExp2020/commit/a7bfffbd4638941b6acc20b89ecacf9584173d6a |
| 15f61d0 | 修复了程序由于无法处理C语言#line宏导致无法定位错误位置的问题                           | 施超烜 | https://github.com/shinkunyachen/SEExp2020/commit/15f61d002f71b662f291a2ab9d924d5f7808a95e |
| d2b0955 | 修复了对于delete表达式将Double-Free误报为Use-After-Free的问题                          | 陈昊东 | https://github.com/shinkunyachen/SEExp2020/commit/d2b0955f94ef09a37493c89fde1fdf120d46fd1b |
| 7a15dc3 | 修复了Clang命令行要求提供编译参数的问题                                                | 陈昊东 | https://github.com/shinkunyachen/SEExp2020/commit/7a15dc34d6e555f4aff4ed091804d45c3c926324 |
| 532eaf9 | 修复了StaticAnalyzer在错误状态下返回值不一致的问题                                     | 陈昊东 | https://github.com/shinkunyachen/SEExp2020/commit/532eaf9cc86d8c221de55ed07e0dee3969229781 |
| f8c484d | 修复了ResultCollector获取CheckerName不对的问题                                         | 陈昊东 | https://github.com/shinkunyachen/SEExp2020/commit/f8c484d3dee4e849312b4aeffae3fbd1eb850622 |
| 97e185f | 修复了MemoryChecker在边界条件下可能解引用空指针的问题                                  | 陈昊东 | https://github.com/shinkunyachen/SEExp2020/commit/97e185f910851ed5b13bf4207de29b8240cfe970 |
| 9afc81b | 修复了REPL在输出格式为空情况下强行输出的问题                                           | 陈昊东 | https://github.com/shinkunyachen/SEExp2020/commit/9afc81bca624ccb5348cbd5fcf4aaf4ccb1c9b26 |
| 26205f8 | 修复了在运算符delete下将Double-Free误报为Use-After-Free的问题                          | 陈昊东 | https://github.com/shinkunyachen/SEExp2020/commit/26205f830af6d7c859203d3c16d8537e82a24beb |
| 332d1ab | 修复了CommandLineParser错误消耗子命令的问题                                            | 陈昊东 | https://github.com/shinkunyachen/SEExp2020/commit/332d1abebf7a7034dc6b35e78ffa52727b696f83 |
| 2d0bb4b | 修复了OutputOptions错误初始化的问题                                                    | 陈昊东 | https://github.com/shinkunyachen/SEExp2020/commit/2d0bb4bf3975b31dc4b18068725cba79e621ad5f |
| f0c5c85 | 修复了CommandLineParser子命令解析越界的问题                                            | 陈昊东 | https://github.com/shinkunyachen/SEExp2020/commit/f0c5c853d469e18cfb9ad317b7862f14035c5dbf |
| a098abe | 修复了Memory Leak中部分节点找不到对应符号名的问题                                      | 陈昊东 | https://github.com/shinkunyachen/SEExp2020/commit/a098abe16716f4d648cd6cefd92f2fb92a83caa9 |
| 70d3a99 | 修复了REPL addsrc错误获得占位符的问题                                                  | 陈昊东 | https://github.com/shinkunyachen/SEExp2020/commit/70d3a99b6450d6364953a1993b6003c1a5c509a9 |
| 6eae1a1 | 修复了REPL info src显示不正确的问题                                                    | 陈昊东 | https://github.com/shinkunyachen/SEExp2020/commit/6eae1a1ebce697841831d2913f7b68c772fe12b5 |
| e8a0224 | 修复了无法定位头文件内缺陷的问题                                                       | 陈昊东 | https://github.com/shinkunyachen/SEExp2020/commit/e8a0224818ccb6d773feeb2530ff8a24e1ebe05b |
| 9df658d | 修复了分析前未正确清空统计信息的问题                                                   | 陈昊东 | https://github.com/shinkunyachen/SEExp2020/commit/9df658d8920db4710e25202d2f201fdbaf6f8f3b |
| 63defe0 | 修复了日志缓存的问题                                                                   | 陈昊东 | https://github.com/shinkunyachen/SEExp2020/commit/63defe0a10569a2084dcff98240052c5cf6c51cd |
| 490eeb  | 修复text和markdown输出不能完全显示统计数据的问题                                       | 吴晓阳 | https://github.com/shinkunyachen/SEExp2020/commit/490eeb266668a7e7f4999a5a2c3f9c87f6b179fe |
| 126d7c0 | 重构Json解析函数，修复Json到字符串转换过程中不能处理字符串元素的问题                   | 吴晓阳 | https://github.com/shinkunyachen/SEExp2020/commit/126d7c0b02f7cc4ab35ffb89a384d809c216a603 |
| 771b2de | 修复Json不能正确输出和解析bool型元素的问题                                             | 吴晓阳 | https://github.com/shinkunyachen/SEExp2020/commit/771b2ded8e0df48cb8335ef987be9aa40fe0b03d |
| a37400c | 修复部分代码无法在linux环境下编译的问题                                                | 吴晓阳 | https://github.com/shinkunyachen/SEExp2020/commit/a37400cbaec4b09205bd90974cc5f795ed3c79d3 |
| 8f7bab2 | 修复在解析Diagnostic变量时未正确生成Json树的问题                                       | 吴晓阳 | https://github.com/shinkunyachen/SEExp2020/commit/d13bb8bd445617ad069d01d626070ac13d14d5cc |
| 5dd3d88 | 修改ArrayBoundChecker，检测前置，防止对空内存区域的操作                                | 周若衡 | https://github.com/shinkunyachen/SEExp2020/commit/5dd3d88fa0c7b870934d7f35be7bd60a1a9c40ac |
| aeeac30 | 升级ArrayBoundChecker，提供检测结构体数组的能力                                        | 周若衡 | https://github.com/shinkunyachen/SEExp2020/commit/aeeac30013842c1dae23734569872883264be4ea |
| befd1a1 | 修改ArrayBoundChecker,解决其不能检测结构体/类数组越界的问题                            | 周若衡 | https://github.com/shinkunyachen/SEExp2020/commit/befd1a11adfde5f9e54d31a19fc6d25c4af7ae14 |
| 368d22e | 为Preprocessor添加清理数据接口，修正GUI交互中途重新开始导致Preprocessor不正常运行的bug | 周若衡 | https://github.com/shinkunyachen/SEExp2020/commit/368d22e7acf427cb0608adc98a8de0df8af28170 |
| 60a615d | 修改Preprocessor的接口，修正Preprocosser所有的bug                                      | 周若衡 | https://github.com/shinkunyachen/SEExp2020/commit/60a615d4e5de62605508db755191ced2373795ae |
| 1cd6adb | 修改JsonProc, 使其提供遍历ArrayValue的能力                                             | 周若衡 | https://github.com/shinkunyachen/SEExp2020/commit/1cd6adb1470ebd07af600b6ed95d790abcdedf6f |
| 81116e8 | 修正StatisticHelper，添加clean api，方便清除统计数据                                   | 周若衡 | https://github.com/shinkunyachen/SEExp2020/commit/81116e82ea37eb967b0f12237243cd14eefedce0 |
| ea8e321 | 修改StatisticHelper                                                                    | 周若衡 | https://github.com/shinkunyachen/SEExp2020/commit/ea8e32177a4cec8030571d03686678416589abfa |
| 7a35f01 | 修正UninitObjChecker的错误, 修正对循环引用判断中的检测，防止空内存区域被引用           | 周若衡 | https://github.com/shinkunyachen/SEExp2020/commit/7a35f01778e64b1845a8eace16e7f34ba6ada867 |
| 45e99a0 | 修正StatisticHelper的错误                                                              | 周若衡 | https://github.com/shinkunyachen/SEExp2020/commit/45e99a002aa4914174173d6935d057e74ac5fd08 |
| 3908a8f | 修正StatisticHelper的错误                                                              | 周若衡 | https://github.com/shinkunyachen/SEExp2020/commit/3908a8f05ffb48dec6adc742c8f758b4d4a77c47 |
| 67fc20c | 使UninitObjChecker更加稳定                                                             | 周若衡 | https://github.com/shinkunyachen/SEExp2020/commit/67fc20c705b35b57017822ae6f964f83301790ee |
| bf74230 | 修正StatisticHelper的错误                                                              | 周若衡 | https://github.com/shinkunyachen/SEExp2020/commit/bf742301fd2f6caecace58471d7cafa4b9b07f32 |
| 592f6c4 | 使Preprocessor更稳定                                                                   | 周若衡 | https://github.com/shinkunyachen/SEExp2020/commit/592f6c4192a4b734f90f17873f6fc8d5e1800424 |
| 408b383 | 使UninitObjChecker更加稳定                                                             | 周若衡 | https://github.com/shinkunyachen/SEExp2020/commit/408b383ca463d68396b2c8cfe0ea59612a0fa013 |
| bce5703 | 修改UninitObjChecker中错误的断言                                                       | 周若衡 | https://github.com/shinkunyachen/SEExp2020/commit/bce5703e52c60675990ac6c71933ad09fc2ff40c |
| f8276b5 | 修正UninitObjChecker和Preprocessor中的错误                                             | 周若衡 | https://github.com/shinkunyachen/SEExp2020/commit/f8276b5e9aab7f6b04bb415b53df1430131c6c2e |
| f9f32e6 | 修正Preprocessor模块                                                                   | 周若衡 | https://github.com/shinkunyachen/SEExp2020/commit/f9f32e605f49c3de11ee005959c3eb65ee4f15c0 |
| 8464383 | 统一了编码格式                                                                         | 程珂   | https://github.com/shinkunyachen/SEExp2020/commit/846438335e83c4e868ea25daff6be5e91899e144 |
| 8464383 | 修复了中文字符常量为乱码的bug                                                          | 程珂   | https://github.com/shinkunyachen/SEExp2020/commit/846438335e83c4e868ea25daff6be5e91899e144 |
| 8464383 | 修复了历史记录无法清空的bug                                                            | 程珂   | https://github.com/shinkunyachen/SEExp2020/commit/846438335e83c4e868ea25daff6be5e91899e144 |
| 457732f | 修复了输入项目路径不支持中文的bug                                                      | 程珂   | https://github.com/shinkunyachen/SEExp2020/commit/457732f000bd374619526d22ce4456471cedf836 |
| 457732f | 修复了无法多次检测的bug                                                                | 程珂   | https://github.com/shinkunyachen/SEExp2020/commit/457732f000bd374619526d22ce4456471cedf836 |
| f47252f | 修复了ProcessDialog 重复提示的bug                                                      | 程珂   | https://github.com/shinkunyachen/SEExp2020/commit/f47252f7fbec001aecee67bf6e2ab38de4c787e4 |
| f47252f | 修复了DirTreeDialog重复提示的bug                                                       | 程珂   | https://github.com/shinkunyachen/SEExp2020/commit/f47252f7fbec001aecee67bf6e2ab38de4c787e4 |
| e73627f | 修复了string 到 QStirng转换的过程中中文转换为乱码的bug                                 | 程珂   | https://github.com/shinkunyachen/SEExp2020/commit/e73627fa30cd0b315861b333520f4091221848ba |
| e73627f | 修复了QString 到 Stirng转换的过程中中文转换为乱码的bug                                 | 程珂   | https://github.com/shinkunyachen/SEExp2020/commit/e73627fa30cd0b315861b333520f4091221848ba |
| 32c2621 | 修复了utf-8编码格式下运行显示中文为乱码的bug                                           | 程珂   | https://github.com/shinkunyachen/SEExp2020/commit/32c26210d7168c637a697baa53cc869bdbb7089b |
| cb8a4f9 | 修复了divzeroChecker注册失败的bug                                                      | 程珂   | https://github.com/shinkunyachen/SEExp2020/commit/cb8a4f9a628b9b22f263c77e6d0b06964a780f04 |

|  所属  | 修复的BUG数量 |
| ------ | ------------- |
| 项目   | 56            |
| 施超烜 | 6             |
| 周若衡 | 18            |
| 程珂   | 11            |
| 吴晓阳 | 5             |
| 陈昊东 | 16            |

#### 测试样例

|        文件名        | 行数 | 编写者 |
| -------------------- | ---- | ------ |
| ArrayBoundChecker/*  | 33   | 周若衡 |
| DereferenceChecker/* | 91   | 施超烜 |
| DivModZeroChecker/*  | 75   | 程珂   |
| MemoryChecker/*      | 158  | 陈昊东 |
| UninitObjChecker/*   | 150  | 周若衡 |
| test_commandline.txt | 11   | 陈昊东 |
| test_repl.txt        | 77   | 陈昊东 |
| test_json.json | 25 | 吴晓阳 |
| GUI测试流程          | 34   | 程珂   |