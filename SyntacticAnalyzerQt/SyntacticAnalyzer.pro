#-------------------------------------------------
#
# Project created by QtCreator 2016-12-07T10:10:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SyntacticAnalyzer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    lexical_analyzer.cpp \
    parsing_table.cpp \
    exception.cpp

HEADERS  += mainwindow.h \
    lexical_analyzer.h \
    parsing_table.h \
    exception.h \
    config.h

FORMS    += mainwindow.ui
