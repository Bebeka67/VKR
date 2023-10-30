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

    void MapView_SignalMouseRelease(int mouseX,int mouseY, int additional_key);


    void FileMenu_OpenMap_Button_clicked();
    void FileMenu_CloseMap_Button_clicked();
    void FileMenu_AppendSitNet_Button_clicked();
    void FileMenu_Exit_Button_clicked();

    void GraphMenu_GetShortWay_Button_clicked();


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









    QString source_path = "/home/andrey/Projects/Source/";
};
#endif // MAINWINDOW_H
