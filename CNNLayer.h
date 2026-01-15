#pragma once
#include<iostream>
#include<vector>
#include<fstream>
#include<ctime>
#include"immintrin.h"
#include<intrin.h>
#include<iomanip>
#include<getCpuId.h>
#include"DataDefine.h"
#include"resLink.h"
#include"batchnorm.h"
#include<thread>

#include <mutex>
#include <condition_variable>

class CyclicBarrier 
{
private:
	mutable std::mutex tx;
	std::condition_variable cv;
	volatile int count;
	int threshold;
	volatile int generation;
	volatile int release;
public:

	CyclicBarrier():count(0), threshold(0), generation(0), release(0){};
	CyclicBarrier(const CyclicBarrier&){};
	~CyclicBarrier() {};
	void initBarrier(int bs) { count = bs; threshold = bs;generation = 0;release = 0; };
	void wait()
	{
		std::unique_lock<std::mutex> lock(tx);
		auto current_gen = generation;
		if (--count == 0)
		{
			generation++;
			count = threshold;
			cv.notify_all();
			lock.unlock();
		}
		else
		{
			while (generation == current_gen)
			{
				cv.wait(lock);
			}
			release++;
			return;
		}
		while (release < threshold - 1) {};
		release = 0;
	};
};
#define SMALLZERO 1E-8
class CNNCalc
{
public:

	CNNCalc();
	~CNNCalc();
	//init current Layer
	void initLayerMemory(int inRows, int inCols, int inChannel);
	void initLayerMemoryV2(int batchSize);
	void initLayerSize(int inRows, int inCols, int inChannel);
	void freeLayerMemory();
	void initKernals(int wtsRow, int wtsCol, int wtsChannel, int ns,int strd,float sd,float bs);
	void SetInput(image imago);//must set
	bool SetKernals(std::vector<kernal> kernals, int kernalrow, int kernalcol, int padsz, int strd, int ns);// must set
	void SetPoolings(int pldim1, int pldim2, int plStrd, int plFun);//can be set
	void SetActivateFun(int Act);//can be set
	void setlayerType(layerType lt){ thisLayerType = lt; }
	void setPaddingMethod(PaddingMethod pm){ padMoethod = pm; }
	void setHiddenNum(int hn){ HideLayerNumth = hn; }
	bool setOutLossss(image umg);

	void UpdateLayerWB();
	void AccumulateDW(float learnrate, int bs);
	bool UpdateLayerLoss(image& retImage);
	bool LaunchConvolution();

	int GetOutLength(){ return neuroNums; }//outlen is same to number of neuros
	int getHiddenNum(){ return HideLayerNumth; }
	layerType getCurrentLayerType(){ return thisLayerType; }
	int getKernalNum(){ return neuroNums; }
	kernal getFirstKernal() {return CNNKernals[0];};
	kernal getiKernal(int i) { return CNNKernals[i]; };
	kernal* getAllKernals() { return CNNKernals; }

	 image getOutImage();
	 image getActImage(){ return actImage; }
	 image getInputImage(){ return inputImage; }
	 image getPaddingImage(){ return padImage; }
	int getActFun() { return activateType; }
	int getBNPos() { return myBatchNorm.getPos(); }
	bool LaunchConvolutionBySimd(int b=0);
	void SetInputSimd(image imago,int b=0);
	bool UpdateLayerLossSimd(image& retImage, int b=0);
	bool UpdateLayerLossSimd( int b = 0);
	image& getCurrentLayerIdealOutDxdy() { return BL.dIdealOutVSdO; }
	image& getCurrentLayerIdealInoutDxdy() { return BL.dIdealInoutVSdO; }
	bool setOutLossssSimd(image umg,int b=0);
	void UpdateLayerWBSimd(float learnrate, float l2Lamda,  int bs);
	void AccumulateDWSimd(float learnrate, int bs,int b=0);

	void addDKernalShadow(int b=0);
	void UpdateLayerWBSGDM(float learnrate, float beta1, float l2Lamda,int t);
	void UpdateLayerWBSGDMW(float learnrate, float beta1, float l2Lamda, int t);
	void UpdateLayerWBSGNAD(float learnrate, float beta1, float l2Lamda,int t);
	void UpdateLayerWBADAM(float learnrate, float beta1, float beta2, float sigma, float l2Lamda, int t);
	void UpdateLayerWBADAMW(float learnrate, float beta1, float beta2, float sigma, float l2Lamda,int t);
	void SetInputSimdV2(image imago, int b);
	void setIdealOut(float* goodOut, int length,int b) { memcpy(BL.IdealOut+outImage.blockSize*b, goodOut, length*sizeof(float)); }

//	void initBatchNorm(int len, int bnp, float sv, int batchsize);
	void setBatchNorm(int len, int bnp, float sv);
	void setReslink(int pre) { myResLink.setResLink(pre); }
	void linkPreImageRes(image resImage);
	bool LaunchConvolutionBySimdBN(int trainOrInference, int b);
	int getResLink() { return myResLink.getPreLayerNumth(); }
	image getDbzActImage() { return BL.dbzImageBe4pad; }
	void updateBNParas(image preImage);
	void layerBNCalc(image preImage, int toi,int b);
	void initBNParas();
private:
	size_t HideLayerNumth;
	image inputImage;
	size_t paddingrow;
	size_t paddingcol;
	image padImage;

	size_t stride;
	size_t neuroNums;//out channel
	size_t kernalRows;
	size_t kernalCols;
	size_t kernalChannel; //kernal
	
	kernal* CNNKernals;
	kernal* CNNKernalsRotate180;
	kernal* CNNKernalsRotate180Simd;
	kernal* dkernal;
	std::vector<kernal*> dkernalSeries;

	image bzactImage;
	image actImage;//buffers before  pooling
	image dpoolingIMG;
	size_t poolingRow;
	size_t poolingCol;
	size_t poolingStride;
	int activateType;
	int poolingFun;    //pooling	

	image outImage;//=pooling image


	PaddingMethod padMoethod;
	layerType thisLayerType;

	bool isSetData;
	bool isSetConfig;
	bool isBufferInitiated;
	//calculate order

	void padding();   //2nd
	void Convolution();  //3rd
	
	inline float activate(float a, int fun);

	void pooling();   //4th
	void setOutBuffer(int batchSize=1);//prior to the convolution step
	void setOutBuffer2(int batchSize = 1);

	void paddingSimd(int b=0);
	void poolingSimdV2(int b = 0);
	void ConvolutionSimd(int b = 0);
	void ConvolutionSimdFullyConv(int b = 0);
	bool activateOperateSimd(int b = 0);
	inline __m256 activateSingleSimd(__m256 a, int fun);

	/// <BatchNorm>
	batchnorm myBatchNorm;
	/// </BatchNorm>
	reslink myResLink;
	size_t cmplementSize;
	size_t cmplementSizePoolImge;

	CyclicBarrier barrier;
class BackLayer
	{
	public:
		BackLayer();
		~BackLayer(){};
		void setIdealOut(image img){ dIdealOutVSdO = img; }
		void setActType(int Act) { acttype = Act; }
		__m256  dactivateSimd(__m256 a, int fun);
		__m128  dactivateSimd128(__m128 a, int fun);
		
		float dactivate(float a, int fun);

		kernal* ShadowMoment;
		kernal* ShadowVelocity;
	public:
			int lossType;//0:1/2*||y-x||,,1: cross Entropy
			image dIdealInoutVSdO;
			image dIdealOutVSdO;
			float* IdealOut;
			float* VBias;

			image dactImage;
			image dbzImage;
			image dactImageW;
			image dbzImageW;
			image dbzImageBe4pad;

			float learnRate;
			float* Loss;
			int acttype;
			kernal aKernal;

			void initDKernals() {};
			bool dConvolutionX(image inPa, image outZ, image bzactImage, kernal* K180, int Kn, int stride, image& dImage);
			bool dConvolutionW(image inPa, image outZ, image bzactImage,kernal*& dkernal, int Kn,int stride);
			bool dPooling(image actImage,image DyDxoutImage,int poolingdim1,int poolingdim2,int poolingstride, image &dPoolingImage);
			void dPadding(image BeforePaddingZ, image& AfterPaddingZ, int padSizeR, int padSizeC);
			
			bool TMatrixKernal(const kernal* kernalSeris, kernal* retKernal180, int neuroNums);
			void innerPadding(image beforePaddingZ, image& AfterPaddingZ ,int innerSizeRow, int innerSizeCol);
			bool outPadding(image beforePaddingZ, image& AfterPaddingZ, int targetRow, int targetCol);
			
			//simd version function
			bool TMatrixKernalSimd(const kernal* kernalSeris, kernal* retKernal180, int neuroNums);
			bool outPaddingSimd(image beforePaddingZ, image& AfterPaddingZ, int targetRow, int targetCol);
			bool dActMultidLdy(image dLdy, image dbzactImageBe4Pad, image& outZ,int b = 0);

			bool dConvolutionXSimd(image inPa, image dbzactImageBe4Pad, kernal* K180, int Kn, int stride, image& dImage, int b=0);
			bool dFullyConvolutionXSimd(image inPa, image outZ, image dbzactImageBe4Pad, kernal* kernalSeries, int Kn, image& dImage,int b = 0);

			bool dPoolingSimd(image actImage, image DyDxoutImage, int poolingdim1, int poolingdim2, int poolingstride, image& dPoolingImage, int b = 0);
			bool dConvolutionWSimd(image inPa, image dbzactImageBe4Pad, kernal*& dkernal, int Kn, int stride, int b = 0);
			void innerPaddingSimd(image beforePaddingZ, image& AfterPaddingZ, int innerSizeRow, int innerSizeCol, int b = 0);
			__m256 _mm256_exp_ps_ft(__m256 a);
			__m128 _mm_exp_ps_ft(__m128 a);
			bool dActivateOperateSimd(image bzactImage, image actImage, int b=0);


		//	bool addResLinkDbzImage(image resDbzactImage, int b);
	}BL;
};

/*
Forward:                           :   class CNNCalc;
	convolution layer:
		1:input->padding
		2:padding->convolution
		3:convolution->activation
		4:activation->pooling (3&4 are combined into one step)
		5:pooling->output
	fullyconnection layer:
		1:input->padding
		2:padding->convolution
		3:convolution->activation	
Backward:                          :   class CNNCalc::BackLayer;
	d convolution layer:
		1: C -> P(L+1) : Delta(L+1)
		2: P(L+1) -> A(L)
		3: A(L) -> Z : dactivate         :   double dactivate(...);
		4: Z -> A(L)   : W(Rot180) Conv Z  :   bool dConvolutionX(...);   step 2 is included
		5: Z -> W(L)   : Z Conv A(L)       :   bool dConvolutionW(...);   step 2 is included
		C->W(L)=C->P(L+1) * P(L+1)->W(L)
				=C->P(L+1) * P(L+1)->A(L) * A(L)->W(L)
				=C->P(L+1) * P(L+1)->A(L) * A(L)->Z * Z->W(L)
				=1 * 2 * 3 * 5
				=Delta(L+1) * 2(L) * 3(L) * 5(L)
		Delta(L+1)=C->P(L+1)
					=C->P(L+2) * P(L+2)->P(L+1) 
					=C->A(L+2) * P(L+2)->A(L+1) * A(L)->P(L+1)
					=C->A(L+2) * P(L+2)->A(L+1) * A(L)->Z * Z->P(L+1)
					=Delta(L+2) * 2(L+1) * 3(L+1) * 4(L+1)
	d fullyconnection layer:
		1: C -> A(L+1) : Delta(L+1)
		2: 
		3: A(L+1) -> Z : dactivate         :   double dactivate(...);
		4: Z -> A(L)   : W(Rot180) Conv Z  :   bool dConvolutionX(...);   step 2 is included
		5: Z -> W(L)   : Z Conv A(L)       :   bool dConvolutionW(...);   step 2 is included
		C->W(L)=C->A(L+1) * A(L+1)->W(L)
				=C->A(L+1) * A(L+1)->Z * Z->W(L)
				=1 * 2 * 4
				=Delta(L+1) * 2(L) * 4(L)
				Delta(L+1)=C->A(L+1)
				=C->A(L+2) * A(L+2)->A(L+1)
				=C->A(L+2) * A(L+2)->Z * Z->A(L+1)
				=Delta(L+2) * 3(L+1) * 5(L+1)
		Delta(L+1)=C->A(L+1)
				=C->A(L+2) * A(L+2)->A(L+1)
				=C->A(L+2) * P(L+2)->Z * Z->A(L+1)
				=Delta(L+2) * 3(L+1) * 4(L+1)
*/

