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

    // Sprites de ataque
    spritePuno1    = QPixmap(":/sprites/goku_puno1.png");           // Brazo listo para golpear
    spritePuno2    = QPixmap(":/sprites/goku_puno2.png");           // Puño extendido
    spriteEspecial = QPixmap(":/sprites/goku_ataque.png");          // Ataque especial

    setPixmap(spriteQuieto.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    setPos(x, y);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

    cambioSpriteTimer = new QTimer(this);
    cambioSpriteTimer->setSingleShot(true);
    connect(cambioSpriteTimer, &QTimer::timeout, this, [=]() {
        if (!estaMuerto && !victoriaMostrada && spriteCaminando.cacheKey() != pixmap().cacheKey()) {
            setPixmap(spriteCaminando.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
        animacionEnProgreso = false;
    });
}
