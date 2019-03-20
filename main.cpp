#include "DIP.h"
#include "Perspective.h"
#include "Tracker.h"
#include "VideoInput.h"
#include "opencv2/videoio.hpp"
#include "opencv2/videoio/videoio.hpp"

using namespace std;
using namespace cv;

int main()
{
	VideoInput input("../1.mp4");

	Mat actualFrame = input.getFrame();
	
	Perspective perspective(actualFrame, Perspective::InitializationType::Manual, Perspective::TransformMatrixType::FromFile);
	perspective.TransformImage(actualFrame);
	TrackingClass tr(TrackingClass::Tracking::opencvTracker, TrackingClass::Background::opencvBS, actualFrame);
	tr.track(actualFrame);

	while (true)
	{
		actualFrame = input.getFrame();
		if (actualFrame.empty())
			break;

		cout << "Fps: " << input.ms() << endl;

		double t1 = getTickCount();
		perspective.TransformImage(actualFrame);
		double t2 = getTickCount();
		double time = (t2 - t1) / getTickFrequency();
		cout << "Perspektywa: " << time << endl;

		t1 = getTickCount();
		tr.track(actualFrame);
		t2 = getTickCount();
		time = (t2 - t1) / getTickFrequency();
		cout << "SledzKlaenie: " << time << endl;

		namedWindow("mask", WINDOW_NORMAL);
		resizeWindow("mask", 400, 300);

		namedWindow("result", WINDOW_NORMAL);
		resizeWindow("result", 400, 300);


		imshow("mask", tr.returnMask());

		imshow("result", actualFrame);

		waitKey(20);
	}

	return 0;
}
