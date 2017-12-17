#include "NavWidget.h"
#include "ui_NavWidget.h"

#include <QSignalMapper>
#include <QPushButton>

NavWidget::NavWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NavWidget)
{
    ui->setupUi(this);

    m_signalMapper = new QSignalMapper(this);
    connect(m_signalMapper, SIGNAL(mapped(QWidget*)),
            this, SIGNAL(tabChanged(QWidget*)));
}

NavWidget::~NavWidget()
{
    delete ui;
}

void NavWidget::addTab(const QString &text, QWidget *w)
{
    QPushButton *button = new QPushButton(text, this);
    connect(button, SIGNAL(clicked()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(button, w);

    ui->horizontalLayout->removeItem(ui->horizontalSpacer);
    ui->horizontalLayout->addWidget(button);
    ui->horizontalLayout->addItem(ui->horizontalSpacer);
}
