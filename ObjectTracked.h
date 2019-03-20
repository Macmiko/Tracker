#pragma once
#include <opencv2/opencv.hpp>

//maksymalna roznica miedzy polozeniem punktu na dwoch kolejnych klatkach, przy ktorej zakladamy, ze jest to ten sam punkt
const int maxDifference = 50;

class ObjectTracked
{
private:
	std::vector<cv::Rect> coordinates;
	std::vector<std::string> w;
	bool active;
public:
	ObjectTracked(cv::Rect initialRect);
	void match(std::vector<cv::Rect> &possible_coordinates);
	void draw(cv::Mat &image, cv::Point &&place);
	cv::Rect getCoordinates();
	bool good();
	~ObjectTracked();
};

