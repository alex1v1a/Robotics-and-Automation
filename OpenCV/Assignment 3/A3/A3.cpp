// Marc Alexander Sferrazza 12164165
// Referenceing from http://docs.opencv.org/2.4/
// http://docs.opencv.org/2.4/doc/tutorials/imgproc/shapedescriptors/find_contours/find_contours.html

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

using namespace cv;
using namespace std;

void transform();

int setpoint;
string name;

int main() {
	cout << "Choose an image to transform: 1, 2, 3, 4, 5...\n\npress esc to quit\n\n-------------------------------------------------------------------------\n" << endl;
	while (1) {
		namedWindow("User Select", CV_WINDOW_NORMAL);
		switch (waitKey(0)) {
		case 27: // Exit prompt "esc"
			return 0;
		case 49: // Image 1 selected
			setpoint = 1;
			name = "1.jpg";
			transform();
			break;
		case 50: // Image 2 selected
			setpoint = 2;
			name = "2.jpg";
			transform();
			break;
		case 51: // Image 3 selected
			setpoint = 3;
			name = "3.jpg";
			transform();
			break;
		case 52: // Image 4 selected
			setpoint = 4;
			name = "4.jpg";
			transform();
			break;
		case 53: // Image 5 selected
			setpoint = 5;
			name = "5.jpg";
			transform();
			break;
		}
	}
	return 0;
}

void transform() {
	// Images stored to memory for transformations
	Mat image, greyscale, threshold;

	// Read the image to memory and resize
	image = imread(name);   
	resize(image, image, Size(), 0.5, 0.5);

	// Convert BGR to Greyscale
	cvtColor(image, greyscale, CV_BGR2GRAY);
	
	// Preform Gaussian blur to smooth the image and reduce noise
	blur(greyscale, greyscale, Size(3, 3));

	// Use canny to detect the edges
	Canny(greyscale, threshold, 150, 300, 3);
	
	// Dilation and errosion used for multi level channel processing, reduced noise
	dilate(threshold, threshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	erode(threshold, threshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	// Finding contours variables and vectors
	RNG rng(12345);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	
	// Find the contours, example linked from here http://docs.opencv.org/2.4/doc/tutorials/imgproc/shapedescriptors/find_contours/find_contours.html
	findContours(threshold, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));
	
	// Remove letter A from image 3 only
	if (contours.size() > 2) {
		contours.erase(contours.begin() + 1, contours.end() - 1);
	}

	// Find the rotated rectangles contour
	vector<RotatedRect> minRect(contours.size());
	for (int i = 0; i < contours.size(); i++) {
		minRect[i] = minAreaRect(Mat(contours[i]));
	}

	// Find bounding rects, circles, and approximate contours to polygons
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());

	for (int i = 0; i < contours.size(); i++) {
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
	}

	// Final image
	Mat final = Mat::zeros(threshold.size(), CV_8UC3);
	
	// Identify the working image
	cout << "For image " << name << "\n" << endl;
	
	// Drawing contours for each image
	for (int i = 0; i< 2; i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(final, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		rectangle(final, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);

		// Rotated rectangle
		Point2f rect_points[4]; minRect[i].points(rect_points);
		for (int j = 0; j < 4; j++) {
			line(final, rect_points[j], rect_points[(j + 1) % 4], color, 1, 8);
		}
		
		// Final centre circle and translation arrow
		circle(final, minRect[i].center, 4, color, -1, 8, 0);
		
		// Display the translational arrow
		arrowedLine(final, minRect[0].center, minRect[1].center, color, 1, 8);
		
		// Read out the center coordinates
		cout << "Coordinates for contour " << i + 1 << " are " << minRect[i].center << endl;
	}

	// Find translation
	// Variables for the translation coordinates
	double Ax = minRect[1].center.x;
	double Ay = minRect[1].center.y;
	double Bx = minRect[0].center.x;
	double By = minRect[0].center.y;
	double translation;
	double Tx = -(Bx - Ax);
	double Ty = -(By - Ay);
	double Tz = 0;
	
	// Preform the translation distance equation
	translation = sqrt(((Bx - Ax)*(Bx - Ax)) + ((By - Ay)*(By - Ay)));

	// Find the rotation of the first character
	double deg0 = minRect[0].angle;
	if (minRect[0].size.width < minRect[0].size.height) {
		deg0 = 90 + deg0;
	}
	
	// Find the rotation of the seccond character
	double deg1 = minRect[1].angle;
	if (minRect[1].size.width < minRect[1].size.height) {
		deg1 = 90 + deg1;
	}
	
	// Find the difference in rotation between characters
	double rotation = (deg0 - deg1);

	// Preform the transform to the matrix table in the z direction
	double transform[4][4] = {
		{ (cos(rotation)), (sin(rotation)), 0, Tx },
		{ (-sin(rotation)), (cos(rotation)), 0, Ty },
		{ 0, 0, 1, Tz },
		{ 0,0,0,1 }
	};

	// Display the homogeneous transform matrix 
	cout << "\nHomogeneous Transfomation Matrix";
	for (int i = 0; i < 4; i++) {
		printf("\n[");
		for (int j = 0; j < 4; j++) {
			printf("%f, ", transform[i][j]);
		}
		cout << "]";
	}

	// Display the rotation and translation between frames
	cout << "\n\nRotation between characters " << rotation << "deg" << endl;
	cout << "Translation between characters " << translation << endl;
	cout << "\n\n" << endl;
	
	// After processing and transforming is complete, display final results, and save for reference
	// Naming the saved output files for reference
	ostringstream save;
	// Image save locations
	string savename = "Output/Final Image ";
	// Converge the string for each transformed image
	save << savename << setpoint << ".jpg";
	
	// Display the initial image 
	namedWindow("Initial Image", CV_WINDOW_NORMAL);  
	imshow("Initial Image", image);
	// Display the transformed image
	imshow(save.str(), final);  
	imwrite(save.str(), final);
	
	// Wait for key press before returning to main menu
	waitKey(0);
	destroyAllWindows();
}
