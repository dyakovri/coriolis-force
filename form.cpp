#include "form.h"
#include "ui_form.h"
#include <cmath>

using namespace QtCharts;

form::form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::form)
{
    ui->setupUi(this);
    timerInterval = 10;
    timer = new QTimer(this); // Создаем объект класса QTimer и передаем адрес переменной
    timer->setInterval(timerInterval); // Задаем интервал таймера
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGraph())); // Подключаем сигнал таймера к нашему слоту
    ui->graphicsView_1->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView_2->setRenderHint(QPainter::Antialiasing);

    seriesYX = new QLineSeries();
    seriesZXY = new QLineSeries();
    coreXY = new QScatterSeries();
    coreZXY = new QScatterSeries();
    coreXY->setMarkerSize(10);
    coreZXY->setMarkerSize(10);

    chart1 = new QChart();
    chart1->addSeries(seriesYX);
    chart1->addSeries(coreXY);
    chart1->createDefaultAxes();
    auto xAxis = chart1->axes(Qt::Horizontal);
    auto yAxis = chart1->axes(Qt::Vertical);
    xAxis[0]->setRange(-25, 25);
    yAxis[0]->setRange(-25, 25);
    chart1->legend()->hide();

    chart2 = new QChart();
    chart2->addSeries(seriesZXY);
    chart2->addSeries(coreZXY);
    chart2->createDefaultAxes();
    auto zAxis = chart2->axes(Qt::Vertical);
    auto xyAxis = chart2->axes(Qt::Horizontal);
    zAxis[0]->setRange(0, 25);
    xyAxis[0]->setRange(0, 25);
    chart2->legend()->hide();

    ui->graphicsView_1->setChart(chart1);
    ui->graphicsView_2->setChart(chart2);
    ui->start->setDisabled(false);
    ui->stop->setDisabled(true);

    coreXY->clear();
    coreXY->append(0,0);
    coreZXY->clear();
    coreZXY->append(sqrt(pow(0,2)+pow(0,2)),0);
    ui->graphicsView_1->update();
    ui->graphicsView_2->update();
}

void form::updateGraph() {
    totalTime = ball->Step(timerInterval);

    QTextStream writeStreqam(fileOut);
    writeStreqam << ball->get_x() <<";"<<ball->get_y()<<";"<<ball->get_z()<<"\n";

    ui->x_axis->setValue(abs(ball->get_x()) + 5);
    ui->x_axis->editingFinished();
    ui->y_axis->setValue(abs(ball->get_y()) + 5);
    ui->y_axis->editingFinished();
    ui->z_axis->setValue(abs(ball->get_z()) + 5);
    ui->z_axis->editingFinished();
    ui->xy_axis->setValue(abs(ball->get_path()) + 5);
    ui->xy_axis->editingFinished();

    if(ball->get_z()>=0){
        seriesYX->append(ball->get_x(), ball->get_y());
        coreXY->clear();
        coreXY->append(ball->get_x(),ball->get_y());
        ui->graphicsView_1->update();

        seriesZXY->append(ball->get_path(),ball->get_z());
        coreZXY->clear();
        coreZXY->append(ball->get_path(),ball->get_z());
        ui->graphicsView_2->update();
        updateStatus();
    }
    else{
        seriesYX->append(ball->get_x(), ball->get_y());
        coreXY->clear();
        coreXY->append(ball->get_x(),ball->get_y());
        ui->graphicsView_1->update();

        seriesZXY->append(ball->get_path(),0);
        coreZXY->clear();
        coreZXY->append(ball->get_path(),0);
        ui->graphicsView_2->update();
        updateStatus();
        ui->stop->click();
        ui->start->setDisabled(true);
    }
}
void form::updateStatus() {
    ui->status->setText("Время со старта : " + QString::number(totalTime) + " Координаты: " + QString::number(ball->get_x(), 'd', 0) + ", " + QString::number(ball->get_y(), 'd', 0) +", " + QString::number(ball->get_z(), 'd', 0) /*+ " Скорость: " + QString::number(doubleStarSatelite->get_V(), 'd', 2) + " км/с"*/);
}

form::~form()
{
    delete ui;
    delete timer;
    delete chart1;
    delete chart2;
    delete seriesYX;
    delete seriesZXY;
    delete coreXY;
    delete coreZXY;
}

void form::on_start_clicked()
{
    if (!ui->stop->isEnabled()) {
        ball = new Cannonball(ui->M->value(),ui->S->value(),0,0,0,ui->V->value(),ui->alpha->value(),ui->beta->value(),ui->p->value(),ui->w->value(), ui->latitude->value());
        fileOut->open(QIODevice::WriteOnly | QIODevice::Text);
    }
    setDisabledSplinBoxes(true);
    timer->start(); // Запускаем таймер
    ui->start->setDisabled(true);
    ui->stop->setText("Пауза");
    ui->stop->setDisabled(false);
}

void form::on_stop_clicked()
{
    if (timer->isActive()) {
        timer->stop();
        ui->start->setDisabled(false);
        ui->stop->setText("Стоп");
    }
    else {
        totalTime = 0;
        ui->stop->setDisabled(true);
        ui->start->setDisabled(false);
        ui->stop->setText("Пауза");
        setDisabledSplinBoxes(false);
        seriesYX->clear();
        seriesZXY->clear();
        ui->status->setText("Остановлено");
        fileOut->close();
        delete ball;
    }
}

void form::setDisabledSplinBoxes(bool value) {
    ui->S->setDisabled(value);
    ui->M->setDisabled(value);
    ui->V->setDisabled(value);
    ui->alpha->setDisabled(value);
    ui->beta->setDisabled(value);
    ui->p->setDisabled(value);
    ui->w->setDisabled(value);
    ui->latitude->setDisabled(value);
}

void form::on_x_axis_editingFinished()
{
    auto xAxis = chart1->axes(Qt::Horizontal);
    xAxis[0]->setRange(-ui->x_axis->value(), ui->x_axis->value());
}

void form::on_y_axis_editingFinished()
{
    auto yAxis = chart1->axes(Qt::Vertical);
    yAxis[0]->setRange(-ui->y_axis->value(), ui->y_axis->value());
}

void form::on_z_axis_editingFinished()
{
    auto zAxis = chart2->axes(Qt::Vertical);
    zAxis[0]->setRange(0, ui->z_axis->value());
}

void form::on_xy_axis_editingFinished()
{
    auto xyAxis = chart2->axes(Qt::Horizontal);
    xyAxis[0]->setRange(0, ui->xy_axis->value());
}
