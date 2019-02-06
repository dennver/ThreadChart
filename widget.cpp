#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(&m_worker, SIGNAL(sendData(QPointF)), this, SLOT(addData(QPointF)));

    // state machine
    QStateMachine *machine = new QStateMachine(this);
    machine->setGlobalRestorePolicy(QState::DontRestoreProperties);

    QState *stateWorking = new QState();
    stateWorking->assignProperty(ui->stopButton, "enabled", QVariant("true"));
    stateWorking->assignProperty(ui->pauseButton, "enabled", QVariant("true"));

    QState *stateStopped = new QState();
    stateStopped->assignProperty(ui->stopButton, "enabled", QVariant("false"));
    stateStopped->assignProperty(ui->pauseButton, "enabled", QVariant("false"));

    QState *statePaused = new QState();
    statePaused->assignProperty(ui->pauseButton, "enabled", QVariant("false"));

    connect(stateWorking, &QState::entered, &m_worker, &CustomThread::processData);
    connect(stateStopped, &QState::entered, this, &Widget::Stop);
    connect(statePaused, &QState::entered, &m_worker, &CustomThread::pauseProcess);

    stateStopped->addTransition(ui->startButton, &QPushButton::clicked, stateWorking);
    stateWorking->addTransition(ui->stopButton, &QPushButton::clicked, stateStopped);
    stateWorking->addTransition(ui->pauseButton, &QPushButton::clicked, statePaused);
    statePaused->addTransition(ui->startButton, &QPushButton::clicked, stateWorking);
    statePaused->addTransition(ui->stopButton, &QPushButton::clicked, stateStopped);

    machine->addState(stateWorking);
    machine->addState(stateStopped);
    machine->addState(statePaused);
    machine->setInitialState(stateStopped);
    machine->start();
    // state machine

    // chart
    QChart *chart = new QChart();
    ui->chartView->setChart(chart); // takes ownership

    m_series = new QScatterSeries();
    m_series->setName("Random array");
    m_series->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    m_series->setMarkerSize(7.0);

    chart->addSeries(m_series); // takes ownership

    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();
    axisX->setRange(0, 50000);
    axisY->setRange(0, 50000);
    axisX->setGridLineVisible(true);
    axisY->setGridLineVisible(true);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    m_series->attachAxis(axisX);
    m_series->attachAxis(axisY);
    // chart
}

Widget::~Widget()
{
    delete ui;
    m_worker.quit();
    m_worker.wait();
}

void Widget::addData(QPointF xy)
{
    *m_series << xy;
}

void Widget::Stop()
{
    m_series->clear();
    m_worker.stopProcess();
}
