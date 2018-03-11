#include "TOC.h"
#include "ui_TOC.h"
QT_CHARTS_USE_NAMESPACE
using namespace std;

TOC::TOC(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TOC)
{
    ui->setupUi(this);
    //数据库新建函数
    setWindowIcon(QIcon("cheer.ico"));     
    sql.connect_db("jinda.db",db);        sql.create_table();      sqlControl();
    //构造函数
    SystemTime=new QDateTime();   initTime=new QDateTime();     S7 = new TypeTransform();      MyPlc=new TS7Client();
    seriesTOCIn = new QLineSeries();   chart = new QChart();   seriesTOCOut = new QLineSeries();        sqlExport=new ExportDataBaseToExcel();
    axisX = new QValueAxis;       axisY = new QValueAxis;
    //初始化界面函数
    ThreadPLC.start();        ThreadLabel.start();      ThreadChart.start();        Timer.start(3600*1000);         Timer1s.start(1000);
    initFrame();
    //信号函数
    connect(&ThreadPLC,SIGNAL(refreshPLC()),this,SLOT(refreshPLC()),Qt::DirectConnection);
    connect(&ThreadLabel,SIGNAL(refreshLabel()),this,SLOT(refreshLabel()),Qt::DirectConnection);
    connect(&ThreadChart,SIGNAL(refreshChart()),this,SLOT(refreshChart()),Qt::DirectConnection);
    connect(&Timer,SIGNAL(timeout()),this,SLOT(TimerReverse()));
    connect(&Timer1s,SIGNAL(timeout()),this,SLOT(OtherThing()));
}
//---------------------------------------------------------------------------
TOC::~TOC()
{
    delete ui;
}
//---------------------------------------------------------------------------
void TOC::initFrame()
{
    Chart_View();
    initAlgorithminit();
    ui->label_SystemStateValue->setText(QString::fromLocal8Bit("待机"));
    for(int i=0;i<256;i++)
    {
         WriteBuffer[i]=0;
         ReadBuffer[i]=0;
    }
    //plc
    if(!MyPlc->ConnectTo("192.168.0.1",0,1))
    {
        ui->label_Plc->setText(QString::fromLocal8Bit("已连接到PLC"));
    }
    else
    {
        ui->label_Plc->setText(QString::fromLocal8Bit("未找到PLC"));
    }

}
//---------------------------------------------------------------------------
void TOC::refreshPLC()
{
    PLCCommunciation();
    sqlControl();
}
//---------------------------------------------------------------------------
void TOC::refreshLabel()
{
    AlgorithmControl();
    ui->label_Time->setText(QString(SystemTime->currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
    ui->label_exitTime->setText(QString::number(exitTime)+QString::fromLocal8Bit("小时"));
    ui->btn_toc_inlet_number->setText(QString::number(toc_inlet_number) + " ppb");
    ui->btn_toc_outlet_number->setText(QString::number(toc_outlet_number) + " ppb");
    ui->btn_inletwater_flow->setText(QString::number(flowControl_3_flow_rec) + " L/min");
    ui->btn_inletwater_pressure->setText(QString::number(flowControl_3_pressure_rec) + " kPa");
    ui->btn_backwater_1flow->setText(QString::number(flowControl_1_flow_rec) + " L/min");
    ui->btn_backwater_1pressure->setText(QString::number(flowControl_1_pressure_rec) + " kPa");
    ui->btn_backwater_2flow->setText(QString::number(flowControl_2_flow_rec) + " L/min");
    ui->btn_backwater_2pressure->setText(QString::number(flowControl_2_pressure_rec) + " kPa");
    ui->btn_rpump_actual->setText(QString::number(pump_speed_actual) + " r/min");
}
//---------------------------------------------------------------------------
void TOC::TimerReverse()
{
    exitTime-=1;
    if(exitTime==0)
    {
        exitTime=72;
        sqlExport->exportData(db,"MetersParaM","AutoSave",1);
    }
}
//---------------------------------------------------------------------------
void TOC::refreshChart()
{
    if(chartrefreshflag)
        Chartrefresh();
    if(t==4)
    {

        if(systemtime<=800)
            systemtime++;
        else
            systemtime=0;
        t=0;
    }
    t++;
}
//---------------------------------------------------------------------------
void TOC::PLCCommunciation()
{
    if(PlcConnectTimeWait==0)
    {
        if(!MyPlc->ConnectTo("192.168.0.1",0,1))
        {
           connectResult=1;
        }
        else
        {
           connectResult=0;
           PlcConnectTimeWait=50; //5S连接一次
        }
    }
    if(PlcConnectTimeWait)
    {
        PlcConnectTimeWait--;
    }
    if(connectResult)
    {
        ui->label_Plc->setText(QString::fromLocal8Bit("已连接到PLC"));
        //信号传输完毕,接收信号

        switch (status)
        {
            case 0||1:
                ui->label_SystemStateValue->setText(QString::fromLocal8Bit("待机"));
                break;
            case 2:
                ui->label_SystemStateValue->setText(QString::fromLocal8Bit("实验"));
                MyPlc->DBRead(2,0,36,&ReadBuffer);
                MyPlc->DBWrite(1,0,50,&WriteBuffer);
                break;
            default:
                break;
        }


        //这里是信号传输
        S7->NumToByte(WriteBuffer, 0, status);  //status send
        S7->NumToByte(WriteBuffer, 2, flowControl_1_flow_send); // flow_control 1's flow send
        S7->NumToByte(WriteBuffer, 14, flowControl_2_flow_send); // flow_control 2's flow send
        S7->NumToByte(WriteBuffer, 26, flowControl_3_flow_send); // flow_control 3's flow send
        S7->NumToByte(WriteBuffer, 38, rpump);  // pump rotation speed send
        //qDebug()<<WriteBuffer[22]<<WriteBuffer[23]<<WriteBuffer[24]<<WriteBuffer[25];
        //信号传输完毕,发送信号

    }
    else
    {
        ui->label_Plc->setText(QString::fromLocal8Bit("未找到plc"));
        ui->label_SystemStateValue->setText(QString::fromLocal8Bit("未知状态"));
        for(int i=0;i<256;i++)
        {
             WriteBuffer[i]=0;
             ReadBuffer[i]=0;
        }
    }
}
//---------------------------------------------------------------------------
void TOC::closeEvent(QCloseEvent* event)
{
    chartrefreshflag=0;
    switch(QMessageBox::information(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("你确定退出该软件?"),QString::fromLocal8Bit("确定"), QString::fromLocal8Bit("取消"),0,1))
    {
    case 0:
        exit(0);
        event->accept();
        break;
    case 1:
    default:
        event->ignore();
        chartrefreshflag=1;
        break;
    }
}
//---------------------------------------------------------------------------
void TOC::sqlControl()
{
    // 将新的数据存入db中
    sql.insert_data(db, ReadBuffer);
    // 取出db的数据用以显示
    QSqlQuery query(db);
    QString sel_tab = QString("SELECT * FROM MetersParaM");
    query.exec(sel_tab);
    query.last();
    toc_inlet_number = query.value(1).toDouble();
    toc_outlet_number = query.value(2).toDouble();
    flowControl_1_flow_rec = query.value(3).toDouble();
    flowControl_1_pressure_rec = query.value(4).toDouble();
    flowControl_2_flow_rec = query.value(5).toDouble();
    flowControl_2_pressure_rec = query.value(6).toDouble();
    flowControl_3_flow_rec = query.value(7).toDouble();
    flowControl_3_pressure_rec = query.value(8).toDouble();
    pump_speed_actual = query.value(9).toDouble();

    query.exec(QString("SELECT TOC1, TOC2 FROM MetersParaM"));
    query.last();
    for(int i = 0 ; i < 1000; i++){
        if(!query.previous()){
            a[i] = 0;
            b[i] = 0;
        }
        else{
            a[i] = query.value(0).toDouble();
            b[i] = query.value(1).toDouble();
        }
    }
    query.exec();
    //取出完毕
}
//---------------------------------------------------------------------------
void TOC::on_btn_Waiting_clicked()
{
    if(connectResult==1)
        WriteBuffer[1]=1;
        status = 1;
}
//---------------------------------------------------------------------------
void TOC::on_btn_Runing_clicked()
{
    if(connectResult==1)
        WriteBuffer[1]=2;
        status =2;
}
//---------------------------------------------------------------------------
void TOC::on_btn_ScreenPrint_clicked()
{
    QPixmap pixmap = QMainWindow::grab();
    pixmap.save(QString("TOC%1.jpg").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss")));
}
//---------------------------------------------------------------------------
void TOC::Chart_View()
{
    //
//    for(int i=0;i<200;i++)
//    {
//        a[i]=rand()%20;
//        b[i]=rand()%16;
//    }
//    for(int i=200;i<1000;i++)
//    {
//        a[i]=a[i%200];
//        b[i]=b[i%200];
//    }

    ui->graphicsView->setChart(chart);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    chart->addSeries(seriesTOCIn);
    chart->addSeries(seriesTOCOut);
    chart->setTitle("TOC");
    //chart->setAnimationOptions(QChart::SeriesAnimations);//设置曲线呈动画显示

    axisX->setRange(0, 100); //设置范围
    axisX->setLabelFormat("%g"); //设置刻度的格式
    axisX->setTitleText("Time(min)"); //设置X轴的标题
    axisX->setGridLineVisible(true); //设置是否显示网格线
    axisX->setMinorTickCount(4); //设置小刻度线的数目
   // axisX->setLabelsVisible(false); //设置刻度是否显示

    axisY->setRange(0, 20);
    axisY->setTitleText("TOC Value(ppb)");
    axisY->setLabelFormat("%.2f");
    axisY->setGridLineVisible(true);

    chart->setAxisX(axisX, seriesTOCIn);
    chart->setAxisY(axisY, seriesTOCIn);
    chart->setAxisX(axisX, seriesTOCOut);
    chart->setAxisY(axisY, seriesTOCOut);
    chart->legend()->hide();

}
//---------------------------------------------------------------------------
void TOC::Chartrefresh()
{
    //
    PointTocIn.clear();
    PointTocOut.clear();
   // seriesTOCIn->clear();

    if(ui->cb_TOCIn->checkState())
    {
        for(int x=0;x<100;x++)
        {
            PointTocIn.append(QPointF(x,a[systemtime+x]));
        }
        seriesTOCIn->setPen(QPen(Qt::red,3,Qt::SolidLine));

        //chart->addSeries(seriesTOCIn);
    }
    else
    {
        PointTocIn.append(QPointF(0,0));
        seriesTOCIn->setPen(QPen(Qt::transparent,3,Qt::SolidLine));
    }
    if(ui->cb_TOCOut->checkState())
    {
        for(int x=0;x<100;x++)
        {
            PointTocOut.append(QPointF(x,b[systemtime+x]));
        }
        seriesTOCOut->setPen(QPen(Qt::green,3,Qt::SolidLine));
        //chart->addSeries(seriesTOCOut);
    }
    else
    {
        PointTocOut.append(QPointF(0,0));
        seriesTOCOut->setPen(QPen(Qt::transparent,3,Qt::SolidLine));
    }

    seriesTOCIn->replace(PointTocIn);
//    seriesTOCOut->clear();
//    seriesTOCOut->append(PointTocOut);
    seriesTOCOut->replace(PointTocOut);
//    seriesTOCIn->clear();
//    seriesTOCIn->append(PointTocIn);

//    chart->removeSeries(seriesTOCIn);
//    chart->removeSeries(seriesTOCOut);


    chart->setTitle("TOC");
//    chart->setAnimationOptions(QChart::SeriesAnimations);//设置曲线呈动画显示

//    axisX->setRange(0, 10); //设置范围
//    axisX->setLabelFormat("%g"); //设置刻度的格式
//    axisX->setTitleText("Time(min)"); //设置X轴的标题
//    axisX->setGridLineVisible(true); //设置是否显示网格线
//    axisX->setMinorTickCount(4); //设置小刻度线的数目
   // axisX->setLabelsVisible(false); //设置刻度是否显示

//    axisY->setRange(0, 20);
//    axisY->setTitleText("TOC Value(ppb)");
//    axisY->setLabelFormat("%.2f");
//    axisY->setGridLineVisible(true);

//    chart->setAxisX(axisX, seriesTOCIn);
//    chart->setAxisY(axisY, seriesTOCIn);
//    chart->setAxisX(axisX, seriesTOCOut);
//    chart->setAxisY(axisY, seriesTOCOut);
//    chart->legend()->hide();
//    ui->graphicsView->setChart(chart);
//    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
}
//---------------------------------------------------------------------------
void TOC::on_btn_dataExpert_clicked()
{
    sqlExport->exportData(db,"MetersParaM","HandleSave",0);
}
//---------------------------------------------------------------------------
void TOC::initAlgorithminit()
{
    ui->rb_handle->setChecked(1);
    ui->groupBox_NeuralNetWork->setEnabled(0);
    ui->edt_beita->setText("1");
    ui->edt_back_flow->setText("16");
    ui->edt_inlet_flow->setText("2.5");
    ui->edt_rpump->setText("3000");
}
//---------------------------------------------------------------------------
void TOC::AlgorithmControl()
{
    // Empty Now
    // ui->edt_beita->text().toDouble()  1/2
    // ui->edt_back_flow->text().toDouble() x+y
    // ui->edt_inlet_flow->text().toDouble() z
    flowControl_1_flow_send = ui->edt_back_flow->text().toDouble() / (ui->edt_beita->text().toDouble() + 1);
    flowControl_2_flow_send = ui->edt_back_flow->text().toDouble() - flowControl_1_flow_send;
    flowControl_3_flow_send = ui->edt_inlet_flow->text().toDouble();
    rpump = ui->edt_rpump->text().toDouble();
}
//---------------------------------------------------------------------------
void TOC::on_rb_NeuralNetwork_clicked()
{
    ui->groupBox_NeuralNetWork->setEnabled(1);
    ui->rb_BackPropagation->setCheckable(1);
    ui->rb_convolution->setCheckable(1);
    ui->rb_BackPropagation->setChecked(1);
    //
    ui->edt_beita->setEnabled(0);
    ui->edt_back_flow->setEnabled(0);
    ui->edt_inlet_flow->setEnabled(0);
    //
}
//---------------------------------------------------------------------------
void TOC::on_rb_handle_clicked()
{
    ui->rb_BackPropagation->setCheckable(0);
    ui->rb_convolution->setCheckable(0);
    ui->groupBox_NeuralNetWork->setEnabled(0);
    //
    ui->edt_beita->setEnabled(1);
    ui->edt_back_flow->setEnabled(1);
    ui->edt_inlet_flow->setEnabled(1);
}

//---------------------------------------------------------------------------
void TOC::on_rb_BackPropagation_clicked()
{

}
//---------------------------------------------------------------------------
void TOC::on_rb_convolution_clicked()
{

}

//---------------------------------------------------------------------------
void TOC::on_btn_setting_clicked()
{

}
//---------------------------------------------------------------------------
void TOC::OtherThing()
{
    ui->btn_background1->setVisible(backgroundFlag);
    ui->btn_background2->setVisible(!backgroundFlag);
    backgroundFlag=!backgroundFlag;
}
//---------------------------------------------------------------------------

void TOC::on_btn_YAxisSetting_clicked()
{
    YAxiaSetting* settingYAxia = new YAxiaSetting();
    settingYAxia->show();
}
//---------------------------------------------------------------------------
void TOC::on_btn_XAxisSetting_clicked()
{
    XAxiaSetting* settingXAxia = new XAxiaSetting();
    settingXAxia->show();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


