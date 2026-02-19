#include "FCCubeInfoWidget.h"
#include "ui_FCCubeInfoWidget.h"
#include <FCGeometryInterface/FCAbsGeoModelBox.h>
#include <QLineEdit>

FCCubeInfoWidget::FCCubeInfoWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FCCubeInfoWidget)
{
    ui->setupUi(this);
}

FCCubeInfoWidget::~FCCubeInfoWidget()
{
    delete ui;
}

void FCCubeInfoWidget::setBoxCommand(FC::FCGeoModelBox* boxCmd)
{
    if (!boxCmd) return;
    double p[3], len[3];
    boxCmd->getPoint1(p);
    boxCmd->getLength(len);
    ui->lineEdit_pointX->setText(QString::number(p[0]));
    ui->lineEdit_pointY->setText(QString::number(p[1]));
    ui->lineEdit_pointZ->setText(QString::number(p[2]));
    ui->lineEdit_length1->setText(QString::number(len[0]));
    ui->lineEdit_length2->setText(QString::number(len[1]));
    ui->lineEdit_length3->setText(QString::number(len[2]));
}
