#ifndef VIDEOPROC_H
#define VIDEOPROC_H

#include <QWidget>
#include <QThread>
#include <QTime>
#include <utils/utils.h>

namespace Ui {
class videoproc;
}


class MyThread : public QThread
{
    Q_OBJECT
public:
    cv::VideoCapture *capthread;

protected:
    void run();

signals:
    void show_frame_on_label(QImage img, int flag);

private:


};



class videoproc : public QWidget
{
    Q_OBJECT

public:
    explicit videoproc(QWidget *parent = nullptr);
    ~videoproc();

    MyThread mth;

signals:
    void videopc_change2_mainwindow();

private slots:
    void on_pushButton_clicked();

    void on_openvideo_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void show_frame_label(QImage img, int flag);

    void on_screenshot_clicked();

    void on_screenshot2_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_2_clicked();

    void on_horizontalSlider_actionTriggered(int action);

    void on_horizontalSlider_sliderReleased();

    void on_horizontalSlider_sliderPressed();

private:
    Ui::videoproc *ui;
    cv::VideoCapture *cap;

};

#endif // VIDEOPROC_H
