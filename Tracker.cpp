#include "Tracker.h"
#include "DIP.h"
#include "IOManager.h"

using namespace std;
using namespace cv;

TrackingClass::TrackingClass(TrackingClass::Tracking ttype, TrackingClass::Background btype,  const cv::Mat &background)
{
	trackingType = ttype;
	backgroundType = btype;

	if (backgroundType == Background::firstFrameBS)
	{
		cvtColor(background, bg, COLOR_BGR2GRAY);
		Mat resized(cvRound(bg.cols / scale), cvRound(bg.rows / scale), CV_8UC1);
		resize(bg, bg, resized.size());

		bgModel.release();
	}
	else if (backgroundType == Background::opencvBS)
	{
		bgModel = createBackgroundSubtractorMOG2().dynamicCast<BackgroundSubtractor>();
		mask.create(Size(cvRound(background.cols / scale), cvRound(background.rows / scale)), background.type());
	}
}

void TrackingClass::track(cv::Mat & image)
{
	if (backgroundType == Background::firstFrameBS)
	{
		Mat gray_image;
		cvtColor(image, gray_image, COLOR_BGR2GRAY);
		resize(gray_image, gray_image, bg.size());

		mask = DIP::maskFirstFrame(gray_image, bg);

		resize(mask, mask, image.size());
	}
	else if (backgroundType == Background::opencvBS)
	{
		Rect ROI(50, 150, (image.cols - 100), (image.rows - 150));
		Mat frameMask(image.size(), CV_8UC1, Scalar::all(0));
		frameMask(ROI).setTo(Scalar::all(255));
		Mat result;
		image.copyTo(result, frameMask);
		resize(result, result, Size(image.cols / 4, image.rows / 4));
		
		bgModel->apply(result, mask, true ? -1 : 0);

		DIP::maskOpencvBS(mask);

		bgModel->getBackgroundImage(bg);

		resize(mask, mask, image.size());
	}

	if (trackingType == Tracking::contours)
	{
		std::vector<Rect> detectedObjects;

		Mat contours_temp;
		mask.copyTo(contours_temp);

		vector< vector<Point> > contours;
		vector<Vec4i> hierarchy;

		findContours(contours_temp, contours, hierarchy, 0, 2);

		for (int i = 0; i < contours.size(); i++)
		{
			if (contourArea(contours[i]) < 1000)
			{
				contours.erase(contours.begin() + i);
			}
		}

		for (int i = 0; i < contours.size(); i++)
		{
			detectedObjects.push_back(boundingRect(contours.at(i)));
		}

		for (unsigned int i = 0; i < trackedObjects.size(); i++)
		{
			trackedObjects[i].match(detectedObjects);
		}

		for (unsigned int i = 0; i < detectedObjects.size(); i++)
		{
			ObjectTracked object(detectedObjects[i]);
			trackedObjects.push_back(object);
		}

		int activeCount = 0;

		for (unsigned int i = 0; i <trackedObjects.size(); i++)
		{
			if (trackedObjects[i].good())
				activeCount++;
			trackedObjects[i].draw(image, Point(20, (50 * activeCount)));
		}

		if (activeCount == 0)
		{
			putText(image, "No objects detected", Point(20, 50), 1, 3, Scalar(255, 255, 255), 2);
		}
	}
	else if (trackingType == Tracking::opencvTracker)
	{
		std::vector<Rect> detectedObjects;

		Mat contoursTemp;
		mask.copyTo(contoursTemp);

		vector< vector<Point> > contours;
		vector<Vec4i> hierarchy;

		findContours(contoursTemp, contours, hierarchy, 0, 2);

		// find contours of objects big enough
		for (int i = 0; i < contours.size(); i++)
		{
			if (contourArea(contours[i]) < 1000) 
			{
				contours.erase(contours.begin() + i);
			}
		}

		for (int i = 0; i < contours.size(); i++)
		{
			detectedObjects.push_back(boundingRect(contours.at(i)));
		}

		//activate tracker once we have 3 objects
		if (!active)
		{
			if (detectedObjects.size() < 3)
				return;
			else
			{
				active = 1;
				Mat copy = image.clone();
				
				DIP::prepare(copy);

				for (int i = 0; i < detectedObjects.size(); i++)
				{
					Ptr<Tracker> tr = TrackerBoosting::create();
					tr->init(copy, detectedObjects[i]);
					tracker.push_back(tr);
				}
			}

		}

		else
		{
			Mat copy = image.clone();

			DIP::prepare(copy);

			//update trackers
			vector<Rect> TrackingRects;
			for (int i = 0; i < tracker.size(); i++)
			{
				Rect2d r;

				// uzywany tracker nie zwraca informacji o zgubieniu obiektu

				if (tracker[i]->update(image, r))
					TrackingRects.push_back(r);
				else
				{
					tracker[i].release();
					tracker.erase(tracker.begin() + i);
				}
			}

			for (unsigned int i = 0; i < trackedObjects.size(); i++)
			{
				trackedObjects[i].match(TrackingRects);
			}

			for (unsigned int i = 0; i < TrackingRects.size(); i++)
			{
				ObjectTracked object(TrackingRects[i]);
				trackedObjects.push_back(object);
			}

			int activeCount = 0;

			for (unsigned int i = 0; i <trackedObjects.size(); i++)
			{
				if (trackedObjects[i].good())
					activeCount++;
				trackedObjects[i].draw(image, Point(20, (50 * activeCount)));
			}

		}
	}

	IOManager::sendObjectData(trackedObjects, "filename");
}

cv::Mat TrackingClass::returnMask()
{
	return mask;
}

TrackingClass::~TrackingClass()
{
	destroyAllWindows();
}
