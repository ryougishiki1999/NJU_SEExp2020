## Code Style

1. 关于命名中的缩写，若在与类命名规则相同的实体下则使用全大写，否则使用全小写 e.g. 
    ```c++
    class CSADriver;
    CSADriver csa_driver;
    ```

2. 变量命名：所有字母小写，单词与单词之间用 ' _ ' 分割，e.g. `delete_expr`

3. 函数命名：所有字母小写，单词与单词之间用 ' _ ' 分割，e.g.

   ```c++
   bool is_deallocated(clang::ento::CheckerContext &ctx,
                         clang::ento::SymbolRef memory) const;
   ```

4. 类名：首字母大写，采用驼峰式命名法，e.g. `MemoryState`

5. 宏命名：所有字母大写，单词与单词之间用 ' _ ' 分割，e.g. `CHECKER_MEMORY_CHECKER_H`

6. 枚举成员命名与类名规则相同 e.g.
   ```c++
   enum State{
       Allocated,
       Deallocated
   };
   ```