#ifndef BANGLADES_H
#define BANGLADES_H

#include "personaje.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QObject>
#include <QTimer>
#include "Plataforma.h"
#include "jugador.h"

class Venda;


class Banglades : public QObject, public personaje, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Banglades(float x, float y, Plataforma* plataformaAsociada, jugador* objetivoGoku);


    void aplicarFisica() override;
    void moverAutomaticamente();
    void dispararVendaje();
    void detenerMovimiento();

    // --- VIDA y estado
    void recibirGolpe(int cantidad);
    int getVida() const { return vida; }
    bool isMuerto() const { return estaMuerto; }
    void setTextoVida(QGraphicsTextItem* txt) { textoVida = txt; }
    void setBarraVida(QGraphicsRectItem* barra) { barraVida = barra; }

    void detenerVendas();
    Banglades* bangladesPadre = nullptr; // <<-- así

    void eliminarVenda(Venda* v);


signals:
    void bandagesDerrotado();


private slots:
    void actualizarMovimiento();

private:
    QTimer* timerMovimiento;
    float direccion; // 1 = derecha, -1 = izquierda

    int vida = 100;
    bool estaMuerto = false;
    QGraphicsTextItem* textoVida = nullptr;
    QGraphicsRectItem* barraVida = nullptr;
    QPixmap spriteMuerto;

    QTimer* timerVendas = nullptr;          // Timer para disparar vendas
    QList<Venda*> listaVendas;

    Plataforma* plataforma = nullptr;
    jugador* jugadorGoku = nullptr;




};

#endif // BANGLADES_H
