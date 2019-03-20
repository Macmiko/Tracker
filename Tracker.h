#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/video/background_segm.hpp"
#include <opencv2/tracking.hpp>
#include "ObjectTracked.h"

class TrackingClass
{
public:
	enum class Tracking{
		contours,
		opencvTracker
	};
	enum class Background {
		opencvBS,
		firstFrameBS
	};
private:
	Tracking trackingType;
	Background backgroundType;
	cv::Mat bg;
	cv::Mat mask;

	std::vector<ObjectTracked> trackedObjects;

	const int scale = 4;

	bool active = 0;

	cv::Ptr<cv::BackgroundSubtractor> bgModel;
	std::vector<cv::Ptr<cv::Tracker>> tracker;

public:
	TrackingClass(TrackingClass::Tracking ttype, TrackingClass::Background btype, const cv::Mat &background);
	~TrackingClass();
	void track(cv::Mat &image);
	cv::Mat returnMask();
};

