#include "soundprocess.h"
#include "ui_soundprocess.h"

SoundProcess::SoundProcess(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SoundProcess)
{
    ui->setupUi(this);

    _decoder = new QAudioDecoder(this);
    _recorder = new QAudioRecorder(this);

	///
	/// Create filters here
	///
    ///
    ///
	_reduceFilterLeft = new ReduceFilter();
	_reduceFilterRight = new ReduceFilter();

    //_vectorFilters.push_back(_reduceFilterLeft);
    //_vectorFilters.push_back(_reduceFilterRight);

    _LowPassFilterLeft = new LowPassFilter(0.72863573103722778512292279629037,0.0024879331648716492171591330162528,109.07216994182063399787489996324,0);
    _LowPassFilterRight = new LowPassFilter(0.72863573103722778512292279629037,0.0024879331648716492171591330162528,109.07216994182063399787489996324,0);

    _vectorFilters.push_back(_LowPassFilterLeft);
    _vectorFilters.push_back(_LowPassFilterRight);

    initPlots();
}

SoundProcess::~SoundProcess()
{
    delete ui;
}


void SoundProcess::on_pushButtonOpenFile_clicked()
{
    QString FileName = QFileDialog::getOpenFileName(this,
        tr("Open file"), QDir::homePath(), tr("wave file (*.wav)"));

    if (!FileName.isEmpty()) {
        processSoundFile(FileName);
    }
}

void SoundProcess::processSoundFile(const QString &filePath)
{

    _inputArray.clear();
    _leftArray.clear();
    _rightArray.clear();

    QAudioFormat desiredFormat;

    desiredFormat.setChannelCount(2);
    desiredFormat.setCodec("audio/pcm");
    desiredFormat.setSampleType(QAudioFormat::SignedInt);
    desiredFormat.setSampleRate(44100);
    desiredFormat.setSampleSize(16);

    _decoder->setAudioFormat(desiredFormat);
    _decoder->setSourceFilename(filePath);

    connect(_decoder, SIGNAL(bufferReady()), this, SLOT(readBuffer()));
    connect(_decoder, SIGNAL(finished()), this, SLOT(stopLoading()));
	connect(this, SIGNAL(percentageComplete(int)), ui->progressBar, SLOT(setValue(int)));
    _decoder->start();
}

void SoundProcess::readBuffer()
{
    QAudioBuffer buffer =  _decoder->read();


    const qint16* data = buffer.constData<qint16>();

    _inputArray.reserve(buffer.byteCount() / 2);

    for (int i = 0; i < buffer.byteCount() / 2; ++i) {
        _inputArray.push_back(data[i]);
    }
}

void SoundProcess::stopLoading()
{
    _leftArray.clear();
    _rightArray.clear();

    _leftArray.reserve(_inputArray.size() / 2);
    _rightArray.reserve(_inputArray.size() / 2);

    for (int i = 0; i < _inputArray.size(); i =  i + 2) {
        _leftArray.push_back(_inputArray.at(i));
        _rightArray.push_back(_inputArray.at(i + 1));
    }

    plotRaw();

    QMessageBox msgBox;
    msgBox.setText("File was loaded.");
    msgBox.exec();
}

void SoundProcess::processFile()
{
	ui->progressBar->setValue(0);
    _leftArrayProcessed.clear();
    _rightArrayProcessed.clear();
    _leftArrayProcessed.reserve(_leftArray.size());
    _rightArrayProcessed.reserve(_rightArray.size());

    for (int i = 0; i < _leftArray.size(); ++i) {
		_leftArrayProcessed.push_back(_vectorFilters.at(0)->update(_leftArray.at(i)));
		_rightArrayProcessed.push_back(_vectorFilters.at(1)->update(_rightArray.at(i)));
		emit percentageComplete(static_cast<int>(i * 100/_leftArray.size()));
    }

	ui->progressBar->setValue(0);

    plotProcessed();
    QMessageBox msgBox;
    msgBox.setText("File was processed.");
    msgBox.exec();
}

void SoundProcess::plotRaw()
{
    time.clear();
    leftChannel.clear();
    rightChannel.clear();

    time.reserve(_leftArray.size());
    leftChannel.reserve(_leftArray.size());
    rightChannel.reserve(_rightArray.size());

    for (int i = 0; i < _leftArray.size(); ++i) {
        time.push_back(double(i) / 44100.0);
        leftChannel.push_back(double(_leftArray.at(i)));
        rightChannel.push_back(double(_rightArray.at(i)));
    }

    _plotLeftChannel->graph(0)->setData(time, leftChannel);
    _plotRightChannel->graph(0)->setData(time, rightChannel);
    _plotLeftChannel->replot();
    _plotRightChannel->replot();
}

void SoundProcess::plotProcessed()
{
    time.clear();
    leftChannelProcessed.clear();
    rightChannelProcessed.clear();

    time.reserve(_leftArray.size());
    leftChannelProcessed.reserve(_leftArrayProcessed.size());
    rightChannelProcessed.reserve(_rightArrayProcessed.size());

    for (int i = 0; i < _leftArrayProcessed.size(); ++i) {
        time.push_back(double(i) / 44100.0);
        leftChannelProcessed.push_back(double(_leftArrayProcessed.at(i)));
        rightChannelProcessed.push_back(double(_rightArrayProcessed.at(i)));
    }

    _plotLeftChannelProcessed->graph(0)->setData(time, leftChannelProcessed);
    _plotRightChannelProcessed->graph(0)->setData(time, rightChannelProcessed);
    _plotLeftChannelProcessed->replot();
	_plotRightChannelProcessed->replot();
}

QByteArray SoundProcess::getWavWithHeader(const QByteArray &array, quint32 sampleRate)
{
	QByteArray result;

	quint32 chunksize=0x10;
	struct
	{
		quint16    wFormatTag;
		quint16    wChannels;
		quint32     dwSamplesPerSec;
		quint32     dwAvgBytesPerSec;
		quint16    wBlockAlign;
		quint16    wBitsPerSample;
	} fmt;

	long samplecount = array.size();
	long riffsize    = samplecount+0x24;
	long datasize    = samplecount*2;

	result.append("RIFF");
	result.append(QByteArray((const char*)&riffsize, 4));
	result.append("WAVEfmt ");
	result.append(QByteArray((const char*)&chunksize, 4));

	int bitsPerSample = 16;
	int bytesPerSample = bitsPerSample/8;
	int channel = 2;

	fmt.wFormatTag = 1;      // PCM
	fmt.wChannels  = channel;
	fmt.dwSamplesPerSec  = sampleRate;
	fmt.dwAvgBytesPerSec = sampleRate*channel*bytesPerSample;
	fmt.wBlockAlign      = channel*bytesPerSample;
	fmt.wBitsPerSample   = bitsPerSample;

	result.append(QByteArray((const char*)&fmt, sizeof(fmt)));
	result.append("data");
	result.append(QByteArray((const char*)&datasize, 4));//

	result.append(array);

	return result;
}

void SoundProcess::saveToWav()
{
	QFile file("output.wav");
	file.open(QIODevice::WriteOnly);
	file.write(getWavWithHeader(generateArray(_leftArrayProcessed, _rightArrayProcessed), 44100));
	file.close();

	QMessageBox msgBox;
	msgBox.setText("File was saved.");
	msgBox.exec();
}

QByteArray SoundProcess::generateArray(const QVector<float> &leftChannel, const QVector<float> &rightChannel) const
{
	QByteArray result;

	if (leftChannel.size() != rightChannel.size()) return result;

	result.reserve(leftChannel.size() * 2);

	for (quint32 i = 0; i < leftChannel.size(); ++i) {
		quint16 lValue = static_cast<qint16>(leftChannel.at(i));
		quint16 rValue = static_cast<qint16>(rightChannel.at(i));

		result.append(QByteArray((const char*)&lValue, 2));
		result.append(QByteArray((const char*)&rValue, 2));
	}

	return result;
}

void SoundProcess::saveToFile(const QVector<float> &leftChannel, const QVector<float> &rightChannel, const QString &filename)
{
	emit percentageComplete(0);
	QFile file(filename);
	file.open(QIODevice::WriteOnly);

	for (quint32 i = 0; i < leftChannel.size(); ++i) {
		QString line = QString("%1 %2\n").arg(QString::number(leftChannel.at(i)), QString::number(rightChannel.at(i)));
		file.write(line.toUtf8());
		emit percentageComplete(i*100/leftChannel.size());
	}
	file.close();
	emit percentageComplete(0);
}

void SoundProcess::on_pushButtonSaveInitAsText_clicked()
{
	saveToFile(_leftArray, _rightArray, "raw.txt");
}

void SoundProcess::initPlots()
{
    _plotLeftChannel = new QCustomPlot(this);
    _plotLeftChannel->yAxis->setLabel("Left channel");
    _plotLeftChannel->yAxis->setRange(-40000, 40000);
    _plotLeftChannel->xAxis->setLabel("seconds");
    _plotLeftChannel->xAxis->setRange(0, 60);
    _plotLeftChannel->addGraph();

    _plotRightChannel = new QCustomPlot(this);
    _plotRightChannel->yAxis->setLabel("Right channel");
    _plotRightChannel->yAxis->setRange(-40000, 40000);
    _plotRightChannel->xAxis->setLabel("seconds");
    _plotRightChannel->xAxis->setRange(0, 60);
    _plotRightChannel->addGraph();

    _plotLeftChannelProcessed = new QCustomPlot(this);
    _plotLeftChannelProcessed->yAxis->setLabel("Left channel");
    _plotLeftChannelProcessed->yAxis->setRange(-40000, 40000);
    _plotLeftChannelProcessed->xAxis->setLabel("seconds");
    _plotLeftChannelProcessed->xAxis->setRange(0, 60);
    _plotLeftChannelProcessed->addGraph();

    _plotRightChannelProcessed = new QCustomPlot(this);
    _plotRightChannelProcessed->yAxis->setLabel("Right channel");
    _plotRightChannelProcessed->yAxis->setRange(-40000, 40000);
    _plotRightChannelProcessed->xAxis->setLabel("seconds");
    _plotRightChannelProcessed->xAxis->setRange(0, 60);
    _plotRightChannelProcessed->addGraph();

	ui->gridLayout->addWidget(_plotLeftChannel, 4, 1);
	ui->gridLayout->addWidget(_plotRightChannel, 4, 2);
	ui->gridLayout->addWidget(_plotLeftChannelProcessed, 5, 1);
	ui->gridLayout->addWidget(_plotRightChannelProcessed, 5, 2);
}

void SoundProcess::on_pushButtonProcessFile_clicked()
{
    processFile();
}

void SoundProcess::on_pushButtonSaveResultAsFile_clicked()
{
	saveToWav();
}

void SoundProcess::on_pushButtonSaveResultAsText_clicked()
{
	saveToFile(_leftArrayProcessed, _rightArrayProcessed, "processed.txt");
}
