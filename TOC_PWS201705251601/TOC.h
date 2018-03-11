#ifndef TOC_H
#define TOC_H

#include"QWidget"
#include <QMainWindow>
#include"S7/snap7.h"
#include"S7/TypeTransform.h"
#include"QMessageBox"
#include"QEvent"
#include"QCloseEvent"
#include"QThread"
#include"Thread/FreshThread.h"
#include"QDateTime"
#include <QtSql>
#include <QDebug>
#include"Sql/SqlData.h"
#include"Sql/exportdatabasetoexcel.h"
#include"QPixmap"
#include <QApplication>
#include"AxiaSetting/YAxiaSetting.h"
#include"AxiaSetting/XAxiaSetting.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QValueAxis>
#include"QtCharts/QSplineSeries"



QT_CHARTS_USE_NAMESPACE

namespace Ui {
class TOC;
}

class TOC : public QMainWindow
{
    Q_OBJECT

public:
    explicit TOC(QWidget *parent = 0);
    ~TOC();
public slots:
    void refreshPLC();
    void refreshLabel();
    void refreshChart();
    void TimerReverse();
    void OtherThing();

private slots:
    void on_btn_Waiting_clicked();
    void on_btn_Runing_clicked();
    void on_btn_ScreenPrint_clicked();
    void on_btn_dataExpert_clicked();
    void on_rb_NeuralNetwork_clicked();
    void on_rb_handle_clicked();
    void on_rb_BackPropagation_clicked();
    void on_rb_convolution_clicked();
    void on_btn_setting_clicked();
    void on_btn_YAxisSetting_clicked();
    void on_btn_XAxisSetting_clicked();


private:
    void closeEvent( QCloseEvent*  event);
    void sqlControl();

private:
    void initFrame();
    void initAlgorithminit();
    void PLCCommunciation();
    void Chart_View();
    void Chartrefresh();
    void AlgorithmControl();

private:
    Ui::TOC *ui;
    QDateTime *initTime;
    QDateTime *SystemTime;
    TS7Client *MyPlc;
    TypeTransform *S7;
    FreshThread ThreadPLC;
    FreshThread ThreadLabel;
    FreshThread ThreadChart;
    byte WriteBuffer[256];
    byte ReadBuffer[256];
    bool connectResult;
    int PlcConnectTimeWait=0;
    QLineSeries *seriesTOCIn;
    QLineSeries *seriesTOCOut;
    QChart* chart;
    QValueAxis *axisX;
    QValueAxis *axisY;
    QList<QPointF> PointTocIn;
    QList<QPointF> PointTocOut;
    //
    QTimer Timer1s;
    QTimer Timer;
    bool backgroundFlag=1;
    QSqlDatabase db;
    SqlData sql;
    ExportDataBaseToExcel* sqlExport;
    double a[1000];
    double b[1000];
    int systemtime=0;
    int t=0;
    bool chartrefreshflag=1;
    bool singledouble=0;
    int exitTime=72;
    // data exchange
    // send
    int status = 0;
    double flowControl_1_flow_send = 0;
    double flowControl_2_flow_send = 0;
    double flowControl_3_flow_send = 0;
    double rpump = 0;
    // receive, temp useless
    double flowControl_1_flow_rec = 0;
    double flowControl_1_pressure_rec = 0;
    double flowControl_2_flow_rec = 0;
    double flowControl_2_pressure_rec = 0;
    double flowControl_3_flow_rec = 0;
    double flowControl_3_pressure_rec = 0;
    double pump_speed_actual = 0;
    double toc_inlet_number = 0;
    double toc_outlet_number = 0;
};

#endif // TOC_H
