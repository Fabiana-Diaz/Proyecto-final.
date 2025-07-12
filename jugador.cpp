#include "jugador.h"
#include <QPixmap>
#include <QDebug>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QPainterPath>
#include "Plataforma.h"
#include "Banglades.h"

// Constructor
jugador::jugador(float x, float y)
    : QObject(), QGraphicsPixmapItem(),
    personaje(x, y),
    velocidadY(0), enElAire(false), teclaPresionada(false),
    animacionEnProgreso(false), estaMuerto(false), saltoPermitido(true),
    enPlataforma(false), velocidadMovimiento(4.0f), vida(200), victoriaMostrada(false)
{
    // Sprites de Goku (quieto, preparación, movimiento, salto, muerto)
    spriteQuieto      = QPixmap(":/sprites/goku_frente.png");         // sprite de pie (quieto)
    spritePreparacion = QPixmap(":/sprites/goku_izq2.png");
    spriteCaminando   = QPixmap(":/sprites/goku_izquierda.png");
    spriteSalto       = QPixmap(":/sprites/goku_salto.png");
    spriteMuerto      = QPixmap(":/sprites/goku_muerto.png");
    spriteVictoria = QPixmap(":/sprites/goku_victoria.png");
    if (spriteVictoria.isNull()) {
        qDebug() << "❌ No se pudo cargar spriteVictoria";
    } else {
        qDebug() << "✅ spriteVictoria cargado correctamente";
    }

