#ifndef MORPHO_H
#define MORPHO_H

#include <QWidget>
#include <utils/utils.h>

namespace Ui {
class morpho;
}

class morpho : public QWidget
{
    Q_OBJECT

public:
    explicit morpho(QWidget *parent = nullptr);
    ~morpho();

signals:
    void morpho_change2_mainwindow();

private slots:
    void on_comeback_clicked();

    void on_openimage_clicked();

private:
    Ui::morpho *ui;
};

#endif // MORPHO_H
