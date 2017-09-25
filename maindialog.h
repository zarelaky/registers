#ifndef MAINDIALOG_H
#define MAINDIALOG_H
#include <QLabel>
#include <QDialog>
#include <QLineEdit>
#include <QMap>

class MainDialog : public QDialog
{
    Q_OBJECT
    struct BIN_V{
        BIN_V(int v, int p)
            : val(v)
            , pow(p){}
        int val;
        const int pow;
    };

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();



protected slots:
    void linkActivated(QString);
    void onReset0(bool);
    void onReset1(bool);
    void edtDecChanged(QString);
    void edtHexChanged(QString);
private:
    void setLabelValue(QLabel* l, int v);
    void calcBinaryValue();
    void resetAllBit(int v);
    void updateLEDT();
    void updateAllBits();
    void updateDec();
    void updateHex();
private:
    QMap<QLabel*, BIN_V*> _l;
    quint32 _dec;
    QLineEdit* _ledtDec;
    QLineEdit* _ledtHex;

};



#endif // MAINDIALOG_H
