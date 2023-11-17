#ifndef VIDEOPROC_H
#define VIDEOPROC_H

#include <QWidget>
#include <utils/utils.h>

namespace Ui {
class videoproc;
}

class videoproc : public QWidget
{
    Q_OBJECT

public:
    explicit videoproc(QWidget *parent = nullptr);
    ~videoproc();


signals:
    void videopc_change2_mainwindow();

private slots:
    void on_pushButton_clicked();

    void on_openvideo_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void show_frame_label(QImage img);

    void on_screenshot_clicked();

    void on_screenshot2_clicked();

    void on_horizontalSlider_actionTriggered(int action);

private:
    Ui::videoproc *ui;
    cv::VideoCapture cap;
};

#endif // VIDEOPROC_H
