QT += gui

CONFIG += qt

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QT += widgets

HEADERS += \
        Edge.h \
        Graphwidget.h \
        Mainwindow.h \
        Node.h \
        Scene.h

SOURCES += \
        Edge.cpp \
        Graphwidget.cpp \
        Mainwindow.cpp \
        Node.cpp \
        Scene.cpp \
        main.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/graphicsview/elasticnodes
INSTALLS += target
