#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <string>
#include <ios>

using namespace cv;
using namespace std;

//define top and bottom bounds of filter
int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 0;
int V_MAX = 256;
int DILATE = 1;
int ERODE = 1;


void on_trackbar(int, void*){
	//for some reason this is needed even though it doesnt do anything, as createTrackbar() has a last parameter that is type Trackbar Callback (int, void*)
}

void createSliders() {
	namedWindow("Sliders", 0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf(TrackbarName, "H_MIN", H_MIN);
	sprintf(TrackbarName, "H_MAX", H_MAX);
	sprintf(TrackbarName, "S_MIN", S_MIN);
	sprintf(TrackbarName, "S_MAX", S_MAX);
	sprintf(TrackbarName, "V_MIN", V_MIN);
	sprintf(TrackbarName, "V_MAX", V_MAX);
	sprintf(TrackbarName, "DILATE", DILATE);
	sprintf(TrackbarName, "ERODE", ERODE);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->      
	createTrackbar("H_MIN", "Sliders", &H_MIN, H_MAX, on_trackbar);
	createTrackbar("H_MAX", "Sliders", &H_MAX, H_MAX, on_trackbar);
	createTrackbar("S_MIN", "Sliders", &S_MIN, S_MAX, on_trackbar);
	createTrackbar("S_MAX", "Sliders", &S_MAX, S_MAX, on_trackbar);
	createTrackbar("V_MIN", "Sliders", &V_MIN, V_MAX, on_trackbar);
	createTrackbar("V_MAX", "Sliders", &V_MAX, V_MAX, on_trackbar);
	createTrackbar("V_MAX", "Sliders", &V_MAX, V_MAX, on_trackbar);
	createTrackbar("V_MAX", "Sliders", &V_MAX, V_MAX, on_trackbar);
	createTrackbar("DILATE", "Sliders", &DILATE, 10, on_trackbar);
	createTrackbar("ERODE", "Sliders", &ERODE, 10, on_trackbar);

}

int main(int argc, char** argv)
{
	//creates the sliders
	createSliders();

	//define the video capture
	VideoCapture cap(0);

	//if it doesn't open, end program
	if (!cap.isOpened()) {
		cout << "Webcam never opened" << endl;
		return -1;
	}

	//define the pixel matrices to store images
	Mat orig;
	Mat greyscale;
	Mat filt;
	Mat tracking;

	string str;

	//define the windows to display within
	namedWindow("Original");
	namedWindow("Filtered");
	//namedWindow("Tracking");
	//namedWindow("Circles");

	//accept escape input 
	waitKey(1000);
	char pressed = 0;

	int moveAvg[5] = { 0,0,0,0,0 };
	int place = 0;

	//while they haven't pressed escape. Check webcam read and if NULL display error
	while (pressed != 27) {
	
		if (cap.read(orig) == NULL) {
			cout << "Frame cannot be read" << endl;
			break;
		}

		//input orig, output orig, convert from Blue Green Red to Hue Saturation Value for better readings
		cvtColor(orig, greyscale, CV_BGR2GRAY);

		//input orig, HUE [6,22], SATURATION [100,255], VALUE [230,255] , output to filt
		inRange(greyscale, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, H_MAX), filt);

		//GaussianBlur(filt, filt,Size(10,10),);

		//get rid of noise
		erode(filt, filt, getStructuringElement(MORPH_ELLIPSE, Size(ERODE, ERODE)));
		dilate(filt, filt, getStructuringElement(MORPH_ELLIPSE, Size(DILATE, DILATE)));

		vector<Vec3f> circles;
		//input, output, mode (only mode right now), quality  (1 is source), min dist between circles, param 1 (?), param 2(lower = more sensitive), min radius, max radius
		HoughCircles(filt, circles, CV_HOUGH_GRADIENT, 1, 25, 100, 13, 10, 50); 

		
	//	if (circles.size() == 0) exit(-1);

		for (size_t current_circle = 0; current_circle < circles.size(); ++current_circle) {
			Point center(round(circles[current_circle][0]), round(circles[current_circle][1]));
			int radius = round(circles[current_circle][2]);
			//cout << radius << endl;
			circle(orig, center, radius, Scalar(0, 255, 0), 8);
			
		}

		moveAvg[place] = circles.size();
		double tot = 0.0;
		for (int i = 0; i < 5; i++) {
			tot += moveAvg[i];
		}
		cout << fixed << "DICE VALUE: " << (int) round(tot/5) << endl;
		place++;
		if (place == 5) place = 0;
	
	//display the two image
	imshow("Original", orig);
	imshow("Filtered", filt);

	//wait for additional keypress
	pressed = waitKey(10);
	}
	return 0;
}
