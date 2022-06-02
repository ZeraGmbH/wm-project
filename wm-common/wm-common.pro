TEMPLATE = lib
CONFIG += staticlib

QT += qt3support 

HEADERS = \
    bigletter.h \
    complex.h \
    cbiface.h \
    cmdinterpret.h \
    dspactvalues.h \
    movingwindowfilter.h \
    parse.h \
    scpi.h \
    zeraglobal.h

SOURCES = \
    bigletter.cpp \
    complex.cpp \
    cmdinterpret.cpp \
    dspactvalues.cpp \
    movingwindowfilter.cpp \
    parse.cpp \
    scpi.cpp

