#ifndef MAINDIALOG_H
#define MAINDIALOG_H
#include <QLabel>
#include <QDialog>
#include <QLineEdit>
#include <QMap>

class BitsWidget;

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();

protected slots:
    void onReset0(bool);
    void onReset1(bool);
    void edtDecChanged(QString);
    void edtHexChanged(QString);
    void edtBinaryChanged(QString txt);
    void bitsValueChanged();
    void selectedValueChanged();

private:
    void setLabelValue(QLabel* l, int v);
    void calcBinaryValue();
    void resetAllBit(int v);
    void updateLEDT();
    void updateDec();
    void updateHex();
    void updateBinary();
    void updateAllBits();
private:
    quint32 _dec;
    BitsWidget* _bitsWidget;
    QLineEdit* _ledtDec;
    QLineEdit* _ledtHex;
    QLineEdit* _ledtBinary;
    QLineEdit* _ledtMarkedDec;
};



#endif // MAINDIALOG_H
