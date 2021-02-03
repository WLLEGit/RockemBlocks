QT       += core gui multimedia widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 resources_big

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
    dialog.cpp \
    gamewidget.cpp \
    gem.cpp \
    hoverbutton.cpp \
    main.cpp \
    menubutton.cpp \
    settingwidget.cpp \
    startwidget.cpp

HEADERS += \
    dialog.h \
    gamewidget.h \
    gem.h \
    hoverbutton.h \
    menubutton.h \
    settingwidget.h \
    startwidget.h

FORMS += \
    dialog.ui \
    gamewidget.ui \
    settingwidget.ui \
    startwidget.ui

TRANSLATIONS += \
    RockemBlocks_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    music.qrc \
    pic.qrc \
    sound.qrc

RC_FILE += icon.rc
