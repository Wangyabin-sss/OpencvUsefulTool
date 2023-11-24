#ifndef HISTOGRAMTOOL_H
#define HISTOGRAMTOOL_H

#include <QWidget>
#include <utils/utils.h>

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

private:
    Ui::histogramtool *ui;
    std::vector<std::string> filelist;
};

#endif // HISTOGRAMTOOL_H
