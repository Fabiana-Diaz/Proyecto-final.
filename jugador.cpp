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

void jugador::keyPressEvent(QKeyEvent* event) {
    if (estaMuerto || victoriaMostrada) return;

    // ATAQUE NORMAL: Z (solo una vez por pulsación)
    if (event->key() == Qt::Key_Z && !enAtaque && !zPresionada) {
        zPresionada = true;
        enAtaque = true;
        setPixmap(spritePuno1.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        QTimer::singleShot(80, this, [this]() {
            setPixmap(spritePuno2.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            for (QGraphicsItem *item : collidingItems()) {
                Banglades* enemigo = dynamic_cast<Banglades*>(item);
                if (enemigo) {
                    enemigo->recibirGolpe(10);
                }
            }
            QTimer::singleShot(100, this, [this]() {
                setPixmap(spriteQuieto.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                enAtaque = false;
            });
        });
        return;
    }
// MOVIMIENTO IZQUIERDA/DERECHA
    if ((event->key() == Qt::Key_Left || event->key() == Qt::Key_Right) && !teclaPresionada) {
        setPixmap(spritePreparacion.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        animacionEnProgreso = true;
        cambioSpriteTimer->start(150);
        teclaPresionada = true;

        if (event->key() == Qt::Key_Left) {
            moverIzquierda();
        } else {
            moverDerecha();
        }
        return;
    }

    // SALTO
    if (event->key() == Qt::Key_Space && !enElAire && saltoPermitido) {
        saltar();
        saltoPermitido = false;
        setPixmap(spriteSalto.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        qDebug() << "⬆ Goku saltó con dirección:" << ultimaDireccion;
        return;
    }
}


