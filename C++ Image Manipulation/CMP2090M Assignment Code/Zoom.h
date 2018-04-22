#pragma once
#include "stdafx.h"
#include "Image.h"

class Zoom : public Image
{
private:
	double scaleFactor;
	Image *imgBuffer;
public:
	Zoom(int width=0, int height=0, double scale=0.0):Image(width, height), scaleFactor(scale), imgBuffer(new Image(width * scaleFactor, height * scaleFactor)){}

	void NearestNeighbourScale();
	void imageStatistics(std::string filename) override;

	Zoom& operator = (const Image &target)
	{
		w = target.w;
		h = target.h;
		this->pixels = new Rgb[target.w * target.h];
		for (int i = 0; i < target.w * target.h; ++i)
			pixels[i] = target.pixels[i];
		return *this;
	}

	virtual ~Zoom()
	{
		delete imgBuffer;
	}


};