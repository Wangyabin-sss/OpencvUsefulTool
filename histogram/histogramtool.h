#ifndef HISTOGRAMTOOL_H
#define HISTOGRAMTOOL_H

#include <QWidget>
#include <utils/utils.h>
#include <QVBoxLayout>
#include <QtCharts>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class histogramtool;
}

class histogramtool : public QWidget
{
    Q_OBJECT

public:
    explicit histogramtool(QWidget *parent = nullptr);
    ~histogramtool();

signals:
    void histogram_change2_mainwindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void show_frame_label(QImage img, int labelnum);

    void on_comboBox_2_currentIndexChanged(int index);

    void update_qcharts_data(QLineSeries *ser);

private:
    Ui::histogramtool *ui;
    std::vector<std::string> filelist;
    QChart *qchartdata;
    QChartView *chartView;
    QLineSeries *series;
};

#endif // HISTOGRAMTOOL_H
