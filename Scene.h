#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>

/*!
     \brief Класс, позволяющий рисовать полигон мышью
 */
class paintScene : public QGraphicsScene
{

    Q_OBJECT

public:
    explicit paintScene(QObject *parent = 0);
    ~paintScene();
    QList <QPointF> getpoints() {return points;}
    void setPaint(bool key) {paint_or_not = key;}
    bool getPaint() {return paint_or_not;}

private:
    QPointF     previousPoint;      // Координаты предыдущей точки
    QList <QGraphicsItem*> list;
    QList<QPointF> points;
    bool paint_or_not;
private:
    // Для рисования используем события мыши
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void removeAfterPaint();

};

#endif // PAINTSCENE_H
