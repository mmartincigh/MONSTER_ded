TARGET = MONSTER_ded

TEMPLATE = app

VERSION = 1.1.0

QT += \
    qml \
    quick \
    widgets

CONFIG += \
    c++11
CONFIG(debug, debug|release) {
    CONFIG += \
        console
}

DEFINES += \
    SW_VERSION=\\\"$$VERSION\\\"
CONFIG(release, debug|release) {
    DEFINES += \
        QT_NO_DEBUG_OUTPUT
}

CONFIG(debug, debug|release) {
    DESTDIR = $${OUT_PWD}/debug
}
CONFIG(release, debug|release) {
    DESTDIR = $${OUT_PWD}/release
}
MOC_DIR = $${DESTDIR}/.moc
OBJECTS_DIR = $${DESTDIR}/.obj
RCC_DIR = $${DESTDIR}/.rcc

INCLUDEPATH += \
    "$$PWD/include"

win32 {
    LIBS += \
        -L"$$PWD\lib\win"
    CONFIG(debug, debug|release) {
        LIBS += \
            -lcryptlibd
    }
    CONFIG(release, debug|release) {
        LIBS += \
            -lcryptlib
    }
}

win32 {
    QMAKE_CXXFLAGS += \
        /wd4100 \
        /wd4189
}

win32 {
    RC_ICONS = 1UpMushroom256x256.ico
}

HEADERS += \
    applicationmanager.h \
    applicationutils.h \
    base.h \
    destinationmanager.h \
    encryptionmanager.h \
    encryptionmanagerimpl.h \
    fileencryptor.h \
    enums.h \
    filemanager.h \
    logmanager.h \
    settingsmanager.h \
    sourcemanager.h \
    utils.h \
    windowmanager.h

SOURCES += \
    applicationmanager.cpp \
    applicationutils.cpp \
    base.cpp \
    destinationmanager.cpp \
    encryptionmanager.cpp \
    encryptionmanagerimpl.cpp \
    fileencryptor.cpp \
    filemanager.cpp \
    logmanager.cpp \
    main.cpp \
    settingsmanager.cpp \
    sourcemanager.cpp \
    utils.cpp \
    windowmanager.cpp

RESOURCES += \
    resources.qrc

win32 {
    OTHER_FILES += \
        $(QTDIR)/bin/D3DCompiler_43.dll \
        $(QTDIR)/bin/icudt54.dll \
        $(QTDIR)/bin/icuin54.dll \
        $(QTDIR)/bin/icuuc54.dll
    CONFIG(debug, debug|release) {
        OTHER_FILES += \
            $(QTDIR)/bin/libEGLd.dll \
            $(QTDIR)/bin/libGLESv2d.dll \
            $(QTDIR)/bin/Qt5Cored.dll \
            $(QTDIR)/bin/Qt5Guid.dll \
            $(QTDIR)/bin/Qt5Networkd.dll \
            $(QTDIR)/bin/Qt5Qmld.dll \
            $(QTDIR)/bin/Qt5Quickd.dll \
            $(QTDIR)/bin/Qt5Widgetsd.dll
    }
    CONFIG(release, debug|release) {
        OTHER_FILES += \
            $(QTDIR)/bin/libEGL.dll \
            $(QTDIR)/bin/libGLESv2.dll \
            $(QTDIR)/bin/Qt5Core.dll \
            $(QTDIR)/bin/Qt5Gui.dll \
            $(QTDIR)/bin/Qt5Network.dll \
            $(QTDIR)/bin/Qt5Qml.dll \
            $(QTDIR)/bin/Qt5Quick.dll \
            $(QTDIR)/bin/Qt5Widgets.dll
    }
}

win32 {
    for(FILE, OTHER_FILES){
        QMAKE_POST_LINK += \
            $$quote(copy /Y \"$$shell_path($${FILE})\" \"$$shell_path($${DESTDIR})\"$$escape_expand(\n\t))
    }
}
