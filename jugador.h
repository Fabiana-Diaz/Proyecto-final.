#ifndef JUGADOR_H
#define JUGADOR_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QTimer>
#include <QList>
#include "personaje.h"

class Plataforma;  // Declaración adelantada

class jugador : public QObject, public QGraphicsPixmapItem, public personaje {

    Q_OBJECT
    friend class GameScene;  // Permite acceso a enPlataforma

public:
    jugador(float x, float y);
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void aplicarFisica();
    void resolverColision(QGraphicsItem* plataforma);
    void activarCaidaLibre();
    void verificarColisionInicial(const QList<Plataforma*>& plataformas);
    void saltar();
    void moverDerecha();
    void moverIzquierda();
    void recibirDanio(int cantidad);

    // --- NUEVO: bloqueo de física y sprite por victoria ---
    void setVictoria(bool valor) { victoriaMostrada = valor; }
    bool isVictoria() const { return victoriaMostrada; }

    QPainterPath shape() const override;
    bool isEnElAire() const { return enElAire; }

    int getVida() const { return vida; }
    bool isMuerto() const { return estaMuerto; }
    void setTextoVida(QGraphicsTextItem* txt) { textoVida = txt; }
    void setBarraVida(QGraphicsRectItem* barra) { barraVida = barra; }

     void mostrarVictoria();
signals:
    void jugadorMuerto();

private:
    // Físicas y estado
    float velocidadY;
    bool enElAire;
    bool teclaPresionada;
    bool estaMuerto;
    bool saltoPermitido;
    bool animacionEnProgreso;
    bool enPlataforma;
    int vida;
    float velocidadMovimiento = 4.0f;
