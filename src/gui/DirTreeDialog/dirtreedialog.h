#ifndef DIRTREEDIALOG_H
#define DIRTREEDIALOG_H

#include<QTreeWidgetItem>
#include <QDialog>
#include<filesystem>
#include<set>
#include<map>
#include<string>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>
#include"preprocessor/Preprocessor.h"
#include"Language/language.h"

enum DirTreeStatus{INIT_FAIL,INIT,CHOSEN_COMPILE_DB,FINISHED};

namespace Ui {
class DirTreeDialog;
}

class DirTreeDialog : public QDialog
{
    Q_OBJECT
private:
    Language lan;
    DirTreeStatus status;
    QString compile_db;
     std::set<std::string>*  file_list;
     std::string source;
     void update_children(QTreeWidgetItem* root,Qt::CheckState state,std::set<std::string>*file_list);
     void update_parent(QTreeWidgetItem* leaf,Qt::CheckState state);
public:
    QPushButton *affirm_button;
    QTreeWidget *tree_widget;
    QPushButton *cancel_button;
public:
    void load_comiple_dbs(std::map<std::filesystem::path,
                          std::set<std::filesystem::path>> &compiledb_mp);

    explicit DirTreeDialog(std::set<std::string>*  file_list, std::string source,
                           Language lan=English,QWidget *parent = nullptr);
    QString get_compiledb_path(){return this->compile_db;};
    ~DirTreeDialog();
public slots:
   void  tree_item_clicked(QTreeWidgetItem*,int);
   void cancel();
   void build_file_tree(const FileTreeNode *root);
   DirTreeStatus get_status(){return this->status;};
   void confirm();
protected:
   void closeEvent(QCloseEvent *event);
};

#endif // DIRTREEDIALOG_H
