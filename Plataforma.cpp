#include "plataforma.h"
#include <QBrush>
#include <QPen>

Plataforma::Plataforma(qreal x, qreal y, qreal ancho, qreal alto, bool visible)
{
    setRect(0, 0, ancho, alto);
    setPos(x, y);

    if (visible) {
        setBrush(QBrush(Qt::blue));      // Color visible (debug)
        setPen(QPen(Qt::black));         // Contorno visible
    } else {
        setBrush(Qt::NoBrush);           // Invisible
        setPen(Qt::NoPen);
    }

    setZValue(0); // Debajo del jugador
}
