#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    colorDialog = new QColorDialog(this);

    ui->customColorPreview->hide();
    ui->customColorPreview->setEnabled(false);

    ui->customColorPreviewBezier->hide();
    ui->customColorPreviewBezier->setEnabled(false);

    ui->customColorPreviewBSpline->hide();
    ui->customColorPreviewBSpline->setEnabled(false);

    ui->customColorPreviewNurbs->hide();
    ui->customColorPreviewNurbs->setEnabled(false);

    ui->customColorPreviewRational->hide();
    ui->customColorPreviewRational->setEnabled(false);

    ui->customColorPreviewCtrlPts->hide();
    ui->customColorPreviewCtrlPts->setEnabled(false);

    ui->customColorPreviewCtrlPts->hide();
    ui->customColorPreviewCtrlPts->setEnabled(false);

    ui->customColorPreviewBSplineSurface->hide();
    ui->customColorPreviewBSplineSurface->setEnabled(false);

    ui->customColorPreviewBSplineSurfaceCnet->hide();
    ui->customColorPreviewBSplineSurfaceCnet->setEnabled(false);

    ui->customColorPreviewBSplineSurfaceCpoint->hide();
    ui->customColorPreviewBSplineSurfaceCpoint->setEnabled(false);

    connect(ui->acceptButton, &QPushButton::clicked, this, &SettingsDialog::accept);
    connect(ui->cancelButton, &QPushButton::clicked, this, &SettingsDialog::reject);

    connect(ui->backgroundColors, static_cast< void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, &SettingsDialog::setBackgroundValueChanged);
    connect(ui->backgroundColors, static_cast< void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, &SettingsDialog::setBezierValueChanged);
    connect(ui->backgroundColors, static_cast< void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, &SettingsDialog::setBSplineValueChanged);
    connect(ui->backgroundColors, static_cast< void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, &SettingsDialog::setNurbsValueChanged);
    connect(ui->backgroundColors, static_cast< void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, &SettingsDialog::setRationalBezierValueChanged);
    connect(ui->backgroundColors, static_cast< void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, &SettingsDialog::setControlPointsValueChanged);
    connect(ui->backgroundColors, static_cast< void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, &SettingsDialog::setBSplineSurfaceValueChanged);
    connect(ui->backgroundColors, static_cast< void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, &SettingsDialog::setBSplineSurfaceCnetValueChanged);
    connect(ui->backgroundColors, static_cast< void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, &SettingsDialog::setBSplineSurfaceCpointsValueChanged);


    connect(ui->customColorButton,         &QPushButton::clicked, this, &SettingsDialog::showBackgroundColorDialog);
    connect(ui->customColorButtonBezier,   &QPushButton::clicked, this, &SettingsDialog::showBezierColorDialog);
    connect(ui->customColorButtonBSpline,  &QPushButton::clicked, this, &SettingsDialog::showBSplineColorDialog);
    connect(ui->customColorButtonNurbs,    &QPushButton::clicked, this, &SettingsDialog::showNurbsColorDialog);
    connect(ui->customColorButtonRational, &QPushButton::clicked, this, &SettingsDialog::showRationalBezierColorDialog);
    connect(ui->customColorButtonCtrlPts,  &QPushButton::clicked, this, &SettingsDialog::showControlPointsColorDialog);
    connect(ui->customColorButtonBSplineSurface,  &QPushButton::clicked, this, &SettingsDialog::showBSplineSurfaceColorDialog);
    connect(ui->customColorButtonBSplineSurfaceCnet,  &QPushButton::clicked, this, &SettingsDialog::showBSplineSurfaceCnetColorDialog);
    connect(ui->customColorButtonBSplineSurfaceCpoint,  &QPushButton::clicked, this, &SettingsDialog::showBSplineSurfaceCpointsColorDialog);


    connect(colorDialog, &QColorDialog::accepted, this, &SettingsDialog::setEmitBackgroundColor);
    connect(colorDialog, &QColorDialog::accepted, this, &SettingsDialog::setEmitBezierColor);
    connect(colorDialog, &QColorDialog::accepted, this, &SettingsDialog::setEmitBSplineColor);
    connect(colorDialog, &QColorDialog::accepted, this, &SettingsDialog::setEmitNurbsColor);
    connect(colorDialog, &QColorDialog::accepted, this, &SettingsDialog::setEmitRationalBezierColor);
    connect(colorDialog, &QColorDialog::accepted, this, &SettingsDialog::setEmitControlPointsColor);
    connect(colorDialog, &QColorDialog::accepted, this, &SettingsDialog::setEmitBSplineSurfaceColor);
    connect(colorDialog, &QColorDialog::accepted, this, &SettingsDialog::setEmitBSplineSurfaceCnetColor);
    connect(colorDialog, &QColorDialog::accepted, this, &SettingsDialog::setEmitBSplineSurfaceCpointsColor);

}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::showBackgroundColorDialog()
{
    buttonClicked = 0;
    colorDialog->show();
}

void SettingsDialog::showBezierColorDialog()
{
    buttonClicked = 1;
    colorDialog->show();
}

void SettingsDialog::showBSplineColorDialog()
{
    buttonClicked = 2;
    colorDialog->show();
}

void SettingsDialog::showNurbsColorDialog()
{
    buttonClicked = 3;
    colorDialog->show();
}

void SettingsDialog::showRationalBezierColorDialog()
{
    buttonClicked = 4;
    colorDialog->show();
}

void SettingsDialog::showControlPointsColorDialog()
{
    buttonClicked = 5;
    colorDialog->show();
}

void SettingsDialog::showBSplineSurfaceColorDialog()
{
    buttonClicked = 6;
    colorDialog->show();
}

void SettingsDialog::showBSplineSurfaceCnetColorDialog()
{
    buttonClicked = 7;
    colorDialog->show();
}

void SettingsDialog::showBSplineSurfaceCpointsColorDialog()
{
    buttonClicked = 8;
    colorDialog->show();
}

void SettingsDialog::keyPressEvent(QKeyEvent *e)
{
    if (e->key() & Qt::Key_Escape)
    {
        reject();
    }
    else if (e->key() & Qt::Key_Escape)
    {
        accept();
    }
}

void SettingsDialog::setEmitBackgroundColor()
{
    if (buttonClicked == 0)
    {
        backgroundColorSelected = colorDialog->currentColor();

        QString styleSheet = QString("background-color: %1").arg(backgroundColorSelected.name());

        ui->customColorPreview->setStyleSheet(styleSheet);

        ui->customColorPreview->show();

        customBackgroundColorSelected = true;
        backgroundColorValueChanged = true;

        ui->backgroundColors->setCurrentIndex(ui->backgroundColors->count()-1);
    }
}

void SettingsDialog::setEmitBezierColor()
{
    if (buttonClicked == 1)
    {
        bezieColorSelected = colorDialog->currentColor();

        QString styleSheet = QString("background-color: %1").arg(bezieColorSelected.name());

        ui->customColorPreviewBezier->setStyleSheet(styleSheet);

        ui->customColorPreviewBezier->show();

        customBezierColorSelected = true;
        bezierColorValueChanged = true;

        ui->bezierCurveColors->setCurrentIndex(ui->bezierCurveColors->count()-1);
    }
}

void SettingsDialog::setEmitBSplineColor()
{
    if (buttonClicked == 2)
    {
        bsplineColorSelected = colorDialog->currentColor();

        QString styleSheet = QString("background-color: %1").arg(bsplineColorSelected.name());

        ui->customColorPreviewBSpline->setStyleSheet(styleSheet);

        ui->customColorPreviewBSpline->show();

        customBSplineColorSelected = true;
        bsplineColorValueChanged = true;

        ui->bsplineCurveColors->setCurrentIndex(ui->bsplineCurveColors->count()-1);
    }
}

void SettingsDialog::setEmitNurbsColor()
{
    if (buttonClicked == 3)
    {
        nurbsColorSelected = colorDialog->currentColor();

        QString styleSheet = QString("background-color: %1").arg(nurbsColorSelected.name());

        ui->customColorPreviewNurbs->setStyleSheet(styleSheet);

        ui->customColorPreviewNurbs->show();

        customNurbsColorSelected = true;
        nurbsColorValueChanged = true;

        ui->nurbsCurveColors->setCurrentIndex(ui->nurbsCurveColors->count()-1);
    }
}

void SettingsDialog::setEmitRationalBezierColor()
{
    if (buttonClicked == 4)
    {
        rationalBezierColorSelected = colorDialog->currentColor();

        QString styleSheet = QString("background-color: %1").arg(rationalBezierColorSelected.name());

        ui->customColorPreviewRational->setStyleSheet(styleSheet);

        ui->customColorPreviewRational->show();

        customRationalBezierColorSelected = true;
        rationalBezierColorValueChanged = true;

        ui->rationalBezierCurveColors->setCurrentIndex(ui->rationalBezierCurveColors->count()-1);
    }
}

void SettingsDialog::setEmitControlPointsColor()
{
    if (buttonClicked == 5)
    {
        controlPointsColorSelected = colorDialog->currentColor();

        QString styleSheet = QString("background-color: %1").arg(controlPointsColorSelected.name());

        ui->customColorPreviewCtrlPts->setStyleSheet(styleSheet);

        ui->customColorPreviewCtrlPts->show();

        customControlPointsColorSelected = true;
        controlPointsColorValueChanged = true;

        ui->controlPointsColors->setCurrentIndex(ui->controlPointsColors->count()-1);
    }
}

void SettingsDialog::setEmitBSplineSurfaceColor()
{
    if (buttonClicked == 6)
    {
        bsplineSurfaceColorSelected = colorDialog->currentColor();

        QString styleSheet = QString("background-color: %1").arg(bsplineSurfaceColorSelected.name());

        ui->customColorPreviewBSplineSurface->setStyleSheet(styleSheet);

        ui->customColorPreviewBSplineSurface->show();

        customBsplineSurfaceColorSelected = true;
        bsplineSurfaceColorValueChanged = true;

        ui->bsplineSurfaceColors->setCurrentIndex(ui->bsplineSurfaceColors->count()-1);
    }
}

void SettingsDialog::setEmitBSplineSurfaceCnetColor()
{
    if (buttonClicked == 7)
    {
        bsplineSurfaceControlNetColorSelected = colorDialog->currentColor();

        QString styleSheet = QString("background-color: %1").arg(bsplineSurfaceControlNetColorSelected.name());

        ui->customColorPreviewBSplineSurfaceCnet->setStyleSheet(styleSheet);
        ui->customColorPreviewBSplineSurfaceCnet->show();

        customBsplineSurfaceCnetColorSelected = true;
        bsplineSurfaceCnetColorValueChanged = true;

        ui->bsplineSurfaceCnetColors->setCurrentIndex(ui->bsplineSurfaceCnetColors->count()-1);
    }
}

void SettingsDialog::setEmitBSplineSurfaceCpointsColor()
{
    if (buttonClicked == 8)
    {
        bsplineSurfaceControlPointsColorSelected = colorDialog->currentColor();

        QString styleSheet = QString("background-color: %1").arg(bsplineSurfaceControlPointsColorSelected.name());

        ui->customColorPreviewBSplineSurfaceCpoint->setStyleSheet(styleSheet);
        ui->customColorPreviewBSplineSurfaceCpoint->show();

        customBsplineSurfaceCpointsColorSelected = true;
        bsplineSurfaceCpointsColorValueChanged = true;

        ui->bsplineSurfaceCpointColors->setCurrentIndex(ui->bsplineSurfaceCpointColors->count()-1);
    }
}

void SettingsDialog::setBezierValueChanged()
{
    bezierColorValueChanged = true;
    if (ui->bezierCurveColors->currentIndex() == ui->bezierCurveColors->count() -1)
    {
        customBezierColorSelected = true;
    }
    else
    {
        customBezierColorSelected = false;
    }
}

void SettingsDialog::setBSplineSurfaceValueChanged()
{
    bsplineSurfaceColorValueChanged = true;
    if (ui->bsplineSurfaceColors->currentIndex() == ui->bsplineSurfaceColors->count() -1)
    {
        customBsplineSurfaceColorSelected = true;
    }
    else
    {
        customBsplineSurfaceColorSelected = false;
    }
}

void SettingsDialog::setBSplineSurfaceCnetValueChanged()
{
    bsplineSurfaceCnetColorValueChanged = true;
    if (ui->bsplineSurfaceCnetColors->currentIndex() == ui->bsplineSurfaceCnetColors->count() -1)
    {
        customBsplineSurfaceCnetColorSelected = true;
    }
    else
    {
        customBsplineSurfaceCnetColorSelected = false;
    }
}

void SettingsDialog::setBSplineValueChanged()
{
    bsplineColorValueChanged = true;
    if (ui->bsplineCurveColors->currentIndex() == ui->bsplineCurveColors->count() -1)
    {
        customBSplineColorSelected = true;
    }
    else
    {
        customBSplineColorSelected = false;
    }
}

void SettingsDialog::setNurbsValueChanged()
{
    nurbsColorValueChanged = true;
    if (ui->nurbsCurveColors->currentIndex() == ui->nurbsCurveColors->count() -1)
    {
        customNurbsColorSelected = true;
    }
    else
    {
        customNurbsColorSelected = false;
    }
}

void SettingsDialog::setRationalBezierValueChanged()
{
    rationalBezierColorValueChanged = true;
    if (ui->rationalBezierCurveColors->currentIndex() == ui->rationalBezierCurveColors->count() -1)
    {
        customRationalBezierColorSelected = true;
    }
    else
    {
        customRationalBezierColorSelected = false;
    }
}

void SettingsDialog::setBackgroundValueChanged()
{
    backgroundColorValueChanged = true;
    if (ui->backgroundColors->currentIndex() == ui->backgroundColors->count() -1)
    {
        customBackgroundColorSelected = true;
    }
    else
    {
        customBackgroundColorSelected = false;
    }
}

void SettingsDialog::setControlPointsValueChanged()
{
    controlPointsColorValueChanged = true;
    if (ui->controlPointsColors->currentIndex() == ui->controlPointsColors->count() -1)
    {
        customControlPointsColorSelected = true;
    }
    else
    {
        customControlPointsColorSelected = false;
    }
}

void SettingsDialog::setBSplineSurfaceCpointsValueChanged()
{
    bsplineSurfaceCpointsColorValueChanged = true;
    if (ui->bsplineSurfaceCpointColors->currentIndex() == ui->bsplineSurfaceCpointColors->count() -1)
    {
        customBsplineSurfaceCpointsColorSelected = true;
    }
    else
    {
        customBsplineSurfaceCpointsColorSelected = false;
    }
}

void SettingsDialog::accept()
{
    if (backgroundColorValueChanged && !customBackgroundColorSelected)
    {
        if (ui->backgroundColors->currentIndex() != (ui->backgroundColors->count() -1))
        {
            emitBackGroundColorChanged(QColor(ui->backgroundColors->currentText()));
            backgroundColorValueChanged = false;
        }
    }
    else if (customBackgroundColorSelected)
    {
        if (backgroundColorSelected.isValid())
        {
            emitBackGroundColorChanged(backgroundColorSelected);
            backgroundColorValueChanged = false;
            customBackgroundColorSelected = false;
        }
    }
    // ========================= Bezier

    if (bezierColorValueChanged && !customBezierColorSelected)
    {
        if (ui->bezierCurveColors->currentIndex() != (ui->bezierCurveColors->count() -1))
        {
            emitBezierCurveColorChanged(QColor(ui->bezierCurveColors->currentText()));
            bezierColorValueChanged = false;
        }
    }
    else if (customBezierColorSelected)
    {
        if (bezieColorSelected.isValid())
        {
            emitBezierCurveColorChanged(bezieColorSelected);
            bezierColorValueChanged = false;
            customBezierColorSelected = false;
        }
    }

    //=========================== Bspline

    if (bsplineColorValueChanged && !customBSplineColorSelected)
    {
        if (ui->bsplineCurveColors->currentIndex() != (ui->bsplineCurveColors->count() -1))
        {
            emitBSplineCurveColorChanged(QColor(ui->bsplineCurveColors->currentText()));
            bsplineColorValueChanged = false;
        }
    }
    else if (customBSplineColorSelected)
    {
        if (bsplineColorSelected.isValid())
        {
            emitBSplineCurveColorChanged(bsplineColorSelected);
            bsplineColorValueChanged = false;
            customBSplineColorSelected = false;
        }
    }

    //============================= Nurbs

    if (nurbsColorValueChanged && !customNurbsColorSelected)
    {
        if (ui->nurbsCurveColors->currentIndex() != (ui->nurbsCurveColors->count() -1))
        {
            emitNurbsCurveColorChanged(QColor(ui->nurbsCurveColors->currentText()));
            nurbsColorValueChanged = false;
        }
    }
    else if (customNurbsColorSelected)
    {
        if (nurbsColorSelected.isValid())
        {
            emitNurbsCurveColorChanged(nurbsColorSelected);
            nurbsColorValueChanged = false;
            customNurbsColorSelected = false;
        }
    }

    //============================== Rational

    if (rationalBezierColorValueChanged && !customRationalBezierColorSelected)
    {
        if (ui->rationalBezierCurveColors->currentIndex() != (ui->rationalBezierCurveColors->count() -1))
        {
            emitRationalBezierCurveColorChanged(QColor(ui->rationalBezierCurveColors->currentText()));
            rationalBezierColorValueChanged = false;
        }
    }
    else if (customRationalBezierColorSelected)
    {
        if (rationalBezierColorSelected.isValid())
        {
            emitRationalBezierCurveColorChanged(rationalBezierColorSelected);
            rationalBezierColorValueChanged = false;
            customRationalBezierColorSelected = false;
        }
    }

    //============================== Control points

    if (controlPointsColorValueChanged && !customControlPointsColorSelected)
    {
        if (ui->controlPointsColors->currentIndex() != (ui->controlPointsColors->count() -1))
        {
            emitControlPointsColorChanged(QColor(ui->controlPointsColors->currentText()));
            controlPointsColorValueChanged = false;
        }
    }
    else if (customControlPointsColorSelected)
    {
        if (controlPointsColorSelected.isValid())
        {
            emitControlPointsColorChanged(controlPointsColorSelected);
            controlPointsColorValueChanged = false;
            customControlPointsColorSelected = false;
        }
    }

    //============================== BSpline surface

    if (bsplineSurfaceColorValueChanged && !customBsplineSurfaceColorSelected)
    {
        if (ui->bsplineSurfaceColors->currentIndex() != (ui->bsplineSurfaceColors->count() -1))
        {
            emitBSplineSurfaceColorChanged(QColor(ui->bsplineSurfaceColors->currentText()));
            bsplineSurfaceColorValueChanged = false;
        }
    }
    else if (customBsplineSurfaceColorSelected)
    {
        if (bsplineSurfaceColorSelected.isValid())
        {
            emitBSplineSurfaceColorChanged(bsplineSurfaceColorSelected);
            bsplineSurfaceColorValueChanged = false;
            customBsplineSurfaceColorSelected = false;
        }
    }

    //============================== BSpline surface Cnets

    if (bsplineSurfaceCnetColorValueChanged && !customBsplineSurfaceCnetColorSelected)
    {
        if (ui->bsplineSurfaceCnetColors->currentIndex() != (ui->bsplineSurfaceCnetColors->count() -1))
        {
            emitBSplineSurfaceCnetColorChanged(QColor(ui->bsplineSurfaceCnetColors->currentText()));
            bsplineSurfaceCnetColorValueChanged = false;
        }
    }
    else if (customBsplineSurfaceCnetColorSelected)
    {
        if (bsplineSurfaceControlNetColorSelected.isValid())
        {
            emitBSplineSurfaceCnetColorChanged(bsplineSurfaceControlNetColorSelected);
            bsplineSurfaceCnetColorValueChanged = false;
            customBsplineSurfaceCnetColorSelected = false;
        }
    }

    //============================== BSpline surface Cpoints

    if (bsplineSurfaceCpointsColorValueChanged && !customBsplineSurfaceCpointsColorSelected)
    {
        if (ui->bsplineSurfaceCpointColors->currentIndex() != (ui->bsplineSurfaceCpointColors->count() -1))
        {
            emitBSplineSurfaceCpointsColorChanged(QColor(ui->bsplineSurfaceCpointColors->currentText()));
            bsplineSurfaceCpointsColorValueChanged = false;
        }
    }
    else if (customBsplineSurfaceCpointsColorSelected)
    {
        if (bsplineSurfaceControlPointsColorSelected.isValid())
        {
            emitBSplineSurfaceCpointsColorChanged(bsplineSurfaceControlPointsColorSelected);
            bsplineSurfaceCpointsColorValueChanged = false;
            customBsplineSurfaceCpointsColorSelected = false;
        }
    }

    QDialog::accept();
}

void SettingsDialog::reject()
{
    ui->customColorPreview->hide();
    QDialog::reject();
}
