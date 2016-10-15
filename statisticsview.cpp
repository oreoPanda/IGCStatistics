#include "statisticsview.h"
#include "ui_statisticsview.h"

StatisticsView::StatisticsView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StatisticsView),
    Yleft(nullptr),
    Yright(nullptr),
    activedata(nullptr),
    comparedata(nullptr)
{
    /*Define the names of data*/
    datanames << "Altitude";
    datanames << "Altitude Delta";
    datanames << "Distance";
    datanames << "Distance Delta";
    datanames << "Flight";
    datanames << "Interval";
    datanames << "Speed";

    /*set up the UI*/
    ui->setupUi(this);

    /*set up the chart's axes*/
    Yleft = new QValueAxis();
    Yright = new QValueAxis();
    ui->chartview->chart()->addAxis(new QValueAxis(), Qt::AlignBottom);
    ui->chartview->chart()->addAxis(Yleft, Qt::AlignLeft);
    ui->chartview->chart()->addAxis(Yright, Qt::AlignRight);
}

StatisticsView::~StatisticsView()
{
    //due to deletion of this chartview is deleted and deletes chart which deletes its series and its axes
    delete ui;
}

void StatisticsView::compareModeToggled(bool checked)
{
    if(checked){

    }
    else{
        if(this->comparedata){
            this->comparedata->setVisible(false);
        }
    }
}

void StatisticsView::change_shown_Adata(const QString &dataname)
{
    deactivate(activedata);
    change_shown_data(dataname);
}

void StatisticsView::change_shown_Bdata(const QString &dataname)
{
    deactivate(comparedata);
    change_shown_data(dataname);
}

/*request the data that belongs to the given name*/
void StatisticsView::change_shown_data(const QString & dataname)
{
    QList<QAbstractSeries *> loadedseries = ui->chartview->chart()->series();
    /*check which type of data the request is asking for*/
    for(int i = 0; i < DTCOUNT; i++)
    {
        if(dataname == this->datanames.at(i)){
            /*check if the requested data is already available*/
            foreach(QAbstractSeries *s, loadedseries)
            {
                if(s->name() == dataname)
                {
                    s->setVisible(true);
                }
                else emit requestData(static_cast<DataType>(i));
            }
        }
    }
}

/*Hand control over the memory of the Line Series to the chart,
 * connect a signal in order to notice when the Series get deleted*/
/*The first series added to the chart will belong to the A set*/
void StatisticsView::plotData(QLineSeries * data)
{
    QChart *chart = ui->chartview->chart();

    /*Add data to chart*/
    chart->addSeries(data);
    data->attachAxis(chart->axisX());

    //data->setPointsVisible(true);

    /*Check which set to add the data to and edit color and legend of data*/
    if(!activedata){
        activedata = data;
        data->attachAxis(Yleft);
    }
    else{
        comparedata = data;
        data->attachAxis(Yright);
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

void StatisticsView::deactivate(QLineSeries * & active)
{
    if(active){
        active->setVisible(false);
        active = nullptr;
    }
}
