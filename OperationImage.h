#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgcodecs.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <QMainWindow>

enum colorSpace { original, grey };
enum filterOper { medianFiltrType, avgFiltrType };
enum morphoOper { mClose, mOpen, mDilate, mErode, mCross, mEllipse, mGradient, mBlackhat, mTophat };
class OperationImage
{
private:
	cv::Mat* OperateImage;
	//cv::Mat Histogram;
	colorSpace imageColorSpace;
	int colorBits;
	double minVal;
	double maxVal;

public:
	cv::Mat createHistogram();
	void fftShift(const cv::Mat& input, cv::Mat& output);
	cv::Mat createWidmo();


	OperationImage() = default;
	OperationImage(const OperationImage& oldImage);
	OperationImage(std::string image_path);
	cv::Mat binaryzation(int _trashold, int type);
	const cv::Mat* getImagePtr();
	//const cv::Mat* getHistogramPtr();
	void setiageColorSpace( colorSpace _colorBits = original);
	colorSpace getColorSpace();
	void updateinaxValues();
	void setNewImage(cv::Mat* newImage);
	cv::Mat addSubRGB(int R, int G, int B);
	//void generateHistogram();
	cv::Mat ContrastBrightness(float contrast, float btightness);

	cv::Mat filterMedian(int ksize);
	cv::Mat filters(int ksize, filterOper operTypt);
	cv::Mat canny(double treshold1, double treshold2);
	cv::Mat noiseGaussian(double mean, double dev);

	cv::Mat unsharpMask(double sigma, double treshold, double amount);
	cv::Mat morphoOperation(int kernelSize, morphoOper operation);
};

