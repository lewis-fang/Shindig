#pragma once

#include"CNNLayer.h"
#include<vector>
#include<iomanip>
#include<thread>
#include <windows.h>
#include<mutex>
#include<condition_variable>
#include<atomic>
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

//
	void setMaxiters(double maxiters){ maxIteration = maxiters; }
	void setMinloss(double ml){ minLoss = ml; }
	void setDiffLoss(double dfls){ diffLoss = dfls; }
	void setLearnRate(float lr){ learnRate = lr; }
	void setNormMethod(int normod, double range, double shift);
	void setBatchSize(int bz) { batchSize = bz; }
	///////////////////training: backward propagation
	bool addInputImage(image im, float* theOutput, int outLenth);
	bool startTrainning();
	void clearInputImage() ;
	int getImageNum() { return InputImageSeries.size(); }
	image getImage(int i) {	return InputImageSeries.at(i);
	}
	//image normliaze(const image iniputImage);
	image normliaze(const image iniputImage);

	std::vector<float> vloss;

	
	bool LaunchCNNModelBySimd(image image0, float* outValues, int outLentgh);
	bool startTrainningSimd();
	bool startTrainningSimdV2();

	image normliazeSimd(const image iniputImage);
	bool VectorToImgSimd(float* vec, int sz, image& outdxdy);

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
	

};