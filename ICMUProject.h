#pragma once

#include <QtWidgets/QMainWindow>
#include <QLocale>
#include <qstring.h>
#include "ui_ICMUProject.h"
#include "MainEngine.h"
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qdir>
#include <chrono>
#include <thread>

class ICMUProject : public QMainWindow
{
    Q_OBJECT

public:
    ICMUProject(QWidget *parent = Q_NULLPTR);
    void inicialActions();
private:
    Ui::ICMUProjectClass ui;
    MainEngine ICMOEngine;
private slots:
    void on_qBinaryzationApplyButton_clicked();
    void on_qBinaryzationTrasholdSlider_valueChanged(int _trashold);
    void on_hujemuje_clicked();
    void on_saveImage_clicked();
    void on_importImage_clicked();
    void on_appleChanges_clicked();
    void on_redoChanges_clicked();
    void on_extensionSelect_currentIndexChanged(int _selected);

    void on_BrightnessSlider_valueChanged(int Brightness);
    void on_ContrastSlider_valueChanged(int Contrast);

    void on_addRSlider_valueChanged(int);
    void on_addGSlider_valueChanged(int);
    void on_addBSlider_valueChanged(int);

    void on_filtrSizeSlider_valueChanged(int);
    void on_filterMedianApply_clicked();

    void on_cannyXSlider_valueChanged(int);
    void on_cannyYSlider_valueChanged(int);

    void on_noiseMeanSlider_valueChanged(int);
    void on_noiseDeviationSlider_valueChanged(int);

    void on_sharpTresholdSlider_valueChanged(int);
    void on_sharpAmountSlider_valueChanged(int);
    void on_sharpApply_clicked();

    void on_morphKernelSlider_valueChanged(int);
    void on_morphoApply_clicked();

};
