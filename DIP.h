#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"
#include <opencv2/imgproc.hpp>
#include <iostream>

class DIP
{
private:
public:
	static void prepare(cv::Mat &image);
	static cv::Mat maskFirstFrame(const cv::Mat &gray, const cv::Mat &background);
	static void maskOpencvBS(cv::Mat &mask);
};

