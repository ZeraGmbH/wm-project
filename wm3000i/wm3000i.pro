TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt debug

INCLUDEPATH	+= .


HEADERS	+= \
	devserver.h \
	formatinfo.h \
	ownerror.h \
	pcbiface.h \
	wmglobal.h \
	wm3000i.h \
	zerainfo.h \
	wm3000scpiface.h \
    wmviewbase.h \
    wmmeasvaluesbase.h \
    wmrawactualvalbase.h \
    confdialogbase.h \
    rangedialogbase.h \
    wmmeasconfigbase.h \
    wmoffsetvalbase.h \

SOURCES	+= \
	devserver.cpp \
	formatinfo.cpp \
	main.cpp \
	ownerror.cpp \
	pcbiface.cpp \
	wm3000i.cpp \
	zerainfo.cpp \
	wm3000scpiface.cpp \
    wmviewbase.cpp \
    wmmeasvaluesbase.cpp \
    wmrawactualvalbase.cpp \
    rangedialogbase.cpp \
    wmmeasconfigbase.cpp \
    wmoffsetvalbase.cpp \
    confdialogbase.cpp


FORMS	= confdialogbase.ui \
	rangedialogbase.ui \
	wmmeasconfigbase.ui \
	wmmeasvaluesbase.ui \
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

INCLUDEPATH += \
    ../wm-common \
    ../wm-common/client-access \
    ../wm-common/dsp \
    ../wm-common/gui \
    ../wm-common/scales-and-units \
    ../wm-common/scpi/definitions \
    ../wm-common/scpi \
    ../wm-common/settings \
    ../wm-common/service-interfaces

LIBS += -L../wm-common -lwm-common

target.path = /usr/bin
INSTALLS += target
