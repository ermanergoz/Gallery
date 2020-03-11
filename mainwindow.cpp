#include "mainwindow.h"
#include "gallerymodel.h"

#include <QMenuBar>
#include <QMenu>
#include <QLabel>
#include <QGridLayout>
#include <QFileDialog>

#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    widget = new QWidget;
    setCentralWidget(widget);

    layout = new QGridLayout;
    widget->setLayout(layout);

    createActions();
    createMenus();

    //QString message = tr("A context menu is available by right-clicking");
    //statusBar()->showMessage(message);

    setWindowTitle(tr("Gallery"));
    setMinimumSize(800, 600);
    resize(800, 600);


    model = new GalleryModel(100, 100);
    connect(model, &GalleryModel::imagesLoaded, this, &MainWindow::showImages);
    //connect(model, &GalleryModel::imageLoadError, this, &MainWindow::imageLoadError);
}

void MainWindow::open()
{
    QString dirPath = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Select Directory"), QDir::currentPath()));
    inputDir = QDir(dirPath);

    model->loadImages(inputDir);
}

void MainWindow::showImages()
{
    //labels.clear();
    //for(int i = 0; i < model->size(); ++i)
    //    labels.append(new QLabel);
    //
    //for(int row = 0; row<layout->rowCount(); ++row)
    //{
    //    for(int column=0; column<layout->columnCount();++column)
    //    {
    //        layout->removeItem()
    //    }
    //}

    for(int i = 0; i < labels.size(); ++i)
    {
        layout->removeWidget(labels[i]);
    }

    labels.clear();
    for(int i = 0; i < model->size(); ++i)
        labels.append(new QLabel);

    for(int row = 0; row < (model->size() + 3) / 4; ++row)
    {
        int colNum = 4;
        if(row == (model->size() + 3) / 4 - 1 && model->size() % 4 != 0) // last row
            colNum = model->size() % 4;
        for(int col = 0; col < colNum; ++col)
        {
            labels[row*4+col]->setPixmap((*model)[row*4+col]);
            labels[row*4+col]->setBaseSize(QSize(100,100));
            layout->addWidget(labels[row*4+col], row, col);
        }
    }
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
}


MainWindow::~MainWindow()
{

}
