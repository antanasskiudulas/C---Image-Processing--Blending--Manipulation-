//main.cpp
#include "stdafx.h"
#include "Image.h"
#include "Zoom.h"
#include "Blend.h"
#include <iostream>
#include <string>

//method forward declarations
int getUserInput();
float getSigmaInput();
double getScalingInput();

int main()
{
	int userInput = 0;
	//loop whilst user hasn't decided to exit
	for (int count = 0; userInput != 5; count++) // count is used to append to written file names to keep them unique so they don't override
	{
		std::cout << "*Image blender/scaler* By Antanas Skiudulas"
			"\nPlease select one of the following image operation algorithms \n" <<
			"1) Mean blend \n" <<
			"2) Median blend \n" <<
			"3) Sigma clipping \n" <<
			"4) Image scaling \n" <<
			"5) Exit the application \n";

		//get input from the user
		userInput = getUserInput();
		//branch out based on the input
		switch (userInput)
		{
		case 1:
		{
			std::cout << "Performing mean blending...\n";
			Blend meanImage(3263, 2505, 13);
			meanImage.stack(false); //false means that the images that will be read will not be added to vector (reduces memory)
			meanImage.meanBlend();
			meanImage.writePPM("Mean_" + std::to_string(count) + ".ppm");//every iteration, the file name is unique
			meanImage.imageStatistics("Mean_" + std::to_string(count) + ".ppm");
			break;
		}
		case 2:
		{
			Blend medianImage(3263, 2505, 13);
			medianImage.stack(true); // true because this algorithm requires the images to be kept
			std::cout << "Performing median blending...\n";
			medianImage.medianBlend();
			medianImage.writePPM("Median_" + std::to_string(count) + ".ppm");
			medianImage.imageStatistics("Median_" + std::to_string(count) + ".ppm");
			break;
		}
		case 3:
		{
			Blend sigmaClippedImage(3263, 2505, 13);
			sigmaClippedImage.stack(true);
			std::cout << "Performing sigma clipping...\n";
			sigmaClippedImage.sigmaClipping(getSigmaInput());
			sigmaClippedImage.writePPM("Sigma_" + std::to_string(count) + ".ppm");
			sigmaClippedImage.imageStatistics("Sigma_" + std::to_string(count) + ".ppm");
			break;
		}
		case 4:
		{
			std::cout << "Nearest neighbour image scaling algorithm... \n";
			Zoom zoomImage(750, 750, getScalingInput());
			zoomImage.readPPM("Images/Zoom/zIMG_1.ppm");
			zoomImage.NearestNeighbourScale();
			zoomImage.writePPM("Scaled_" + std::to_string(count) + ".ppm");
			//demonstrating the powers of polymorphism
			Image &ref = zoomImage;
			ref.imageStatistics("Scaled_" + std::to_string(count) + ".ppm");
			break;
		}
		}
	}
	return 0;
}

/*Getting the input for the operation choice*/
int getUserInput()
{
	//loops until correct option is given
	while (true) 
	{
		std::cout << "Enter your option: \n";
		int input;
		std::cin >> input;
		//checks the input match
		if (input == 1 || input == 2 || input == 3 || input == 4 || input == 5)
			return input;
		else 
			std::cout << "Such option doesn't exist..\n";
	}
}
/*Get's the floating value required for sigma boundries*/
float getSigmaInput()
{
	while (true)
	{
		std::cout << "Please enter a sigma float value (affects boundries), use 0.5 for default: \n";
		float input;
		std::cin >> input;

		//if the value provided is not float
		if (std::cin.fail())
		{
			std::cin.clear();//clear the failed flag
			std::cin.ignore(32767, '\n');//ignore all items in buffer
		}
		else
			return input; // returning correct input
	}
}

double getScalingInput()
{
	while (true)
	{
		std::cout << "Enter a (double) scale factor for the image: \n";
		double input;
		std::cin >> input;

		//if the value provided is not float
		if (std::cin.fail())
		{
			std::cin.clear();//clear the failed flag
			std::cin.ignore(32767, '\n');//ignore all items in buffer
		}
		else
			return input; // returning correct input
	}
}