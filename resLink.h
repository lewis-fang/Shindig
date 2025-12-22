#pragma once
#include"DataDefine.h"

class reslink
{
public:
	reslink() ;
	~reslink() {};
	void linkPreImage(image preImage);
	void setResLink(int pre, trpDim covDim, int resCovChannel = -1, int preChannel = -1, float sd=1.0);
	bool resAdd(image bzImage, int b) ;
	void setResInput(image resPre);
	int getPreLayerNumth() { return preLayerNumth; }
	void freeSpace();
private:
	int preLayerNumth;//1,2,3...
//	int postLayerNumth;

	kernal* resCovKernal;
	int resCovChannel;
	//image resOutput;//bzactimage=bzactimage+resInput

	bool resLink;
	image covImage;
	image resInput;//outImage (poolingImage)

};