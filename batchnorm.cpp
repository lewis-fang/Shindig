#include"batchnorm.h"
batchnorm::batchnorm()
{
	featureLen = 0;
	mu = NULL;
	sigma = NULL;
	//gamma = NULL;
//	beta = NULL;
	mavmu = NULL;
	mavsigma = NULL;
	paraw = 0;
	parab = 0;
	smallvalue = 1e-6;
	alpha = 0.8;
	BNPos = BNUnkown;
	/*
	* 0: no batch norm
	* 1: pre act
	* 2: post act
	*/
	batchSize = 0;
}
void batchnorm::setBatchNorm(int len, BNPosition bnp, float sv)
{
	featureLen = len;
	//batchsize = bsz;
	int offset = AlignBytes / sizeof(float);
	int spaceLen = AlignVec(featureLen, offset);

	BNPos = bnp;//default: postActivate
	smallvalue = sv;

	
}
void batchnorm::linkImage(const image preImage)
{
	batchnormInputImage.rows = preImage.rows;
	batchnormInputImage.cols = preImage.cols;
	batchnormInputImage.channel = preImage.channel;	
}
void batchnorm::initSpace(int bs)
{
	int offset = AlignBytes / sizeof(float);
	int spaceLen = AlignVec(featureLen, offset);
	mu = (float*)_mm_malloc(spaceLen * sizeof(float), AlignBytes);
	sigma = (float*)_mm_malloc(spaceLen * sizeof(float), AlignBytes);
	mavmu = (float*)_mm_malloc(spaceLen * sizeof(float), AlignBytes);
	mavsigma = (float*)_mm_malloc(spaceLen * sizeof(float), AlignBytes);
	//gamma = (float*)_mm_malloc(spaceLen * sizeof(float), AlignBytes);
	//beta = (float*)_mm_malloc(spaceLen * sizeof(float), AlignBytes);

	paraw = (float*)_mm_malloc(spaceLen * sizeof(float), AlignBytes);
	parab = (float*)_mm_malloc(spaceLen * sizeof(float), AlignBytes);


	memset(mu, 0, spaceLen * sizeof(float));
	for (int i = 0;i < featureLen;i++)
	{
		sigma[i] = 1.0;
	}
	memset(mavmu, 0, spaceLen * sizeof(float));
	memset(mavsigma, 0, spaceLen * sizeof(float));
	memset(paraw, 1, spaceLen * sizeof(float));
	memset(parab, 0, spaceLen * sizeof(float));

	gamma.channel = featureLen;
	gamma.col = 1;
	gamma.row = 1;
	gamma.initKernal(1.0);
	beta.channel = featureLen;
	beta.col = 1;
	beta.row = 1;
	beta.initKernal(0);

	for (int b = 0;b < bs;b++)
	{
		kernal idg,idb;
		idg.channel = featureLen;
		idg.col = 1;
		idg.row = 1;
		idg.initKernal(0);

		idb.channel = featureLen;
		idb.col = 1;
		idb.row = 1;
		idb.initKernal(0);

		dg.push_back(idg);
		db.push_back(idb);
	}

	batchSize = bs;
	printf("BN batch size is set to %d\n", batchSize);

	shadowGM.channel = featureLen;
	shadowGM.col = 1;
	shadowGM.row = 1;
	shadowGM.initKernal(0);
	shadowGV.channel = featureLen;
	shadowGV.col = 1;
	shadowGV.row = 1;
	shadowGV.initKernal(0);
	shadowBM.channel = featureLen;
	shadowBM.col = 1;
	shadowBM.row = 1;
	shadowBM.initKernal(0);
	shadowBV.channel = featureLen;
	shadowBV.col = 1;
	shadowBV.row = 1;
	shadowBV.initKernal(0);

	tmp1.channel = featureLen;
	tmp1.col = 1;
	tmp1.row = 1;
	tmp1.initKernal(0);
	tmp2.channel = featureLen;
	tmp2.col = 1;
	tmp2.row = 1;
	tmp2.initKernal(0);

	batchnormInputImage.initImage(0.0,batchSize);
}
void batchnorm::preCalcParas()
{
	int offset = AlignBytes / sizeof(float);
	int spaceLen = AlignVec(featureLen, offset);
	float* gammadata = gamma.vWeight;
	float* betadata = beta.vWeight;
	for (int i = 0;i < spaceLen;i += offset)
	{
		__m256 regSigma = _mm256_load_ps(sigma + i);
		__m256 regGamma = _mm256_load_ps(gammadata + i);
		__m256 regMu = _mm256_load_ps(mu + i);
		__m256 regBeta = _mm256_load_ps(betadata + i);

		regSigma = _mm256_fmadd_ps(regSigma, regSigma, _mm256_set1_ps(smallvalue));
		regSigma = _mm256_sqrt_ps(regSigma);

		__m256 reg1 = _mm256_div_ps(regGamma, regSigma);
		_mm256_stream_ps(paraw + i, reg1);

		__m256 reg2 = _mm256_div_ps(regMu, reg1);
		reg2 = _mm256_sub_ps(regBeta, reg2);
		_mm256_stream_ps(parab + i, reg2);
		// = (regInData - regmu) / regsigma * reggamma + regbeta;

	}
}
void batchnorm::deleteSpace()
{
	if (!mu)
	{
		delete mu;
		mu = NULL;
	}
	if (!sigma)
	{
		delete sigma;
		sigma = NULL;
	}
	gamma.FreeWtsSpace();
	beta.FreeWtsSpace();
	shadowGM.FreeWtsSpace();
	shadowGV.FreeWtsSpace();
	shadowBM.FreeWtsSpace();
	shadowBV.FreeWtsSpace();
	tmp1.FreeWtsSpace();
	tmp2.FreeWtsSpace();

	if (dg.size() == batchSize)
	{
		for (int i = 0;i < batchSize;i++)
		{
			dg.at(i).FreeWtsSpace();
		}
		dg.clear();
	}
	if (db.size() == batchSize)
	{
		for (int i = 0;i < batchSize;i++)
		{
			db.at(i).FreeWtsSpace();
		}
		db.clear();
	}
	
	if (!paraw)
	{
		delete paraw;
		paraw = NULL;
	}
	if (!parab)
	{
		delete parab;
		parab = NULL;
	}
	featureLen = 0;
	batchnormInputImage.freeImageSpace();
}

bool batchnorm::updateBNParas(image preImage)
{
	int bs = batchSize;
	int offset = AlignBytes / sizeof(float);
	int offset16 = AlignBytes16 / sizeof(float);
	int row = preImage.rows;
	int col = preImage.cols;
	int channel = preImage.channel;
	int ch = 0;
	int N =  (bs * row * col);
	for (;ch+ offset -1 < channel;ch += offset)
	{
		__m256 sumreg = _mm256_setzero_ps();
		__m256 sumreg2 = _mm256_setzero_ps();
		__m256 Nreg = _mm256_set1_ps(1.0 / N);
		__m256 alphareg = _mm256_set1_ps(alpha);
		__m256 invalphareg = _mm256_set1_ps(1 - alpha);
		for (int b = 0;b < bs;b++)
		{
			float* curInputPos = preImage.imageAtIndex(b);
			for (int r = 0;r < row;r++)
			{
				for (int c = 0;c < col;c++)
				{
					int posShift = r * col * channel + c * channel + ch;
					__m256 regInData = _mm256_loadu_ps(curInputPos + posShift);
					sumreg = _mm256_add_ps(sumreg, regInData);
					sumreg2 = _mm256_fmadd_ps(regInData, regInData, sumreg2);
				}
			}
		}
		sumreg = _mm256_mul_ps(sumreg, Nreg);
		_mm256_stream_ps(mu + ch, sumreg);

		__m256 movmureg = _mm256_load_ps(mavmu + ch);
		__m256 premovstep = _mm256_mul_ps(movmureg, alphareg);
		__m256 postmovstep = _mm256_mul_ps(sumreg, invalphareg);
		movmureg = _mm256_add_ps(premovstep, postmovstep);
		_mm256_stream_ps(mavmu + ch, movmureg);

		sumreg2 = _mm256_mul_ps(sumreg2, Nreg);
		sumreg = _mm256_mul_ps(sumreg, sumreg);
		sumreg2 = _mm256_sub_ps(sumreg2, sumreg);
		sumreg2 = _mm256_sqrt_ps(sumreg2);
		_mm256_stream_ps(sigma + ch, sumreg2);

		__m256 movsigmareg = _mm256_load_ps(mavsigma + ch);
		movsigmareg = _mm256_mul_ps(movsigmareg, alphareg);
		sumreg2 = _mm256_mul_ps(sumreg2, invalphareg);
		movsigmareg = _mm256_add_ps(movsigmareg, sumreg2);
		_mm256_stream_ps(mavsigma + ch, movsigmareg);
	}
	for (;ch + offset16 - 1 < channel;ch += offset16)
	{
		__m128 sumreg = _mm_setzero_ps();
		__m128 sumreg2 = _mm_setzero_ps();
		__m128 Nreg = _mm_set1_ps(1.0 / N);
		__m128 alphareg = _mm_set1_ps(alpha);
		__m128 invalphareg = _mm_set1_ps(1 - alpha);
		for (int b = 0;b < bs;b++)
		{
			float* curInputPos = preImage.imageAtIndex(b);
			for (int r = 0;r < row;r++)
			{
				for (int c = 0;c < col;c++)
				{
					int posShift = r * col * channel + c * channel + ch;
					__m128 regInData = _mm_loadu_ps(curInputPos + posShift);
					sumreg = _mm_add_ps(sumreg, regInData);
					sumreg2 = _mm_fmadd_ps(regInData, regInData, sumreg2);
				}
			}
		}
		sumreg = _mm_mul_ps(sumreg, Nreg);
		_mm_stream_ps(mu + ch, sumreg);

		__m128 movmureg = _mm_load_ps(mavmu + ch);
		__m128 premovstep = _mm_mul_ps(movmureg, alphareg);
		__m128 postmovstep = _mm_mul_ps(sumreg, invalphareg);
		movmureg = _mm_add_ps(premovstep, postmovstep);
		_mm_stream_ps(mavmu + ch, movmureg);

		sumreg2 = _mm_mul_ps(sumreg2, Nreg);
		sumreg = _mm_mul_ps(sumreg, sumreg);
		sumreg2 = _mm_sub_ps(sumreg2, sumreg);
		sumreg2 = _mm_sqrt_ps(sumreg2);
		_mm_stream_ps(sigma + ch, sumreg2);

		__m128 movsigmareg = _mm_load_ps(mavsigma + ch);
		movsigmareg = _mm_mul_ps(movsigmareg, alphareg);
		sumreg2 = _mm_mul_ps(sumreg2, invalphareg);
		movsigmareg = _mm_add_ps(movsigmareg, sumreg2);
		_mm_stream_ps(mavsigma + ch, movsigmareg);
	}
	for (;ch < channel;ch += 1)
	{
		float sumreg = 0.0;
		float sumreg2 = 0.0;
		float Nreg = (1.0 / N);
		float alphareg = (alpha);
		float invalphareg = (1 - alpha);
		for (int b = 0;b < bs;b++)
		{
			float* curInputPos = preImage.imageAtIndex(b);
			for (int r = 0;r < row;r++)
			{
				for (int c = 0;c < col;c++)
				{
					int posShift = r * col * channel + c * channel + ch;
					float regInData = *(curInputPos + posShift);
					sumreg = (sumreg + regInData);
					sumreg2 = (regInData * regInData + sumreg2);
				}
			}
		}
		sumreg = (sumreg * Nreg);
		*(mu + ch) = sumreg;

		float movmureg = *(mavmu + ch);
		float premovstep = (movmureg * alphareg);
		float postmovstep = (sumreg * invalphareg);
		movmureg = (premovstep + postmovstep);
		*(mavmu + ch) = movmureg;

		sumreg2 = (sumreg * Nreg);
		sumreg = (sumreg * sumreg);
		sumreg2 = (sumreg2 - sumreg);
		sumreg2 = std::sqrt(sumreg2);
		*(sigma + ch) = sumreg2;

		float movsigmareg = *(mavsigma + ch);
		movsigmareg = (movsigmareg * alphareg);
		sumreg2 = (sumreg2 * invalphareg);
		movsigmareg = (movsigmareg + sumreg2);
		*(mavsigma + ch) = movsigmareg;
	}
	return true;
}

bool batchnorm::calcBN(TrainOrInference trainOrInference, image preImage, int b)
{
	switch (trainOrInference)
	{
	case train://train
		return	calcBatchNormTrain(preImage, b);
		break;
	case inference:
		return calcBatchNormInference(preImage, b);
		break;
	default:
		printf("trainOrInference wrong para\n");
		return false;
	}
}
bool batchnorm::calcBatchNormTrain(image preImage, int b)
{
	if (featureLen == 0)
	{
		printf("your batch norm is not ready");
		return false;
	}
	int row = preImage.rows;
	int col = preImage.cols;
	int channel = preImage.channel;

	float* gammadata = gamma.vWeight;
	float* betadata = beta.vWeight;
	memcpy(batchnormInputImage.imageAtIndex(b), preImage.imageAtIndex(b), batchnormInputImage.blockSize * sizeof(float));
	if (channel != featureLen)
	{
		printf("channel is not equal to featurelen: %d, %d ", channel, featureLen);
		return false;
	}
	int offset = AlignBytes / sizeof(float);
	int offset16 = AlignBytes16 / sizeof(float);
	int spaceLen = AlignVec(featureLen, offset);
	float* curInputPos = batchnormInputImage.imageAtIndex(b);
	float* curOutPos = preImage.imageAtIndex(b);
	int ch = 0;
	__m256 regSv = _mm256_set1_ps(smallvalue);
	__m128 regSv16 = _mm_set1_ps(smallvalue);

	for (;ch + offset - 1 < featureLen;ch += offset)
	{
		__m256 regmu = _mm256_load_ps(mu + ch);
		__m256 regsigma = _mm256_load_ps(sigma + ch);
		__m256 reggamma = _mm256_load_ps(gammadata + ch);
		__m256 regbeta = _mm256_load_ps(betadata + ch);
		for (int r = 0;r < row;r++)
		{
			for (int c = 0;c < col;c++)
			{
				int posShift = r * col * channel + c * channel + ch;
				__m256 regInData = _mm256_loadu_ps(curInputPos + posShift);
				__m256 reg = _mm256_sub_ps(regInData, regmu);

				regsigma = _mm256_fmadd_ps(regsigma, regsigma, regSv);
				regsigma = _mm256_sqrt_ps(regsigma);

				reg = _mm256_div_ps(reg, regsigma);
				reg = _mm256_mul_ps(reg, reggamma);
				reg = _mm256_add_ps(reg, regbeta);

				_mm256_storeu_ps(curOutPos + posShift, reg);
			}
		}

	}
	for (;ch + offset16 - 1 < featureLen;ch += offset16)
	{
		__m128 regmu = _mm_load_ps(mu + ch);
		__m128 regsigma = _mm_load_ps(sigma + ch);
		__m128 reggamma = _mm_load_ps(gammadata + ch);
		__m128 regbeta = _mm_load_ps(betadata + ch);
		for (int r = 0;r < row;r++)
		{
			for (int c = 0;c < col;c++)
			{
				int posShift = r * col * channel + c * channel + ch;
				__m128 regInData = _mm_loadu_ps(curInputPos + posShift);
				__m128 reg = _mm_sub_ps(regInData, regmu);

				regsigma = _mm_fmadd_ps(regsigma, regsigma, regSv16);
				regsigma = _mm_sqrt_ps(regsigma);

				reg = _mm_div_ps(reg, regsigma);
				reg = _mm_mul_ps(reg, reggamma);
				reg = _mm_add_ps(reg, regbeta);

				_mm_storeu_ps(curOutPos + posShift, reg);
			}
		}
	}
	for (;ch < featureLen;ch += 1)
	{
		float regmu = *(mu + ch);
		float regsigma = *(sigma + ch);
		float reggamma = *(gammadata + ch);
		float regbeta = *(betadata + ch);
		for (int r = 0;r < row;r++)
		{
			for (int c = 0;c < col;c++)
			{
				int posShift = r * col * channel + c * channel + ch;
				float regInData = *(curInputPos + posShift);
				regsigma = std::sqrt(regsigma * regsigma + smallvalue);
				float reg = (regInData - regmu) / regsigma * reggamma + regbeta;
				*(curOutPos + posShift) = reg;
			}
		}
	}
	return true;
}

bool batchnorm::calcBatchNormInference(image preImage, int b)
{//it should pre calced
	if (featureLen == 0)
	{
		printf("your batch norm is not ready");
		return false;
	}
	int row = preImage.rows;
	int col = preImage.cols;
	int channel = preImage.channel;

	if (channel != featureLen)
	{
		printf("channel is not equal to featurelen: %d, %d ", channel, featureLen);
		return false;
	}
	int offset = AlignBytes / sizeof(float);
	int offset16 = AlignBytes16 / sizeof(float);
	int spaceLen = AlignVec(featureLen, offset);
	float* curInputPos = preImage.imageAtIndex(b);
	float* curOutPos = preImage.imageAtIndex(b);

	int ch = 0;
	__m256 regSv = _mm256_set1_ps(smallvalue);
	__m128 regSv16 = _mm_set1_ps(smallvalue);

	for (;ch + offset - 1 < featureLen;ch += offset)
	{
		__m256 regW = _mm256_load_ps(paraw + ch);
		__m256 regB = _mm256_load_ps(parab + ch);
		for (int r = 0;r < row;r++)
		{
			for (int c = 0;c < col;c++)
			{
				int posShift = r * col * channel + c * channel + ch;
				__m256 regInData = _mm256_load_ps(curInputPos + posShift);
				__m256 reg = _mm256_fmadd_ps(regInData, regW, regB);
				_mm256_stream_ps(curOutPos + posShift, reg);
			}
		}

	}
	for (;ch + offset16 - 1 < featureLen;ch += offset16)
	{
		__m128 regW = _mm_load_ps(paraw + ch);
		__m128 regB = _mm_load_ps(parab + ch);
		for (int r = 0;r < row;r++)
		{
			for (int c = 0;c < col;c++)
			{
				int posShift = r * col * channel + c * channel + ch;
				__m128 regInData = _mm_load_ps(curInputPos + posShift);
				__m128 reg = _mm_fmadd_ps(regInData, regW, regB);

				_mm_stream_ps(curOutPos + posShift, reg);
			}
		}
	}
	for (;ch < featureLen;ch += 1)
	{
		float regW = *(paraw + ch);
		float regB = *(parab + ch);
		for (int r = 0;r < row;r++)
		{
			for (int c = 0;c < col;c++)
			{
				int posShift = r * col * channel + c * channel + ch;
				float regInData = *(curInputPos + posShift);
				float reg = regInData * regW + regB;
				//float reg = (regInData - regmu) / regsigma * reggamma + regbeta;
				*(curOutPos + posShift) = reg;
			}
		}
	}
	return true;
}

bool batchnorm::derivativeDLDx(image dLdy, image& dLdx, int b)
{
	int bs = batchSize;
	int offset = AlignBytes / sizeof(float);
	int offset16 = AlignBytes16 / sizeof(float);
	int row = batchnormInputImage.rows;
	int col = batchnormInputImage.cols;
	int channel = batchnormInputImage.channel;

	int N = (bs * row * col);
	int ch = 0;
	__m256 svreg = _mm256_set1_ps(smallvalue);
	__m128 svreg16 = _mm_set1_ps(smallvalue);
	for (;ch + offset - 1 < featureLen;ch += offset)
	{
		__m256 sum1 = _mm256_setzero_ps();
		__m256 sum2 = _mm256_setzero_ps();
		__m256 Nreg = _mm256_set1_ps(1.0 / N);

		__m256 mureg = _mm256_load_ps(mu + ch);
		__m256 musigma = _mm256_load_ps(sigma + ch);
		musigma = _mm256_add_ps(musigma, svreg);
		__m256 	P = _mm256_fmadd_ps(musigma, musigma, svreg);
		__m256 	PP = _mm256_mul_ps(P, P);
		for (int b = 0; b < bs;b++)
		{
			float* datareg = dLdy.imageAtIndex(b);
			float* inputreg = batchnormInputImage.imageAtIndex(b);
			for (int r = 0;r < row;r++)
			{
				for (int c = 0;c < col;c++)
				{
					int posShift = r * col * channel + c * channel + ch;
					__m256 reglossData = _mm256_loadu_ps(datareg + posShift);
					__m256 regInData = _mm256_loadu_ps(inputreg + posShift);
					regInData = _mm256_sub_ps(regInData, mureg);
					__m256 tmp = _mm256_mul_ps(reglossData, regInData);
					sum1 = _mm256_add_ps(tmp, sum1);
					sum2 = _mm256_add_ps(reglossData, sum2);
				}
			}
		}
		sum1 = _mm256_mul_ps(sum1, Nreg);
		sum1 = _mm256_div_ps(sum1, P);
		for (int b = 0; b < bs;b++)
		{
			float* datareg = dLdy.imageAtIndex(b);
			float* inputreg = batchnormInputImage.imageAtIndex(b);
			for (int r = 0;r < row;r++)
			{
				for (int c = 0;c < col;c++)
				{
					int posShift = r * col * channel + c * channel + ch;
					__m256 reglossData = _mm256_loadu_ps(datareg + posShift);
					__m256 regInData = _mm256_loadu_ps(inputreg + posShift);
					reglossData = _mm256_div_ps(reglossData, P);

					regInData = _mm256_sub_ps(regInData, mureg);
					regInData = _mm256_mul_ps(regInData, Nreg);
					regInData = _mm256_div_ps(regInData, PP);
					regInData = _mm256_sub_ps(_mm256_set1_ps(1.0), regInData);
					regInData = _mm256_mul_ps(regInData, sum1);
					__m256 tmp = _mm256_sub_ps(reglossData, regInData);
					float* outlosss = dLdx.imageAtIndex(b) + posShift;
					_mm256_stream_ps(outlosss, tmp);
				}
			}
		}
	}
	for (;ch + offset16 - 1 < featureLen;ch += offset16)
	{
		__m128 sum1 = _mm_setzero_ps();
		__m128 sum2 = _mm_setzero_ps();
		__m128 Nreg = _mm_set1_ps(1.0 / N);

		__m128 mureg = _mm_load_ps(mu + ch);
		__m128 musigma = _mm_load_ps(sigma + ch);
		musigma = _mm_add_ps(musigma, svreg16);
		__m128 	P = _mm_fmadd_ps(musigma, musigma, svreg16);
		__m128 	PP = _mm_mul_ps(P, P);
		for (int b = 0; b < bs;b++)
		{
			float* datareg = dLdy.imageAtIndex(b);
			float* inputreg = batchnormInputImage.imageAtIndex(b);
			for (int r = 0;r < row;r++)
			{
				for (int c = 0;c < col;c++)
				{
					int posShift = r * col * channel + c * channel + ch;
					__m128 reglossData = _mm_loadu_ps(datareg + posShift);
					__m128 regInData = _mm_loadu_ps(inputreg + posShift);
					regInData = _mm_sub_ps(regInData, mureg);
					__m128 tmp = _mm_mul_ps(reglossData, regInData);
					sum1 = _mm_add_ps(tmp, sum1);
					sum2 = _mm_add_ps(reglossData, sum2);
				}
			}
		}
		sum1 = _mm_mul_ps(sum1, Nreg);
		sum1 = _mm_div_ps(sum1, P);
		for (int b = 0; b < bs;b++)
		{
			float* datareg = dLdy.imageAtIndex(b);
			float* inputreg = batchnormInputImage.imageAtIndex(b);
			for (int r = 0;r < row;r++)
			{
				for (int c = 0;c < col;c++)
				{
					int posShift = r * col * channel + c * channel + ch;
					__m128 reglossData = _mm_loadu_ps(datareg + posShift);
					__m128 regInData = _mm_loadu_ps(inputreg + posShift);
					reglossData = _mm_div_ps(reglossData, P);

					regInData = _mm_sub_ps(regInData, mureg);
					regInData = _mm_mul_ps(regInData, Nreg);
					regInData = _mm_div_ps(regInData, PP);
					regInData = _mm_sub_ps(_mm_set1_ps(1.0), regInData);
					regInData = _mm_mul_ps(regInData, sum1);
					__m128 tmp = _mm_sub_ps(reglossData, regInData);
					float* outlosss = dLdx.imageAtIndex(b) + posShift;
					_mm_stream_ps(outlosss, tmp);
				}
			}
		}
	}
	for (;ch < featureLen;ch += 1)
	{
		float sum1 = 0.0;
		float sum2 = 0.0;
		float Nreg = (1.0 / N);

		float mureg = *(mu + ch);
		float musigma = *(sigma + ch);
		musigma = (musigma + smallvalue);
		float 	P = (musigma + musigma * smallvalue);
		float 	PP = (P * P);
		for (int b = 0; b < bs;b++)
		{
			float* datareg = dLdy.imageAtIndex(b);
			float* inputreg = batchnormInputImage.imageAtIndex(b);
			for (int r = 0;r < row;r++)
			{
				for (int c = 0;c < col;c++)
				{
					int posShift = r * col * channel + c * channel + ch;
					float reglossData = *(datareg + posShift);
					float regInData = *(inputreg + posShift);
					regInData = (regInData - mureg);
					float tmp = (reglossData * regInData);
					sum1 = (tmp + sum1);
					sum2 = (reglossData + sum2);
				}
			}
		}
		sum1 = (sum1 * Nreg);
		sum1 = (sum1 / P);
		for (int b = 0; b < bs;b++)
		{
			float* datareg = dLdy.imageAtIndex(b);
			float* inputreg = batchnormInputImage.imageAtIndex(b);
			for (int r = 0;r < row;r++)
			{
				for (int c = 0;c < col;c++)
				{
					int posShift = r * col * channel + c * channel + ch;
					float reglossData = *(datareg + posShift);
					float regInData = *(inputreg + posShift);
					reglossData = (reglossData / P);

					regInData = (regInData - mureg);
					regInData = (regInData * Nreg);
					regInData = (regInData / PP);
					regInData = ((1.0) - regInData);
					regInData = (regInData * sum1);
					float tmp = (reglossData - regInData);
					float* outlosss = dLdx.imageAtIndex(b) + posShift;
					*(outlosss) = tmp;
				}
			}
		}
	}
	return true;
}
/* N=bs*row* col
* u=sum(x)/N   dim=channel
* s=sum(x*x)-sum(x)*sum(x)
* xx=(x-u)/sqrt(s+sv)
* y=xx*g+b
* dL/dxi=sum(dL/dyj*dyj/dxxj * dxxj/dxi)=sum(dL/dyj *gamma *dxxj/dxi )=gamma*sum(dL/dyj * dxxj/dxi)
*				i=j:			dxxi/dxi=(1-1/N)*sqrt(s+sv)-(xj-u)*(-1/2)/sqrt(s+sv)*2*(xi-u)/N      ///(s+sv)
 *				i!=j:			dxxj/dxi=-1/N*sqrt(s+sv)-(xj-u)*(-1/2)/sqrt(s+sv)*2*(xi-u)/N           ///(s+sv)
*                            P=sqrt(s+sv)
*						i=j		=(1-1/N)*P+(xj-u)/P/N     ///(PP)
*						i!=j       =-1/N*P+		(xj-u)/P/N   ///(PP)
*
*                      =dL/dyi*P///PP-1/N*P*sum(dL/dyj)///(PP)   +sum(dL/dyj *(xi-u))/P/N ///PP
*						=dL/dyi*P///PP-sum(dL/dyj)/N///(P)   +sum(dL/dyj *(xi-u))/N ///PPP
*						=dL / dyi // P - sum(dL / dyj) / N///(P)  (1-  (xi-u))/N ///PP)

* */

bool batchnorm::derivativeDLDw(image dLdy, int b)
{
	int offset = AlignBytes / sizeof(float);
	int offset16 = AlignBytes16 / sizeof(float);
	int row = batchnormInputImage.rows;
	int col = batchnormInputImage.cols;
	int channel = batchnormInputImage.channel;
	float* datareg = dLdy.imageAtIndex(b);
	float* inputreg = batchnormInputImage.imageAtIndex(b);
	int ch = 0;

	for (;ch + offset - 1 < featureLen;ch += offset)
	{
		__m256 sumdg = _mm256_setzero_ps();
		__m256 sumdb = _mm256_setzero_ps();
		for (int r = 0;r < row;r++)
		{
			for (int c = 0;c < col;c++)
			{
				int posShift = r * col * channel + c * channel + ch;
				__m256 reglossData = _mm256_loadu_ps(datareg + posShift);
				__m256 regInData = _mm256_loadu_ps(inputreg + posShift);
				sumdg = _mm256_fmadd_ps(reglossData, regInData, sumdg);
				sumdb = _mm256_add_ps(reglossData, sumdb);
			}
		}
		_mm256_stream_ps(dg[b].vWeight + ch, sumdg);
		_mm256_stream_ps(db[b].vWeight + ch, sumdb);
	}
	for (;ch + offset16 - 1 < featureLen;ch += offset16)
	{
		__m128 sumdg = _mm_setzero_ps();
		__m128 sumdb = _mm_setzero_ps();
		for (int r = 0;r < row;r++)
		{
			for (int c = 0;c < col;c++)
			{
				int posShift = r * col * channel + c * channel + ch;
				__m128 reglossData = _mm_loadu_ps(datareg + posShift);
				__m128 regInData = _mm_loadu_ps(inputreg + posShift);
				sumdg = _mm_fmadd_ps(reglossData, regInData, sumdg);
				sumdb = _mm_add_ps(reglossData, sumdb);
			}
		}
		_mm_stream_ps(dg[b].vWeight + ch, sumdg);
		_mm_stream_ps(db[b].vWeight + ch, sumdb);
	}
	for (;ch < featureLen;ch += 1)
	{
		float sumdg = 0.0;
		float sumdb = 0.0;
		for (int r = 0;r < row;r++)
		{
			for (int c = 0;c < col;c++)
			{
				int posShift = r * col * channel + c * channel + ch;
				float reglossData = *(inputreg + posShift);
				float regInData = *(datareg + posShift);
				sumdg += reglossData * regInData;
				sumdb += reglossData;
			}
		}
		*(dg[b].vWeight + ch) = sumdg;
		*(db[b].vWeight + ch) = sumdb;
	}
	return true;
}

/*
*dL / gc = sum(dL / dyj * dyi / dg) = sum(dL / dyj * xxj)
* dL / db = sum(dL / dyj * dyi / db) = sum(dL / dyj)
* */
bool batchnorm::mysweetfc(float learnrate, float beta1, float beta2, float sigma, float l2Lamda, int t)
{
	bool ret=adam( learnrate*featureLen,  beta1,  beta2,  sigma,  l2Lamda,  t,  dg[0],  shadowGM, shadowGV, gamma);
	ret&=adam(learnrate * featureLen, beta1, beta2, sigma, l2Lamda, t, db[0], shadowBM, shadowBV, beta);
	return ret;
}

bool batchnorm::adam(float learnrate, float beta1, float beta2, float sigma, float l2Lamda, int t, kernal g, kernal m, kernal v,kernal w)
{//SGNAD
	/*
	* m(t)=beta1*m(t-1)+(1-beta1)*g(t)
	* V(t)=max{V(t-1),beta2*V(t-1)+(1-beta2)*g(t)^2}
	*/

	m.applySimd(beta1);//m(t-1)*beta
	tmp1.copySimd(g);
	g.applySimd((1 - beta1) / (1 - std::powf(beta1, t)));//g(t)*(1-beta)/(1-beta^t)
	m.addSimd(g);//m(t)=m(t-1)*beta+g(t)*(1-beta)/(1-beta^t)

	tmp2.copySimd(v);
	v.applySimd(beta2);//beta2*V(t-1)
	tmp1.mulSimd(tmp1);//gt*gt
	tmp1.applySimd((1 - beta2) / (1 - std::powf(beta2, t)));//g(t)*(1-beta2)/(1-beta2^t)
	v.addSimd(tmp1);//V(t)
	//BL.ShadowVelocity[k].maxSimd(dkernalSeries.at(2)[k]);//max:V(t),V(t-1))
	tmp1.copySimd(v);//V(t)

	tmp1.sqrtSimd();//sqrt(Vt')
	tmp1.copySimd(m);
	tmp1.divSimd(tmp1, sigma);//m(t)/(sqrt(t)+sigma)

	tmp2.applySimd(-learnrate);
	if (l2Lamda > 1e-6)
	{
		w.applySimd(1 - l2Lamda);//weight decay
	}
	w.addSimd(tmp2);//w(t)-alpha*(1-beta)*g(t)

	tmp1.applySimd(0);
	tmp2.applySimd(0);
	return true;
}

bool batchnorm::accumulateDW(int b)
{
	if (b >=batchSize || BNPos== BNUnkown)
	{
		//printf("batchnorm accumulateDW b exceed maxBatchSize£¬ %d, %d\n",b,batchSize);
		return false;
	}
	dg[0].addSimd(dg[b]);
	db[0].addSimd(db[b]);
	return true;
}