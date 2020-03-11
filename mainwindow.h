#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QVector>

class QAction;
class QMenu;
class QGridLayout;
class GalleryModel;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QGridLayout* layout;

    QWidget* widget;
    QMenu* fileMenu;
    QAction* openAct;

    QDir inputDir;

    GalleryModel* model;

    QVector<QLabel*> labels;

    void createActions();
    void createMenus();

private slots:
    void open();
    void showImages();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
