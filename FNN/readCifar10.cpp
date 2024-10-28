#include"readCifar10.h"

bool cifar10Reader::pullTestImages(char* path,int pullTestImages)
{
	std::ifstream ifile;
	ifile.open(path,std::ios::binary);
	char whichClass;
	char* imageStreamRed=new char[1024];
	char* imageStreamGreen=new char[1024];
	char* imageStreamBlue=new char[1024];
	ifile.seekg(0,std::ios::end);
	int len = ifile.tellg();
	ifile.seekg(0, std::ios::beg);
	bool ret=true;
	if (len < (cifarSize::D1 * cifarSize::D2 * cifarSize::D3 + 1) * cifarSize::PicNum)
	{
		std::cout<<"your image size is too big"<<std::endl;
		ret = false;
	}

	if (pullTestImages > cifarSize::PicNum)
	{
		ret = false;
	}
	printf("test pic binary file length: %d bytes\n", len);
	if (ret)
	{
		animalType.clear();
		for (image im : testGroup)
		{
			im.freeImage();
		}
		std::vector<image>().swap(testGroup);
		for (int i = 0;i < pullTestImages;i++)
		{
			image aImage;
			aImage.setSize(cifarSize::D1, cifarSize::D2, cifarSize::D3);
			aImage.initImage(1);
			ifile.read(&whichClass, 1);

			ifile.read(imageStreamRed, 1024);

			ifile.read(imageStreamGreen, 1024);

			ifile.read(imageStreamBlue, 1024);

			animalType.push_back(whichClass);
			printf("type: %d\n", whichClass);
			for (int r = 0;r < cifarSize::D1;r++)
			{
				for (int c = 0;c < cifarSize::D2;c++)
				{
					aImage.imageData[0][r][c] = imageStreamRed[c +  r* 32];
					aImage.imageData[1][r][c] = imageStreamGreen[c + r * 32];
					aImage.imageData[2][r][c] = imageStreamBlue[c + r * 32];
					aImage.vImageData[c * 3 + r * 3 * cifarSize::D2 + 0] = aImage.imageData[0][r][c];
					aImage.vImageData[c * 3 + r * 3 * cifarSize::D2 + 1] = aImage.imageData[1][r][c];
					aImage.vImageData[c * 3 + r * 3 * cifarSize::D2 + 2] = aImage.imageData[2][r][c];
				}
				
			}
			testGroup.push_back(aImage);
		}
	}
	ifile.close();
	return ret;

}
bool cifar10Reader::pullTestImages1(char* path, int pullTestImages)
{
	std::ifstream ifile;
	ifile.open(path, std::ios::binary);
	char whichClass;
	char* imageStreamRed = new char[1024];
	char* imageStreamGreen = new char[1024];
	char* imageStreamBlue = new char[1024];
	ifile.seekg(0, std::ios::end);
	int len = ifile.tellg();
	ifile.seekg(0, std::ios::beg);
	bool ret = true;
	if (len < (cifarSize::D1 * cifarSize::D2 * cifarSize::D3 + 1) * cifarSize::PicNum)
	{
		std::cout << "your image size is too big" << std::endl;
		ret = false;
	}
	if (pullTestImages > cifarSize::PicNum)
	{
		ret = false;
	}
	printf("test pic binary file length: %d bytes\n", len);
	if (ret)
	{
		animalType1.clear();
		for (image im : testGroup1)
		{
			im.freeImage();
		}
		std::vector<image>().swap(testGroup1);
		for (int i = 0;i < pullTestImages;i++)
		{
			image aImage;
			aImage.setSize(cifarSize::D1, cifarSize::D2, cifarSize::D3);
			aImage.initImage(1);
			ifile.read(&whichClass, 1);

			ifile.read(imageStreamRed, 1024);

			ifile.read(imageStreamGreen, 1024);

			ifile.read(imageStreamBlue, 1024);

			animalType1.push_back(whichClass);
		//	printf("type: %d\n", whichClass);
			for (int r = 0;r < cifarSize::D1;r++)
			{
				for (int c = 0;c < cifarSize::D2;c++)
				{
					aImage.imageData[0][r][c] = imageStreamRed[c + r * 32];
					aImage.imageData[1][r][c] = imageStreamGreen[c + r * 32];
					aImage.imageData[2][r][c] = imageStreamBlue[c + r * 32];
					aImage.vImageData[c * 3 + r * 3 * cifarSize::D2 + 0] = aImage.imageData[0][r][c];
					aImage.vImageData[c * 3 + r * 3 * cifarSize::D2 + 1] = aImage.imageData[1][r][c];
					aImage.vImageData[c * 3 + r * 3 * cifarSize::D2 + 2] = aImage.imageData[2][r][c];
				}

			}
			testGroup1.push_back(aImage);
		}
	}
	ifile.close();
	return ret;

}