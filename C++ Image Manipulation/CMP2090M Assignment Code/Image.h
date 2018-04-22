#pragma once
//*********************************************
//Image class to hold and allow manipulation of images once read into the code
//from https://www.scratchapixel.com/
//*********************************************
#include "stdafx.h"
#include <cstdlib> 
#include <cstdio> 
#include <vector>
#include <iostream>  
#include <cmath>

class Image
{
public:
	// Rgb structure, i.e. a pixel 
	struct Rgb
	{
		//constructors to accommodate different types
		Rgb() : r(0), g(0), b(0) {}
		Rgb(float c) : r(c), g(c), b(c) {}
		Rgb(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {}
		Rgb(const Rgb &c) : r(c.r), g(c.g), b(c.b) {}
		bool operator != (const Rgb &c) const
		{
			return c.r != r || c.g != g || c.b != b;
		}
		//overloaded operations that return modified object's address
		Rgb& operator /= (const float c)
		{
			r /= c, g /= c, b /= c; return *this;
		}
		Rgb& operator *= (const Rgb &rgb)
		{
			r *= rgb.r, g *= rgb.g, b *= rgb.b; return *this;
		}
		Rgb& operator += (const Rgb &rgb)
		{
			r += rgb.r, g += rgb.g, b += rgb.b; return *this;
		}
		friend float& operator += (float &f, const Rgb rgb)
		{
			f += (rgb.r + rgb.g + rgb.b) / 3.f; return f;
		}
		Rgb& operator = (const Rgb &rgb)
		{
			r = rgb.r, g = rgb.g, b = rgb.b; return *this;
		}
		Rgb& operator = (const float &c)
		{
			r = c, g = c, b = c; return *this;
		}

		//Returns anonymous objects with modified values
		Rgb operator * (const Rgb &rgb)
		{
			return Rgb(r * rgb.r, g * rgb.g, b * rgb.b);
		}
		Rgb operator * (const float c)
		{
			return Rgb(r * c, g * c, b * c);
		}
		Rgb operator / (const float c)
		{
			return Rgb(r / c, g / c, b / c);
		}
		Rgb operator / (const Rgb &rgb)
		{
			return Rgb(r / rgb.r, g / rgb.g, b / rgb.b);
		}

		Rgb operator - (const Rgb &rgb)
		{
			return Rgb(r - rgb.r, g - rgb.g, b - rgb.b);
		}
		Rgb operator + (const Rgb &rgb)
		{
			return Rgb(r + rgb.r, g + rgb.g, b + rgb.b);
		}
		bool operator < (const Rgb &rgb) const
		{
			return ((r + g + b) < (rgb.r + rgb.g + rgb.b));
		}
		bool operator > (const Rgb &rgb) const
		{
			return ((r + g + b) > (rgb.r + rgb.g + rgb.b));
		}

		float r, g, b;
	};

	Image() : w(0), h(0), pixels(nullptr) { /* empty image */ }
	Image(const unsigned int &_w, const unsigned int &_h, const Rgb &c = kBlack) :
		w(_w), h(_h), pixels(NULL)
	{
		pixels = new Rgb[w * h];
		for (int i = 0; i < w * h; ++i)
			pixels[i] = c;
	}
	//copy constructor
	Image(const Image &im)
	{
		w = im.w;
		h = im.h;
		pixels = new Rgb[im.w * im.h];
		for (int i = 0; i < im.w * im.h; ++i)
			pixels[i] = im.pixels[i];
	}
	//copy assignment operator
	Image& operator=(const Image& other)
	{
		w = other.w;
		h = other.h;
		pixels = new Rgb[other.w * other.h];
		for (int i = 0; i < other.w * other.h; ++i)
			pixels[i] = other.pixels[i];

		return *this;
	}

	//pixel access operator
	const Rgb& operator [] (const unsigned int &i) const
	{
		return pixels[i];
	}


	//forward declarations for all the functions defined in image.cpp

	void readPPM(const std::string filename);
	void writePPM(const std::string filename);
	virtual void imageStatistics(std::string filename);
	//destructor which deleted this object once it exits the scope
	virtual ~Image() //so the derived classes get to call their destructor
	{
		if (pixels != NULL)
			delete[] pixels;
		//delete[] pixels;
	}

	//unsigned int w, h; // Image resolution 
	int w, h; // Image resolution
	Rgb *pixels; // 1D array of pixels 
	static const Rgb kBlack, kWhite, kRed, kGreen, kBlue; // Preset colors 
};


