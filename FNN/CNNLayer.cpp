#include"CNNLayer.h"

CNNCalc::CNNCalc()
{
	isSetData = false;
	isSetConfig = false;
	activateType = 4;
	poolingFun = 0;
	poolingRow = 2;
	poolingCol = 2;
	poolingStride = 2;
	HideLayerNumth = 0;
	paddingrow = 0;
	paddingcol = 0;
	padMoethod = SMALLER;
	thisLayerType = CONVOLUTION;
	isBufferInitiated = false;
}
CNNCalc::~CNNCalc()
{
	/*
	for (float* v : vlineCNNKernals)
	{
		_mm_free(v);
	}
	for (float* v : vCNNKernalsRotate180)
	{
		_mm_free(v);
	}
	for (float* v : vdkernal)
	{
		_mm_free(v);
	}
	if (rowImg != NULL)
	{
		_mm_free(rowImg);
		rowImg = NULL;
	}
	vlineCNNKernals.clear();
	vCNNKernalsRotate180.clear();
	vdkernal.clear();
	*/
}
void CNNCalc::SetActivateFun(int Act)
{
	activateType = Act;
	BL.setActType(Act);
}
void CNNCalc::SetInput(image imago)
{
	bool rowOk = inputImage.rows == imago.rows;
	bool colOk = inputImage.cols == imago.cols;
	bool channelOk = inputImage.channel == imago.channel;
	if (rowOk && colOk && channelOk)
	{
		for (int ch = 0; ch < imago.channel; ch++)
		{
			for (int r = 0; r < imago.rows; r++)
			{
				for (int c = 0; c < imago.cols; c++)
				{
					inputImage.imageData[ch][r][c] = imago.imageData[ch][r][c];
				}
			}
		}
		isSetData = true;
	}
	else
	{
		std::cout << "siz of input image that it to be is not expected" << std::endl;
		isSetData = false;
	}
}

bool CNNCalc::LaunchConvolution()
{
	if (isSetData && isSetConfig)
	{
		padding();
		Convolution();
		if (thisLayerType == CONVOLUTION)
		{
			pooling();
		}
		return true;
	}
	else
	{
		return false;
	}
}
bool CNNCalc::LaunchConvolutionBySimd(int b)
{
	if (isSetData && isSetConfig)
	{
		paddingSimd(b);
		if (thisLayerType == CONVOLUTION)
		{
			ConvolutionSimd(b);
			poolingSimdV2(b);
		}
		else
		{
			ConvolutionSimdFullyConv(b);
		}
		return true;
	}
	else
	{
		return false;
	}
}
bool CNNCalc::SetKernals(std::vector<kernal> kernals, int kernalrow, int kernalcol, int padsz, int strd, int ns)
{
	paddingrow = padsz;
	paddingcol = padsz;
	stride = strd;
	kernalRows = kernalrow;
	kernalCols = kernalcol;
	kernalChannel = 1;
	neuroNums = ns;

	CNNKernals = new kernal[neuroNums];
	if (kernals.size() == ns)
	{
		for (int kn = 0; kn < neuroNums; kn++)
		{
			CNNKernals[kn].row = kernalRows;
			CNNKernals[kn].col = kernalCols;
			CNNKernals[kn].channel = kernalChannel;
			CNNKernals[kn].initKernal(1);
			for (int ch = 0; ch < kernalChannel; ch++)
			{
				for (int r = 0; r < kernalrow; r++)
				{
					for (int c = 0; c < kernalcol; c++)
					{
						CNNKernals[kn].weight[ch][r][c] = kernals.at(kn).weight[ch][r][c];
					}
				}
			}
		}

		isSetConfig = true;
		return true;
	}
	else
	{
		return false;
	}

}
void CNNCalc::SetPoolings(int pldim1, int pldim2, int plStrd,int plFun)
{
	poolingRow = pldim1;
	poolingCol = pldim2;
	poolingStride = plStrd;
	poolingFun = plFun;
}


void CNNCalc::Convolution()
{
	for (int kn = 0; kn < neuroNums; kn++)
	{
		kernal CurrentKernal = CNNKernals[kn];
		int OutBuffer1IndexX = 0;
		for (int r = 0; r < padImage.rows - kernalRows + 1; r += stride)
		{
			int OutBuffer1IndexY = 0;
			for (int c = 0; c < padImage.cols - kernalCols + 1; c += stride)
			{
				float currentValue = 0.0;
				for (int kr = 0; kr < kernalRows; kr++)
				{
					for (int kc = 0; kc < kernalCols; kc++)
					{
						for (int kch = 0; kch < kernalChannel; kch++)
						{
							currentValue += CurrentKernal.weight[kch][kr][kc] * padImage.imageData[kch][r + kr][c + kc];
						}
					}
				}
				bzactImage.imageData[kn][OutBuffer1IndexX][OutBuffer1IndexY] = currentValue + CurrentKernal.bias;
				actImage.imageData[kn][OutBuffer1IndexX][OutBuffer1IndexY] = activate(currentValue + CurrentKernal.bias, activateType);
				OutBuffer1IndexY++;

			}
			OutBuffer1IndexX++;
		}

	}

}
void CNNCalc::padding()
{

	for (int ch = 0; ch < padImage.channel; ch++)
	{
		for (int r = 0; r < padImage.rows; r++)
		{
			for (int c = 0; c < padImage.cols; c++)
			{
				if ((r < paddingrow) || (r > inputImage.rows + paddingrow - 1) || (c < paddingcol) || (c > inputImage.cols + paddingcol - 1))
				{
					padImage.imageData[ch][r][c] = 0;
				}
				else
				{
					padImage.imageData[ch][r][c] = inputImage.imageData[ch][r - paddingrow][c - paddingcol];
				}

			}
		}
	}
}

void CNNCalc::pooling()
{
	for (int kn = 0; kn < actImage.channel; kn++)
	{
		int outBuffer2IndexX = 0;
		for (int r = 0; r < actImage.rows - poolingRow + 1; r += poolingStride)
		{
			int outBuffer2IndexY = 0;
			for (int c = 0; c < actImage.cols - poolingCol + 1; c += poolingStride)
			{
				float currentValue = actImage.imageData[kn][r][c ];
				for (int i = 0; i < poolingRow; i++)
				{
					for (int j = 0; j < poolingCol; j++)
					{
						if (poolingFun == 0)
						{
							if (actImage.imageData[kn][r + i][c + j] > currentValue)
							{
								currentValue = actImage.imageData[kn][r + i][c + j];
							}
						}
						else
						{
							currentValue += actImage.imageData[kn][r + i][c + j] / (poolingRow * poolingCol);
						}
					}
				}
				outImage.imageData[kn][outBuffer2IndexX][outBuffer2IndexY] = currentValue;
				outBuffer2IndexY++;
			}
			outBuffer2IndexX++;
		}
	}
}
void CNNCalc::SetInputSimd(image imago,int b)
{

	float* imagoBase = imago.imageAtIndex(b);
	float* inputImgBase = inputImage.imageAtIndex(b);
	bool rowOk = inputImage.rows == imago.rows;
	bool colOk = inputImage.cols == imago.cols;
	bool channelOk = inputImage.channel == imago.channel;
	size_t sz = inputImage.rows * inputImage.cols * inputImage.channel;
	if (rowOk && colOk && channelOk)
	{
		memcpy(inputImgBase, imagoBase, sizeof(float) * sz);
		isSetData = true;
	}
	else
	{
		std::cout << "siz of input image that it to be is not expected" << std::endl;
		isSetData = false;
	}
}
void CNNCalc::SetInputSimdV2(image imago, int b)
{

	float* imagoBase = imago.imageAtIndex(0);
	float* inputImgBase = inputImage.imageAtIndex(b);
	bool rowOk = inputImage.rows == imago.rows;
	bool colOk = inputImage.cols == imago.cols;
	bool channelOk = inputImage.channel == imago.channel;
	size_t sz = inputImage.rows * inputImage.cols * inputImage.channel;
	if (rowOk && colOk && channelOk)
	{
		memcpy(inputImgBase, imagoBase, sizeof(float) * sz);
		isSetData = true;
	}
	else
	{
		std::cout << "siz of input image that it to be is not expected" << std::endl;
		isSetData = false;
	}
}

void CNNCalc::ConvolutionSimdFullyConv(int b)
{
	float* actImgBase = actImage.imageAtIndex(b);
	float* bzactImgBase = bzactImage.imageAtIndex(b);
	float* padImgBase =  padImage.imageAtIndex(b);
	size_t offset = AlignBytes / sizeof(float);
	size_t offset16 = AlignBytes16 / sizeof(float);
	size_t length = padImage.rows * padImage.cols * padImage.channel ;
	for (int kn = 0; kn < neuroNums; kn++)
	{
		float currentValue = CNNKernals[kn].biasSimd;
		float* CurrentKernal = CNNKernals[kn].vWeight;
		__m256 sumRegister = _mm256_setzero_ps();
		int i = 0;

		for (;i< length;i += offset)
		{
			__m256 line1 = _mm256_load_ps(CurrentKernal+i);
			__m256 line2 = _mm256_load_ps(padImgBase + i);
			sumRegister = _mm256_fmadd_ps(line1, line2, sumRegister);
		}

		sumRegister = _mm256_hadd_ps(sumRegister, sumRegister);
		sumRegister = _mm256_hadd_ps(sumRegister, sumRegister);

		currentValue += sumRegister.m256_f32[0];
		currentValue += sumRegister.m256_f32[4];
		*(bzactImgBase +kn) = currentValue;
		*(actImgBase+kn) = activate(currentValue, activateType);
	}
}
void CNNCalc::ConvolutionSimd(int b)
{
	float* actImgBase = actImage.imageAtIndex(b);
	float* bzactImgBase = bzactImage.imageAtIndex(b);
	
	trpDim kernalSz(kernalRows, kernalCols, kernalChannel);
	padImage.im2row(kernalSz,stride,b);
	float* padRowImgBase = padImage.rowImg + b * padImage.rowImgBlockSize;
	size_t offset = AlignBytes / sizeof(float);
	size_t offset16 = AlignBytes16 / sizeof(float);
	size_t reorderCol = AlignVec(kernalRows * kernalCols * kernalChannel, offset);
	size_t rowstrp = (padImage.rows - kernalRows) / stride + 1;
	size_t colstrp = (padImage.cols - kernalCols) / stride + 1;
	size_t reorderRow = rowstrp * colstrp;
	//std::cout <<"rowstrp:"<< rowstrp << std::endl;
	for (int kn = 0; kn < neuroNums; kn++)
	{
		int OutBuffer1IndexY = 0;
		int OutBuffer1IndexX = 0;
		float* CurrentKernal = CNNKernals[kn].vWeight;
		for (size_t convRow = 0;convRow < reorderRow;convRow++)
		{
			float currentValue = CNNKernals[kn].biasSimd;
			__m256 sumRegister = _mm256_setzero_ps();
			float* baseRowImagePointer = padRowImgBase + convRow * reorderCol;
			int i = 0;
			for (;i  < reorderCol;i += offset)
			{
				__m256 vector32bytesWeight = _mm256_load_ps(CurrentKernal + i);
				__m256 vector32bytesImage = _mm256_load_ps(baseRowImagePointer + i);
			//	std::cout << vector32bytesWeight.m256_f32[0] << "," << vector32bytesImage.m256_f32[0] << std::endl;
				sumRegister = _mm256_fmadd_ps(vector32bytesWeight, vector32bytesImage, sumRegister);
			}
			sumRegister = _mm256_hadd_ps(sumRegister, sumRegister);
			sumRegister = _mm256_hadd_ps(sumRegister, sumRegister);

			currentValue += sumRegister.m256_f32[0];
			currentValue += sumRegister.m256_f32[4];
			if (OutBuffer1IndexY == colstrp)
			{
				OutBuffer1IndexX++;
				OutBuffer1IndexY = 0;
			}
			*(bzactImgBase+kn + OutBuffer1IndexX * colstrp * neuroNums + OutBuffer1IndexY * neuroNums) = currentValue;
			*(actImgBase+kn + OutBuffer1IndexX * colstrp * neuroNums + OutBuffer1IndexY * neuroNums) = activate(currentValue, activateType);

			OutBuffer1IndexY++;
		}
	}

}


void CNNCalc::paddingSimd(int b)
{
	float* padImgBase = padImage.imageAtIndex(b);
	float* inputImgBase =inputImage.imageAtIndex(b);
	for (int r = paddingrow; r+ paddingrow < padImage.rows; r++)
	{
		float* baseRowPointerSrc = inputImgBase + (r- paddingrow) * inputImage.channel * inputImage.cols;
		float* baseRowPointerDst = padImgBase + r * padImage.channel * padImage.cols+paddingcol* padImage.channel;
		memcpy( baseRowPointerDst, baseRowPointerSrc, sizeof(float) * inputImage.channel * inputImage.cols);
	}
}

void CNNCalc::poolingSimdV2(int b)
{
	float* actImgBase =  actImage.imageAtIndex(b);
	float* outImgBase =outImage.imageAtIndex(b);
	size_t offset = AlignBytes / sizeof(float);
	size_t offset16 = AlignBytes16 / sizeof(float);
	size_t colstrp = (actImage.cols - poolingCol) / poolingStride + 1;
	__m256 aveNo = _mm256_set1_ps(1.0/poolingCol / poolingRow);
	size_t ch = 0;
	for (;ch+ offset -1 < actImage.channel ;ch += offset)
	{
		for (int OutBuffer1IndexX = 0;OutBuffer1IndexX < outImage.rows;OutBuffer1IndexX += 1)
		{
			for (int OutBuffer1IndexY = 0;OutBuffer1IndexY < outImage.cols;OutBuffer1IndexY += 1)
			{
				int Dim1 = OutBuffer1IndexX * poolingStride;
				int Dim2 = (OutBuffer1IndexY * poolingStride) * actImage.channel + ch;
				size_t BasePointer = Dim2 + Dim1 * actImage.cols * actImage.channel;
				__m256 poolRegister = _mm256_loadu_ps(actImgBase + BasePointer);
				for (int pr = 0;pr < poolingRow;pr++)
				{
					for (int pc = 0;pc < poolingCol;pc++)
					{
						Dim1 = OutBuffer1IndexX * poolingStride+pr;
						Dim2 = (OutBuffer1IndexY * poolingStride + pc) * actImage.channel + ch;
						BasePointer = Dim2 + Dim1 * actImage.cols * actImage.channel;
						__m256 maxRegisterTobeCompared = _mm256_loadu_ps(actImgBase + BasePointer);
						if (poolingFun == 0)
						{
							poolRegister = _mm256_max_ps(maxRegisterTobeCompared, poolRegister);
						}
						else
						{
							poolRegister = _mm256_add_ps(maxRegisterTobeCompared, poolRegister);
						}
					}
				}
				if (poolingFun == 1)
				{
					poolRegister = _mm256_mul_ps(poolRegister, aveNo);
				}
				size_t outBasePointer = OutBuffer1IndexX * colstrp * actImage.channel + OutBuffer1IndexY * actImage.channel + ch;
				_mm256_storeu_ps(outImgBase + outBasePointer, poolRegister);
			}
		}
	}
	for (;ch + offset16 - 1 < actImage.channel;ch += offset16)
	{
		for (int OutBuffer1IndexX = 0;OutBuffer1IndexX < outImage.rows;OutBuffer1IndexX += 1)
		{
			for (int OutBuffer1IndexY = 0;OutBuffer1IndexY < outImage.cols;OutBuffer1IndexY += 1)
			{
				int Dim1 = OutBuffer1IndexX * poolingStride;
				int Dim2 = (OutBuffer1IndexY * poolingStride) * actImage.channel + ch;
				size_t BasePointer = Dim2 + Dim1 * actImage.cols * actImage.channel;
				__m128 maxRegister = _mm_loadu_ps(actImgBase + BasePointer);
				for (int pr = 0;pr < poolingRow;pr++)
				{
					for (int pc = 0;pc < poolingCol;pc++)
					{
						Dim1 = OutBuffer1IndexX * poolingStride + pr;
						Dim2 = (OutBuffer1IndexY * poolingStride + pc) * actImage.channel + ch;
						BasePointer = Dim2 + Dim1 * actImage.cols * actImage.channel;
						__m128 maxRegisterTobeCompared = _mm_loadu_ps(actImgBase + BasePointer);
						maxRegister = _mm_max_ps(maxRegisterTobeCompared, maxRegister);
					}
				}
				size_t outBasePointer = OutBuffer1IndexX * colstrp * actImage.channel + OutBuffer1IndexY * actImage.channel + ch;
				_mm_storeu_ps(outImgBase + outBasePointer, maxRegister);
			}
		}
	}
	for(;ch < actImage.channel;ch += 1)
	{
		for (int OutBuffer1IndexX = 0;OutBuffer1IndexX < outImage.rows;OutBuffer1IndexX += 1)
		{
			for (int OutBuffer1IndexY = 0;OutBuffer1IndexY < outImage.cols;OutBuffer1IndexY += 1)
			{
				int Dim1 = OutBuffer1IndexX * poolingStride;
				int Dim2 = (OutBuffer1IndexY * poolingStride) * actImage.channel + ch;
				size_t BasePointer = Dim2 + Dim1 * actImage.cols * actImage.channel;
				float maxRegister = *(actImgBase + BasePointer);
				for (int pr = 0;pr < poolingRow;pr++)
				{
					for (int pc = 0;pc < poolingCol;pc++)
					{
						Dim1 = OutBuffer1IndexX * poolingStride + pr;
						Dim2 = (OutBuffer1IndexY * poolingStride + pc) * actImage.channel + ch;
						BasePointer = Dim2 + Dim1 * actImage.cols * actImage.channel;
						float maxRegisterTobeCompared = *(actImgBase + BasePointer);
						maxRegister = std::max(maxRegisterTobeCompared, maxRegister);
					}
				}
				size_t outBasePointer = OutBuffer1IndexX * colstrp * actImage.channel + OutBuffer1IndexY * actImage.channel + ch;
				outImgBase[outBasePointer]= maxRegister;
			}
		}
	}
}

float CNNCalc::activate(float a, int fun)
{
	if (fun == 2)
	{//Relu
		if (a < 0)
		{
			return 0;
		}
		else
		{
			return a;
		}
	}
	else if (fun == 1)
	{//sigmoid
		return 1.0 / (1 + exp(-a));
	}
	else if (fun == 3)
	{//tan
		return (exp(a)-exp(-a)) / (exp(a) + exp(-a));
	}
	else if (fun == 4)
	{
		if (a < 0)
		{
			return exp(a) - 1;
		}
		else
		{
			return a;
		}
	}
	else
	{
		if (a < 0)
		{
			return 0;
		}
		else
		{
			return a;
		}
	}
}

void CNNCalc::initKernals(int wtsRow, int wtsCol, int wtsChannel, int ns,int strd, float sd, float bs)
{

	stride = strd;
	kernalRows = wtsRow;
	kernalCols = wtsCol;
	kernalChannel = wtsChannel;
	neuroNums = ns;

	CNNKernals = new kernal[neuroNums];
	BL.ShadowMoment = new kernal[neuroNums];
	BL.ShadowVelocity = new kernal[neuroNums];
	CNNKernalsRotate180 = new kernal[neuroNums];
	dkernal = new kernal[neuroNums];
	for (int kn = 0; kn < neuroNums; kn++)
	{
		CNNKernals[kn].row = kernalRows;
		CNNKernals[kn].col = kernalCols;
		CNNKernals[kn].channel = kernalChannel;
		CNNKernals[kn].initKernal(sd);

		BL.ShadowMoment[kn].row = kernalRows;
		BL.ShadowMoment[kn].col = kernalCols;
		BL.ShadowMoment[kn].channel = kernalChannel;
		BL.ShadowMoment[kn].initKernal(0);

		BL.ShadowVelocity[kn].row = kernalRows;
		BL.ShadowVelocity[kn].col = kernalCols;
		BL.ShadowVelocity[kn].channel = kernalChannel;
		BL.ShadowVelocity[kn].initKernal(0);

		CNNKernalsRotate180[kn].row = kernalRows;
		CNNKernalsRotate180[kn].col = kernalCols;
		CNNKernalsRotate180[kn].channel = kernalChannel;
		CNNKernalsRotate180[kn].initKernal(0);
		
		dkernal[kn].row = kernalRows;
		dkernal[kn].col = kernalCols;
		dkernal[kn].channel = kernalChannel;
		dkernal[kn].initKernal(0.0);
	}
	CNNKernalsRotate180Simd = new kernal[kernalChannel];
	for (int inCh = 0;inCh < kernalChannel;inCh++)
	{
		CNNKernalsRotate180Simd[inCh].row = kernalRows;
		CNNKernalsRotate180Simd[inCh].col = kernalCols;
		CNNKernalsRotate180Simd[inCh].channel = neuroNums;
		CNNKernalsRotate180Simd[inCh].initKernal(0);
	}
	BL.aKernal.row = kernalRows;
	BL.aKernal.col = kernalCols;
	BL.aKernal.channel = neuroNums;
	BL.aKernal.initKernal(0);
	isSetConfig = true;
	
}


void CNNCalc::initLayerMemory(int inRows, int inCols, int inChannel)
{
	inputImage.channel = inChannel;
	inputImage.rows = inRows;
	inputImage.cols = inCols;
	inputImage.initImage();
	
	if (thisLayerType == FULLYCONNECTION)
	{
		paddingrow = 0;
		paddingcol = 0;
	}
	else
	{
		if (padMoethod == SMALLER  )
		{
			paddingrow = 0;
			paddingcol = 0;
		}
		else
		{//SAME
			paddingrow = (kernalRows-1) / 2;
			paddingcol = (kernalCols-1) / 2;
		}

	}

	padImage.channel = inputImage.channel;
	padImage.rows = inputImage.rows + 2 * paddingrow;
	padImage.cols = inputImage.cols + 2 * paddingcol;
	padImage.initImage();
	
	trpDim kernaldim(kernalRows, kernalCols, kernalChannel);
	padImage.initRowImage(kernaldim, stride);


	std::cout << "padding image is initialized~" << std::endl;
	setOutBuffer();
	std::cout << "activated image is initialized~" << std::endl;
	setOutBuffer2();
	std::cout << "Out image is initialized~" << std::endl;

	isBufferInitiated = true;

}
void CNNCalc::initLayerSize(int inRows, int inCols, int inChannel)
{
	inputImage.channel = inChannel;
	inputImage.rows = inRows;
	inputImage.cols = inCols;
	//inputImage.initImage();

	if (thisLayerType == FULLYCONNECTION)
	{
		paddingrow = 0;
		paddingcol = 0;
	}
	else
	{
		if (padMoethod == SMALLER)
		{
			paddingrow = 0;
			paddingcol = 0;
		}
		else
		{//SAME
			paddingrow = (kernalRows - 1) / 2;
			paddingcol = (kernalCols - 1) / 2;
		}

	}

	padImage.channel = inputImage.channel;
	padImage.rows = inputImage.rows + 2 * paddingrow;
	padImage.cols = inputImage.cols + 2 * paddingcol;

	actImage.rows = (padImage.rows - kernalRows) / stride + 1;
	actImage.cols = (padImage.cols - kernalCols) / stride + 1;
	actImage.channel = neuroNums;

	bzactImage.rows = (padImage.rows - kernalRows) / stride + 1;
	bzactImage.cols = (padImage.cols - kernalCols) / stride + 1;
	bzactImage.channel = neuroNums;

	outImage.channel = neuroNums;
	if (thisLayerType == layerType::CONVOLUTION)
	{
		outImage.rows = (actImage.rows - poolingRow) / poolingStride + 1;
		outImage.cols = (actImage.cols - poolingCol) / poolingStride + 1;
	}
	else
	{
		outImage.rows = actImage.rows;
		outImage.cols = actImage.cols;
	}
}
void CNNCalc::freeLayerMemory()
{
	inputImage.freeImageSpace();
	padImage.freeImageSpace();
	actImage.freeImageSpace();
	bzactImage.freeImageSpace();
	dpoolingIMG.freeImageSpace();
	outImage.freeImageSpace();
	BL.dIdealOutVSdO.freeImageSpace();
	BL.dactImage.freeImageSpace();
	BL.dbzImage.freeImageSpace();
	BL.dactImageW.freeImageSpace();
	BL.dbzImageW.freeImageSpace();
	for (int b = 0;b < dkernalSeries.size();b++)
	{
		if (dkernalSeries.at(b) != NULL)
		{
			for (int k = 0;k < neuroNums;k++)
			{
				dkernalSeries.at(b)[k].FreeWtsSpace();
			}
		}
	
	}

}
void CNNCalc::initLayerMemoryV2(int batchSize)
{

	inputImage.initImage(0,batchSize);
	padImage.initImage(0, batchSize);
	trpDim kernaldim(kernalRows, kernalCols, kernalChannel);
	padImage.initRowImage(kernaldim, stride, batchSize);
//	std::cout << "padding image is initialized~" << std::endl;
	setOutBuffer(batchSize);
//	std::cout << "activated image is initialized~" << std::endl;
	setOutBuffer2(batchSize);
//	std::cout << "Out image is initialized~" << std::endl;
	dkernalSeries.clear();
	for (int b = 0;b < std::max(3,batchSize);b++)
	{
		kernal* bkernal = new kernal[neuroNums];
		for (int k = 0;k < neuroNums;k++)
		{
			bkernal[k].row = kernalRows;
			bkernal[k].col = kernalCols;
			bkernal[k].channel = kernalChannel;
			bkernal[k].initKernal(0.0);
		}
		dkernalSeries.push_back(bkernal);
	}
	isBufferInitiated = true;
}
void CNNCalc::setOutBuffer(int batchSize)
{

	actImage.initImage(0, batchSize);
	bzactImage.initImage(0, batchSize);
	dpoolingIMG.channel = actImage.channel;
	dpoolingIMG.rows = actImage.rows;
	dpoolingIMG.cols = actImage.cols;
	dpoolingIMG.initImage(0, batchSize);
}

void CNNCalc::setOutBuffer2(int batchSize)
{
	outImage.initImage(0, batchSize);
	BL.dIdealOutVSdO.channel = neuroNums;
	BL.dIdealOutVSdO.rows = outImage.rows;
	BL.dIdealOutVSdO.cols = outImage.cols;
	BL.dIdealOutVSdO.initImage(0, batchSize);

	int targetRow = inputImage.rows + kernalRows - 1;
	int targetCol = inputImage.cols + kernalCols - 1;
	BL.dactImage.channel = neuroNums;
	BL.dactImage.rows = targetRow;
	BL.dactImage.cols = targetCol;
	BL.dactImage.initImage(0, batchSize);


	BL.dbzImage.channel = neuroNums;
	BL.dbzImage.rows = targetRow;
	BL.dbzImage.cols = targetCol;
	BL.dbzImage.initImage(0, batchSize);
		
	trpDim kernaldim(kernalRows, kernalCols, neuroNums);
	BL.dbzImage.initRowImage(kernaldim,stride, batchSize);
	BL.dactImage.initRowImage(kernaldim, stride,batchSize);
	int targetRowW = 0;
	int targetColW = 0;
	if (thisLayerType == FULLYCONNECTION)
	{
		targetRowW = (BL.dIdealOutVSdO.rows - 1) * (stride - 1) + BL.dIdealOutVSdO.rows;
		targetColW = (BL.dIdealOutVSdO.cols - 1) * (stride - 1) + BL.dIdealOutVSdO.cols;
	}
	else
	{
		targetRowW = (dpoolingIMG.rows - 1) * (stride - 1) + dpoolingIMG.rows;
		targetColW = (dpoolingIMG.cols - 1) * (stride - 1) + dpoolingIMG.cols;
	}
	BL.dactImageW.channel = neuroNums;
	BL.dactImageW.rows = targetRowW;
	BL.dactImageW.cols = targetColW;
	BL.dactImageW.initImage(0, batchSize);


	BL.dbzImageW.channel = neuroNums;
	BL.dbzImageW.rows = targetRowW;
	BL.dbzImageW.cols = targetColW;
	BL.dbzImageW.initImage(0,batchSize);
}

image CNNCalc::getOutImage()
{
	if (thisLayerType == CONVOLUTION)
	{
		return outImage;
	}
	else
	{
		return actImage;
	}	
}
bool CNNCalc::UpdateLayerLoss(image& retImage)
{//update dC/dI
	
	
	bool ret = true;
	BL.TMatrixKernal(CNNKernals, CNNKernalsRotate180, neuroNums);
	
	if (thisLayerType == layerType::CONVOLUTION)
	{
		ret = BL.dPooling(actImage, BL.dIdealOutVSdO, poolingRow, poolingCol, poolingStride, dpoolingIMG);
		//dc/dA=dC/dOut(.)dOut/dA
		if (ret)
		{		
			ret = BL.dConvolutionX(inputImage, dpoolingIMG, bzactImage,CNNKernalsRotate180, neuroNums, stride,retImage);
		}
	}
	else if (thisLayerType == layerType::FULLYCONNECTION)
	{
		ret = BL.dConvolutionX(inputImage, BL.dIdealOutVSdO, bzactImage, CNNKernalsRotate180, neuroNums, stride,retImage);
		
	}
	else
	{
		ret = false;
	}
	
	return ret;

}

bool CNNCalc::UpdateLayerLossSimd(image& retImage,int b)
{//update dC/dI
	bool ret = true;
	if (thisLayerType == layerType::CONVOLUTION)
	{
		BL.TMatrixKernalSimd(CNNKernals, CNNKernalsRotate180Simd, neuroNums);
		ret = BL.dPoolingSimd(actImage, BL.dIdealOutVSdO, poolingRow, poolingCol, poolingStride, dpoolingIMG,b);	
		if (ret)
		{
			ret = BL.dConvolutionXSimd(inputImage, dpoolingIMG, bzactImage, CNNKernalsRotate180Simd, neuroNums, stride, retImage,b);
			//std::cout << "dConvolutionXSimd matrix sucee" << std::endl;
		}
	}
	else if (thisLayerType == layerType::FULLYCONNECTION)
	{
		ret = BL.dFullyConvolutionXSimd(inputImage, BL.dIdealOutVSdO, bzactImage, CNNKernals, neuroNums, retImage,b);
	}
	else
	{
		ret = false;
	}

	return ret;

}


bool CNNCalc::setOutLossss(image umg)
{
	if ((BL.dIdealOutVSdO.channel == umg.channel) && (BL.dIdealOutVSdO.rows == umg.rows) && (BL.dIdealOutVSdO.cols == umg.cols))
	{
		for (int ch = 0; ch < umg.channel;ch++)
		{
			for (int r = 0; r < umg.rows; r++)
			{
				for (int c = 0; c < umg.cols; c++)
				{
					BL.dIdealOutVSdO.imageData[ch][r][c] = umg.imageData[ch][r][c];
				}
			}
		}
	}
	else
	{
		std::cout << "ideal out is not matched to that of updated loss" << std::endl;
		return false;
	}
	
	
	return true;
}
bool CNNCalc::setOutLossssSimd(image umg,int b)
{
	if ((BL.dIdealOutVSdO.channel == umg.channel) && (BL.dIdealOutVSdO.rows == umg.rows) && (BL.dIdealOutVSdO.cols == umg.cols))
	{
		memcpy(BL.dIdealOutVSdO.imageAtIndex(b), umg.vImageData, sizeof(float) * umg.channel * umg.rows * umg.cols);
	}
	else
	{
		std::cout << "ideal out is not matched to that of updated loss" << std::endl;
		return false;
	}


	return true;
}
void CNNCalc::AccumulateDW(float learnrate,int bs)
{//update weights
	bool ret = true;
	for (int i = 0; i < neuroNums;i++)
	{
		dkernal[i].apply(0.0);
	}
	if (thisLayerType == layerType::CONVOLUTION)
	{
		ret = BL.dPooling(actImage, BL.dIdealOutVSdO,poolingRow,poolingCol,poolingStride, dpoolingIMG);
		//dc/dA=dC/dOut(.)dOut/dA
		if (ret)
		{		
			ret = BL.dConvolutionW(inputImage, dpoolingIMG, bzactImage, dkernal, neuroNums,stride);
		}
	}
	else if (thisLayerType == layerType::FULLYCONNECTION)
	{
		ret = BL.dConvolutionW(inputImage, BL.dIdealOutVSdO, bzactImage, dkernal, neuroNums,stride);
	}
	else
	{
		ret = false;
	}

	if (ret)
	{
		for (int kn = 0; kn < neuroNums; kn++)
		{
			dkernal[kn].apply(-learnrate/bs);
			BL.ShadowMoment[kn] += dkernal[kn];
		}
	}
}
void CNNCalc::AccumulateDWSimd(float learnrate, int bs, int b)
{//update weights
	bool ret = true;
	for (int i = 0; i < neuroNums;i++)
	{
		dkernalSeries.at(b)[i].applySimd(0.0);
	}
	if (thisLayerType == layerType::CONVOLUTION)
	{
		ret = BL.dPoolingSimd(actImage, BL.dIdealOutVSdO, poolingRow, poolingCol, poolingStride, dpoolingIMG,b);
		//dc/dA=dC/dOut(.)dOut/dA
		if (ret)
		{
			ret = BL.dConvolutionWSimd(inputImage, dpoolingIMG, bzactImage, dkernalSeries.at(b), neuroNums, stride,b);
		}
	}
	else if (thisLayerType == layerType::FULLYCONNECTION)
	{
		ret = BL.dConvolutionWSimd(inputImage, BL.dIdealOutVSdO, bzactImage, dkernalSeries.at(b), neuroNums, stride,b);
	}
	else
	{
		ret = false;
	}
}
void CNNCalc::UpdateLayerWB()
{//SGD
	for (int k = 0; k < neuroNums; k++)
	{
		CNNKernals[k]+= dkernalSeries.at(0)[k];
	}
}


void CNNCalc::addDKernalShadow(int b)
{

	for (int k = 0;k < neuroNums;k++)
	{
			dkernalSeries.at(0)[k].addSimd(dkernalSeries.at(b)[k]);
	}
}
void CNNCalc::UpdateLayerWBSimd(float learnrate, float l2Lamda,int bs)
{
	for (int k = 0; k < neuroNums; k++)		
	{
		dkernalSeries.at(0)[k].applySimd(-learnrate / sqrt(bs));
		if (l2Lamda > SMALLZERO)
		{
			CNNKernals[k].applySimd(1-l2Lamda);
		}
		CNNKernals[k].addSimd(dkernalSeries.at(0)[k]);
	}
}

void CNNCalc::UpdateLayerWBSGDM(float learnrate,float beta1, float l2Lamda,int t)
{//SGDM
	for (int k = 0; k < neuroNums; k++)
	{
		BL.ShadowMoment[k].applySimd(beta1);
		if (l2Lamda > SMALLZERO)
		{
			dkernalSeries.at(1)[k].copySimd(CNNKernals[k]);
			dkernalSeries.at(1)[k].applySimd(l2Lamda);
			dkernalSeries.at(0)[k].addSimd(dkernalSeries.at(1)[k]);//weight decay
		}
		dkernalSeries.at(0)[k].applySimd((1 - beta1) / (1 - std::powf(beta1, t)));
		dkernalSeries.at(0)[k].addSimd(BL.ShadowMoment[k]);
		BL.ShadowMoment[k].copySimd(dkernalSeries.at(0)[k]);
		dkernalSeries.at(0)[k].applySimd(-learnrate);
		
		CNNKernals[k].addSimd(dkernalSeries.at(0)[k]);
	}
}
void CNNCalc::UpdateLayerWBSGDMW(float learnrate, float beta1, float l2Lamda, int t)
{//SGDM
	for (int k = 0; k < neuroNums; k++)
	{
		BL.ShadowMoment[k].applySimd(beta1);
		dkernalSeries.at(0)[k].applySimd((1 - beta1) / (1 - std::powf(beta1, t)));
		dkernalSeries.at(0)[k].addSimd(BL.ShadowMoment[k]);
		BL.ShadowMoment[k].copySimd(dkernalSeries.at(0)[k]);
		dkernalSeries.at(0)[k].applySimd(-learnrate);
		if (l2Lamda > SMALLZERO)
		{
			CNNKernals[k].applySimd(1 - l2Lamda);
		}

		CNNKernals[k].addSimd(dkernalSeries.at(0)[k]);
	}
}
void CNNCalc::UpdateLayerWBSGNAD(float learnrate, float beta1, float l2Lamda, int t)
{//SGNAD
	/*
	* m(t)=beta*m(t-1)+(1-beta)*(g(t)-g(t-1))/(1-beta^t)
	*/
	for (int k = 0; k < neuroNums; k++)
	{	
		BL.ShadowMoment[k].applySimd(beta1);//m(t-1)*beta
		dkernalSeries.at(0)[k].applySimd((1 - beta1) / (1 - std::powf(beta1, t)));//g(t)*(1-beta)/(1-beta^t)
		BL.ShadowMoment[k].addSimd(dkernalSeries.at(0)[k]);//m(t)=m(t-1)*beta+g(t)*(1-beta)/(1-beta^t)
		dkernalSeries.at(0)[k].applySimd(-learnrate);
		CNNKernals[k].addSimd(dkernalSeries.at(0)[k]);//w(t)-alpha*(1-beta)*g(t)

		dkernalSeries.at(0)[k].copySimd(BL.ShadowMoment[k]);//save m(t)' to dkernal
		dkernalSeries.at(0)[k].applySimd(-learnrate*beta1);// m(t)'*(alpha*beta) 
		CNNKernals[k].addSimd(dkernalSeries.at(0)[k]);//w(t)-alpha*(beta)*m(t)'
	}
}
void CNNCalc::UpdateLayerWBADAM(float learnrate, float beta1, float beta2, float sigma, float l2Lamda, int t)
{//SGNAD
	/*
	* m(t)=beta1*m(t-1)+(1-beta1)*g(t)
	* V(t)=max{V(t-1),beta2*V(t-1)+(1-beta2)*g(t)^2}
	*/
	for (int k = 0; k < neuroNums; k++)
	{
		BL.ShadowMoment[k].applySimd(beta1);//m(t-1)*beta
		if (l2Lamda > SMALLZERO)
		{
			dkernalSeries.at(1)[k].copySimd(CNNKernals[k]);
			dkernalSeries.at(1)[k].applySimd(l2Lamda);
			dkernalSeries.at(0)[k].addSimd(dkernalSeries.at(1)[k]);//weight decay
		}
		dkernalSeries.at(1)[k].copySimd(dkernalSeries.at(0)[k]);
		dkernalSeries.at(0)[k].applySimd((1 - beta1) / (1 - std::powf(beta1, t)));//g(t)*(1-beta)/(1-beta^t)
		BL.ShadowMoment[k].addSimd(dkernalSeries.at(0)[k]);//m(t)=m(t-1)*beta+g(t)*(1-beta)/(1-beta^t)

		BL.ShadowVelocity[k].applySimd(beta2);//beta2*V(t-1)
		dkernalSeries.at(1)[k].mulSimd(dkernalSeries.at(1)[k]);//gt*gt
		dkernalSeries.at(1)[k].applySimd((1 - beta2) / (1 - std::powf(beta2, t)));//g(t)*(1-beta2)/(1-beta2^t)
		BL.ShadowVelocity[k].addSimd(dkernalSeries.at(1)[k]);//V(t)
		dkernalSeries.at(1)[k].copySimd(BL.ShadowVelocity[k]);//V(t)

		dkernalSeries.at(1)[k].sqrtSimd();//sqrt(Vt')
		dkernalSeries.at(2)[k].copySimd(BL.ShadowMoment[k]);
		dkernalSeries.at(2)[k].divSimd(dkernalSeries.at(1)[k], sigma);//m(t)/(sqrt(t)+sigma)

		dkernalSeries.at(2)[k].applySimd(-learnrate);
		CNNKernals[k].addSimd(dkernalSeries.at(2)[k]);//w(t)-alpha*(1-beta)*g(t)

		dkernalSeries.at(1)[k].applySimd(0);
	}
}
void CNNCalc::UpdateLayerWBADAMW(float learnrate, float beta1,float beta2,float sigma, float l2Lamda, int t)
{//SGNAD
	/*
	* m(t)=beta1*m(t-1)+(1-beta1)*g(t)
	* V(t)=max{V(t-1),beta2*V(t-1)+(1-beta2)*g(t)^2}
	*/
	for (int k = 0; k < neuroNums; k++)
	{	
		BL.ShadowMoment[k].applySimd(beta1);//m(t-1)*beta
		dkernalSeries.at(1)[k].copySimd(dkernalSeries.at(0)[k]);
		dkernalSeries.at(0)[k].applySimd((1 - beta1) / (1 - std::powf(beta1, t)));//g(t)*(1-beta)/(1-beta^t)
		BL.ShadowMoment[k].addSimd(dkernalSeries.at(0)[k]);//m(t)=m(t-1)*beta+g(t)*(1-beta)/(1-beta^t)

		dkernalSeries.at(2)[k].copySimd(BL.ShadowVelocity[k]);
		BL.ShadowVelocity[k].applySimd(beta2);//beta2*V(t-1)
		dkernalSeries.at(1)[k].mulSimd(dkernalSeries.at(1)[k]);//gt*gt
		dkernalSeries.at(1)[k].applySimd((1 - beta2) / (1 - std::powf(beta2, t)));//g(t)*(1-beta2)/(1-beta2^t)
		BL.ShadowVelocity[k].addSimd(dkernalSeries.at(1)[k]);//V(t)
		//BL.ShadowVelocity[k].maxSimd(dkernalSeries.at(2)[k]);//max:V(t),V(t-1))
		dkernalSeries.at(1)[k].copySimd(BL.ShadowVelocity[k]);//V(t)

		dkernalSeries.at(1)[k].sqrtSimd();//sqrt(Vt')
		dkernalSeries.at(2)[k].copySimd(BL.ShadowMoment[k]);
		dkernalSeries.at(2)[k].divSimd(dkernalSeries.at(1)[k], sigma);//m(t)/(sqrt(t)+sigma)

		dkernalSeries.at(2)[k].applySimd(-learnrate);
		if (l2Lamda > SMALLZERO)
		{
			CNNKernals[k].applySimd(1 - l2Lamda);//weight decay
		}
		CNNKernals[k].addSimd(dkernalSeries.at(2)[k]);//w(t)-alpha*(1-beta)*g(t)

		dkernalSeries.at(1)[k].applySimd(0);
		dkernalSeries.at(2)[k].applySimd(0);
	}
}
bool CNNCalc::BackLayer::dConvolutionX(image inPa, image outZ,image bzactImage, kernal* K180, int Kn, int stride, image& dImage)
{//(4) 
	int kernalcol = K180[0].col;
	int kernalrow = K180[0].row;
	if (outZ.channel != Kn)
	{
		return false;
	}
	dactImage.apllyratio(0.0);
	dbzImage.apllyratio(0.0);
	bool ret = true;

	innerPadding(outZ, dactImage, stride - 1, stride - 1);
	innerPadding(bzactImage, dbzImage, stride - 1, stride - 1);

	dImage.rows = inPa.rows;
	dImage.cols = inPa.cols;
	dImage.channel = inPa.channel;
	dImage.initImage();
	int ConvolutionX = 0;
	int ConvolutionY = 0;
	for (int ch = 0; ch < inPa.channel; ch++)
	{
		ConvolutionX = 0;
		for (int r = 0; r < dactImage.rows - kernalrow+1; r += 1)
		{
			ConvolutionY = 0;
			for (int c = 0; c < dactImage.cols-kernalcol+1; c += 1)
			{
				float currentValue = 0.0;
				for (int k = 0; k < Kn; k++)
				{
					for (int kr = 0; kr < K180[k].row; kr++)
					{
						for (int kc = 0; kc < K180[k].col; kc++)
						{
							float dValue = dactivate(dbzImage.imageData[k][r + kr][c + kc], acttype)*dactImage.imageData[k][r + kr][c + kc];
							currentValue += K180[k].weight[ch][kr][kc] * dValue;
						}
					}
				}
				dImage.imageData[ch][ConvolutionX][ConvolutionY] = currentValue;
				ConvolutionY++;
			}
			ConvolutionX++;
		}
	}
	/*dactImage.freeImage();
	dbzImage.freeImage();*/
	return true;
}
bool CNNCalc::BackLayer::dConvolutionXSimd(image inPa, image outZ, image bzactImage, kernal* K180, int Kn, int stride, image& dImage, int b)
{//(4) 
	bool ret = true;
	int kernalcol = K180[0].col;
	int kernalrow = K180[0].row;
	if (outZ.channel != Kn)
	{
		std::cout << "ideal image size is not equal to the neuros~" << std::endl;
		ret= false;
	}
	dactImage.apllyratioSimd(0.0,b);
	dbzImage.apllyratioSimd(0.0,b);
	innerPaddingSimd(outZ, dactImage, stride - 1, stride - 1,b);
	innerPaddingSimd(bzactImage, dbzImage, stride - 1, stride - 1,b);
	if((dImage.rows == inPa.rows) && (dImage.cols == inPa.cols) && (dImage.channel == inPa.channel))
	{
		int offset = AlignBytes / sizeof(float);
		trpDim kernalSz(kernalrow, kernalcol, Kn);
		dactImage.im2row(kernalSz, 1,b);
		dbzImage.im2row(kernalSz, 1,b);
		int ConvolutionX = 0;
		int ConvolutionY = 0;
		size_t rowStrp = dactImage.rows - kernalrow + 1;
		size_t colStrp = dactImage.cols - kernalcol + 1;
		size_t reorderRow = rowStrp * colStrp;
		size_t reorderCol = AlignVec(kernalrow * kernalcol * Kn,offset);

		for (int ch = 0; ch < inPa.channel; ch++)
		{
			ConvolutionX = 0;
			ConvolutionY = 0;
			for (int ir = 0;ir < reorderRow;ir++)
			{
				float currentValue = 0.0;
				float* dbzImageBase = dbzImage.rowImg+ dbzImage .rowImgBlockSize*b+ ir * reorderCol;
				float* dactImageBase = dactImage.rowImg + dactImage.rowImgBlockSize * b + ir * reorderCol;
				float* cvWeright = K180[ch].vWeight;
				__m256 sumRegister = _mm256_setzero_ps();
				for (int ic = 0;ic < reorderCol;ic += offset)
				{
					__m256 dactLine = _mm256_load_ps(dactImageBase + ic);
					__m256 dbzactLine = _mm256_load_ps(dbzImageBase + ic);
					__m256 dbzactLineAct = dactivateSimd(dbzactLine ,acttype);
					__m256 wLine = _mm256_load_ps(cvWeright + ic);
					__m256 mulLine = _mm256_mul_ps(dactLine, dbzactLineAct);
					sumRegister = _mm256_fmadd_ps(mulLine, wLine, sumRegister);
				}
				sumRegister = _mm256_hadd_ps(sumRegister, sumRegister);
				sumRegister = _mm256_hadd_ps(sumRegister, sumRegister);
				currentValue += sumRegister.m256_f32[0];
				currentValue += sumRegister.m256_f32[4];
				if (ConvolutionY == colStrp)
				{
					ConvolutionY = 0;
					ConvolutionX++;
				}
				dImage.imageAtIndex(b)[ch + ConvolutionX * dImage.channel * dImage.cols + ConvolutionY * dImage.channel] = currentValue;
				ConvolutionY++;
			}
		}
	}
	else
	{
		std::cout <<"last dydx size is not equal to current layer input" << std::endl;
		ret = false;
	}


	return ret;
}

bool CNNCalc::BackLayer::dFullyConvolutionXSimd(image inPa, image outZ, image bzactImage, kernal* kernalSeries, int Kn, image& dImage, int b)
{
	bool ret = true;
	int kernalcol = kernalSeries[0].col;
	int kernalrow = kernalSeries[0].row;
	int kernalchannel = kernalSeries[0].channel;
	if (outZ.channel != Kn)
	{
		std::cout << "ideal image size is not equal to the neuros~" << std::endl;
		ret = false;
	}
	if (inPa.rows!= kernalrow || inPa.cols!= kernalcol || inPa.channel!= kernalchannel)
	{
		std::cout << "input image size is not equal to the kernals~" << std::endl;
		ret = false;
	}
	if (ret)
	{
		size_t offset = AlignBytes / sizeof(float);
		for (int i = 0;i < kernalchannel;i += offset)
		{
			__m256 sumR = _mm256_setzero_ps();
			for (int k = 0;k < Kn;k++)
			{
				__m256 outImageR = _mm256_set1_ps(outZ.imageAtIndex(b)[k]);
				__m256 bzRegister = _mm256_set1_ps(bzactImage.imageAtIndex(b)[k]);
				float* cWpointer = kernalSeries[k].vWeight + i;
				__m256 wRegister = _mm256_load_ps(cWpointer);
				__m256 dbzRegister = dactivateSimd(bzRegister, acttype);
				__m256 ddImage = _mm256_mul_ps(wRegister, dbzRegister);
				sumR = _mm256_fmadd_ps(ddImage, outImageR, sumR);
			}
			float* dst = dImage.imageAtIndex(b) + i;
			_mm256_stream_ps(dst, sumR);
		}
	}

	return ret;
}
bool CNNCalc::BackLayer::dConvolutionW(image inPa, image outZ, image bzactImage, kernal*& dkernal, int Kn, int stride)
{
	//dactImageW.apllyratio(0.0);
	//dbzImageW.apllyratio(0.0);

	innerPadding(outZ, dactImageW, stride - 1, stride - 1);
	innerPadding(bzactImage, dbzImageW, stride - 1, stride - 1);


	for (int k = 0; k < Kn; k++)
	{
		double subBiasValue = 0.0;
		for (int i = 0; i < dactImageW.rows; i++)
		{
			for (int j = 0; j < dactImageW.cols; j++)
			{
				double dvalue = dactImageW.imageData[k][i][j] * dactivate(dbzImageW.imageData[k][i][j], acttype);
				subBiasValue += dvalue;
			}
		}
		dkernal[k].bias = subBiasValue;
	}

	int WX = 0;
	int WY = 0;
	for (int k = 0; k < Kn; k++)
	{
		for (int ch = 0; ch < inPa.channel; ch++)
		{
			WX = 0;
			for (int r = 0; r < inPa.rows-dactImageW.rows+1; r++)
			{
				WY = 0;
				for (int c = 0; c < inPa.cols-dactImageW.cols+1; c++)
				{
					float currentValue = 0;
					for (int i = 0; i < dactImageW.rows; i++)
					{
						for (int j = 0; j < dactImageW.cols; j++)
						{
							float dvalue= dactivate(dbzImageW.imageData[k][i][j], acttype) * dactImageW.imageData[k][i][j];
							currentValue += inPa.imageData[ch][r + i][c + j] * dvalue;
						}
					}
					dkernal[k].weight[ch][WX][WY] = currentValue;
					WY++;
				}
				WX++;
			}
			
		}
	}
	return true;
}

bool CNNCalc::BackLayer::dConvolutionWSimd(image inPa, image outZ, image bzactImage, kernal*& dkernal, int Kn, int stride, int b)
{//ret = BL.dConvolutionWSimd(inputImage, dpoolingIMG, bzactImage, dkernal, neuroNums, stride);
	//dactImageW.apllyratioSimd(0.0);
	//dbzImageW.apllyratioSimd(0.0);
	innerPaddingSimd(outZ, dactImageW, stride - 1, stride - 1,b);
	innerPaddingSimd(bzactImage, dbzImageW, stride - 1, stride - 1,b);
	size_t offset = AlignBytes / sizeof(float);
	size_t offset16 = AlignBytes16 / sizeof(float);
	size_t cols = dactImageW.cols;
	size_t k = 0;
	float* VBias = (float*)_mm_malloc(Kn * sizeof(float), AlignBytes);
	for (;k + offset - 1 < Kn;k +=offset)
	{
		float* baseImgPointerAct = dactImageW.imageAtIndex(b) + k;
		float* baseImgPointerBaact = dbzImageW.imageAtIndex(b) + k;
		__m256 sumRegistr = _mm256_setzero_ps();
		for (size_t i = 0; i < dactImageW.rows; i++)
		{
			for (size_t j = 0; j < dactImageW.cols; j++)
			{
				__m256 currentLoadDact= _mm256_loadu_ps(baseImgPointerAct + j * Kn + i * Kn * cols);
				__m256 currentLoadBz= _mm256_loadu_ps(baseImgPointerBaact + j * Kn + i * Kn * cols);
				__m256 dactivateBz = dactivateSimd(currentLoadBz,acttype);
				sumRegistr = _mm256_fmadd_ps(dactivateBz, currentLoadDact, sumRegistr);
			}
		}
		_mm256_stream_ps(VBias + k, sumRegistr);
	}
	for (;k + offset16 - 1 < Kn;k += offset16)
	{
		float* baseImgPointerAct = dactImageW.imageAtIndex(b) + k;
		float* baseImgPointerBaact = dbzImageW.imageAtIndex(b) + k;
		__m128 sumRegistr = _mm_setzero_ps();
		for (size_t i = 0; i < dactImageW.rows; i++)
		{
			for (size_t j = 0; j < dactImageW.cols; j++)
			{
				__m128 currentLoadDact = _mm_loadu_ps(baseImgPointerAct + j * Kn + i * Kn * cols);
				__m128 currentLoadBz = _mm_loadu_ps(baseImgPointerBaact + j * Kn + i * Kn * cols);
				__m128 dactivateBz = dactivateSimd128(currentLoadBz, acttype);
				sumRegistr = _mm_fmadd_ps(dactivateBz, currentLoadDact, sumRegistr);
			}
		}
		_mm_stream_ps(VBias + k, sumRegistr);
	}
	for (;k < Kn;k += 1)
	{
		float* baseImgPointerAct = dactImageW.imageAtIndex(b) + k;
		float* baseImgPointerBaact = dbzImageW.imageAtIndex(b) + k;
		float sumRegistr = 0;
		for (size_t i = 0; i < dactImageW.rows; i++)
		{
			for (size_t j = 0; j < dactImageW.cols; j++)
			{
				float currentLoadDact = *(baseImgPointerAct + j * Kn + i * Kn * cols);
				float currentLoadBz = *(baseImgPointerBaact + j * Kn + i * Kn * cols);
				float dactivateBz = dactivate(currentLoadBz, acttype);
				sumRegistr = dactivateBz * currentLoadDact + sumRegistr;
			}
		}
		*(VBias + k)= sumRegistr;
	}
	for (k = 0;k < Kn;k += 1)
	{
		dkernal[k].biasSimd = VBias[k];
	}
	_mm_free(VBias);
	int WX = 0;
	int WY = 0;
	int kernalCol = dkernal[0].col;
	int kernalChannel = dkernal[0].channel;
	for (k = 0; k < Kn; k++)
	{
		int ch = 0;
		for (; ch+offset-1 < inPa.channel; ch+=offset)
		{
			WX = 0;
			for (int r = 0; r < inPa.rows - dactImageW.rows + 1; r++)
			{
				WY = 0;
				for (int c = 0; c < inPa.cols - dactImageW.cols + 1; c++)
				{
					float* baseImgPointerInpa = inPa.imageAtIndex(b) + ch+ c* inPa.channel+r* inPa.cols * inPa.channel;
					__m256 sumRegistr = _mm256_setzero_ps();
					float currentValue = 0;
					for (int i = 0; i < dactImageW.rows; i++)
					{
						for (int j = 0; j < dactImageW.cols; j++)
						{
							float* ImgPointerInpa = baseImgPointerInpa  +j * inPa.channel + i * inPa.cols * inPa.channel;
							int index = k + i * dactImageW.channel * dactImageW.cols + j * dactImageW.channel;
							float dvalue = dactivate(dbzImageW.imageAtIndex(b)[index], acttype) * dactImageW.imageAtIndex(b)[index];
							__m256 dvalueRgst = _mm256_set1_ps(dvalue);
							__m256 inpaRgst = _mm256_loadu_ps(ImgPointerInpa);
							//std::cout << dbzImageW.vImageData[index]<<","<<dactImageW.vImageData[index] << "," << dvalue << "," << inpaRgst.m256_f32[0] << std::endl;
							sumRegistr = _mm256_fmadd_ps(dvalueRgst, inpaRgst, sumRegistr);
						}
					}
					float* dkernalChannelLoad = dkernal[k].vWeight + ch + WX * kernalCol * kernalChannel + WY * kernalChannel;
					_mm256_storeu_ps(dkernalChannelLoad, sumRegistr);
					WY++;
				}
				WX++;
			}
		}
		for (; ch + offset16 - 1 < inPa.channel; ch+=offset16)
		{
			WX = 0;
			for (int r = 0; r < inPa.rows - dactImageW.rows + 1; r++)
			{
				WY = 0;
				for (int c = 0; c < inPa.cols - dactImageW.cols + 1; c++)
				{
					float* baseImgPointerInpa = inPa.imageAtIndex(b) + ch + c * inPa.channel + r * inPa.cols * inPa.channel;
					__m128 sumRegistr = _mm_setzero_ps();
					float currentValue = 0;
					for (int i = 0; i < dactImageW.rows; i++)
					{
						for (int j = 0; j < dactImageW.cols; j++)
						{
							float* ImgPointerInpa = baseImgPointerInpa + j * inPa.channel + i * inPa.cols * inPa.channel;
							int index = k + i * dactImageW.channel * dactImageW.cols + j * dactImageW.channel;
							float dvalue = dactivate(dbzImageW.imageAtIndex(b)[index], acttype) * dactImageW.imageAtIndex(b)[index];
							__m128 dvalueRgst = _mm_set1_ps(dvalue);
							__m128 inpaRgst = _mm_loadu_ps(ImgPointerInpa);
							sumRegistr = _mm_fmadd_ps(dvalueRgst, inpaRgst, sumRegistr);
						}
					}
					float* dkernalChannelLoad = dkernal[k].vWeight + ch + WX * kernalCol * kernalChannel + WY * kernalChannel;
					_mm_storeu_ps(dkernalChannelLoad, sumRegistr);
					WY++;
				}
				WX++;
			}

		}
		for (; ch < inPa.channel; ch+=1)
		{
			WX = 0;
			for (int r = 0; r < inPa.rows - dactImageW.rows + 1; r++)
			{
				WY = 0;
				for (int c = 0; c < inPa.cols - dactImageW.cols + 1; c++)
				{
					float* baseImgPointerInpa = inPa.imageAtIndex(b) + ch;
					float sumRegistr = 0;
					float currentValue = 0;
					for (int i = 0; i < dactImageW.rows; i++)
					{
						for (int j = 0; j < dactImageW.cols; j++)
						{
							float* ImgPointerInpa = baseImgPointerInpa + (c + j) * inPa.channel + (r + i) * cols * inPa.channel;
							int index = k + i * dactImageW.channel * dactImageW.cols + j * dactImageW.channel;
							float dvalue = dactivate(dbzImageW.imageAtIndex(b)[index], acttype) * dactImageW.imageAtIndex(b)[index];
							float inpaRgst = *(ImgPointerInpa);
							sumRegistr = dvalue * inpaRgst+ sumRegistr;
						}
					}
					float* dkernalChannelLoad = dkernal[k].vWeight + ch + WX * kernalCol * kernalChannel + WY * kernalChannel;
					*dkernalChannelLoad= sumRegistr;
					WY++;
				}
				WX++;
			}

		}
	}
	return true;
}
void CNNCalc::BackLayer::dPadding(image BeforePaddingZ, image& AfterPaddingZ, int padSizeR, int padSizeC)
{
	AfterPaddingZ.rows = BeforePaddingZ.rows + 2 * padSizeR;
	AfterPaddingZ.cols = BeforePaddingZ.cols + 2 * padSizeC;
	AfterPaddingZ.channel = BeforePaddingZ.channel;
	for (int ch = 0; ch < AfterPaddingZ.channel; ch++)
	{
		for (int r = 0; r < AfterPaddingZ.rows; r++)
		{
			for (int c = 0; c < AfterPaddingZ.cols; c++)
			{
				int rindex = 0;
				int cindex = 0;
				if (r < padSizeR)
				{
					rindex = 0;
				}
				else if (r> BeforePaddingZ.rows + padSizeR - 1)
				{
					rindex = BeforePaddingZ.rows - 1;
				}
				else
				{
					rindex = r-padSizeR;
				}
				if (c < padSizeC)
				{
					cindex = 0;
				}
				else if (c> BeforePaddingZ.cols + padSizeC - 1)
				{
					cindex = BeforePaddingZ.cols - 1;
				}
				else
				{
					cindex =c-padSizeC;
				}
				AfterPaddingZ.imageData[ch][r][c] = BeforePaddingZ.imageData[ch][rindex][cindex];	

			}
		}
	}
}
float CNNCalc::BackLayer::dactivate(float a, int fun)
{
	if (fun == 2)
	{//Relu
		if (a < 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else if (fun == 1)
	{
		float sx = 1.0 / (1 + exp(-a));
		return sx * (1 - sx);
	}
	else if (fun == 3)
	{
		return (exp(a) + exp(-a)) / (exp(a) - exp(-a));
	}
	else if (fun == 4)
	{
		if (a < 0)
		{
			//std::cout << a << std::endl;
			return exp(a);
		}
		else
		{
			return 1;
		}
	}
	else
	{
		if (a < 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}
__m128  CNNCalc::BackLayer::dactivateSimd128(__m128 a, int fun)
{
	__m128 zero = _mm_setzero_ps();
	
	if (fun == 2)
	{//Relu
		__m128 cmpRegister= _mm_cmp_ps(zero, a, 1);
		return _mm_and_ps(cmpRegister, _mm_set1_ps(1));
	}
	else if (fun == 1)
	{
		__m128 ones = _mm_set1_ps(1.0);
		a=_mm_sub_ps(zero, a);
		a=_mm_exp_ps(a);
		a = _mm_add_ps(ones, a);
		a = _mm_div_ps(ones, a);
		a = _mm_fmsub_ps(a, a, a);
		a = _mm_sub_ps(zero, a);
		return a;
	}
	else if (fun == 3)
	{
		__m128 inva= _mm_sub_ps(zero, a);
		__m128 expa = _mm_exp_ps(a);
		__m128 expinva = _mm_exp_ps(inva);
		__m128 expadd = _mm_add_ps(expa, expinva);
		__m128 expsub = _mm_add_ps(expa, expinva);
		return  _mm_div_ps(expa, expinva);
	}
	else if (fun == 4)
	{//modified relu   x<0?exp(x):1
		__m128 cmpValue1 = _mm_cmp_ps(a, zero, 1);
		//a<0?1:0
		//__m128 cmpValue2 = _mm_cmp_ps(zero, a, 2);//a<0?0:1
		//__m128i mask2 = _mm_castps_si128(cmpValue2);
		//__m128i cmpValue1And2 = _mm_and_si128(mask2, _mm_set1_epi32(1));
		//__m128 mask1Ps2 = _mm_cvtepi32_ps(cmpValue1And2);

		__m128 mask1Ps2 = _mm_andnot_ps(cmpValue1,_mm_set1_ps(1));
		__m128 expa = _mm_exp_ps(a);

		__m128 andFloat = _mm_and_ps(cmpValue1, expa);//<0:exp(a);>0:0
		return _mm_add_ps(andFloat, mask1Ps2);
	}
	else
	{
		return a;
	}
}
__m256  CNNCalc::BackLayer::dactivateSimd(__m256 a, int fun)
{
	__m256 zero = _mm256_setzero_ps();

	if (fun == 2)
	{//Relu
		__m256 cmpRegister = _mm256_cmp_ps(zero, a, 1);
		return _mm256_and_ps(cmpRegister, _mm256_set1_ps(1));
	}
	else if (fun == 1)
	{
		__m256 ones = _mm256_set1_ps(1.0);
		a = _mm256_sub_ps(zero, a);
		a = _mm256_exp_ps(a);
		a = _mm256_add_ps(ones, a);
		a = _mm256_div_ps(ones, a);
		a = _mm256_fmsub_ps(a, a, a);
		a = _mm256_sub_ps(zero, a);
		return a;
	}
	else if (fun == 3)
	{
		__m256 inva = _mm256_sub_ps(zero, a);
		__m256 expa = _mm256_exp_ps(a);
		__m256 expinva = _mm256_exp_ps(inva);
		__m256 expadd = _mm256_add_ps(expa, expinva);
		__m256 expsub = _mm256_add_ps(expa, expinva);
		return  _mm256_div_ps(expa, expinva);
	}
	else if (fun == 4)
	{//modified relu   x<0?exp(x):1
		/**/
		__m256 cmpValue1 = _mm256_cmp_ps(a, zero, 1);
		__m256 mask1Ps2 = _mm256_andnot_ps(cmpValue1, _mm256_set1_ps(1));
		__m256 expa = _mm256_exp_ps(a);

		__m256 andFloat = _mm256_and_ps(cmpValue1, expa);//<0:exp(a);>0:0
		return _mm256_add_ps(andFloat, mask1Ps2);
	}
	else
	{
		return a;
	}
}
bool CNNCalc::BackLayer::dPooling(image actImage, image DyDxoutImage, int poolingdim1, int poolingdim2, int poolingstride, image& dPoolingImage)
{
	int outIndexRow = 0;
	int outIndexCol = 0;
	
	dPoolingImage.apllyratio(0.0);
	for (int ch = 0; ch < actImage.channel; ch++)
	{
		outIndexRow = 0;
		for (int r = 0; r < actImage.rows- poolingdim1 +1; r += poolingstride)
		{
			outIndexCol = 0;
			for (int c = 0; c < actImage.cols- poolingdim2 +1; c += poolingstride)
			{
				float currentMaxValue = actImage.imageData[ch][r][c];
				int maxXValueIndex =0;
				int maxYValueIndex = 0;

				for (int i = 0; i < poolingdim1; i++)
				{
					for (int j = 0; j < poolingdim2; j++)
					{
						if (actImage.imageData[ch][r + i][c + j]>currentMaxValue)
						{
							currentMaxValue = actImage.imageData[ch][r + i][c + j];
							maxXValueIndex = i;
							maxYValueIndex = j;
						}
					}
				}
				dPoolingImage.imageData[ch][r + maxXValueIndex][c + maxYValueIndex] += DyDxoutImage.imageData[ch][outIndexRow][outIndexCol];
				outIndexCol++;
			}
			outIndexRow++;
		}
	}
	return true;
}
bool CNNCalc::BackLayer::dPoolingSimd(image actImage, image DyDxoutImage, int poolingdim1, int poolingdim2, int poolingstride, image& dPoolingImage, int b)
{
	//ret = BL.dPooling(actImage, BL.dIdealOutVSdO, poolingRow, poolingCol, poolingStride, dpoolingIMG);
	int outIndexRow = 0;
	int outIndexCol = 0;
	size_t offset = AlignBytes / (sizeof(float));
	size_t offset16 = AlignBytes16 / (sizeof(float));
	size_t rstridEnd = (actImage.rows - poolingdim1) / poolingstride + 1;
	size_t cstridEnd = (actImage.cols - poolingdim2) / poolingstride + 1;
	size_t actsmallstrp = actImage.channel * actImage.cols;
	dPoolingImage.apllyratioSimd(0.0,b);
	int ch = 0;
	const int PO = 0;
	for (; ch+offset-1 < actImage.channel; ch+= offset)
	{
		for (int xstrp = 0; xstrp < rstridEnd; xstrp += 1)
		{
			for (int ystrp = 0; ystrp < cstridEnd; ystrp += 1)
			{
				float* curActBase = actImage.imageAtIndex(b) + xstrp * poolingstride* actsmallstrp + ystrp * poolingstride * actImage.channel+ch;
				__m256 maxReg = _mm256_loadu_ps(curActBase);
				for (int i = 0; i < poolingdim1; i++)
				{
					for (int j = 0; j < poolingdim2; j++)
					{
						__m256 simdVarb = _mm256_loadu_ps(curActBase + i * actsmallstrp + j * actImage.channel);
						maxReg = _mm256_max_ps(simdVarb, maxReg);
					}
				}
				__m256 DyDxValue = _mm256_loadu_ps(DyDxoutImage.imageAtIndex(b) +ch + xstrp * cstridEnd * actImage.channel + ystrp * actImage.channel);
				float* curdPoolBase = dPoolingImage.imageAtIndex(b) + xstrp * poolingstride * actsmallstrp + ystrp * poolingstride * actImage.channel + ch;
				for (int i = 0; i < poolingdim1; i++)
				{
					for (int j = 0; j < poolingdim2; j++)
					{
						__m256 simdVarb = _mm256_loadu_ps(curActBase + i * actsmallstrp + j * actImage.channel);
						__m256 dPoolomg = _mm256_loadu_ps(curdPoolBase + i * actsmallstrp + j * actImage.channel);
						__m256 cmpMatrix0 = _mm256_cmp_ps(maxReg, simdVarb, PO);
						__m256 cmpMatrixPs = _mm256_and_ps(cmpMatrix0, DyDxValue);
						dPoolomg = _mm256_add_ps(cmpMatrixPs, dPoolomg);
						_mm256_storeu_ps(curdPoolBase + i * actsmallstrp + j * actImage.channel, dPoolomg);
					}
				}
			}
		}
	}
	for (; ch + offset16 - 1 < actImage.channel; ch += offset16)
	{
		for (int xstrp = 0; xstrp < rstridEnd; xstrp += 1)
		{
			for (int ystrp = 0; ystrp < cstridEnd; ystrp += 1)
			{
				float* curActBase = actImage.imageAtIndex(b) + xstrp * poolingstride * actsmallstrp + ystrp * poolingstride * actImage.channel + ch;
				__m128 maxReg = _mm_loadu_ps(curActBase);
				for (int i = 0; i < poolingdim1; i++)
				{
					for (int j = 0; j < poolingdim2; j++)
					{
						__m128 simdVarb = _mm_loadu_ps(curActBase + i * actsmallstrp + j * actImage.channel);
						maxReg = _mm_max_ps(simdVarb, maxReg);
					}
				}
				__m128 DyDxValue = _mm_loadu_ps(DyDxoutImage.imageAtIndex(b) +ch+xstrp* cstridEnd* actImage.channel+ystrp*actImage.channel);
				float* curdPoolBase = dPoolingImage.imageAtIndex(b) + xstrp * poolingstride * actsmallstrp + ystrp * poolingstride * actImage.channel + ch;
				for (int i = 0; i < poolingdim1; i++)
				{
					for (int j = 0; j < poolingdim2; j++)
					{
						__m128 simdVarb = _mm_loadu_ps(curActBase + i * actsmallstrp + j * actImage.channel);
						__m128 dPoolomg = _mm_loadu_ps(curdPoolBase + i * actsmallstrp + j * actImage.channel);
						__m128 cmpMatrix0 = _mm_cmp_ps(maxReg, simdVarb, PO);
						__m128 cmpMatrixPs = _mm_and_ps(cmpMatrix0, DyDxValue);
						dPoolomg = _mm_add_ps(cmpMatrixPs, dPoolomg);
						_mm_storeu_ps(curdPoolBase + i * actsmallstrp + j * actImage.channel, dPoolomg);
					}
				}
			}
		}
	}
	for (; ch < actImage.channel; ch +=1)
	{
		for (int xstrp = 0; xstrp < rstridEnd; xstrp += 1)
		{
			for (int ystrp = 0; ystrp < cstridEnd; ystrp += 1)
			{
				float* curActBase = actImage.imageAtIndex(b) + xstrp * poolingstride * actsmallstrp + ystrp * poolingstride * actImage.channel + ch;
				float maxReg = *(curActBase);
				for (int i = 0; i < poolingdim1; i++)
				{
					for (int j = 0; j < poolingdim2; j++)
					{
						float simdVarb = *(curActBase + i * actsmallstrp + j * actImage.channel);
						maxReg = std::max(simdVarb, maxReg);
					}
				}
				float DyDxValue = DyDxoutImage.imageAtIndex(b)[ch + xstrp * cstridEnd * actImage.channel + ystrp * actImage.channel];
				float* curdPoolBase = dPoolingImage.imageAtIndex(b) + xstrp * poolingstride * actsmallstrp + ystrp * poolingstride * actImage.channel + ch;
				for (int i = 0; i < poolingdim1; i++)
				{
					for (int j = 0; j < poolingdim2; j++)
					{
						float simdVarb = *(curActBase + i * actsmallstrp + j * actImage.channel);
						float cmpMatrix = simdVarb<maxReg?0:1;
						*(curdPoolBase + i * actsmallstrp + j * actImage.channel) += cmpMatrix*DyDxValue;
					}
				}
			}
		}
	}
	return true;
}
void CNNCalc::BackLayer::innerPadding(image beforePaddingZ, image& AfterPaddingZ,int innerSizeRow,int innerSizeCol)
{
	int innerpaddingszRow = (beforePaddingZ.rows - 1) * innerSizeRow + beforePaddingZ.rows;
	int innerpaddingszCol = (beforePaddingZ.cols - 1) * innerSizeCol + beforePaddingZ.cols;
	int edgeRow = (AfterPaddingZ.rows - innerpaddingszRow) / 2;
	int edgeCol = (AfterPaddingZ.cols - innerpaddingszCol) / 2;
	for (int ch = 0; ch < beforePaddingZ.channel; ch++)
	{
		for (int r = 0; r < beforePaddingZ.rows; r += 1)
		{
			for (int c = 0; c < beforePaddingZ.cols; c += 1)
			{
				int currentRow = r * (innerSizeRow + 1) + edgeRow;
				int currentCol = c * (innerSizeCol + 1) + edgeCol;
				AfterPaddingZ.imageData[ch][currentRow][currentCol] = beforePaddingZ.imageData[ch][r][c];
			}
		}
	}
}
void CNNCalc::BackLayer::innerPaddingSimd(image beforePaddingZ, image& AfterPaddingZ, int innerSizeRow, int innerSizeCol, int b)
{
	int innerpaddingszRow = (beforePaddingZ.rows - 1) * innerSizeRow + beforePaddingZ.rows;
	int innerpaddingszCol = (beforePaddingZ.cols - 1) * innerSizeCol + beforePaddingZ.cols;
	int edgeRow = (AfterPaddingZ.rows - innerpaddingszRow) / 2;
	int edgeCol = (AfterPaddingZ.cols - innerpaddingszCol) / 2;
	//printf("%d,%d,%d,%d\n", innerpaddingszRow, innerpaddingszCol, edgeRow,edgeCol);
	
	if ((innerSizeRow > 0) || (innerSizeCol > 0))
	{
		for (int r = 0; r < beforePaddingZ.rows; r += 1)
		{
			for (int c = 0; c < beforePaddingZ.cols; c += 1)
			{
				int currentRow = r * (innerSizeRow + 1) + edgeRow;
				int currentCol = c * (innerSizeCol + 1) + edgeCol;
				float* src = beforePaddingZ.imageAtIndex(b) + r * beforePaddingZ.channel * beforePaddingZ.cols + c * beforePaddingZ.channel;
				float* dst = AfterPaddingZ.imageAtIndex(b) + currentRow * AfterPaddingZ.channel * AfterPaddingZ.cols + currentCol * AfterPaddingZ.channel;
				memcpy(dst, src, beforePaddingZ.channel * sizeof(float));
			}
		}
	}
	else
	{
		for (int r = 0; r < beforePaddingZ.rows; r += 1)
		{
				int currentRow = r * (innerSizeRow + 1) + edgeRow;
				float* src = beforePaddingZ.imageAtIndex(b) + r * beforePaddingZ.channel * beforePaddingZ.cols;
				float* dst = AfterPaddingZ.imageAtIndex(b) + currentRow * AfterPaddingZ.channel * AfterPaddingZ.cols + edgeCol * AfterPaddingZ.channel;
				memcpy(dst, src, beforePaddingZ.cols * beforePaddingZ.channel * sizeof(float));
		}
	}
	
}

bool CNNCalc::BackLayer::outPadding(image beforePaddingZ, image& AfterPaddingZ, int targetRow, int targetCol)
{
	
	AfterPaddingZ.channel = beforePaddingZ.channel;
	AfterPaddingZ.rows = targetRow;
	AfterPaddingZ.cols = targetCol;
	AfterPaddingZ.initImage();
	
	//std::cout << "target size ::" << targetRow << "," << targetCol << std::endl;
	bool isBeforeEvenRow = (1.0 * beforePaddingZ.rows / 2 - beforePaddingZ.rows / 2) > 0.25;
	bool isBeforeEvenCol = (1.0 * beforePaddingZ.cols / 2 - beforePaddingZ.cols / 2) > 0.25;
	bool isAfterEvenRow = (1.0 * AfterPaddingZ.rows / 2 - AfterPaddingZ.rows / 2) > 0.25;
	bool isAfterEvenCol = (1.0 * AfterPaddingZ.cols / 2 - AfterPaddingZ.cols / 2) > 0.25;
	

	if ((isBeforeEvenRow == isAfterEvenRow) && (isBeforeEvenCol == isAfterEvenCol))
	{
		int edgeRow = (targetRow - beforePaddingZ.rows) / 2;
		int edgeCol = (targetCol - beforePaddingZ.cols) / 2;
		for (int ch = 0; ch < beforePaddingZ.channel; ch++)
		{
			for (int r = edgeRow; r < targetRow - edgeRow; r++)
			{
				for (int c = edgeCol; c < targetCol - edgeCol; c++)
				{
					AfterPaddingZ.imageData[ch][r][c] = beforePaddingZ.imageData[ch][r - edgeRow][c - edgeCol];
				}
			}
		}
		return true;
	}
	else
	{
		std::cout << "outpadding input image and output image are nor to judge even or odd" << std::endl;
		return false;
	}

}
bool CNNCalc::BackLayer::outPaddingSimd(image beforePaddingZ, image& AfterPaddingZ, int targetRow, int targetCol)
{


	bool isBeforeEvenRow = (1.0 * beforePaddingZ.rows / 2 - beforePaddingZ.rows / 2) > 0.25;
	bool isBeforeEvenCol = (1.0 * beforePaddingZ.cols / 2 - beforePaddingZ.cols / 2) > 0.25;
	bool isAfterEvenRow = (1.0 * AfterPaddingZ.rows / 2 - AfterPaddingZ.rows / 2) > 0.25;
	bool isAfterEvenCol = (1.0 * AfterPaddingZ.cols / 2 - AfterPaddingZ.cols / 2) > 0.25;

	int BeforeRowLength = beforePaddingZ.cols * beforePaddingZ.channel;
	int AfterRowLength = AfterPaddingZ.cols * AfterPaddingZ.channel;

	if ((isBeforeEvenRow == isAfterEvenRow) && (isBeforeEvenCol == isAfterEvenCol))
	{
		int edgeRow = (targetRow - beforePaddingZ.rows) / 2;
		int edgeCol = (targetCol - beforePaddingZ.cols) / 2;
		for (int r = edgeRow; r < targetRow - edgeRow; r++)
		{
			float* src = beforePaddingZ.vImageData + (r - edgeRow) * BeforeRowLength;
			float* dst = AfterPaddingZ.vImageData + r * AfterRowLength + edgeCol * AfterPaddingZ.channel;
			memcpy(dst, src, BeforeRowLength*sizeof(float));
		}
		return true;
	}
	else
	{
		std::cout << "outpadding input image and output image are nor to judge even or odd" << std::endl;
		return false;
	}

}
bool CNNCalc::BackLayer::TMatrixKernal(const kernal* kernalSeris, kernal* retKernal180, int neuroNums)
{
	//kernal* retKernal180 = new kernal[neuroNums];
	bool ret = true;
	int channel = retKernal180[0].channel;
	int row = retKernal180[0].row;
	int col = retKernal180[0].col;
	for (int i = 0; i < neuroNums; i++)
	{
		for (int ch = 0; ch < channel; ch++)
		{
			for (int r = 0; r < row; r++)
			{
				for (int c = 0; c < col; c++)
				{
					retKernal180[i].weight[ch][r][c] = kernalSeris[i].weight[ch][row - r-1][col-c-1];
				}
			}
		}
	}
	return true;
}
bool CNNCalc::BackLayer::TMatrixKernalSimd(const kernal* kernalSeris, kernal* retKernal180, int neuroNums)
{
	//kernal aKernal;
	
	int channel = kernalSeris[0].channel;
	int row = kernalSeris[0].row;
	int col = kernalSeris[0].col;
	for (int ch = 0;ch < channel;ch++)
	{
		for (int i = 0; i < neuroNums; i++)
		{
			for (int c = 0; c < col; c++)
			{
				for (int r = 0; r < row; r++)
				{
					float* src = kernalSeris[i].vWeight + (row - r-1) * channel * col + (col - c-1) * channel;
					*(aKernal.vWeight + r * col * neuroNums + c * neuroNums+i) = *(src + ch);
				}
			}
		}
		memcpy(retKernal180[ch].vWeight, aKernal.vWeight, row * neuroNums * col * sizeof(float));		
	}
	
	return true;
}
CNNCalc::BackLayer::BackLayer()
{ 
	acttype = 4;
	learnRate = 0.01; 
}
__m256 CNNCalc::BackLayer::_mm256_exp_ps_ft(__m256 a)
{
	return _mm256_set_ps
	(
		expf(a.m256_f32[0]),
		expf(a.m256_f32[1]),
		expf(a.m256_f32[2]),
		expf(a.m256_f32[3]),
		expf(a.m256_f32[4]),
		expf(a.m256_f32[5]),
		expf(a.m256_f32[6]),
		expf(a.m256_f32[7])
	);
}

__m128 CNNCalc::BackLayer::_mm_exp_ps_ft(__m128 a)
{
	return _mm_set_ps
	(
		expf(a.m128_f32[0]),
		expf(a.m128_f32[1]),
		expf(a.m128_f32[2]),
		expf(a.m128_f32[3])
	);
	//_mm_exp_ps(_mm_set1_ps(1));
}
