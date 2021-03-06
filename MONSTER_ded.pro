TARGET = MONSTER_ded

TEMPLATE = app

VERSION = 2.1.0

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
    CONFIG(debug, debug|release) {
        contains(QMAKE_TARGET.arch, x86_64) {
            # Windows x64 (64bit) debug
            LIBS += \
                -L"$$PWD/lib/win/x64/debug"
        } else {
            # Windows x86 (32bit) debug
            LIBS += \
                -L"$$PWD/lib/win/x86/debug"
        }
    }
    CONFIG(release, debug|release) {
        contains(QMAKE_TARGET.arch, x86_64) {
            # Windows x64 (64bit) release
            LIBS += \
                -L"$$PWD/lib/win/x64/release"
        } else {
            # Windows x86 (32bit) release
            LIBS += \
                -L"$$PWD/lib/win/x86/release"
        }
    }
    LIBS += \
        -lcryptlib
}
unix {
    contains(QMAKE_HOST.arch, x86_64) {
        # Linux x64 (64bit)
        LIBS += \
            -L$$PWD/lib/linux/x64
    } else {
        # Linux x86 (32bit)
        LIBS += \
            -L$$PWD/lib/linux/x86
    }
    LIBS += \
        -lcryptopp
}

win32 {
    QMAKE_CXXFLAGS += \
        /wd4100 \
        /wd4189 \
        /wd4351
}

win32 {
    RC_ICONS = 1UpMushroom256x256.ico
}

HEADERS += \
    applicationmanager.h \
    applicationutils.h \
    base.h \
    decryptionmanager.h \
    decryptionmanagerimpl.h \
    destinationpathmanager.h \
    encryptionmanager.h \
    encryptionmanagerimpl.h \
    enums.h \
    filemanagerbase.h \
    ifilemanager.h \
    ipathmanager.h \
    iprocess.h \
    iprocessimpl.h \
    logmanager.h \
    pathmanagerbase.h \
    processimplbase.h \
    processmanager.h \
    securefilemanager.h \
    securepathmanager.h \
    settingsmanager.h \
    singleapplication.h \
    sourcefilemanager.h \
    sourcepathmanager.h \
    statsmanager.h \
    systemtrayiconmanager.h \
    utils.h \
    windowmanager.h

SOURCES += \
    applicationmanager.cpp \
    applicationutils.cpp \
    base.cpp \
    decryptionmanager.cpp \
    decryptionmanagerimpl.cpp \
    destinationpathmanager.cpp \
    encryptionmanager.cpp \
    encryptionmanagerimpl.cpp \
    filemanagerbase.cpp \
    logmanager.cpp \
    main.cpp \
    pathmanagerbase.cpp \
    processimplbase.cpp \
    processmanager.cpp \
    securefilemanager.cpp \
    securepathmanager.cpp \
    settingsmanager.cpp \
    singleapplication.cpp \
    sourcefilemanager.cpp \
    sourcepathmanager.cpp \
    statsmanager.cpp \
    systemtrayiconmanager.cpp \
    utils.cpp \
    windowmanager.cpp

RESOURCES += \
    resources.qrc

win32 {
    OTHER_FILES += \
        $(QTDIR)/bin/d3dcompiler_47.dll \
        $$PWD/AES_key.bin \
        $$PWD/AES_iv.bin
    lessThan(QT_MINOR_VERSION, 7) {
        OTHER_FILES += \
            $(QTDIR)/bin/icudt54.dll \
            $(QTDIR)/bin/icuin54.dll \
            $(QTDIR)/bin/icuuc54.dll
    }
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
