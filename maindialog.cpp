#include "maindialog.h"
#include <QHBoxLayout>

#include <QGridLayout>
#include <QDebug>
#include <QLineEdit>
#include <QPushButton>
#include <math.h>

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent)
{
    setFixedSize(640,400);
    _ledtHex = new QLineEdit(this);
    _ledtDec = new QLineEdit(this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    QGridLayout* gridlayout = new QGridLayout(this);
    int maxBitCount = 32;
    for (int i = 0; i < maxBitCount; ++i) {
        QLabel* t = new QLabel(QString("%1").arg(maxBitCount - 1 - i));

        t->setAlignment(Qt::AlignCenter);
        gridlayout->addWidget(t, 0, i);
        QLabel* l = new QLabel(this);

        l->setAlignment(Qt::AlignCenter);
        setLabelValue(l, 0);
        _l[l]= new BIN_V(0, maxBitCount - 1 - i);
        connect(l, SIGNAL(linkActivated(QString)), SLOT(linkActivated(QString)));
        gridlayout->setColumnMinimumWidth(i, 44);
        gridlayout->addWidget(l, 1, i);

    }

    QPushButton* btnReset0 = new QPushButton("0", this);
    connect(btnReset0, SIGNAL(clicked(bool)), SLOT(onReset0(bool)));
    gridlayout->addWidget(btnReset0, 1, 33);

    QPushButton* btnReset1 = new QPushButton("1",this);
    connect(btnReset1, SIGNAL(clicked(bool)), SLOT(onReset1(bool)));
    gridlayout->addWidget(btnReset1, 1, 34);

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


    layout->addSpacerItem(new QSpacerItem(0, 300,
                                          QSizePolicy::Maximum,
                                          QSizePolicy::Maximum));
    _dec = 0;
    setLayout(layout);


    calcBinaryValue();
    updateLEDT();
}


MainDialog::~MainDialog()
{
    QMapIterator<QLabel*, BIN_V*> itr(_l);
    while (itr.hasNext()) {
        BIN_V* p = itr.next().value();
        delete p;
    }
}

void MainDialog::calcBinaryValue() {
    _dec = 0;
    QMapIterator<QLabel*, BIN_V*> itr(_l);
    while (itr.hasNext()) {
        BIN_V* p = itr.next().value();
        if (p->val) {
            _dec += pow(2, p->pow);
        }
    }
}

void MainDialog::setLabelValue(QLabel* l, int v) {
    if (_l.find(l) != _l.end()) {
        _l[l]->val = v;
    }
    l->setText(QString("<a href=\"%1\">%1</a>").arg(v));

}

void MainDialog::linkActivated(QString linkText) {
    QLabel* l = qobject_cast<QLabel*>(sender());
    if (l) {
        int v = 0;
        qDebug() << linkText << " " << l->text();
        if (0 == linkText.compare("0")) {
            v = 1;
        }
        setLabelValue(l, v);
        calcBinaryValue();
        updateLEDT();
    }
}

void MainDialog::resetAllBit(int v)
{
    QMapIterator<QLabel*, BIN_V*> itr(_l);
    while (itr.hasNext()) {
        itr.next();
        BIN_V* p = itr.value();
        QLabel* l = itr.key();
        setLabelValue(l, v);
        p->val = v;

    }
}

void MainDialog::onReset0(bool) {
    resetAllBit(0);
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

void MainDialog::onReset1(bool)
{
    resetAllBit(1);
    _dec = -1;
    updateLEDT();
}

void MainDialog::updateAllBits()
{
    QMapIterator<QLabel*, BIN_V*> itr(_l);
    while (itr.hasNext()) {
        itr.next();
        BIN_V* p = itr.value();
        setLabelValue(itr.key(), (_dec & (1 << p->pow))? 1:0);
    }
}

void MainDialog::updateBinary() {
    QMapIterator<QLabel*, BIN_V*> itr(_l);
    QString str = "00000000000000000000000000000000";
    while (itr.hasNext()) {
        itr.next();
        BIN_V* p = itr.value();
        str.replace(31-p->pow, 1, (_dec & (1 << p->pow))? "1":"0");
    }
    _ledtBinary->setText(str);
}


void MainDialog::edtDecChanged(QString txt)
{
    _dec = txt.toUInt();
    updateHex();
    updateAllBits();
    updateBinary();
}

void MainDialog::edtHexChanged(QString txt)
{
    _dec = txt.toUInt(NULL, 16);
    updateDec();
    updateAllBits();
    updateBinary();
}

void MainDialog::edtBinaryChanged(QString txt) {
    _dec = txt.toUInt(NULL, 2);
    updateHex();
    updateDec();
    updateAllBits();
}
