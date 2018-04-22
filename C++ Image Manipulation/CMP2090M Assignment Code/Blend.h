#pragma once
#include "stdafx.h"
#include "Image.h"
#include <vector>
#include <string>
class Blend : public Image
{
private:
	std::vector<Image> imageStack;
	std::vector<Rgb*> pixelBuffer;//only works with pointers (no copying)
	int m_noOfImages;

	Rgb* getMedianFromSet();
	Rgb getMeanFromSet();
	Rgb getStdDev(Rgb meanPix);
public:
	Blend(int width = 0, int height = 0, int noOfImages = 0):Image(width, height), m_noOfImages(noOfImages), imageStack(noOfImages), pixelBuffer(noOfImages)
	{}
public:
	void stack(bool push = true);
	void meanBlend();
	void medianBlend();
	void sigmaClipping(float sigma = 0.5f);
};