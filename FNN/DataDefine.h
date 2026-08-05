#pragma once
#include<iostream>
#include<vector>
#include<fstream>
#include<ctime>
#include"immintrin.h"
#include<intrin.h>
#include<iomanip>
#include<getCpuId.h>
#include<sstream>
#include<random>
#define AlignBytes 32 
#define AlignBytesPool 32 
#define AlignBytes16 16 
#define TESTLOOP 1000
#define LOGPRE 4
#define AlignVec(x,V) ((x)%(V)==0?(x):((x)/(V)+1)*(V))

static std::default_random_engine gen;
typedef struct trpDim
{
	int row;
	int col;
	int channel;
	trpDim() :
		row(0),
		col(0),
		channel(0)
	{}
	trpDim(int a, int b, int c) :
		row(a),
		col(b),
		channel(c)
	{}
}trpDim;


typedef struct image
{
	size_t rows;
	size_t cols;
	size_t channel;
	size_t blockSize;
	size_t rowImgBlockSize;
	int bs;
	float* vImageData;
	float* rowImg;
	image() :
		rows(0),
		cols(0),
		channel(0),
		blockSize(0),
		rowImgBlockSize(0),
		vImageData(NULL),
		rowImg(NULL),
		bs(1)
	{}
	image(int r, int c, int ch) :
		rows(r),
		cols(c),
		channel(ch)
	{
		initImage();
	}
	void setSize(int d1, int d2, int d3)
	{
		rows = d1;
		cols = d2;
		channel = d3;
	}
	float* imageAtIndex(size_t b)
	{
		return vImageData + b * blockSize;
	}
	bool initImage(float sd = 0,int batchSize=1)
	{
		if ((rows > 0) && (cols > 0) && (channel > 0))
		{

			size_t offset = AlignBytes / sizeof(float);
		//	blockSize = AlignVec(rows * cols * channel, offset);
			blockSize = AlignVec(rows * cols * channel, offset);
			vImageData = (float*)_mm_malloc(batchSize * blockSize * sizeof(float), AlignBytes);
			memset(vImageData, 0, batchSize * blockSize * sizeof(float));
			bs = batchSize;
			return true;
		}
		else
		{
			return false;
		}
	}
	void freeImage()
	{
		if (vImageData != NULL)
		{
			_mm_free(vImageData);
			vImageData = NULL;
		}
		if (rowImg != NULL)
		{
			_mm_free(rowImg);
			rowImg = NULL;
		}
		channel = 0;
		cols = 0;
		rows = 0;
	}
	void freeImageSpace()
	{
		if (vImageData != NULL)
		{
			_mm_free(vImageData);
			vImageData = NULL;
		}
		if (rowImg != NULL)
		{
			_mm_free(rowImg);
			rowImg = NULL;
		}
	}
	void initRowImage(trpDim kernalDim, int MovStride,int batchSize=1)
	{
		size_t kernalRows = kernalDim.row;
		size_t kernalCols = kernalDim.col;
		size_t kernalChannel = kernalDim.channel;
		size_t offset = AlignBytes / sizeof(float);
		size_t reorderCol = AlignVec(kernalRows * kernalCols * kernalChannel, offset);

		size_t rowstrp = (rows - kernalRows) / MovStride + 1;
		size_t colstrp = (cols - kernalCols) / MovStride + 1;
		size_t reorderRow = rowstrp * colstrp;

		rowImgBlockSize = reorderRow * reorderCol;
		rowImg = (float*)_mm_malloc(batchSize*rowImgBlockSize * sizeof(float), AlignBytes);
		memset(rowImg, 0, batchSize * rowImgBlockSize * sizeof(float));
	}
	bool im2row(trpDim kernalDim, int MovStride, int b = 0)
	{
		if (rowImg != NULL)
		{
			float* rowImgBase = rowImg + rowImgBlockSize * b;
			float* vImgBase = vImageData + blockSize * b;
			size_t offset = AlignBytes / sizeof(float);
			int tmpIndeY = (cols - kernalDim.col) / MovStride + 1;
			int blockSize = AlignVec(kernalDim.row * kernalDim.col * kernalDim.channel, offset);
			size_t RowStrpEnd = rows - kernalDim.row + 1;
			size_t ColStrpEnd = cols - kernalDim.col + 1;
			size_t smStep = kernalDim.channel * kernalDim.col;
			int OutBuffer1IndexX = 0;
			int prow = 0;
			int innkerIK = 0;
			for (int r = 0; r < RowStrpEnd; r += MovStride)
			{
				int OutBuffer1IndexY = 0;
				for (int c = 0; c < ColStrpEnd; c += MovStride)
				{
					for (int kr = 0; kr < kernalDim.row; kr++)
					{
						prow = OutBuffer1IndexX * tmpIndeY + OutBuffer1IndexY;
						innkerIK = kr * smStep;
						memcpy(rowImgBase + prow * blockSize + innkerIK, vImgBase + (r + kr) * kernalDim.channel * cols + c * kernalDim.channel, smStep * sizeof(float));
					}
					OutBuffer1IndexY++;

				}
				OutBuffer1IndexX++;
			}
		}
		else 
		{
			std::cout << "error: rowImg is not initialized~" << std::endl;
			return false;
		}
		return true;

	}
	void addSimd(const image im,int b=0)
	{
		if ((channel == im.channel) && (rows == im.rows) && (cols == im.cols))
		{
			size_t offset = AlignBytes / sizeof(float);
			int length = rows * cols * channel;
			for (int i = 0;i < length;i += offset)
			{
				__m256 line1 = _mm256_load_ps(vImageData + blockSize * b +i);
				__m256 line2 = _mm256_load_ps(im.vImageData+im.blockSize*b+i);
				__m256 sumRegister = _mm256_add_ps(line1, line2);
				_mm256_stream_ps(vImageData + blockSize * b + i, sumRegister);

			}
		}
		else
		{
			std::cout << "error: funcion add, images those are not same in size operate adding" << std::endl;
			std::cout << channel<<","<<rows<<","<<cols << std::endl;
			std::cout << im.channel << "," << im.rows << "," << im.cols << std::endl;
		}
	}
	void apllyratioSimd(double rs,int b=0)
	{
		if ((rows > 0) && (cols > 0) && (channel > 0)  && (NULL != vImageData))
		{
			size_t offset = AlignBytes / sizeof(float);
			size_t offset16 = AlignBytes16 / sizeof(float);

			int length = rows * cols * channel;
			__m256 rsRegister256 = _mm256_set1_ps(rs);
			__m128 rsRegister = _mm_set1_ps(rs);
			for (int i = 0;i < length;i += offset)
			{
				__m256 line1 = _mm256_load_ps(vImageData+blockSize*b + i);
				__m256 lineMul = _mm256_mul_ps(line1, rsRegister256);
				_mm256_stream_ps(vImageData + blockSize * b + i, lineMul);
			}
		}
	}
	image retApllyratioSimd(double rs)
	{
		image im;
		im.cols = cols;
		im.rows = rows;
		im.channel = channel;
		im.initImage();
		if ((rows > 0) && (cols > 0) && (channel > 0)  && (NULL != vImageData))
		{
			size_t offset = AlignBytes / sizeof(float);
			size_t offset16 = AlignBytes16 / sizeof(float);

			int length = rows * cols * channel;
			__m256 rsRegister256 = _mm256_set1_ps(rs);
			__m128 rsRegister = _mm_set1_ps(rs);
			for (int i = 0;i < length;i += offset)
			{
				__m256 line1 = _mm256_load_ps(vImageData + i);
				__m256 lineMul = _mm256_mul_ps(line1, rsRegister256);
				_mm256_stream_ps(im.vImageData + i, lineMul);
			}
		}
		return im;
	}
	void printImage(int fg,const char* imagename)
	{

		std::ofstream fo(".\\w.log", std::ios::app);
		fo << imagename << std::endl;
		for (int ch = 0; ch < channel; ch++)
		{
			fo << "channel:" << ch << std::endl;
			for (int r = 0; r < rows; r++)
			{
				for (int c = 0; c < cols; c++)
				{
					fo << std::setprecision(LOGPRE) << vImageData[ch + r * channel * cols + c * channel] << ",";
				}
			}
			fo << std::endl << std::endl;
		}
		fo.close();
	}
}image;
typedef struct kernal
{
	size_t row;
	size_t col;
	size_t channel;
	float* vWeight;
	float bias;
	float biasSimd;
	kernal() :
		row(0),
		col(0),
		channel(0),
		bias(0.0),
		vWeight(NULL),
		biasSimd(0.0)
	{}
	void initKernal(float sd)
	{
	
		size_t offset = AlignBytes / sizeof(float);
		size_t blockSoze = AlignVec(row * channel * col, offset);
		//std::cout << blockSoze << std::endl;
		vWeight = (float*)_mm_malloc(blockSoze * sizeof(float), AlignBytes);
		for (int i = 0;i < blockSoze;i++)
		{
			vWeight[i] = 0.0;
		}
		if (sd > 0)
		{
			

			std::normal_distribution<float> nd(0, sd);
			for (int ch = 0; ch < channel; ch++)
			{
				for (int r = 0; r < row; r++)
				{
					for (int c = 0; c < col;c++)
					{
						float randfloat = nd(gen);
						vWeight[r * channel * col + c * channel + ch] = randfloat;

					}
				}
			}
		}
		else
		{//it must be initialized to zero if sd=0
			for (int ch = 0; ch < channel; ch++)
			{
				for (int r = 0; r < row; r++)
				{
					for (int c = 0; c < col;c++)
					{
						vWeight[r * channel * col + c * channel + ch] = 0.0;

					}
				}
			}
		}
		bias = 0.0;
		biasSimd = 0.0;
	}
	void FreeWtsSpace()
	{
		if (NULL != vWeight)
		{
			_mm_free(vWeight);
			vWeight = NULL;
		}
		row = 0;
		col = 0;
		channel = 0;
	}
	void FreeWts()
	{

		if (NULL != vWeight)
		{
			_mm_free(vWeight);
			vWeight = NULL;
		}
	}
	void addSimd(kernal k1)
	{
		if ((vWeight != NULL) && (k1.vWeight != NULL) && (row * col * channel == k1.channel * k1.col * k1.row))
		{
			int length = row * col * channel;
			int offset = AlignBytes / sizeof(float);
			for (int i = 0;i  < length;i += offset)
			{
				__m256 line1 = _mm256_load_ps(vWeight + i);
				__m256 line2 = _mm256_load_ps(k1.vWeight + i);
				__m256 sumline = _mm256_add_ps(line1, line2);
				_mm256_stream_ps(vWeight + i, sumline);
			}
			biasSimd += k1.biasSimd;
		}
		else
		{
			std::cout << "kernal plus not equal" << std::endl;
		}
	}
	void maxSimd(kernal k1)
	{
		if ((vWeight != NULL) && (k1.vWeight != NULL) && (row * col * channel == k1.channel * k1.col * k1.row))
		{
			int length = row * col * channel;
			int offset = AlignBytes / sizeof(float);
			for (int i = 0;i< length;i += offset)
			{
				__m256 line1 = _mm256_load_ps(vWeight + i);
				__m256 line2 = _mm256_load_ps(k1.vWeight + i);
				__m256 maxline = _mm256_max_ps(line1, line2);
				_mm256_stream_ps(vWeight + i, maxline);
			}
			biasSimd = std::max(biasSimd, k1.biasSimd);
		}
		else
		{
			std::cout << "kernal plus not equal" << std::endl;
		}
	}
	void divSimd(kernal k1,float sigma)
	{
		if ((vWeight != NULL) && (k1.vWeight != NULL) && (row * col * channel == k1.channel * k1.col * k1.row))
		{
			int length = row * col * channel;
			int offset = AlignBytes / sizeof(float);
			__m256 sigmaR = _mm256_set1_ps(sigma);
			for (int i = 0;i < length;i += offset)
			{
				__m256 line1 = _mm256_load_ps(vWeight + i);
				__m256 line2 = _mm256_load_ps(k1.vWeight + i);
				line2=_mm256_add_ps(line2, sigmaR);
				__m256 sumline = _mm256_div_ps(line1, line2);
				_mm256_stream_ps(vWeight + i, sumline);
			}
		}
		else
		{
			std::cout << "kernal div not equal" << std::endl;
		}
		biasSimd /= (k1.biasSimd+sigma);
	}
	void mulSimd(kernal k1)
	{
		if ((vWeight != NULL) && (k1.vWeight != NULL) && (row * col * channel == k1.channel * k1.col * k1.row))
		{
			int length = row * col * channel;
			int offset = AlignBytes / sizeof(float);
			for (int i = 0;i< length;i += offset)
			{
				__m256 line1 = _mm256_load_ps(vWeight + i);
				__m256 line2 = _mm256_load_ps(k1.vWeight + i);
				__m256 sumline = _mm256_mul_ps(line1, line2);
				_mm256_stream_ps(vWeight + i, sumline);
			}
		}
		else
		{
			std::cout << "kernal mul not equal" << std::endl;
		}
		biasSimd *= k1.biasSimd;
	}
	void applySimd(float ra)
	{
		if (vWeight != NULL)
		{
			int length = row * col * channel;
			int offset = AlignBytes / sizeof(float);
			__m256 mra = _mm256_set1_ps(ra);
			for (int i = 0;i < length;i += offset)
			{
				__m256 line1 = _mm256_load_ps(vWeight + i);
				__m256 sumline = _mm256_mul_ps(line1, mra);
				_mm256_stream_ps(vWeight + i, sumline);
			}
			biasSimd *= ra;
		}
	}
	void sqrtSimd()
	{
		if (vWeight != NULL)
		{
			int length = row * col * channel;
			int offset = AlignBytes / sizeof(float);
			for (int i = 0;i < length;i += offset)
			{
				__m256 line1 = _mm256_load_ps(vWeight + i);
				__m256 sumline = _mm256_sqrt_ps(line1);
				_mm256_stream_ps(vWeight + i, sumline);
			}
			biasSimd = std::sqrt(biasSimd);
		}
	}
	void copySimd(kernal k1)
	{
		if ((vWeight != NULL) && (k1.vWeight != NULL) && (row * col * channel == k1.channel * k1.col * k1.row))
		{
			int length = row * col * channel;
			int offset = AlignBytes / sizeof(float);
			for (int i = 0;i < length;i += offset)
			{
				__m256 line2 = _mm256_load_ps(k1.vWeight + i);
				_mm256_stream_ps(vWeight + i, line2);
			}
		}
		else
		{
			std::cout << "kernal copy not equal" << std::endl;
		}
		biasSimd = k1.biasSimd;
	}

	void printKernal(int i,const char* kernalname)
	{
		std::ofstream fo(".\\weight.log", std::ios::app);
		fo << kernalname << std::endl;
		for (int ch = 0; ch < channel; ch++)
		{
			fo << "kernal channel:" << ch << std::endl;
			for (int r = 0; r < row; r++)
			{
				for (int c = 0; c < col; c++)
				{
					 fo << std::setprecision(LOGPRE) << vWeight[ch + r * col * channel + c * channel] << ",";
				}
			}
			fo << std::endl;
		}
		if (i == 0) fo << std::setprecision(LOGPRE) << "bias:" << bias << std::endl << std::endl;
		else fo << std::setprecision(LOGPRE) << "biasSimd:" << bias << std::endl << std::endl;
		fo.close();
	}
}kernal;
typedef enum
{
	SMALLER = 0,
	SAME,
	BIGGER

}PaddingMethod;
typedef enum
{
	INPUTLayer = 0,
	CONVOLUTION,
	FULLYCONNECTION,
	OUTPUT

}layerType;


typedef struct layerHead
{
	char layerType;
	int inputRows;
	int inputCols;
	int inputChannel;

	char paddingMethod;

	char pollingFun;
	int poolingRow;
	int poolingCol;

	int kernalNumbers;
	int kernalRow;
	int kernalCol;
	int kernalChannel;

	char actType;

	int outRow;
	int outCol;
	int outChannel;

}layerHead;

typedef struct modelHead
{
	char Version[20];
	char trainTime[100];
	char trainPersonName[20];

	int inputRows;
	int inputCols;
	int inputChannel;
	char inputNormaMethod;

	int layerDepth;

	int outRow;
	int outCol;
	int outChannel;
	char outNormMethod;

}modelHead;