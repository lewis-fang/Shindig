#pragma once

#include"CNNLayer.h"
#include<vector>
#include<iomanip>
#include<thread>
#include <windows.h>
#include<mutex>
#include<condition_variable>
#include<atomic>

enum OptiMethod
{
	NooP=0,
	SGD,
	SGDM,
	SGDMW,
	SGNAD,
	ADAM,
	ADAMW
};
typedef struct Optimizer
{
	OptiMethod method;
	float beta1;
	float beta2;
	float sigma;
	Optimizer()
	{
		beta1 = 0.9;
		beta2 = 0.99;
		sigma = 0.000001;
		method = NooP;
	}
}Optimizer;
class CNNModel
{
public:
	CNNModel();
	~CNNModel(){};

	bool LaunchCNNModel(image image0, float* outValues, int outLentgh);
	void addCNNLayer(const CNNCalc& aLayer);

	void popLayer();
	void clearModel();


	int getLastLayerOutChannel();
	int getLastLayerNum(){ return CNNLayerSeries.size(); }
	void getLastLayerOutSize(int& dim1, int& dim2);
	CNNCalc getLayer(int layerIndex);
	layerType getLastLayerType();

	bool getTraingFlag() { return traingFlag; }
	void softMax(double& x1, double& x2){};

	void setMaxiters(double maxiters){ maxIteration = maxiters; }
	void setMinloss(double ml){ minLoss = ml; }
	void setDiffLoss(double dfls){ diffLoss = dfls; }
	void setLearnRate(float lr){ learnRate = lr; }
	void setNormMethod(int normod, double range, double shift);
	void setBatchSize(int bz) { batchSize = bz; }
	void setL2Lamda(float l2Lamda) { L2Lamda = l2Lamda; }

	void setOptimizer(Optimizer mo) { memcpy(&mOptimizer, &mo, sizeof(Optimizer)); }
	///////////////////training: backward propagation
	bool addInputImage(image im, float* theOutput, int outLenth);
	bool startTrainning();
	void clearInputImage() ;
	int getImageNum() { return InputImageSeries.size(); }
	image getImage(int i) {	return InputImageSeries.at(i);
	}
	image normliaze(const image iniputImage);

	std::vector<float> vloss;

	bool LaunchCNNModelBySimd(image image0, float* outValues, int outLentgh,int b=0);
	bool LaunchCNNModelBySimdNonParrallel(std::vector<image> imagen, float* outValues, int outLentgh);
	bool startTrainningSimd();
	bool startTrainningSimdV2();
	bool startTrainningSimdV3();
	bool LaunchCNNModelParrallel(std::vector<image> imagen, float* outValues, int outLentgh, int batch);

	image normliazeSimd(const image iniputImage);
	bool VectorToImgSimd(float* vec, int sz, image& outdxdy);
	void initMemory(int batch);
	void freeMemory();
	bool saveModel(char* path, bool isSimdTrained);
	bool readModel(char* path, bool isSimdTrained);

	bool createThread(int useSimd);
	void layerUpdatedKernal(int layerIndex);
	void avb(int a);
private:
	std::vector<CNNCalc> CNNLayerSeries;


	/////////////////trainigp configs
	int maxIteration;
	double minLoss;
	double diffLoss;
	float learnRate;
	float L2Lamda;
	int normMethod;
	/*
	0: none
	1: global range and a shift
	2: normalized to specified range and shift
	*/
	double normGlobalValueOrRange;
	double normShift;

	void globalNorm( image& iniputImage);
	void rangeorm(image& iniputImage);
	///////////////////////////
	
	std::vector<image> InputImageSeries;
	std::vector<float*> IdealOutput;

	Optimizer mOptimizer;

	int outLen;
	int batchSize;
	double C;	
	int lossType;
	inline float calculateC(float* y, float*x, int sz);
	template< typename  T>
	inline void dcalculateC(T* y, T*x, T* dyVdx, int sz);
	
	std::atomic<int> ato;
	std::condition_variable cond;
	bool traingFlag;
	std::mutex myMutexA;
	///////////fundenmental functions
	image VectorToImg(float*vec, int sz);
	int layerUpdatedKernalDone;
	//void threadTrain(int im,int b, image inputBatch);
	std::condition_variable condB;
	std::mutex myMutexB;
	volatile int parrellelIBatchCalDone;

	std::condition_variable condC;
	std::mutex myMutexC;
	volatile int parrellelIBatchTrainDone;

	void LaunchCNNModelBySimdBatch(image image0, float* outValues, int outLentgh, int batch, int b);
	void updateLossParrallel(image bImage,float* outValue,float* vC,int outLen,int realIndex,int b);
	bool LaunchCNNModelBySimdV2(image image0, float* outValues, int outLentgh, int b);
	//void updateWeightParrallel(int b);
};