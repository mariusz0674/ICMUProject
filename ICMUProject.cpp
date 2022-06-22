#include "MainEngine.h"
#include "ICMUProject.h"
#include <windows.h>
//eloo
ICMUProject::ICMUProject(QWidget *parent)
    : QMainWindow(parent){
    ui.setupUi(this);
    ICMOEngine = MainEngine(&ui);
    //ICMOEngine.refresWorkspace();
}

void ICMUProject::on_qBinaryzationApplyButton_clicked(){
    ICMOEngine.binaryzeImage(ui.qBinaryzationSetType->currentIndex(), ui.qBinaryzationTrasholdSlider->value());
}

void ICMUProject::inicialActions(){
    ICMOEngine.refresWorkspace();
}

void ICMUProject::on_qBinaryzationTrasholdSlider_valueChanged(int _treshold){
    if (ui.qBinaryzationViewLiveRB->isChecked() == true) {
        ICMOEngine.binaryzeImage(ui.qBinaryzationSetType->currentIndex(), ui.qBinaryzationTrasholdSlider->value());
    }
    ui.qBinaryzationTrasholdValue->setText(QString::number(_treshold));
}


void ICMUProject::on_saveImage_clicked(){
    QString filters = "Image PNG (*.png) ;; Image JPEG (*.jpeg) ;; Image JPG (*.jpg)";
    QString path = QFileDialog::getSaveFileName(this, "Save image", QDir::homePath(), filters);
    if (path == "" || path == " ") return;
    ICMOEngine.saveImage(path.toStdString());
}

void ICMUProject::on_importImage_clicked(){
    QString filters = "Image PNG (*.png) ;; Image JPEG (*.jpeg) ;; Image JPG (*.jpg)";
    QString path = QFileDialog::getOpenFileName(this, "Pick Iage", QDir::homePath(), filters);
    if (path == "" || path == " ") return;
    ICMOEngine.upluadNewImage(path.toStdString());
}

void ICMUProject::on_appleChanges_clicked(){
    ICMOEngine.saveChanges();
}

void ICMUProject::on_redoChanges_clicked(){
    ICMOEngine.redoChanges();
}

void ICMUProject::on_extensionSelect_currentIndexChanged(int _selected) {
    if (_selected == 0) {
        ICMOEngine.changeExtansionView(histogram);
    }
    else if (_selected == 1) {
        ICMOEngine.changeExtansionView(widmo);
    }
    else {
        printf("Error");
    }
}

void ICMUProject::on_BrightnessSlider_valueChanged(int Brightness){
    QString label = "Brightness: ";
    label += QString::number(Brightness);
    ui.BrightnessLabel->setText(label);
    ICMOEngine.changeContrastBrightnes(ui.ContrastSlider->value()*0.1, Brightness);
}

void ICMUProject::on_ContrastSlider_valueChanged(int Contrast){
    QString label = "Contrast: ";
    label += QString::number(Contrast*0.1);
    ui.ContrastLabel->setText(label);
    ICMOEngine.changeContrastBrightnes(Contrast*0.1, ui.BrightnessSlider->value());
}

void ICMUProject::on_addRSlider_valueChanged(int val){
    QString label = "R: ";
    label += QString::number(val);
    ui.addRLabel->setText(label);
    ICMOEngine.addSubRGB(ui.addRSlider->value(), ui.addGSlider->value(), ui.addBSlider->value());
}

void ICMUProject::on_addGSlider_valueChanged(int val){
    QString label = "G: ";
    label += QString::number(val);
    ui.addGLabel->setText(label);
    ICMOEngine.addSubRGB(ui.addRSlider->value(), ui.addGSlider->value(), ui.addBSlider->value());
}

void ICMUProject::on_addBSlider_valueChanged(int val){
    QString label = "B: ";
    label += QString::number(val);
    ui.addBLabel->setText(label);
    ICMOEngine.addSubRGB(ui.addRSlider->value(), ui.addGSlider->value(), ui.addBSlider->value());
}

void ICMUProject::on_filtrSizeSlider_valueChanged(int val){
    QString label = "Size: ";
    label += QString::number(1+2*val);
    ui.filtrSizeLabel->setText(label);
    //ICMOEngine.addSubRGB(ui.addRSlider->value(), ui.addGSlider->value(), ui.addBSlider->value());
    if (ui.filterViewLive->isChecked()) {
        ICMUProject::on_filtrApplyButton_clicked();
    }
}

void ICMUProject::on_filtrApplyButton_clicked(){
    switch (ui.filtrTypeSelect->currentIndex()){
    case medianFiltrType:
        ICMOEngine.filters(1 + 2 * ui.filtrSizeSlider->value(), medianFiltrType);
        break;
    case avgFiltrType:
        ICMOEngine.filters(1 + 2 * ui.filtrSizeSlider->value(), avgFiltrType);
        break;
    default:
        break;
    }
}



void ICMUProject::on_cannyXSlider_valueChanged(int val){
    QString label = "Threshold: ";
    label += QString::number(10*val);
    ui.cannyXLabel->setText(label);
    if (ui.cannyViewLive->isChecked()) {
        ICMUProject::on_cannyApply_clicked();
    }
}

void ICMUProject::on_cannyApply_clicked(){
    ICMOEngine.canny(ui.cannyXSlider->value() * 10, 10);
}


void ICMUProject::on_noiseMeanSlider_valueChanged(int val){
    QString label = "Mean: ";
    label += QString::number(1 * val);
    ui.noiseMeanLabel->setText(label);
    ICMOEngine.addNoiseGaussian(ui.noiseMeanSlider->value() * 1, ui.noiseDeviationSlider->value() * 1);

}

void ICMUProject::on_noiseDeviationSlider_valueChanged(int val){
    QString label = "Deviation: ";
    label += QString::number(1 * val);
    ui.noiseDeviationLabel->setText(label);
    ICMOEngine.addNoiseGaussian(ui.noiseMeanSlider->value() * 1, ui.noiseDeviationSlider->value() * 1);

}


void ICMUProject::on_sharpTresholdSlider_valueChanged(int val){
    QString label = "Threshold: ";
    label += QString::number(0.1 * val);
    ui.sharpTresholdLabel->setText(label);
    if (ui.sharpViewLive->isChecked() == true) {
        ICMUProject::on_sharpApply_clicked();
    }
}

void ICMUProject::on_sharpAmountSlider_valueChanged(int val){
    QString label = "Amount: ";
    label += QString::number(0.1 * val);
    ui.sharpAmountLabel->setText(label);
    if (ui.sharpViewLive->isChecked() == true) {
        ICMUProject::on_sharpApply_clicked();
    }
}

void ICMUProject::on_sharpApply_clicked(){
    ICMOEngine.unsharpMask(ui.sharpSelectSigma->currentIndex(), ui.sharpTresholdSlider->value(), ui.sharpAmountSlider->value() * 0.1);
}

void ICMUProject::on_morphKernelSlider_valueChanged(int val){
    QString label = "Kernel: ";
    label += QString::number(val);
    ui.morphKernelSliderLabel->setText(label);
    if (ui.morphoViewLive->isChecked() == true) {
        ICMUProject::on_morphoApply_clicked();
    }
  //  if(ui.morphoViewLive.is)
}

void ICMUProject::on_morphoApply_clicked() {
    morphoOper operation;
    switch (ui.morphoSelectOperation->currentIndex()) {
        case 0: operation = mClose;
            break;
        case 1: operation = mOpen;
            break;
        case 2: operation = mDilate;
            break;
        case 3: operation = mErode;
            break;
        case 4: operation = mCross;
            break;
        case 5: operation = mEllipse;
            break;
        case 6: operation = mGradient;
            break;
        case 7: operation = mBlackhat;
            break;
        case 8: operation = mTophat;
            break;
        default: operation = mClose;
    }
    ICMOEngine.morphoOperation(ui.morphKernelSlider->value(), operation);
}
