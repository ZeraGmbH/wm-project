QT += testlib
QT -= gui

# enable make check
CONFIG += testcase

CONFIG += no_testcase_installs

CONFIG += qt console warn_on depend_includepath
CONFIG -= app_bundle

TEMPLATE = app


HEADERS = \
    main-unittest.h

SOURCES = \
    main-unittest.cpp

