/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "Graphwidget.h"
#include "Edge.h"
#include "Node.h"
#include "Scene.h"

#include "math.h"
#include <QDir>
#include <QKeyEvent>
#include <QFileDialog>

GraphWidget::GraphWidget(QWidget *parent, QString str)
    : QGraphicsView(parent), filename (str)
{
    scene = new paintScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-200, -200, 400, 400);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(400, 400);

    group = NULL;

}
GraphWidget::~GraphWidget()
{

    deleteAfterWork();

}

void  GraphWidget::deleteAfterWork()
{
    if (node.size() != 0)
    {
        for (Node* node: qAsConst(node))
            delete node;
    }

    if (edge.size() != 0)
    {
        for (Edge* edge: qAsConst(edge))
            delete edge;
    }

    const QList<QGraphicsItem* > items = scene->items();

    for(QGraphicsItem* item : items)
        delete item;

    delete scene;
}

void GraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}


void GraphWidget::groupBy()
{
    QList<QGraphicsItem*> items;

    for(Node* node: qAsConst(node) )
    {
        items.append(qgraphicsitem_cast<QGraphicsItem*>(node));
    }
    for(Edge* edge: qAsConst(edge) )
    {
        items.append(qgraphicsitem_cast<QGraphicsItem*>(edge));
    }

    group = scene->createItemGroup(items);
    group->setCursor(Qt::PointingHandCursor);

    group->setFlag(QGraphicsItem::ItemIsMovable);
}

void GraphWidget::fallGroupBy()
{
    if (group)
    {
        QList<QGraphicsItem*> items;

        for(Node* node: qAsConst(node) )
        {
            items.append(qgraphicsitem_cast<QGraphicsItem*>(node));
        }
        for(Edge* edge: qAsConst(edge) )
        {
            items.append(qgraphicsitem_cast<QGraphicsItem*>(edge));
        }

        for (QGraphicsItem* item: items)
        {
            group->removeFromGroup(item);
        }
    }
}

void GraphWidget::setItems()
{
    QList<QPointF> source;
    Node *nodetmp = NULL;
    QList<QPointF> list = scene->getpoints();

    source = list;

    //deleteAfterWork();
    edge.clear();
    node.clear();

    for(int i = 0; i < source.size() ; ++i)
    {
        Node *nodem = new Node(this);
        node.append(nodem);
        scene->addItem(nodem);
        if (i != 0)
        {
            Edge *nod = new Edge(nodem, nodetmp);
            scene->addItem(nod);
            edge.append(nod);
        }
        nodetmp = nodem;
        nodem->setPos(source[i].x(), source[i].y());

        if (i == source.size() - 1 )
        {
            Edge *nod = new Edge(nodem, qgraphicsitem_cast<Node *> (node[0]));
            scene->addItem(nod);
            edge.append(nod);
        }
    }

}

void GraphWidget::mousePressEvent(QMouseEvent *mouseEvent)
{
    if(mouseEvent->button()== Qt::RightButton && scene->getPaint())
        viewport()->setCursor(Qt::CrossCursor);

    if(scene->getPaint())
    {
        fallGroupBy();

        for (Node *node : qAsConst(this->node))
        {
            if(node->contains(mapToScene(mouseEvent->pos())))
            {
                fallGroupBy();
                QGraphicsView::mousePressEvent(mouseEvent);
                return;
            }
        }

        QPolygonF pol;
        for(Node* node  : qAsConst(this->node))
        {
            pol << node->pos();
        }

        QGraphicsPolygonItem polygon (pol);

        if (polygon.contains(mapToScene(mouseEvent->pos())))
        {
            groupBy();
            QGraphicsView::mousePressEvent(mouseEvent);

            return;
        }
    }

     QGraphicsView::mousePressEvent(mouseEvent);

}
void GraphWidget::mouseDoubleClickEvent(QMouseEvent *mouseEvent)
{
    setItems();
    viewport()->setCursor(Qt::ArrowCursor);
    QGraphicsView::mouseDoubleClickEvent(mouseEvent);

}

void GraphWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    QList<Node*> nodes;
    const QList<QGraphicsItem *> items = scene->items();
    for (QGraphicsItem *item : items)
    {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
        {
            nodes << node;
        }
    }

    for (Node *node : qAsConst(nodes))
    {
        node->changePos();

    }

    bool itemsMoved = false;
    for (Node *node : qAsConst(nodes)) {
        if (node->setPosition())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }

}

void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow(2., -event->angleDelta().y() / 240.0));
}

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);
    QRectF sceneRect = this->sceneRect();
    painter->drawPixmap(QRect(sceneRect.left(), sceneRect.top(), sceneRect.width(), sceneRect.height()), QPixmap(this->filename));

}

void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

void GraphWidget::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);

    fitInView(this->sceneRect(), Qt::KeepAspectRatio);
}


