#include "DIP.h"

using namespace cv;

void DIP::prepare(cv::Mat & image)
{
	for (int i = 0; i < image.cols; i++)
	{
		for (int j = 0; j < image.rows; j++)
		{
			Vec3b colour = image.at<Vec3b>(Point(i, j));
			if (colour.val[0] > 150 && colour.val[2] > 200)
			{
				image.at<Vec3b>(Point(i, j)) = Vec3b(0,0,0);
			}
		}
	}
}

cv::Mat DIP::maskFirstFrame(const cv::Mat & gray, const cv::Mat & background)
{
	Mat mask;
	absdiff(background, gray, mask);
	threshold(mask, mask, 20, 255, THRESH_BINARY);
	blur(mask, mask, cv::Size(10,10));
	threshold(mask, mask, 150, 255, THRESH_BINARY);
	return mask;
}

void DIP::maskOpencvBS(cv::Mat &mask)
{
	GaussianBlur(mask, mask, Size(11, 11), 3.5, 3.5);
	threshold(mask, mask, 80, 255, THRESH_BINARY);
	medianBlur(mask, mask, 3);
	threshold(mask, mask, 80, 255, THRESH_BINARY);

	erode(mask, mask, Mat(), Point(-1, -1), 5);
	dilate(mask, mask, Mat(), Point(-1, -1), 5);
}