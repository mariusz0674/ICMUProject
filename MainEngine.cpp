#include "MainEngine.h"



MainEngine::MainEngine(const Ui::ICMUProjectClass* _ui){
	ui = _ui;
	extensionView = histogram;
	//image_path = "C:/Users/mariu/OneDrive/Obrazy/lena.png";
	image_path = "Lenna.png";
	mainImageA = OperationImage(image_path);
	mainImageB = OperationImage(image_path);
}

void MainEngine::binaryzeImage(int _type, int _trashold){
	cv::Mat imga = mainImageA.binaryzation(_trashold, _type);
	cv::Mat* img = new cv::Mat;
	imga.copyTo(*img);
	mainImageB.setNewImage(img);
	mainImageB.setiageColorSpace(grey);
	displayResultsExtension();
	displayImageInLabel(mainImageB.getImagePtr(), ui->mainImageBLabel);
	//refresWorkspace();

	
}

void MainEngine::displayMainImageLabels( QLabel* imgLab,  QLabel* imgLabB){
	displayImageInLabel(mainImageA.getImagePtr(), imgLab);
	displayImageInLabel(mainImageB.getImagePtr(), imgLabB);

}

void MainEngine::displayImageInLabel(const cv::Mat* image,  QLabel* imgLab){
	cv::Mat img = *image;
	fitmainImageToLabel(&img, imgLab);
}

void MainEngine::displayextensions(){
	switch (extensionView) {
	case histogram:
		drowHistogrem(&mainImageA, ui->extensionImageALabel);
		drowHistogrem(&mainImageB, ui->extensionImageBLabel);
		break;
	case widmo:
		drowFae(&mainImageA, ui->extensionImageALabel);
		drowFae(&mainImageB, ui->extensionImageBLabel);
		break;
	default:
		printf("error my friend");
	}
}

void MainEngine::refresWorkspace(){
	displayMainImageLabels(ui->mainImageALabel, ui->mainImageBLabel);
	displayextensions();
	displayResultsExtension();
	displayImageInLabel(mainImageB.getImagePtr(), ui->mainImageBLabel);
	displayImageInLabel(mainImageA.getImagePtr(), ui->mainImageALabel);

}






void MainEngine::fitmainImageToLabel(cv::Mat* imageToFit, QLabel* lebel){
	int border_v = 0;
	int border_h = 0;
	int lebHe = lebel->height();
	int lebVe = lebel->width();

	if ((imageToFit->cols / imageToFit->rows)>= (lebVe / lebVe) ) {
		border_v = int((((imageToFit->cols / imageToFit->rows) * lebVe) - lebVe) / 2.0);
	}
	else {
		border_h = int((((imageToFit->rows / imageToFit->cols) * lebVe) - lebVe) / 2.0);
	}
	cv::copyMakeBorder(*imageToFit, *imageToFit, border_v, border_v, border_h, border_h, 0, 0);

	//cv::imshow("bodz 4 / 3", *imageToFit);
	cv::resize(*imageToFit, *imageToFit, cv::Size(lebel->width(), lebel->height()), 0, 0, cv::INTER_LINEAR);
	cv::cvtColor(*imageToFit, *imageToFit, cv::COLOR_BGR2RGB); //Qt reads in RGB whereas CV in BGR
	QImage imdisplay((uchar*)imageToFit->data, imageToFit->cols, imageToFit->rows, imageToFit->step, QImage::Format_RGB888);
	lebel->setPixmap(QPixmap::fromImage(imdisplay));

}

void MainEngine::fitExtansionToLabel(cv::Mat* imageToFit, QLabel* lebel)
{
	cv::resize(*imageToFit, *imageToFit, cv::Size(lebel->width(), lebel->height()), 0, 0, cv::INTER_LINEAR);
	cv::cvtColor(*imageToFit, *imageToFit, cv::COLOR_BGR2RGB); //Qt reads in RGB whereas CV in BGR
	QImage imdisplay((uchar*)imageToFit->data, imageToFit->cols, imageToFit->rows, imageToFit->step, QImage::Format_RGB888);
	lebel->setPixmap(QPixmap::fromImage(imdisplay));

}

bool MainEngine::upluadNewImage(std::string path){
	//cv::Mat imageToAdd = cv::imread(path, 1);
	mainImageA = OperationImage(path);
	mainImageB = OperationImage(path);
	refresWorkspace();

	return false;
}

void MainEngine::saveImage(std::string path){
	bool isSuccess = cv::imwrite(path, mainImageA.getImagePtr()->clone());
	if (!isSuccess) {
		printf("penis");
	}
}


void MainEngine::changeExtansionView(extensionViewType toChange){
	extensionView = toChange;
	displayextensions();
}

void MainEngine::drowHistogrem(OperationImage* operationImagePtr, QLabel* imgLabToInsert){
	cv::Mat img = operationImagePtr->createHistogram();
	fitExtansionToLabel(&img, imgLabToInsert);
}

void MainEngine::drowFae(OperationImage* operationImagePtr, QLabel* imgLabToInsert){
	cv::Mat* penis= new cv::Mat;
	*penis = operationImagePtr->createWidmo().clone();
	fitExtansionToLabel(penis, imgLabToInsert);

	delete penis;
}

void MainEngine::displayResultsExtension() {
	switch (extensionView) {
	case histogram:
		drowHistogrem(&mainImageB, ui->extensionImageBLabel);
		break;
	case widmo:
		drowFae(&mainImageB, ui->extensionImageBLabel);
		break;
	default:
		printf("error my friend");
	}
}

void MainEngine::saveChanges(){
	imagesHistory.push(OperationImage(mainImageA));
	OperationImage* temp = new OperationImage;
	//temp = &mainImageB;
	mainImageA = OperationImage(mainImageB);
	refresWorkspace();
}

void MainEngine::redoChanges(){
	if (!imagesHistory.empty()) {
		OperationImage temp = imagesHistory.top();
		mainImageA = temp;
		imagesHistory.pop();
	}
	mainImageB = OperationImage(mainImageA);
	refresWorkspace();
}

void MainEngine::changeContrastBrightnes(float Contrast, float Brightness){
	cv::Mat new_Image = mainImageA.ContrastBrightness(Contrast, Brightness);
	cv::Mat* img = new cv::Mat;
	new_Image.copyTo(*img);
	mainImageB.setNewImage(img);
	mainImageB.setiageColorSpace(mainImageA.getColorSpace());
	displayResultsExtension();
	displayImageInLabel(mainImageB.getImagePtr(), ui->mainImageBLabel);
}

void MainEngine::addSubRGB(int R, int G, int B){
	cv::Mat new_Image = mainImageA.addSubRGB(R, G, B);
	cv::Mat* img = new cv::Mat;
	new_Image.copyTo(*img);
	mainImageB.setNewImage(img);
	mainImageB.setiageColorSpace(mainImageA.getColorSpace());
	displayResultsExtension();
	displayImageInLabel(mainImageB.getImagePtr(), ui->mainImageBLabel);
}



void MainEngine::filters(int ksize, filterOper filterType){
	cv::Mat img = mainImageA.filters(ksize, filterType);
	mainImageB.setNewImage(&img);
	mainImageB.setiageColorSpace(mainImageA.getColorSpace());
	displayResultsExtension();
	displayImageInLabel(mainImageB.getImagePtr(), ui->mainImageBLabel);
}

void MainEngine::canny(double treshold1, double treshold2){
	cv::Mat img = mainImageA.canny(treshold1, treshold2);
	mainImageB.setNewImage(&img);
	mainImageB.setiageColorSpace(grey);
	
	displayResultsExtension();
	displayImageInLabel(mainImageB.getImagePtr(), ui->mainImageBLabel);

}

void MainEngine::addNoiseGaussian(double mean, double dev){
	cv::Mat img = mainImageA.noiseGaussian(mean, dev);
	mainImageB.setNewImage(&img);
	mainImageB.setiageColorSpace(mainImageA.getColorSpace());
	displayResultsExtension();
	displayImageInLabel(mainImageB.getImagePtr(), ui->mainImageBLabel);

}

void MainEngine::unsharpMask(double sigma, double treshold, double amount){
	cv::Mat img = mainImageA.unsharpMask(sigma, treshold, amount);
	mainImageB.setNewImage(&img);
	mainImageB.setiageColorSpace(mainImageA.getColorSpace());
	displayResultsExtension();
	displayImageInLabel(mainImageB.getImagePtr(), ui->mainImageBLabel);

}

void MainEngine::morphoOperation(int kernelSize, morphoOper operrationType){
	cv::Mat img = mainImageA.morphoOperation(kernelSize, operrationType);
	mainImageB.setNewImage(&img);
	mainImageB.setiageColorSpace(mainImageA.getColorSpace());
	displayResultsExtension();
	displayImageInLabel(mainImageB.getImagePtr(), ui->mainImageBLabel);
}

