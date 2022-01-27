//Original Source: https://github.com/opencv/opencv_contrib/blob/master/modules/aruco/samples/detect_markers.cpp
//Edits made by ModalAI.com to clear non-apriltag items

#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>
#include <iostream>
#include <iomanip>

//3 is the aruco parameter for AprilTag
#define APRIL_DET_PARAM 3 
//aruco has 17 other (not april) detectors that we're ignoring
#define NUM_IGNORED_DICTIONARIES 17

using namespace std;
using namespace cv;

namespace {
    const char* about = "Basic marker detection";
    const char* keys  =
        "{h        |       | Help, show this message }"
        "{d        | 3     | Dictionary Selection: AprilTag-16h5=0, AprilTag-25h9=1, AprilTag-36h10=2, AprilTag-36h11=3}"
        "{f        |       | Input from file, if ommited, input comes from camera }"
        "{ci       | -1    | Camera id if input doesnt come from file (-f) }"
        "{of       | 30    | Output frequency for info }"
        "{r        |       | show rejected candidates too }"
        "{i        |       | show additional info}"
        "{gui      |       | show graphical items}";
}

int main(int argc, char *argv[]) {

    //Parse options
    CommandLineParser parser(argc, argv, keys);
    parser.about(about);

    //Help mode: print help message then quit
    if(parser.has("h")){
        parser.printMessage();
        return 0;
    }

    //Get parsed values
    int dictionaryId = parser.get<int>("d");
    int camId = parser.get<int>("ci");
    bool showRejected = parser.has("r");
    bool showInfo = parser.has("i");
    bool drawResults = parser.has("gui");
    int frequency = parser.get<int>("of");

    //Create and configure the detector
    Ptr<aruco::DetectorParameters> detectorParams = aruco::DetectorParameters::create();
    detectorParams->cornerRefinementMethod = APRIL_DET_PARAM;//Set detector to AprilTag mode

    //Check for any input errors before proceeding
    if(!parser.check()) {
        parser.printErrors();
        return 0;
    }    

    //Set ID dictionary to user selection (default is 36h11)
    Ptr<aruco::Dictionary> dictionary =
        aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(dictionaryId+NUM_IGNORED_DICTIONARIES));

    VideoCapture input;
    int waitTime;

    //Check for video file or camera input
    if(parser.has("f")) {
        input.open(parser.get<String>("f"));
        waitTime = 0;
    } else {
        input.open(camId, cv::CAP_V4L);
        waitTime = 10;
    }

    double totalTime = 0;
    int totalMatches = 0;
    int iterationCount = 0;

    if(showInfo){
        //Format output for info
        cout << fixed << setprecision(2);
    }

    while(input.grab()) {
        Mat image, imageCopy;
        input.retrieve(image);
        double tick = (double)getTickCount();

        vector< int > ids;
        vector< vector< Point2f > > corners, rejected;

        //Detect markers
        aruco::detectMarkers(image, dictionary, corners, ids, detectorParams, rejected);
        
        //Calculate cycle times
        double currentTime = ((double)getTickCount() - tick) / getTickFrequency();
        totalTime += currentTime;
        totalMatches += ids.size();
        iterationCount++;    

        //Show additional info
        if(showInfo && iterationCount == frequency) {
            cout << "Average Cycle Time = " << 1000 * totalTime / double(frequency) << " ms" 
                << ", Average # Matches = " << ((double)totalMatches)/frequency << endl;
            totalTime = 0;
            totalMatches = 0;
            iterationCount = 0;
        }

        //Draw Graphics
        if(drawResults){
        
            //Draw results
            if(ids.size() > 0){
                aruco::drawDetectedMarkers(imageCopy, corners, ids);
            }
        
            //Show rejected candidates
            if(showRejected && rejected.size() > 0){
                aruco::drawDetectedMarkers(imageCopy, rejected, noArray(), Scalar(100, 0, 255));
            }
            
            imshow("out", imageCopy);

            //Escape key will exit
            char key = (char)waitKey(waitTime);
            if(key == 27) return 0;
        }

    }

    return 0;
}
