# 测试文档

撰写人：施超烜 1985045185@qq.com

## 引言

本文档用于记录本项目组开发的静态分析程序hisa的测试，包括测试用例、测试命令与测试结果。目的是达到总体代码覆盖率90%的要求，并在测试的过程中验证软件的功能是否符合预期。

## 测试计划

### 测试人员

命令行程序：施超烜

Gui程序：程珂

JsonProc：程珂

### 测试工具

开源工具：[OpenCppCoverage](https://github.com/OpenCppCoverage/OpenCppCoverage)

### 测试方式

命令行程序：通过命令行调用OpenCppCoverage程序对带有pdb数据库的程序通过固定的测试命令进行运行，并将多次测试结果合并为一个代码覆盖率的测试报告。

Gui程序：通过命令行调用OpenCppCoverage程序对带有pdb的在VS2019下开发的qt程序进行运行，并通过设计固定的测试流程进行测试，从而得到代码覆盖率的测试报告。

JsonProc：设计单独的测试用例和测试程序测试JsonProc的接口，并使用OpenCppCoverage进行代码覆盖率的测试。

## 测试总结

### 20200606测试命令行程序

[详情](../docs/20200606_console_test_doc/index.html)

#### 测试用例

test\DereferenceChecker\\*

test\DivModZeroChecker\\*

test\MemoryChecker\\*

test\UninitPbjChecker\\*

#### 测试命令

##### 命令行

[test_commandline.txt](../test/test_commandline.txt)

其中前四条命令

##### REPl

测试命令详情：[test_repl.txt](../test/test_repl.txt)

#### 测试结果

命令行版本和核心部分总体覆盖率达到87%，其中src\core\Json\JsonProc.cpp(8%)、src\core\Logger.cpp(64%)、src\core\Json\JsonProc.h(66%)、src\core\StaticAnalyzer.cpp(73%)、src\core\AnalyzerOptions.cpp(78%)、src\core\Json\DiagJsonTransform.cpp(78%)测试覆盖率较低。

覆盖率较低的可能原因为：部分代码的接口未被使用，同时部分边界分支未覆盖。

### 20200608测试命令行程序

[详情](../docs/20200608_console_test_doc/index.html)

#### 测试用例

test\DereferenceChecker\\*

test\DivModZeroChecker\\*

test\MemoryChecker\\*

test\UninitPbjChecker\\*

#### 测试命令

##### 命令行

[test_commandline.txt](../test/test_commandline.txt)

全部命令

##### REPL

测试命令详情：[test_repl.txt](../test/test_repl.txt)

#### 测试结果

命令行版本与核心部分总体覆盖率达到89%，其中src\core\Json\JsonProc.cpp(8%)、src\core\Json\DiagJsonTransform.cpp(68%)、src\core\Json\JsonProc.h(70%)测试覆盖率较低。

覆盖率较低的可能原因为：部分代码的接口未被完全使用。

### 20200625测试Gui程序

[详情](../docs/20200625_gui_test_doc/index.html)

#### 测试流程

详情见[Gui测试流程](../docs/gui_test_instruction/测试流程.md)

#### 测试结果

Gui程序的代码覆盖率达到78%，其中JsonProc.h(27%)、Preprocesspr.cpp(32%)、JsonProc.cpp（43%）、FileSystemTree.cpp(45%)、FileSystemTree.h(51%)覆盖率较低

覆盖率较低的可能原因：部分模块的接口未被完全使用。

### 20200629测试JsonProc

[详情](../docs/20200629_JsonProc_test_doc/index.html)

#### 测试目的

为了充分测试JsonProc的所有接口，对JsonProc.h与JsonProc.cpp进行单独的代码覆盖度测试

#### 测试代码

```c++
void JsonUseCase()
{
 
    // generate json and transform to string
    {
        auto object_value = Json::make_object();
        auto int_value = Json::make_value(1);
        auto double_value = Json::make_value(1.1);
        auto bool_value = Json::make_value(true);
        auto string_value = Json::make_value("str");
        auto array_value = Json::make_array();
 
        array_value.append(int_value);
        array_value.append(double_value);
        array_value.append(bool_value);
        array_value.append(string_value);
 
        object_value.append("array", array_value);
        // expected json text: {"array":[1,1.100000,true,"str"]}
 
        // you can stringify any type of value
        std::string json_txt = Json::stringify(object_value);
        //std::cout << json_txt << std::endl;
 
        auto &root = Json::parse(json_txt);
        std::string txt = Json::stringify(root);
        //std::cout << txt << std::endl;
        root.release();
    }
 
    // generate json tree on heap
    {
        auto &root = Json::new_object();
        root.append("int", Json::new_value(1));
        root.append("double", Json::new_value(1.1));
 
        std::string t = Json::stringify(root);
        //std::cout << t << "\n";
 
        // use release to free memory on json tree
        root.release();
    }
}
int main(){
	JsonUseCase();
	return 0;
}
```

#### 测试结果

代码覆盖率达到了78%，有所提高，但仍然无法满足要求

代码覆盖率低的原因：测试用例不够丰富，无法测试所有的代码

### 20200630测试Gui程序

[详情](../docs/20200630_gui_test_doc/index.html)

#### 测试用例

详情见[Gui测试流程](../docs/gui_test_instruction/测试流程.md)

#### 测试结果

Gui版本总体覆盖率达到了94%，已经达到代码覆盖率要求

### 20200630测试JsonProc

[详情](../docs/20200630_JsonProc_test_doc/index.html)

#### 测试目的

为了进一步测试代码，使得代码覆盖率达到要求

#### 测试代码

```c++
void JsonUseCase()
{
 
    // generate json and transform to string
    {
        auto object_value = Json::make_object();
        auto int_value = Json::make_value(1);
        auto double_value = Json::make_value(1.1);
        auto bool_value = Json::make_value(true);
        auto bool2_value = Json::new_value(false);
        auto string_value = Json::make_value("str");
        auto array_value = Json::make_array();
        std::string s2 = "str2";
        auto string2_value = Json::make_value(s2);
        std::string s3 = "str3";
        auto string3_value = Json::make_value(std::move(s3));
 
        array_value.append(int_value);
        array_value.append(double_value);
        array_value.append(bool_value);
        array_value.append(bool2_value);
        array_value.append(string_value);
        array_value.append(string2_value);
        array_value.append(string3_value);
 
        object_value.append("array", array_value);
        // expected json text: {"array":[1,1.100000,true,"str"]}
 
        // you can stringify any type of value
        std::string json_txt = Json::stringify(object_value);
        //std::cout << json_txt << std::endl;
 
        auto &root = Json::parse(json_txt);
        std::string txt = Json::stringify(root);
        //std::cout << txt << std::endl;
        root.release();
    }
 
    // generate json tree on heap
    {
        auto &object_value = Json::new_object();
        auto &array_value = Json::new_array();
        auto &int_value = Json::new_value(1);
        auto &double_value = Json::new_value(1.1);
        auto &bool_value = Json::new_value(true);
        auto &bool2_value = Json::new_value(false);
        auto &string_value = Json::new_value("str");
        std::string str2 = "str2";
        auto &string2_value = Json::new_value(str2);
        std::string str3 = "str3";
        auto &string3_value = Json::new_value(std::move(str3));
 
        object_value.append("array", array_value);
        array_value.append(int_value);
        array_value.append(double_value);
        array_value.append(bool_value);
        array_value.append(bool2_value);
        array_value.append(string_value);
        array_value.append(string2_value);
        array_value.append(string3_value);
 
        std::string txt = Json::stringify(object_value);
        auto &parsed_result = Json::parse(txt);
        //std::cout << txt << "\n";
 
        // use release to free memory on json tree
        object_value.release();
    }
}
int main(){
 
	JsonUseCase();
	return 0;
}
```

#### 测试结果

代码覆盖率达到了92%，满足了代码覆盖率的要求

### 20200701测试命令行程序

[详情](../docs/20200701_console_test_doc/index.html)

#### 测试用例

test\DereferenceChecker\\*

test\DivModZeroChecker\\*

test\MemoryChecker\\*

test\UninitPbjChecker\\*

test\ArrayBoundChecker\*

#### 测试命令

##### 命令行

[test_commandline.txt](../test/test_commandline.txt)

全部命令

##### REPL

测试命令详情：[test_repl.txt](../test/test_repl.txt)

#### 测试结果

命令行版本与核心部分（屏蔽了JsonProc部分，由于有些接口并不会被用到，改为通过单独设计测试用例的方式检测JsonProc代码）总体覆盖率达到了96%，已经达到代码覆盖率要求。

### 20200703测试Gui程序

[详情](../docs/20200703_gui_test_doc/index.html)

#### 测试用例