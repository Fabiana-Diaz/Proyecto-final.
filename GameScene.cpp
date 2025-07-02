#include "GameScene.h"
#include "jugador.h"
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QDebug>
#include <QTimer>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent)
{
    setSceneRect(0, 0, 800, 600);

    // Fondo del juego
    QPixmap fondoPixmap(":/sprites/fondo.jpg");
    if (fondoPixmap.isNull()) {
        qDebug() << "❌ No se pudo cargar el fondo.";
    } else {
        QGraphicsPixmapItem *fondoItem = new QGraphicsPixmapItem(
            fondoPixmap.scaled(sceneRect().size().toSize(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)
            );
        fondoItem->setZValue(-10);
        addItem(fondoItem);
    }

    // Crear al jugador (Goku)
    jugadorGoku = new jugador(100, 100);
    jugadorGoku->getSprite()->setZValue(1);
    addItem(jugadorGoku->getSprite());  // ⬅️ Agrega el sprite, no el objeto jugador directamente

    // Timer para aplicar física
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=]() {
        jugadorGoku->aplicarFisica();
    });
    timer->start(16);
}
