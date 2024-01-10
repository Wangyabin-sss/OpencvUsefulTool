#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include <QWidget>
#include <QMessageBox>
#include <QTime>
#include <QMenu>
#include <QToolBar>
#include <QHBoxLayout>
#include <QLineEdit>
#include <utils/utils.h>


#define MAX_RESETIMGNUMS 10


namespace Ui {
class preprocessing;
}

class preprocessing : public QWidget
{
    Q_OBJECT

public:
    explicit preprocessing(QWidget *parent = nullptr);
    ~preprocessing();


signals:
    void preprocess_change2_mainwindow();


private slots:
    void on_comeback_clicked();

    void on_saveimage_clicked();

    void on_processimg_clicked();

    void on_chehui_clicked();

    void on_openimg_clicked();

    void show_frame_label(QImage img, int labelnum);

    void on_comboBox_currentIndexChanged(int index);

    void actionsSlot(void);

    int loop_add_one(int num);
    int loop_sub_one(int num);

private:
    Ui::preprocessing *ui;
    int imgsub;
    QRect fontsizechar;

    QHBoxLayout *funcwight;
    QLabel *funclabel1;
    QLabel *funclabel2;
    QLabel *funclabel3;
    QLabel *funclabel4;
    QLabel *funclabel5;
    QComboBox *funccombox1;
    QComboBox *funccombox2;
    QComboBox *funccombox3;
    QLineEdit *funcline1;
    QLineEdit *funcline2;
    QLineEdit *funcline3;
};

#endif // PREPROCESSING_H
