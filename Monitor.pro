QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += sql

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bean/baseconfig.cpp \
    bean/deviceinfo.cpp \
    bean/xmlreader.cpp \
    deviceconfig.cpp \
    file/crcchecksum.cpp \
    file/pathutils.cpp \
    file/runningstate.cpp \
    parsedatathread.cpp \
    widget/devicewidget.cpp \
    widget/flagwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    window/createinstructiondialog.cpp \
    window/lookuplogdialog.cpp

HEADERS += \
    bean/baseconfig.h \
    bean/deviceinfo.h \
    bean/xmlreader.h \
    deviceconfig.h \
    file/crcchecksum.h \
    file/pathutils.h \
    file/runningstate.h \
    parsedatathread.h \
    widget/devicewidget.h \
    widget/flagwidget.h \
    mainwindow.h \
    window/createinstructiondialog.h \
    window/lookuplogdialog.h

FORMS += \
    mainwindow.ui \
    window/createinstructiondialog.ui \
    window/lookuplogdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
