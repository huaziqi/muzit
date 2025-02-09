#include "sharewidget.h"
#include "ui_sharewidget.h"

ShareWidget::ShareWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ShareWidget)
{
    ui->setupUi(this);
}

ShareWidget::~ShareWidget()
{
    delete ui;
}
