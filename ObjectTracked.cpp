#include "ObjectTracked.h"

using namespace std;
using namespace cv;

float RectDistance(const Rect &r1, const Rect &r2)
{
	float x1 = r1.x + r1.width / 2;
	float y1 = r1.x + r2.height / 2;

	float x2 = r2.x + r2.width / 2;
	float y2 = r2.x + r2.height / 2;

	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

ObjectTracked::ObjectTracked(cv::Rect initialRect)
{
	active = 1;
	coordinates.push_back(initialRect);
}

void ObjectTracked::match(std::vector<cv::Rect> &possible_coordinates)
{
	if (!active)
		return;
	
	else
	{
		int bestMatch = 0;
		float bestDistance = 2 * maxDifference;
		for (unsigned int i = 0; i < possible_coordinates.size(); i++)
		{
			float distance = RectDistance(possible_coordinates[i], coordinates.back());
			if (distance < maxDifference)
			{
				if (distance < bestDistance)
				{
					bestDistance = distance;
					bestMatch = i;
				}
			}
		}

		if (bestDistance < maxDifference)
		{
			coordinates.push_back(possible_coordinates[bestMatch]);
			possible_coordinates.erase(possible_coordinates.begin() + bestMatch);
		}
		else
		{
			active = 0;
		}
	}
}

void ObjectTracked::draw(cv::Mat & image, cv::Point &&place)
{
	int x = coordinates.back().x + coordinates.back().width / 2;
	int y = coordinates.back().y + coordinates.back().height / 2;

	if (!active)
	{
		if (coordinates.size() >= 5)
		{
			int s = coordinates.size();
			s--;
			float mean = 0;
			for (int i = 0; i < 4; i++)
			{
				mean += RectDistance(coordinates.at(s),coordinates.at(s-1-i));
			}
			mean = mean / 5;
			if (mean < 5);
			{	
				putText(image, "No object detected, last position at (" + to_string(x) + "," + to_string(y) + ")", place, 1, 3, Scalar(255, 255, 255), 2);
				active = 0;
			}
		}
		return;
	}
	else
	{
		rectangle(image, coordinates.back(), Scalar(0, 255, 0), 4);
		putText(image, "Tracking object at (" + to_string(x) + "," + to_string(y) + ")", place, 1, 3, Scalar(255, 255, 255), 2);
	}
}

cv::Rect ObjectTracked::getCoordinates()
{
	return coordinates.back();
}

bool ObjectTracked::good()
{
	if (active == 1)
		return true;
	else
		return false;
}

ObjectTracked::~ObjectTracked()
{
}