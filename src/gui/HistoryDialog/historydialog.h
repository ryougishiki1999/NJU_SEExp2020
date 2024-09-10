#ifndef HISTORYDIALOG_H
#define HISTORYDIALOG_H

#include <QDialog>
#include<QLabel>
#include<QDialog>
#include<QPushButton>
#include <QtWidgets/QTableView>
#include<QStandardItemModel>
#include<QMenu>
#include<QAction>
#include<QSpacerItem>
#include<QTextBrowser>
#include<QGridLayout>
#include"../Language/language.h"
#include"../History/History.h"


namespace Ui {
class HistoryDialog;
class DetailInfoDialog;
}

// class Deitailed information Dialog
class DetailInfoDialog : public QDialog
{
public:
    Language lan;
    QWidget *widget;
    QGridLayout *gridLayout;
    //labels
    QLabel *in_addr_label;
    QLabel *out_addr_label;
    QLabel *mode_label;
    QLabel *time_label;
    //textBrowsers
    QTextBrowser *in_addr_textBrowser;
    QTextBrowser *out_addr_textBrowser;
    QTextBrowser *mode_textBrowser;
    QTextBrowser *time_textBrowser;
    //spaceitems
    QSpacerItem *verticalSpacer_1;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *verticalSpacer_4;
    QSpacerItem *verticalSpacer_5;

    QSpacerItem *horizontalSpacer_1;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *horizontalSpacer_8;

    explicit DetailInfoDialog(item his_item,Language l=English,QWidget *parent = 0);
    ~DetailInfoDialog();

private:
};

// class History Dialog
class HistoryDialog : public QDialog
{
    Q_OBJECT
private:
    void clear_history();
public:
    QLabel *title_label;
    QTableView *history_table_view;
    QPushButton *clear_button;
    QStandardItemModel *data_model;
    QMenu *table_menu;
    QAction *delete_his_item_action;
    QAction *detailed_info_action;
    History h;
    Language lan;

    explicit HistoryDialog(Language l=English,QWidget *parent = 0);
    ~HistoryDialog();
public slots:
    void confirm_clear();
    void show_detail_info();
    void delete_item();
    void show_table_menu(const QPoint &pos);
private:
    Ui::HistoryDialog *ui;
};

#endif // HISTORYDIALOG_H
