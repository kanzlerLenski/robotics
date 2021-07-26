#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include <QDateTime>
#include "qcustomplot.h"
#include "blocks/gain/gain.h"
#include "blocks/integrator/integrator.h"
#include "blocks/continuous/continuos.h"
#include "blocks/plant/plant.h"
#include "blocks/discrete_plant/discrete_plant.h"
#include "blocks/discrete_continuous/discrete_continuous.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void makePlot();

private:
    Ui::Widget *ui;
    QGridLayout *mainlayout;
    QCustomPlot *inputPlot;
    QCustomPlot *outputPlot;
	QPushButton* startButton;
	QPushButton* saveButton;

	qint64 startTime = 0;
	qint64 relativeTime = 0;
	qint64 dt;

    QTimer * timer;

    // --------------------------
    // Add pointer to the object here
    // --------------------------
    gain *object;   // <=
    // --------------------------
    // Add pointer to the object here
    // --------------------------
    integrator *object2;
    continuos *object3;
    plant *object4;
    discrete_continuous *object5;
    discrete_plant *object6;
};

#endif // WIDGET_H
