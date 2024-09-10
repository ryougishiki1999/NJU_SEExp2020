#ifndef EXPLANATIONDIALOG_H
#define EXPLANATIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>


namespace Ui {
class ExplanationDialog;
}

class ExplanationDialog : public QDialog
{
    Q_OBJECT
public:
      QTextBrowser *dialog_content;
      QPushButton *confirm_button;
      QLabel *title_label;
      QWidget *widget;
      QVBoxLayout *vbox_layout;
public slots:
    void click_confirm_button();
public:
    explicit ExplanationDialog(QWidget *parent = 0,
                               QString title="",
                               QString content="",
                               QString button_text="");
    ~ExplanationDialog();

};

#endif // EXPLANATIONDIALOG_H
