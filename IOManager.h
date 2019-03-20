#pragma once

#include <opencv2/opencv.hpp>
#include <fstream>
#include <time.h>
#include <vector>
#include "ObjectTracked.h"

#define SIZE 26

class IOManager
{
public:
	static void sendObjectData(std::vector<ObjectTracked> &o, std::string filename);
};

