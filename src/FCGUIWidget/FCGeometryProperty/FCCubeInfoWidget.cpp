#include "FCCubeInfoWidget.h"
#include "ui_FCCubeInfoWidget.h"
#include <FCGeometryInterface/FCAbsGeoModelBox.h>
#include <FCGeometryInterface/FCGeoCommandList.h>
#include <FCGeometryInterface/FCAbsGeoCommand.h>
#include <FCGeometryEntity/FCGeometryEntityBuilder.h>
#include <FCGeometryEntity/FCGeometryEntityModel.h>
#include <FCData/FCDataRepo.h>
#include <QLineEdit>
#include <QPushButton>
#include <QDebug>

FCCubeInfoWidget::FCCubeInfoWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FCCubeInfoWidget)
{
    ui->setupUi(this);
    connect(ui->pushButton_build, &QPushButton::clicked, this, &FCCubeInfoWidget::onBuildClicked);
    connect(ui->pushButton_buildAll, &QPushButton::clicked, this, &FCCubeInfoWidget::onBuildAllClicked);
}

FCCubeInfoWidget::~FCCubeInfoWidget()
{
    delete ui;
}

void FCCubeInfoWidget::setBoxCommand(FC::FCGeoModelBox* boxCmd)
{
    m_boxCmd = boxCmd;
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

void FCCubeInfoWidget::onBuildClicked()
{
    if (!m_boxCmd) {
        qWarning() << "FCCubeInfoWidget: no box command to build";
        return;
    }
    bool ok = false;
    double p[3] = {
        ui->lineEdit_pointX->text().toDouble(&ok),
        ui->lineEdit_pointY->text().toDouble(&ok),
        ui->lineEdit_pointZ->text().toDouble(&ok)
    };
    double len[3] = {
        ui->lineEdit_length1->text().toDouble(&ok),
        ui->lineEdit_length2->text().toDouble(&ok),
        ui->lineEdit_length3->text().toDouble(&ok)
    };
    m_boxCmd->setPoint1(p);
    m_boxCmd->setLength(len);
    if (!m_boxCmd->update()) {
        qWarning() << "FCCubeInfoWidget: boxCmd->update() failed";
        return;
    }
    rebuildGeometryEntityModel();
    emit geometryBuilt(m_boxCmd);
}

void FCCubeInfoWidget::onBuildAllClicked()
{
    FC::FCGeoCommandList* geoList = FCDATAREPO->getFirstDataByType<FC::FCGeoCommandList>();
    if (!geoList) return;
    QList<FC::FCAbsGeoCommand*> rootCmds = geoList->getRootCommandList();
    // COMSOL 方式：只构建几何序列中“当前节点”及之前的命令（当前 = 本属性页对应的 m_boxCmd）
    int currentIndex = -1;
    for (int i = 0; i < rootCmds.size(); ++i) {
        if (rootCmds.at(i) == m_boxCmd) {
            currentIndex = i;
            break;
        }
    }
    if (currentIndex < 0)
        currentIndex = rootCmds.size() - 1;
    QList<FC::FCAbsGeoCommand*> builtCmds;
    for (int i = 0; i <= currentIndex && i < rootCmds.size(); ++i) {
        FC::FCAbsGeoCommand* cmd = rootCmds.at(i);
        if (cmd) {
            cmd->update();
            builtCmds.append(cmd);
        }
    }
    rebuildGeometryEntityModel();
    emit geometrySequenceBuilt(builtCmds);
}

void FCCubeInfoWidget::rebuildGeometryEntityModel()
{
    FC::FCGeoCommandList* geoList = FCDATAREPO->getFirstDataByType<FC::FCGeoCommandList>();
    FC::FCGeometryEntityModel* entityModel = FCDATAREPO->getFirstDataByType<FC::FCGeometryEntityModel>();
    if (!geoList || !entityModel) return;
    FC::FCGeometryEntityBuilder builder;
    if (!builder.rebuild(geoList, entityModel)) {
        qWarning() << "FCCubeInfoWidget: Failed to rebuild FCGeometryEntityModel";
    }
}
