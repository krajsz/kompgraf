#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "canvas.h"
#include "settingsdialog.h"
#include "shadersourceeditdialog.h"
#include <QMainWindow>
#include <QKeyEvent>
#include <QFileDialog>
#include <QDir>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initialize();

private:
    Canvas* canvas;
    Ui::MainWindow *ui;
    SettingsDialog* settingsDialog;
    ShaderSourceEditDialog* shaderSourceEditDialog;

Q_SIGNALS:

    void openedModelFileName(const QString& fileName);

private Q_SLOTS:
    void setResetButtonEnabled(bool enabled);

    void onModelRadioButton(bool toggled);

    void openFileDialog(bool clicked);

    void setSubdivisionCurveIterations();

    void bsplineSurfaceCheckBoxStateChanged(bool state);

    void setSelectedpointSliderValues(float x, float y, float z);

    void setAddingDisabled(bool disabled = false);

    void setShaderSourceEditDialogText(const QString &text);
    void setShaderSourceEditDialogAtClickText();

    void setSelectedpointX();
    void setSelectedpointY();
    void setSelectedpointZ();

    void setShaderSourceEditDialogTextAccordingToVertButton(bool state);
    void setShaderSourceEditDialogTextAccordingToFragButton(bool state);
    void setShaderSourceEditDialogTextAccordingToGeomButton(bool state);

    void setLcdNumberDisplay(int value);

    void setAddingRadioButtonCheckable(bool checkable);

protected:
    void keyPressEvent(QKeyEvent* e) Q_DECL_OVERRIDE;
};

#endif // MAINWINDOW_H
