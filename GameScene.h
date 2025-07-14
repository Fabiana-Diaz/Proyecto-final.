#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QList>
#include <QTimer>
#include "jugador.h"
#include "Plataforma.h"
//#include <QMediaPlayer>
//#include <QMediaPlaylist>



// 👇 Declaración adelantada para evitar error de tipo
class Banglades;

class GameScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GameScene(QObject *parent = nullptr);
    jugador* getJugadorGoku() const { return jugadorGoku; }


public slots:
    void mostrarVictoria();
    void focusGoku();


private:
    jugador* jugadorGoku;
    QList<Plataforma*> plataformas;
    QTimer* timerVendas;
    Banglades* enemigoBanglades;

    void lanzarVendaDesdeBanglades();  // función para lanzar vendas desde el enemigo
    QGraphicsTextItem* textoVidaGoku;
    QGraphicsTextItem* textoVidaBanglades;

    QGraphicsRectItem* barraVidaGoku;
    QGraphicsRectItem* barraVidaBanglades;




private slots:
    void detenerEnemigosYVendas();


    // Al final de tu clase GameScene
protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;



};

#endif // GAMESCENE_H

