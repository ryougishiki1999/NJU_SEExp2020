#include "explanationdialog.h"
#include<QPushButton>

/*****************************************************************************
This is source coode of ExplanationDialog.
ExplanationDialog shows the help information for users to use HiSA.
This file contains the initialization of all the members of ExplanationDialog
and the implementation of all of its functions
******************************************************************************/

// const value of ExplanationDialog
const int DIALOG_WIDTH=500;
const int DIALOG_HEIGHT=600;
const int TITLE_FONT_SIZE=14;
const int TITLE_FONT_WEIGHT=75;
const int CENTER_WIDGET_X = 10;
const int CENTER_WIDGET_Y = 10;
const int CENTER_WIDGET_WIDTH = 480;
const int CENTER_WIDGET_HEIGHT = 580;
const int TEXT_FONT_SIZE=12;
const int BUTTON_HEIGHT=40;
const int CONTENT_MARGIN = 0;

//initialize ExplanationDialog and its widgets
ExplanationDialog::ExplanationDialog(QWidget *parent,QString title,QString content,QString button_text) :
    QDialog(parent)
{
    //initialize Dialog
    this->setFixedWidth(DIALOG_WIDTH);
    this->setFixedHeight(DIALOG_HEIGHT);
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowCloseButtonHint;setWindowFlags(flags);
    this->setWindowFlags(flags);
    this->setWindowTitle(title);

    //initialize central widget
    widget = new QWidget(this);
    widget->setObjectName(QStringLiteral("widget"));
    widget->setGeometry(QRect(CENTER_WIDGET_X, CENTER_WIDGET_Y, 
                        CENTER_WIDGET_WIDTH, CENTER_WIDGET_HEIGHT));

    //initialize vBoxlayout
    vbox_layout = new QVBoxLayout(widget);
    vbox_layout->setObjectName( QStringLiteral ("vbox_layout"));
    vbox_layout->setContentsMargins(CONTENT_MARGIN, CONTENT_MARGIN, 
                                    CONTENT_MARGIN, CONTENT_MARGIN);

    //initialize title label
    this->title_label=new QLabel(widget);
    title_label->setObjectName( QStringLiteral ("title_label"));

    //initialize title label font
    QFont title_font;
    title_font.setFamily(QStringLiteral("Agency FB"));
    title_font.setPointSize(TITLE_FONT_SIZE);
    title_font.setBold(true);
    title_font.setWeight(TITLE_FONT_WEIGHT);

    //set font for title label
    title_label->setFont(title_font);
    this->title_label->setText(title);
    vbox_layout->addWidget(title_label);

    //initialize text font
    QFont text_font;
    text_font.setFamily(QStringLiteral("Bahnschrift"));
    text_font.setPointSize(TEXT_FONT_SIZE);

    //initialize TextBrowser
    this->dialog_content=new QTextBrowser(widget);
    this->dialog_content->setObjectName( QStringLiteral ("dialog_content"));
    this->dialog_content->setText(content);
    this->dialog_content->setFont(text_font);
    vbox_layout->addWidget(dialog_content);

    //initialize confrm button
    this->confirm_button=new QPushButton(widget);
    this->confirm_button->setObjectName( QStringLiteral ("confirm_button"));
    this->confirm_button->setFixedHeight(BUTTON_HEIGHT);
    this->confirm_button->setText(button_text);

    //initialize font of button
    QFont button_font;
    button_font.setFamily(QStringLiteral("Agency FB"));
    button_font.setPointSize(TITLE_FONT_SIZE);
    this->confirm_button->setFont(button_font);
    vbox_layout->addWidget(confirm_button);
    connect(confirm_button,SIGNAL(clicked()),this,SLOT(click_confirm_button()));
}

void ExplanationDialog::click_confirm_button()
{
    this->close();
}

ExplanationDialog::~ExplanationDialog()
{
}
