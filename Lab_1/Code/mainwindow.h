#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QMainWindow>
#include <QMouseEvent>
#include <qspinbox.h>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void on_choseColor1_currentIndexChanged(int index);

    void on_choseColor2_currentIndexChanged(int index);

    void on_choseColor3_currentIndexChanged(int index);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void Show();

    void clear();

    void drawSB(QComboBox *CB, int index);

    void darwPos(QComboBox *CB, QSpinBox *SB1, QSpinBox *SB2, QSpinBox *SB3);

    void on_RGB_valueChanged();

    void on_CMYK_valueChanged();

    void on_HLS_valueChanged();

    void on_HSV_valueChanged();

    void on_XYZ_valueChanged();

    void on_LAB_valueChanged();

    void on_vopyButton_clicked();

    void on_SelectColorBotton_clicked();

private:
    Ui::MainWindow *ui;
    QColor color_ = QColor(0,0,0);
    QSpinBox *red_;
    QSpinBox *green_;
    QSpinBox *blue_;

    QSpinBox *cian_;
    QSpinBox *magenta_;
    QSpinBox *yellow_;
    QSpinBox *key_;

    QSpinBox *HLS1_;
    QSpinBox *HLS2_;
    QSpinBox *HLS3_;

    QSpinBox *HSV1_;
    QSpinBox *HSV2_;
    QSpinBox *HSV3_;

    QDoubleSpinBox *X_;
    QDoubleSpinBox *Y_;
    QDoubleSpinBox *Z_;

    QDoubleSpinBox *L_;
    QDoubleSpinBox *A_;
    QDoubleSpinBox *B_;

    uint8_t Activied_[3];
    uint8_t curent_ = 6;
    QClipboard *clipboard_;
};

#endif // MAINWINDOW_H
