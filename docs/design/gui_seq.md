```sequence
Title:Gui 开始分析的过程
participant MainWindow as M
participant History_item as H_i
participant DirTreeDialog as D
participant History as H
participant ProcessDialog as P
Note left of M:用户输入项目文件夹路径\n报告输出路径\n并点击Go!按钮
M->H_i:item()
H_i-->M:
M->D:DirTreeDialog()
D-->M:dir_tree_dialog
M->D:get_status()
D-->M:DirTreeStatus
M->D:get_compiledb_path()
D-->M:compiledb_path
M->H:History()
H-->M:
M->H:add_history()
M->P:ProcessDialog()
P-->M:
M->P:exec_command()
P-->M:is_successful
M->M:command_to_file
```











```mermaid
sequenceDiagram
participant M as MainWindow
participant H_i as History_item
participant D as DirTreeDialog
participant H as History
participant P as ProcessDialog
note left of M:选择输入输出路径
M->>H_i:item()
H_i-->>M:hty_item
alt outAddr_line->text().trimmed().size()==0
note over M,D:Return And Show Warning To User 
else 
M->>D:DirTreeDialog()
D-->>M:dir_tree_dialog
M->>D:get_status()
D-->>M:DirTreeStatus
alt DirTreeStatus==INIT_FAIL
note over M,D:Return And Show Warning To User 
else 
alt file_list.empty() 
alt DirTreeStatus==FINISHED
note over M,D:Show Warning To User 
end
note over H_i:Return
end
alt DirTreeStatus!=FINISHED
note over M,D:Return
end
end
M->>D:get_compiledb_path()
D-->>M:compiledb_path
end
M->>M: gen_exec_command
M->>H:History()
H-->>M:hty
M->>H:add_history()
M->>P:ProcessDialog()
P-->>M:process_dialog
M->>P:exec_command()
P-->>M:is_successful
alt is_successful==false
M->>M:command_to_file
note over M: Error To User
end
```
