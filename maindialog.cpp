#include "maindialog.h"
#include <QHBoxLayout>

#include <QGridLayout>
#include <QDebug>
#include <QLineEdit>
#include <QPushButton>
#include <math.h>
#include "bitswidget.h"

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent)
{
    setFixedSize(640,400);
    _ledtHex = new QLineEdit(this);
    _ledtDec = new QLineEdit(this);


    QVBoxLayout* layout = new QVBoxLayout(this);
    QGridLayout* gridlayout = new QGridLayout(this);

    _bitsWidget = new BitsWidget();
    _bitsWidget->setFixedHeight(48);
    layout->addWidget(_bitsWidget);
    connect(_bitsWidget, SIGNAL(valueChanged()), SLOT(bitsValueChanged()));
    connect(_bitsWidget, SIGNAL(selectedValueChanged()), SLOT(selectedValueChanged()));


    QFont fontTitle(font());
    fontTitle.setPixelSize(11);

    QPushButton* btnReset0 = new QPushButton("0", this);
    connect(btnReset0, SIGNAL(clicked(bool)), SLOT(onReset0(bool)));
    gridlayout->addWidget(btnReset0, 1, 0);

    QPushButton* btnReset1 = new QPushButton("1",this);
    connect(btnReset1, SIGNAL(clicked(bool)), SLOT(onReset1(bool)));
    gridlayout->addWidget(btnReset1, 1, 1);

    layout->addLayout(gridlayout);

    _ledtHex->setAlignment(Qt::AlignRight);
    connect(_ledtHex, SIGNAL(textChanged(QString)), SLOT(edtHexChanged(QString)));
    gridlayout->addWidget(_ledtHex, 2, 0, 1, 32);
    gridlayout->addWidget(new QLabel("HEX"), 2,33,1,2);


    _ledtDec->setAlignment(Qt::AlignRight);
    connect(_ledtDec, SIGNAL(textChanged(QString)), SLOT(edtDecChanged(QString)));
    gridlayout->addWidget(_ledtDec, 3, 0, 1, 32);
    gridlayout->addWidget(new QLabel("DEC"), 3,33,1,2);

    _ledtBinary = new QLineEdit(this);
    _ledtBinary->setAlignment(Qt::AlignRight);
    connect(_ledtBinary, SIGNAL(textChanged(QString)), SLOT(edtBinaryChanged(QString)));
    gridlayout->addWidget(_ledtBinary, 4, 0, 1, 32);
    gridlayout->addWidget(new QLabel("BIN"), 4,33,1,2);

    _ledtMarkedDec = new QLineEdit(this);
    _ledtMarkedDec->setAlignment(Qt::AlignRight);
    _ledtMarkedDec->setReadOnly(true);
    gridlayout->addWidget(_ledtMarkedDec, 5, 0, 1, 32);
    gridlayout->addWidget(new QLabel("DEC(marked)"), 5, 33, 1, 2);



    layout->addSpacerItem(new QSpacerItem(0, 300,
                                          QSizePolicy::Maximum,
                                          QSizePolicy::Maximum));
    _dec = 0;
    setLayout(layout);
    updateLEDT();
}


MainDialog::~MainDialog() {

}

void MainDialog::onReset0(bool) {
    _dec = 0;
    updateLEDT();
}

void MainDialog::updateDec()
{
    _ledtDec->setText(QString().sprintf("%u", _dec));
}

void MainDialog::updateHex()
{
    _ledtHex->setText(QString().sprintf("%08X", _dec));
}

void MainDialog::updateLEDT()
{
    updateDec();
    updateHex();
    updateBinary();
}

void MainDialog::onReset1(bool){
    _dec = -1;
    updateLEDT();
}


void MainDialog::selectedValueChanged() {
    BitsWidget::SelectedValueInfo svi = _bitsWidget->selectedValue();
    unsigned int v = svi.value >> svi.shift;
    _ledtMarkedDec->setText(QString().sprintf("%u", v));
}

void MainDialog::updateBinary() {
    QString str = "00000000000000000000000000000000";
    for (int i = 0; i < 32; ++i) {
        str.replace(31-i, 1, (_dec & (1 << i))? "1":"0");
    }
    _ledtBinary->setText(str);
    _bitsWidget->setValue(_dec);
}



void MainDialog::edtDecChanged(QString txt)
{
    _dec = txt.toUInt();
    updateHex();
    updateBinary();
}

void MainDialog::edtHexChanged(QString txt)
{
    _dec = txt.toUInt(NULL, 16);
    updateDec();
    updateBinary();
}

void MainDialog::edtBinaryChanged(QString txt) {
    _dec = txt.toUInt(NULL, 2);
    updateHex();
    updateDec();
}

void MainDialog::bitsValueChanged() {
    _dec = _bitsWidget->value();
    updateLEDT();
}
