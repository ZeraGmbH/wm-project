TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt

INCLUDEPATH	+= .


HEADERS	+= \
    devserver.h \
    pcbiface.h \
    wm3kscpispecial.h \
    wmglobal.h \
    wm3000i.h \
    wmioffsetcustomlabels.h \
    wmrawactualvalbase.h \
    confdialogbase.h \
    rangedialogbase.h \
    wmviewbasei.h \

SOURCES	+= \
    devserver.cpp \
    main.cpp \
    pcbiface.cpp \
    wm3000i.cpp \
    wm3kscpispecial.cpp \
    wmioffsetcustomlabels.cpp \
    wmrawactualvalbase.cpp \
    rangedialogbase.cpp \
    confdialogbase.cpp \
    wmviewbasei.cpp \

FORMS	= confdialogbase.ui \
	rangedialogbase.ui \
	wmrawactualvalbase.ui \


######################################################################
# Automatically generated by qmake (1.07a) Thu May 12 09:23:36 2005
######################################################################


TRANSLATIONS    = wm3000i_gb.ts \
                  wm3000i_pl.ts

QT += widgets network xml qt3support core

INCLUDEPATH += \
    ../wm-common \
    ../wm-common/client-access \
    ../wm-common/dsp \
    ../wm-common/gui \
    ../wm-common/just \
    ../wm-common/scales-and-units \
    ../wm-common/scpi/definitions \
    ../wm-common/scpi \
    ../wm-common/settings \
    ../wm-common/service-interfaces

# Qt4.8's QString  on modern env causes loads of warnings
QMAKE_CXXFLAGS += -Wno-deprecated-copy

LIBS += -L../wm-common -lwm-common

target.path = /usr/bin
INSTALLS += target
