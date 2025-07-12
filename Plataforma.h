#ifndef PLATAFORMA_H
#define PLATAFORMA_H

#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>

class Plataforma : public QGraphicsRectItem {
public:
    Plataforma(qreal x, qreal y, qreal width, qreal height, bool visible);
};

#endif // PLATAFORMA_H
