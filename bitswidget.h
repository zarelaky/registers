#ifndef BITSWIDGET_H
#define BITSWIDGET_H
#include <QWidget>
#include <list>

class BitsWidget : public QWidget
{
    Q_OBJECT
public:
    BitsWidget();
    struct SelectedValueInfo {
        unsigned int value;
        int          shift;
    };
    SelectedValueInfo selectedValue() const;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    void setValue(unsigned int value);
    unsigned int value() const;
private:
    void dumpValues();
signals:
    void valueChanged();
    void selectedValueChanged();

private:
    int     bitValue(const int index) const;
    void    setBit(const int index);
    void    clearBit(const int index);
    int     indexAtPosition(const QPoint& pos);
    bool    isValidBitIndex(const int index) const;
    void    beginSelect(const QPoint& pos,const QPoint& lastPos);
    void    select(const QPoint& pos, const QPoint& lastPos);
    void    endSelect(const QPoint& pos,const QPoint& lastPos);
    void    addOrRemoveFromSelectIndex(const int index);
    bool    isSelected(const int index) const;



private:
    int             _bitsCount;
    unsigned int    _value;
    std::list<int>  _selectedIndexes;
    QPoint          _lastPos;
};

#endif // BITSWIDGET_H
