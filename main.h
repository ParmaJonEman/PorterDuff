//
// Created by jonem on 2/11/2023.
//

#ifndef PORTERDUFF_MAIN_H
#define PORTERDUFF_MAIN_H
#pragma once

#include <conio.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <locale.h>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <opencv2/core/utility.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include "opencv2/imgproc.hpp"
#include <algorithm>
#include <iterator>

using namespace cv;
static Mat createMask(Mat image);
static Mat createImage1();
static Mat createImage2();
static Mat over(Mat imageA, Mat imageB, Mat maskA, Mat maskB);
static Mat inside(Mat imageA, Mat maskA, Mat maskB);
static Mat outside(Mat imageA, Mat maskA, Mat maskB);
static Mat atop(Mat imageA, Mat imageB, Mat maskA, Mat maskB);
static Mat xOrB(Mat imageA, Mat imageB, Mat maskA, Mat maskB);
static int parseParameters(int argc, char** argv, String* imageFile1, String* imageFile2, String* maskFile1, String* maskFile2);
#endif //PORTERDUFF_MAIN_H
