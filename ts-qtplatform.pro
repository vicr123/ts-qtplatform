#-------------------------------------------------
#
# Project created by QtCreator 2016-11-21T17:28:30
#
#-------------------------------------------------

QT       += gui x11extras gui-private multimedia concurrent svg
CONFIG   += c++14

TARGET = ts-qtplatform
TEMPLATE = lib

DEFINES += TSQTPLATFORM_LIBRARY

SOURCES += platformtheme.cpp \
    plugin.cpp \
    messagedialoghelper.cpp \
    messagedialog.cpp \
    iconengine.cpp \
    platformmenu.cpp \
    filedialoghelper.cpp \
    filedialog.cpp

HEADERS += platformtheme.h\
        ts-qtplatform_global.h \
    plugin.h \
    messagedialoghelper.h \
    messagedialog.h \
    iconengine.h \
    platformmenu.h \
    filedialoghelper.h \
    filedialog.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    plugin.json

FORMS += \
    messagedialog.ui \
    filedialog.ui

RESOURCES += \
    resources.qrc
