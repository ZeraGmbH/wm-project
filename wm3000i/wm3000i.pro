TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt debug

INCLUDEPATH	+= .


HEADERS	+= \
	confdata.h \
	devserver.h \
	dspiface.h \
	eparameter.h \
	formatinfo.h \
	logfileview.h \
	ownerror.h \
	pcbiface.h \
	wmglobal.h \
	wm3000i.h \
	zhclientsocket.h \
	zhserveriface.h \
	zerainfo.h \
	wm3000scpiface.h \
	scpiface.h \
    wmviewbase.h \
    wmmeasvaluesbase.h \
    wmoeviewbase.h \
    wmrawactualvalbase.h \
    en61850monitor.h \
    confdialogbase.h \
    rangedialogbase.h \
    versionviewbase.h \
    wmmeasconfigbase.h \
    wmrawactualconfigbase.h \
    wmoffsetvalbase.h \

SOURCES	+= \
	confdata.cpp \
	devserver.cpp \
	dspiface.cpp \
	eparameter.cpp \
	formatinfo.cpp \
	logfileview.cpp \
	main.cpp \
	ownerror.cpp \
	pcbiface.cpp \
	wm3000i.cpp \
	zhclientsocket.cpp \
	zhserveriface.cpp \
	zerainfo.cpp \
	wm3000scpiface.cpp \
	scpiface.cpp \
    wmviewbase.cpp \
    wmmeasvaluesbase.cpp \
    wmoeviewbase.cpp \
    wmrawactualvalbase.cpp \
    en61850monitor.cpp \
    rangedialogbase.cpp \
    versionviewbase.cpp \
    wmmeasconfigbase.cpp \
    wmrawactualconfigbase.cpp \
    wmoffsetvalbase.cpp \
    confdialogbase.cpp


FORMS	= confdialogbase.ui \
	en61850monitor.ui \
	rangedialogbase.ui \
	versionviewbase.ui \
	wmmeasconfigbase.ui \
	wmmeasvaluesbase.ui \
	wmoeviewbase.ui \
	wmrawactualconfigbase.ui \
	wmrawactualvalbase.ui \
        wmoffsetvalbase.ui \
	wmviewbase.ui


######################################################################
# Automatically generated by qmake (1.07a) Thu May 12 09:23:36 2005
######################################################################


TRANSLATIONS    = wm3000i_gb.ts \
                  wm3000i_pl.ts

# Input
#The following line was inserted by qt3to4
QT += xml  qt3support 

INCLUDEPATH += ../wm-common
LIBS += -L../wm-common -lwm-common

target.path = /usr/bin
INSTALLS += target
