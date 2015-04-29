#include "perf_precomp.hpp"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;
using namespace perf;
using std::tr1::make_tuple;
using std::tr1::get;

CV_ENUM(AgastType, AgastFeatureDetector::AGAST_5_8, AgastFeatureDetector::AGAST_7_12d,
                   AgastFeatureDetector::AGAST_7_12s, AgastFeatureDetector::OAST_9_16)

typedef std::tr1::tuple<string, AgastType> File_Type_t;
typedef perf::TestBaseWithParam<File_Type_t> agast;

#define AGAST_IMAGES \
    "cv/detectors_descriptors_evaluation/images_datasets/leuven/img1.png",\
    "stitching/a3.png"

PERF_TEST_P(agast, detect, testing::Combine(
                            testing::Values(AGAST_IMAGES),
                            AgastType::all()
                          ))
{
    string filename = getDataPath(get<0>(GetParam()));
    int type = get<1>(GetParam());
    Mat frame = imread(filename, IMREAD_GRAYSCALE);

    if (frame.empty())
        FAIL() << "Unable to load source image " << filename;

    declare.in(frame);

    Ptr<FeatureDetector> fd = AgastFeatureDetector::create(20, true, type);
    ASSERT_FALSE( fd.empty() );
    vector<KeyPoint> points;

    TEST_CYCLE() fd->detect(frame, points);

    SANITY_CHECK_KEYPOINTS(points);
}
