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

    // --- Controles y físicas generales ---
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
    void setMuertePorCaida(bool activa) { muertePorCaida = activa; }
    void setVelocidadMovimiento(float v) { velocidadMovimiento = v; }
    void alinearSobrePlataforma(QGraphicsItem* plataforma);

    // --- FÍSICA Y COLISIÓN NIVEL 2 ---
    float getVelocidadY() const;
    void setVelocidadY(float v);

    bool isEnElAire() const;
    void setEnElAire(bool estado);
    bool isSaltoPermitido() const;
    void setSaltoPermitido(bool estado);

    // Métodos exclusivos del nivel 2:
    void aplicarFisicaNivel2();
    void activarCaidaLibreNivel2();
    void resolverColisionNivel2(QGraphicsItem* plataforma);
    void verificarColisionInicialNivel2(const QList<Plataforma*>& plataformas);
    bool getEnPlataforma() const { return enPlataforma; }
    void setEnPlataforma(bool estado) { enPlataforma = estado; }

    bool getEnElAire() const; // por compatibilidad con debug anterior

    // --- NUEVO: bloqueo de física y sprite por victoria ---
    void setVictoria(bool valor) { victoriaMostrada = valor; }
    bool isVictoria() const { return victoriaMostrada; }

    QPainterPath shape() const override;

    int getVida() const { return vida; }
    bool isMuerto() const { return estaMuerto; }
    void setTextoVida(QGraphicsTextItem* txt) { textoVida = txt; }
    void setBarraVida(QGraphicsRectItem* barra) { barraVida = barra; }
    void mostrarVictoria();

//-----estrellas------
    void sumarEstrella();

    int estrellasRecolectadas = 0;

    void activarSuperGolpe();
    void desactivarSuperGolpe();

    bool puedeUsarSuperGolpe() const;

    // --- NIVEL 2: estado de plataforma ---
    bool enPlataforma = false; // (Hazlo público por simplicidad, si prefieres pon un getter/setter)

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
    int vida;
    float velocidadMovimiento = 4.0f;
    bool muertePorCaida = true;
    bool estadoSuperGolpe = false;     // Estado del super golpe

    int estrellasRecogidas = 0;
    bool superGolpeActivo = false;


protected:
    float gravedad = 0.5f; // O el valor que uses en tu juego.
    float velocidadMaxima = 20.0f; // Límite para velocidadY

    // Sprites y animaciones
    QTimer* cambioSpriteTimer;
    QPixmap spriteQuieto;
    QPixmap spritePuno1;
    QPixmap spritePuno2;
    QPixmap spriteEspecial;
    QPixmap spriteCaminando;
    QPixmap spriteSalto;
    QPixmap spriteMuerto;
    QString ultimaDireccion = "quieto";
    QPixmap spritePreparacion;
    QPixmap spriteVictoria;

    QTimer* timerSuperGolpe = nullptr;


    // Lógica de ataque
    bool enAtaque;
    bool ataqueEspecialActivo;
    bool zPresionada = false;

    // UI
    QGraphicsTextItem* textoVida;
    QGraphicsRectItem* barraVida;

    // --- NUEVO: bandera de victoria
    bool victoriaMostrada = false;
};

#endif // JUGADOR_H
