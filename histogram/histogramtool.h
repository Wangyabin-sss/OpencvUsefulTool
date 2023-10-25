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

private:
    Ui::histogramtool *ui;
};

#endif // HISTOGRAMTOOL_H
