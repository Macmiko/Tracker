#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/video.hpp>

class VideoInput
{
private:
	cv::VideoCapture capture;
public:
	VideoInput(std::string path);
	cv::Mat getFrame();
	double ms();
	~VideoInput();
};

