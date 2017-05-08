#include <opencv2\opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <string>

using namespace cv;
using namespace std;

Mat maxsuppress(Mat mag, Mat angle);
void hysterysis(int y, int x, Mat *suppress);

int main(int, char** argv){

	Mat image, greyscale, blurred, maxsuppression, finalimage;
	
	//Read in the image
	image = imread("Image 1.jpg", IMREAD_COLOR);   

	//Convert to greyscale
	cvtColor(image, greyscale, CV_BGR2GRAY);
	imwrite("Grey Scale.jpg", greyscale);	//Save greyscale

	//Noise reduction
	GaussianBlur(greyscale, blurred, Size(5, 5), 1.4);
	imwrite("Blurred.jpg", greyscale);		//Save blurred

	//Gradient
	Mat gradx, grady, mag, angle;
	int scale = 2, delta = 0, ddepth = CV_32F;

	//Grad X
	Sobel(blurred, gradx, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);

	//Grad Y
	Sobel(blurred, grady, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);

	//Magnitude, Angle
	cartToPolar(gradx, grady, mag, angle, true);

	//Save images
	imwrite("Mag.jpg", mag);
	imwrite("Angle.jpg", angle);
	
	maxsuppression = maxsuppress(mag, angle);
	imwrite("MaxSuppression.jpg", maxsuppression);

	for (int y = 1; y < mag.rows - 1; y++) {
		for (int x = 1; x < mag.cols - 1; x++) {
			if (mag.at<float>(y, x) == 255.0) {
				hysterysis(mag.rows, mag.cols, &maxsuppression);
			}
		}
	}

	cv::Mat temp;
	threshold(maxsuppression, temp, 245, 255, 0);
	
	imwrite("Final.jpg", temp);

	waitKey(27);
	destroyAllWindows();
	return 0;
}

//Non Max Suppression
Mat maxsuppress(Mat mag, Mat angle){
	Mat suppress;
	float clr;
	suppress = Mat(mag.rows, mag.cols, CV_32F);
	for (int y = 1; y < mag.rows - 1; y++) {
		for (int x = 1; x < mag.cols - 1; x++) {
			float anglem = angle.at<float>(y, x);
			if (((anglem > 337.5) && (anglem <= 0)) || ((anglem <= 22.5) && (anglem > 0)) || ((anglem > 157.5) && (anglem <= 202.5))){
				//Thresholding
				if ((mag.at<float>(y, x - 1) < mag.at<float>(y, x)) && (mag.at<float>(y, x) > mag.at<float>(y, x + 1))){
					if (mag.at<float>(y, x) > 70) {
						clr = 255.0;
					}
					if ((mag.at<float>(y, x) < 70) && (mag.at<float>(y, x) < 1))
						clr = 100.0;
				}
				else {
					clr = 0.0;
				}
				suppress.at<float>(y, x) = clr;
			}
			if (((anglem > 22.5) && (anglem <= 67.5)) || ((anglem > 202.5) && (anglem <= 247.5))){
				//Thresholding
				if ((mag.at<float>(y, x - 1) < mag.at<float>(y, x)) && (mag.at<float>(y, x) > mag.at<float>(y, x + 1))){
					if (mag.at<float>(y, x) > 70) {
						clr = 255.0;
					}
					if ((mag.at<float>(y, x) < 70) && (mag.at<float>(y, x) < 1))
						clr = 100.0;
				}
				else {
					clr = 0.0;
				}
				suppress.at<float>(y, x) = clr;
			}
			if (((anglem > 67.5) && (anglem <= 112.5)) || ((anglem > 245.5) && (anglem <= 292.5))){
				//Thresholding
				if ((mag.at<float>(y, x - 1) < mag.at<float>(y, x)) && (mag.at<float>(y, x) > mag.at<float>(y, x + 1))){
					if (mag.at<float>(y, x) > 70) {
						clr = 255.0;
					}
					if ((mag.at<float>(y, x) < 70) && (mag.at<float>(y, x) < 1))
						clr = 100.0;
				}
				else {
					clr = 0.0;
				}
				suppress.at<float>(y, x) = clr;
			}
			if (((anglem > 112.5) && (anglem <= 157.5)) || ((anglem > 292.5) && (anglem <= 337.5))){
				//Thresholding
				if ((mag.at<float>(y, x - 1) < mag.at<float>(y, x)) && (mag.at<float>(y, x) > mag.at<float>(y, x + 1))){
					if (mag.at<float>(y, x) > 70) {
						clr = 255.0;
					}
					if ((mag.at<float>(y, x) < 70) && (mag.at<float>(y, x) < 1))
						clr = 100.0;
				}
				else {
					clr = 0.0;
				}
				suppress.at<float>(y, x) = clr;
			}
		}
	}
	return suppress;
}

void hysterysis(int y, int x, Mat *suppress) {
	if (suppress->at<float>(y, x + 1) == 100) {
		suppress->at<float>(y, x + 1) = 255.0;
		hysterysis(y, x + 1, suppress);
	}
	if (suppress->at<float>(y, x - 1) == 100) {
		suppress->at<float>(y, x - 1) = 255.0;
		hysterysis(y, x - 1, suppress);
	}
	if (suppress->at<float>(y + 1, x) == 100) {
		suppress->at<float>(y + 1, x) = 255.0;
		hysterysis(y + 1, x, suppress);
	}
	if (suppress->at<float>(y - 1, x) == 100) {
		suppress->at<float>(y - 1, x) = 255.0;
		hysterysis(y - 1, x, suppress);
	}
	if (suppress->at<float>(y - 1, x + 1) == 100) {
		suppress->at<float>(y - 1, x + 1) = 255.0;
		hysterysis(y - 1, x + 1, suppress);
	}
	if (suppress->at<float>(y - 1, x - 1) == 100) {
		suppress->at<float>(y - 1, x - 1) = 255.0;
		hysterysis(y - 1, x - 1, suppress);
	}
	if (suppress->at<float>(y + 1, x + 1) == 100) {
		suppress->at<float>(y + 1, x + 1) = 255.0;
		hysterysis(y + 1, x + 1, suppress);
	}
	if (suppress->at<float>(y + 1, x - 1) == 100) {
		suppress->at<float>(y + 1, x - 1) = 255.0;
		hysterysis(y + 1, x - 1, suppress);
	}
	else { 
		suppress->at<float>(y,x) = 0.0;
	}
}
