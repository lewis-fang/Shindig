#include"resLink.h"

reslink::reslink() 
{
	preLayerNumth = -1;//0,1,2,3...
	//postLayerNumth = -1;
	resLink = false;
	resCovKernal = NULL;
	resCovChannel = -1;
}

void reslink::linkPreImage(image preImage)
{
	memcpy(&resInput, &preImage, sizeof(image));
}
void reslink::setResInput(image resPre)
{
	
}
bool reslink::resAdd(image resImage, int b)
{
	if (resLink)
	{
		float* bzactImageData=resImage.imageAtIndex(b);
		float* resImageData = resInput.imageAtIndex(b);
		int offset = AlignBytes / sizeof(float);
		for (int i = 0;i < resImage.blockSize;i += offset)
		{
			__m256 reg1 = _mm256_load_ps(bzactImageData + i);
			__m256 reg2 = _mm256_load_ps(resImageData + i);
			reg1 = _mm256_add_ps(reg1, reg2);
			_mm256_stream_ps(bzactImageData + i, reg1);
		}
	}
	return true;
}

void reslink::setResLink(int pre, trpDim covDim, int resConCh,int preChannel,float sd)
{ 
	preLayerNumth = pre;
	resLink = true; 
	if (resCovChannel>0 && preChannel>0 && covDim.channel== resCovChannel)
	{
		resCovChannel = resConCh;
		resCovKernal = new kernal[resCovChannel];
		for (int k = 0;k < resCovChannel;k++)
		{
			resCovKernal[k].col = 1;
			resCovKernal[k].row = 1;
			resCovKernal[k].channel = preChannel;
			resCovKernal[k].initKernal(sd);
		}
		covImage.cols = covDim.col;
		covImage.rows = covDim.row;
		covImage.channel = resCovChannel;
		covImage.initImage();
	}
}
void reslink::freeSpace()
{
	if (resCovKernal != NULL && resCovChannel>0)
	{
		for (int k = 0;k < resCovChannel;k++)
		{
			resCovKernal[k].FreeWtsSpace();
		}
		covImage.freeImageSpace();
	}
}
