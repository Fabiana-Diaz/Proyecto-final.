#include "GameScene.h"
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QDebug>
#include <QTimer>
#include <QPen>
#include "banglades.h"
#include "Venda.h"

// NUEVO: Para el cartel de victoria
#include <QGraphicsScene>
#include <QGraphicsRectItem>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent)
{
    setSceneRect(0, 0, 800, 620);

    // Fondo
    QPixmap fondoPixmap(":/sprites/fondo.jpg");
    if (!fondoPixmap.isNull()) {
        QGraphicsPixmapItem *fondoItem = new QGraphicsPixmapItem(
            fondoPixmap.scaled(sceneRect().size().toSize(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        fondoItem->setZValue(-1);
        addItem(fondoItem);
    }

    // Plataforma visible
    QPixmap pixmapPlataforma(":/sprites/Plataforma2.png");
    if (!pixmapPlataforma.isNull()) {
        QGraphicsPixmapItem* imagenPlataforma = new QGraphicsPixmapItem(
            pixmapPlataforma.scaledToWidth(500, Qt::SmoothTransformation));
        imagenPlataforma->setPos(150, 86);
        imagenPlataforma->setZValue(3);
        addItem(imagenPlataforma);
    }

    // Plataforma invisible para colisiones físicas
    Plataforma* plataformaColision = new Plataforma(150, 310, 500, 30, false);
    plataformaColision->setZValue(1);
    addItem(plataformaColision);
    plataformas.append(plataformaColision);

    // Jugador
    jugadorGoku = new jugador(175, 225);
    jugadorGoku->setZValue(4);
    addItem(jugadorGoku);
    // --- IMPORTANTE ---
    jugadorGoku->setFlag(QGraphicsItem::ItemIsFocusable);
    jugadorGoku->setFocus();
    connect(jugadorGoku, &jugador::jugadorMuerto, this, &GameScene::detenerEnemigosYVendas);

    // Vida de Goku (texto y barra)
    textoVidaGoku = new QGraphicsTextItem();
    textoVidaGoku->setPlainText("Goku: 100");
    textoVidaGoku->setDefaultTextColor(QColor(255, 140, 0));
    textoVidaGoku->setFont(QFont("Arial", 14, QFont::Bold));
    textoVidaGoku->setPos(10, 20);
    addItem(textoVidaGoku);

    barraVidaGoku = new QGraphicsRectItem(0, 0, 100, 10);
    barraVidaGoku->setBrush(QColor(255, 140, 0));
    barraVidaGoku->setPen(QPen(Qt::black));
    barraVidaGoku->setZValue(10);
    barraVidaGoku->setPos(10, 10);
    addItem(barraVidaGoku);

    jugadorGoku->setTextoVida(textoVidaGoku);
    jugadorGoku->setBarraVida(barraVidaGoku);

    // Banglades (con plataforma y Goku)
    enemigoBanglades = new Banglades(550, 240, plataformaColision, jugadorGoku);
    enemigoBanglades->setZValue(4);
    addItem(enemigoBanglades);

    // Conexión para mostrar victoria al morir Banglades
    connect(enemigoBanglades, &Banglades::bandagesDerrotado, this, &GameScene::mostrarVictoria);

    textoVidaBanglades = new QGraphicsTextItem();
    textoVidaBanglades->setPlainText("Banglades: 100");
    textoVidaBanglades->setDefaultTextColor(Qt::white);
    textoVidaBanglades->setFont(QFont("Arial", 14, QFont::Bold));
    textoVidaBanglades->setPos(600, 20);
    addItem(textoVidaBanglades);

    barraVidaBanglades = new QGraphicsRectItem(0, 0, 100, 10);
    barraVidaBanglades->setBrush(Qt::white);
    barraVidaBanglades->setPen(QPen(Qt::black));
    barraVidaBanglades->setZValue(10);
    barraVidaBanglades->setPos(650, 10);
    addItem(barraVidaBanglades);

    enemigoBanglades->setBarraVida(barraVidaBanglades);
    enemigoBanglades->setTextoVida(textoVidaBanglades);

    // Corazón para Goku
    QGraphicsPixmapItem* corazonGoku = new QGraphicsPixmapItem(QPixmap(":/sprites/corazon.png").scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    corazonGoku->setPos(0, 8);
    corazonGoku->setZValue(12);
    addItem(corazonGoku);

    // Corazón para Banglades
    QGraphicsPixmapItem* corazonBanglades = new QGraphicsPixmapItem(QPixmap(":/sprites/corazon.png").scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    corazonBanglades->setPos(630, 8);
    corazonBanglades->setZValue(12);
    addItem(corazonBanglades);

    // Verificación inicial
    jugadorGoku->verificarColisionInicial(plataformas);

    // Temporizador de física
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        bool aterrizo = false;
        for (Plataforma* plat : plataformas) {
            if (jugadorGoku->collidesWithItem(plat)) {
                jugadorGoku->resolverColision(plat);
                if (!jugadorGoku->isEnElAire()) {
                    aterrizo = true;
                    break;
                }
            }
        }
        if (!aterrizo) {
            jugadorGoku->activarCaidaLibre();
        }
        jugadorGoku->aplicarFisica();
    });
    timer->start(16);
}

void GameScene::detenerEnemigosYVendas() {
    if (enemigoBanglades)
        enemigoBanglades->detenerMovimiento();
    // Nada más, no se elimina vendas aquí.
}

// --- NUEVO: Slot para mostrar la animación de victoria ---
void GameScene::mostrarVictoria() {
    // 1. Cambia sprite de Goku al de victoria (asegúrate de tener spriteVictoria)
    jugadorGoku->mostrarVictoria();

    // 2. Crea y muestra la imagen "You Win" en el centro (ajusta tamaño si quieres)
    QPixmap cartelWin(":/sprites/you_win.png"); // Cambia la ruta por la tuya
    QGraphicsPixmapItem* cartelWinItem = new QGraphicsPixmapItem(cartelWin.scaled(320, 240, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    cartelWinItem->setZValue(30);
    cartelWinItem->setPos(width()/2 - cartelWinItem->boundingRect().width()/2, height()/2 - cartelWinItem->boundingRect().height()/2);
    addItem(cartelWinItem);

    // Si quieres que desaparezca después de unos segundos, usa un QTimer:
    QTimer::singleShot(2800, this, [this, cartelWinItem]() {
        if (cartelWinItem->scene()) removeItem(cartelWinItem);
        delete cartelWinItem;
    });
}

void GameScene::focusGoku() {
    if (jugadorGoku) {
        jugadorGoku->setFocus();
        qDebug() << "Foco puesto en Goku!";
    }
}

void GameScene::keyPressEvent(QKeyEvent* event) {
    // Le pasa la tecla a Goku SI existe
    if (jugadorGoku)
        jugadorGoku->keyPressEvent(event);
}

void GameScene::keyReleaseEvent(QKeyEvent* event) {
    if (jugadorGoku)
        jugadorGoku->keyReleaseEvent(event);
}

