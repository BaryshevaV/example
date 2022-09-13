#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include "Graphwidget.h"

/*!
     \brief Класс главного окна
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    GraphWidget* widget;
    QMenu* pmnu;
    void createMenu();

public slots:
    void showLoadMenu();
    void paintPol();
    void stopPol();

public :
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

};












#endif // MAINWINDOW_H
