#include "statisticsview.h"
#include "ui_statisticsview.h"

StatisticsView::StatisticsView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StatisticsView),
    Yleft(new QValueAxis()),
    Yright(new QValueAxis()),
    activedata(nullptr),
    comparedata(nullptr)
{
    /*Define the names of data*/
    datanames << "Altitude" << "Altitude Delta" << "Distance" << "Distance Delta" << "Flight" << "Interval" << "Speed";

    /*set up the UI*/
    ui->setupUi(this);

    /*set up the chart's axes*/
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
     activedata->setName(dataname);
    ui->chartview->chart()->axisY(activedata)->setTitleText(activedata->name());
}

void StatisticsView::change_shown_Bdata(const QString &dataname)
{
    deactivate(comparedata);
    change_shown_data(dataname);
    comparedata->setName(dataname);
    ui->chartview->chart()->axisY(comparedata)->setTitleText(comparedata->name());
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
            bool available = false;
            foreach(QAbstractSeries *s, loadedseries)
            {
                if(s->name() == dataname)
                {
                    s->setVisible(true);
                    available = true;
                    break;
                }
            }
            if(!available){
                emit requestData(static_cast<DataType>(i));
            }
        }
    }
}

/*Hand control over the memory of the Line Series to the chart,
 * connect a signal in order to notice when the Series get deleted*/
/*The first series added to the chart will belong to the A set*/
void StatisticsView::plotData(const QList<QPointF> & data)
{
    QChart *chart = ui->chartview->chart();
    QLineSeries *series = new QLineSeries();

    /*Add data to chart*/
    *series << data;
    chart->addSeries(series);
    series->attachAxis(chart->axisX());

    //data->setPointsVisible(true);
    /*resize the axes in order to fit data
        - calculate maximum and minimum on X and Y axes
        - resize*/
    double Xmin = 0;
    double Xmax = 0;
    double Ymin = 0;
    double Ymax = 0;
    for(int i = 0; i<series->count(); i++)
    {
        const QPointF & p = series->at(i);
        if(p.x() < Xmin){
            Xmin = p.x();
        }else if(p.x() > Xmax){
            Xmax = p.x();
        }
        if(p.y() < Ymin){
            Ymin = p.y();
        }else if(p.y() > Ymax){
            Ymax = p.y();
        }
    }

    /*Check which set to add the data to and edit color and legend of data*/
    if(!activedata){
        activedata = series;
        series->attachAxis(Yleft);
        Yleft->setRange(Ymin, Ymax);
    }
    else{
        comparedata = series;
        series->attachAxis(Yright);
        Yright->setRange(Ymin, Ymax);
    }
}

void StatisticsView::deactivate(QLineSeries * & active)
{
    if(active){
        active->setVisible(false);
        active = nullptr;
    }
}