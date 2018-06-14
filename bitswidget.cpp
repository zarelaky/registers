#include "bitswidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <algorithm>
#include <QDebug>

const int INVALID_BIT_INDEX = -1;

BitsWidget::BitsWidget()
    : _bitsCount(32)
    , _value(0)
{
    connect(this, &BitsWidget::valueChanged, &BitsWidget::dumpValues);
    connect(this, &BitsWidget::valueChanged, &BitsWidget::selectedValueChanged);
}

int BitsWidget::bitValue(const int index) const {
    return (_value & (1<<(_bitsCount - 1 - index))) == 0 ? 0:1;
}

void BitsWidget::setBit(const int index) {
    unsigned int oldV = _value;
    _value |= (1 << (_bitsCount - 1 - index));
    if (oldV != _value) {
        emit valueChanged();
    }
}

void BitsWidget::clearBit(const int index) {
    unsigned int oldV = _value;

    _value &= ~(1 << (_bitsCount - 1 - index));

    if (oldV != _value) {
        emit valueChanged();
    }
}
void BitsWidget::paintEvent(QPaintEvent */*event*/) {
    QPainter p(this);
    p.fillRect(rect(), qRgb(0xff,0xff,0xff));
    float w = rect().width()/_bitsCount;
    float h = (rect().height() - 1)/2;
    QRectF r(0,0, w, h);
    QRectF r2(0, h, w, h);
    QTextOption to;
    to.setAlignment(Qt::AlignCenter);

    for (int i = 0; i < _bitsCount; ++i) {
        r.moveTo(i*w, r.top());
        r2.moveTo(i*w, h);
        p.drawText(r, QString("%1").arg(_bitsCount - 1 - i), to);
        p.drawText(r2, QString("%1").arg(bitValue(i)), to);
        p.save();
        if (isSelected(i)) {
            QPen pen;
            pen.setStyle(Qt::SolidLine);
            pen.setColor(qRgb(0,0xff,0));
            p.setPen(pen);
            p.drawRect(r2.adjusted(0,0,-1,1));
        } else {
            p.drawRect(r2);
        }


        p.restore();
    }
}

int BitsWidget::indexAtPosition(const QPoint& pos) {
    QRect rc = rect();
    rc.setTop(rc.top()+rc.height()/2);
    if (!rc.contains(pos)) {
        return INVALID_BIT_INDEX;
    }
    return pos.x()/(rc.width()/_bitsCount);
}

bool BitsWidget::isValidBitIndex(const int index) const {
    return index >= 0 && index < _bitsCount;
}

void BitsWidget::beginSelect(const QPoint &pos,const QPoint& lastPos)
{
    select(pos, lastPos);
}

void BitsWidget::select(const QPoint &pos, const QPoint& lastPos)
{
    const int lastIndex = indexAtPosition(lastPos);
    const int index = indexAtPosition(pos);
    if (isValidBitIndex(index)&& lastIndex != index) {
        addOrRemoveFromSelectIndex(index);
    }
}

void BitsWidget::endSelect(const QPoint &pos, const QPoint& lastPos)
{
    select(pos, lastPos);
    _lastPos.setX(0);
    _lastPos.setY(0);
}

void BitsWidget::addOrRemoveFromSelectIndex(const int index) {
    if (std::find(_selectedIndexes.begin(), _selectedIndexes.end(), index) != _selectedIndexes.end()) {
        _selectedIndexes.remove(index);
        qDebug() << "remove index " << index;
    } else {
        _selectedIndexes.push_back(index);
        qDebug() << "add index " << index;
    }
    update();
    dumpValues();
    emit selectedValueChanged();
}

bool BitsWidget::isSelected(const int index) const {
    return std::find(_selectedIndexes.begin(), _selectedIndexes.end(), index) != _selectedIndexes.end();
}

BitsWidget::SelectedValueInfo BitsWidget::selectedValue() const
{
    SelectedValueInfo svi = { 0 , _bitsCount};

    std::for_each(_selectedIndexes.begin(), _selectedIndexes.end(), [&svi, this](int index){
        if ((_bitsCount - 1 - index) < svi.shift) {
            svi.shift = _bitsCount - 1 - index;
        }
        if (bitValue(index)) {
            svi.value |= (1<<(_bitsCount - 1 - index));
        }
    });
    return svi;
}

void BitsWidget::mousePressEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton)) {
        const int bitIndex = indexAtPosition(event->pos());
        if (isValidBitIndex(bitIndex)) {
            if (bitValue(bitIndex)) {
                clearBit(bitIndex);
            } else {
                setBit(bitIndex);
            }

            update(rect());
        }

    } else if (event->buttons() & Qt::RightButton) {
        beginSelect(event->pos(), _lastPos);
    }
    _lastPos = event->pos();
}

void BitsWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::RightButton) {
        endSelect(event->pos(), _lastPos);
    }
    _lastPos.setX(0);
    _lastPos.setY(0);

}

void BitsWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::RightButton) {
        select(event->pos(), _lastPos);
    }
    _lastPos  = event->pos();
}

void BitsWidget::setValue(unsigned int value) {
    _value = value;
    update();
    emit selectedValueChanged();
}

unsigned int BitsWidget::value() const {
    return _value;
}

void BitsWidget::dumpValues() {
    SelectedValueInfo svi = selectedValue();
    qDebug() << "selected value:" << svi.value << " shift:" << svi.shift;
}
