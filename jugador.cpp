#include "jugador.h"
#include <QPixmap>

jugador::jugador(float x, float y)
    : personaje(x, y, 50.0f), velocidadY(0), enElAire(false)
{
    sprite = new QGraphicsPixmapItem(QPixmap(":/sprites/goku_frente.png").scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    sprite->setPos(x, y);
    sprite->setFlag(QGraphicsItem::ItemIsFocusable);
    sprite->setFocus();
}

QGraphicsPixmapItem* jugador::getSprite() {
    return sprite;
}

void jugador::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Left) {
        posX -= 10;
    } else if (event->key() == Qt::Key_Right) {
        posX += 10;
    } else if (event->key() == Qt::Key_Space && !enElAire) {
        velocidadY = -12;
        enElAire = true;
    }

    sprite->setPos(posX, posY);
}

void jugador::aplicarFisica() {
    if (enElAire) {
        velocidadY += 0.7;
        posY += velocidadY;

        if (posY >= 400) {
            posY = 400;
            velocidadY = 0;
            enElAire = false;
        }

        sprite->setPos(posX, posY);
    }
}
