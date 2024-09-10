#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include<QFileDialog>
#include<map>
#include<QString>
#include"../Language/language.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
Language language;
std::map<QString,bool> check_mode_mp;
std::map<QString,bool> output_info_mp;
std::map<QString,bool> report_type_mp;
std::map<QString,bool> checker_mp;
QString console_version_addr;
QString command;

public:
QMenu *help_menu;
QMenu *options_help_menu;
QAction *in_addr_help;
QAction *check_mode_help;
QAction *output_info_help;
QAction *output_format_help;
QAction *checker_help;
QAction *contact_us_help;
QAction *action_history;
QAction *action_rst_dft_ops;

QWidget *central_widget;
QMenuBar *menu_bar;
QStatusBar *status_bar;
QToolBar *main_tool_bar;
QMenu *settings_menu;
QMenu *language_menu;
QAction* action_Chinese;
QAction* action_English;

//layout and spacer
QGridLayout *gridLayout_global;
QGridLayout *gridLayout_option_table;
QHBoxLayout *horizontalLayout;
QHBoxLayout *horizontalLayout_2;
QSpacerItem *verticalSpacer;
QSpacerItem *verticalSpacer_1;
QSpacerItem *verticalSpacer_2;
QSpacerItem *verticalSpacer_3;
QSpacerItem *verticalSpacer_4;
QSpacerItem *verticalSpacer_5;
QSpacerItem *verticalSpacer_6;
QSpacerItem *verticalSpacer_7;
QSpacerItem *verticalSpacer_8;
QSpacerItem *verticalSpacer_9;
QSpacerItem *verticalSpacer_10;
QSpacerItem *verticalSpacer_11;
QSpacerItem *verticalSpacer_12;
QSpacerItem *verticalSpacer_13;
QSpacerItem *verticalSpacer_14;
QSpacerItem *verticalSpacer_15;
QSpacerItem *verticalSpacer_16;
QSpacerItem *verticalSpacer_17;
QSpacerItem *verticalSpacer_18;
QSpacerItem *verticalSpacer_19;
QSpacerItem *verticalSpacer_20;
QSpacerItem *verticalSpacer_21;
QSpacerItem *horizontalSpacer;
QSpacerItem *horizontalSpacer_1;
QSpacerItem *horizontalSpacer_2;

//project addr input widget
QLabel *in_addr_reminder_label;
QLineEdit *in_addr_line_edit;
QPushButton *choose_inaddr_button;

//project addr output widget
QLabel *out_addr_reminder_label;
QLineEdit *out_addr_line_edit;
QPushButton *choose_outaddr_button;

//check mode choice list
QLabel *checkmode_choice_title_label;
QCheckBox * profession_box;
QCheckBox * student_box;

//output information choice list
QLabel *outinfo_choice_title_label;
QCheckBox * analyzeTime_box;
QCheckBox * bugType_box;
QCheckBox * bugLocation_box;
QCheckBox * checker_box;
QCheckBox * desc_box;
QCheckBox * statistics_box;
//reportformat information choice list
QLabel *reporformat_choice_title_label;
QCheckBox * html_box;
QCheckBox * json_box;
QCheckBox * text_box;

//checker choice list
QLabel *checker_choice_title_label;
QCheckBox * memory_box;
QCheckBox * divzero_box;
QCheckBox * deference_box;
QCheckBox * uninit_struct_box;
QCheckBox * array_bound_box;

QCommandLinkButton *start_button;
public:
    explicit MainWindow(bool & is_successful,QWidget *parent = 0);
    ~MainWindow();
private:
void stay_checked(QCheckBox* cb){
    cb->setChecked(true);
}

void   reverse_checkbox(QCheckBox* cb){
        if(cb->isChecked()==true){
            cb->setChecked(false);
        }else{
            cb->setChecked(true);
        }
}
QString get_console_addr();
void forbidden_empty();
void hide_checkers();
void reshow_checkers();
QString direct_exec_command(QString command);
bool command_to_file(QString command);
public slots:
    void change_godmodebox();
    void change_studentbox();
    void change_bugtypebox();
    void change_buglocationbox();
    void change_json_box();
    void change_html_box();
    void change_text_box();
    void change_analyzetime_box();
    void change_checker_box();
    void change_description_box();
    void change_statistics_box();
    void change_divzero_box();
    void change_memory_box();
    void change_deference_box();
    void change_uninit_box();
    void change_array_bound_box();

    void choose_in_addr();
    void choose_out_addr();
    void convert_to_english();
    void convert_to_chinese();
    void about_in_addr();
    void about_check_mode();
    void about_output_info();
    void about_output_format();
    void about_checker();
    void about_contact_us();
    void start_analyze();
    void show_history();
    void restore_default_options();
};

#endif // MAINWINDOW_H
