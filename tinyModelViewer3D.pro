#-------------------------------------------------
#
# Project created by QtCreator 2016-02-29T14:19:11
#
#-------------------------------------------------

QT       += core gui
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += opengl

TARGET = tinyModelViewer3D
TEMPLATE = app


SOURCES +=\
    BoundingBox.cpp \
    Quads.cpp \
    ShapeGroup.cpp \
    Triangles.cpp \
    LoadObjectStatusDialog.cpp \
    SceneStat.cpp \
    Camera.cpp \
    MainWindow.cpp \
    Material.cpp \
    RenderWidget.cpp \
    Scene.cpp \
    StringUtilities.cpp \
    ObjReader.cpp \
    Texture.cpp \
    Main.cpp

HEADERS  += \
    BoundingBox.h \
    Quads.h \
    ShapeGroup.h \
    Triangles.h \
    LoadObjectStatusDialog.h \
    SceneStat.h \
    Camera.h \
    MainWindow.h \
    Material.h \
    RenderWidget.h \
    Scene.h \
    StringUtilities.h \
    ObjReader.h \
    Texture.h

FORMS    += \
    LoadObjectStatusDialog.ui \
    MainWindow.ui

RESOURCES += \
    res.qrc

unix {
    LIBS += -lGLU
    LIBS += -lGL
}

win32 {
    LIBS += -lopengl32
    LIBS += -lglu32
}
