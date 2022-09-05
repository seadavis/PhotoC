TEMPLATE += app
QT += gui declarative
SOURCES += main.cpp
TEMPLATE = app
TARGET = field_of_view

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
HEADERS += window.h
SOURCES += window.cpp