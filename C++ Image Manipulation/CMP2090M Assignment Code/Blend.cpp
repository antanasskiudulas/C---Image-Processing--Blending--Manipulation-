#include "stdafx.h"
#include "Blend.h"
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <iostream>
/*Reads in the files within an image direction and pushes it into an image vector
or adds the pixels to a calling object (only for mean blend)*/
void Blend::stack(bool push)
{
	int fnum = 1;
	for (Image &image : imageStack)
	{
		if (push)
		{
			image = Image(this->w, this->h);
			image.readPPM("Images/ImageStacker_set1/IMG_" + std::to_string(fnum) + ".ppm");
		}
		//for cases where the algorithm doesn't require for images to be in the stack
		else if(!push)
		{
			Image *temp = new Image(this->w, this->h);
			temp->readPPM("Images/ImageStacker_set1/IMG_" + std::to_string(fnum) + ".ppm");
			for (int i = 0; i < w * h; i++)
				this->pixels[i] += temp->pixels[i];
			delete temp;
		}
		fnum++;
	}
}

//This assumes the image has already been stacked. Reduces intensity of stacked pixels to produce mean blend.
void Blend::meanBlend()
{
	//loops through each pixel and divides by the pixel set size
	for (int i = 0; i < this->h*this->w; i++)
		this->pixels[i] /= (float)m_noOfImages;
}

//A function to help compare two Rgb pointer via in-built sort function for median blend
inline bool comparePtrToRgb(const Image::Rgb *a, const Image::Rgb *b) { return (*a < *b); }

/*Since this function will be called a lot and is short, it's best to make it inline to
avoid performance overhead from function calls*/
Image::Rgb* Blend::getMedianFromSet()
{
	//sorts pixel vector for nth pixel by passing iterators and predicate parameter
	std::sort(pixelBuffer.begin(), pixelBuffer.end(), comparePtrToRgb);
	return pixelBuffer[pixelBuffer.size() / 2]; // returns the middle value of the set
}
/*sets the current pixel to a median from a collected set within the loop*/
void Blend::medianBlend()
{
	for (int pix = 0; pix < h * w; pix++)
	{
		for (int i = 0; i < imageStack.size(); i++)
			pixelBuffer[i] = &imageStack[i].pixels[pix];
		this->pixels[pix] = *getMedianFromSet();
	}
}
/*Calculates the mean pixel from the set of pixels*/
Image::Rgb Blend::getMeanFromSet()
{
	Rgb temp;
	//loop through and add each pixel
	for (Rgb* &pix : pixelBuffer)
		temp += *pix;
	return temp = temp / (float)pixelBuffer.size(); //return the mean by dividing by set's size
}
/*Calculates the standard deviation of a particular pixel vector (used exclusively for sigma clipping)*/
Image::Rgb Blend::getStdDev(Rgb meanPix)
{
	//collects the deviation from the mean
	Rgb sumOut;
	//accumulates the deviation from the mean for each pixel
	for (Rgb* &pix : pixelBuffer)
		sumOut += ((*pix - meanPix) * (*pix - meanPix));
	//calculates the standard deviation from the aformentioned value
	Rgb stdDevOut(sqrt(sumOut.r / (float)pixelBuffer.size()),
		sqrt(sumOut.g / (float)pixelBuffer.size()), 
		sqrt(sumOut.b / (float)pixelBuffer.size()));

	return stdDevOut;
}

/*Clips unwanted outliers from the set to increase the signal to noise ration for each pixel in the image
This algorithm is aided by sigma value which dictates the size of the high & low boundries*/
void Blend::sigmaClipping(float sigma)
{
	for (int pix = 0; pix < h * w; pix++)
	{
		//populating the vector with the nth pixel of all the images in the image array
		pixelBuffer.resize(m_noOfImages);//since pixels are being erased, the array must be resized
		for (int i = 0; i < imageStack.size(); i++)
			pixelBuffer[i] = &imageStack[i].pixels[pix];

		//getting first deviation value from the set (declared outside so it can be tested against tolerance)
		Rgb stdDevOfSet = getStdDev(getMeanFromSet());
		Rgb stdDevOfSetOld; //place holder for the deviation of previous loop
		do
		{
			//calculating the median of the new set
			Rgb medianOfSet = *getMedianFromSet();
			//ensuring the old standard deviation is kept for testing the tolerance of the loop
			stdDevOfSetOld = stdDevOfSet;
			//recalculating the new  deviant (this is hopefully done after the loop below with reduced set
			stdDevOfSet = getStdDev(getMeanFromSet());

			//setting an iterator to the beginning of pixel vector
			auto i = pixelBuffer.begin();
			//removing values until it hits the end of the vector
			while (i != pixelBuffer.end()) {
				//comparing dereference iterator against upper and lower boundries
				if (*(*i) > (medianOfSet + (stdDevOfSet * sigma)) || *(*i) < (medianOfSet - (stdDevOfSet * sigma)))
					i = pixelBuffer.erase(i); // removing any values outside of them
				else
					++i; //keep going forward if value is within range
			}
		} while (stdDevOfSetOld > 1); //tests whether the tolerance level has been reached

		this->pixels[pix] = getMeanFromSet(); //recalculating the mean from the reduced set and assigning it to current pixel
	}
}