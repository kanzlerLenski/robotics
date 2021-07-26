#include "widget.h"
#include "ui_widget.h"
#include <iostream>
#include <math.h>

// --------------------------
// Set stop time here
// --------------------------
#define ENDOFTIME 50
#define SAMPLINGTIMEMSEC 10
// --------------------------
// Set stop time here
// --------------------------

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

	// Create dir for logs
	if (!QDir("logs").exists()) {
		QDir().mkdir("logs");
	}


    // Set window size
	this->setMinimumSize(640, 480);

    // Add main layout with two plots
    mainlayout = new QGridLayout(this);
    inputPlot = new QCustomPlot(this);
    outputPlot = new QCustomPlot(this);
	mainlayout->addWidget(inputPlot, 0, 0);
	mainlayout->addWidget(outputPlot, 0, 1);
	inputPlot->addGraph();
	outputPlot->addGraph();

	startButton = new QPushButton("Start");
	connect(startButton, &QPushButton::clicked, this, [=](){
		startTime = 0;
		relativeTime = 0;
		inputPlot->graph(0)->data()->clear();
		outputPlot->graph(0)->data()->clear();
		timer->start();
	});
	mainlayout->addWidget(startButton, 1, 0);

	saveButton = new QPushButton("Save");
	connect(saveButton, &QPushButton::clicked, this, [=](){
		QString timestamp = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss");
		inputPlot->savePng(QString("./logs/%1_input.png").arg(timestamp));
		outputPlot->savePng(QString("./logs/%1_output.png").arg(timestamp));
	});
	mainlayout->addWidget(saveButton, 1, 1);

    // Give the axes some labels:
    inputPlot->xAxis->setLabel("t");
    inputPlot->yAxis->setLabel("input");
    outputPlot->xAxis->setLabel("t");
    outputPlot->yAxis->setLabel("output");

    // --------------------------
    // Change ranges if you need
    // --------------------------
    // Set axes ranges so see all data:
    inputPlot->xAxis->setRange(0, ENDOFTIME);
    inputPlot->yAxis->setRange(-3, 3);
    outputPlot->xAxis->setRange(0, ENDOFTIME);
    outputPlot->yAxis->setRange(-3, 3);

    // --------------------------
    // Create the object here
    // --------------------------
    object = new gain(2.0);   // <=
    // --------------------------
    // Create the object here
    // --------------------------
    //object2 = new integrator(0.0);
    object3 = new continuos(cos(M_PI/6), 2*sin(M_PI/6), -0.25);
    object4 = new plant(0, 1, 0, 0, 0, 1, -1, -2, -2, 0, 0, 1, 0.5, 0, 0, 1, 0, 0, 0);

    // discrete_plant continuous
    //object5 = new discrete_continuous(0.995, 0.2, -0.05, 0.995, 0, 0, 0, 0, 1, cos(M_PI/6), 2*sin(M_PI/6)); // 1/5
    //object5 = new discrete_continuous(1, 0.02, -0.005, 0.999, 0, 0, 0, 1, cos(M_PI/6), 2*sin(M_PI/6), 0); // 1/50
    object5 = new discrete_continuous(1, 0.01, -0.002, 0.999, 0, 0, 0, 1, cos(M_PI/6), 2*sin(M_PI/6), 0); // 1/100

    // discrete_plant plant
    //object6 = new discrete_plant(0.999, 0.198, 0.017, -0.017, 0.964, 0.163, -0.163, -0.343, 0.639, 0.001, 0.017, 0.163, 0.5, 0, 0, 1, 0, 0, 0); // 1/5
    //object6 = new discrete_plant(1, 0.02, 0, 0, 1, 0.02, -0.02, -0.039, 0.96, 0, 0, 0.02, 0.5, 0, 0, 1, 0, 0, 0); // 1/50
    object6 = new discrete_plant(1, 0.01, 0, 0, 1, 0.01, -0.01, -0.02, 0.98, 0, 0, 0.01, 0.5, 0, 0, 1, 0, 0, 0); // 1/100

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(makePlot()));

    // --------------------------
    // Set sampling time here
    // --------------------------
	timer->setInterval(SAMPLINGTIMEMSEC);
    // --------------------------
    // Set sampling time here
    // --------------------------
}

Widget::~Widget()
{
    delete ui;
    delete inputPlot;
    delete outputPlot;
    delete timer;
    delete mainlayout;

    // --------------------------
    // Delete the object here
    // --------------------------
    delete object;
    // --------------------------
    // Delete the object here
    // --------------------------
}

void Widget::makePlot() {

	// --------------------------
	// Replace input signal with ours
	// --------------------------
    double signal = 2 * std::sin(relativeTime / 2000.0 + M_PI/6); // 2sin(t/2+pi/6)
	// --------------------------
	// Replace input signal with ours
	// --------------------------

	// --------------------------
	// Plot initial values here
	// --------------------------
	if (startTime == 0) {
		// First step only plot initial values
		startTime = QDateTime::currentMSecsSinceEpoch();

		inputPlot->graph(0)->addData(0.0, 0);
		outputPlot->graph(0)->addData(0.0, 0);
		return;
	} else {
		dt = relativeTime;
		relativeTime = QDateTime::currentMSecsSinceEpoch() - startTime;
		dt = relativeTime - dt;
	}
;
    // --------------------------
    // Update the object here
    // --------------------------
	object->update(signal);
    // --------------------------
    // Update the object here
    // --------------------------
    //object2->update(signal, dt/1000.0);
    //object2->update(object2->getState(), dt/1000.0);

    // first: contituous - plant
    /*
    object3->update(dt/1000.0);
    object4->update(object3->getState(), dt/1000.0);
    inputPlot->graph(0)->addData(relativeTime / 1000.0, object3->getState());
    outputPlot->graph(0)->addData(relativeTime / 1000.0, object4->getState());
    */

    // second:
    // continuous - discrete_continuous
    object3->update(dt/1000.0);
    object5->update(object3->getState());

    // continuous - plant - discrete_plant
    object4->update(object3->getState(), dt/1000.0);
    object6->update(object4->getState());

    inputPlot->graph(0)->addData(relativeTime / 1000.0, object5->getState());
    outputPlot->graph(0)->addData(relativeTime / 1000.0, object6->getState());

    inputPlot->replot();
    outputPlot->replot();

	if (relativeTime / 1000.0 > ENDOFTIME) {
		timer->blockSignals(true);
		timer->stop();
		timer->blockSignals(false);
	}
}
