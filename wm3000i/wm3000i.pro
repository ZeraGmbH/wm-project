TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt

INCLUDEPATH	+= .


HEADERS	+= \
	devserver.h \
	ownerror.h \
	pcbiface.h \
	wmglobal.h \
	wm3000i.h \
	zerainfo.h \
	wm3000scpiface.h \
    wmviewbase.h \
    wmrawactualvalbase.h \
    confdialogbase.h \
    rangedialogbase.h \
    wmoffsetvalbase.h \

SOURCES	+= \
	devserver.cpp \
	main.cpp \
	ownerror.cpp \
	pcbiface.cpp \
	wm3000i.cpp \
	zerainfo.cpp \
	wm3000scpiface.cpp \
    wmviewbase.cpp \
    wmrawactualvalbase.cpp \
    rangedialogbase.cpp \
    wmoffsetvalbase.cpp \
    confdialogbase.cpp


FORMS	= confdialogbase.ui \
	rangedialogbase.ui \
	wmrawactualvalbase.ui \
        wmoffsetvalbase.ui \
	wmviewbase.ui


######################################################################
# Automatically generated by qmake (1.07a) Thu May 12 09:23:36 2005
######################################################################


TRANSLATIONS    = wm3000i_gb.ts \
                  wm3000i_pl.ts

QT += widgets network xml qt3support

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
