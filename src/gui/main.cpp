#include "MainWindow/mainwindow.h"
#include <QApplication>

/*****************************************************************************
This is source coode of Gui Module of HiSA(Higlhly intelligent Static Analyzer). 
Gui consists of 7 child modules
Thay are MainWindow,ExplanationDialog,DirTreeDialog,History,HistoryDialog,(Programmmed by Ke Cheng)
Preprocessor and FileSyetem.(Programmmed by Ruoheng Zhou)
******************************************************************************/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    bool is_successful;
    MainWindow w(is_successful);
    if (is_successful)w.show();
    return a.exec();
}
