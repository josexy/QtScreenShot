QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG    += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(QHotkey/qhotkey.pri)
INCLUDEPATH += $$PWD/QHotkey/QHotkey

LIBS += -L$$PWD/QHotkey/QHotkey -lQHotkey

SOURCES += core/core.cpp\
    core/core_fs_check.cpp \
    core/core_settings.cpp\
    core/core_system.cpp\
    drawer/drawer.cpp \
    drawer/arrowdrawer.cpp \
    drawer/erasedrawer.cpp \
    drawer/imageblurdrawer.cpp \
    drawer/pendrawer.cpp \
    drawer/rectangleblurdrawer.cpp \
    drawer/shapedrawer.cpp \
    drawer/textdrawer.cpp \
    tmain.cpp \
    tool/arrowtool.cpp \
    tool/basetool.cpp \
    tool/erasetool.cpp \
    widget/basebutton.cpp\
    widget/basecombobox.cpp\
    widget/colorpanel.cpp\
    widget/colorpicker.cpp\
    widget/customtextedit.cpp\
    widget/toolwidget.cpp\
    widget/inputtextedit.cpp\
    widget/pixelpanel.cpp\
    widget/pixelwidget.cpp\
    widget/separator.cpp\
    tool/shapetool.cpp\
    tool/texttool.cpp\
    tool/pentool.cpp\
    helper/textlogger.cpp\
    main.cpp\
    capturewindow.cpp\
    settingswindow.cpp\
    JSON/src/JsonDocument.cpp\
    JSON/src/JsonObject.cpp\
    JSON/src/JsonArray.cpp\
    JSON/src/JsonConvert.cpp\
    JSON/src/JsonParser.cpp

HEADERS += core/core.h\
    core/core_fs_check.h \
    core/core_settings.h\
    core/core_system.h\
    drawer/drawer.h \
    drawer/arrowdrawer.h \
    drawer/erasedrawer.h \
    drawer/imageblurdrawer.h \
    drawer/pendrawer.h \
    drawer/rectangleblurdrawer.h \
    drawer/shapedrawer.h \
    drawer/textdrawer.h \
    tmain.h \
    tool/arrowtool.h \
    tool/basetool.h \
    tool/erasetool.h \
    widget//basebutton.h\
    widget/basecombobox.h\
    widget/colorpanel.h\
    widget/colorpicker.h\
    widget/customtextedit.h\
    widget/inputtextedit.h\
    widget/toolwidget.h\
    widget/pixelpanel.h\
    widget/pixelwidget.h\
    widget/separator.h\
    tool/pentool.h\
    tool/shapetool.h\
    tool/texttool.h\
    helper/textlogger.h\
    capturewindow.h\
    settingswindow.h\
    JSON/include/JsonDocument.h\
    JSON/include/JsonObject.h\
    JSON/include/JsonArray.h\
    JSON/include/JsonConvert.h\
    JSON/include/JsonParser.h

FORMS +=\
    ui/settingswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

TRANSLATIONS = languages/UI_Translation_zh_CN.ts
