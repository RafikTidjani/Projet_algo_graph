QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    composents/coutinput.cpp \
    composents/fsapsInput.cpp \
    composents/matadjinput.cpp \
    composents/ordonnancement.cpp \
    composents/outputalgo.cpp \
    composents/vertexinput.cpp \
    composents/graphview.cpp \
    graph/algorithmes.cpp \
    graph/graph.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    composents/coutinput.h \
    composents/fsapsInput.h \
    composents/matadjinput.h \
    composents/ordonnancement.h \
    composents/outputalgo.h \
    composents/vertexinput.h \
    composents/graphview.h \
    graph/algorithmes.h \
    graph/graph.h \
    mainwindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

resources.files = \
    assets/Logo_Graphe.png
resources.prefix = /

RESOURCES = resources
