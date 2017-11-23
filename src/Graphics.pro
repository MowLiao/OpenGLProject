

TEMPLATE = app
TARGET = Graphics
INCLUDEPATH += . /opt/local/include
QT += widgets opengl gui 
LIBS +=  -lglut -lGLU


# Input
#CONFIG += debug
HEADERS += GraphicsWidget.h GraphicsWindow.h 
SOURCES += Main.cpp \
           GraphicsWidget.cpp \
           GraphicsWindow.cpp
