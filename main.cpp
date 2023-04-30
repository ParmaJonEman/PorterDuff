#include "main.h"
using namespace cv;
int main(int argc, char **argv) {
    String imageFile1;
    String imageFile2;
    String maskFile1;
    String maskFile2;

    Mat image1;
    Mat image2;
    Mat mask1;
    Mat mask2;

    if(!parseParameters(argc, argv, &imageFile1, &imageFile2, &maskFile1, &maskFile2)){
        return 0;
    };
    try {
        if (!imageFile1.empty()) {
            image1 = imread(imageFile1);
            if (image1.empty())
                throw (String("Cannot open input image ") + imageFile1);
        } else {
            image1 = createImage1();
        }
        if (!imageFile2.empty()) {
            image2 = imread(imageFile2);
            if (image2.empty())
                throw (String("Cannot open input image ") + imageFile2);
        } else {
            image2 = createImage2();
        }
        if(image1.size()!=image2.size()){
            Mat image3 = Mat::zeros(image2.size(), CV_8UC3);
            resize(image1, image3, image2.size());
            image1 = image3;
        }
        if (!maskFile1.empty()) {
            mask1 = imread(maskFile1);
            if (mask2.empty())
                throw (String("Cannot open input image ") + maskFile1);
        } else {
            mask1 = createMask(image1);
        }
        if (!maskFile2.empty()) {
            mask2 = imread(maskFile2);
            if (mask2.empty())
                throw (String("Cannot open input image ") + maskFile2);
        } else {
            mask2 = createMask(image2);
        }

    }
    catch (String &str) {
        std::cerr << "Error: " << argv[0] << ": " << str << std::endl;
        return (1);
    }
    catch (Exception &e) {
        std::cerr << "Error: " << argv[0] << ": " << e.msg << std::endl;
        return (1);
    }
    imshow("image1", image1);
    imshow("mask1", mask1);
    imshow("image2", image2);
    imshow("mask2", mask2);
    waitKey();

    Mat aOverB = over(image1, image2, mask1, mask2);
    Mat bOverA = over(image2, image1, mask2, mask1);
    imshow("aOverB", aOverB);
    imshow("bOverA", bOverA);
    waitKey();

    Mat aInsideB = inside(image1, mask1, mask2);
    Mat bInsideA = inside(image2, mask2, mask1);
    imshow("aInsideB", aInsideB);
    imshow("bInsideA", bInsideA);
    waitKey();


    Mat aOutsideB = outside(image1, mask1, mask2);
    Mat bOutsideA = outside(image2, mask2, mask1);
    imshow("aOutsideB", aOutsideB);
    imshow("bOutsideA", bOutsideA);
    waitKey();

    Mat aAtopB = atop(image1, image2, mask1, mask2);
    Mat bAtopA = atop(image2, image1, mask2, mask1);
    imshow("aAtopB", aAtopB);
    imshow("bAtopA", bAtopA);
    waitKey();

    Mat aXORB = xOrB(image1, image2, mask1, mask2);
    imshow("aXORB", aXORB);
    waitKey();
    return 0;
}

static Mat createMask(Mat image){
    Mat mask = Mat::zeros(image.size(), CV_8U);
    for(int i = 0; i < image.rows; i++){
        for(int j = 0; j< image.cols; j++){
            if((image.at<Vec3b>(i, j) != Vec3b(0,0,0))){
                mask.at<uchar>(i, j) = 255;
            }
        }
    }
    return mask;
}

static Mat createImage1(){
    Mat image = Mat::zeros(Size(640, 480), CV_8UC3);

    circle(image,
           Point((640/2), (480/2)), 150,
           Scalar(255, 0, 0), //blue
           FILLED, LINE_8
    );
    return image;
}

static Mat createImage2(){
    Mat image = Mat::zeros(Size(640, 480), CV_8UC3);

    rectangle(image,
              Rect((640/2)-(512/2),(480/2)-(96/2), 512, 96),
              Scalar(0, 0, 255),
              FILLED, LINE_8
    );

    rectangle(image,
              Rect((640/2)-(128/2),(480/2)-(384/2), 128, 384),
              Scalar(0, 0, 255),
              FILLED, LINE_8
    );
    return image;
}

static Mat over(Mat imageA, Mat imageB, Mat maskA, Mat maskB){
    Mat result = Mat::zeros(imageA.size(), CV_8UC3);

    for(int i = 0; i < maskA.rows; i++){
        for(int j = 0; j < maskA.cols; j++){
            if(maskA.at<uchar>(i, j)){
                result.at<Vec3b>(i, j) = imageA.at<Vec3b>(i, j);
            }
            else if(maskB.at<uchar>(i, j)){
                result.at<Vec3b>(i, j) = imageB.at<Vec3b>(i, j);
            }
        }
    }
    return result;
}

static Mat inside(Mat imageA, Mat maskA, Mat maskB){
    Mat result = Mat::zeros(imageA.size(), CV_8UC3);

    for(int i = 0; i < maskA.rows; i++){
        for(int j = 0; j< maskA.cols; j++){
            if((maskA.at<uchar>(i,j)) && (maskB.at<uchar>(i,j))){
                result.at<Vec3b>(i, j) = imageA.at<Vec3b>(i, j);
            }
        }
    }
    return result;
}

static Mat outside(Mat imageA, Mat maskA, Mat maskB){
    Mat result = Mat::zeros(imageA.size(), CV_8UC3);

    for(int i = 0; i < maskA.rows; i++){
        for(int j = 0; j< maskA.cols; j++){
            if(maskA.at<uchar>(i, j) && !maskB.at<uchar>(i, j)){
                result.at<Vec3b>(i, j) = imageA.at<Vec3b>(i, j);
            }
        }
    }
    return result;
}

static Mat atop(Mat imageA, Mat imageB, Mat maskA, Mat maskB){
    Mat result = Mat::zeros(imageA.size(), CV_8UC3);

    for(int i = 0; i < maskA.rows; i++){
        for(int j = 0; j< maskA.cols; j++){
            if(maskB.at<uchar>(i, j)) {
                if (maskA.at<uchar>(i, j)) {
                    result.at<Vec3b>(i, j) = imageA.at<Vec3b>(i, j);
                }
                else{
                    result.at<Vec3b>(i, j) = imageB.at<Vec3b>(i, j);
                }
            }
        }
    }
    return result;
}

static Mat xOrB(Mat imageA, Mat imageB, Mat maskA, Mat maskB){
    Mat result = Mat::zeros(imageA.size(), CV_8UC3);

    for(int i = 0; i < maskA.rows; i++){
        for(int j = 0; j< maskA.cols; j++){
            if(maskA.at<uchar>(i, j) && !maskB.at<uchar>(i, j)){
                result.at<Vec3b>(i, j) = imageA.at<Vec3b>(i, j);
            }
            if(!maskA.at<uchar>(i, j) && maskB.at<uchar>(i, j)){
                result.at<Vec3b>(i, j) = imageB.at<Vec3b>(i, j);
            }
        }
    }
    return result;
}

static int parseParameters(int argc, char** argv, String* imageFile1, String* imageFile2, String* maskFile1, String* maskFile2)
{
    String keys =
            {
                    "{help h usage ? |                            | print this message   }"
                    "{@imageFile1      || first image you want to use	}"
                    "{@imageFile2      || second image you want to use	}"
                    "{@maskFile1      || first image you want to use	}"
                    "{@maskFile2      || second image you want to use	}"
            };

    // Get required parameters. If any are left blank, defaults are set based on the above table
    // If no directory is passed in, or if the user passes in a help param, usage info is printed
    CommandLineParser parser(argc, argv, keys);
    parser.about("porter-duff v1.0");

    if (parser.has("help"))
    {
        parser.printMessage();
        return(0);
    }
    if (parser.has("@imageFile1"))
    {
        *imageFile1 = parser.get<String>("@imageFile1");
    }
    if (parser.has("@imageFile2"))
    {
        *imageFile2 = parser.get<String>("@imageFile2");
    }
    if (parser.has("@maskFile1"))
    {
        *maskFile1 = parser.get<String>("@maskFile1");
    }
    if (parser.has("@maskFile2"))
    {
        *maskFile2 = parser.get<String>("@maskFile2");
    }

    return(1);
}