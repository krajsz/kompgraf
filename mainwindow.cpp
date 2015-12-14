#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    settingsDialog =  new SettingsDialog();
    shaderSourceEditDialog = new ShaderSourceEditDialog();
    ui->setupUi(this);
    ui->controlPointCountLcdNumber->setPalette(Qt::red);

    ui->bsplineSurfaceCheckBox->setChecked(false);
    ui->bsplineSurfaceShadedCheckBox->setChecked(false);

    ui->rotateXslider->setEnabled(false);
    ui->rotateZslider->setEnabled(false);
    ui->densityOfBsplinesSlider->setEnabled(false);

    canvas = new Canvas(ui->canvas);

    connect(ui->settingsButton, &QPushButton::clicked, settingsDialog, &SettingsDialog::show);
    connect(ui->editShaderButton, &QPushButton::clicked, shaderSourceEditDialog, &ShaderSourceEditDialog::show);
    connect(shaderSourceEditDialog, &ShaderSourceEditDialog::vertShaderSourceSaved, canvas, &Canvas::updateVertexShaderSource);
    connect(shaderSourceEditDialog, &ShaderSourceEditDialog::fragShaderSourceSaved, canvas, &Canvas::updateFragmentShaderSource);
    connect(shaderSourceEditDialog, &ShaderSourceEditDialog::geomShaderSourceSaved, canvas, &Canvas::updateGeometryShaderSource);

    connect(ui->editShaderButton, &QPushButton::clicked, this, &MainWindow::setShaderSourceEditDialogAtClickText);

    connect(shaderSourceEditDialog, &ShaderSourceEditDialog::vertShaderRadioButtonStateChanged, this, &MainWindow::setShaderSourceEditDialogTextAccordingToVertButton);
    connect(shaderSourceEditDialog, &ShaderSourceEditDialog::fragShaderRadioButtonStateChanged, this, &MainWindow::setShaderSourceEditDialogTextAccordingToFragButton);
    connect(shaderSourceEditDialog, &ShaderSourceEditDialog::geomShaderRadioButtonStateChanged, this, &MainWindow::setShaderSourceEditDialogTextAccordingToGeomButton);

    connect(ui->bezierCheckBox,  &QCheckBox::stateChanged, canvas, &Canvas::setBezierVisibility);
    connect(ui->bSplineCheckBox, &QCheckBox::stateChanged, canvas, &Canvas::setBSplineVisibility);
    connect(ui->bsplineSurfaceShadedCheckBox, &QCheckBox::stateChanged, canvas, &Canvas::setBSplineShadedVisibility);
    connect(ui->bsplineSurfaceShadedCheckBox, &QCheckBox::stateChanged, ui->flatShadingRadioButton, &QRadioButton::setChecked);

    connect(ui->flatShadingRadioButton, &QRadioButton::toggled, canvas, &Canvas::setFlatShading);
    connect(ui->gouraudShadingRadioButton, &QRadioButton::toggled, canvas, &Canvas::setGouraudShading);

    connect(ui->bsplineSurfaceCheckBox, &QCheckBox::stateChanged, canvas, &Canvas::setBSplineSurfaceVisibility);
    connect(ui->nurbsCheckBox, &QCheckBox::stateChanged, canvas, &Canvas::setNurbsVisibility);
    connect(ui->rationalBezierCheckBox, &QCheckBox::stateChanged, canvas, &Canvas::setRationalBezierVisibility);
    connect(ui->bsplineControlNetCheckBox, &QCheckBox::stateChanged, canvas, &Canvas::setBSplineSurfaceControlNetVisibility);
    connect(ui->subdivisionCurveCheckBox, &QCheckBox::stateChanged, canvas, &Canvas::setSubdivisionCurveVisibility);

    connect(ui->bsplineSurfaceCheckBox, &QCheckBox::stateChanged, ui->bsplineControlNetCheckBox, &QCheckBox::setChecked);
    connect(ui->bsplineSurfaceCheckBox, &QCheckBox::stateChanged, ui->densityOfBsplinesSlider, &QSlider::setEnabled);

    connect(ui->bsplineSurfaceShadedCheckBox, &QCheckBox::stateChanged, ui->rotateXslider, &QSlider::setEnabled);
    connect(ui->bsplineSurfaceShadedCheckBox, &QCheckBox::stateChanged, ui->rotateZslider, &QSlider::setEnabled);


    connect(ui->useLeapCheckBox, &QCheckBox::stateChanged, canvas, &Canvas::setUsingLeap);

    connect(ui->densityOfBsplinesSlider, &QSlider::valueChanged, canvas, &Canvas::recalculateSurfaceWithDensity);

    connect(canvas, &Canvas::setUseLeap, ui->useLeapCheckBox, &QCheckBox::setChecked);

    connect(ui->subdivisionIterationsSlider, &QSlider::sliderReleased, this, &MainWindow::setSubdivisionCurveIterations);
    connect(canvas, &Canvas::setUseLeap, [this](bool checkable){ ui->useLeapCheckBox->setEnabled(checkable);
    });
    connect(ui->rotateXslider, &QSlider::valueChanged, canvas, &Canvas::rotationXchanged);
    connect(ui->rotateZslider, &QSlider::valueChanged, canvas, &Canvas::rotationZchanged);

    connect(ui->newControlPointButton, &QRadioButton::toggled, canvas, &Canvas::setAddingControlPoints);
    connect(ui->movingButton,          &QRadioButton::toggled, canvas, &Canvas::setMoving);

    connect(ui->bsplineSurfaceCheckBox, &QCheckBox::stateChanged, ui->rotateXslider, &QSlider::setEnabled);
    connect(ui->bsplineSurfaceCheckBox, &QCheckBox::stateChanged, ui->rotateZslider, &QSlider::setEnabled);

    connect(canvas, &Canvas::setAddingControlPointsDisabled, this, &MainWindow::setAddingDisabled);
    connect(ui->bsplineSurfaceCheckBox, &QCheckBox::stateChanged, this, &MainWindow::setAddingDisabled);
    connect(ui->bsplineSurfaceCheckBox, &QCheckBox::stateChanged, this, &MainWindow::bsplineSurfaceCheckBoxStateChanged);

    connect(ui->bsplineSurfaceShadedCheckBox, &QCheckBox::stateChanged, this, &MainWindow::bsplineSurfaceCheckBoxStateChanged);

    connect(canvas, &Canvas::setLcdNumberValue, [this](int value){ui->controlPointCountLcdNumber->display(value);});
    connect(ui->resetWeightsButton, &QPushButton::clicked, canvas, &Canvas::resetWeights);

    connect(canvas, &Canvas::setResetButtonEnabled, ui->resetWeightsButton, &QPushButton::setEnabled);

    connect(settingsDialog, &SettingsDialog::emitBackGroundColorChanged, canvas, &Canvas::setBackGroundColor);

    connect(ui->flatShadingRadioButton, &QRadioButton::toggled, canvas, &Canvas::setFlatShading);
    connect(ui->gouraudShadingRadioButton, &QRadioButton::toggled, canvas, &Canvas::setGouraudShading);
    connect(ui->selectedPointXIndexSpinBox, static_cast<void(QSpinBox::*) (int)>(&QSpinBox::valueChanged), canvas, &Canvas::setSelectedControlPointI);
    connect(ui->selectedPointYIndexSpinBox, static_cast<void(QSpinBox::*) (int)>(&QSpinBox::valueChanged), canvas, &Canvas::setSelectedControlPointJ);

    connect(canvas, &Canvas::setSelectedControlpointSliderValues, this, &MainWindow::setSelectedpointSliderValues);
    connect(ui->selectedPointXIndexSpinBox,  static_cast<void(QSpinBox::*) (int)>(&QSpinBox::valueChanged), canvas, &Canvas::emitSelectedControlpointSliderValues);
    connect(ui->selectedPointYIndexSpinBox,  static_cast<void(QSpinBox::*) (int)>(&QSpinBox::valueChanged), canvas, &Canvas::emitSelectedControlpointSliderValues);

    connect(ui->selectedPointXCoordinateSlider, &QSlider::sliderReleased, this, &MainWindow::setSelectedpointX);
    connect(ui->selectedPointYCoordinateSlider, &QSlider::sliderReleased, this, &MainWindow::setSelectedpointY);
    connect(ui->selectedPointZCoordinateSlider, &QSlider::sliderReleased, this, &MainWindow::setSelectedpointZ);

    connect(ui->loadModel, &QPushButton::clicked, this, &MainWindow::openFileDialog);
    connect(ui->modelRadioButton, &QRadioButton::toggled, this, &MainWindow::onModelRadioButton);
    connect(ui->modelRadioButton, &QRadioButton::toggled, canvas, &Canvas::setModelVisibility);
    connect(this, &MainWindow::openedModelFileName, canvas, &Canvas::loadModel);

    connect(settingsDialog, &SettingsDialog::emitBezierCurveColorChanged, canvas, &Canvas::setBezierCurveColor);
    connect(settingsDialog, &SettingsDialog::emitBSplineCurveColorChanged, canvas, &Canvas::setBSplineCurveColor);
    connect(settingsDialog, &SettingsDialog::emitNurbsCurveColorChanged, canvas, &Canvas::setNurbsColor);
    connect(settingsDialog, &SettingsDialog::emitRationalBezierCurveColorChanged, canvas, &Canvas::setRationalBezierColor);
    connect(settingsDialog, &SettingsDialog::emitControlPointsColorChanged, canvas, &Canvas::setControlPointsColor);
    connect(settingsDialog, &SettingsDialog::emitBSplineSurfaceColorChanged, canvas, &Canvas::setBSplineSurfaceColor);
    connect(settingsDialog, &SettingsDialog::emitBSplineSurfaceCpointsColorChanged, canvas, &Canvas::setBSplineSurfaceCpointsColor);
    connect(settingsDialog, &SettingsDialog::emitBSplineSurfaceCnetColorChanged, canvas, &Canvas::setBSplineSurfaceCnetColor);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setSubdivisionCurveIterations()
{
    canvas->setSubdivisionCurveIterationsCount(ui->subdivisionIterationsSlider->value());
}

void MainWindow::onModelRadioButton(bool toggled)
{
    if (toggled)
    {
        ui->bezierCheckBox->setEnabled(false);
        ui->bSplineCheckBox->setEnabled(false);
        ui->bsplineControlNetCheckBox->setEnabled(false);
        ui->bsplineSurfaceCheckBox->setEnabled(false);
        ui->bsplineSurfaceShadedCheckBox->setEnabled(false);
        ui->densityOfBsplinesSlider->setEnabled(false);
        ui->movingButton->setChecked(false);
        ui->newControlPointButton->setEnabled(false);
        ui->nurbsCheckBox->setEnabled(false);
        ui->rationalBezierCheckBox->setEnabled(false);
        ui->subdivisionCurveCheckBox->setEnabled(false);
        ui->subdivisionIterationsSlider->setEnabled(false);

        ui->bsplineSurfaceCheckBox->setChecked(false);
        ui->bsplineSurfaceShadedCheckBox->setChecked(false);
        ui->movingButton->setCheckable(false);

        ui->rotateXslider->setEnabled(true);
        ui->rotateZslider->setEnabled(true);

        ui->subdivisionIterationsSlider->setEnabled(true);
    }
    else
    {
        ui->movingButton->setCheckable(true);
        ui->bezierCheckBox->setEnabled(true);
        ui->bSplineCheckBox->setEnabled(true);
        ui->bsplineControlNetCheckBox->setEnabled(true);
        ui->bsplineSurfaceCheckBox->setEnabled(true);
        ui->bsplineSurfaceShadedCheckBox->setEnabled(true);
        ui->densityOfBsplinesSlider->setEnabled(true);
        ui->movingButton->setChecked(true);
        ui->newControlPointButton->setEnabled(true);
        ui->nurbsCheckBox->setEnabled(true);
        ui->rationalBezierCheckBox->setEnabled(true);
        ui->subdivisionCurveCheckBox->setEnabled(true);
        ui->subdivisionIterationsSlider->setEnabled(true);

        ui->subdivisionIterationsSlider->setEnabled(false);
    }
}

void MainWindow::openFileDialog(bool clicked)
{
    const QString fileName = QFileDialog::getOpenFileName(this ,tr("Open your OBJ file!"),
                                                    QDir::homePath(),
                                                    tr("Obj files ( *.obj"));
    if (!fileName.isEmpty())
    {
        emit openedModelFileName(fileName);
    }
}

void MainWindow::initialize()
{
}

void MainWindow::setSelectedpointX()
{
    canvas->setSelectedControlPointX(ui->selectedPointXCoordinateSlider->value());
}

void MainWindow::setSelectedpointZ()
{
    canvas->setSelectedControlPointZ(ui->selectedPointZCoordinateSlider->value());
}

void MainWindow::setSelectedpointY()
{
    canvas->setSelectedControlPointY(ui->selectedPointYCoordinateSlider->value());
}

void MainWindow::setSelectedpointSliderValues(float x, float y, float z)
{
    ui->selectedPointXCoordinateSlider->setValue(x * 100);
    ui->selectedPointYCoordinateSlider->setValue(y * 100);
    ui->selectedPointZCoordinateSlider->setValue(z * 100);
}

void MainWindow::bsplineSurfaceCheckBoxStateChanged(bool state)
{
    if (state)
    {
        ui->bezierCheckBox->setCheckable(false);
        ui->bSplineCheckBox->setCheckable(false);
        ui->rationalBezierCheckBox->setCheckable(false);
        ui->nurbsCheckBox->setCheckable(false);

        ui->nurbsCheckBox->setChecked(false);
        ui->bezierCheckBox->setChecked(false);
        ui->bSplineCheckBox->setChecked(false);
        ui->rationalBezierCheckBox->setChecked(false);

        ui->subdivisionCurveCheckBox->setCheckable(false);
        ui->subdivisionCurveCheckBox->setChecked(false);

        ui->movingButton->setChecked(true);
        ui->newControlPointButton->setChecked(false);

        ui->bsplineControlNetCheckBox->setCheckable(true);
        ui->bsplineControlNetCheckBox->setChecked(true);

        ui->bsplineSurfaceShadedCheckBox->setCheckable(true);
        ui->bsplineSurfaceShadedCheckBox->setChecked(true);

        ui->controlPointCountLcdNumber->display(16);
        ui->subdivisionIterationsSlider->setEnabled(false);
    }
    else
    {
        if (!ui->bsplineSurfaceShadedCheckBox->isChecked())
        {
            ui->subdivisionIterationsSlider->setEnabled(true);

            ui->subdivisionCurveCheckBox->setCheckable(true);
            ui->subdivisionCurveCheckBox->setChecked(true);

            ui->bezierCheckBox->setCheckable(true);
            ui->bSplineCheckBox->setCheckable(true);
            ui->rationalBezierCheckBox->setCheckable(true);
            ui->nurbsCheckBox->setCheckable(true);

            ui->nurbsCheckBox->setChecked(true);
            ui->bezierCheckBox->setChecked(true);
            ui->bSplineCheckBox->setChecked(true);
            ui->rationalBezierCheckBox->setChecked(true);
            ui->controlPointCountLcdNumber->display(canvas->getControlPointsCount());
        }
    }
}

void MainWindow::setAddingDisabled(bool state)
{
    if (state)
    {
        ui->newControlPointButton->setEnabled(!state);
    }
    else
    {
        ui->newControlPointButton->setEnabled(!state);
    }

    update();
}

void MainWindow::setResetButtonEnabled(bool enabled)
{
    ui->resetWeightsButton->setEnabled(enabled);
}


void MainWindow::setLcdNumberDisplay(int value)
{
    ui->controlPointCountLcdNumber->display(value);
}

void MainWindow:: setAddingRadioButtonCheckable(bool checkable)
{
    ui->newControlPointButton->setCheckable(checkable);
}

void MainWindow::setShaderSourceEditDialogTextAccordingToVertButton(bool state)
{
    if (state)
    {
        setShaderSourceEditDialogText(canvas->getVertexShaderSource());
    }
}

void MainWindow::setShaderSourceEditDialogTextAccordingToFragButton(bool state)
{
    if (state)
    {
        setShaderSourceEditDialogText(canvas->getFragmentShaderSource());
    }
}
void MainWindow::setShaderSourceEditDialogTextAccordingToGeomButton(bool state)
{
    if (state)
    {
        setShaderSourceEditDialogText(canvas->getGeometryShaderSource());
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() & Qt::Key_Escape)
    {
        close();
    }
    else if (e->key() & Qt::Key_S)
    {
    }
}

void MainWindow::setShaderSourceEditDialogText(const QString& text)
{
    shaderSourceEditDialog->setShaderEditText(text);
}

void MainWindow::setShaderSourceEditDialogAtClickText()
{
    shaderSourceEditDialog->setShaderEditText(canvas->getVertexShaderSource());
}
