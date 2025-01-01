#include"CNNModel.h"

CNNModel::CNNModel()
{
	C = 0;
	maxIteration = 1000;
	minLoss = 0.01;
	learnRate = 0.01;
	diffLoss = 0.001;
	lossType = 1;
	batchSize = 1;
	normMethod = 0;
	L2Lamda = 0;
	traingFlag = false;
}
void CNNModel::avb(int a)
{
	std::cout << batchSize << std::endl;
}
bool CNNModel::createThread(int useSimd)
{
	if (traingFlag == false)
	{
		traingFlag=true;
		if (useSimd == 0)
		{
			std::thread thr(&CNNModel::startTrainning, this);
			thr.detach();
		}
		else if (useSimd == 1)
		{
			std::thread thr(&CNNModel::startTrainningSimd, this);
			thr.detach();
		}
		else if (useSimd == 2)
		{
			std::thread thr(&CNNModel::startTrainningSimdV2, this);
			thr.detach();
		}
		else if (useSimd == 3)
		{
			std::thread thr(&CNNModel::startTrainningSimdV3, this);
			thr.detach();
		}
		else
		{
			std::cout << "this version has not been realized!" << std::endl;
			return false;
		}
	}
	else
	{
		std::cout << "a traing thread is on" << std::endl;
		return false;
	}
	return true;
}
void CNNModel::setNormMethod(int normod, double range, double shift)
{
	normMethod = normod;
	normGlobalValueOrRange = range;
	normShift = shift;
}
bool CNNModel::LaunchCNNModel(image image0, float* outValues, int outLentgh)
{
	bool ret = true;
	if (CNNLayerSeries.size() == 0)
	{
		ret = false;
	}
	image tempIamge = image0;	
	for (CNNCalc layer : CNNLayerSeries)
	{
		layer.SetInput(tempIamge);
		if (layer.LaunchConvolution())
		{
			tempIamge = layer.getOutImage();
		}
		else
		{
			ret = false;
			break;
		}
	}
	ret &= outValues != NULL;
	if (ret)
	{
		for (int ch = 0; ch < tempIamge.channel; ch++)
		{
			outValues[ch] = tempIamge.imageData[ch][0][0];
		}
	}

	return ret;
}
bool CNNModel::LaunchCNNModelBySimdNonParrallel(std::vector<image> imagen, float* outValues, int outLentgh)
{
	
	bool ret = true;
	for (image image0 : imagen)
	{
		bool ret = LaunchCNNModelBySimd(image0, outValues, outLentgh);
	}
	return ret;
}
bool CNNModel::LaunchCNNModelBySimd(image image0, float* outValues, int outLentgh,int b)
{
	bool ret = true;
	if (CNNLayerSeries.size() == 0)
	{
		ret = false;
	}
	image tempIamge = image0;
	for (CNNCalc layer : CNNLayerSeries)
	{	
		layer.SetInputSimd(tempIamge,b);
		if (layer.LaunchConvolutionBySimd(b))
		{
			tempIamge = layer.getOutImage();
			
		}
		else
		{
			ret = false;
			break;
		}
	}
	ret &= outValues != NULL;
	if (ret)
	{
		memcpy(outValues + b * outLentgh, tempIamge.imageAtIndex(b), outLentgh * sizeof(float));
	}
	return ret;
}
bool CNNModel::LaunchCNNModelBySimdV2(image image0, float* outValues, int outLentgh, int b)
{
	bool ret = true;
	if (CNNLayerSeries.size() == 0)
	{
		ret = false;
	}
	CNNCalc& layer0 = CNNLayerSeries.at(0);
	layer0.SetInputSimdV2(image0, b);
	image tempIamge;
	int depth = CNNLayerSeries.size();
	for (int ly=0;ly< depth;ly++)
	{
		CNNCalc& layer = CNNLayerSeries.at(ly);
		if (layer.LaunchConvolutionBySimd(b))
		{
			tempIamge = layer.getOutImage();
			if (ly != (depth - 1))
			{
				CNNCalc& nextlayer = CNNLayerSeries.at(ly+1);			
				nextlayer.SetInputSimd(tempIamge, b);
			}
		}
		else
		{
			ret = false;
			break;
		}
	}
	ret &= outValues != NULL;
	if (ret)
	{
		memcpy(outValues + b * outLentgh, tempIamge.imageAtIndex(b), outLentgh * sizeof(float));
	}
	return ret;
}
void CNNModel::initMemory(int batch)
{
	for (CNNCalc& layer : CNNLayerSeries)
	{
		layer.initLayerMemoryV2(batch);
	}

}void CNNModel::freeMemory()
{
	for (CNNCalc& layer : CNNLayerSeries)
	{
		layer.freeLayerMemory();
	}
}
bool CNNModel::LaunchCNNModelParrallel(std::vector<image> imagen, float* outValues, int outLentgh, int batch)
{

	std::thread* thr = new std::thread[batch];
	for (image image0 : imagen)
	{
		parrellelIBatchCalDone = 0;
		for (int b = 0;b < batch;b++)
		{
			thr[b] = std::thread(&CNNModel::LaunchCNNModelBySimdBatch, this, image0, outValues, outLentgh, batch, b);
			thr[b].detach();
		}
		std::unique_lock<std::mutex> unique(myMutexB);
		while (parrellelIBatchCalDone < batch)
		{
			condB.wait(unique);
		}
	}
	return true;
}
void CNNModel::LaunchCNNModelBySimdBatch(image image0,float* outValues,int outLentgh, int batch, int b)
{
	bool ret = true;
	if (CNNLayerSeries.size() == 0)
	{
		ret = false;
	}
	image tempIamge = image0;
	for (CNNCalc layer : CNNLayerSeries)
	{
		layer.SetInputSimd(tempIamge, b);
		if (layer.LaunchConvolutionBySimd(b))
		{
			tempIamge = layer.getOutImage();

		}
		else
		{
			ret = false;
			break;
		}
	}
	ret &= outValues != NULL;
	if (ret)
	{
		memcpy(outValues+b* outLentgh, tempIamge.imageAtIndex(b), outLentgh * sizeof(float));
	}
	std::unique_lock<std::mutex> unique(myMutexB);
	parrellelIBatchCalDone++;
	if (parrellelIBatchCalDone == batch)
	{
		condB.notify_one();
	}
	unique.unlock();
}
void CNNModel::addCNNLayer(const CNNCalc& aLayer)
{
	CNNLayerSeries.push_back(aLayer); 
}
void CNNModel::popLayer()
{
	if (CNNLayerSeries.size() > 0)
	{
		CNNLayerSeries.pop_back();
		if (CNNLayerSeries.size() == 0)
		{
			std::vector<float>().swap(vloss);
		}
	}

}
void CNNModel::clearModel()
{
	CNNLayerSeries.clear();
	vloss.clear();
}
int CNNModel::getLastLayerOutChannel()
{
	if (CNNLayerSeries.size() > 0)
	{
		return CNNLayerSeries.back().GetOutLength();
	}
	else
	{
		return 0;
	}
}
void CNNModel::getLastLayerOutSize(int& dim1, int& dim2)
{
	if (CNNLayerSeries.size() > 0)
	{
		dim1 = CNNLayerSeries.back().getOutImage().rows;
		dim2 = CNNLayerSeries.back().getOutImage().cols;
	}
	else
	{
		dim1 = 0;
		dim2 = 0;
	}
}
layerType CNNModel::getLastLayerType()
{
	if (CNNLayerSeries.size() > 0)
	{
		return CNNLayerSeries.back().getCurrentLayerType();
	}
	else
	{
		return CONVOLUTION;
	}
}
CNNCalc CNNModel::getLayer(int layerIndex)
{
	if ((layerIndex >= 0) & (layerIndex < CNNLayerSeries.size()))
	{
		return CNNLayerSeries.at(layerIndex);
	}
	else
	{
		CNNCalc aNewLayer;
		aNewLayer.setHiddenNum(-1);
		return aNewLayer;
	}
}
bool CNNModel::addInputImage(image im, float* theOutput, int outLenth)
{//chack size of img BY comparring to the first
	if ((InputImageSeries.size() > 0) & (outLen == outLenth))
	{
		image lastImage = InputImageSeries.back();
		bool colEqual = lastImage.cols == im.cols;
		bool channelEqual = lastImage.channel == im.channel;
		bool rowEqual = lastImage.rows == im.rows;
		if (colEqual & channelEqual & rowEqual)
		{
			InputImageSeries.push_back(im);
			IdealOutput.push_back(theOutput);
		}
		else
		{
			return false;
		}
	}
	else
	{
		InputImageSeries.push_back(im);
		IdealOutput.push_back(theOutput);
	}
	return true;
}
bool CNNModel::startTrainning()
{
	bool ret = true;
	if (CNNLayerSeries.size() > 0)
	{
		if ((CNNLayerSeries.back().getCurrentLayerType() != FULLYCONNECTION))
		{
			std::cout << "cnn model is not bulit properly!" << std::endl;
			ret = false;
		}
		outLen = CNNLayerSeries.back().GetOutLength();
	}
	else
	{
		std::cout << "cnn model is not bulit properly!" << std::endl;
		ret = false;
	}
	if ((InputImageSeries.size() == 0) || (IdealOutput.size()==0) || (outLen==0))
	{
		std::cout << "no image is input~" << std::endl;
		ret = false;
	}
	if (InputImageSeries.size() != IdealOutput.size())
	{
		std::cout << "size of input images is not equal to that of output" << std::endl;
		ret = false;
	}
	//size of each img has been check when added
	int NIMG = InputImageSeries.size();
	std::cout << "start traning~" << std::endl;
	if (ret == true)
	{	
		
		int iterK = 0;
		float C = 0.0, lastC = 0.0;
		int dpth = CNNLayerSeries.size();
		image* DeltaImageLayerSeries = new image[dpth];
		for (int i = 0; i < dpth; i++)
		{
			DeltaImageLayerSeries[i].rows = CNNLayerSeries[i].getOutImage().rows;
			DeltaImageLayerSeries[i].cols = CNNLayerSeries[i].getOutImage().cols;
			DeltaImageLayerSeries[i].channel = CNNLayerSeries[i].getOutImage().channel;
			DeltaImageLayerSeries[i].initImage(1);
		}
		std::vector<image> normImageSeries;
		for (image img: InputImageSeries)
		{
			normImageSeries.push_back(normliaze(img));
		}
		float* outValue = new float[outLen];
		
		while (iterK<maxIteration)
		{
			clock_t st = clock();
			C = 0.0;
			std::cout << "------------------Iteration" << 1+iterK<<"------------------" << std::endl;
			for (int im = 0; im < NIMG; im+=batchSize)
			{
				for (int i = 0; i < dpth; i++)
				{
					DeltaImageLayerSeries[i].apllyratio(0.0);
				}
				for (int b = 0; b < batchSize; b++)
				{
					image currentIMG = normImageSeries.at(im+b);
					ret = LaunchCNNModel(currentIMG, outValue, outLen);	
					C += calculateC(outValue, IdealOutput.at(im + b), outLen);
					if (ret == true)
					{						
						float* dyVdx = new float[outLen];
						dcalculateC(IdealOutput.at(im + b), outValue, dyVdx, outLen);
						image outImage = VectorToImg(dyVdx, outLen);
						
						DeltaImageLayerSeries[dpth-1] += outImage;						
						CNNLayerSeries.back().setOutLossss(VectorToImg(dyVdx, outLen));
						outImage.freeImage();
						delete[] dyVdx;

						for (int ly = dpth-1; ly >= 0; ly--)
						{
							CNNCalc layer = CNNLayerSeries.at(ly);
							image DeltaImage;
							ret = layer.UpdateLayerLoss(DeltaImage);
							
							if (ret && (ly >0))
							{
								DeltaImageLayerSeries[ly-1] += DeltaImage;
								CNNLayerSeries.at(ly - 1).setOutLossss(DeltaImage);
							}
							else
							{
								break;
							}
							DeltaImage.freeImage();
						}
					}
					else
					{
						break;
					}
				}
			//	std::cout << "time to calculate out bia no-simd:" << double(clock() - st) << std::endl;
				if (ret == true)
				{
					for (int ly = CNNLayerSeries.size()-1; ly >= 0; ly--)
					{
						CNNCalc layer = CNNLayerSeries.at(ly);
						CNNLayerSeries.at(ly).setOutLossss(DeltaImageLayerSeries[ly]);
						for (int b = 0; b < batchSize; b++)
						{
							image currentIMG = normImageSeries.at(im + b);
							ret = LaunchCNNModel(currentIMG, outValue, outLen);
							layer.AccumulateDW(learnRate,batchSize);
							
						}
						
						layer.UpdateLayerWB();
					}
				}
				else
				{
					break;
				}
			//	std::cout << "time to update weights no-simd:" << double(clock() - st) << std::endl;
			}
			C = sqrt(C / NIMG) ;
			vloss.push_back(C);
			if ((C < minLoss) || (std::abs(C - lastC) < diffLoss))
			{
				break;
			}
			else
			{
				
				lastC = C;
			}
			iterK++;
			printf("iter: %d,C:%f\n", iterK,C);
			std::cout << "used time:"<<(double)(clock() - st) << std::endl;
		}
		delete[] outValue;
		printf("iter: %d,C:%f\n", iterK, C);
		for (int i = 0; i < dpth; i++)
		{
			DeltaImageLayerSeries[i].freeImage();
		}
		if (DeltaImageLayerSeries != NULL)
		{
			delete[] DeltaImageLayerSeries;
			DeltaImageLayerSeries = NULL;
		}
		
	}
	traingFlag = false;
	return ret;
}
/*
	* version, both forward simd version and propagation simd version
	* develop date 2024-07-14
	* test date 2024-07-28
*/
bool CNNModel::startTrainningSimd()
{

	bool ret = true;
	if ((InputImageSeries.size() == 0) || (IdealOutput.size() == 0) || (outLen == 0))
	{
		std::cout << "no image is input~" << std::endl;
		ret = false;
	}
	if (InputImageSeries.size() != IdealOutput.size())
	{
		std::cout << "size of input images is not equal to that of output" << std::endl;
		ret = false;
	}
	//size of each img has been check when added
	if (CNNLayerSeries.size() > 0)
	{
		if ((CNNLayerSeries.back().getCurrentLayerType() != FULLYCONNECTION))
		{
			std::cout << "cnn model is not bulit properly!" << std::endl;
			ret = false;
		}
		outLen = CNNLayerSeries.back().GetOutLength();
	}
	else
	{
		std::cout << "cnn model is not bulit properly!" << std::endl;
		ret = false;
	}
	int NIMG = InputImageSeries.size();
	std::cout << "start traning~" << std::endl;
	if (ret == true)
	{

		int iterK = 0;
		float C = 0.0, lastC = 0.0;
		int dpth = CNNLayerSeries.size();
		image* DeltaImageLayerSeries = new image[dpth];
		for (int i = 0; i < dpth; i++)
		{
			DeltaImageLayerSeries[i].rows = CNNLayerSeries[i].getOutImage().rows;
			DeltaImageLayerSeries[i].cols = CNNLayerSeries[i].getOutImage().cols;
			DeltaImageLayerSeries[i].channel = CNNLayerSeries[i].getOutImage().channel;
			DeltaImageLayerSeries[i].initImage(1);
		}
		std::vector<image> normImageSeries;
		for (image img : InputImageSeries)
		{
			normImageSeries.push_back(normliazeSimd(img));
		}
		float* outValue=(float*)_mm_malloc(sizeof(float)*outLen,AlignBytes);
		float* dyVdx = (float*)_mm_malloc(sizeof(float) * outLen, AlignBytes);
		while ( (iterK < maxIteration) && ret)
		{
			clock_t st = clock();
			C = 0.0;
			std::cout << "------------------Iteration SIMD " << iterK << "------------------" << std::endl;
			for (int im = 0; im < NIMG; im += batchSize)
			{
				clock_t st = clock();
				for (int i = 0; i < dpth; i++)
				{
					DeltaImageLayerSeries[i].apllyratioSimd(0.0);
				}
				for (int b = 0; b < batchSize; b++)
				{//batch size will be optimized by multithread
					image currentIMG = normImageSeries.at(im + b);
					/*
						* a forward-calculation to get all layer out of the model for current input image 
					*/
					//currentIMG.printImage(1, "NORMALIZED IMAGED");
					ret = LaunchCNNModelBySimd(currentIMG, outValue, outLen);
					C += calculateC(outValue, IdealOutput.at(im + b), outLen);
					if (C != C)
					{
						ret = false;
						std::cout << iterK << "," << im << std::endl;
					}
					if (ret == true)
					{					
						/*
							* get last layer's out bias vector
						*/
						dcalculateC(IdealOutput.at(im + b), outValue, dyVdx, outLen);		
						/*
							* convert out bias vector to a 1-1-outLen image
						*/
						CNNCalc layer = CNNLayerSeries.at(dpth - 1);
						image outDydx = layer.getCurrentLayerIdealOutDxdy();
						ret= VectorToImgSimd(dyVdx, outLen, outDydx);
						//outDydx.printImage(1, "outDydx");
						/*
							* sum out bias of every image for each layer
						*/
						DeltaImageLayerSeries[dpth - 1].addSimd(outDydx);
						for (int ly = dpth - 1; ly > 0; ly--)
						{
							CNNCalc layer = CNNLayerSeries.at(ly);
							image DeltaImage= CNNLayerSeries.at(ly - 1).getCurrentLayerIdealOutDxdy();							
							ret = layer.UpdateLayerLossSimd(DeltaImage);
							if (ret)
							{								
								DeltaImageLayerSeries[ly - 1].addSimd(DeltaImage);
							}
							else
							{
								std::cout << "UpdateLayerLoss fail" << std::endl;
								break;
							}
						}
					}
					else
					{
						break;
					}
				}
				bool iscalc = false;
				//std::cout << "period calculating out bias simd:" << (double)(clock() - st) << std::endl;
				if (ret == true)
				{
					for (int ly = CNNLayerSeries.size() - 1; ly >= 0; ly--)
					{
						
						DeltaImageLayerSeries[ly].apllyratioSimd(1.0 / batchSize);
						CNNLayerSeries[ly].setOutLossssSimd(DeltaImageLayerSeries[ly]);
					}
					for (int b = 0; b < batchSize; b++)
					{
						
						image currentIMG = normImageSeries.at(im + b);
						ret = LaunchCNNModelBySimd(currentIMG, outValue, outLen);			
						for (int ly = CNNLayerSeries.size() - 1; ly >= 0; ly--)
						{
							CNNCalc layer = CNNLayerSeries.at(ly);	
							layer.AccumulateDWSimd(learnRate, batchSize);	
							layer.addDKernalShadow(1);
						}
					}
					
					for (CNNCalc layer: CNNLayerSeries)
					{
						layer.UpdateLayerWBSimd(learnRate,L2Lamda,batchSize);
					}
				}
				else
				{
					break;
				}
				//std::cout << "period updating weights simd:" << (double)(clock() - st) << std::endl;
			}
			C = sqrt(C / NIMG) ;
			vloss.push_back(C);
			std::cout << "used time:" << (double)(clock() - st) << std::endl;
			if ((C < minLoss) || (std::abs(C - lastC) < diffLoss))
			{
				break;
			}
			else
			{

				lastC = C;
			}
			iterK++;
			printf("iter: %d,C:%f\n", iterK, C);
			
		}
		_mm_free(dyVdx);
		_mm_free(outValue);
		printf("End iter: %d,C:%f\n", iterK, C);
		for (int i = 0; i < dpth; i++)
		{
			DeltaImageLayerSeries[i].freeImage();
		}
		if (DeltaImageLayerSeries != NULL)
		{
			delete[] DeltaImageLayerSeries;
			DeltaImageLayerSeries = NULL;
		}
		printf("delete tmepimages\n");
	}
	traingFlag = false;
	return true;;
}

bool CNNModel::startTrainningSimdV2()
{

	bool ret = true;
	if (CNNLayerSeries.size() > 0)
	{
		if ((CNNLayerSeries.back().getCurrentLayerType() != FULLYCONNECTION))
		{
			std::cout << "cnn model is not bulit properly!" << std::endl;
			ret = false;
		}
		outLen = CNNLayerSeries.back().GetOutLength();
	}
	else
	{
		std::cout << "cnn model is not bulit properly!" << std::endl;
		ret = false;
	}
	if ((InputImageSeries.size() == 0) || (IdealOutput.size() == 0) || (outLen == 0))
	{
		std::cout << "no image is input~" << std::endl;
		std::cout << InputImageSeries.size() << "," << IdealOutput.size() << "," << outLen << std::endl;
		ret = false;
	}
	if (InputImageSeries.size() != IdealOutput.size())
	{
		std::cout << "size of input images is not equal to that of output" << std::endl;
		ret = false;
	}
	//size of each img has been check when added
		
	int NIMG = InputImageSeries.size();
	std::cout << "start traning~" << std::endl;
	if (ret == true)
	{

		int iterK = 0;
		float C = 0.0, lastC = 0.0;
		const  int dpth = CNNLayerSeries.size();
		std::thread* thr = new std::thread[dpth];
		image* DeltaImageLayerSeries = new image[dpth];
		for (int i = 0; i < dpth; i++)
		{
			DeltaImageLayerSeries[i].rows = CNNLayerSeries[i].getOutImage().rows;
			DeltaImageLayerSeries[i].cols = CNNLayerSeries[i].getOutImage().cols;
			DeltaImageLayerSeries[i].channel = CNNLayerSeries[i].getOutImage().channel;
			DeltaImageLayerSeries[i].initImage(1);
			CNNLayerSeries.at(i).initLayerMemoryV2(1);
		}
		std::vector<image> normImageSeries;
		for (image img : InputImageSeries)
		{
			normImageSeries.push_back(normliazeSimd(img));
		}
		float* outValue = (float*)_mm_malloc(sizeof(float) * outLen, AlignBytes);
		float* dyVdx = (float*)_mm_malloc(sizeof(float) * outLen, AlignBytes);
		while ((iterK < maxIteration) && ret)
		{
			clock_t st = clock();
			C = 0.0;
			std::cout << "------------------Iteration SIMD " << iterK << "------------------" << std::endl;
			for (int im = 0; im < NIMG; im += batchSize)
			{
				clock_t st = clock();
				for (int i = 0; i < dpth; i++)
				{
					DeltaImageLayerSeries[i].apllyratioSimd(0.0);
				}
				for (int b = 0; b < batchSize; b++)
				{//batch size will be optimized by multithread
					image currentIMG = normImageSeries.at(im + b);
					/*
						* a forward-calculation to get all layer out of the model for current input image
					*/
					//currentIMG.printImage(1, "NORMALIZED IMAGED");
					ret = LaunchCNNModelBySimd(currentIMG, outValue, outLen);
					C += calculateC(outValue, IdealOutput.at(im + b), outLen);
					if (C != C)
					{
						ret = false;
						std::cout << iterK << "," << im << std::endl;
					}
					if (ret == true)
					{
						/*
							* get last layer's out bias vector
						*/
						dcalculateC(IdealOutput.at(im + b), outValue, dyVdx, outLen);
						/*
							* convert out bias vector to a 1-1-outLen image
						*/
						CNNCalc layer = CNNLayerSeries.at(dpth - 1);
						image outDydx = layer.getCurrentLayerIdealOutDxdy();
						ret = VectorToImgSimd(dyVdx, outLen, outDydx);
						//outDydx.printImage(1, "outDydx");
						/*
							* sum out bias of every image for each layer
						*/
						DeltaImageLayerSeries[dpth - 1].addSimd(outDydx);
						for (int ly = dpth - 1; ly > 0; ly--)
						{
							CNNCalc layer = CNNLayerSeries.at(ly);
							image DeltaImage = CNNLayerSeries.at(ly - 1).getCurrentLayerIdealOutDxdy();
							ret = layer.UpdateLayerLossSimd(DeltaImage);

							if (ret)
							{
								DeltaImageLayerSeries[ly - 1].addSimd(DeltaImage);
							}
							else
							{
								std::cout << "UpdateLayerLoss fail" << std::endl;
								break;
							}
						}
					}
					else
					{
						break;
					}
				}
				bool iscalc = false;
				//std::cout << "period calculating out bias simd:" << (double)(clock() - st) << std::endl;
				if (ret == true)
				{
					if (batchSize > 1)
					{
						for (int ly = CNNLayerSeries.size() - 1; ly >= 0; ly--)
						{
							CNNCalc layer = CNNLayerSeries.at(ly);
							DeltaImageLayerSeries[ly].apllyratioSimd(1.0 / batchSize);
							layer.setOutLossssSimd(DeltaImageLayerSeries[ly]);
						}
					}
						
					for (int b = 0; b < batchSize; b++)
					{
						if (batchSize > 1)
						{
							image currentIMG = normImageSeries.at(im + b);
							ret = LaunchCNNModelBySimd(currentIMG, outValue, outLen);
						}							
						layerUpdatedKernalDone = 0;						
						
						for (int ly = 1; ly < dpth; ly++)
						{
							thr[ly]=std::thread(&CNNModel::layerUpdatedKernal, this, ly);
							thr[ly].detach();
						}
						layerUpdatedKernal(0);
						std::unique_lock<std::mutex> unilock(myMutexA);
						while (layerUpdatedKernalDone < dpth)
						{
							cond.wait(unilock);
						}					
					}
					for (CNNCalc layer : CNNLayerSeries)
					{
						layer.UpdateLayerWBSimd(learnRate, L2Lamda,batchSize);
					}
				}
				else
				{
					break;
				}
				//std::cout << "period updating weights simd:" << (double)(clock() - st) << std::endl;
			}
			C = sqrt(C / NIMG);
			vloss.push_back(C);
			std::cout << "used time:" << (double)(clock() - st) << std::endl;
			if ((C < minLoss) || (std::abs(C - lastC) < diffLoss))
			{
				break;
			}
			else
			{

				lastC = C;
			}
			iterK++;
			printf("iter: %d,C:%f\n", iterK, C);

		}
		_mm_free(dyVdx);
		_mm_free(outValue);
		printf("End iter: %d,C:%f\n", iterK, C);
		for (int i = 0; i < dpth; i++)
		{
			DeltaImageLayerSeries[i].freeImage();
		}
		if (DeltaImageLayerSeries != NULL)
		{
			delete[] DeltaImageLayerSeries;
			DeltaImageLayerSeries = NULL;
		}
		for (image img : normImageSeries)
		{
			img.freeImage();
		}
		printf("delete tmepimages\n");
		delete[] thr;
		for (int i = 0; i < dpth; i++)
		{
			CNNLayerSeries.at(i).freeLayerMemory();
		}
	}
	traingFlag = false;
	return true;
}
bool CNNModel::startTrainningSimdV3()
{

	bool ret = true;
	if (CNNLayerSeries.size() > 0)
	{
		if ((CNNLayerSeries.back().getCurrentLayerType() != FULLYCONNECTION))
		{
			std::cout << "cnn model is not bulit properly!" << std::endl;
			ret = false;
		}
		outLen = CNNLayerSeries.back().GetOutLength();
	}
	else
	{
		std::cout << "cnn model is not bulit properly!" << std::endl;
		ret = false;
	}
	if ((InputImageSeries.size() == 0) || (IdealOutput.size() == 0) || (outLen == 0))
	{
		std::cout << "no image is input~" << std::endl;
		std::cout << InputImageSeries.size() << "," << IdealOutput.size() << "," << outLen << std::endl;
		ret = false;
	}
	if (InputImageSeries.size() != IdealOutput.size())
	{
		std::cout << "size of input images is not equal to that of output" << std::endl;
		ret = false;
	}
	int NIMG = InputImageSeries.size();
	std::cout << "start traning~" << std::endl;
	if (ret == true)
	{
		int epoch = 0;
		float C = 0.0, lastC = 0.0;
		const  int dpth = CNNLayerSeries.size();
		std::thread* thrTrain = new std::thread[batchSize];

		for (int i = 0; i < dpth; i++)
		{
			CNNLayerSeries.at(i).initLayerMemoryV2(batchSize);
		}
		std::vector<image> normImageSeries;
		std::vector<int> randInt;
		for (image img : InputImageSeries)
		{
			normImageSeries.push_back(normliazeSimd(img));
		}
		for (int i = 0;i < InputImageSeries.size();i++)
		{
			randInt.push_back(i);
		}
	
		float* outValue = (float*)_mm_malloc(sizeof(float) * outLen*batchSize, AlignBytes);
		float* vC = (float*)_mm_malloc(sizeof(float) * batchSize, AlignBytes);
		
		std::cout << "-Start Traning\n" << "---BatchSize: " << batchSize << "\n---Loss: " << minLoss << "\n---Gradient Loss: " << diffLoss<<std::endl;
		std::cout << "---Learn rate: " << learnRate << "\n---Optimizer: ";
		switch (mOptimizer.method)
		{
		case NooP:
			std::cout << "NooP\n";
			break;
		case SGD:
			std::cout << "SGD\n";
			break;
		case SGDM:
			std::cout << "SGDM\n";
			break;
		case SGDMW:
			std::cout << "SGDMW\n";
			break;
		case SGNAD:
			std::cout << "SGNAD\n";
			break;
		case ADAM:
			std::cout << "ADAM\n";
			break;
		case ADAMW:
			std::cout << "ADAMW\n";
			break;
		default:
			std::cout << "SGD\n";
			break;
		}
		int iterK = vloss.size()+1;
	
		while ((epoch < maxIteration) && ret)
		{
			std::random_shuffle(randInt.begin(), randInt.end());
			clock_t st = clock();
			C = 0.0;
			std::cout << "------------------SIMD Epoch T " << epoch << "------------------" << std::endl;
			for (int im = 0; im < NIMG; im += batchSize)
			{			
				clock_t st = clock();	
				parrellelIBatchTrainDone = 0;
				for (int b = 0; b < batchSize; b++)
				{
					int realIndex = randInt.at(im + b);
					thrTrain[b] = std::thread(&CNNModel::updateLossParrallel, this, normImageSeries.at(realIndex), outValue,vC, outLen, realIndex, b);
					thrTrain[b].detach();
				}
				std::unique_lock<std::mutex> uniquelock(myMutexC);
				while (parrellelIBatchTrainDone<batchSize)
				{
					condC.wait(uniquelock);
				}
				for (int b = 0;b < batchSize;b++)
				{
					C += vC[b];
				}
				for (CNNCalc layer : CNNLayerSeries)
				{
					if (batchSize > 1)
					{
						for (int b = 1;b < batchSize;b++)
						{
							layer.addDKernalShadow(b);
						}
					}
					
					switch (mOptimizer.method)
					{
					case NooP:
						layer.UpdateLayerWBSimd(learnRate,L2Lamda, batchSize);
						break;
					case SGDM:
						layer.UpdateLayerWBSGDM(learnRate,mOptimizer.beta1, L2Lamda, iterK);
						break;
					case SGNAD:
						layer.UpdateLayerWBSGNAD(learnRate, mOptimizer.beta1, L2Lamda, iterK);
						break;
					case SGDMW:
						layer.UpdateLayerWBSGDMW(learnRate, mOptimizer.beta1, L2Lamda, iterK);
						break;
					case ADAM:
						layer.UpdateLayerWBADAM(learnRate, mOptimizer.beta1, mOptimizer.beta2, mOptimizer.sigma, L2Lamda, iterK);
						break;
					case ADAMW:
						layer.UpdateLayerWBADAMW(learnRate, mOptimizer.beta1, mOptimizer.beta2, mOptimizer.sigma,L2Lamda, iterK);
						break;
					default:
						layer.UpdateLayerWBSimd(learnRate, L2Lamda, batchSize);
					}
					
				}
				iterK++;
			}
			C = sqrt(C / NIMG);
			vloss.push_back(C);
			
			if ((C < minLoss) || (std::abs(C - lastC) < diffLoss))
			{
				break;
			}
			else
			{

				lastC = C;
			}
			epoch++;
			printf("Used Time : %d\nC:%f\n", (int)(clock() - st), C);

		}
	
		_mm_free(outValue);
		printf("End iter: %d,C:%f\n", epoch, C);
		for (image img : normImageSeries)
		{
			img.freeImage();
		}

		printf("delete tmepimages\n");
		delete[] thrTrain;

		for (int i = 0; i < dpth; i++)
		{
			CNNLayerSeries.at(i).freeLayerMemory();
		}
	}
	traingFlag = false;
	return true;
}
void CNNModel::updateLossParrallel(image bImage, float* outValue, float* vC, int outLen, int realIndex, int b)
{
	bool ret = LaunchCNNModelBySimdV2(bImage, outValue, outLen,b);
	vC[b]= calculateC(outValue+b*outLen, IdealOutput.at(realIndex), outLen);
	if (vC[b] != vC[b])
	{
		ret = false;
		std::cout << b << "," << realIndex << std::endl;
	}
	if (ret == true)
	{
		image lastDxdy = CNNLayerSeries.back().getCurrentLayerIdealOutDxdy();
		dcalculateC(IdealOutput.at(realIndex), outValue + b * outLen, lastDxdy.imageAtIndex(b), outLen);
		int dpth = CNNLayerSeries.size();
		for (int ly = dpth - 1; ly > 0; ly--)
		{
			CNNCalc layer = CNNLayerSeries.at(ly);
			image DeltaImage = CNNLayerSeries.at(ly - 1).getCurrentLayerIdealOutDxdy();
			ret = layer.UpdateLayerLossSimd(DeltaImage,b);

		}
	}
	for (CNNCalc layer : CNNLayerSeries)
	{
		layer.AccumulateDWSimd(learnRate, batchSize, b);
	}
	std::unique_lock<std::mutex> unilock(myMutexC);
	parrellelIBatchTrainDone++;
	if (parrellelIBatchTrainDone == batchSize)
	{
		condC.notify_one();
	}
	unilock.unlock();
}

void CNNModel::layerUpdatedKernal(int layerIndex )
{
	CNNCalc layer = CNNLayerSeries.at(layerIndex);
	layer.AccumulateDWSimd(learnRate, batchSize);
	layer.addDKernalShadow(0);
	std::unique_lock<std::mutex> unilock(myMutexA);
	layerUpdatedKernalDone++;
	if (layerUpdatedKernalDone == CNNLayerSeries.size() && layerIndex>0)
	{
		cond.notify_one();
	}
	unilock.unlock();
}

image CNNModel::VectorToImg(float*vec, int sz)
{
	image retImage;
	retImage.cols = 1;
	retImage.rows = 1;
	retImage.channel = sz;
	retImage.initImage();
	for (int ch = 0; ch < sz; ch++)
	{
		retImage.imageData[ch][0][0] = vec[ch];
	}
	return retImage;
}
bool CNNModel::VectorToImgSimd(float* vec, int sz,image& outdxdy)
{
	bool ret = true;
	if(outdxdy.channel == sz)
	{
		memcpy(outdxdy.vImageData, vec, sizeof(float) * sz);
	}
	else
	{
		ret = false;
	}
	return ret;
}

float CNNModel::calculateC(float* y, float*x, int sz)
{//a:label b:out

	float sum = 0.0;
	for (int i = 0; i < sz; i++)
	{
		if (lossType == 1)
		{//square
			sum += 0.5*(y[i] - x[i])*(y[i] - x[i]);
		}
		else
		{//cross entropy
			sum += -(y[i]*std::log(x[i])+(1-y[i])*std::log(1-x[i]));
		}
	}
	return sum / sz;
}
template< typename  T>
void CNNModel::dcalculateC(T* x, T*y, T*dyVdx, int sz)
{

	for (int i = 0; i < sz; i++)
	{
		if (lossType == 1)
		{//square
			dyVdx [i]= y[i] - x[i];
		}
		else
		{//cross entropy
			dyVdx[i] = -(y[i] / x[i] - (1 - y[i]) / (1 - x[i]));
		}
	}
	
}

image CNNModel::normliaze(const image iniputImage)
{
	image img;
	img.channel = iniputImage.channel;
	img.rows = iniputImage.rows;
	img.cols = iniputImage.cols;
	img.initImage();

	switch (normMethod)
	{
	case(2) :
		if (iniputImage.imageData != NULL)
		{
			for (int ch = 0; ch < img.channel; ch++)
			{
				float maxPix = -9999, minPix = 9999;
				for (int r = 0; r < img.rows; r++)
				{
					for (int c = 0; c < img.cols; c++)
					{
						if (iniputImage.imageData[ch][r][c] > maxPix) maxPix = iniputImage.imageData[ch][r][c];
						if (iniputImage.imageData[ch][r][c] < minPix) minPix = iniputImage.imageData[ch][r][c];
					}
				}
				for (int r = 0; r < img.rows; r++)
				{
					for (int c = 0; c < img.cols; c++)
					{						
						float currentPixValue = iniputImage.vImageData[ch + r * img.channel * img.cols + c * img.channel];
						img.imageData[ch][r][c] = (currentPixValue - minPix) / (maxPix - minPix);
						img.vImageData[ch+r* img.channel*img.cols+c*img.channel] = (currentPixValue - minPix) / (maxPix - minPix);
					}
				}
			}
		}
		break;
	case(1) :
		globalNorm(img);
		break;
	default:
		break;
	}
	return img;
}
image CNNModel::normliazeSimd(image iniputImage)
{
	
	image img;
	img.channel = iniputImage.channel;
	img.rows = iniputImage.rows;
	img.cols = iniputImage.cols;
	img.initImage();

	if (iniputImage.imageData == NULL)
	{
		std::cout << "input image not initialized" << std::endl;
		return img;
	}
	switch (normMethod)
	{
	case(2)://normalize every channel by each min and max 	
	{
		size_t offset = AlignBytes / sizeof(float);
		size_t offset16 = AlignBytes16 / sizeof(float);
		int ch = 0;
		for (; ch+offset-1 < img.channel; ch+=offset)
		{
			float* baseChannel = iniputImage.vImageData + ch;
			__m256 maxRegister = _mm256_load_ps(baseChannel);//aligned to 32 bytess
			__m256 minRegister = _mm256_load_ps(baseChannel);//aligned to 32 bytess
			for (int r = 0;r < img.rows;r++)
			{
				for (int c = 0;c < img.cols;c++)
				{
					__m256 lineAlongCh = _mm256_load_ps(baseChannel + r * iniputImage.cols * iniputImage.channel + c * iniputImage.channel);
					maxRegister = _mm256_max_ps(maxRegister, lineAlongCh);
					minRegister = _mm256_min_ps(minRegister, lineAlongCh);
				}
			}
			for (int r = 0; r < img.rows; r++)
			{
				for (int c = 0; c < img.cols; c++)
				{
					float* src = baseChannel + r * iniputImage.cols * iniputImage.channel + c * iniputImage.channel;
					__m256 lineAlongCh = _mm256_load_ps(src);
					__m256 denominator = _mm256_sub_ps(maxRegister, minRegister);
					__m256 numerator = _mm256_sub_ps(lineAlongCh, minRegister);
					__m256 quotient = _mm256_div_ps(numerator, denominator);
					float* dst = img.vImageData + ch + r * img.channel * img.cols + c * img.channel;
					_mm256_store_ps(dst, quotient);
				}
			}
		}
		for (; ch+offset16-1 < img.channel; ch += offset16)
		{
			float* baseChannel = iniputImage.vImageData + ch;
			__m128 maxRegister = _mm_load_ps(baseChannel);//aligned to 32 bytess
			__m128 minRegister = _mm_load_ps(baseChannel);//aligned to 32 bytess
			for (int r = 0;r < img.rows;r++)
			{
				for (int c = 0;c < img.cols;c++)
				{
					__m128 lineAlongCh = _mm_loadu_ps(baseChannel + r * iniputImage.cols * iniputImage.channel + c * iniputImage.channel);
					maxRegister = _mm_max_ps(maxRegister, lineAlongCh);
					minRegister = _mm_min_ps(minRegister, lineAlongCh);
				}
			}

			for (int r = 0; r < img.rows; r++)
			{
				for (int c = 0; c < img.cols; c++)
				{
					__m128 lineAlongCh = _mm_loadu_ps(baseChannel + r * iniputImage.cols * iniputImage.channel + c * iniputImage.channel);
					__m128 denominator = _mm_sub_ps(maxRegister, minRegister);
					__m128 numerator = _mm_sub_ps(lineAlongCh, minRegister);
					__m128 quotient = _mm_div_ps(numerator, denominator);
					float* dst = img.vImageData + ch + r * img.channel * img.cols + c * img.channel;
					_mm_storeu_ps(dst, quotient);
				}
			}
		}
		for (; ch  < img.channel; ch += 1)
		{
			float* baseChannel = iniputImage.vImageData + ch;
			float maxRegister = *(baseChannel);//aligned to 32 bytess
			float minRegister = *(baseChannel);//aligned to 32 bytess
			for (int r = 0;r < img.rows;r++)
			{
				for (int c = 0;c < img.cols;c++)
				{
					float lineAlongCh = *(baseChannel + r * iniputImage.cols * iniputImage.channel + c * iniputImage.channel);
					maxRegister = std::fmax(maxRegister, lineAlongCh);
					minRegister = std::fmin(minRegister, lineAlongCh);
				}
			}
			for (int r = 0; r < img.rows; r++)
			{
				for (int c = 0; c < img.cols; c++)
				{
					float lineAlongCh = *(baseChannel + r * iniputImage.cols * iniputImage.channel + c * iniputImage.channel);
					float quotient = (lineAlongCh - minRegister)/(maxRegister - minRegister);
					float* dst = img.vImageData + ch + r * img.channel * img.cols + c * img.channel;
					*dst= quotient;
				}
			}
		}
		break;
	}
	case(1):
		return iniputImage.retApllyratioSimd(1.0 / normGlobalValueOrRange + normShift);
		break;
	default:
		break;
	}
	return img;
}
void CNNModel::globalNorm(image& img)
{
	int row = img.rows;
	int col = img.cols;
	int chn = img.channel;
	for (int ch = 0; ch < img.channel; ch++)
	{
		for (int r = 0; r < img.rows; r++)
		{
			for (int c = 0; c < img.cols; c++)
			{
				img.imageData[ch][r][c] *=1/normGlobalValueOrRange+normShift;
			}
		}
	}
}

void CNNModel::rangeorm(image& img)
{
	int row = img.rows;
	int col = img.cols;
	int chn = img.channel;
	
	for (int ch = 0; ch < img.channel; ch++)
	{
		double minValue = 99999, maxValue = -99999;
		for (int r = 0; r < img.rows; r++)
		{
			for (int c = 0; c < img.cols; c++)
			{
				if (img.imageData[ch][r][c]>maxValue) maxValue = img.imageData[ch][r][c];
				if (img.imageData[ch][r][c]<minValue) minValue = img.imageData[ch][r][c];
				//img.imageData[ch][r][c] *= 1 / normGlobalValueOrRange + normShift;
			}
		}
		for (int r = 0; r < img.rows; r++)
		{
			for (int c = 0; c < img.cols; c++)
			{
				double value = img.imageData[ch][r][c];
				img.imageData[ch][r][c] = (value-minValue)/(maxValue-minValue)*normGlobalValueOrRange+normShift;
			}
		}
	}
}
void CNNModel::clearInputImage()
{
	for (image& img : InputImageSeries)
	{
	//	img.freeImage();
	}
	std::vector<image>().swap(InputImageSeries);
	for (int i = 0;i < IdealOutput.size();i++)
	{
		delete IdealOutput.at(i);
	}
	IdealOutput.clear();

}
bool CNNModel::saveModel(char* path, bool isSimdTrained)
{
	std::ofstream of(path,std::ios::binary);
	char* chrModelHead = (char*)_mm_malloc(sizeof(modelHead), AlignBytes);
	char* chrLayerHead = (char*)_mm_malloc(sizeof(layerHead), AlignBytes);
	of << chrModelHead;//weights
	
	for (CNNCalc layer : CNNLayerSeries)
	{
		kernal* allkernals = layer.getAllKernals();
		layerHead nlayerHead;
		float* VBias = (float*)_mm_malloc(sizeof(float)* layer.getKernalNum(), AlignBytes);
		char* chrBias = (char*)_mm_malloc(sizeof(float) * layer.getKernalNum(), AlignBytes);
		size_t sz = allkernals[0].row * allkernals[0].col * allkernals[0].channel;
		char* chrWeights = (char*)_mm_malloc(sizeof(float) * sz, AlignBytes);
		for (int n = 0;n < layer.getKernalNum();n++)
		{		
			memcpy(chrLayerHead, &nlayerHead, sizeof(layerHead));
			of << chrLayerHead;//layer head information
			float* vweights = allkernals[n].vWeight;		
			memcpy(chrWeights, vweights, sizeof(float) * sz);
			of << chrWeights;//weights
			VBias[n] = allkernals[n].biasSimd;
		}		
		memcpy(chrBias, VBias, sizeof(float) * layer.getKernalNum());
		of << chrBias;//weights
		_mm_free(chrWeights);
		_mm_free(chrBias);
		_mm_free(VBias);
	}
	_mm_free(chrLayerHead);
	of.close();
	return true;
}
bool CNNModel::readModel(char* path, bool isSimdTrained)
{
	std::ofstream of(path, std::ios::binary);
	char* chrModelHead = (char*)_mm_malloc(sizeof(modelHead), AlignBytes);
	char* chrLayerHead = (char*)_mm_malloc(sizeof(layerHead), AlignBytes);
	of << chrModelHead;//weights
	for (CNNCalc layer : CNNLayerSeries)
	{
		kernal* allkernals = layer.getAllKernals();
		layerHead nlayerHead;
		float* VBias = (float*)_mm_malloc(sizeof(float) * layer.getKernalNum(), AlignBytes);
		char* chrBias = (char*)_mm_malloc(sizeof(float) * layer.getKernalNum(), AlignBytes);
		size_t sz = allkernals[0].row * allkernals[0].col * allkernals[0].channel;
		char* chrWeights = (char*)_mm_malloc(sizeof(float) * sz, AlignBytes);
		for (int n = 0;n < layer.getKernalNum();n++)
		{
			memcpy(chrLayerHead, &nlayerHead, sizeof(layerHead));
			of << chrLayerHead;//layer head information
			float* vweights = allkernals[n].vWeight;
			memcpy(chrWeights, vweights, sizeof(float) * sz);
			of << chrWeights;//weights
			VBias[n] = allkernals[n].biasSimd;
		}
		memcpy(chrBias, VBias, sizeof(float) * layer.getKernalNum());
		of << chrBias;//weights
		_mm_free(chrWeights);
		_mm_free(chrBias);
		_mm_free(VBias);
		of << '|' ;
	}
	_mm_free(chrLayerHead);
	of.close();
	return true;
}
