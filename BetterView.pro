QT += core widgets gui svg concurrent

CONFIG += c++11

TARGET = BetterView
TEMPLATE = app

VERSION = 0.0.1
RC_ICONS = icons/icon.ico

SOURCES += main.cpp \
           mainwindow.cpp \
           imageviewer.cpp \
           imageloader.cpp \
           gifviewer.cpp \
           filemanager.cpp \
           settingsmanager.cpp \
           fileplaylist.cpp \
           zoomcontrol.cpp

HEADERS += mainwindow.h \
           imageviewer.h \
           imageloader.h \
           gifviewer.h \
           filemanager.h \
           settingsmanager.h \
           fileplaylist.h \
           zoomcontrol.h \
           version.h

RESOURCES += resources.qrc