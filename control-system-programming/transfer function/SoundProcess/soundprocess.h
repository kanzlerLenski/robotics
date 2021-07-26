#ifndef SOUNDPROCESS_H
#define SOUNDPROCESS_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QTextStream>

#include <QAudioFormat>
#include <QAudioEncoderSettings>
#include <QAudioDecoder>
#include <QAudioRecorder>
#include <QAudioOutput>
#include <QAudioDeviceInfo>

#include "qcustomplot/qcustomplot.h"
#include "Filters/AbstractFilter/abstractfilter.h"

#include "Filters/ReduceFilter/reducefilter.h"

#include "Filters/LowPassFilter/lowpassfilter.h"


QT_BEGIN_NAMESPACE
namespace Ui { class SoundProcess; }
QT_END_NAMESPACE

class SoundProcess : public QMainWindow
{
    Q_OBJECT

public:
    SoundProcess(QWidget *parent = nullptr);
    ~SoundProcess();

signals:
	void percentageComplete(int);

private slots:
    void on_pushButtonOpenFile_clicked();

    void processSoundFile(QString const& filePath);
    void readBuffer();
    void stopLoading();
    void processFile();
    void plotRaw();
    void plotProcessed();
	static QByteArray getWavWithHeader(QByteArray const& array, quint32 sampleRate);
    void saveToWav();

	QByteArray generateArray(QVector<float> const& leftChannel, QVector<float>const& rightChannel) const;
	void saveToFile(QVector<float> const& leftChannel, QVector<float>const& rightChannel, QString  const& filename);

    void on_pushButtonSaveInitAsText_clicked();
    void on_pushButtonProcessFile_clicked();

	void on_pushButtonSaveResultAsFile_clicked();

	void on_pushButtonSaveResultAsText_clicked();

private:

    void initPlots();

    Ui::SoundProcess *ui;

    QAudioDecoder* _decoder;
    QAudioRecorder* _recorder;

    QVector<qint16> _inputArray;

    QVector<float> _leftArray;
    QVector<float> _rightArray;

    QVector<float> _leftArrayProcessed;
    QVector<float> _rightArrayProcessed;

    QCustomPlot* _plotLeftChannel;
    QCustomPlot* _plotRightChannel;

    QCustomPlot* _plotLeftChannelProcessed;
    QCustomPlot* _plotRightChannelProcessed;

    QVector<double> time;
    QVector<double> leftChannel;
    QVector<double> rightChannel;
    QVector<double> leftChannelProcessed;
    QVector<double> rightChannelProcessed;

    ReduceFilter* _reduceFilterLeft;
    ReduceFilter* _reduceFilterRight;

    LowPassFilter* _LowPassFilterLeft;
    LowPassFilter* _LowPassFilterRight;

	QVector<AbstractFilter*> _vectorFilters;
};
#endif // SOUNDPROCESS_H
