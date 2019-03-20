#include "VideoInput.h"

using namespace cv;
using namespace std;

VideoInput::VideoInput(std::string path)
{
	if (path != "0")
		capture = VideoCapture(path);
	else
		capture = VideoCapture(0);
}

cv::Mat VideoInput::getFrame()
{
	Mat frame;
	if(capture.read(frame))
		return frame;
	else
	{
		return Mat();
	}
}

double VideoInput::ms()
{
	return 1000 / capture.get(CAP_PROP_FPS);
}

VideoInput::~VideoInput()
{
}
