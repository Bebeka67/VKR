#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

#define ID_GETSHORTWAY 0

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void closeEvent(QCloseEvent* e);

    void GraphButtonClick();


public slots:

    void MapView_SignalMouseRelease(int,int,int);

    void MapOpenAction_activated();
    void MapCloseAction_activated();
    void MapAppendSitAction_activated();

    void ExitAction_activated();

    void GetShortWayAction_activated();

public:

    Ui::MainWindow ui;

    HGRAPH FGraph;
    HSITE SitHandle;
    HOBJ hObj1;
    HOBJ hObj2;
    int CurrentRegime;
    QTextCodec *MainCodec;


private:

    QDMapView* MapView;
    QDMapNet* ObjNet;
    QDMapObj* MapObj1;
    QDMapObj* MapObj2;

    void runSetupMaps();

};
#endif // MAINWINDOW_H
