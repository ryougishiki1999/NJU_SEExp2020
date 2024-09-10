#include "processdialog.h"
#include<QApplication>
#include<QMessageBox>
#include <QCloseEvent>
#include <cstdlib>
#include <ctime>
#include<QDebug>

// const value of ProcessDialog
const int DIALOG_HEIHGT=700;
const int DIALOG_WIDTH=900;
const int TITLE_X_COORD=0;
const int TITLE_Y_COORD=0;
const int TITLE_WIDTH=DIALOG_WIDTH;
const int TITLE_HEIGHT=70;
const int FONT_SIZE=18;
const int FONT_WEIGHT=75;
const int TEXT_BROWSER_X_COORD=20;
const int TEXT_BROWSER_Y_COORD=120;
const int TEXT_BROWSER_WIDTH=860;
const int TEXT_BROWSER_HEIGHT=510;
const int PROGRESS_BAR_X_COORD=20;
const int PROGRESS_BAR_Y_COORD=80;
const int PROGRESS_BAR_WIDTH=880;
const int PROGRESS_BAR_HEIGHT=30;
const int CANCEL_BUTTON_X_COORD=760;
const int CANCEL_BUTTON_Y_COORD=640;
const int CANCEL_BUTTON_WIDTH=120;
const int CANCEL_BUTTON_HEIGHT=40;
const int WAIT_TIME=3;
const int PROCRSS_UPDATE_MARGIN = 1500;

//initialize ProcessDialog and its Widget
ProcessDialog::ProcessDialog(Language lan,QWidget *parent) :
    QDialog(parent)
{
    this->lan=lan;
    //initialize the status of the process
    is_finished=false;
    is_canceled=false;
    Qt::WindowFlags flags=Qt::Dialog;flags |=Qt::WindowCloseButtonHint;setWindowFlags(flags);
    this->setWindowFlags(flags);
    this->setFixedSize(DIALOG_WIDTH, DIALOG_HEIHGT);

    //initialize title label
    title_label = new QLabel(this);
    title_label->setObjectName(QStringLiteral("title_label"));
    title_label->setGeometry(QRect(TITLE_X_COORD, TITLE_Y_COORD, TITLE_WIDTH, TITLE_HEIGHT));

    //initialize title label font
    QFont font;
    font.setFamily(QStringLiteral("Bahnschrift Light"));
    font.setPointSize(FONT_SIZE);
    font.setBold(true);
    font.setWeight(FONT_WEIGHT);

    //initialize title label
    title_label->setFont(font);
    title_label->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
    title_label->setAlignment(Qt::AlignCenter);

    //initialize text browser
    text_browser = new QTextBrowser(this);
    text_browser->setObjectName(QStringLiteral("text_browser"));
    text_browser->setGeometry(QRect(TEXT_BROWSER_X_COORD, TEXT_BROWSER_Y_COORD,
                                    TEXT_BROWSER_WIDTH, TEXT_BROWSER_HEIGHT));
    text_browser->setStyleSheet(QStringLiteral("background-color: rgb(240, 240, 240);\n"
    "border-color: rgb(240, 240, 240);"));

    //initialize progress bar
     progress_bar = new QProgressBar(this);
     progress_bar->setObjectName(QStringLiteral("progress_bar"));
     progress_bar->setGeometry(QRect(PROGRESS_BAR_X_COORD,PROGRESS_BAR_Y_COORD,
                                     PROGRESS_BAR_WIDTH, PROGRESS_BAR_HEIGHT));
     progress_bar->setValue(0);

     //initialize cancel button
     cancel_button = new QPushButton(this);
     cancel_button->setObjectName(QStringLiteral("cancel_button"));
     cancel_button->setGeometry(QRect(CANCEL_BUTTON_X_COORD, CANCEL_BUTTON_Y_COORD,
                                      CANCEL_BUTTON_WIDTH, CANCEL_BUTTON_HEIGHT));
     connect(cancel_button,SIGNAL(clicked(bool)),this,SLOT(cancel()));

     //Sets the text content of the widget according to the language
     switch(this->lan)
     {
     case Chinese:
         this->setWindowTitle(QStringLiteral("分析进度"));
         title_label->setText(QStringLiteral("Hisa正在进行分析"));
         cancel_button->setText(QStringLiteral("取消"));
         break;
     default:
         this->setWindowTitle(QStringLiteral("Analyzing Information"));
         title_label->setText(QStringLiteral("Hisa Analyzing"));
         cancel_button->setText(QStringLiteral("Cancel"));
     }
    cmd=new QProcess(this);
    connect(cmd, SIGNAL(readyRead()), this, SLOT(read_output()));
    connect(cmd, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(analyze_finish()));
    cmd->setReadChannel(QProcess::StandardOutput);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(PROCRSS_UPDATE_MARGIN);
}

//wait and judge if start successfully
//then return flag
bool ProcessDialog::exec_command(QString command)
{
       cmd->start(command);
       if(!cmd->waitForStarted(WAIT_TIME))
       {
          return false;
       }
       output ="";
       text_browser->setText(output);
       return true;
}

void ProcessDialog::read_output()
{
   output += cmd->readAllStandardOutput();
   text_browser->setText(output);
}

//show MessageBox to User to judge if he really wants to cancel
void ProcessDialog::cancel()
{
    if(!is_finished)
    {
        int r;
        //Sets the text content of the MessageBox according to the language
        switch(this->lan)
        {
        case Chinese:
            r=QMessageBox::warning(this,QStringLiteral("警告"),
                         QStringLiteral("项目分析还未完成，您确定要取消吗？"),
                                     QStringLiteral("确认"), QStringLiteral("取消"));
            break;
        default:
            r=QMessageBox::warning(this,QStringLiteral("warning"),
                         QStringLiteral("The program is not finished yet. Are you sure to cancel it?"),
                                     QStringLiteral("Yes"), QStringLiteral("No"));
        }
        if (r == QStringLiteral("Yes").toInt() 
            || r == QStringLiteral("确认").toInt())
        {
            cmd->kill();
            this->close();
            is_canceled=true;
        }
    }
    else
    {
    cmd->kill();
    this->close();
    }
}

//update progress bar every 1.5 seconds
void ProcessDialog::update()
{   
    srand(time(NULL));
    int new_value =progress_bar->value()+ (rand() % 6);
    if (new_value > 99)
        new_value = 99;
    progress_bar->setValue(new_value);
}

//remind user that analysis has been finished
void ProcessDialog::analyze_finish()
{
    timer->stop();
    if(!is_canceled)
    {
    is_finished=true;
    progress_bar->setValue(100);
    //Sets the text content of the MessageBox according to the language
    switch(this->lan)
    {
    case Chinese:
        QMessageBox::information(this, QStringLiteral("完成!"), QStringLiteral("项目已经分析完成!"),
                                 QStringLiteral("确认"));
        break;
    default:
        QMessageBox::information(this, QStringLiteral("Done!"),
                                 QStringLiteral("The project analysis has been finished"),
                                 QStringLiteral("OK"));
    }
    }
}

void ProcessDialog::closeEvent(QCloseEvent *)
{
    cmd->kill();
}

ProcessDialog::~ProcessDialog()
{
}
