
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QSpinBox>

#include <QLayout>
#include <QPalette>
#include <QClipboard>
#include <QColorDialog>
#include <QImage>

double funcRtoX(double num) {
    if (num >= 0.04045) {
        return pow((double(num) + 0.055) / 1.055, 2.4);
    }
    return double(num) / 12.92;
}

void RGBtoXYZ(int r, int g, int b, double &X, double &Y, double &Z) {
    double Rn = funcRtoX(double(r) / 255.0);
    double Gn = funcRtoX(double(g) / 255.0);
    double Bn = funcRtoX(double(b) / 255.0);

    X = 0.412453 * Rn + 0.357580 * Gn + 0.180423 * Bn;
    Y = 0.212671 * Rn + 0.715160 * Gn + 0.072169 * Bn;
    Z = 0.019334 * Rn + 0.119193 * Gn + 0.950227 * Bn;

    X *= 100.0;
    Y *= 100.0;
    Z *= 100.0;
}

double funcXtoR(double num) {
    if (abs(num)<0.0031308) {
            return 12.92 * num;
    }
    return 1.055 * pow(num, 0.41666) - 0.055;
}

void XYZtoRGB(double x, double y, double z, int &r, int &g, int &b) {

    double Rn = 3.2404542 * x - 1.5371385 * y  - 0.4985314 * z;
    double Gn = -0.9692660 * x + 1.8760108 * y  + 0.0415560 * z;
    double Bn = 0.0556434 * x - 0.2040259 * y  + 1.0572252 * z;

    Rn/=100.0;
    Gn/=100.0;
    Bn/=100.0;

    r = round(funcXtoR(Rn) * 255);
    g = round(funcXtoR(Gn) * 255);
    b = round(funcXtoR(Bn) * 255);
}

double funcXtoL(double num) {
    if (num >= 0.008856) {
            return pow(num, 1.0/3.0);
        }
        return 7.787 * num + 16.0 /(double) 116.0;
}

void XYZtoLAB(double x, double y, double z, double &l, double &a, double &b) {
    x /= 95.047;
    y /= 100.0;
    z /= 108.883;
    l = 116 * funcXtoL(y) - 16;
    a = 500 * (funcXtoL(x) - funcXtoL(y));
    b = 200 * (funcXtoL(y) - funcXtoL(z));
}

double funcLtoX(double num) {
    if (pow(num,3) >= 0.008856) {
        return pow(num, 3);
    }
    return (num - 16 / 116) / 7.878;
}

void LABtoRGB(double l, double a, double b, int &red, int &green, int &blue) {
    double x = funcLtoX(a / 500.0 + (l + 16.0) / 116.0) * 95.047;
    double y = funcLtoX((l + 16.0) / 116.0) * 100.0;
    double z = funcLtoX((l + 16.0) / 116.0 - b / 200) * 108.883;

    XYZtoRGB(x, y, z, red, green, blue);
}

std::string Func(int num) {
    switch (num) {
        case 10:
            return "A";
        case 11:
            return "B";
        case 12:
            return "C";
        case 13:
            return "D";
        case 14:
            return "E";
        case 15:
            return "F";
        case 0:
            return "0";
        default:
            return std::to_string(num);
    }
}

std::string Convert(int num) {
    if (num > 255) {
        return "FF";
    }

    int n1 = (num / 16) % 16;
    int n2 = num % 16;
    std::string ans = Func(n1) + Func(n2);
    return ans;
}

std::string GetHexRepresentation(int red, int green, int blue) {
    return Convert(red) + Convert(green) + Convert(blue);
}

void Resize(Ui::MainWindow *ui, MainWindow *mw) {
    ui->chosencolor->setGeometry(QRect(mw->width() - 200, 50,
                                       150, 150));
    ui->infLabel->setGeometry(mw->width() - 200, 30,
                              100, 20);
    ui->chosencolor->setText("");
}

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    setMinimumSize(600, 460);
    setMaximumSize(600, 460);
    QPixmap pic("pallete.png");
    ui->pallete->setPixmap(pic);
    QFrame *frame = new QFrame(this);
    frame->setGeometry(20, 30, 340, 180);
    frame->setCursor(Qt::CrossCursor);
    this->layout()->addWidget(frame);

    red_ = new QSpinBox;
    green_ = new QSpinBox;
    blue_ = new QSpinBox;
    connect(red_, SIGNAL(valueChanged(int)), SLOT(on_RGB_valueChanged()));
    connect(green_, SIGNAL(valueChanged(int)), SLOT(on_RGB_valueChanged()));
    connect(blue_, SIGNAL(valueChanged(int)), SLOT(on_RGB_valueChanged()));
    red_->setRange(0, 255);
    green_->setRange(0, 255);
    blue_->setRange(0, 255);

    cian_ = new QSpinBox;
    magenta_ = new QSpinBox;
    yellow_ = new QSpinBox;
    key_ = new QSpinBox;
    connect(cian_, SIGNAL(valueChanged(int)), SLOT(on_CMYK_valueChanged()));
    connect(magenta_, SIGNAL(valueChanged(int)), SLOT(on_CMYK_valueChanged()));
    connect(yellow_, SIGNAL(valueChanged(int)), SLOT(on_CMYK_valueChanged()));
    connect(key_, SIGNAL(valueChanged(int)), SLOT(on_CMYK_valueChanged()));
    cian_->setRange(0, 100);
    magenta_->setRange(0, 100);
    yellow_->setRange(0, 100);
    key_->setRange(0, 100);

    HLS1_ = new QSpinBox;
    HLS2_ = new QSpinBox;
    HLS3_ = new QSpinBox;
    connect(HLS1_, SIGNAL(valueChanged(int)), SLOT(on_HLS_valueChanged()));
    connect(HLS2_, SIGNAL(valueChanged(int)), SLOT(on_HLS_valueChanged()));
    connect(HLS3_, SIGNAL(valueChanged(int)), SLOT(on_HLS_valueChanged()));
    HLS1_->setRange(0, 255);
    HLS2_->setRange(0, 255);
    HLS3_->setRange(0, 359);

    HSV1_ = new QSpinBox;
    HSV2_ = new QSpinBox;
    HSV3_ = new QSpinBox;
    connect(HSV1_, SIGNAL(valueChanged(int)), SLOT(on_HSV_valueChanged()));
    connect(HSV2_, SIGNAL(valueChanged(int)), SLOT(on_HSV_valueChanged()));
    connect(HSV3_, SIGNAL(valueChanged(int)), SLOT(on_HSV_valueChanged()));
    HSV1_->setRange(0, 255);
    HSV2_->setRange(0, 255);
    HSV3_->setRange(0, 359);

    X_ = new QDoubleSpinBox;
    Y_ = new QDoubleSpinBox;
    Z_ = new QDoubleSpinBox;
    connect(X_, SIGNAL(valueChanged(double)), SLOT(on_XYZ_valueChanged()));
    connect(Y_, SIGNAL(valueChanged(double)), SLOT(on_XYZ_valueChanged()));
    connect(Z_, SIGNAL(valueChanged(double)), SLOT(on_XYZ_valueChanged()));
    X_->setRange(0, 95.047);
    Y_->setRange(0, 100.0);
    Z_->setRange(0, 108.883);
    X_->setSingleStep(0.01);
    Y_->setSingleStep(0.01);
    Z_->setSingleStep(0.01);

    L_ = new QDoubleSpinBox;
    A_ = new QDoubleSpinBox;
    B_ = new QDoubleSpinBox;
    connect(L_, SIGNAL(valueChanged(double)), SLOT(on_LAB_valueChanged()));
    connect(A_, SIGNAL(valueChanged(double)), SLOT(on_LAB_valueChanged()));
    connect(B_, SIGNAL(valueChanged(double)), SLOT(on_LAB_valueChanged()));
    L_->setRange(0, 100);
    A_->setRange(-128, 128);
    B_->setRange(-128, 128);
    L_->setSingleStep(0.01);
    A_->setSingleStep(0.01);
    B_->setSingleStep(0.01);

    ui->choseColor1->setCurrentIndex(6);
    ui->choseColor2->setCurrentIndex(6);
    ui->choseColor3->setCurrentIndex(6);
    Show();
}

void MainWindow::Show() {

    ui->statusbar->showMessage("  #" + QString::fromStdString(GetHexRepresentation(int(color_.red()),
                                                                                   int(color_.green()),
                                                                                   int(color_.blue()))));
    QPalette palette;
    palette.setColor(QPalette::Window, color_);
    ui->chosencolor->setAutoFillBackground(true);
    ui->chosencolor->setPalette(palette);
    Resize(ui, this);


    red_->blockSignals(true);
    green_->blockSignals(true);
    blue_->blockSignals(true);
    cian_->blockSignals(true);
    magenta_->blockSignals(true);
    yellow_->blockSignals(true);
    key_->blockSignals(true);
    HLS1_->blockSignals(true);
    HLS2_->blockSignals(true);
    HLS3_->blockSignals(true);
    HSV1_->blockSignals(true);
    HSV2_->blockSignals(true);
    HSV3_->blockSignals(true);
    X_->blockSignals(true);
    Y_->blockSignals(true);
    Z_->blockSignals(true);
    L_->blockSignals(true);
    A_->blockSignals(true);
    B_->blockSignals(true);

    if (curent_ != 0) {
        red_->setValue(color_.red());
        green_->setValue(color_.green());
        blue_->setValue(color_.blue());
    }

    if (curent_ != 1) {
        cian_->setValue(int(color_.cyanF() * 100));
        magenta_->setValue(int(color_.magentaF() * 100));
        yellow_->setValue(int(color_.yellowF() * 100));
        key_->setValue(int(color_.blackF() * 100));
    }

    if (curent_ != 2) {
        HLS1_->setValue(color_.hslHue());
        HLS2_->setValue(color_.lightness());
        HLS3_->setValue(color_.hslSaturation());
    }

    if (curent_ != 3) {
        HSV1_->setValue(color_.hsvHue());
        HSV2_->setValue(color_.value());
        HSV3_->setValue(color_.hsvSaturation());
    }

    if (curent_ != 4) {
        double x, y, z;
        RGBtoXYZ(color_.red(), color_.green(), color_.blue(), x, y, z);
        X_->setValue(x);
        Y_->setValue(y);
        Z_->setValue(z);
    }
    if (curent_ != 5) {
        double l, a, b;
        XYZtoLAB(X_->value(), Y_->value(), Z_->value(), l, a, b);
        L_->setValue(l);
        A_->setValue(a);
        B_->setValue(b);
    }

    red_->blockSignals(false);
    green_->blockSignals(false);
    blue_->blockSignals(false);
    cian_->blockSignals(false);
    magenta_->blockSignals(false);
    yellow_->blockSignals(false);
    key_->blockSignals(false);
    HLS1_->blockSignals(false);
    HLS2_->blockSignals(false);
    HLS3_->blockSignals(false);
    HSV1_->blockSignals(false);
    HSV2_->blockSignals(false);
    HSV3_->blockSignals(false);
    X_->blockSignals(false);
    Y_->blockSignals(false);
    Z_->blockSignals(false);
    L_->blockSignals(false);
    A_->blockSignals(false);
    B_->blockSignals(false);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::clear() {
    if (Activied_[0] != 0 and Activied_[1] != 0 and Activied_[2] != 0) {
        red_->setGeometry(0, 0, 0, 0);
        green_->setGeometry(0, 0, 0, 0);
        blue_->setGeometry(0, 0, 0, 0);
    }
    if (Activied_[0] != 1 and Activied_[1] != 1 and Activied_[2] != 1) {
        cian_->setGeometry(0, 0, 0, 0);
        magenta_->setGeometry(0, 0, 0, 0);
        yellow_->setGeometry(0, 0, 0, 0);
        key_->setGeometry(0, 0, 0, 0);
    }
    if (Activied_[0] != 2 and Activied_[1] != 2 and Activied_[2] != 2) {
        HLS1_->setGeometry(0, 0, 0, 0);
        HLS2_->setGeometry(0, 0, 0, 0);
        HLS3_->setGeometry(0, 0, 0, 0);
    }
    if (Activied_[0] != 3 and Activied_[1] != 3 and Activied_[2] != 3) {
        HSV1_->setGeometry(0, 0, 0, 0);
        HSV2_->setGeometry(0, 0, 0, 0);
        HSV3_->setGeometry(0, 0, 0, 0);
    }
    if (Activied_[0] != 4 and Activied_[1] != 4 and Activied_[2] != 4) {
        X_->setGeometry(0, 0, 0, 0);
        Y_->setGeometry(0, 0, 0, 0);
        Z_->setGeometry(0, 0, 0, 0);
    }
    if (Activied_[0] != 5 and Activied_[1] != 5 and Activied_[2] != 5) {
        L_->setGeometry(0, 0, 0, 0);
        A_->setGeometry(0, 0, 0, 0);
        B_->setGeometry(0, 0, 0, 0);
    }
}

void MainWindow::darwPos(QComboBox *CB, QSpinBox *SB1, QSpinBox *SB2, QSpinBox *SB3) {
    SB1->setGeometry(CB->pos().x(),
                      CB->pos().y() + 30,
                      CB->size().rwidth(),
                      CB->size().rheight() - 5);
    SB2->setGeometry(CB->pos().x(),
                        CB->pos().y() + 55,
                        CB->size().rwidth(),
                        CB->size().rheight() - 5);
    SB3->setGeometry(CB->pos().x(),
                       CB->pos().y() + 80,
                       CB->size().rwidth(),
                       CB->size().rheight() - 5);

    this->layout()->addWidget(SB1);
    this->layout()->addWidget(SB2);
    this->layout()->addWidget(SB3);
}

void MainWindow::drawSB(QComboBox *CB, int index) {
    if (index == 0) {
        darwPos(CB,red_, green_, blue_);
    }
    if (index == 1) {
        darwPos(CB,cian_, magenta_, yellow_);
        key_->setGeometry(CB->pos().x(),
                          CB->pos().y() + 105,
                          CB->size().rwidth(),
                          CB->size().rheight() - 5);
        this->layout()->addWidget(key_);
    }
    if (index == 2) {
        darwPos(CB,HLS1_, HLS2_, HLS3_);
    }
    if (index == 3) {
        darwPos(CB,HSV1_, HSV2_, HSV3_);
    }
    if (index == 4) {
        X_->setGeometry(CB->pos().x(),
                        CB->pos().y() + 30,
                        CB->size().rwidth(),
                        CB->size().rheight() - 5);
        Y_->setGeometry(CB->pos().x(),
                        CB->pos().y() + 55,
                        CB->size().rwidth(),
                        CB->size().rheight() - 5);
        Z_->setGeometry(CB->pos().x(),
                        CB->pos().y() + 80,
                        CB->size().rwidth(),
                        CB->size().rheight() - 5);

        this->layout()->addWidget(X_);
        this->layout()->addWidget(Y_);
        this->layout()->addWidget(Z_);
    }
    if (index == 5) {
        L_->setGeometry(CB->pos().x(),
                        CB->pos().y() + 30,
                        CB->size().rwidth(),
                        CB->size().rheight() - 5);
        A_->setGeometry(CB->pos().x(),
                        CB->pos().y() + 55,
                        CB->size().rwidth(),
                        CB->size().rheight() - 5);
        B_->setGeometry(CB->pos().x(),
                        CB->pos().y() + 80,
                        CB->size().rwidth(),
                        CB->size().rheight() - 5);

        this->layout()->addWidget(L_);
        this->layout()->addWidget(A_);
        this->layout()->addWidget(B_);
    }
}

void MainWindow::on_choseColor1_currentIndexChanged(int index) {
    Activied_[0] = index;
    clear();
    if (Activied_[1] == index) {
        ui->choseColor2->setCurrentIndex(6);
    }
    if (Activied_[2] == index) {
        ui->choseColor3->setCurrentIndex(6);
    }

    drawSB(ui->choseColor1, index);
}


void MainWindow::on_choseColor2_currentIndexChanged(int index) {
    Show();
    Activied_[1] = index;
    clear();
    if (Activied_[0] == index) {
        ui->choseColor1->setCurrentIndex(6);
    }
    if (Activied_[2] == index) {
        ui->choseColor3->setCurrentIndex(6);
    }

    drawSB(ui->choseColor2, index);
}

void MainWindow::on_choseColor3_currentIndexChanged(int index) {
    Show();
    Activied_[2] = index;
    clear();
    if (Activied_[0] == index) {
        ui->choseColor1->setCurrentIndex(6);
    }
    if (Activied_[1] == index) {
        ui->choseColor2->setCurrentIndex(6);
    }

    drawSB(ui->choseColor3, index);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->pos().x() > 20 and event->pos().x() < 360 and event->pos().y() > 30 and event->pos().y() < 210) {
        curent_ = 6;
        color_ = ui->pallete->pixmap().toImage().pixel(QPoint(event->pos().x() - 20, event->pos().y() - 30));
        Show();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->pos().x() > 20 and event->pos().x() < 360 and event->pos().y() > 30 and event->pos().y() < 210) {
        curent_ = 6;
        color_ = ui->pallete->pixmap().toImage().pixel(QPoint(event->pos().x() - 20, event->pos().y() - 30));
        Show();
    }
}

void MainWindow::on_RGB_valueChanged() {
    curent_ = 0;
    color_.setRgb(red_->value(), green_->value(), blue_->value());
    Show();
}


void MainWindow::on_CMYK_valueChanged() {
    curent_ = 1;
    double c, m, y, k;
    c = (double)cian_->value() / 100;
    m = (double)magenta_->value() / 100;
    y = (double)yellow_->value() / 100;
    k = (double)key_->value() / 100;
    color_.setCmykF(c, m, y, k);
    Show();
}

void MainWindow::on_HLS_valueChanged() {
    curent_ = 2;
    color_.setHsl(HLS1_->value(), HLS3_->value(), HLS2_->value());
    Show();
}

void MainWindow::on_HSV_valueChanged() {
    curent_ = 3;
    color_.setHsv(HSV1_->value(), HSV3_->value(), HSV2_->value());
    Show();
}

void MainWindow::on_XYZ_valueChanged() {
    curent_ = 4;
    int r, g ,b;
    XYZtoRGB(X_->value(), Y_->value(), Z_->value(), r, g, b);
    color_.setRgb(r, g, b);
    Show();
}

void MainWindow::on_LAB_valueChanged() {
    curent_ = 5;
    int r, g ,b;
    LABtoRGB(L_->value(), A_->value(), B_->value(), r, g, b);
    color_.setRgb(r, g, b);
    Show();
}


void MainWindow::on_vopyButton_clicked() {
    clipboard_->setText("#" + QString::fromStdString(
            GetHexRepresentation(int(color_.red()),
                                 int(color_.green()),
                                 int(color_.blue()))));
}

void MainWindow::on_SelectColorBotton_clicked() {
    color_ = QColorDialog().getColor();
    curent_ = 6;
    Show();
}

