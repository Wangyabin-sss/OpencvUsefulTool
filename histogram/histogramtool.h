#ifndef HISTOGRAMTOOL_H
#define HISTOGRAMTOOL_H

#include <QWidget>

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

private:
    Ui::histogramtool *ui;
};

#endif // HISTOGRAMTOOL_H
