// MatchFrame.cpp : Defines the entry point for the console application.
//

#include <iostream>

#include "opencv2/videoio.hpp"

// findmatch refvid refframe tgtvid

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	int retval = EXIT_FAILURE;

	if (argc < 4) {
		cerr << "Get specified frame from specified reference video and return frame number" << endl
             << "of nearest match in target video." << endl << endl
			 << "Usage: " << argv[0] << " <reference video file> <reference frame number> <target video file>" << endl;
		goto Error;
	}

	{
		VideoCapture refvid(argv[1]);
		if (!refvid.isOpened()) {
			cerr << "ERROR: Unable to open reference video (\"" << argv[1] << "\")" << endl;
			goto Error;
		}
cerr << "* Reference video opened" << endl;
		VideoCapture tgtvid(argv[3]);
		if (!tgtvid.isOpened()) {
			cerr << "ERROR: Unable to open target video (\"" << argv[3] << "\")" << endl;
			goto Error;
		}
cerr << "* Target video opened" << endl;
		if (!refvid.set(CAP_PROP_POS_FRAMES, (double)atoi(argv[2]))) {
			cerr << "ERROR: Invalid reference video frame specified (" << argv[2] << ")" << endl;
			goto Error;
		}
		Mat refframe;
		if (!refvid.read(refframe)) {
			cerr << "ERROR: Unable to read frame (" << argv[2] << ") from reference video" << endl;
			goto Error;
		}
cerr << "* Reference frame read" << endl;
		int tgtframes = (int)tgtvid.get(CAP_PROP_FRAME_COUNT);
cerr << "* Target has " << tgtframes << " frames" << endl;
		int bestFrame = 0;
		double bestMatch = DBL_MAX;
		Mat checkframe;
		for (int nextFrame = 0; nextFrame < tgtframes; nextFrame++) {
//cerr << "* Checking frame " << nextFrame << endl;
//checkframe.release();
			if (!tgtvid.read(checkframe)) {
				cerr << "ERROR: Error reading frame (" << nextFrame << ") from target video" << endl;
				goto Error;
			}
			double nextMatch = norm(refframe, checkframe, CV_L2);
			if (nextMatch < bestMatch) {
cerr << "* Frame " << nextFrame << " is better, with a match of " << nextMatch << endl;
				bestFrame = nextFrame;
				bestMatch = nextMatch;
			}
//else {
//	cerr << "* Frame " << nextFrame << " difference (" << nextMatch << ") IS NOT better than best (" << bestMatch << ")" << endl;
//}
		}
		cout << bestFrame;
	}

	retval = EXIT_SUCCESS;
Error:
	return retval;
}

