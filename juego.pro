QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Banglades.cpp \
    Estrella.cpp \
    GameScene.cpp \
    GameScene2.cpp \
    Plataforma.cpp \
    Resplandor.cpp \
    akkuman.cpp \
    jugador.cpp \
    main.cpp \
    mainwindow.cpp \
    personaje.cpp \
    venda.cpp

HEADERS += \
    Banglades.h \
    Estrella.h \
    GameScene.h \
    GameScene2.h \
    Plataforma.h \
    Resplandor.h \
    akkuman.h \
    jugador.h \
    mainwindow.h \
    personaje.h \
    venda.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    sprites/momia_izquierda.png
