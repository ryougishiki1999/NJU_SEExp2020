#ifndef PROCESSDIALOG_H
#define PROCESSDIALOG_H

#include<QProcess>
#include <QDialog>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include"Language/language.h"
#include<QTimer>

class ProcessDialog : public QDialog
{
    Q_OBJECT
private:
    QProcess* cmd;
    QString output;
    Language lan;
    QTimer* timer;
  public:
    bool is_canceled;
    QLabel *title_label;
    QTextBrowser *text_browser;
    QProgressBar *progress_bar;
    QPushButton *cancel_button;
public:
    explicit ProcessDialog(Language lan=English,QWidget *parent = 0);
    bool exec_command(QString command);
    bool is_finished;
    ~ProcessDialog();
protected:
     void closeEvent(QCloseEvent *event);
public slots:
    void read_output();
    void cancel();
    void analyze_finish();
    void update();
};

#endif // PROCESSDIALOG_H
