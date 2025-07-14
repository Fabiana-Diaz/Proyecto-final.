#include "Estrella.h"
#include "jugador.h"
#include <QDebug>
#include <QGraphicsScene>

Estrella::Estrella(float x, float y, jugador* objetivo)
    : QObject(), QGraphicsPixmapItem(), objetivoGoku(objetivo) {
    setPixmap(QPixmap(":/sprites/estrella.png").scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setPos(x, y);
    timerCaida = new QTimer(this);
    connect(timerCaida, &QTimer::timeout, this, &Estrella::caer);
    timerCaida->start(30);
}

void Estrella::caer() {
    setY(y() + velocidadCaida);

    if (y() > scene()->height()) {
        timerCaida->stop();
        scene()->removeItem(this);
        deleteLater();
        return;
    }

    if (objetivoGoku && collidesWithItem(objetivoGoku)) {
        timerCaida->stop();
        scene()->removeItem(this);
        deleteLater();
        objetivoGoku->sumarEstrella();  // Método que implementaremos en jugador
        qDebug() << "Estrella recogida!";
    }
}
