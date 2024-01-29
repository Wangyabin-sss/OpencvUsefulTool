#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <colortool/colortool.h>
#include <histogram/histogramtool.h>
#include <utils/utils.h>
#include <videoprocess/videoproc.h>
#include <preprocessing/preprocessing.h>
#include <dimensions/dimensions.h>

extern "C"
{
#include <clog/log.h>
}

using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    //主界面按键
    void on_Histogram_clicked();
    void on_colortool_clicked();
    void on_videopc_clicked();
    void on_preprocessingtool_clicked();
    void on_dimenstool_clicked();
    //子界面返回槽函数
    void colortool_change_2_mainui_slot();
    void histogramtool_change_2_mainui_slot();
    void videopctool_change_2_mainui_slot();
    void preproctool_change_2_mainui_slot();
    void dimens_change_2_mainui_slot();


private:
    Ui::MainWindow *ui;
    colortool coltool;
    histogramtool histool;
    videoproc videopc;
    preprocessing preproc;
    Dimensions dimens;
};
#endif // MAINWINDOW_H
