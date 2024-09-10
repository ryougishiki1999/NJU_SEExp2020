#include "dirtreedialog.h"
#include<QTreeWidgetItem>
#include<QString>
#include<QMessageBox>
#include<QDebug>
using namespace std;

/*****************************************************************************
This is source coode of DirTreeDialog.
DirTreeDialog shows the compile_commands.json file list to user to choose
and then use the chosen json file to generate DocumentTree for user to choose
the file he wants to analyze.
This file contains the initialization of all the members of DirTreeDialog 
and the implementation of all of its functions
******************************************************************************/

//const value of DirTreeDialog
const int DIALOG_WIDTH=720;
const int DIALOG_HEIGHT=700;
const int AFFIRM_BUTTON_X=170;
const int AFFIRM_BUTTON_Y=650;
const int AFFIRM_BUTTON_WIDTH=120;
const int AFFIRM_BUTTON_HEIGHT=40;
const int BUTTON_INTERVAL=250;
const int WIDGET_BUTTON_X=20;
const int WIDGET_BUTTON_Y=20;
const int WIDGET_BUTTON_WIDTH=680;
const int WIDGET_BUTTON_HEIGHT=610;
const int FONT_SIZE = 14;

//initialize DirTreeDialog and its widgets
DirTreeDialog::DirTreeDialog(set<string> * file_list,string source,Language lan,QWidget *parent) :
    QDialog(parent)
{
    this->lan=lan;
    this->file_list=file_list;
    if (!this->file_list->empty())
        this->file_list->clear();

    // this is used to differentiate status of the dialog
    //different status will lead to different actions when users click "confirm" button
    this->status=INIT;

    //initialize the dialog window
    Qt::WindowFlags flags=Qt::Dialog;flags |=Qt::WindowCloseButtonHint;setWindowFlags(flags);
    this->setWindowFlags(flags);
    this->setFixedWidth(DIALOG_WIDTH);
    this->setFixedHeight(DIALOG_HEIGHT);

    //initialize button text font
    QFont font;
    font.setFamily(QStringLiteral("Cambria"));
    font.setPointSize(FONT_SIZE);

    //initialize affirm_button
     affirm_button = new QPushButton(this);
    affirm_button->setObjectName(QStringLiteral("affirm_button"));
     affirm_button->setGeometry(QRect(AFFIRM_BUTTON_X, AFFIRM_BUTTON_Y,
                                                    AFFIRM_BUTTON_WIDTH, AFFIRM_BUTTON_HEIGHT));
     affirm_button->setFont(font);
     connect(affirm_button,SIGNAL(clicked()),this,SLOT(confirm()));

     //initialize cancel button
     cancel_button = new QPushButton(this);
     cancel_button->setObjectName(QStringLiteral ("cancel_button"));
     cancel_button->setGeometry(QRect(AFFIRM_BUTTON_X+BUTTON_INTERVAL, AFFIRM_BUTTON_Y,
                                            AFFIRM_BUTTON_WIDTH, AFFIRM_BUTTON_HEIGHT));
     cancel_button->setFont(font);
     connect(cancel_button,SIGNAL(clicked()),this,SLOT(cancel()));

     //Sets the text content of the widget according to the language
     switch (this->lan) {
     case Chinese:
         this->setWindowTitle(QStringLiteral ("文件列表"));
         affirm_button->setText(QStringLiteral("确认"));
         cancel_button->setText(QStringLiteral("取消"));
         break;
     default:
         this->setWindowTitle(QStringLiteral ("File List"));
         affirm_button->setText(QStringLiteral("Affirm"));
         cancel_button->setText(QStringLiteral("Cancel"));
     }

     //initialize tree wdget
     tree_widget = new QTreeWidget(this);
     tree_widget->setObjectName(QStringLiteral ("tree_widget"));
     tree_widget->setGeometry(QRect(WIDGET_BUTTON_X, WIDGET_BUTTON_Y,
                                                WIDGET_BUTTON_WIDTH, WIDGET_BUTTON_HEIGHT));
     tree_widget->setHeaderHidden(true);
     connect(tree_widget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),
             this,SLOT(tree_item_clicked(QTreeWidgetItem*,int)));
     //start to generate documents tree
        Preprocessor::instance();
        if (Preprocessor::instance()->resolve_source(source))
        {
            Preprocessor::instance()->search_compile_commands_json_entry();
            if (!Preprocessor::instance()
                     ->get_compile_commands_list()
                     .empty()) // 项目文件夹下存在compile_commands.json，可能为多数
            {
                Preprocessor::instance()->parse_compile_commands_json_entry();
                // 返回compile_commands,json和其对应的文件列表，可能为多数
                map<filesystem::path, set<filesystem::path>> compiledb_mp=Preprocessor::instance()->get_corres_file_map();
                // TODO: 用户交互界面，指定compile_commands.json，返回其对应的文件列表
                load_comiple_dbs(compiledb_mp);
            }
            else // 项目文件下不存在compile_commands.json
            {
                status=INIT_FAIL;
                Preprocessor::instance()->clear();
                //Sets the text content of the MessageBox according to the language
                switch(this->lan){
                case Chinese:
                    QMessageBox::warning(tree_widget, QStringLiteral("错误"),QStringLiteral("HISA 不支持该项目,\n"
                                                             "您应该向我们提供 compile_commands.json 文件,\n"
                                                            "您可以从 www.Hisa.com 网站获得更多信息"));
                    break;
                default:
                    QMessageBox::warning(tree_widget, "Error",QStringLiteral("HISA does not support this kind of project,\n"
                                                             "you should provide compile_commands.json,\n"
                                                            "and you can get mroe information from www.Hisa.com"));
                }
                return;
            }
        }
        else
        {
            status=INIT_FAIL;
            Preprocessor::instance()->clear();
            //Sets the text content of the MessageBox according to the language
            switch(this->lan){
            case Chinese:
                QMessageBox::warning(tree_widget, QStringLiteral ("错误"), QStringLiteral ("您提供的路径不正确,\n"
                                                     "您可以从 www.Hisa.com 网站获得更多信息"));
                break;
            default:
                QMessageBox::warning(tree_widget, QStringLiteral ("Error"), QStringLiteral ("Your path is not correct,\n"
                                                     "get mroe information from www.Hisa.com"));
            }
        }
}

//build compile_commands.json file list 
//user can only choose one file in the list
void DirTreeDialog::load_comiple_dbs(map<filesystem::path, set<filesystem::path>> &compiledb_mp)
{
    for(map<filesystem::path, set<filesystem::path>>::iterator i=compiledb_mp.begin();i!=compiledb_mp.end();i++)
    {
        QTreeWidgetItem* group = new QTreeWidgetItem(tree_widget);
        group->setText(0,QString::fromLocal8Bit(( i->first).string().c_str()));
        group->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        group->setCheckState(0,Qt::Unchecked);
        group->setToolTip(0,QString::fromLocal8Bit(( i->first).string().c_str()));
    }
}

//when one node is chosen all of its child node will be chosen
//use bfs to update
//update file list at the same timme
void DirTreeDialog::update_children(QTreeWidgetItem* root,
                                    Qt::CheckState state,set<string>*file_list){
    //bfs
    queue<QTreeWidgetItem*> q;
    q.push(root);
    while(!q.empty()){
        QTreeWidgetItem* item=q.front();
        q.pop();
        item->setCheckState(0,state);
        int child_count=item->childCount();
        if(child_count==0)
        {
            if(Qt::Checked == state)
            {
                if(file_list->find(item->text(0).toStdString()) ==file_list->end()){
                     file_list->insert(item->text(0).toStdString());
                }
            }else if(Qt::Unchecked == state)
            {
                set<string>::iterator it=file_list->find(item->text(0).toStdString());
                if(it!=file_list->end())
                {
                    file_list->erase(it);
                }
            }
        }
        else
        {
            for(int i=0;i<child_count;i++)q.push(item->child(i));
        }
    }
}

// when one node is chosen we mush check its parent
// judge if all child nodes of its parent is chosen
// and then update its parent
void DirTreeDialog::update_parent(QTreeWidgetItem* leaf,
                                  Qt::CheckState state)
{
    QTreeWidgetItem* parent=leaf->parent();
    while(parent!=NULL)
    {
         if(state==Qt::Unchecked)
         {
                    bool flag=true;
                    for(int i=0;i<parent->childCount();i++)
                    {
                            if(parent->child(i)->checkState(0)==Qt::Checked ||
                                    parent->child(i)->checkState(0)==Qt::PartiallyChecked)
                            {
                                flag=false;
                                break;
                            }
                    }
                    if(!flag)parent->setCheckState(0,Qt::PartiallyChecked);
                    else parent->setCheckState(0,Qt::Unchecked);
        }else if(state==Qt::Checked)
        {
             bool flag=true;
             for(int i=0;i<parent->childCount();i++)
             {
                     if(parent->child(i)->checkState(0)==Qt::Unchecked ||
                             parent->child(i)->checkState(0)==Qt::PartiallyChecked)
                     {
                         flag=false;
                         break;
                     }
             }
             if(!flag)parent->setCheckState(0,Qt::PartiallyChecked);
             else parent->setCheckState(0,Qt::Checked);
         }
    parent=parent->parent();
    }
}

//change the CheckStatus of the clicked item
//update chosen file list
void DirTreeDialog:: tree_item_clicked(QTreeWidgetItem* item,int col)
{
    if(Qt::Checked == item->checkState(0))
    {
           if(status==INIT)
           {
                compile_db=item->text(col);
                QTreeWidgetItemIterator it(this->tree_widget);
                while(*it)
                {
                    if(*it!=item)
                    {
                       (*it)->setCheckState(0,Qt::Unchecked);
                    }
                    it++;
                }
           }else if(status==CHOSEN_COMPILE_DB)
           {
               update_children(item,Qt::Checked,file_list);
               update_parent(item,Qt::Checked);
           }
    }
    else if(Qt::Unchecked == item->checkState(0))
    {
            if(status==INIT)
            {
                this->compile_db="";
            }
            else if(status==CHOSEN_COMPILE_DB){
                update_children(item,Qt::Unchecked,file_list);
                update_parent(item,Qt::Unchecked);
            }
    }
}

//bfs to build document tree
void DirTreeDialog::build_file_tree(const FileTreeNode *root)
{
    queue<const FileTreeNode*>  q;
    q.push(root);
    map<const FileTreeNode*, QTreeWidgetItem*> file_wdigt_mp;
    QTreeWidgetItem* item = new QTreeWidgetItem(this->tree_widget);
    item->setText(0,QString::fromLocal8Bit(root->get_file_path().string().c_str()));
    item->setToolTip(0, QString::fromLocal8Bit(root->get_file_path().string().c_str()));
    item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setCheckState(0,Qt::Checked);
    file_wdigt_mp[root]=item;
    while(!q.empty())
    {
        const FileTreeNode* node=q.front();
        q.pop();
        if(!node->is_leaf()){
            vector<FileTreeNode*> children=node->get_children();
            for (unsigned int i=0;i<children.size();i++)
            {
                const FileTreeNode*child=children[i];
                QTreeWidgetItem* item = new QTreeWidgetItem(file_wdigt_mp[node]);
                item->setText(0, QString::fromLocal8Bit(child->get_file_path().string().c_str()));
                item->setToolTip(0, QString::fromLocal8Bit(child->get_file_path().filename().string().c_str()));
                item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
                item->setCheckState(0,Qt::Checked);
                if(child->is_leaf())
                {
                    file_list->insert(child->get_file_path().string());
                }
                q.push(child);
                file_wdigt_mp[child]=item;
            }
        }
    }
}

//when status is INIT,check if compile_commands.json file has been chosen
//if compile_commands.json file has been selected,change the status to FINISHED
void DirTreeDialog::confirm()
{
    if(status==INIT)
    {
        if(compile_db=="")
        {
            switch (this->lan)
            {
            case Chinese:
                QMessageBox::warning(this,  QStringLiteral ("警告"),  QStringLiteral ("请选择一个 compile_jason.json 文件"));
                break;
            default:
                QMessageBox::warning(this->tree_widget, QStringLiteral("warning"),  QStringLiteral ("Please choose a compile_commands.json file"));
            }                         
        }else
        {
            filesystem:: path command_db_path=compile_db.toStdString();
            map<filesystem::path, set<filesystem::path>> compiledb_mp=Preprocessor::instance()->get_corres_file_map();
            Preprocessor::instance()->set_target_source_file_list(compiledb_mp[command_db_path]);
            Preprocessor::instance()->build_file_tree();
            const FileSystemTree *tree = Preprocessor::instance()->get_file_tree();
            const FileTreeNode *root = tree->get_root();
            this->tree_widget->clear();
            build_file_tree(root);
            this->status=CHOSEN_COMPILE_DB;
        }
    }else if(status==CHOSEN_COMPILE_DB)
    {
        this->status = FINISHED;
        this->close();
    }
}

//canel analyse,clear chosen file list
void DirTreeDialog::cancel()
{
    if(status==CHOSEN_COMPILE_DB){
        file_list->clear();
    }
    this->close();
}

//clear preprocessor's file system to prepare for the next analysis
void DirTreeDialog::closeEvent(QCloseEvent *event)
{
    Preprocessor::instance()->clear();
}

DirTreeDialog::~DirTreeDialog()
{
}
