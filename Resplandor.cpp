#include "Resplandor.h"
#include "jugador.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QTimer>

Resplandor::Resplandor(float startX, float startY, jugador* objetivo, DireccionResplandor dir)
    : QObject(), QGraphicsPixmapItem(), objetivoGoku(objetivo), direccion(dir), velocidadX(8.0f), velocidadY(6.0f)
{
    setPixmap(QPixmap(":/sprites/resplandor.png").scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setPos(startX, startY);

    timerMovimiento = new QTimer(this);
    connect(timerMovimiento, &QTimer::timeout, this, &Resplandor::mover);
    timerMovimiento->start(30);
}

Resplandor::~Resplandor() {
    timerMovimiento->stop();
}

void Resplandor::mover() {
    if (direccion == DireccionResplandor::Horizontal) {
        setX(x() - velocidadX);  // mueve hacia la izquierda
        if (x() < 0) {
            scene()->removeItem(this);
            deleteLater();
            return;
        }
    } else if (direccion == DireccionResplandor::Vertical) {
        setY(y() + velocidadY);  // mueve hacia abajo
        if (scene() && y() > scene()->height()) {
            scene()->removeItem(this);
            deleteLater();
            return;
        }
    }

    // Colisión con Goku
    if (objetivoGoku && collidesWithItem(objetivoGoku)) {
        objetivoGoku->recibirDanio(25);
        qDebug() << "Resplandor golpeó a Goku, vida restante:" << objetivoGoku->getVida();
        scene()->removeItem(this);
        deleteLater();
    }
}
