#include "statisticsview.h"
#include "ui_statisticsview.h"

StatisticsView::StatisticsView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StatisticsView),
    chart(nullptr),
    activeA(nullptr),
    altitudesA(nullptr),
    altitudedeltaA(nullptr),
    distancesA(nullptr),
    distancedeltaA(nullptr),
    flightA(nullptr),
    speedsA(nullptr),
    activeB(nullptr),
    altitudesB(nullptr),
    distancesB(nullptr),
    intervalB(nullptr),
    speedsB(nullptr)
{
    ui->setupUi(this);
}

StatisticsView::~StatisticsView()
{
    //due to deletion of this chartview is deleted and deletes chart which deletes its series
    delete ui;
}

void StatisticsView::compareModeToggled(bool checked)
{
    if(checked){

    }
    else{

    }
}



void StatisticsView::change_shown_Adata(const QString &dataname)
{
    if(dataname == "Altitude"){
        requestAltitudeA();
    }
    else if(dataname == "Altitude Delta"){
        requestAltitudeDeltaA();
    }
    else if(dataname == "Distance"){
        requestDistanceA();
    }
    else if(dataname == "Distance Delta"){
        requestDistanceDeltaA();
    }
    else if(dataname == "Flight"){
        requestFlightA();
    }
    else if(dataname == "Speed"){
        requestSpeedA();
    }
    else{
        //TODO invalid data requested
    }
}

void StatisticsView::change_shown_Bdata(const QString &dataname)
{
    if(dataname == "Altitude"){
        requestAltitudeB();
    }
    else if(dataname == "Distance"){
        requestDistanceB();
    }
    else if(dataname == "Interval"){
        requestIntervalB();
    }
    else if(dataname == "Speed"){
        requestSpeedB();
    }
    else{
        //TODO invalid data requested
    }
}

void StatisticsView::requestAltitudeA()
{
    deactivate(activeA);
    if(!altitudesA){
        emit requestAltitudeData();
        altitudesA = activeA;
    }
    else{
        plotData(altitudesA);
    }
}

void StatisticsView::requestAltitudeDeltaA()
{
    deactivate(activeA);
    if(!altitudedeltaA){
        emit requestAltitudeDelta();
        altitudedeltaA = activeA;
    }
    else{
        plotData(altitudedeltaA);
    }
}

void StatisticsView::requestAltitudeB()
{
    deactivate(activeB);
    if(!altitudesB){
        emit requestAltitudeData();
        altitudesB = activeB;
    }
    else{
        plotData(altitudesB);
    }
}

void StatisticsView::requestDistanceA()
{
    deactivate(activeA);
    if(!distancesA){
        emit requestDistanceData();
        distancesA = activeA;
    }
    else{
        plotData(distancesA);
    }
}

void StatisticsView::requestDistanceDeltaA()
{
    deactivate(activeA);
    if(!distancedeltaA){
        emit requestDistanceDelta();
        distancedeltaA = activeA;
    }
    else{
        plotData(distancedeltaA);
    }
}

void StatisticsView::requestDistanceB()
{
    deactivate(activeB);
    if(!distancesB){
        emit requestDistanceData();
        distancesB = activeB;
    }
    else{
        plotData(distancesB);
    }
}

void StatisticsView::requestIntervalB()
{
    deactivate(activeB);
    if(!intervalB){
        emit requestIntervalData();
        intervalB = activeB;
    }
    else{
        plotData(intervalB);
    }
}

void StatisticsView::requestFlightA()
{
    deactivate(activeA);
    if(!flightA){
        emit requestFlightData();
        flightA = activeA;
    }
    else{
        plotData(flightA);
    }
}

/*UI wants the speed data of the A set
    make whatever is active from the A set invisible
    if speed data is available, plot it
    if speed data is not available, request it, which makes
        it be plotted and then set the speeds pointer to
        point at what active points at
*/
void StatisticsView::requestSpeedA()
{
    deactivate(activeA);
    if(!speedsA){
        emit requestSpeedData();
        speedsA = activeA;
    }
    else{
        plotData(speedsA);
    }
}

void StatisticsView::requestSpeedB()
{
    deactivate(activeB);
    if(!speedsB){
        emit requestSpeedData();
        speedsB = activeB;
    }
    else{
        plotData(speedsB);
    }
}

void StatisticsView::deactivate(QLineSeries * & active)
{
    if(active)
    {
        active->setVisible(false);
        active = nullptr;
    }
}

/*Hand control over the memory of the Line Series to the chart,
 * connect a signal in order to notice when the Series get deleted*/
/*The first series added to the chart will belong to the A set*/
void StatisticsView::plotData(QLineSeries * data)
{
    /*Set the chart pointer if necessary*/
    if(!chart)
    {
        chart = ui->chartview->chart();
        QValueAxis *X = new QValueAxis();
        QValueAxis *Y = new QValueAxis();
        chart->addAxis(X, Qt::AlignBottom);
        chart->addAxis(Y, Qt::AlignLeft);
        chart->addAxis(new QValueAxis(), Qt::AlignRight);
    }
    /*Check whether data is in chart already
        - if yes, make it visible
        - if not, add it*/
    QList<QAbstractSeries *> s = chart->series();
    if(s.contains(data)){
        s.at(s.indexOf(data))->setVisible(true);
    }
    else{
        connect(data, SIGNAL(destroyed(QObject*)), this, SLOT(seriesDestroyed(QObject*)) );
        chart->addSeries(data);
        data->attachAxis(chart->axisX());
        data->attachAxis(chart->axisY());
    }
    data->setPointsVisible(true);
    /*Check which set to add the data to and edit color and legend of data*/
    if(!activeA){
        activeA = data;
    }
    else{
        activeB = data;
    }
    /*resize the axes in order to fit data
        - calculate average
        - resize*/
    double average = 0;
    for(int i = 0; i<data->count(); i++)    //TODO this assumes the series have first point at index 0
    {
        average += data->at(i).y();
    }
    average /= data->count();         //TODO same assumption here

    //chart->axisY(data)->setMax(2*average);

    /*little playground*/
    QPoint p;
    int rx = p.rx();
    int & rrx = p.rx();
    rx = 3;     //should not set x in p
    rrx = 5;    //should set x in p
    int  x = p.x();
}

void StatisticsView::seriesDestroyed(QObject * obj)
{
    /*check if it was in A set*/
    if(obj == activeA){
        activeA = nullptr;
    }
    if(obj == altitudesA){
        altitudesA = nullptr;
    }
    else if(obj == altitudedeltaA){
        altitudedeltaA = nullptr;
    }
    else if(obj == distancesA){
        distancesA = nullptr;
    }
    else if(obj == distancedeltaA){
        distancedeltaA = nullptr;
    }
    else if(obj == flightA){
        flightA = nullptr;
    }
    else if(obj == speedsA){
        speedsA = nullptr;
    }

    else if(obj == activeA){
        activeA = nullptr;
    }
    if(obj == altitudesB){
        altitudesB = nullptr;
    }
    else if(obj == distancesB){
        distancesB = nullptr;
    }
    else if(obj == intervalB){
        intervalB = nullptr;
    }
    else if(obj == speedsB){
        speedsB = nullptr;
    }

    else{
        //TODO unknown object about to be deleted
    }
}
