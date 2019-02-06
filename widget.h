#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCharts>
#include "customthread.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void addData(QPointF xy);
    void Stop();

private:
    Ui::Widget *ui;
    CustomThread m_worker;
    QScatterSeries *m_series;
};

#endif // WIDGET_H
