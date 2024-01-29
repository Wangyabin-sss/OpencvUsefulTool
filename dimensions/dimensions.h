#ifndef DIMENSIONS_H
#define DIMENSIONS_H

#include <QWidget>
#include <QMouseEvent>
#include <utils/utils.h>

namespace Ui {
class Dimensions;
}

class Dimensions : public QWidget
{
    Q_OBJECT


signals:
    void dimens_change2_mainwindow();

public:
    explicit Dimensions(QWidget *parent = nullptr);
    ~Dimensions();

private slots:
    void on_comeback_clicked();

    void on_openimage_clicked();

    void show_img_label();

    void on_addpoint_clicked();

    void on_xpointval_textChanged(const QString &arg1);

    void on_ypointval_textChanged(const QString &arg1);

    void wheelEvent(QWheelEvent *event);

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::Dimensions *ui;
    std::vector<cv::Point> points;
};

#endif // DIMENSIONS_H
