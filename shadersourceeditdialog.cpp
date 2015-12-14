#include "shadersourceeditdialog.h"
#include "ui_shadersourceeditdialog.h"

ShaderSourceEditDialog::ShaderSourceEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShaderSourceEditDialog)
{
    ui->setupUi(this);
    connect(ui->saveButton,   &QPushButton::clicked, this, &ShaderSourceEditDialog::accept);
    connect(ui->rejectButton, &QPushButton::clicked, this, &ShaderSourceEditDialog::reject);
    connect(ui->vertexShaderRadioButton,   &QRadioButton::toggled, this, &ShaderSourceEditDialog::vertShaderRadioButtonStateChanged);
    connect(ui->fragmentShaderRadioButton, &QRadioButton::toggled, this, &ShaderSourceEditDialog::fragShaderRadioButtonStateChanged);
    connect(ui->geometryShaderRadioButton, &QRadioButton::toggled, this, &ShaderSourceEditDialog::geomShaderRadioButtonStateChanged);

}

ShaderSourceEditDialog::~ShaderSourceEditDialog()
{
    delete ui;
}

void ShaderSourceEditDialog::accept()
{
    shaderSource = ui->shaderEdit->toPlainText();

    if (ui->vertexShaderRadioButton->isChecked())
    {
        qDebug() << "vertChecked\n";

        emit vertShaderSourceSaved(shaderSource);
    }
    else if (ui->fragmentShaderRadioButton->isChecked())
    {
        qDebug() << "fragChecked\n";

        emit fragShaderSourceSaved(shaderSource);
    }
    else if (ui->geometryShaderRadioButton->isChecked())
    {
        qDebug() << "geomChecked\n";
        emit geomShaderSourceSaved(shaderSource);
    }

    ui->vertexShaderRadioButton->setChecked(true);

    QDialog::accept();
}

void ShaderSourceEditDialog::keyPressEvent(QKeyEvent *e)
{
    if (e->key() & Qt::Key_Escape)
    {
        reject();
    }
    else if (e->key() & Qt::Key_Enter)
    {
        accept();
    }
}

void ShaderSourceEditDialog::reject()
{
    ui->vertexShaderRadioButton->setChecked(true);
    QDialog::reject();
}

void ShaderSourceEditDialog::setShaderEditText(const QString &text)
{
    ui->shaderEdit->setText(text);
}
