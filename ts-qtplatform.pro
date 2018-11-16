#-------------------------------------------------
#
# Project created by QtCreator 2016-11-21T17:28:30
#
#-------------------------------------------------

QT       += gui x11extras gui-private multimedia concurrent svg
CONFIG   += c++14 plugin

TARGET = ts
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

TRANSLATIONS += translations/vi_VN.ts \
    translations/da_DK.ts \
    translations/es_ES.ts \
    translations/lt_LT.ts \
    translations/nl_NL.ts \
    translations/pl_PL.ts \
    translations/pt_BR.ts \
    translations/ru_RU.ts \
    translations/sv_SE.ts \
    translations/en_AU.ts \
    translations/en_US.ts \
    translations/en_GB.ts \
    translations/en_NZ.ts \
    translations/de_DE.ts \
    translations/id_ID.ts \
    translations/au_AU.ts \
    translations/it_IT.ts \
    translations/nb_NO.ts \
    translations/no_NO.ts \
    translations/ro_RO.ts \
    translations/cy_GB.ts \
    translations/fr_FR.ts \
    translations/ur_PK.ts

qtPrepareTool(LUPDATE, lupdate)
genlang.commands = "$$LUPDATE -no-obsolete -source-language en_US $$_PRO_FILE_"

qtPrepareTool(LRELEASE, lrelease)
rellang.commands = "$$LRELEASE -removeidentical $$_PRO_FILE_"
QMAKE_EXTRA_TARGETS = genlang rellang
PRE_TARGETDEPS = genlang rellang

# Turn off stripping as this causes the install to fail :(
QMAKE_STRIP = echo

unix {
    target.path = $$[QT_INSTALL_PLUGINS]/platformthemes/

    translations.path = /usr/share/theshell/ts-qtplatform/translations
    translations.files = translations/*

    INSTALLS += target
}

DISTFILES += \
    plugin.json

FORMS += \
    messagedialog.ui \
    filedialog.ui

RESOURCES += \
    resources.qrc
