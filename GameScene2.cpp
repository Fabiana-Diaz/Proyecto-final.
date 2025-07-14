#include "GameScene2.h"
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QTimer>
#include <QKeyEvent>
#include <QRandomGenerator>
#include "Plataforma.h"
#include "jugador.h"
#include "Akkuman.h"
#include "Resplandor.h"
#include "Estrella.h"  // Incluye la clase Estrella



// Prototipo función colisión nivel 2 (si usas fuera de jugador.cpp)
void resolverColisionNivel2(jugador* goku, Plataforma* plataforma);

GameScene2::GameScene2(QObject *parent)
    : QGraphicsScene(parent)
{
    setSceneRect(0, 0, 1280, 853);

    // Fondo
    QGraphicsPixmapItem* fondo = new QGraphicsPixmapItem(QPixmap(":/sprites/fondo.jpg"));
    fondo->setPos(0, 0);
    fondo->setZValue(-10);
    addItem(fondo);

    // Plataforma principal
    int plataformaX = 210;
    int plataformaY = 460;
    int plataformaW = 860;
    int plataformaH = 40;
    plataformaPrincipal = new Plataforma(plataformaX, plataformaY, plataformaW, plataformaH, false);
    //plataformaPrincipal->setBrush(Qt::blue);
    //plataformaPrincipal->setOpacity(0.5);
    addItem(plataformaPrincipal);

    plataformas.append(plataformaPrincipal);

    // Crear jugador Goku
    int gokuX = plataformaX + 10;
    int gokuY = plataformaY - 100;
    jugadorGoku = new jugador(gokuX, gokuY);
    jugadorGoku->setZValue(4);
    addItem(jugadorGoku);
    jugadorGoku->verificarColisionInicialNivel2(plataformas);

    // Crear Akkuman
    akkuman = new Akkuman(800, 200, 1.0f);
    akkuman->setObjetivo(jugadorGoku);
    akkuman->setPixmap(QPixmap(":/sprites/akkumam.png").scaled(140, 140, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    addItem(akkuman);
    akkuman->iniciarVuelo();

    // Conectar solo para mostrar victoria, sin eliminar aquí
    connect(akkuman, &Akkuman::enemigoDerrotado, this, [=](Akkuman* enemigo){
        qDebug() << "Akkuman derrotado, mostrando victoria...";
        if (jugadorGoku) {
            jugadorGoku->mostrarVictoria();
        }
    });

    // Barra de vida y texto para Goku
    QGraphicsRectItem* barraVidaGoku = new QGraphicsRectItem(0, 0, 100, 10);
    barraVidaGoku->setBrush(QColor(255, 140, 0)); // Naranja
    barraVidaGoku->setPen(Qt::NoPen);
    barraVidaGoku->setPos(10, 10);
    addItem(barraVidaGoku);

    QGraphicsTextItem* textoVidaGoku = new QGraphicsTextItem("Goku: 100");
    textoVidaGoku->setDefaultTextColor(QColor(255, 140, 0));
    textoVidaGoku->setFont(QFont("Arial", 14, QFont::Bold));
    textoVidaGoku->setPos(10, 20);
    addItem(textoVidaGoku);

    jugadorGoku->setBarraVida(barraVidaGoku);
    jugadorGoku->setTextoVida(textoVidaGoku);

    // Barra de vida y texto para Akkuman
    QGraphicsRectItem* barraVidaAkkuman = new QGraphicsRectItem(0, 0, 150, 10);
    barraVidaAkkuman->setBrush(QColor(46, 139, 87)); // Verde
    barraVidaAkkuman->setPen(Qt::NoPen);
    barraVidaAkkuman->setPos(sceneRect().width() - 160, 10);
    addItem(barraVidaAkkuman);

    QGraphicsTextItem* textoVidaAkkuman = new QGraphicsTextItem("Akkuman: 150");
    textoVidaAkkuman->setDefaultTextColor(QColor(46, 139, 87));
    textoVidaAkkuman->setFont(QFont("Arial", 12, QFont::Bold));
    textoVidaAkkuman->setPos(sceneRect().width() - 160, 15);
    addItem(textoVidaAkkuman);

    akkuman->setBarraVida(barraVidaAkkuman);
    akkuman->setTextoVida(textoVidaAkkuman);

    // Para Goku
    QGraphicsPixmapItem* corazonGoku = new QGraphicsPixmapItem(QPixmap(":/sprites/corazon.png").scaled(25, 25, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    corazonGoku->setPos(10 - 10, 5);  // Ajusta la posición un poco a la izquierda y arriba de la barra
    addItem(corazonGoku);

    // Repite el mismo patrón para Akkuman
    QGraphicsPixmapItem* corazonAkkuman = new QGraphicsPixmapItem(QPixmap(":/sprites/corazon.png").scaled(25, 25, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    corazonAkkuman->setPos(sceneRect().width() - 160 - 30, 5);
    addItem(corazonAkkuman);




    // Conectar señal para eliminar Akkuman cuando sea derrotado
    connect(akkuman, &Akkuman::enemigoDerrotado, this, [=](Akkuman* enemigo){
        if (this->items().contains(enemigo)) {
            this->removeItem(enemigo);
        }
        enemigo->deleteLater();
        mostrarVictoria();
    });

    // Debug: posición de Goku
    qDebug() << "Goku en X:" << jugadorGoku->x()
             << "Y:" << jugadorGoku->y()
             << "Ancho:" << jugadorGoku->boundingRect().width()
             << "Alto:" << jugadorGoku->boundingRect().height();

    // Timer para física y lógica
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        bool aterrizo = false;
        for (Plataforma* plat : plataformas) {
            jugadorGoku->resolverColisionNivel2(plat);
            if (jugadorGoku->getEnPlataforma()) {
                aterrizo = true;
                break;
            }
        }

        if (!aterrizo) {
            jugadorGoku->setEnPlataforma(false);
            jugadorGoku->activarCaidaLibreNivel2();
        }

        jugadorGoku->aplicarFisicaNivel2();

        akkuman->mover();
    });
    timer->start(16);

    // Timer para crear estrellas que caen
    QTimer* timerEstrellas = new QTimer(this);
    connect(timerEstrellas, &QTimer::timeout, this, [=]() {
        // Posición aleatoria X para la estrella que caerá
        float xEstrella = QRandomGenerator::global()->bounded(sceneRect().width() - 40);
        Estrella* estrella = new Estrella(xEstrella, 0, jugadorGoku);
        addItem(estrella);
    });
    timerEstrellas->start(3000);  // Cada 3 segundos cae una estrella
}

void GameScene2::keyPressEvent(QKeyEvent* event) {
    if (jugadorGoku)
        jugadorGoku->keyPressEvent(event);
}

void GameScene2::keyReleaseEvent(QKeyEvent* event) {
    if (jugadorGoku)
        jugadorGoku->keyReleaseEvent(event);
}

void GameScene2::focusGoku() {
    if (jugadorGoku)
        jugadorGoku->setFocus();
}

void GameScene2::mostrarVictoria() {
    jugadorGoku->mostrarVictoria();

    QPixmap cartelWin(":/sprites/you_win.png");
    auto* cartelWinItem = new QGraphicsPixmapItem(cartelWin.scaled(320, 240, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    cartelWinItem->setZValue(30);

    QRectF rect = sceneRect();  // Usamos sceneRect() para posición centrada
    cartelWinItem->setPos(rect.width()/2 - cartelWinItem->boundingRect().width()/2,
                          rect.height()/2 - cartelWinItem->boundingRect().height()/2);
    addItem(cartelWinItem);

    QTimer::singleShot(2800, this, [this, cartelWinItem]() {
        if (cartelWinItem && cartelWinItem->scene()) {
            this->removeItem(cartelWinItem);
            delete cartelWinItem;  // Liberamos memoria correctamente aquí
        }
    });
}

