#include "mainwindow.h"
#include<QFileDialog>
#include<QMessageBox>
#include"../ExplanationDialog/explanationdialog.h"
#include"../HistoryDialog/historydialog.h"
#include<stdio.h>
#include<QDateTime>
#include"../ProcessDialog/processdialog.h"
#include"../History/History.h"
#include "DirTreeDialog/dirtreedialog.h"
#include<set>
#include<string>
using namespace std;
#include<fstream>

// const value of mainWindow
const int WINDOW_WIDTH=800;
const int WINDOW_HEIGHT=560;
const QString html_enable=QStringLiteral("html");
const QString json_enable=QStringLiteral("json");
const QString text_enable=QStringLiteral("text");
const QString godmode_enable=QStringLiteral("-g");
const QString divZero_enable=QStringLiteral("hisa.DivZeroChecker");
const QString memory_enable=QStringLiteral("hisa.MemoryChecker");
const QString deference_enable=QStringLiteral("hisa.DereferenceChecker");
const QString arraybound_enable=QStringLiteral("hisa.ArrayBoundChecker");
const QString uninit_enable=QStringLiteral("hisa.UninitObjChecker");
const QString time_enable=QStringLiteral("time");
const QString checker_enable=QStringLiteral("checker");
const QString desc_enale=QStringLiteral("desc");
const QString stats_enable=QStringLiteral("stats");
const QString config_addr=QStringLiteral("./config.txt");
const int LINEEDIT_HEIGHT=30;

//initialize MainWindow and its widgets.
//initialize options and store the results in each map
MainWindow::MainWindow(bool &is_successful,QWidget *parent) :
    QMainWindow(parent)
{
    is_successful = true;
    this->console_version_addr = get_console_addr();
    if (console_version_addr.trimmed().size() == 0)
    {   
        QMessageBox::critical(this, QStringLiteral("Console Version Lost"), 
            QStringLiteral("Hisa Gui is supported by the console version,\n"
            "we can't find correct console version address in config.txt"));
        is_successful = false;
        return;
    }

    language=English;
    command="";
    // initialize mainwindow
    this->setMinimumSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    this->setWindowTitle(QStringLiteral("Hisa"));

    //initialize menu bar
    this->menu_bar=new QMenuBar(this);
    this->menu_bar->setObjectName(QStringLiteral("menu_bar"));
    this->menu_bar->setGeometry(QRect(0, 0, 800, 20));
    this->setMenuBar(menu_bar);

    //initialize help menu
    this->help_menu=new QMenu(QStringLiteral("Help"));
    this->help_menu->setObjectName(QStringLiteral("help_menu"));
    this->menuBar()->addMenu(help_menu);

    //initialize in addr help action
    this->in_addr_help=new QAction(this);
    this->in_addr_help->setObjectName(QStringLiteral("in_addr_help"));
    this->in_addr_help->setText(QStringLiteral("About project addr"));
    help_menu->addAction(in_addr_help);
    this->help_menu->addSeparator();
    QObject::connect(in_addr_help,SIGNAL(triggered()),this,SLOT(about_in_addr()));

    //initialize help menu
    this->options_help_menu=new QMenu(QStringLiteral("About Options"));
    this->options_help_menu->setObjectName(QStringLiteral("options_help_menu"));
    this->help_menu->addMenu(options_help_menu);
    this->help_menu->addSeparator();

    //initialize check mode help action
    this->check_mode_help=new QAction(this);
    this->check_mode_help->setObjectName(QStringLiteral("check_mode_help"));
    this->check_mode_help->setText(QStringLiteral("About Check Mode Options"));
    QObject::connect(check_mode_help,SIGNAL(triggered()),this,SLOT(about_check_mode()));
    options_help_menu->addAction(check_mode_help);
    this->options_help_menu->addSeparator();

    //initialize output info help action
    this->output_info_help=new QAction(this);
    this->output_info_help->setObjectName(QStringLiteral("output_info_help"));
    this->output_info_help->setText(QStringLiteral("About Output Info Options"));
    QObject::connect(output_info_help,SIGNAL(triggered()),this,SLOT(about_output_info()));
    options_help_menu->addAction(output_info_help);
    this->options_help_menu->addSeparator();

    //initialize format help action
    this->output_format_help=new QAction(this);
    this->output_format_help->setObjectName(QStringLiteral("output_format_help"));
    this->output_format_help->setText(QStringLiteral("About Report Type Options"));
    QObject::connect(output_format_help,SIGNAL(triggered()),this,SLOT(about_output_format()));
    options_help_menu->addAction(output_format_help);
    this->options_help_menu->addSeparator();


    //initialize checker help action
    this->checker_help=new QAction(this);
    this->checker_help->setObjectName(QStringLiteral("checker_help"));
    this->checker_help->setText(QStringLiteral("About Checker Options"));
    QObject::connect(checker_help,SIGNAL(triggered()),this,SLOT(about_checker()));
    options_help_menu->addAction(checker_help);
    this->options_help_menu->addSeparator();

    //initialize help menu
    this->contact_us_help=new QAction(this);
    this->contact_us_help->setObjectName(QStringLiteral("contact_us_help"));
    this->contact_us_help->setText(QStringLiteral("Contact us"));
    QObject::connect(contact_us_help,SIGNAL(triggered()),this,SLOT(about_contact_us()));
    help_menu->addAction(contact_us_help);

    //initialize settings menu_bar
    this->settings_menu=new QMenu(QStringLiteral("Settings"));
    this->settings_menu->setObjectName(QStringLiteral("settings_menu"));
    this->language_menu=new QMenu(QStringLiteral("Language"));
    this->language_menu->setObjectName(QStringLiteral("language_menu"));

    //initialize Chinese languag action
    this->action_Chinese=new QAction(language_menu);
    this->action_Chinese->setObjectName(QStringLiteral("action_Chinese"));
    this->action_Chinese->setText( QStringLiteral("Chinese"));
    this->language_menu->addAction(action_Chinese);
    this->language_menu->addSeparator();
    QObject::connect(action_Chinese,SIGNAL(triggered()),this,SLOT(convert_to_chinese()));

    //initialize English languag action
    this->action_English=new QAction(language_menu);
    this->action_English->setObjectName(QStringLiteral("action_English"));
    this->action_English->setText(QStringLiteral("English"));
    this->language_menu->addAction(action_English);
    QObject::connect(action_English,SIGNAL(triggered()),this,SLOT(convert_to_english()));

    //initialize History setting action
    this->action_history=new QAction(this);
    this->action_history->setObjectName(QStringLiteral("action_history"));
    this->action_history->setText(QStringLiteral("History"));
    QObject::connect(action_history,SIGNAL(triggered()),this,SLOT(show_history()));

    //initialize restore default options action
    this->action_rst_dft_ops=new QAction(this);
    this->action_rst_dft_ops->setObjectName(QStringLiteral("action_rst_dft_ops"));
    this->action_rst_dft_ops->setText(QStringLiteral("Restore Default Config"));
    QObject::connect(action_rst_dft_ops,SIGNAL(triggered()),this,SLOT(restore_default_options()));

    //insert actions and menu into settings menu
    this->settings_menu->addMenu(language_menu);
    this->settings_menu->addSeparator();
    this->settings_menu->addAction(action_history);
    this->settings_menu->addSeparator();
    this->settings_menu->addAction(action_rst_dft_ops);
    this->menu_bar->addMenu(settings_menu);

    //initialize central widget
    this->central_widget=new QWidget(this);
    this->central_widget->setObjectName(QStringLiteral("central_widget"));

    //initialize global gridLayout
    this->gridLayout_global = new QGridLayout(central_widget);
    this->gridLayout_global->setObjectName(QStringLiteral("gridLayout_global"));
    this->gridLayout_global->setSpacing(6);
    this->gridLayout_global->setContentsMargins(11, 11, 11, 11);

    //initialize horizontalLayout
    this->horizontalLayout = new QHBoxLayout();
    this->horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
    this->horizontalLayout->setSpacing(6);

    //initialize input address reminder
    this->in_addr_reminder_label=new QLabel(central_widget);
    this->in_addr_reminder_label->setObjectName(QStringLiteral("in_addr_reminder_label"));
    this->in_addr_reminder_label->setText(QStringLiteral("where is the project:  "));
    this->horizontalLayout->addWidget(in_addr_reminder_label);


    //initialize input address LineEdit
    this->in_addr_line_edit = new QLineEdit(central_widget);
    this->in_addr_line_edit->setObjectName(QStringLiteral("in_addr_line_edit"));
    this->in_addr_line_edit->setFixedHeight(LINEEDIT_HEIGHT);
    this->horizontalLayout->addWidget(in_addr_line_edit);

    //initialize inpiut address choice button
    this->choose_inaddr_button = new QPushButton(central_widget);
    this->choose_inaddr_button->setObjectName(QStringLiteral("choose_inaddr_button"));
    QObject::connect(this->choose_inaddr_button,SIGNAL(clicked()),this,SLOT(choose_in_addr()));
    this->choose_inaddr_button->setText(QStringLiteral("Browse Source..."));
    this->horizontalLayout->addWidget(choose_inaddr_button);

    this->gridLayout_global->addLayout(horizontalLayout,
                                       0, 0, 1, 1);

    //initialize horizontalLayout_2
    this->horizontalLayout_2 = new QHBoxLayout();
    this->horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
    this->horizontalLayout_2->setSpacing(6);

    //initialize output address reminder
    this->out_addr_reminder_label = new QLabel(central_widget);
    this->out_addr_reminder_label->setObjectName(QStringLiteral("out_addr_reminder_label"));
    this->out_addr_reminder_label->setText(QStringLiteral("where to put bug reort:"));
    this->horizontalLayout_2->addWidget(out_addr_reminder_label);

    //initialize output address LineEdit
    this->out_addr_line_edit = new QLineEdit(central_widget);
    this->out_addr_line_edit->setObjectName(QStringLiteral("out_addr_line_edit"));
    this->out_addr_line_edit->setFixedHeight(LINEEDIT_HEIGHT);
    this->horizontalLayout_2->addWidget(out_addr_line_edit);

    //initialize outpiut address choice button
    this->choose_outaddr_button = new QPushButton(central_widget);
    this->choose_outaddr_button->setObjectName(QStringLiteral("choose_outaddr_button"));
    QObject::connect(this->choose_outaddr_button,SIGNAL(clicked()),this,SLOT(choose_out_addr()));
    this->choose_outaddr_button->setText(QStringLiteral("Browse Source..."));
    this->horizontalLayout_2->addWidget(choose_outaddr_button);
    this->gridLayout_global->addLayout(horizontalLayout_2,
                                       3, 0, 1, 1);

    //initilize title font
    QFont title_font;
    title_font.setFamily(QStringLiteral("Agency FB"));
    title_font.setPointSize(10);
    title_font.setBold(true);
    title_font.setWeight(75);

    //initialize option list gridLayout
    this->gridLayout_option_table = new QGridLayout();
    this->gridLayout_option_table->setObjectName(QStringLiteral("gridLayout_option_table"));
    this->gridLayout_option_table->setSpacing(6);

    //fill the option list gridlayout
    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->gridLayout_option_table->addItem(verticalSpacer,
                                          1, 4, 1, 1);
    verticalSpacer_1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->gridLayout_option_table->addItem(verticalSpacer_1,
                                          1, 6, 1, 1);
    verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->gridLayout_option_table->addItem(verticalSpacer_2,
                                          1, 2, 1, 1);

    //initilize CheckMode choice List

    //initialize CheckMode choice list title
    this->checkmode_choice_title_label = new QLabel(central_widget);
    this->checkmode_choice_title_label->setObjectName(QStringLiteral("checkmode_choice_title_label"));
    this->checkmode_choice_title_label->setFont(title_font);
    this->checkmode_choice_title_label->setText(QStringLiteral("Check Mode"));
    this->gridLayout_option_table->addWidget(checkmode_choice_title_label,
                                            0, 0, 1, 1);

    //initialize student checkbox
    this->student_box= new QCheckBox(central_widget);
    this->student_box->setObjectName(QStringLiteral("student_box"));
    this->student_box->setText(QStringLiteral("Student"));
    this->student_box->setChecked(true);
    this->student_box->setToolTip(QStringLiteral("Use the Checkers written\n "
                                  "by our team to analyze"));
    QObject::connect(this->student_box,SIGNAL(clicked()),this,SLOT(change_studentbox()));
    this->gridLayout_option_table->addWidget(student_box,
                                            2, 0, 1, 1);

    //initialize profession checkbox
    this->profession_box = new QCheckBox(central_widget);
    this->profession_box->setObjectName(QStringLiteral("profession_box"));
    this->profession_box->setText(QStringLiteral("Godmode"));
    this->profession_box->setToolTip(QStringLiteral("Use the checkers in \n "
                                     "clang-static-analyzer to analyze.\n"
                                     "The checker options will be disable"));
    QObject::connect(this->profession_box,SIGNAL(clicked()),this,SLOT(change_godmodebox()));
    this->gridLayout_option_table->addWidget(profession_box,
                                            4, 0, 1, 1);
    check_mode_mp[godmode_enable]=profession_box->isChecked();
    //initilize output information choice List

    //initilize output information choice List title label
    this->outinfo_choice_title_label = new QLabel(central_widget);
    this->outinfo_choice_title_label->setObjectName(QStringLiteral("outinfo_choice_title_label"));
    this->outinfo_choice_title_label->setFont(title_font);
    this->outinfo_choice_title_label->setText(QStringLiteral("Output Info"));
    this->gridLayout_option_table->addWidget(outinfo_choice_title_label,
                                            0, 2, 1, 1);

    //initilize bugLocation checkbox
    this->bugLocation_box = new QCheckBox(central_widget);
    this->bugLocation_box->setObjectName(QStringLiteral("bugLocation_box"));
    this->bugLocation_box->setText(QStringLiteral("Bug Location"));
    this->bugLocation_box->setChecked(true);
    this->bugLocation_box->setToolTip(QStringLiteral("you can not cancel this option"));
    QObject::connect(this->bugLocation_box,SIGNAL(clicked()),this,SLOT(change_buglocationbox()));
    this->gridLayout_option_table->addWidget(bugLocation_box,
                                            2, 2, 1, 1);

    //initilize bugType checkbox
    this->bugType_box = new QCheckBox(central_widget);
    this->bugType_box->setObjectName(QStringLiteral("bugType_box"));
    this->bugType_box->setText(QStringLiteral("Bug Type"));
    this->bugType_box->setChecked(true);
    this->bugType_box->setToolTip(QStringLiteral("you can not cancel this option"));
    QObject::connect(this->bugType_box,SIGNAL(clicked()),this,SLOT(change_bugtypebox()));
    this->gridLayout_option_table->addWidget(bugType_box,
                                            4, 2, 1, 1);

    //initilize analyzeTime checkbox
    this->analyzeTime_box= new QCheckBox(central_widget);
    this->analyzeTime_box->setObjectName(QStringLiteral("analyzeTime_box"));
    this->analyzeTime_box->setText(QStringLiteral("Analyze Time"));
    this->gridLayout_option_table->addWidget(analyzeTime_box,
                                            6, 2, 1, 1);
    QObject::connect(this->analyzeTime_box,SIGNAL(clicked()),this,SLOT(change_analyzetime_box()));
    output_info_mp[time_enable]=analyzeTime_box->isChecked();

    //initilize checker checkbox
    this->checker_box = new QCheckBox(central_widget);
    this->checker_box->setObjectName(QStringLiteral("checker_box"));
    this->checker_box->setText(QStringLiteral("Checker"));
    this->gridLayout_option_table->addWidget(checker_box,
                                            8, 2, 1, 1);
    QObject::connect(this->checker_box,SIGNAL(clicked()),this,SLOT(change_checker_box()));
    output_info_mp[checker_enable]=checker_box->isChecked();

    //initilize description checkbox
    this->desc_box = new QCheckBox(central_widget);
    this->desc_box->setObjectName(QStringLiteral("desc_box"));
    this->desc_box->setText(QStringLiteral("Description"));
    this->gridLayout_option_table->addWidget(desc_box,
                                            10, 2, 1, 1);
    QObject::connect(this->desc_box,SIGNAL(clicked()),this,SLOT(change_description_box()));
    output_info_mp[desc_enale]=desc_box->isChecked();

    //initilize statistics checkbox
    this->statistics_box = new QCheckBox(central_widget);
    this->statistics_box->setObjectName(QStringLiteral("statistics_box"));
    this->statistics_box->setText(QStringLiteral("Statistics"));
    this->gridLayout_option_table->addWidget(statistics_box,
                                            12, 2, 1, 1);
    QObject::connect(this->statistics_box,SIGNAL(clicked()),this,SLOT(change_statistics_box()));
    output_info_mp[stats_enable]=statistics_box->isChecked();

    //initilize report format choice List

    //initilize report format choice List title label
    this->reporformat_choice_title_label = new QLabel(central_widget);
    this->reporformat_choice_title_label->setObjectName(QStringLiteral("reporformat_choice_title_label"));
    this->reporformat_choice_title_label->setFont(title_font);
    this->reporformat_choice_title_label->setText(QStringLiteral("Report Type"));
    this->gridLayout_option_table->addWidget(reporformat_choice_title_label,
                                            0, 4, 1, 1);
    //initilize html checkbox
    this->html_box = new QCheckBox(central_widget);
    this->html_box->setObjectName(QStringLiteral("html_box"));
    this->html_box->setText(QStringLiteral("Html"));
     this->html_box->setChecked(true);
    this->html_box->setToolTip(QStringLiteral("you must choose one of the formats"));
    QObject::connect(this->html_box,SIGNAL(clicked()),this,SLOT(change_html_box()));
    this->gridLayout_option_table->addWidget(html_box,
                                            2, 4, 1, 1);
    report_type_mp[html_enable]=html_box->isChecked();

    //initilize json checkbox
    this->json_box = new QCheckBox(central_widget);
    this->json_box->setObjectName(QStringLiteral("json_box"));
    this->json_box->setText(QStringLiteral("Json"));
    this->json_box->setToolTip(QStringLiteral("you must choose one of the formats"));
    QObject::connect(this->json_box,SIGNAL(clicked()),this,SLOT(change_json_box()));
    this->gridLayout_option_table->addWidget(json_box,
                                            4, 4, 1, 1);
    report_type_mp[json_enable]=json_box->isChecked();

    //initilize text checkbox
    this->text_box = new QCheckBox(central_widget);
    this->text_box->setObjectName(QStringLiteral("text_box"));
    this->text_box->setText(QStringLiteral("Text"));
    this->text_box->setToolTip(QStringLiteral("you must choose one of the formats"));
    QObject::connect(this->text_box,SIGNAL(clicked()),this,SLOT(change_text_box()));
    this->gridLayout_option_table->addWidget(text_box,
                                            6, 4, 1, 1);
    report_type_mp[text_enable]=text_box->isChecked();

    //initilize checker choice List

    //initilize checker choice List title
    this->checker_choice_title_label = new QLabel(central_widget);
    this->checker_choice_title_label->setObjectName(QStringLiteral("checker_choice_title_label"));
    this->checker_choice_title_label->setFont(title_font);
    this->checker_choice_title_label->setText(QStringLiteral("Checker"));
    this->gridLayout_option_table->addWidget(checker_choice_title_label,
                                            0, 6, 1, 1);

    //initilize divide and mod zero checker box
    this->divzero_box = new QCheckBox(central_widget);
    this->divzero_box->setObjectName(QStringLiteral("divzero_box"));
    this->divzero_box->setText(QStringLiteral("Divide Zero"));
    this->divzero_box->setChecked(true);
    this->divzero_box->setToolTip(QStringLiteral("divide zero\n"
                                  "mod zero"));
    QObject::connect(this->divzero_box,SIGNAL(clicked()),this,SLOT(change_divzero_box()));
    this->gridLayout_option_table->addWidget(divzero_box,
                                            2, 6, 1, 1);
    checker_mp[divZero_enable]=divzero_box->isChecked();

    //initilize memory checker box
    this->memory_box = new QCheckBox(central_widget);
    this->memory_box->setObjectName(QStringLiteral("memory_box"));
    this->memory_box->setText(QStringLiteral("Memory"));
    this->memory_box->setChecked(true);
    this->memory_box->setToolTip(QStringLiteral("double free\n"
                                 "use after free\n"
                                 "memory leak"));
    QObject::connect(this->memory_box,SIGNAL(clicked()),this,SLOT(change_memory_box()));
    this->gridLayout_option_table->addWidget(memory_box,
                                            4, 6, 1, 1);
    checker_mp[memory_enable]=memory_box->isChecked();

    //initilize null pointer deference box
    this->deference_box = new QCheckBox(central_widget);
    this->deference_box->setObjectName(QStringLiteral("deference_box"));
    this->deference_box->setText(QStringLiteral("Deference"));
    this->deference_box->setChecked(true);
    this->deference_box->setToolTip(QStringLiteral("null pointer dereference"));
    this->gridLayout_option_table->addWidget(deference_box,
                                            6, 6, 1, 1);
    QObject::connect(this->deference_box,SIGNAL(clicked()),this,SLOT(change_deference_box()));
    checker_mp[deference_enable]=deference_box->isChecked();

    //initilize uninitialized struct box
    this->uninit_struct_box = new QCheckBox(central_widget);
    this->uninit_struct_box->setObjectName(QStringLiteral("uninit_struct_box"));
    this->uninit_struct_box->setText(QStringLiteral("Uninit Struct"));
    this->uninit_struct_box->setChecked(true);
    this->uninit_struct_box->setToolTip(QStringLiteral("non union\n"
                                        "primitive uninitialized struct\n"
                                        "reference uninitialized"));
    this->gridLayout_option_table->addWidget(uninit_struct_box,
                                            8, 6, 1, 1);
    QObject::connect(this->uninit_struct_box,SIGNAL(clicked()),this,SLOT(change_uninit_box()));
    checker_mp[uninit_enable]=uninit_struct_box->isChecked();


    //initilize arrayBound box
    this->array_bound_box = new QCheckBox(central_widget);
    this->array_bound_box->setObjectName(QStringLiteral("array_bound_box"));
    this->array_bound_box->setText(QStringLiteral("Array Bound"));
    this->array_bound_box->setChecked(true);
    this->array_bound_box->setToolTip(QStringLiteral("array index out-of-bound"));
    this->gridLayout_option_table->addWidget(array_bound_box,
                                            10, 6, 1, 1);
    QObject::connect(this->array_bound_box,SIGNAL(clicked()),this,SLOT(change_array_bound_box()));
    checker_mp[arraybound_enable]=deference_box->isChecked();

    this->setCentralWidget(central_widget);

    //fill the option table layout with spacer
    verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->gridLayout_option_table->addItem(verticalSpacer_3,
                                          1, 0, 1, 1);
    verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->gridLayout_option_table->addItem(verticalSpacer_4,
                                          3, 2, 1, 1);
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    this->gridLayout_option_table->addItem(horizontalSpacer,
                                          0, 1, 1, 1);
    horizontalSpacer_1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    this->gridLayout_option_table->addItem(horizontalSpacer_1,
                                          0, 3, 1, 1);
    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    this->gridLayout_option_table->addItem(horizontalSpacer_2,
                                          0, 5, 1, 1);
    verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->gridLayout_option_table->addItem(verticalSpacer_5,
                                          3, 0, 1, 1);
    verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->gridLayout_option_table->addItem(verticalSpacer_6,
                                          7, 6, 1, 1);
    verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->gridLayout_option_table->addItem(verticalSpacer_7,
                                          11, 2, 1, 1);
    verticalSpacer_8 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->gridLayout_option_table->addItem(verticalSpacer_8,
                                          5, 2, 1, 1);
    verticalSpacer_9 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->gridLayout_option_table->addItem(verticalSpacer_9,
                                          3, 6, 1, 1);
    verticalSpacer_10 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->gridLayout_option_table->addItem(verticalSpacer_10,
                                          5, 6, 1, 1);
    verticalSpacer_11 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->gridLayout_option_table->addItem(verticalSpacer_11,
                                          9, 2, 1, 1);
    verticalSpacer_12 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->gridLayout_option_table->addItem(verticalSpacer_12,
                                          7, 2, 1, 1);
    verticalSpacer_13 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->gridLayout_option_table->addItem(verticalSpacer_13,
                                          3, 4, 1, 1);

    //fill the global layout with spacer
    this->gridLayout_global->addLayout(gridLayout_option_table,
                                       6, 0, 1, 1);
    verticalSpacer_14 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->gridLayout_global->addItem(verticalSpacer_14,
                                     4, 0, 1, 1);
    verticalSpacer_15 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->gridLayout_global->addItem(verticalSpacer_15,
                                     2, 0, 1, 1);
    verticalSpacer_16 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->gridLayout_global->addItem(verticalSpacer_16,
                                     11, 0, 1, 1);
    verticalSpacer_17 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->gridLayout_global->addItem(verticalSpacer_17,
                                     10, 0, 1, 1);
    verticalSpacer_18 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->gridLayout_global->addItem(verticalSpacer_18,
                                     8, 0, 1, 1);
    verticalSpacer_19 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->gridLayout_global->addItem(verticalSpacer_19,
                                     5, 0, 1, 1);
    verticalSpacer_20 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->gridLayout_global->addItem(verticalSpacer_20,
                                     7, 0, 1, 1);
    verticalSpacer_21 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    this->gridLayout_global->addItem(verticalSpacer_21,
                                     1, 0, 1, 1);

    //initialize start_to_analyze button
    this->start_button = new QCommandLinkButton(central_widget);
    this->start_button->setObjectName(QStringLiteral("start_button"));
    this->start_button->setText(QStringLiteral("GO!"));
    QObject::connect(this->start_button,SIGNAL(clicked()),this,SLOT(start_analyze()));
    this->start_button->setDescription(QStringLiteral("Start to analyze your project"));
    this->gridLayout_global->addWidget(start_button, 9, 0, 1, 1);


    //initialize bars
    main_tool_bar = new QToolBar(this);
    this->main_tool_bar->setObjectName(QStringLiteral("main_tool_bar"));
    this->addToolBar(Qt::TopToolBarArea, main_tool_bar);
    status_bar = new QStatusBar(this);
    this->status_bar->setObjectName(QStringLiteral("status_bar"));
    this->setStatusBar(status_bar);
}

//read the addr of console version
//user can not start gui if fail
QString MainWindow::get_console_addr()
{
    string file_name = "config.txt";
    ifstream fin;
    fin.open(file_name, ios::in);
    if (!fin)
    { // if file does not exist return empty
        return "";
    }
    string console_addr;
    fin >> console_addr;
    fin.close();
    return QString::fromLocal8Bit(console_addr.c_str());
}

//users can only choose one of student mode box and check mode box 
void MainWindow::change_godmodebox()
{
    if(this->profession_box->checkState()==
            this->student_box->checkState())
        reverse_checkbox(this->student_box);
    if(this->profession_box->isChecked())
    {
        hide_checkers();
        check_mode_mp[godmode_enable]=true;
    }
    else 
    {
        reshow_checkers();
        check_mode_mp[godmode_enable]=false;
    }
}

// users can only choose one of student mode box and check mode box
void MainWindow::change_studentbox()
{
    if(this->profession_box->checkState()==
            this->student_box->checkState())
        reverse_checkbox(this->profession_box);
    if(this->profession_box->isChecked())
    {
        hide_checkers();
        check_mode_mp[godmode_enable]=true;
    }
    else 
    {
        reshow_checkers();
        check_mode_mp[godmode_enable]=false;
    }
}

//users can't cancel bugtypebox
void MainWindow::change_bugtypebox(){
    stay_checked(this->bugType_box);
    switch (this->language)
    {
    case Chinese:
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("缺陷位置和缺陷类型为必选项"));
        break;
    default:
        QMessageBox::warning(this, QStringLiteral("forbidden"), QStringLiteral("Bug Location and Bug Type can't be canceled"));
    }
}

// users can't cancel buglocationbox
void MainWindow::change_buglocationbox()
{
    stay_checked(this->bugLocation_box);
    switch (this->language)
    {
    case Chinese:
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("缺陷位置和缺陷类型为必选项"));
        break;
    default:
        QMessageBox::warning(this, QStringLiteral("forbidden"), QStringLiteral("Bug Location and Bug Type can't be canceled"));
    }
}

void MainWindow::change_analyzetime_box()
{
    output_info_mp[time_enable]=analyzeTime_box->isChecked();
}

void MainWindow::change_checker_box()
{
    output_info_mp[checker_enable]=checker_box->isChecked();
}

void MainWindow::change_description_box()
{
    output_info_mp[desc_enale]=desc_box->isChecked();
}

void MainWindow::change_statistics_box()
{
    output_info_mp[stats_enable]=statistics_box->isChecked();
}

void MainWindow::change_json_box()
{
    report_type_mp[json_enable]=json_box->isChecked();
    forbidden_empty();
}

void MainWindow::change_html_box()
{
    report_type_mp[html_enable]=html_box->isChecked();
    forbidden_empty();
}

void MainWindow::change_text_box()
{
    report_type_mp[text_enable]=text_box->isChecked();
    forbidden_empty();
}

//it is uesd in ReportType choices list
//user must choose one of the reporttypes
void MainWindow::forbidden_empty()
{
    if(!this->html_box->isChecked() &&
       !this->json_box->isChecked() &&
       !this->text_box->isChecked()){
        this->html_box->setChecked(true);
        report_type_mp[html_enable]=true;
        switch (this->language)
        {
        case Chinese:
            QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("请至少选择一个输出类型"));
            break;
        default:
            QMessageBox::warning(this, QStringLiteral("forbidden"), QStringLiteral("you must chose one of the output type boxs"));
        }        
    }
}

void MainWindow::change_divzero_box()
{
    checker_mp[divZero_enable]=divzero_box->isChecked();
}

void MainWindow::change_memory_box()
{
    checker_mp[memory_enable]=memory_box->isChecked();
}

void MainWindow::change_deference_box()
{
    checker_mp[deference_enable]=deference_box->isChecked();
}

void MainWindow::change_array_bound_box()
{
    checker_mp[arraybound_enable]=array_bound_box->isChecked();
}

void MainWindow::change_uninit_box()
{
    checker_mp[uninit_enable]=uninit_struct_box->isChecked();
}

void MainWindow::choose_in_addr()
{
    QString project_addr= QFileDialog::getExistingDirectory(this,QStringLiteral("choose your project path"),"",0);
    this->in_addr_line_edit->setText(project_addr);
}

void MainWindow::choose_out_addr()
{
    QString report_addr= QFileDialog::getExistingDirectory(this,QStringLiteral("choose your project path"),"",0);
    this->out_addr_line_edit->setText(report_addr);
}

//firtly,check if input address or output address is empty
//secondly check if compile_commands.json file exists
//thirdly build document tree according to compile_commands.json
//then show user document tree and let him choose the files he wants to analyze
//last show processDialog and call console version to analyze 
void MainWindow::start_analyze()
{
    item his_item;
    if(this->in_addr_line_edit->text().trimmed().size()==0)
    {
        switch (this->language)
        {
        case Chinese:
            QMessageBox::warning(this, QStringLiteral("项目地址遗漏"), QStringLiteral("请输入您的项目文件所在地址"));
            break;
        default:
            QMessageBox::warning(this, QStringLiteral("Project Address Lost"), QStringLiteral("Please input your project address"));
        }        
        return;
    }
    QString compile_db_path;
    QString input_file_command=QStringLiteral("-input ");
    if(this->out_addr_line_edit->text().trimmed().size()==0)
    {
        switch (this->language)
        {
        case Chinese:
            QMessageBox::warning(this, QStringLiteral("输出地址遗漏"), QStringLiteral("请输入您的缺陷报告输出地址"));
            break;
        default:
            QMessageBox::warning(this, QStringLiteral("Output Address Lost"), QStringLiteral("Please input the report address"));
        }        
        return;
     }
    else{
       std::set<std::string> file_list;
       DirTreeDialog d(&file_list,string(in_addr_line_edit->text().trimmed().toLocal8Bit()),this->language,this);
       if(d.get_status()==INIT_FAIL) return;
       else {
           d.exec();
           if(file_list.empty()){
               if (d.get_status() == FINISHED)
                   switch (this->language)
                   {
                   case Chinese:
                       QMessageBox::warning(this, QStringLiteral("文件列表缺失"), QStringLiteral("请至少选择一个需要被检测的文件"));
                       break;
                   default:
                       QMessageBox::warning(this, QStringLiteral("File List Lost"), QStringLiteral("Please choose one file at least to analyze"));
                   }                          
               return;
           }
           else if (d.get_status()!=FINISHED)return ;
           compile_db_path=d.get_compiledb_path();
           for(std::set<std::string>::iterator it=file_list.begin();it!=file_list.end() ;it++)
           {
                input_file_command=input_file_command+QString::fromLocal8Bit(it->c_str())+QStringLiteral(" ");
           }
       }
    }
    command="";
    command = QStringLiteral("-compilation-database-path ") + compile_db_path.trimmed() + QStringLiteral(" ") +
            command+input_file_command+ QStringLiteral("-output --path ") + out_addr_line_edit->text().trimmed() +
            QStringLiteral(" ");

    //set attributes of his_item
    his_item.in_addr=in_addr_line_edit->text().trimmed().toLocal8Bit();
    his_item.out_addr=out_addr_line_edit->text().trimmed().toLocal8Bit();

    QString output_info_str=QStringLiteral("--info ");
    bool enable=false;
    for(std::map<QString,bool>::iterator i=output_info_mp.begin();i!=output_info_mp.end();i++)
    {
        if(i->second)
        {
            output_info_str+=i->first+QString(" ");
            enable=true;
        }
    }
    if(enable)command+=output_info_str;

    QString format_str=QStringLiteral("--format ");
    for(std::map<QString,bool>::iterator i=report_type_mp.begin();i!=report_type_mp.end();i++)
    {
        if(i->second)
        {
            format_str+=i->first+QStringLiteral(" ");
        }
    }
    command+=format_str;

    if(check_mode_mp[godmode_enable])
    {
        command+=godmode_enable;
        his_item.is_godmode=true;
    }
    else{
    QString checker_str=QStringLiteral("-enable-checker ");
        for(std::map<QString,bool>::iterator i=checker_mp.begin();i!=checker_mp.end();i++)
        {
            if(i->second)
            {
                checker_str += i->first + QStringLiteral(" ");
            }
        }
        command+=checker_str;
    }
    command=command.trimmed();
    QString exec_command=direct_exec_command(command);
    QDateTime current_time=QDateTime::currentDateTime();
    his_item.time=current_time.toString("yyyy-MM-dd,hh:mm:ss").toStdString();
    History temp_h;
    temp_h.add_history(his_item);
    ProcessDialog *p=new ProcessDialog(this->language,this);
    if(p->exec_command(exec_command))p->exec();
    else {
        command_to_file(command);
        switch (this->language)
        {
        case Chinese:
            QMessageBox::critical(this, QStringLiteral("执行错误"),
                QStringLiteral("Hisa无法对您的项目进行分析，"
                "可能是因为config.txt文件中的地址发生了错误"
               "我们将对应的命令输出在了您指定的输出地址，您可以调用手动调用命令行版本输入命令进行分析"));
            break;
        default:
            QMessageBox::critical(this, QStringLiteral("execution error"),
                                  QStringLiteral("Hisa can't execute your project,"
                                  "this may be caused by address error occurred in the config.txt,"
                                  "and we have written the corresponding command to the specified output file,"
                                  "you can use console version to execute,"
                                  "you can get mroe information from www.Hisa.com"));
        }                          
    }
}

//when the godmode is chosen
//checker list should hide
void MainWindow::hide_checkers()
{
    this->checker_choice_title_label->hide();
    this->memory_box->hide();
    this->divzero_box->hide();
    this->deference_box->hide();
    this->uninit_struct_box->hide();
    this->array_bound_box->hide();
}

// when the student mode is chosen after godmode
//checker list should reshow
void MainWindow::reshow_checkers()
{
    this->checker_choice_title_label->show();
    this->memory_box->show();
    this->divzero_box->show();
    this->deference_box->show();
    this->uninit_struct_box->show();
    this->array_bound_box->show();
}

//convert all the English remind and content to Chinese 
void MainWindow::convert_to_english()
{
    if(language==English)return;
    language=English;
    this->in_addr_help->setText(QStringLiteral("About project addr"));
    this->options_help_menu->setTitle(QStringLiteral("About Options"));
    this->check_mode_help->setText(QStringLiteral("About Check Mode Options"));
     this->output_info_help->setText(QStringLiteral("About Output Info Options"));
    this->output_format_help->setText(QStringLiteral("About Report Type Options"));
    this->checker_help->setText(QStringLiteral("About Checker Options"));
    this->contact_us_help->setText(QStringLiteral("Contact us"));
    this->help_menu->setTitle(QStringLiteral("Help"));
    this->settings_menu->setTitle(QStringLiteral("Settings"));
    this->language_menu->setTitle(QStringLiteral("Language"));
    this->action_Chinese->setText(QStringLiteral("Chinese"));
    this->action_English->setText(QStringLiteral("English"));
    this->action_history->setText(QStringLiteral("History"));
    this->action_rst_dft_ops->setText(QStringLiteral("Restore Default Config"));
    this->in_addr_reminder_label->setText(QStringLiteral("where is the project:  "));
    this->choose_inaddr_button->setText(QStringLiteral("Browse Source..."));
    this->out_addr_reminder_label->setText(QStringLiteral("where to put bug reort:"));
    this->choose_outaddr_button->setText(QStringLiteral("Browse Source..."));
    this->checkmode_choice_title_label->setText(QStringLiteral("Check Mode"));
    this->student_box->setText(QStringLiteral("Student"));
    this->profession_box->setText(QStringLiteral("Godmode"));
    this->outinfo_choice_title_label->setText(QStringLiteral("Output Info"));
    this->bugLocation_box->setText(QStringLiteral("Bug Location"));
    this->bugType_box->setText(QStringLiteral("Bug Type"));
    this->analyzeTime_box->setText(QStringLiteral("Analyze Time"));
    this->checker_box->setText(QStringLiteral("Checker"));
    this->desc_box->setText(QStringLiteral("Description"));
    this->statistics_box->setText(QStringLiteral("Statistics"));
    this->reporformat_choice_title_label->setText(QStringLiteral("Report Type"));
    this->checker_choice_title_label->setText(QStringLiteral("Checker"));
    this->divzero_box->setText(QStringLiteral("Divide Zero"));
    this->divzero_box->setToolTip(QStringLiteral("divide zero\n"
                                      "mod zero"));
    this->memory_box->setText(QStringLiteral("Memory"));
    this->memory_box->setToolTip(QStringLiteral("double free\n"
                                     "use after free\n"
                                     "memory leak"));
    this->deference_box->setText(QStringLiteral("Deference"));
    this->deference_box->setToolTip(QStringLiteral("null pointer dereference"));
    this->uninit_struct_box->setText(QStringLiteral("Uninit Struct"));
    this->uninit_struct_box->setToolTip(QStringLiteral("non union\n"
                                            "primitive uninitialized struct\n"
                                            "reference uninitialized"));
    this->array_bound_box->setText(QStringLiteral("Array Bound"));
    this->array_bound_box->setToolTip(QStringLiteral("array index out-of-bound"));
    this->start_button->setText(QStringLiteral("GO!"));
    this->start_button->setDescription(QStringLiteral("Start to analyse your project"));
    this->student_box->setToolTip(QStringLiteral("Use the Checkers written\n "
                                      "by our team to analyze"));
    this->profession_box->setToolTip(QStringLiteral("Use the checkers in \n "
                                         "clang-static-analyzer to analyze.\n"
                                         "The checker options will be disable"));
    this->bugLocation_box->setToolTip(QStringLiteral("you can not cancel this option"));
    this->bugType_box->setToolTip(QStringLiteral("you can not cancel this option"));
    this->html_box->setToolTip(QStringLiteral("you must choose one of the formats"));
    this->json_box->setToolTip(QStringLiteral("you must choose one of the formats"));
    this->text_box->setToolTip(QStringLiteral("you must choose one of the formats"));
}

// convert all the Chinese remind and content to English
void MainWindow::convert_to_chinese()
{
    if(language==Chinese)return;
    language=Chinese;
    in_addr_help->setText(QStringLiteral("关于项目地址"));
    options_help_menu->setTitle(QStringLiteral("关于选项"));
    check_mode_help->setText(QStringLiteral("关于检测模式选项"));
    output_info_help->setText(QStringLiteral("关于输出信息选项"));
    output_format_help->setText(QStringLiteral("关于报告格式选项"));
    checker_help->setText(QStringLiteral("关于检测器选项"));
    contact_us_help->setText(QStringLiteral("联系我们"));
    help_menu->setTitle(QStringLiteral("帮助"));
    settings_menu->setTitle(QStringLiteral("设置"));
    language_menu->setTitle(QStringLiteral("语言"));
    action_Chinese->setText(QStringLiteral("中文(简体)"));
    action_English->setText(QStringLiteral("英文"));
    action_history->setText(QStringLiteral("历史记录"));
    action_rst_dft_ops->setText(QStringLiteral("恢复默认设置"));
    this->in_addr_reminder_label->setText(QStringLiteral("待检测项目地址:  "));
    this->choose_inaddr_button->setText(QStringLiteral("打开..."));
    this->out_addr_reminder_label->setText(QStringLiteral("检测报告输出地址:"));
    this->choose_outaddr_button->setText(QStringLiteral("打开..."));
    this->checkmode_choice_title_label->setText(QStringLiteral("检测模式"));
    this->student_box->setText(QStringLiteral("学生版"));
    this->profession_box->setText(QStringLiteral("专业版"));
    this->outinfo_choice_title_label->setText(QStringLiteral("输出信息"));
    this->bugLocation_box->setText(QStringLiteral("缺陷位置"));
    this->bugType_box->setText(QStringLiteral("缺陷种类"));
    this->analyzeTime_box->setText(QStringLiteral("检测时间"));
    this->checker_box->setText(QStringLiteral("检测器"));
    this->desc_box->setText(QStringLiteral("缺陷描述"));
    this->statistics_box->setText(QStringLiteral("统计信息"));
    this->reporformat_choice_title_label->setText(QStringLiteral("报告格式"));
    this->checker_choice_title_label->setText(QStringLiteral("检测器"));
    this->divzero_box->setText(QStringLiteral("除模0检测器"));
    this->divzero_box->setToolTip(QStringLiteral("除0\n"
                                      "模0"));
    this->memory_box->setText(QStringLiteral("内存检测器"));
    this->memory_box->setToolTip(QStringLiteral("内存重复释放\n"
                                     "内存释放后使用\n"
                                     "内存溢出"));
    this->deference_box->setText(QStringLiteral("解引用检测器"));
    this->deference_box->setToolTip(QStringLiteral("空指针解引用"));
    this->uninit_struct_box->setText(QStringLiteral("初始化检测器"));
    this->uninit_struct_box->setToolTip(QStringLiteral("结构体成员未初始化\n"
                                            "内置类型变量未初始化\n"
                                            "引用未初始化"));
    this->array_bound_box->setText(QStringLiteral("数组越界检测器"));
    this->array_bound_box->setToolTip(QStringLiteral("数组下标访问越界"));
    this->start_button->setText(QStringLiteral("启动!"));
    this->start_button->setDescription(QStringLiteral("开始检测您的项目"));
    this->student_box->setToolTip(QStringLiteral("使用我们团队自主开发的\n "
                                      "检测器进行缺陷检测"));
    this->profession_box->setToolTip(QStringLiteral("使用 clang 内置的 \n "
                                         "clang-static-analyzer 进行缺陷检测.\n"
                                         "选中该项后\n检测器选项列表将不可见"));
    this->bugLocation_box->setToolTip(QStringLiteral("该选项为必选选项"));
    this->bugType_box->setToolTip(QStringLiteral("该选项为必选选项"));
    this->html_box->setToolTip(QStringLiteral("您必须选择一个输出格式"));
    this->json_box->setToolTip(QStringLiteral("您必须选择一个输出格式"));
    this->text_box->setToolTip(QStringLiteral("您必须选择一个输出格式"));
}

//give user information about input address
//there is information in two languages to be chosen
void MainWindow::about_in_addr()
{
    QString content;
    QString title;
    QString button_text;
    switch(this->language){
        case Chinese:
            title=QStringLiteral("关于项目地址");
            content=QStringLiteral("如果项目有复杂的依赖，那么项目地址的目录中必须包含"
                    "用cmake或其他工具生成的 compile_commands.json");
            button_text=QStringLiteral("确认");
            break;
        default:
            title=QStringLiteral("About Project Address");
            content=QStringLiteral("If the project has complex dependencies,the directory of "
                    "the project catalog must contain "
                    "compile_commands.json generated by cmake or otehr tools");
            button_text=QStringLiteral("OK");
    }
    ExplanationDialog *e_d=new ExplanationDialog(this,title,content,button_text);
    e_d->show();
}

// give user information about checkmode choices list
// there is information in two languages to be chosen
void MainWindow::about_check_mode()
{
    QString content;
    QString title;
    QString button_text;
    switch(this->language){
        case Chinese:
            title=QStringLiteral("关于检测模式");
            content=QStringLiteral("检测模式分为学生版和专业版，"
                    "只能选择其中之一进行缺陷检测：\n\n"
                    "学生版：是由南京大软件工程综合实验团队合作开发的，"
                    "不支持过程间的缺陷检测并且"
                    "其中部分检测功能不完善，漏报率，误报率较高\n\n"
                    "专业版：直接使用clang-static-anlyzer进行缺陷检测，"
                    "不支持过程间的缺陷检测，并且当选中该选项后默认开启所有检测器，"
                    "因此当选中该选项后检测器选择列表被隐藏");
            button_text=QStringLiteral("确认");
            break;
        default:
            title=QStringLiteral("About Check Mode");
            content=QStringLiteral("The check mode is divided into student version and godmode version,"
                    "you can only choose one of them to analyze: \n\n"
                    "Student：jointly developed by the integrated experimental team "
                    "of software engineering at nanjing university,"
                    "analysis between different processes is not supported and"
                    "the checkers' performance is far from perfection,"
                    "false positive rate and false negative rate are high \n\n"
                    "GodMode：directly use clang-static-anlyzer to analyze,,"
                    "analysis between processes is not supported."
                    "And all the checkers are turned on by default when this option is selected,"
                    "so the detector selection list is hidden when this option is selected ");
            button_text=QStringLiteral("OK");
    }
    ExplanationDialog *e_d=new ExplanationDialog(this,title,content,button_text);
    e_d->show();
}

// give user information about output address
// there is information in two languages to be chosen
void MainWindow::about_output_info()
{
    QString content;
    QString title;
    QString button_text;
    switch(this->language){
        case Chinese:
            title=QStringLiteral("关于输出信息");
            content=QStringLiteral("输出信息共有6个选项：\n\n"
                    "缺陷位置：必选项，输出缺陷所在的文件名和行数\n\n"
                    "缺陷种类：必选项，输出缺陷的种类\n\n"
                    "检测时间：可选项，输出缺陷检测所用的总时间，以秒为单位\n\n"
                    "检测器：可选项，输出检测出每个缺陷的检测器\n\n"
                    "缺陷描述：可选项，输出对每个缺陷的缺陷种类的描述\n\n"
                    "统计信息：可选项，输出所有的缺陷种类以及每种缺陷在所有缺陷中所占的比例");
            button_text=QStringLiteral("确认");
            break;
        default:
            title=QStringLiteral("About Output Information");
            content=QStringLiteral("6 options for output information: \n\n"
                    "Bug Location: required, output file name and line number of bugs\n\n"
                    "Bug Type: required, type of bugs \n\n"
                    "Analyze Time: optional, output total bug analyze time in seconds \n\n"
                    "Checker: optional, output the checker of each bug\n\n"
                    "Description: optional output description of each bug\n\n"
                    "Statistics: optional, output all bug types and the percentage of each type in all bugs ");
            button_text=QStringLiteral("OK");
    }
    ExplanationDialog *e_d=new ExplanationDialog(this,title,content,button_text);
    e_d->show();
}

// give user information about report type choices list
// there is information in two languages to be chosen
void MainWindow::about_output_format()
{
    QString content;
    QString title;
    QString button_text;
    switch(this->language){
        case Chinese:
            title=QStringLiteral("关于输出格式");
            content=QStringLiteral("我们为用户提供了三种报告的输出格式供用户选择：\n\n"
                    "Html：以.html为后缀的文件输出，可以通过超链接在统计信息和详细代码之间进行跳转\n\n"
                    "Json：使用.json为后缀的文件输出，方便用户将检测结果集成进自己的工作流中\n\n"
                    "Text：使用.txt为后缀的文件输出，方便用户生成报告从而进行阅览和展示");
            button_text=QStringLiteral("确认");
            break;
        default:
            title=QStringLiteral("About Output Type");
            content=QStringLiteral("We provide users with three report output formats to choose from:\n\n"
                    "Html: file output with.html suffix, "
                    "you can jump between statistics and detailed code by hyperlink\n\n"
                    "Json: output with .json suffixed files to "
                    "make it easier for users to integrate test results into their own workflows.\n\n"
                    "Text: use file output with the suffix .txt "
                    "to facilitate user report generation for viewing and presentation");
            button_text=QStringLiteral("OK");
    }
    ExplanationDialog *e_d=new ExplanationDialog(this,title,content,button_text);
    e_d->show();
}

// give user information about checker choices list
// there is information in two languages to be chosen
void MainWindow::about_checker()
{
    QString content;
    QString title;
    QString button_text;
    switch(this->language){
        case Chinese:
            title=QStringLiteral("关于检测器");
            content=QStringLiteral("在专业版本中开启clang-static-analyzer中的所有检测器进行检测\n\n"
                    "在学生版本中可以自行选择学生自主开发的检测器：\n\n"
                    "除模0检测器：对代码中过程内发生的整型常量和变量的除0和模0缺陷进行检测\n\n"
                    "内存检测器：对代码中过程内发生的重复释放内存，内存泄露和内存释放后使用缺陷进行检测\n\n"
                    "解引用检测器：对代码中过程内发生的空指针解引用缺陷进行检测\n\n"
                    "初始化检测器：对代码中过程内发生的变量未初始化(包括结构体成员)和引用未初始化进行检测\n\n"
                    "数组越界检测器：对代码中过程内发生的数组下标越界错误进行检测");
            button_text=QStringLiteral("确认");
            break;
        default:
            title=QStringLiteral("About Checker");
            content=QStringLiteral("Turn on all detectors in clang-static-analyzer for detection in the GodMode version\n\n"
                    "Checkers can be selected from the student version.\n\n"
                    "Divide and Mod 0 Checker: analysis of defects of integer constants and"
                    " variables in each process of code with divding 0 and modulating 0 \n\n"
                    "Memory Checker: analysis of double free, memory leaks, "
                    "and use-after-free occurring in each process of code \n\n"
                    "Deference Checker: analysis of null pointer deference in each process of code\n\n"
                    "Uninitialization Checker: analysis uninitialized variables (including struct members) "
                    "and uninitialized references in each process of code \n\n"
                    "ArrayBound Checker：analysis of array index out-of-bound error in each process of code");
            button_text=QStringLiteral("OK");
    }
    ExplanationDialog *e_d=new ExplanationDialog(this,title,content,button_text);
    e_d->show();
}

// give user information about developer
// there is information in two languages to be chosen
void MainWindow::about_contact_us()
{
    QString content;
    QString title;
    QString button_text;
    switch(this->language){
        case Chinese:
            title=QStringLiteral("联系我们");
            content=QStringLiteral("本项目由南京大学的5位同学在助教何杰煊的帮助下完成，"
                    "在使用的过程中遇到任何问题请发送邮件至："
                    "kecheng.nju@gmail.com，我们会尽快回复。\n\n"
                    "并且我们会尽快完成用户手册与官网的搭建，方便用户使用");
            button_text=QStringLiteral("确认");
            break;
        default:
            title=QStringLiteral("Contact Us");
            content=QStringLiteral("The project was completed by five students from nanjing university "
                    "with the help of assistant professor Jiexuan He."
                    "If you meet any problems, please send an email to:"
                    "Kecheng.nju@gmail.com, we will reply as soon as possible.\n\n"
                    "And we will complete the user manual and "
                    "official website as soon as possible for the convenience of users");
            button_text=QStringLiteral("OK");
    }
    ExplanationDialog *e_d=new ExplanationDialog(this,title,content,button_text);
    e_d->show();
}

// show HistoryDialog to user
void MainWindow::show_history()
{
    HistoryDialog *h_d;
    h_d=new HistoryDialog(this->language);
    h_d->exec();
    delete h_d;
}

//generate finally command
QString MainWindow::direct_exec_command(QString command)
{
    return console_version_addr+" "+command;
}

//when console version address lost
//output console command to outputaddress
bool MainWindow::command_to_file(QString exec_command)
{
    QString command_file_name=(out_addr_line_edit->text().trimmed()+QStringLiteral("/command.txt"));
    FILE *command_file=fopen(command_file_name.toStdString().c_str(),
                             "w");
    if(command_file==NULL)return false;
    fprintf(command_file,"%s",exec_command.toStdString().c_str());
    fclose(command_file);
    return true;
}

//reset language to English
//reset to default choices
void MainWindow::restore_default_options()
{
    //restore language option
    convert_to_english();

    //restore CheckMode options
    student_box->setChecked(true);
    if(profession_box->isChecked())change_studentbox();

    //restore output information options
    analyzeTime_box->setChecked(false);
    checker_box->setChecked(false);
    desc_box->setChecked(false);
    statistics_box->setChecked(false);

    //restore output type options
    html_box->setChecked(true);
    json_box->setChecked(false);
    text_box->setChecked(false);

    //restore checker optons
    divzero_box->setChecked(true);
    memory_box->setChecked(true);
    deference_box->setChecked(true);
    uninit_struct_box->setChecked(true);
    array_bound_box->setChecked(true);
}

MainWindow::~MainWindow(){
}
