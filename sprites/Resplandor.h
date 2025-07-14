// Resplandor.h
#ifndef RESPLANDOR_H
#define RESPLANDOR_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class jugador; // Declaración adelantada
enum class DireccionResplandor { Horizontal, Vertical };

class Resplandor : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Resplandor(float startX, float startY, jugador* objetivo, DireccionResplandor dir);
    ~Resplandor();

private slots:
    void mover();

private:
    jugador* objetivoGoku;
    QTimer* timerMovimiento;
    float velocidadX;
    float velocidadY;
    DireccionResplandor direccion;
};

#endif // RESPLANDOR_H
