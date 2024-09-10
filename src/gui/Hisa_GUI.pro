#-------------------------------------------------
#
# Project created by QtCreator 2020-05-12T17:36:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Hisa_GUI
TEMPLATE = app
CONFIG += console c++17

SOURCES += main.cpp\
    MainWindow/mainwindow.cpp \
    ExplanationDialog/explanationdialog.cpp \
    HistoryDialog/historydialog.cpp \
    ProcessDialog/processdialog.cpp \
    History/History.cpp \
    PreProcessor/FileSystemTree.cpp \
    PreProcessor/Preprocessor.cpp \
    ../core/Json/JsonProc.cpp \
    DirTreeDialog/dirtreedialog.cpp

HEADERS  += MainWindow/mainwindow.h \
    ExplanationDialog/explanationdialog.h \
    HistoryDialog/historydialog.h \
    ProcessDialog/processdialog.h \
    History/History.h \
    Language/language.h \
    PreProcessor/FileSystemTree.h \
    PreProcessor/Preprocessor.h \
    ../core/Json/JsonProc.h \
    DirTreeDialog/dirtreedialog.h



