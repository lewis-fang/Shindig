#pragma once

#include <QtCharts/qchartview.h>
#include "QtCharts/qscatterseries.h"
#include "QtCharts/qlineseries.h"
#include "ui_FNN.h"
#include "qfiledialog.h"
#include "qtextstream.h"
#include "CNNLayer.h"
#include <vector>
#include"qstandarditemmodel.h"
#include "CNNModel.h"
#include <QtCharts/qvalueaxis.h>
#include "readCifar10.h"
#include <thread>
#include"DataDefine.h"
#define loopImg 10000
#define CHECKSPEED
using namespace QtCharts;

class FNN : public QMainWindow
{
Q_OBJECT

public:
	FNN(QWidget *parent = Q_NULLPTR);

private:
	Ui::FNNClass ui;
	
	std::vector<image> ImportImages;
	std::vector<float*> IdealOut;
	std::vector<kernal> kernalSeries;
	//CNNCalc CNNLayer;
	CNNModel MyCNNModel;
	bool isDataImported;
	bool isOutReady;
	QStandardItemModel* treeViewModel;

	QChartView * qChart1Loss;
	QLineSeries* qLineLoss;
	QLineSeries* qLineLossDiff;
	cifar10Reader cifar;

	void FreeDts();
	void UpdateCNNTreeView();
	int getCpuId();

	
	int findmax(float* ar, int len);
	void updateLoss();
public slots:
//	void ImpotsDatas();
	void ImpotWts();
	void SetInputSize();
	void Launch();
	void ViewOut(int);
	void ViewiNPUT(int i);
	void AddLayer();
	void LauchCNNModel();
	void uiSetFCPara(int fc);
	void popBack();
	void ChangeTrainMode(int);
	void LaunchTraing();
//	void ImportIdlO();
	void ImportCifarTrain(QString fileName ="");
	void ImportCifarTest();
	void LaunchTraingThread();
	void LauchCNNModelParrallel();

	void buildDefault1CifarModel();
	void buildDefault1CifarModelBN();

	void egonicTestImages();
	void egonicTrainImages();

	void buildDefault1CifarModelBNResNet();
};