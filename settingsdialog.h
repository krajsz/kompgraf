#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QKeyEvent>
#include <QString>
#include <QColor>
#include <QColorDialog>
#include <QPalette>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private:
    Ui::SettingsDialog *ui;

    bool bezierColorValueChanged = false;
    bool bsplineColorValueChanged = false;
    bool nurbsColorValueChanged = false;
    bool rationalBezierColorValueChanged = false;
    bool controlPointsColorValueChanged = false;
    bool backgroundColorValueChanged = false;
    bool bsplineSurfaceColorValueChanged = false;
    bool bsplineSurfaceCnetColorValueChanged = false;
    bool bsplineSurfaceCpointsColorValueChanged = false;


    bool customBezierColorSelected = false;
    bool customBSplineColorSelected = false;
    bool customNurbsColorSelected = false;
    bool customRationalBezierColorSelected = false;
    bool customControlPointsColorSelected = false;
    bool customBackgroundColorSelected = false;
    bool customBsplineSurfaceColorSelected = false;
    bool customBsplineSurfaceCnetColorSelected = false;
    bool customBsplineSurfaceCpointsColorSelected = false;


    QColorDialog* colorDialog;

    QColor backgroundColorSelected;
    QColor bezieColorSelected;
    QColor bsplineColorSelected;
    QColor nurbsColorSelected;
    QColor rationalBezierColorSelected;
    QColor controlPointsColorSelected;
    QColor bsplineSurfaceColorSelected;
    QColor bsplineSurfaceControlNetColorSelected;
    QColor bsplineSurfaceControlPointsColorSelected;

    int buttonClicked = 0;

protected:
    void keyPressEvent(QKeyEvent* e) Q_DECL_OVERRIDE;
    void accept() Q_DECL_OVERRIDE;
    void reject() Q_DECL_OVERRIDE;

private slots:
    void setBackgroundValueChanged();
    void setEmitBackgroundColor();

    void setBezierValueChanged();
    void setEmitBezierColor();

    void setBSplineValueChanged();
    void setEmitBSplineColor();

    void setNurbsValueChanged();
    void setEmitNurbsColor();

    void setRationalBezierValueChanged();
    void setEmitRationalBezierColor();

    void setControlPointsValueChanged();
    void setEmitControlPointsColor();

    void setBSplineSurfaceValueChanged();
    void setEmitBSplineSurfaceColor();

    void setBSplineSurfaceCnetValueChanged();
    void setEmitBSplineSurfaceCnetColor();

    void setBSplineSurfaceCpointsValueChanged();
    void setEmitBSplineSurfaceCpointsColor();

    void showBackgroundColorDialog();
    void showBezierColorDialog();
    void showBSplineColorDialog();
    void showBSplineSurfaceCnetColorDialog();
    void showBSplineSurfaceColorDialog();
    void showBSplineSurfaceCpointsColorDialog();
    void showNurbsColorDialog();
    void showRationalBezierColorDialog();
    void showControlPointsColorDialog();

signals:
    void emitBackGroundColorChanged(const QColor& color);

    void emitBezierCurveColorChanged(const QColor& color);
    void emitBSplineCurveColorChanged(const QColor& color);
    void emitNurbsCurveColorChanged(const QColor& color);
    void emitRationalBezierCurveColorChanged(const QColor& color);
    void emitControlPointsColorChanged(const QColor& color);
    void emitBSplineSurfaceCnetColorChanged(const QColor& color);
    void emitBSplineSurfaceColorChanged(const QColor& color);
    void emitBSplineSurfaceCpointsColorChanged(const QColor& color);


};

#endif // SETTINGSDIALOG_H
