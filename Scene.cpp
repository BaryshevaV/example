#include "Scene.h"
#include <QKeyEvent>
#include <QCursor>
#include <QApplication>
#include <QGraphicsItem>

paintScene::paintScene(QObject *parent) : QGraphicsScene(parent)
{
    paint_or_not = false;
}

paintScene::~paintScene()
{

}

void paintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (paint_or_not)
    {
        if (event->button() == Qt::RightButton)
        {
            if (points.size() ==0)
                previousPoint = event->scenePos();

            QGraphicsLineItem * item = addLine(previousPoint.x(),
                previousPoint.y(),
                event->scenePos().x(),
                event->scenePos().y(),
                QPen(Qt::red,1,Qt::SolidLine,Qt::RoundCap));

            list.append(qgraphicsitem_cast<QGraphicsItem *>(item));
            points.append(previousPoint);

            previousPoint = event->scenePos();

        }
    }

    QGraphicsScene::mousePressEvent(event);
}

void paintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    QGraphicsScene::mouseMoveEvent(event);
}
void paintScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(paint_or_not)
        removeAfterPaint();
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);

}
void paintScene::removeAfterPaint()
{
    for(int i = 0; i < list.size(); ++i)
    {
        QGraphicsScene::removeItem(list[i]);

    }
    points.clear();
    list.clear();

}
