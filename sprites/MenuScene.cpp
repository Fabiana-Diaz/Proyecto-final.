#include "MenuScene.h"
#include <QGraphicsProxyWidget>
#include <QFont>
#include <QDebug>

MenuScene::MenuScene(QObject* parent)
    : QGraphicsScene(parent)
{
    setSceneRect(0, 0, 800, 600);

    // Título
    QGraphicsTextItem* titulo = new QGraphicsTextItem("DRAGON BALL GAME");
    titulo->setFont(QFont("Arial", 28, QFont::Bold));
    titulo->setDefaultTextColor(Qt::yellow);
    titulo->setPos(190, 40);
    addItem(titulo);

    // Botón Nivel 1
    QPushButton* btnNivel1 = new QPushButton("NIVEL 1: GOKU VS BANDAGES");
    btnNivel1->setMinimumSize(300, 50);
    btnNivel1->setFont(QFont("Arial", 16, QFont::Bold));
    QGraphicsProxyWidget* proxy1 = addWidget(btnNivel1);
    proxy1->setPos(250, 180);

    // Botón Nivel 2
    QPushButton* btnNivel2 = new QPushButton("NIVEL 2: GOKU VS AKKUMAN");
    btnNivel2->setMinimumSize(300, 50);
    btnNivel2->setFont(QFont("Arial", 16, QFont::Bold));
    QGraphicsProxyWidget* proxy2 = addWidget(btnNivel2);
    proxy2->setPos(250, 270);

    // Conexiones de los botones
    connect(btnNivel1, &QPushButton::clicked, this, [=]() {
        emit nivelSeleccionado(1);
    });
    connect(btnNivel2, &QPushButton::clicked, this, [=]() {
        emit nivelSeleccionado(2);
    });
}
