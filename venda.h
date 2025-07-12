#ifndef VENDA_H
#define VENDA_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
#include "jugador.h"
#include "Banglades.h"
#include "venda.h"

class Banglades;  // Declaración adelantada arriba

class Venda : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Venda(jugador* objetivo, float inicioX, float inicioY, Banglades* padreBanglades = nullptr);
    void detenerse();
    ~Venda() override;
    bool yaDetenida = false;


private slots:
    void mover();

private:
    jugador* objetivoGoku;
    QTimer* timerMovimiento;
    float angulo;     // ángulo que se va incrementando
    float radio;      // distancia desde el centro (crece para hacer espiral)
    float velocidadAngular;
    int tiempoActivo;
    QPointF centroInicial;  // punto desde el que se genera la espiral
    Banglades* bangladesPadre = nullptr;


};

#endif // VENDA_H
