#pragma once

#include "OperationImage.h"

#include <opencv2/core/core.hpp>

#include <opencv2/imgcodecs.hpp>
#include "opencv2/imgproc/imgproc.hpp"


#include <QtWidgets/QMainWindow>
#include "ui_ICMUProject.h"
#include "MainEngine.h"
#include <opencv2/opencv.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <stack>

//enum morphoOper { mClose, mOpen, mDilate, mErode, mCross, mEllipse, mGradient, mBlackhat, mTophat };
enum extensionViewType{ widmo, histogram};
class MainEngine
{
private:
	std::string image_path;
	//OperationImage IimagesHistory[10];
	std::stack<OperationImage> imagesHistory;
	OperationImage mainImageA;
	OperationImage mainImageB;
	extensionViewType extensionView;
	QImage mainImageDispl;

	const Ui::ICMUProjectClass *ui;
public:
	MainEngine() = default;
	MainEngine(const Ui::ICMUProjectClass* _ui);
	void binaryzeImage(int _type=1, int _trashold = 128);

	void displayMainImageLabels( QLabel *imgLab,  QLabel* imgLabB);
	void displayImageInLabel(const cv::Mat* image,  QLabel* imgLab);
	void displayextensions();
	void refresWorkspace();
	void fitmainImageToLabel(cv::Mat* imageToFit, QLabel* lebel);
	void fitExtansionToLabel(cv::Mat* imageToFit, QLabel* lebel);
	bool upluadNewImage(std::string path);

	void saveImage(std::string path);
	void changeExtansionView(extensionViewType toChange);
	//Verificted
	void drowHistogrem(OperationImage* operationImagePtr, QLabel* imgLabToInsert);
	void drowFae(OperationImage* operationImagePtr, QLabel* imgLabToInsert);
	void displayResultsExtension();
	void saveChanges();
	void redoChanges();
	void changeContrastBrightnes(float Contrast, float Brightness);
	void addSubRGB(int R, int G, int B);
	void filters(int ksize, filterOper filterType);
	void canny(double treshold1, double treshold2);
	void addNoiseGaussian(double mean, double dev);
	void unsharpMask(double sigma, double treshold, double amount);
	void morphoOperation(int kernelSize, morphoOper operrationType);

};

