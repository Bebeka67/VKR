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

    // �������� �������� �� �� �������

    connect(MapView, &QDMapView::SignalMouseRelease, this, &MainWindow::MapView_SignalMouseRelease);

    //MENU_FILE Buttons

    connect(ui.FileMenu_OpenMap_Button,         &QAction::triggered, this, &MainWindow::FileMenu_OpenMap_Button_clicked);
    connect(ui.FileMenu_CloseMap_Button,        &QAction::triggered, this, &MainWindow::FileMenu_CloseMap_Button_clicked);
    connect(ui.FileMenu_AppendSitNet_Button,    &QAction::triggered, this, &MainWindow::FileMenu_AppendSitNet_Button_clicked);
    connect(ui.FileMenu_Exit_Button,            &QAction::triggered, this, &MainWindow::FileMenu_Exit_Button_clicked);

    connect(ui.GraphMenu_GetShortWay_Button,    &QAction::triggered, this, &MainWindow::GraphMenu_GetShortWay_Button_clicked);



    runSetupMaps();
}

MainWindow::~MainWindow()
{
    if(FGraph != 0)
    {
        ObjNet->CloseGraph(FGraph);
        FGraph = 0;
    }

    SitHandle = 0; // ������������� ����� ����� �����
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

void MainWindow::MapView_SignalMouseRelease(int mouseX,int mouseY, int additional_key)
{
    Q_UNUSED(additional_key);
    if(!MapView->GetMapHandle()) return;

    int left = 0;
    int top = 0;
    int ret = 0;

    DOUBLEPOINT dpoint;

    MapView->GetMapLeftTop(&left, &top);

    dpoint.X = left + mouseX;
    dpoint.Y = top + mouseY;
    MapView->ConvertMetric(&dpoint.X, &dpoint.Y, PP_PICTURE, PP_PLANE);


    switch (CurrentRegime) {
    case ID_GETSHORTWAY:
    {
        if(hObj1 == 0)
        {
            if(ObjNet->GetNodeByPoint(MapObj1->VarObjHandle, &dpoint) != 0)
            {
                MapObj1->SetStyle(QDMapObj::OS_SELECT);
                hObj1 = MapObj1->VarObjHandle;
            }
        }
        else
        {
            if(hObj2 == 0)
            {
                if(ObjNet->GetNodeByPoint(MapObj2->VarObjHandle, &dpoint) != 0)
                {
                    MapObj2->SetStyle(QDMapObj::OS_SELECT);
                    hObj2 = MapObj2->VarObjHandle;
                }
            }
        }

        break;
    }
    default:
        break;
    }


}


void MainWindow::FileMenu_OpenMap_Button_clicked()
{
    // ������� ��� �����  TODO �������� ���������� � ����������� � ��������� ������
    QString fileName = QFileDialog::getOpenFileName(this,                       // widget parent
                                                    "�������� ������� �����",   // title
                                                    QDir::currentPath(),
                                                    "Sites (*.sit);; Maps (*.map)");

    if(fileName.isEmpty())
        return; // ����� �� �������

    // �������� ���� � ��� ��������������
    if(FGraph != 0)
    {
        ObjNet->CloseGraph(FGraph);
        FGraph = 0;
    }

    SitHandle = 0; // ������������� ����� ����� �����
    hObj1     = 0;
    hObj2     = 0;
    CurrentRegime = -1;

    MapView->SetMapFileName(fileName);
    MapView->SetMapActive(TRUE);
    MapView->SetMapVisible(TRUE);
}

void MainWindow::FileMenu_CloseMap_Button_clicked()
{
    if  (FGraph != 0)
    {
      ObjNet->CloseGraph(FGraph);
      FGraph = 0;
    }

    SitHandle = 0; // ������������� ����� �����
    hObj1     = 0;
    hObj2     = 0;
    CurrentRegime = -1;

    MapView->SetMapVisible(FALSE);
    MapView->SetMapActive(FALSE);
}

void MainWindow::FileMenu_AppendSitNet_Button_clicked()
{
    if (MapView->GetMapHandle() == 0)
    {
        QMessageBox::critical(this, QString::fromUtf16(WS("�������� ����")),
                              QString::fromUtf16(WS("�� ������� ������� �����!")),
                              QString::fromUtf16(WS("��")));
        return;
    }

    // ������� ��� �����
    QString FileName = QFileDialog::getOpenFileName(this, "�������� ���� �����", QDir::currentPath(), QString::fromUtf16(WS("���� ����� � ������ ����� (*.sit)")));

    if (FileName.isEmpty()) return; // ���� ����� �� �������

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

void MainWindow::GraphMenu_GetShortWay_Button_clicked()
{
    GraphButtonClick();
    CurrentRegime = ID_GETSHORTWAY;
    ui.statusbar->showMessage("������ ����� ������ ����", 5000);
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
    QString fileMapName = source_path + "Noginsk/Noginsk.sit";//"/home/andpa/Desktop/common data/Data/����� �������/Noginsk_3857/Noginsk.sit";
//    QString fileName = "/home/andpa/Desktop/common data/Data/����� �������/Noginsk_SK42/Noginsk.sit";

    MapView->SetMapFileName(fileMapName);
    MapView->SetMapActive(TRUE);
    MapView->SetMapVisible(TRUE);

    if (MapView->GetMapHandle() == 0)
    {
        QMessageBox::critical(this, QString::fromUtf16(WS("�������� ����")),
                              QString::fromUtf16(WS("�� ������� ������� �����!")),
                              QString::fromUtf16(WS("��")));
        return;
    }

    QString fileSitName = source_path+"graph/Noginsk_graph_bugfix.sit";// "/home/andpa/Desktop/common data/graph/Noginsk_graph_bugfix.sit";

    MapView->VarMapSites->Append(MainCodec->fromUnicode(fileSitName).data());
    ObjNet->SetNetName(MainCodec->fromUnicode(fileSitName).data());
    MapView->Repaint();

    if (FGraph != 0) ObjNet->CloseGraph(FGraph);
    if (ObjNet->OpenGraph(MapView->GetMapHandle(), ObjNet->FHSite) == 0) return;
    FGraph    = ObjNet->GetGraphHandle();
    SitHandle = ObjNet->FHSite;
    hObj1     = 0;
    hObj2     = 0;
    CurrentRegime = -1;
}

void MainWindow::FileMenu_Exit_Button_clicked()
{
    close();
}
