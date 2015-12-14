#ifndef SHADERSOURCEEDITDIALOG_H
#define SHADERSOURCEEDITDIALOG_H

#include <QDialog>
#include <QKeyEvent>
#include <QDebug>

namespace Ui {
class ShaderSourceEditDialog;
}

class ShaderSourceEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShaderSourceEditDialog(QWidget *parent = 0);
    ~ShaderSourceEditDialog();

signals:
    void vertShaderSourceSaved(const QString& source);
    void fragShaderSourceSaved(const QString& source);
    void geomShaderSourceSaved(const QString& source);

    void vertShaderRadioButtonStateChanged(bool state);
    void fragShaderRadioButtonStateChanged(bool state);
    void geomShaderRadioButtonStateChanged(bool state);


public slots:
    void setShaderEditText(const QString& text);
private slots:


private:
    Ui::ShaderSourceEditDialog *ui;
    QString shaderSource;

protected:
    void accept() Q_DECL_OVERRIDE;
    void reject() Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent * e) Q_DECL_OVERRIDE;
};

#endif // SHADERSOURCEEDITDIALOG_H
