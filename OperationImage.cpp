#include "OperationImage.h"
using namespace cv;

OperationImage::OperationImage(const OperationImage& oldImage)
{
	OperateImage = new cv::Mat;
	*OperateImage = oldImage.OperateImage->clone();
	imageColorSpace = oldImage.imageColorSpace;
	colorBits = oldImage.colorBits;
	minMaxLoc(*OperateImage, &minVal, &maxVal);
}

OperationImage::OperationImage(std::string image_path)
{
	OperateImage = new cv::Mat;
	*OperateImage = cv::imread(image_path, cv::IMREAD_COLOR);
	imageColorSpace = original;
	colorBits = 255;
	minMaxLoc(*OperateImage, &minVal, &maxVal);

}

cv::Mat OperationImage::createHistogram()
{
	if (imageColorSpace == grey) {
		Mat histOper;
		int histSize = 256;
		bool uniform = true;
		float range[] = { 0, 256 };
		const float* histRange[] = { range };

		calcHist(OperateImage, 1, 0, Mat(), histOper, 1, &histSize, histRange, uniform, accumulate);

		int hist_w = 512, hist_h = 400;
		int bin_w = cvRound((double)hist_w / histSize);
		Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
		normalize(histOper, histOper, 0, histImage.rows, NORM_MINMAX, -1, Mat());

		for (int i = 1; i < histSize; i++)
		{
			line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(histOper.at<float>(i - 1))),
				Point(bin_w * (i), hist_h - cvRound(histOper.at<float>(i))),
				Scalar(255, 255, 255), 2, 8, 0);
		}
		return histImage;
	}



	std::vector<Mat> bgr_planes;
	split(*OperateImage, bgr_planes);
	int histSize = 256;


	float range[] = { 0, 256 }; 
	const float* histRange[] = { range };

	bool uniform = true, accumulate = false;

	Mat b_hist, g_hist, r_hist;
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate);

	int hist_w = 512, hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());


	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}

	return histImage;

}

void OperationImage::fftShift(const cv::Mat& input, cv::Mat& output) {
	output = input.clone();
	int cx = output.cols / 2;
	int cy = output.rows / 2;
	// Create a ROI per quadrant
	Mat q0(output, Rect(0, 0, cx, cy));             // Top-Left
	Mat q1(output, Rect(cx, 0, cx, cy));            // Top-Right
	Mat q2(output, Rect(0, cy, cx, cy));            // Bottom-Left
	Mat q3(output, Rect(cx, cy, cx, cy));           // Bottom-Right

	// swap quadrants (Top-Left with Bottom-Right)
	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	// swap quadrant (Top-Right with Bottom-Left)
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);
}

cv::Mat OperationImage::createWidmo(){
	Mat modImg = OperateImage->clone();
	if(imageColorSpace != grey) cvtColor(*OperateImage, modImg, COLOR_BGR2GRAY);

	Mat padded;
	int m = getOptimalDFTSize(modImg.rows);
	int n = getOptimalDFTSize(modImg.cols);

	//expand input image to optimal size, on the border add zero values
	copyMakeBorder(modImg, padded, 0, m - modImg.rows, 0, n - modImg.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	Mat complexImg;

	merge(planes, 2, complexImg);                   // Add to the expanded another plane with zeros
	dft(complexImg, complexImg);                    // this way the result may fit in the source matrix

	// compute the magnitude and switch to logarithmic scale
	// => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
	split(complexImg, planes);                      // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
	magnitude(planes[0], planes[1], planes[0]);     // planes[0] = magnitude

	Mat magImg = planes[0];
	magImg += Scalar::all(1);                       // switch to logarithmic scale
	log(magImg, magImg);

	// crop the spectrum, if it has an odd number of rows or columns
	magImg = magImg(Rect(0, 0, magImg.cols & -2, magImg.rows & -2));

	// rearrange the quadrants of Fourier image so that the origin is at the image center
	fftShift(magImg, magImg);

	// transform the matrix with float values into a viewable image form (float between values 0 and 1).
	normalize(magImg, magImg, 0, 1, NORM_MINMAX);
	//normalize(magImg, magImg, 0, magImg.rows, NORM_MINMAX, -1, Mat());


	//normalization toooo QT widgets
	int w = magImg.cols;
	int h = magImg.rows;
	QImage qim(w, h, QImage::Format_RGB32);
	QRgb pixel;
	Mat im;
	normalize(magImg.clone(), im, 0.0, 255.0, NORM_MINMAX, CV_8UC1);
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			int gray = (int)im.at<unsigned char>(j, i);
			pixel = qRgb(gray, gray, gray);
			qim.setPixel(i, j, pixel);
		}
	}

	return im;
}



const cv::Mat* OperationImage::getImagePtr()
{

	return OperateImage;
}

cv::Mat OperationImage::binaryzation(int _trashold, int type)
{
	Mat tempResult = OperateImage->clone();
	if (imageColorSpace != grey) cvtColor(tempResult, tempResult, COLOR_BGR2GRAY); 
	threshold(tempResult, tempResult, _trashold, maxVal, type);
	return tempResult;

}

void OperationImage::setiageColorSpace(colorSpace _colorBits){
	imageColorSpace = _colorBits;
}

colorSpace OperationImage::getColorSpace(){
	return imageColorSpace;
}

void OperationImage::updateinaxValues(){
	minMaxLoc(*OperateImage, &minVal, &maxVal);
}

void OperationImage::setNewImage(cv::Mat* newImage){
	newImage->copyTo(*OperateImage);
}

cv::Mat OperationImage::addSubRGB(int R = 0, int G = 0, int B = 0) {
	if (imageColorSpace == grey) {
		return *OperateImage;
	}
	Mat img = OperateImage->clone();
	Mat Bands[3];
	split(img, Bands);
	std::vector<Mat> channels = { Bands[0],Bands[1],Bands[2] };
	add(Bands[0], B, Bands[0]);
	add(Bands[1], G, Bands[1]);
	add(Bands[2], R, Bands[2]);
	merge(channels, img);
	return img;
}

cv::Mat OperationImage::ContrastBrightness(float contrast, float btightness){
	Mat new_image = OperateImage->clone();
	OperateImage->convertTo(new_image, -1, contrast, btightness);
	return new_image;
}

cv::Mat OperationImage::filterMedian(int ksize){
	Mat newImage = Mat::zeros(OperateImage->size(), OperateImage->type());
	try { medianBlur(*OperateImage, newImage, ksize); }
	catch (Exception e) { printf("hii"); }
	return newImage;
}

cv::Mat OperationImage::filters(int ksize, filterOper operTypt){
	Mat newImage = Mat::zeros(OperateImage->size(), OperateImage->type());
	switch (operTypt) {
		case medianFiltrType: {
			medianBlur(*OperateImage, newImage, ksize);
		}break;
		case avgFiltrType: {
			blur(*OperateImage, newImage, Size2i(ksize, ksize));
		}break;
	}
	return newImage;
}

cv::Mat OperationImage::canny(double treshold1, double treshold2){
	Mat newImage = Mat::zeros(OperateImage->size(), OperateImage->type());
	Canny(*OperateImage, newImage, treshold1, treshold2);
	return newImage;
}

cv::Mat OperationImage::noiseGaussian(double mean, double dev){
	Mat mSrc_16SC;
	Mat mGaussian_noise = Mat(OperateImage->size(), CV_16SC3);
	randn(mGaussian_noise, Scalar::all(mean), Scalar::all(dev));
	Mat resultImage = OperateImage->clone();
	resultImage.convertTo(mSrc_16SC, CV_16SC3);
	addWeighted(mSrc_16SC, 1.0, mGaussian_noise, 1.0, 0.0, mSrc_16SC);
	mSrc_16SC.convertTo(resultImage, resultImage.type());
	return resultImage;
}

cv::Mat OperationImage::unsharpMask(double sigma, double treshold, double amount){
	Mat blurred, lowContrastMask, sharpened;
	Mat resultImage = OperateImage->clone();
	GaussianBlur(resultImage, blurred, Size(), sigma, sigma);
	lowContrastMask = abs(resultImage - blurred) < treshold;
	sharpened = resultImage * (1 + amount) + blurred * (-amount);
	resultImage.copyTo(sharpened, lowContrastMask);
	sharpened.copyTo(resultImage);
	return resultImage;
}

cv::Mat OperationImage::morphoOperation(int kernelSize, morphoOper operation){
	Mat kernel = Mat::ones(kernelSize, kernelSize, CV_32F) / ((double)kernelSize * (double)kernelSize);
	Mat resultImage = OperateImage->clone();
	switch (operation) {
		case mClose: {
			morphologyEx(resultImage, resultImage, MORPH_CLOSE, kernel);
		}break;
		case mOpen: {
			morphologyEx(resultImage, resultImage, MORPH_OPEN, kernel);
		}break;
		case mDilate: {
			morphologyEx(resultImage, resultImage, MORPH_DILATE, kernel);
		}break;
		case mErode: {
			morphologyEx(resultImage, resultImage, MORPH_ERODE, kernel);
		}break;
		case mCross: {
			morphologyEx(resultImage, resultImage, MORPH_CROSS, kernel);
		}break;
		case mEllipse: {
			morphologyEx(resultImage, resultImage, MORPH_ELLIPSE, kernel);
		}break;
		case mGradient: {
			morphologyEx(resultImage, resultImage, MORPH_GRADIENT, kernel);
		}break;
		case mBlackhat: {
			morphologyEx(resultImage, resultImage, MORPH_BLACKHAT, kernel);
		}break;
		case mTophat: {
			morphologyEx(resultImage, resultImage, MORPH_TOPHAT, kernel);
		}break;
		default: printf("Error");
	}
	return resultImage;
}

