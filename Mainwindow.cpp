#include "Mainwindow.h"
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent):
   QMainWindow(parent)
{
        createMenu();
        setMinimumSize(400, 400);
        setWindowTitle("Paint movable polygons on image");
        widget = NULL;
}
MainWindow::~MainWindow()
{
        if (widget)
            delete widget;
        delete pmnu;
}

void MainWindow::createMenu()
{
    pmnu = new QMenu("&Menu", this);

    QAction * saveAction = new QAction(tr("&Load image"), this);
    QAction * paintAction = new QAction(tr("&Paint"), this);
    QAction * stopAction = new QAction(tr("&Stop paint"), this);

    connect(saveAction, SIGNAL(triggered()), this, SLOT(showLoadMenu()));
    connect(paintAction, SIGNAL(triggered()), this, SLOT(paintPol()));
    connect(stopAction, SIGNAL(triggered()), this, SLOT(stopPol()));

    pmnu->addAction(saveAction);

    pmnu->addAction(paintAction);
    pmnu->addAction(stopAction);

    menuBar()->addMenu(pmnu);

    show();

}

void MainWindow::showLoadMenu()
{
    if(widget)
        delete widget;

    QString filename = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.jpg *.png");
    widget = new GraphWidget(this, filename);
    setCentralWidget(widget);

}
void MainWindow::paintPol()
{
    if (widget)
    {
        widget ->getscene()->setPaint(true);
    }

}
void MainWindow::stopPol()
{
    if (widget)
    {
        widget ->getscene()->setPaint(false);
    }

}
