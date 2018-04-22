#include "stdafx.h"
#include "Image.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <iterator>

//forward declarations
const Image::Rgb Image::kBlack = Image::Rgb(0);
const Image::Rgb Image::kWhite = Image::Rgb(1);
const Image::Rgb Image::kRed = Image::Rgb(1, 0, 0);
const Image::Rgb Image::kGreen = Image::Rgb(0, 1, 0);
const Image::Rgb Image::kBlue = Image::Rgb(0, 0, 1);

//Read ppm files into the code
//They need to be in 'binary' format (P6) with no comments in the header
//The first line is the 'P'number - P6 indicates it is a binary file, then the image dimensions and finally the colour range
//This header is then followed by the pixel colour data
//eg:	P6
//		3264 2448
//		255
//Open a .ppm file in notepad++ to see this header (caution: they are large files!)
void Image::readPPM(const std::string filename)
{
	//Remove this cout to prevent multiple outputs
	std::cout << "Reading " << filename << std::endl;
	std::ifstream ifs;
	ifs.open(filename, std::ios::binary);
	try {
		if (ifs.fail()) {
			throw("Can't open the input file - is it named correctly/is it in the right directory?");
		}
		std::string header;
		int w, h, b;
		ifs >> header;
		if (strcmp(header.c_str(), "P6") != 0) throw("Can't read the input file - is it in binary format (Has P6 in the header)?");
		ifs >> w >> h >> b;
		this->w = w;
		this->h = h;
		std::cout << w << "x" << h << std::endl;
		ifs.ignore(256, '\n'); // skip empty lines in necessary until we get to the binary data 
		unsigned char pix[3]; // read each pixel one by one and convert bytes to floats 
		for (int i = 0; i < w * h; ++i) {
			ifs.read(reinterpret_cast<char *>(pix), 3);
			this->pixels[i].r = pix[0] / 255.f;
			this->pixels[i].g = pix[1] / 255.f;
			this->pixels[i].b = pix[2] / 255.f;
		}
		ifs.close();
	}
	catch (const char *err) {
		fprintf(stderr, "%s\n", err);
		ifs.close();
	}
	//Confirm image read
	//Delete this to prevent multiple lines output
	std::cout << "Successfully read." << std::endl;
}

//Write data out to a ppm file
//Constructs the header as above
void Image::writePPM(const std::string filename)
{
	//std::cout << filename << std::endl;
	std::cout << "Writing image " << filename <<std::endl;
	if (this->w == 0 || this->h == 0) { fprintf(stderr, "Can't save an empty image\n"); return; }
	std::ofstream ofs;
	try {
		ofs.open(filename, std::ios::binary); // need to spec. binary mode for Windows users 
		if (ofs.fail()) throw("Can't open output file");
		ofs << "P6\n" << this->w << " " << this->h << "\n255\n";
		//std::cout << "P6\n" << img.w << " " << img.h << "\n255\n";
		unsigned char r, g, b;
		// loop over each pixel in the image, clamp and convert to byte format
		for (int i = 0; i < this->w * this->h; ++i) {
			r = static_cast<unsigned char>(std::min(1.f, pixels[i].r) * 255);
			g = static_cast<unsigned char>(std::min(1.f, pixels[i].g) * 255);
			b = static_cast<unsigned char>(std::min(1.f, pixels[i].b) * 255);
			ofs << r << g << b;
		}
		ofs.close();
		//Confirm image write
		std::cout << "Successfully written." << std::endl;
	}
	catch (const char *err) {
		fprintf(stderr, "%s\n", err);
		ofs.close();
	}
}

void Image::imageStatistics(std::string filename)
{

	std::cout << "\nFile name: " << filename << std::endl
		<< "Resolution (w*h): " << this->w << " x " << this->h << std::endl
		<< "The color depth: 24-bit (true color) which is " << std::pow(2, 24) << " colors." << std::endl
		<< "Estimated size of the file: " << (((this->w * this->h) * 24) / 8) / 1024 << " kilobytes" << std::endl;

	std::ofstream log;
	std::cout << "Writting the image statistics into ImageStatistics.log\n\n";
	try
	{
		log.open("ImageStatistics.log", std::ios::app);
		if (log.fail()) throw("Can't open log file");
		log << "\nFile name: " << filename << std::endl
			<< "Resolution (w*h): " << this->w << " x " << this->h << std::endl
			<< "The color depth: 24-bit (true color) which is " << std::pow(2,24) << " colors."<< std::endl
			<< "Estimated size of the file: " << (((this->w * this->h) * 24)/8)/1024 << " kilobytes\n" << std::endl;
		log.close();
	}
	catch (const char *err) {
		fprintf(stderr, "%s\n", err);
		log.close();
	}
}