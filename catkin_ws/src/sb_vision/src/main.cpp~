#include <opencv2/improc/improc.h>
#include <opencv2/highgui/highgui.h>
#include <cv_bridge/cv_bridge.h>

int main(int argc, const char* argv[])
{
    const cv::Mat input = cv::imread("input.jpg", 0); //Load as grayscale

    cv::SiftFeatureDetector detector;
    std::vector<cv::KeyPoint> keypoints;
    detector.detect(input, keypoints);

    // Add results to image and save.
    cv::Mat output;
    cv::drawKeypoints(input, keypoints, output);
    cv::imwrite("sift_result.jpg", output);

    return 0;
}
