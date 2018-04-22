#include "stdafx.h"
#include "Zoom.h"
#include <iostream>
#include <fstream>
#include <sstream>

void Zoom::NearestNeighbourScale()
{
	double w_ratio = (double)(this->w / (this->w * scaleFactor));
	double h_ratio = (double)(this->h / (this->h * scaleFactor));
	int px, py;
	for (int i = 0; i < this->h * scaleFactor; i++)
		for (int j = 0; j < this->w * scaleFactor; j++)
		{
			px = j * w_ratio;
			py = i * h_ratio;
			imgBuffer->pixels[(int)(i*this->w*scaleFactor) + j] = this->pixels[(int)(py*this->w) + px];
		}

	*this = *imgBuffer; //assignment operator is overloaded to accommodate class difference
}

void Zoom::imageStatistics(std::string filename)
{
	std::cout << "\nFile name: " << filename << std::endl
		<< "Resolution (w*h): " << this->w << " x " << this->h << std::endl
		<< "The color depth: 24-bit (true color) which is " << std::pow(2, 24) << " colors." << std::endl
		<< "Estimated size of the file: " << (((this->w * this->h) * 24) / 8) / 1024 << " kilobytes" << std::endl
		<< "The scale factor of this image is: " << scaleFactor << std::endl
		<< "This image has " << (float)((this->w * this->h) / (750 * 750)) << " times more pixels than the orignal.\n";

	std::ofstream log;
	std::cout << "Writting the image statistics into ImageStatistics.log\n\n";
	try
	{
		log.open("ImageStatistics.log", std::ios::app);
		if (log.fail()) throw("Can't open log file");
		log << "\nFile name: " << filename << std::endl
			<< "Resolution (w*h): " << this->w << " x " << this->h << std::endl
			<< "The color depth: 24-bit (true color) which is " << std::pow(2, 24) << " colors." << std::endl
			<< "Estimated size of the file: " << (((this->w * this->h) * 24) / 8) / 1024 << " kilobytes" << std::endl
			<< "The scale factor of this image is: " << scaleFactor << std::endl
			<< "This image has " << (float)((this->w * this->h) / (750 * 750)) << " times more pixels than the orignal.\n";
		log.close();
	}
	catch (const char *err) {
		fprintf(stderr, "%s\n", err);
		log.close();
	}
}