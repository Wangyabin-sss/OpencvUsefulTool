#ifndef CROPTOOL_H
#define CROPTOOL_H

#include <QWidget>

namespace Ui {
class croptool;
}

class croptool : public QWidget
{
    Q_OBJECT

public:
    explicit croptool(QWidget *parent = nullptr);
    ~croptool();


signals:
    void croptool_change2_mainwindow();


private slots:
    void on_comback_clicked();

private:
    Ui::croptool *ui;
};

#endif // CROPTOOL_H
