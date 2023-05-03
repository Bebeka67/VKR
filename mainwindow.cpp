#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    MainCodec = QTextCodec::codecForName("KOI8-R");

    ui.setupUi(this);
    MapView = ui.DMapView;
    ObjNet = ui.ObjNet;
    MapObj1 = ui.MapObj1;
    MapObj2 = ui.MapObj2;

    ObjNet->SetMapView(MapView);
    MapObj1->SetMapView(MapView);
    MapObj2->SetMapView(MapView);

    ui.ObjNet->SetMapView(ui.DMapView);
    ui.MapObj1->SetMapView(ui.DMapView);
    ui.MapObj2->SetMapView(ui.DMapView);

    FGraph    = 0;
    SitHandle = 0;
    hObj1     = 0;
    hObj2     = 0;
    CurrentRegime = -1;


    // создаем коннекты для связи интерфейса и задника

    connect(ui.ActionOpenMap,       &QAction::triggered, this, &MainWindow::MapOpenAction_activated);
    connect(ui.ActionCloseMap,      &QAction::triggered, this, &MainWindow::MapCloseAction_activated);
    connect(ui.ActionAppendSitNet,  &QAction::triggered, this, &MainWindow::MapAppendSitAction_activated);
    connect(ui.ActionExit,          &QAction::triggered, this, &MainWindow::ExitAction_activated);

    runSetupMaps();
}

MainWindow::~MainWindow()
{
    if(FGraph != 0)
    {
        ObjNet->CloseGraph(FGraph);
        FGraph = 0;
    }

    SitHandle = 0; // Идентификатор сайта графа дорог
    hObj1     = 0;
    hObj2     = 0;
    CurrentRegime = -1;

    if (MapView->GetMapHandle() != 0)
    {
        MapView->SetMapVisible(FALSE);
        MapView->SetMapActive(FALSE);
    }

    delete MapView;
    delete ObjNet;
    delete MapObj1;
    delete MapObj2;
}

void MainWindow::closeEvent(QCloseEvent *e)
{
  MapView->SetMapVisible(FALSE);
  MapView->SetMapActive(FALSE);

  return QMainWindow::closeEvent(e);
}


void MainWindow::MapOpenAction_activated()
{
    // Выбрать имя файла  TODO добавить директорию с исходниками в собранную версию
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите базовую карту", "/home/andpa/Desktop/common data", "Sites (*.sit);; Maps (*.map)");

    if(fileName.isEmpty())
        return; // карта не выбрана

    // обнуляем граф и все идентификаторы
    if(FGraph != 0)
    {
        ObjNet->CloseGraph(FGraph);
        FGraph = 0;
    }

    SitHandle = 0; // Идентификатор сайта графа дорог
    hObj1     = 0;
    hObj2     = 0;
    CurrentRegime = -1;


    MapView->SetMapFileName(fileName);
    MapView->SetMapActive(TRUE);
    MapView->SetMapVisible(TRUE);
}
void MainWindow::MapCloseAction_activated()
{
    if  (FGraph != 0)
    {
      ObjNet->CloseGraph(FGraph);
      FGraph = 0;
    }

    SitHandle = 0; // Идентификатор сайта графа дорог
    hObj1     = 0;
    hObj2     = 0;
    CurrentRegime = -1;

    MapView->SetMapVisible(FALSE);
    MapView->SetMapActive(FALSE);
}
void MainWindow::MapAppendSitAction_activated()
{
    if (MapView->GetMapHandle() == 0)
    {
        QMessageBox::critical(this, QString::fromUtf16(WS("Добавить сайт")),
                              QString::fromUtf16(WS("Не открыта базовая карта!")),
                              QString::fromUtf16(WS("Да")));
        return;
    }

    // Выбрать имя файла
    QString FileName = QFileDialog::getOpenFileName(this, "Выберите граф дорог", "/home/andpa/Desktop/common data/graph", QString::fromUtf16(WS("Файл карты с сетью (*.sit)")));

    if (FileName.isEmpty()) return; // Если карта не выбрана

    MapView->VarMapSites->Append(MainCodec->fromUnicode(FileName).data());
    ObjNet->SetNetName(MainCodec->fromUnicode(FileName).data());
    MapView->Repaint();

    if (FGraph != 0) ObjNet->CloseGraph(FGraph);
    if (ObjNet->OpenGraph(MapView->GetMapHandle(), ObjNet->FHSite) == 0) return;
    FGraph    = ObjNet->GetGraphHandle();
    SitHandle = ObjNet->FHSite;
    hObj1     = 0;
    hObj2     = 0;
    CurrentRegime = -1;
}


void MainWindow::GraphButtonClick()
{
    if(!MapView->GetMapHandle())
        return;

    hObj1 = 0;
    hObj2 = 0;
    MapObj1->SetStyle(QDMapObj::OS_NOVIEW);
    MapObj2->SetStyle(QDMapObj::OS_NOVIEW);

    MapView->SetObjSelected(NULL, 0, true);
    CurrentRegime = -1;
}


void MainWindow::runSetupMaps()
{
    qDebug("Runs");
    QString fileMapName = "/home/andpa/Desktop/common data/Data/Планы городов/Noginsk_3857/Noginsk.sit";
    QString fileSitName = "/home/andpa/Desktop/common data/graph/Noginsk_graph_bugfix.sit";
//    QString fileName = "/home/andpa/Desktop/common data/Data/Планы городов/Noginsk_SK42/Noginsk.sit";

    // обнуляем граф и все идентификаторы


    MapView->SetMapFileName(fileMapName);
    MapView->SetMapActive(TRUE);
    MapView->SetMapVisible(TRUE);

    if (FGraph != 0) ObjNet->CloseGraph(FGraph);
    if (ObjNet->OpenGraph(MapView->GetMapHandle(), ObjNet->FHSite) == 0) return;
    FGraph    = ObjNet->GetGraphHandle();
    SitHandle = ObjNet->FHSite;

    SitHandle = 0; // Идентификатор сайта графа дорог
    hObj1     = 0;
    hObj2     = 0;
    CurrentRegime = -1;


    MapView->VarMapSites->Append(MainCodec->fromUnicode(fileSitName).data());
    ObjNet->SetNetName(MainCodec->fromUnicode(fileSitName).data());
    MapView->Repaint();

    qDebug("antiRuns");
}






void MainWindow::ExitAction_activated()
{
    close();
}
