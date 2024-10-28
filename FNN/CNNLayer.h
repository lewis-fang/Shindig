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
class CNNCalc
{
public:
	CNNCalc();
	~CNNCalc();
	//init current Layer
	void initLayerMemory(int inRows, int inCols, int inChannel);
	void initKernals(int wtsRow, int wtsCol, int wtsChannel, int ns,int strd,float sd,float bs);
	void SetInput(image imago);//must set
	bool SetKernals(std::vector<kernal> kernals, int kernalrow, int kernalcol, int padsz, int strd, int ns);// must set
	void SetPoolings(int pldim1, int pldim2, int plStrd, int plFun);//can be set
	void SetActivateFun(int Act);//can be set
	void setlayerType(layerType lt){ thisLayerType = lt; }
	void setPaddingMethod(PaddingMethod pm){ padMoethod = pm; }
	void setHiddenNum(int hn){ HideLayerNumth = hn; }
	bool setOutLossss(image umg);
	void resetShadowKernal();
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

	bool LaunchConvolutionBySimd();
	void SetInputSimd(image imago);
	bool UpdateLayerLossSimd(image& retImage);
	image& getCurrentLayerIdealOutDxdy() { return BL.dIdealOutVSdO; }
	bool setOutLossssSimd(image umg);
	void resetShadowKernalSimd();
	void UpdateLayerWBSimd();
	void AccumulateDWSimd(float learnrate, int bs);
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
	void setOutBuffer();//prior to the convolution step
	void setOutBuffer2();

	void paddingSimd();
	void poolingSimdV2();
	void ConvolutionSimd();
	void ConvolutionSimdFullyConv();


	size_t cmplementSize;
	size_t cmplementSizePoolImge;
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

		kernal* shadowKernals;
	public:
			int lossType;//0:1/2*||y-x||,,1: cross Entropy
			image dIdealOutVSdO;
			image dactImage;
			image dbzImage;
			image dactImageW;
			image dbzImageW;

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
			bool dConvolutionXSimd(image inPa, image outZ, image bzactImage, kernal* K180, int Kn, int stride, image& dImage);
			bool dFullyConvolutionXSimd(image inPa, image outZ, image bzactImage, kernal* kernalSeries, int Kn, image& dImage);
			bool dPoolingSimd(image actImage, image DyDxoutImage, int poolingdim1, int poolingdim2, int poolingstride, image& dPoolingImage);
			bool dConvolutionWSimd(image inPa, image outZ, image bzactImage, kernal*& dkernal, int Kn, int stride);
			void innerPaddingSimd(image beforePaddingZ, image& AfterPaddingZ, int innerSizeRow, int innerSizeCol);
			__m256 _mm256_exp_ps_ft(__m256 a);
			__m128 _mm_exp_ps_ft(__m128 a);
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

