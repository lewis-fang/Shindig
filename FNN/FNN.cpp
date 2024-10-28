#include "FNN.h"
#include <iostream>
#include<atomic>
const InstructionSet::InstructionSet_Internal InstructionSet::CPU_Rep;

FNN::FNN(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.pushButton_importImage, SIGNAL(clicked()), this, SLOT(ImpotsDatas()), Qt::AutoConnection);
	connect(ui.pushButton_ImportIdealOut, SIGNAL(clicked()), this, SLOT(ImportIdlO()), Qt::AutoConnection);

	connect(ui.pushButton_ImportWts, SIGNAL(clicked()), this, SLOT(ImpotWts()), Qt::AutoConnection);
//	connect(ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeTrainMode(int)), Qt::AutoConnection);
//	connect(ui.pushButton_LaunchCNNLayer, SIGNAL(clicked()), this, SLOT(Launch()), Qt::AutoConnection);
	connect(ui.spinBox_inputImageIndex, SIGNAL(valueChanged(int)), this, SLOT(ViewiNPUT(int)), Qt::AutoConnection);
	//view train images
	connect(ui.spinBox_viewOutTest, SIGNAL(valueChanged(int)), this, SLOT(ViewOut(int)), Qt::AutoConnection);
	//view test image and out
	connect(ui.pushButton_addLayer, SIGNAL(clicked()), this, SLOT(AddLayer()), Qt::AutoConnection);
	connect(ui.pushButton_LaunchModel, SIGNAL(clicked()), this, SLOT(LauchCNNModel()), Qt::AutoConnection);
	connect(ui.comboBox_LayerType, SIGNAL(currentIndexChanged(int)), this, SLOT(uiSetFCPara(int)), Qt::AutoConnection);
	connect(ui.pushButton_popBackLayer, SIGNAL(clicked()), this, SLOT(popBack()), Qt::AutoConnection);
//	connect(ui.pushButton_TraainingMode, SIGNAL(clicked()), this, SLOT(LaunchTraing()),Qt::AutoConnection);
	connect(ui.pushButton_startTraning, SIGNAL(clicked()), this, SLOT(LaunchTraingThread()), Qt::AutoConnection);
	connect(ui.pushButton_cifarTrains, SIGNAL(clicked()), this, SLOT(ImportCifarTrain()), Qt::AutoConnection);
	connect(ui.pushButton_cifarTest, SIGNAL(clicked()), this, SLOT(ImportCifarTest()), Qt::AutoConnection);

	
	//ui.pushButton_addLayer->setfill
	isDataImported = false;

	treeViewModel = new QStandardItemModel;
	ui.treeView->setModel(treeViewModel);
	QStringList header;
	header << "Layer" << "Dim1"<<"Dim2"<<"Dim3";
	treeViewModel->setHorizontalHeaderLabels(header);
	ui.treeView->setColumnWidth(0, 400);
	ui.treeView->setColumnWidth(1, 100);
	ui.treeView->setColumnWidth(2, 100);
	ui.treeView->setColumnWidth(3, 100);

	qChart1Loss = new QChartView;
	qLineLoss = new QLineSeries;
	qLineLossDiff = new QLineSeries;
	
	qChart1Loss->chart()->addSeries(qLineLoss);
	qChart1Loss->chart()->addSeries(qLineLossDiff);


	QValueAxis* axix = new QValueAxis;
	
	QValueAxis* axiy = new QValueAxis;
	QValueAxis* secaxiy = new QValueAxis;
	axix->setLabelFormat("%.0f");
	axix->setRange(0, 1);
	axix->setTickCount(2);
	
	axiy->setLabelFormat("%.3f");
	secaxiy->setLabelFormat("%.3f");
	qChart1Loss->chart()->addAxis(axix, Qt::AlignBottom);
	qChart1Loss->chart()->addAxis(axiy, Qt::AlignLeft);
	qChart1Loss->chart()->addAxis(secaxiy, Qt::AlignRight);
	qLineLoss->attachAxis(axix);
	qLineLoss->attachAxis(axiy);
	qLineLossDiff->attachAxis(axix);
	qLineLossDiff->attachAxis(secaxiy);
	qChart1Loss->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
	qChart1Loss->chart()->legend()->setVisible(false);
	ui.verticalLayout_Loss->addWidget(qChart1Loss);
	
	getCpuId();


}
void FNN::ChangeTrainMode(int mode)
{

}
void FNN::ImportIdlO()
{
	FreeDts();
	for (int i = 0; i < IdealOut.size(); i++)
	{
		delete IdealOut.at(i);
	}
	IdealOut.clear();
	int imgNum = ui.lineEdit_ImgNumbers->text().toInt();
	int outlen = ui.spinB_ExpectedOutLen->value();
	int rows = ui.spinBox_rows->value();
	int cols = ui.spinBox_rows->value();
	for (int im = 0; im < imgNum; im++)
	{
		double* IdealOutCevtor=new double[outlen];
		//dIdealOut.push_back(IdealOutCevtor);
	}


	QString fileName = QFileDialog::getOpenFileName(this, tr("import IdealOut"), "", tr("CSV(*.csv)","TXT(*,txt)")); //选择路径
	//std::cout << fileName.toLocal8Bit().data() << std::endl;
	QFile IDO(fileName);
	int cnRows = -1;
	bool GoodCol = true;
	int imIndex = 0;
	if (IDO.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString qLine;
		QTextStream qstream(&IDO);

		while (!qstream.atEnd() & (++cnRows)<imgNum)
		{
			qLine = qstream.readLine();
			QStringList listLine = qLine.split(',');
			if (listLine.size() == outlen)
			{
				double* o;
				int x = -1;
				//std::where(x < 0, x) = -x;
				//double* o = IdealOut[cnRows];
				for (int i = 0; i < outlen; i++)
				{
					o[i] = listLine.at(i).toDouble();
				}
			}
			else
			{
				std::cout << "A bad line is read~" << std::endl;
				GoodCol = false;
				isOutReady = false;
				break;
			}
		}
	}
	if (cnRows == imgNum )
	{
		isOutReady = true;
		for (int r = 0; r < rows; r++)
		{
			QString aLne;
			for (int c = 0; c < cols; c++)
			{
				aLne += QString::number(ImportImages.at(0).imageData[0][r][c]) + ',';

			}
			ui.textBrowser->append(aLne);
		}
	//	ui.lineEdit->setText("Yes");
		ui.textBrowser->append("Dts are read!");
		//CNNLayer.SetInput(ImportImages.at(0));
		ui.textBrowser->append("Dts are Delivered to CNN Layer!");
		isDataImported = true;
	}
	else
	{
//		ui.lineEdit->setText("No");
	}


}
void FNN::ImpotsDatas()
{
	FreeDts();
	int imgNum = ui.lineEdit_ImgNumbers->text().toInt();
	int rows = ui.spinBox_rows->value();
	int cols = ui.spinBox_cols->value();
	for (int im = 0; im < imgNum; im++)
	{
		image ImportImage;
		ImportImage.channel = ui.spinBox_chans->value();
		ImportImage.rows = rows;
		ImportImage.cols = cols;
		ImportImage.initImage();
		ImportImages.push_back(ImportImage);
	}


	QString fileName = QFileDialog::getOpenFileName(this, tr("import Dts"), "", tr("CSV(*.csv)")); //选择路径
	std::cout << fileName.toLocal8Bit().data() << std::endl;
	QFile Dts(fileName);
	int cnRows = -1;
	bool GoodCol=true;
	int imIndex = 0;
	if (Dts.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString qLine;
		QTextStream qstream(&Dts);
		qLine = qstream.readLine();
		
		while (!qstream.atEnd() & (++cnRows)<imgNum*rows)
		{
			qLine = qstream.readLine();
			QStringList listLine = qLine.split(',');
			if (listLine.size() == cols+1)
			{
				imIndex = std::ceil(cnRows / rows);
				int curtRow = cnRows - rows*imIndex;
				if (curtRow < rows)
				{

					for (int c = 0; c < cols; c++)
					{
						ImportImages.at(imIndex).imageData[0][curtRow][c] = listLine.at(c + 1).toDouble();
					}
				}				
			}
			else
			{
				std::cout << "A bad line is read~" << std::endl;
				GoodCol = false;
				break;
			}
		}
	}
	if (cnRows == imgNum*rows & GoodCol)
	{
		for (int r = 0; r < rows; r++)
		{
			QString aLne;
			for (int c = 0; c < cols; c++)
			{
				aLne += QString::number(ImportImages.at(0).imageData[0][r][c]) + ',';
				
			}
			ui.textBrowser->append(aLne);
		}
//		ui.lineEdit->setText("Yes");
		ui.textBrowser->append("Dts are read!");
		//CNNLayer.SetInput(ImportImages.at(0));
		ui.textBrowser->append("Dts are Delivered to CNN Layer!");
		isDataImported = true;
	}
	else
	{
//		ui.lineEdit->setText("No");
	}

	
}
void FNN::SetInputSize()
{
}
void FNN::ImpotWts()
{
	//FreeWts();
	int neuroNum = ui.spinBox_NeuroNums->value();
	QString fileName = QFileDialog::getOpenFileName(this, tr("import Dts"), "", tr("CSV(*.csv)")); //选择路径
	std::cout << fileName.toLocal8Bit().data() << std::endl;
	QFile Wts(fileName);
	bool GoodLine = true;
	int row = ui.spinBox_WtsRows->value();
	int col = ui.spinBox_WtsCols->value();
	int padsize = 0;
	int strd = ui.spinBox_Strides->value();
	if (Wts.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString qLine;
		QTextStream qstream(&Wts);
		while (!qstream.atEnd())
		{
			qLine = qstream.readLine();
			QStringList qstrlist = qLine.split(',');
			if (qstrlist.size() == row*col)
			{
				kernal tempKernal;
				tempKernal.channel = 1;
				tempKernal.row = row;
				tempKernal.col = col;
				tempKernal.initKernal(1);
				for (int r = 0; r < row; r++)
				{
					for (int c = 0; c < col; c++)
					{
						tempKernal.weight[0][r][c] = qstrlist.at(r* col + c).toDouble();
					}
				}
				kernalSeries.push_back(tempKernal);
			}
			else
			{
				std::cout << "a bad line is read~" << std::endl;
				GoodLine = false;
			}
		}
	}
	if ((kernalSeries.size() == neuroNum) & (GoodLine == true))
	{
		for (int kn = 0; kn < neuroNum; kn++)
		{
			QString aLne;
			for (int r = 0; r < kernalSeries.at(kn).row; r++)
			{			
				for (int c = 0; c < kernalSeries.at(kn).col; c++)
				{
					aLne += QString::number(kernalSeries.at(kn).weight[0][r][c]) + ',';

				}
				
			}
			ui.textBrowser->append(aLne);
		}

//		ui.lineEdit_WtsReady->setText("Yes");
		ui.textBrowser->append("Wts are read!");
		//CNNLayer.SetKernals(kernalSeries, row, col, padsize, strd, neuroNum);
		ui.textBrowser->append("Wts are tranported to CNN Layer!");
	}
	else
	{
		std::cout << "read neros:" << kernalSeries.size() << std::endl;
//		ui.lineEdit_WtsReady->setText("No");
	}
}
void FNN::FreeDts()
{
	ImportImages.clear();

}
void FNN::Launch()
{


}
void FNN::ViewOut(int)
{
	int currentDim = ui.spinBox_viewOutTest->value();
	if ((ImportImages.size()==0) | (IdealOut.size()==0))
	{
		ui.textBrowser->append("test images are empty");
		return;
	}
	if (ImportImages.size() != IdealOut.size())
	{
		ui.textBrowser->append("image number and expected out data number are not equal");
		return;;
	}
	if (currentDim > (ImportImages.size() - 1)) currentDim = ImportImages.size() - 1;
	image im= ImportImages.at(currentDim);
	float* expectedOut = IdealOut.at(currentDim);
	const int sizeDim = 128;
	unsigned char* imageStream = new unsigned char[sizeDim * sizeDim * 3];
	for (int ch = 0;ch < 3;ch++)
	{
		for (int r = 0;r < sizeDim;r++)
		{
			for (int c = 0;c < sizeDim;c++)
			{
				imageStream[ch + r * sizeDim * 3 + c * 3] = (unsigned char)im.imageData[ch][r / (sizeDim / cifarSize::D1)][c / (sizeDim / cifarSize::D2)];
			}
		}
	}
	QImage img(imageStream, sizeDim, sizeDim, sizeDim * 3, QImage::Format_RGB888);
	QPixmap pix = QPixmap::fromImage(img);
	img.scaled(ui.label_pITURE->size(), Qt::KeepAspectRatio);
	ui.label_pITURE->setPixmap(pix);
	/*
	float* out = new float[1];
	image normalizedImage = MyCNNModel.normliaze(im);
	if (MyCNNModel.LaunchCNNModelBySimd(normalizedImage, out, 1))
	{
		ui.textBrowser->append("your answer is: " + QString::number(out[0] * 10));
		ui.textBrowser->append("the expected answer is: " + QString::number(expectedOut[0] * 10));
	}
	else
	{
		ui.textBrowser->append("make sure your model has been properly built");

	}
	*/
}
void FNN::ViewiNPUT(int i)
{
	
	int maxSize = MyCNNModel.getImageNum();
	if (maxSize > 0)
	{
		if (i >= maxSize) i = maxSize - 1;
		const int sizeDim = 128;
		unsigned char* imageStream = new unsigned char[sizeDim * sizeDim * 3];
		image currentImage = MyCNNModel.getImage(i);
		for (int ch = 0;ch < 3;ch++)
		{
			for (int r = 0;r < sizeDim;r++)
			{
				for (int c = 0;c < sizeDim;c++)
				{
					imageStream[ch + r * sizeDim * 3 + c * 3] = (unsigned char)currentImage.imageData[ch][r/(sizeDim/ cifarSize::D1)][c/ (sizeDim / cifarSize::D2)];
				}
			}
		}
		QImage img(imageStream, sizeDim, sizeDim, sizeDim * 3, QImage::Format_RGB888);
		QPixmap pix = QPixmap::fromImage(img);
		img.scaled(ui.label_pITURE->size(), Qt::KeepAspectRatio);
		ui.label_pITURE->setPixmap(pix);
	}

}
void FNN::AddLayer()
{
	bool checkPara = true;
	if (ui.comboBox_LayerType->currentIndex() == 0)
	{
		if ((ui.spinBox_WtsRows->value() < 1) | (ui.spinBox_WtsCols->value()<1))
		{
			checkPara = false;
			ui.textBrowser->append("kernal size must be larger than 1!");
		}
		if (MyCNNModel.getLastLayerType() == FULLYCONNECTION)
		{
			checkPara = false;
			ui.textBrowser->append("Convolution layer can not be put behind a FC!");
		}
	}

	if (checkPara)
	{
		int neuroNum = ui.spinBox_NeuroNums->value();
		int WtsRow = ui.spinBox_WtsRows->value();
		int WtsCol = ui.spinBox_WtsCols->value();
		int strd = ui.spinBox_Strides->value();
		int WtsChannel = MyCNNModel.getLastLayerOutChannel();
		int poolingdime1 = ui.spinBox_poolingDim1->value();
		int poolingdime2= ui.spinBox_poolingDim2->value();
		int poolingstride = ui.spinBox_poolingStride->value();
		int poolingFun = ui.spinBox_poolingFun->value();

		float sd = ui.lineEdit_initSd->text().toFloat();
		float bs = ui.lineEdit_bias->text().toFloat();
		if (WtsChannel == 0)
		{//first layer
			WtsChannel = ui.spinBox_chans->value();
		}
		
		CNNCalc CurrentLayer;
		size_t blockSize = WtsRow * WtsCol * WtsChannel;

		sd = sqrt(2.0 / blockSize);
		CurrentLayer.initKernals(WtsRow, WtsCol, WtsChannel, neuroNum, strd,sd,bs);
	
		CurrentLayer.setHiddenNum(MyCNNModel.getLastLayerNum());
		CurrentLayer.SetPoolings(poolingdime1, poolingdime2, poolingstride, poolingFun);
		CurrentLayer.SetActivateFun(ui.spinBox_actFunction->value());
		CurrentLayer.setlayerType((layerType)(ui.comboBox_LayerType->currentIndex() + 1));
		int lastRow = 0;
		int lastCol = 0;
		MyCNNModel.getLastLayerOutSize(lastRow, lastCol);
		if ((lastRow == 0) | (lastCol == 0))
		{
			lastRow = ui.spinBox_rows->value();
			lastCol = ui.spinBox_cols->value();
		}
		CurrentLayer.initLayerMemory(lastRow, lastCol, WtsChannel);//th		
		MyCNNModel.addCNNLayer(CurrentLayer);	
		UpdateCNNTreeView();
		if (ui.comboBox_LayerType->currentIndex() == 1)
		{
			MyCNNModel.getLastLayerOutSize(lastRow, lastCol);
			ui.spinBox_WtsRows->setValue(lastRow);
			ui.spinBox_WtsCols->setValue(lastCol);
		}
		
	}
	
}
void FNN::LauchCNNModel()
{

	bool isModelReady = true;
	int expectedOutLen = ui.spinB_ExpectedOutLen->value();
	int outRow = 0;
	int outCol = 0;
	int normMethod = ui.comboBox_normalizedMethd->currentIndex();
	double globalNormValue = ui.lineEdit_NormalizedRange->text().toDouble();
	double shiftValue = ui.lineEdit_NormalizedSHif->text().toDouble();
	MyCNNModel.setNormMethod(normMethod, globalNormValue, shiftValue);
	MyCNNModel.getLastLayerOutSize(outRow, outCol);
	if (MyCNNModel.getLastLayerOutChannel() != expectedOutLen)
	{
		ui.textBrowser->append("current output Lentgh is not equal to that of expectd!");
		isModelReady = false;
	}
	if ((outRow != 1)| (outCol != 1))
	{
		ui.textBrowser->append("the Model is not complete!");
		isModelReady = false;
	}
	if (isModelReady &  isDataImported)
	{
		float*outVector = new float[expectedOutLen];
		int indexImg = ui.spinBox_viewOutTest->value();
		
		if ((indexImg<ImportImages.size()))
		{			
#ifdef CHECKSPEED
			clock_t st = clock();
			for (int i = 0;i < loopImg;i++)
			{
				image normImg = MyCNNModel.normliaze(ImportImages.at(indexImg));
				MyCNNModel.LaunchCNNModel(normImg, outVector, expectedOutLen);
				normImg.freeImage();
			}	
			std::cout << "time non-simd:" << double(clock() - st) << std::endl;
			std::cout << std::setprecision(6) << "out non-simd:" << outVector[0] << std::endl;
			clock_t st2 = clock();
			for (int i = 0;i < loopImg;i++)
			{
				image normImgSimd = MyCNNModel.normliaze(ImportImages.at(indexImg));
				MyCNNModel.LaunchCNNModelBySimd(normImgSimd, outVector, expectedOutLen);
				normImgSimd.freeImage();
			}
			std::cout << "time simd:" << double(clock() - st2) << std::endl;
			std::cout << std::setprecision(6) << "out non-simd:" << outVector[0] << std::endl;
#else
			int traingflag = ui.comboBox_useSimd->currentIndex();
			if (traingflag==1 | traingflag==2)
			{
				image normImgSimd = MyCNNModel.normliazeSimd(ImportImages.at(indexImg));
				MyCNNModel.LaunchCNNModelBySimd(normImgSimd, outVector, expectedOutLen);
				normImgSimd.freeImage();
			}
			else
			{
				image normImgSimd = MyCNNModel.normliaze(ImportImages.at(indexImg));
				MyCNNModel.LaunchCNNModel(normImgSimd, outVector, expectedOutLen);
				normImgSimd.freeImage();
			}
			
#endif
			

			
			ui.textBrowser->append("CNN calc successfully!");
			for (int i = 0; i < expectedOutLen; i++)
			{
				ui.textBrowser->append("Model Out is:"+QString::number(outVector[i], 'f', 2));
				ui.textBrowser->append("Expected Out is:" + QString::number(IdealOut.at(indexImg)[i], 'f', 2));
			}
		}
		else
		{
			ui.textBrowser->append("CNN calc faily!");
		}
		delete outVector;
	}
	else
	{
		ui.textBrowser->append("Image is not imported Or Model is not constructed!");
	}
}
void FNN::uiSetFCPara(int fc)
{
	fc = ui.comboBox_LayerType->currentIndex();
	if (fc == 1)
	{
		int row = 0;
		int col = 0;
		MyCNNModel.getLastLayerOutSize(row, col);
		if ((row == 0) | (col == 0))
		{
			row = ui.spinBox_rows->value();
			col = ui.spinBox_cols->value();
		}
		ui.spinBox_WtsRows->setValue(row);
		ui.spinBox_WtsCols->setValue(col);

	}
}
void FNN::popBack()
{
	MyCNNModel.popLayer();
	int lastRow = 0;
	int lastCol = 0;
	MyCNNModel.getLastLayerOutSize(lastRow, lastCol);
	if ((lastRow == 0) | (lastCol == 0))
	{
		lastRow = ui.spinBox_rows->value();
		lastCol = ui.spinBox_cols->value();
		ui.textBrowser->append("the model has be emptied!");
	}
	else
	{
		ui.textBrowser->append("pop layer successfully!");
	}
	
	ui.lineEdit_depth->setText(QString::number(MyCNNModel.getLastLayerNum()));
	UpdateCNNTreeView();
}
void FNN::UpdateCNNTreeView()
{
	//treeViewModel->clear();
	int dpth = MyCNNModel.getLastLayerNum();
	if (dpth > treeViewModel->rowCount())
	{
		CNNCalc currentLayer = MyCNNModel.getLayer(dpth - 1);
		if (currentLayer.getHiddenNum() == dpth - 1)
		{
			QString strLayerType;
			if (currentLayer.getCurrentLayerType() == CONVOLUTION)
			{
				strLayerType = "CONVOLUTION";
			}
			else
			{
				strLayerType = "FULLY CONNECTION";
			}
			QStandardItem* newItemRow0 = new QStandardItem;
			newItemRow0->setText("Layer " + QString::number(dpth - 1)+": "+strLayerType);
			//input image
			QList<QStandardItem*> InputRow;
			QStandardItem* newItemRol10 = new QStandardItem;
			QStandardItem* newItemRol11 = new QStandardItem;
			QStandardItem* newItemRol12 = new QStandardItem;
			QStandardItem* newItemRol13 = new QStandardItem;

			newItemRol10->setText("Input Image");
			newItemRol11->setText(QString::number(currentLayer.getInputImage().rows));
			newItemRol12->setText(QString::number(currentLayer.getInputImage().cols));
			newItemRol13->setText(QString::number(currentLayer.getInputImage().channel));
			InputRow << newItemRol10 << newItemRol11 << newItemRol12 << newItemRol13;
			newItemRow0->appendRow(InputRow);


			//padding image
			QList<QStandardItem*> PaddingRow;
			QStandardItem* newItemRol20 = new QStandardItem;
			QStandardItem* newItemRol21 = new QStandardItem;
			QStandardItem* newItemRol22 = new QStandardItem;
			QStandardItem* newItemRol23 = new QStandardItem;
			newItemRol20->setText("Padding Image");
			newItemRol21->setText(QString::number(currentLayer.getPaddingImage().rows));
			newItemRol22->setText(QString::number(currentLayer.getPaddingImage().cols));
			newItemRol23->setText(QString::number(currentLayer.getPaddingImage().channel));
			PaddingRow << newItemRol20 << newItemRol21 << newItemRol22 << newItemRol23;
			newItemRow0->appendRow(PaddingRow);

			//WET image
			QList<QStandardItem*> KernalRow;
			QStandardItem* newItemRolK0 = new QStandardItem;
			QStandardItem* newItemRolK1 = new QStandardItem;
			QStandardItem* newItemRolK2 = new QStandardItem;
			QStandardItem* newItemRolK3 = new QStandardItem;
			newItemRolK0->setText("kernals x " + QString::number(currentLayer.GetOutLength()));
			newItemRolK1->setText(QString::number(currentLayer.getFirstKernal().row));
			newItemRolK2->setText(QString::number(currentLayer.getFirstKernal().col));
			newItemRolK3->setText(QString::number(currentLayer.getFirstKernal().channel));
			KernalRow << newItemRolK0 << newItemRolK1 << newItemRolK2 << newItemRolK3;
			newItemRow0->appendRow(KernalRow);
			

			if (currentLayer.getCurrentLayerType() == CONVOLUTION)
			{//activation image
				QList<QStandardItem*> ActRow;
				QStandardItem* newItemRol30 = new QStandardItem;
				QStandardItem* newItemRol31 = new QStandardItem;
				QStandardItem* newItemRol32 = new QStandardItem;
				QStandardItem* newItemRol33 = new QStandardItem;
				newItemRol30->setText("Convolution & Activate Image");
				newItemRol31->setText(QString::number(currentLayer.getActImage().rows));
				newItemRol32->setText(QString::number(currentLayer.getActImage().cols));
				newItemRol33->setText(QString::number(currentLayer.getActImage().channel));
				ActRow << newItemRol30 << newItemRol31 << newItemRol32 << newItemRol33;
				newItemRow0->appendRow(ActRow);
			}
			//output image
			QList<QStandardItem*> outRow;
			QStandardItem* newItemRol40 = new QStandardItem;
			QStandardItem* newItemRol41 = new QStandardItem;
			QStandardItem* newItemRol42 = new QStandardItem;
			QStandardItem* newItemRol43 = new QStandardItem;
			newItemRol40->setText("Out Image");
			newItemRol41->setText(QString::number(currentLayer.getOutImage().rows));
			newItemRol42->setText(QString::number(currentLayer.getOutImage().cols));
			newItemRol43->setText(QString::number(currentLayer.getOutImage().channel));
			outRow << newItemRol40 << newItemRol41 << newItemRol42 << newItemRol43;
			newItemRow0->appendRow(outRow);

			treeViewModel->appendRow(newItemRow0);
		}
		else
		{
			ui.textBrowser->append("It's not the layer i need~");
		}
	}
	else
	{
		treeViewModel->removeRow(treeViewModel->rowCount() - 1);
	}
	ui.treeView->expandAll();
}
void FNN::LaunchTraingThread()
{
	int batchSize = ui.spinBox_batchSize->value();
	int normMethod = ui.comboBox_normalizedMethd->currentIndex();
	double globalNormValue = ui.lineEdit_NormalizedRange->text().toDouble();
	double shiftValue = ui.lineEdit_NormalizedSHif->text().toDouble();
	MyCNNModel.setNormMethod(normMethod, globalNormValue, shiftValue);
	MyCNNModel.setMaxiters(ui.lineEdit_maxIters->text().toInt());
	MyCNNModel.setDiffLoss(ui.lineEdit_diffLoss->text().toDouble());
	MyCNNModel.setMinloss(ui.lineEdit_Loss->text().toDouble());
	MyCNNModel.setLearnRate(ui.lineEdit_learnRate->text().toDouble());
	MyCNNModel.setBatchSize(batchSize);
	bool ret = true;
	int trainflag = ui.comboBox_useSimd->currentIndex();
	ret = MyCNNModel.createThread(trainflag);

	std::thread trFriend(&FNN::updateLoss, this);
	trFriend.detach();
}
void contst()
{

}
void FNN::LaunchTraing()
{
	int batchSize = ui.spinBox_batchSize->value();
	int normMethod = ui.comboBox_normalizedMethd->currentIndex();
	double globalNormValue = ui.lineEdit_NormalizedRange->text().toDouble();
	double shiftValue = ui.lineEdit_NormalizedSHif->text().toDouble();
	MyCNNModel.setNormMethod(normMethod, globalNormValue, shiftValue);
	MyCNNModel.setMaxiters(ui.lineEdit_maxIters->text().toInt());
	MyCNNModel.setDiffLoss(ui.lineEdit_diffLoss->text().toDouble());
	MyCNNModel.setMinloss(ui.lineEdit_Loss->text().toDouble());
	MyCNNModel.setLearnRate(ui.lineEdit_learnRate->text().toDouble());
	MyCNNModel.setBatchSize(batchSize);
	bool ret = true;
	int trainflag = ui.comboBox_useSimd->currentIndex();
	if (trainflag==1 | trainflag==2)
	{
		ret=MyCNNModel.startTrainningSimd();
		///ret = MyCNNModel.createThread();
	}
	else
	{
		ret=MyCNNModel.startTrainning();
	}
	qLineLoss->clear();
	if (ret)
	{
		
		int currentIter = MyCNNModel.vloss.size();
		float maxvalue = 0.0;
		for (int i = 0;i < currentIter;i++)
		{
			qLineLoss->append(QPointF(i, MyCNNModel.vloss.at(i)));
			if (MyCNNModel.vloss.at(i) > maxvalue) maxvalue = MyCNNModel.vloss.at(i);
		}
		qChart1Loss->chart()->axisY()->setRange(0.0, maxvalue*1.1);
		qChart1Loss->chart()->axisX()->setRange(0, currentIter);
	//	qChart1Loss->chart()->createDefaultAxes();
	}
	else
	{
		std::cout << "training fail" << std::endl;
	}
}
void FNN::ImportCifarTrain()
{
	int pictureNum= ui.lineEdit_ImgNumbers->text().toInt();
	QString fileName = QFileDialog::getOpenFileName(this, tr("import cifar train"), "", tr("BIN(*.bin)")); //选择路径
	std::cout << fileName.toLocal8Bit().data() << std::endl;
	if(cifar.pullTestImages(fileName.toLocal8Bit().data(), pictureNum))
	{
		MyCNNModel.clearInputImage();
		if (cifar.checkSizeImageAndOut())
		{
			for( int i = 0;i< cifar.testGroup.size();i++)
			{
				float* o = new float[1];
				o[0] = 1.0*cifar.animalType.at(i)/10;
				if (!MyCNNModel.addInputImage(cifar.testGroup.at(i), o, 1))
				{
					std::cout << "add image to model error" << std::endl;
					MyCNNModel.clearInputImage();
					break;
				}
			}
		}
		else
		{
			std::cout << "check size fail" << std::endl;
		}
		std::cout << "read " << MyCNNModel.getImageNum() << " pictures" << std::endl;
	}
}

void FNN::ImportCifarTest()
{
	int pictureNum = ui.lineEdit_testImgNum->text().toInt();
	QString fileName = QFileDialog::getOpenFileName(this, tr("import cifar train"), "", tr("BIN(*.bin)")); //选择路径
	//std::cout << fileName.toLocal8Bit().data() << std::endl;
	if (cifar.pullTestImages1(fileName.toLocal8Bit().data(), pictureNum))
	{
		std::vector<image>().swap(ImportImages);
		for (float* a : IdealOut)
		{
			if (a != NULL)
			{
				delete a;
				a = NULL;
			}
		}
		IdealOut.clear();
		if (cifar.checkSizeImageAndOut1())
		{
			for (int i = 0;i < cifar.testGroup1.size();i++)
			{
				float* o = new float[1];
				o[0] = 1.0 * cifar.animalType1.at(i) / 10;
				IdealOut.push_back(o);
				ImportImages.push_back(cifar.testGroup1.at(i));
			}
		}
		isDataImported = true;
		ui.spinBox_viewOutTest->setMaximum(cifar.testGroup1.size()-1);
	}
}
int FNN::getCpuId()
{
	auto& outstream = std::cout;

	auto support_message = [&outstream](std::string isa_feature, bool is_supported) {
		outstream << isa_feature << (is_supported ? " supported" : " not supported") << std::endl;
	};

	std::cout << InstructionSet::Vendor() << std::endl;
	std::cout << InstructionSet::Brand() << std::endl;

	support_message("3DNOW", InstructionSet::_3DNOW());
	support_message("3DNOWEXT", InstructionSet::_3DNOWEXT());
	support_message("ABM", InstructionSet::ABM());
	support_message("ADX", InstructionSet::ADX());
	support_message("AES", InstructionSet::AES());
	support_message("AVX", InstructionSet::AVX());
	support_message("AVX2", InstructionSet::AVX2());
	support_message("AVX512CD", InstructionSet::AVX512CD());
	support_message("AVX512ER", InstructionSet::AVX512ER());
	support_message("AVX512F", InstructionSet::AVX512F());
	support_message("AVX512PF", InstructionSet::AVX512PF());
	support_message("BMI1", InstructionSet::BMI1());
	support_message("BMI2", InstructionSet::BMI2());
	support_message("CLFSH", InstructionSet::CLFSH());
	support_message("CMPXCHG16B", InstructionSet::CMPXCHG16B());
	support_message("CX8", InstructionSet::CX8());
	support_message("ERMS", InstructionSet::ERMS());
	support_message("F16C", InstructionSet::F16C());
	support_message("FMA", InstructionSet::FMA());
	support_message("FSGSBASE", InstructionSet::FSGSBASE());
	support_message("FXSR", InstructionSet::FXSR());
	support_message("HLE", InstructionSet::HLE());
	support_message("INVPCID", InstructionSet::INVPCID());
	support_message("LAHF", InstructionSet::LAHF());
	support_message("LZCNT", InstructionSet::LZCNT());
	support_message("MMX", InstructionSet::MMX());
	support_message("MMXEXT", InstructionSet::MMXEXT());
	support_message("MONITOR", InstructionSet::MONITOR());
	support_message("MOVBE", InstructionSet::MOVBE());
	support_message("MSR", InstructionSet::MSR());
	support_message("OSXSAVE", InstructionSet::OSXSAVE());
	support_message("PCLMULQDQ", InstructionSet::PCLMULQDQ());
	support_message("POPCNT", InstructionSet::POPCNT());
	support_message("PREFETCHWT1", InstructionSet::PREFETCHWT1());
	support_message("RDRAND", InstructionSet::RDRAND());
	support_message("RDSEED", InstructionSet::RDSEED());
	support_message("RDTSCP", InstructionSet::RDTSCP());
	support_message("RTM", InstructionSet::RTM());
	support_message("SEP", InstructionSet::SEP());
	support_message("SHA", InstructionSet::SHA());
	support_message("SSE", InstructionSet::SSE());
	support_message("SSE2", InstructionSet::SSE2());
	support_message("SSE3", InstructionSet::SSE3());
	support_message("SSE4.1", InstructionSet::SSE41());
	support_message("SSE4.2", InstructionSet::SSE42());
	support_message("SSE4a", InstructionSet::SSE4a());
	support_message("SSSE3", InstructionSet::SSSE3());
	support_message("SYSCALL", InstructionSet::SYSCALL());
	support_message("TBM", InstructionSet::TBM());
	support_message("XOP", InstructionSet::XOP());
	support_message("XSAVE", InstructionSet::XSAVE());
	return 1;
}

void FNN::updateLoss()
{
	Sleep(100);
	qLineLoss->clear();
	qLineLossDiff->clear();
	int currentIter = 0;
	float maxvalue = 0.0;
	float minvalue = 99999;
	bool exiet = false;
	float diff = 0;
	while (!exiet)
	{
		if (MyCNNModel.getTraingFlag() == false)
		{
			exiet=true;
		}
		Sleep(50);
		if (MyCNNModel.vloss.size() > currentIter)
		{		
			for (int i = currentIter;i < MyCNNModel.vloss.size();i++)
			{
				qLineLoss->append(QPointF(i+1.0, MyCNNModel.vloss.at(i)));
				if (i > 0)
				{
					diff = std::log(std::fabs(MyCNNModel.vloss.at(i) - MyCNNModel.vloss.at(i-1)));
					qLineLossDiff->append(QPointF(i + 1.0, diff));
				}
				
				if (MyCNNModel.vloss.at(i) > maxvalue) maxvalue = MyCNNModel.vloss.at(i);
				if (diff < minvalue) minvalue = diff;
			}
			qChart1Loss->chart()->axes().at(1)->setRange(0, maxvalue * 1.1);
			qChart1Loss->chart()->axes().at(2)->setRange(minvalue*1.0, 0);
			qChart1Loss->chart()->axisX()->setRange(1, currentIter);
			currentIter = MyCNNModel.vloss.size();
			
		}
	}
}