#pragma once
#include<vector>
#include "CNNLayer.h"
#include <fstream>

typedef enum
{
	D1 = 32,
	D2 = 32,
	D3 = 3,
	PicNum=10000,
}cifarSize;

class cifar10Reader
{
public:
	cifar10Reader() {};
	~cifar10Reader() {};
public:
	bool pullTestImages(char* path,int readPictureNum);
	bool pullTestImages1(char* path, int readPictureNum);
	std::vector<image> testGroup;
	std::vector<char> animalType;
	std::vector<image> testGroup1;
	std::vector<char> animalType1;
	bool checkSizeImageAndOut() { return testGroup.size() == animalType.size(); }
	bool checkSizeImageAndOut1() { return testGroup1.size() == animalType1.size(); }
};
