#pragma once
#include"dataDefine.h"
enum TrainOrInference
{
	train=1,
	inference=2
};
enum  BNPosition
{
	BNUnkown = 0,
	Linear_Act = 1,
	Act_Pooling = 2
};
class batchnorm
{
public:
	batchnorm();
	~batchnorm() {};
	void linkImage(const image preImage);
	void setBatchNorm(int len, BNPosition bnp, float sv);

	void initParas();
	void initSpace(int bs);
	void deleteSpace();
	void preCalcParas();
	bool updateBNParas(image preImage);
	bool calcBN(TrainOrInference trainOrInference, image preImage, int b);//use initial paras
	bool calcBatchNormTrain(image preImage, int b);//use initial paras
	bool calcBatchNormInference(image preImage, int b);//use precaled paras
	//image getOutImage() { return batchnormOutImage; }
	BNPosition getPos() { return BNPos; }

	bool derivativeDLDx(image dLdy, image& dLdx, int b);
	bool derivativeDLDw(image dLdy, int b);
	bool accumulateDW(int b);
	bool mysweetfc(float learnrate, float beta1, float beta2, float sigma, float l2Lamda, int t);
	bool adam(float learnrate, float beta1, float beta2, float sigma, float l2Lamda, int t, kernal g, kernal m, kernal v,kernal w);
private:
	image batchnormInputImage;
	float* mu;
	float* sigma;
	kernal gamma;
	kernal beta;

	float* mavmu;
	float* mavsigma;
	//	image batchnormOutImage;
	int featureLen;
	float smallvalue;
	float alpha;
	BNPosition BNPos;
	/*
	* 0: pre activate  BN+res+activate
	* 1: post activate  res+activate
	*/
	float* paraw;
	float* parab;


	bool utest() {};

	int batchSize;

	std::vector<kernal> dg;
	std::vector<kernal> db;

	kernal shadowGM;
	kernal shadowGV;

	kernal shadowBM;
	kernal shadowBV;

	kernal tmp1;
	kernal tmp2;
};
///dd
/*
* N=bs*row* col
* u=sum(x)/N   dim=channel
* s=sum(x*x)-sum(x)*sum(x)
* xx=(x-u)/sqrt(s+sv)
* y=xx*g+b
* dL/dxi=sum(dL/dyj*dyj/dxxj * dxxj/dxi)=sum(dL/dyj *gamma *dxxj/dxi )=gamma*sum(dL/dyj * dxxj/dxi)
*				i=j:			=(1-1/N)*sqrt(s+sv)-(xj-u)*(-1/2)/sqrt(s+sv)*2*(xi-u)/N      ///(s+sv)
 *				i!=j:			=-1/N*sqrt(s+sv)-(xj-u)*(-1/2)/sqrt(s+sv)*2*(xi-u)/N           ///(s+sv)
*                            P=sqrt(s+sv)
*						i=j		=(1-1/N)*P+(xj-u)/P/N     ///(PP)
*						i!=j       =-1/N*P+		(xj-u)/P/N   ///(PP)
* 
*                      =dL/dyi*P///PP-1/N*P*sum(dL/dyj)///(PP)   +sum(dL/dyj *(xi-u))/P/N ///PP
*						=dL/dyi*P///PP-sum(dL/dyj)/N///(P)   +sum(dL/dyj *(xi-u))/N ///PPP
*						=dL / dyi // P - sum(dL / dyj) / N///(P)  (1-  (xi-u))/N ///PP)
* 
* 
* dL/gc=sum(dL/dyj * dyi/dg)=sum(dL/dyj * xxj)
* dL/db=sum(dL/dyj * dyi/db)=sum(dL/dyj)
* */