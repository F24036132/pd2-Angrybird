#include "explain.h"
#include "ui_explain.h"

Explain::Explain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Explain)
{
    ui->setupUi(this);
    this->setWindowTitle("Help");
}

Explain::~Explain()
{
    delete ui;
}
