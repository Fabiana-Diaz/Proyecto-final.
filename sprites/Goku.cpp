#include "goku.h"
#include <QKeyEvent>
#include <QPixmap>


goku::goku(float x, float y)
    : jugador(x, y), velocidadY(0), enElAire(false)
{
    setPixmap(QPixmap(":/sprites/goku_frente.png").scaled(50, 50));
    setPos(x, y);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}

void goku::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Left) {
        posX -= 10;
    } else if (event->key() == Qt::Key_Right) {
        posX += 10;
    } else if (event->key() == Qt::Key_Space && !enElAire) {
        velocidadY = -12;
        enElAire = true;
    }

    setPos(posX, posY);
}

void goku::aplicarFisica() {
    if (enElAire) {
        velocidadY += 0.7;
        posY += velocidadY;

        if (posY >= 400) {
            posY = 400;
            velocidadY = 0;
            enElAire = false;
        }

        setPos(posX, posY);
    }
}
