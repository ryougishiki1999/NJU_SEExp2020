#include "historydialog.h"
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableView>
#include<QMessageBox>
#include<QString>
#include"QStandardItemModel"

/*****************************************************************************
This is source coode of HistoryDialog.
HistoryDialog shows the analyse history records.
Each record contains the input address,output address,analyse time and check mode,
This file contains the initialization of all the members of HistoryDialog
and the implementation of all of its functions
******************************************************************************/

// const value of HistoryDialog
const int DETAILED_DIALOG_WIDH=500;
const int DETAILED_DIALOG_HEIGHT=380;
const int WIDGET_X=20;
const int WIDGET_Y=20;
const int WIDGET_WIDH=480;
const int WIDGET_HEIGHT=340;
const int CONTENT_MARGIN=0;

//class detailed information dialog
DetailInfoDialog::DetailInfoDialog(item his_item,Language l,QWidget *parent) :
    QDialog(parent)
{
    this->lan=l;
    Qt::WindowFlags flags=Qt::Dialog;flags |=Qt::WindowCloseButtonHint;setWindowFlags(flags);
    this->setWindowFlags(flags);
    this->setFixedSize(DETAILED_DIALOG_WIDH, DETAILED_DIALOG_HEIGHT);

    //initialize central widget
    widget = new QWidget(this);
    widget->setObjectName(QStringLiteral("widget"));
    widget->setGeometry(QRect(WIDGET_X, WIDGET_Y, WIDGET_WIDH, WIDGET_HEIGHT));
    
    ////initialize  global Layout
    gridLayout = new QGridLayout(widget);
    gridLayout->setObjectName(QStringLiteral("gridLayout"));
    gridLayout->setContentsMargins(CONTENT_MARGIN, CONTENT_MARGIN, CONTENT_MARGIN, CONTENT_MARGIN);

    //initialize input address label
    in_addr_label = new QLabel(widget);
    in_addr_label->setObjectName(QStringLiteral("in_addr_label"));
    gridLayout->addWidget(in_addr_label, 1, 0, 1, 1);

    //initialize output address label
    out_addr_label = new QLabel(widget);
    out_addr_label->setObjectName(QStringLiteral("out_addr_label"));
    gridLayout->addWidget(out_addr_label, 3, 0, 1, 1);

    //initialize time label
    time_label = new QLabel(widget);
    time_label->setObjectName(QStringLiteral("time_label"));
    gridLayout->addWidget(time_label, 5, 0, 1, 1);

    //initialize mode label
    mode_label = new QLabel(widget);
    mode_label->setObjectName(QStringLiteral("mode_label"));
    gridLayout->addWidget(mode_label, 7, 0, 1, 1);

    //initialize input address textBrowser
    in_addr_textBrowser = new QTextBrowser(widget);
    in_addr_textBrowser->setObjectName(QStringLiteral("in_addr_textBrowser"));
    gridLayout->addWidget(in_addr_textBrowser, 1, 2, 1, 1);

    //initialize output address textBrowser
    out_addr_textBrowser = new QTextBrowser(widget);
    out_addr_textBrowser->setObjectName(QStringLiteral("out_addr_textBrowser"));
    gridLayout->addWidget(out_addr_textBrowser, 3, 2, 1, 1);

    //initialize analysis time textBrowser
    time_textBrowser = new QTextBrowser(widget);
    time_textBrowser->setObjectName(QStringLiteral("time_textBrowser"));
    gridLayout->addWidget(time_textBrowser, 5, 2, 1, 1);

    //initialize analysis moe textBrowser
    mode_textBrowser = new QTextBrowser(widget);
    mode_textBrowser->setObjectName(QStringLiteral("mode_textBrowser"));
    gridLayout->addWidget(mode_textBrowser, 7, 2, 1, 1);

    //set vertical spacer items for layout
    verticalSpacer_1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayout->addItem(verticalSpacer_1, 6, 2, 1, 1);
    verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayout->addItem(verticalSpacer_2, 4, 2, 1, 1);
    verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayout->addItem(verticalSpacer_3, 2, 2, 1, 1);
    verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayout->addItem(verticalSpacer_4, 8, 2, 1, 1);
    verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gridLayout->addItem(verticalSpacer_5, 0, 2, 1, 1);

    //set horizontal spacer items for layout
    horizontalSpacer_1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(horizontalSpacer_1, 3, 3, 1, 1);
    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(horizontalSpacer_2, 5, 3, 1, 1);
    horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(horizontalSpacer_3, 7, 3, 1, 1);
    horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(horizontalSpacer_4, 1, 3, 1, 1);
    horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(horizontalSpacer_5, 7, 1, 1, 1);
    horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(horizontalSpacer_6, 5, 1, 1, 1);
    horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(horizontalSpacer_7, 3, 1, 1, 1);
    horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(horizontalSpacer_8, 1, 1, 1, 1);

    in_addr_textBrowser->setText(QString(his_item.in_addr.c_str()));
    out_addr_textBrowser->setText(QString(his_item.out_addr.c_str()));
    time_textBrowser->setText(QString(his_item.time.c_str()));
    switch(lan)
    {
    case Chinese:
        this->setWindowTitle(QStringLiteral("历史记录详细信息"));
        in_addr_label->setText(QStringLiteral("输入地址:"));
        time_label->setText(QStringLiteral("检测时间:"));
        mode_label->setText(QStringLiteral("检测模式:"));
        out_addr_label->setText(QStringLiteral("输出地址:"));
        if(his_item.is_godmode)
            mode_textBrowser->setText(QStringLiteral("专家模式"));
        else
            mode_textBrowser->setText(QStringLiteral("学生模式"));
        break;
    case English:
        this->setWindowTitle(QStringLiteral("History Detail"));
        in_addr_label->setText(QStringLiteral("Input Address:"));
        time_label->setText(QStringLiteral("Analyze Time:"));
        mode_label->setText(QStringLiteral("Analyze Mode:"));
        out_addr_label->setText(QStringLiteral("Output Address:"));
        if(his_item.is_godmode)
            mode_textBrowser->setText(QStringLiteral("god mode"));
        else
            mode_textBrowser->setText(QStringLiteral("studet mode"));
        break;
    }
}

DetailInfoDialog::~DetailInfoDialog()
{
}

//const values of class history dialog
const int DIALOG_WIDTH=900;
const int DIALOG_HEIHGT=800;

const int TITLE_X_COORD=0;
const int TITLE_Y_COORD=0;
const int TITLE_WIDTH=DIALOG_WIDTH;
const int TITLE_HEIGHT=70;

const int TABLE_VIEW_X_COORD=30;
const int TABLE_VIEW_Y_COORD=75;
const int TABLE_VIEW_WIDTH=840;
const int TABLE_VIEW_HEIGHT=660;

const int CLEAR_BUTTON_WIDTH=140;
const int CLEAR_BUTTON_HEIGHT=45;
const int CLEAR_BUTTON_X_COORD=(DIALOG_WIDTH-CLEAR_BUTTON_WIDTH)/2;
const int CLEAR_BUTTON_Y_COORD=TABLE_VIEW_Y_COORD+TABLE_VIEW_HEIGHT+10;

const int DEFAULT_CHOSEN_INDEX=0;

// class history dialog
HistoryDialog::HistoryDialog(Language l,QWidget *parent) :
    QDialog(parent)
{
    this->lan=l;

    this->setFixedSize(DIALOG_WIDTH,DIALOG_HEIHGT);
    Qt::WindowFlags flags=Qt::Dialog;flags |=Qt::WindowCloseButtonHint;setWindowFlags(flags);
    this->setWindowFlags(flags);

    //initialize the taitle label
    title_label = new QLabel(this);
    title_label->setObjectName(QStringLiteral("label"));
    title_label->setGeometry(QRect(TITLE_X_COORD, TITLE_Y_COORD,
                                   TITLE_WIDTH, TITLE_HEIGHT));
    //initialize the taitle label font
    QFont font;
    font.setFamily(QStringLiteral("Arial"));
    font.setPointSize(18);
    title_label->setFont(font);
    title_label->setAlignment(Qt::AlignCenter);

    //initialize the table view
    history_table_view = new QTableView(this);
    history_table_view->setObjectName(QStringLiteral("tableView"));
    history_table_view->setGeometry(QRect(TABLE_VIEW_X_COORD, TABLE_VIEW_Y_COORD,
                                          TABLE_VIEW_WIDTH, TABLE_VIEW_HEIGHT));

    //initialize the clear button
    clear_button=new QPushButton(this);
    clear_button->setObjectName(QStringLiteral("clear_button"));
    clear_button->setGeometry(CLEAR_BUTTON_X_COORD,CLEAR_BUTTON_Y_COORD,
                              CLEAR_BUTTON_WIDTH,CLEAR_BUTTON_HEIGHT);
    connect(clear_button,SIGNAL(clicked(bool)),this,SLOT(confirm_clear()));

    //initialize actions
    delete_his_item_action = new QAction(this);
    delete_his_item_action->setObjectName(QStringLiteral("delete_his_item_action"));
    detailed_info_action = new QAction(this);

    //initialize table_menu
    table_menu=new QMenu(this);
    table_menu->setObjectName(QStringLiteral("table_menu"));
    table_menu->addAction(delete_his_item_action);
    table_menu->addSeparator();
    table_menu->addAction(detailed_info_action);

    connect( delete_his_item_action, SIGNAL(triggered(bool)),this,SLOT(delete_item()));
    connect( detailed_info_action,SIGNAL(triggered(bool)),this,SLOT(show_detail_info()));
    this->history_table_view->setContextMenuPolicy(Qt::CustomContextMenu);

    //initialize the history table
    data_model=new QStandardItemModel;
    data_model->setObjectName(QStringLiteral("data_model"));
    data_model->setColumnCount(ATTR_NUM);
    QString headers[4];
    switch(this->lan)
    {
    case Chinese:
        headers[0]=QStringLiteral("输入地址");
        headers[1]=QStringLiteral("输出地址");
        headers[2]=QStringLiteral("时间");
        headers[3]=QStringLiteral("检测模式");
        this->setWindowTitle(QStringLiteral("检测历史"));
        title_label->setText(QStringLiteral("历史"));
        clear_button->setText(QStringLiteral("清空历史记录"));
        delete_his_item_action->setText(QStringLiteral("删除选中历史记录"));
        detailed_info_action->setText(QStringLiteral("查看详细信息"));
        break;
    case English:
        headers[0]=QStringLiteral("input address");
        headers[1]=QStringLiteral("output address");
        headers[2]=QStringLiteral("time");
        headers[3]=QStringLiteral("mode");
        this->setWindowTitle(QStringLiteral("Aanlysis History"));
        title_label->setText(QStringLiteral("History"));
        clear_button->setText(QStringLiteral("Clear History"));
        delete_his_item_action->setText(QStringLiteral("delete this item"));
        detailed_info_action->setText(QStringLiteral("detailed information"));
        break;
    }

    for (int i = 0; i < ATTR_NUM; i++)
    {
         data_model->setHeaderData(i, Qt::Horizontal, headers[i]);
    }
    history_table_view->setModel(data_model);
    //Set the column width to be adaptive
    history_table_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //load history file and bind data with view
    h.load_history();
    data_model->setRowCount(h.size());
    for(int i=0;i<h.size();i++)
    {
        item his_temp=h.get(i);
        data_model->setItem(i,0,new QStandardItem(QString(his_temp.in_addr.c_str())));
        data_model->setItem(i,1,new QStandardItem(QString(his_temp.out_addr.c_str())));
        data_model->setItem(i,2,new QStandardItem(QString(his_temp.time.c_str())));
        if(his_temp.is_godmode)
        {
            if(lan==Chinese)
            {
                data_model->setItem(i,3,new QStandardItem(QStringLiteral("专家模式")));
            }
            else if(lan==English)
            {
                data_model->setItem(i,3,new QStandardItem(QStringLiteral("god mode")));
            }
        }else
        {
            if(lan==Chinese)
            {
                data_model->setItem(i,3,new QStandardItem(QStringLiteral("学生模式")));
            }
            else if(lan==English)
            {
                data_model->setItem(i,3,new QStandardItem(QStringLiteral("student mode")));
            }
        }
        data_model->item(i, 3)->setTextAlignment(Qt::AlignCenter);
    }

    //Set properties of the table view
    history_table_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    history_table_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    history_table_view->setAlternatingRowColors(true);
    history_table_view->setSelectionMode(QAbstractItemView::SingleSelection);
    if(h.size()>0){
    history_table_view->selectRow(DEFAULT_CHOSEN_INDEX);
    }
    connect(history_table_view,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(show_table_menu(QPoint)));
}

//show MessageBox to user to confirm if he really wants to clear all the history records
void HistoryDialog::confirm_clear()
{
   int r;
   QString yes_ans;
   if(!h.empty())
   {
    switch (lan)
    {
        case Chinese:
            r=QMessageBox::information(this,QStringLiteral("确认"),
                            QStringLiteral("您确认要清空全部历史记录吗？"),
                                         QStringLiteral("确认"), QStringLiteral("取消"));
            yes_ans=QStringLiteral("确认");
            break;
        default:
            r = QMessageBox::information(this, QStringLiteral("Confirm"), 
                            QStringLiteral("Are you sure you to clear the history?"),
                                         QStringLiteral("Yes"), QStringLiteral("No"));
            yes_ans = QStringLiteral("Yes");
   }
    if(r ==yes_ans.toInt())
    {
        clear_history();
    }else return;
  }
}

//clear dialog and clear history.txt
void HistoryDialog::clear_history()
{
    data_model->clear();
    h.clear_history();
}

// show user the detailed Information of the history record selected
void HistoryDialog::show_detail_info()
{
    int row_index=history_table_view->currentIndex().row();
    item temp;
    if(row_index==-1)return ;
    else 
    {
        QModelIndex index = data_model->index(row_index,0);
        temp.in_addr= data_model->data(index).toString().toStdString();

        index = data_model->index(row_index,1);
        temp.out_addr = data_model->data(index).toString().toStdString();

        index = data_model->index(row_index,2);
        temp.time = data_model->data(index).toString().toStdString();

        index = data_model->index(row_index,3);
        if(data_model->data(index).toString()==QStringLiteral("god mode") ||
              data_model->data(index).toString()==QStringLiteral("专家模式")  )
        temp.is_godmode=true;
        else temp.is_godmode=false;
    }
    DetailInfoDialog did(temp,this->lan);
    did.exec();
}

//check if there is a item being selected
//then delete it
void HistoryDialog::delete_item()
{
    int row_index=history_table_view->currentIndex().row();
    if(row_index==-1|| !h.delete_his_by_index(row_index))
    {
        switch (lan)
        {
         case Chinese:
            QMessageBox::warning(this,QStringLiteral("失败"),
                                 QStringLiteral("删除失败!"));
            break;
         default:
             QMessageBox::warning(this, QStringLiteral("Failure"), 
                                QStringLiteral("Fail To Delete!"));
        }
    }
    else data_model->removeRow(row_index);
}

void HistoryDialog::show_table_menu(const QPoint &pos)
{
    if(history_table_view->currentIndex().row()!=-1&& !h.empty())
    {
        table_menu->exec(QCursor::pos());
    }
}

HistoryDialog::~HistoryDialog(){
}
