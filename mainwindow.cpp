#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QStringList MainWindow::Convert_Biner()
{
    QString result_decode_base64 = decode_base64(ui->txtRawPayload->toPlainText());
    QString result_hex_to_bin = hex_to_bin_conversion(result_decode_base64);
//    QString result_5cut_32get = format_5cut_32get(result_hex_to_bin);
    int count = 0; QString temp; QStringList result; QString SIN; QString MIN;
//    for (int i = 0; i < result_5cut_32get.size(); i++) {
//        count++;
//        temp.append(result_5cut_32get.at(i));
//        if (count == 32) {
//            result.insert(result.length(), temp);
//            temp.clear();
//            count = 0;
//        }
//    }
    for (int i = 0; i < result_hex_to_bin.length(); i++) {
        if (count < 8) {
            SIN = SIN + result_hex_to_bin.at(i);
        } else if (count >= 8 && count < 16) {
            MIN = MIN + result_hex_to_bin.at(i);
        } else {
            temp = temp + result_hex_to_bin.at(i);
        }
        count++;
    }
    result.clear(); count = 0;
    for (int i = 0; i < temp.length(); i+=37) {
        result.insert(count, "");
        for (int j = i; j < i+37; j++) {
            if (j < temp.length()) {
                result[count] = result.at(count) + temp.at(j);
            }
        }
        count++;
    }
    temp.clear(); QString separator; QString value; QStringList biner;
    for (int i = 0; i < result.length(); i++) {
        temp = result.at(i);
        for (int j = 0; j < temp.length(); j++) {
            if (j < 5) {
                separator = separator + temp.at(j);
            } else {
                value = value + temp.at(j);
            }
        }
        biner.append(separator); biner.append(value);
        separator.clear(); value.clear();
    }
    return biner;
}

void MainWindow::on_pbBiner_clicked()
{
    ui->txtResult->clear();
    QStringList biner = this->Convert_Biner();
    int count = 0;
    for (int i = 0; i < biner.length(); i+=2) {
        ui->txtResult->append(QString::number(count) + " :: " + biner.at(i) + " - " + biner.at(i+1));
        count++;
    }
}

void MainWindow::on_pbData_clicked()
{
    ui->txtResult->clear();
    QStringList biner = this->Convert_Biner();
    int count = 0;
    for (int i = 0; i < biner.length(); i+=2) {
        ui->txtResult->append(QString::number(count) + " :: " + this->binerToData(biner.at(i+1)));
        count++;
    }
}

QString MainWindow::decode_base64(QString data){

    QString text_akhir;
    QByteArray ba;

    ba.append(data);

    text_akhir = QByteArray::fromBase64(ba).toHex();

    return (QString) text_akhir;
}

QString MainWindow::hex_to_bin(char dat){
    QString biner;

    if (dat == 'F' || dat == 'f') biner = "1111";
    else if (dat == 'E' || dat == 'e') biner = "1110";
    else if (dat == 'D' || dat == 'd') biner = "1101";
    else if (dat == 'C' || dat == 'c') biner = "1100";
    else if (dat == 'B' || dat == 'b') biner = "1011";
    else if (dat == 'A' || dat == 'a') biner = "1010";
    else if (dat == '9') biner = "1001";
    else if (dat == '8') biner = "1000";
    else if (dat == '7') biner = "0111";
    else if (dat == '6') biner = "0110";
    else if (dat == '5') biner = "0101";
    else if (dat == '4') biner = "0100";
    else if (dat == '3') biner = "0011";
    else if (dat == '2') biner = "0010";
    else if (dat == '1') biner = "0001";
    else if (dat == '0') biner = "0000";

    return (QString) biner;
}

QString MainWindow::hex_to_bin_conversion(QString hex){
    QString biner = "";

    char dat[hex.size()+1];

    strcpy(dat, hex.toLatin1());

    /* 4 bit pertama di buang, karena hanya sbg penanda
       SIN / MIN dari data SkyWave */

//    for (int i = 4; i < hex.size(); i++){
//        biner.sprintf("%s%s", biner.toLocal8Bit().data(), hex_to_bin(dat[i]).toLocal8Bit().data());
//    }

    for (int i = 0; i < hex.size(); i++){
        biner.sprintf("%s%s", biner.toLocal8Bit().data(), hex_to_bin(dat[i]).toLocal8Bit().data());
    }

    return (QString) biner;
}

QString MainWindow::format_5cut_32get(QString biner){

    char dat[(biner.size()+1)];

    int c_5bit  = 0;
    int c_32bit = 0;

    bool buang_5bit  = false;
//    bool ambil_32bit = false;

    QString dat_5bit  = ""; /* 5 bit yang di buang */
    QString dat_32bit = ""; /* 32 bit yang di ambil */
    QString all_32bit = ""; /* gabung semua 32 bit */

    strcpy(dat, biner.toLatin1());

    for (int i = 0; i < biner.size(); i++){
        if (buang_5bit == false){
            dat_5bit.sprintf("%s%c", dat_5bit.toLocal8Bit().data(), dat[i]);

            if (c_5bit == 4){
                buang_5bit = true;
                c_32bit = 0;
                dat_5bit = "";
            }
            c_5bit++;
        }
        else if(buang_5bit == true){
            dat_32bit.sprintf("%s%c", dat_32bit.toLocal8Bit().data(), dat[i]);

            if (c_32bit == 31){
                all_32bit.sprintf("%s%s", all_32bit.toLocal8Bit().data(), dat_32bit.toLocal8Bit().data());

                buang_5bit = false;
                c_5bit = 0;
                dat_32bit = "";
            }
            c_32bit++;
        }
    }
    return (QString) all_32bit;
}

QString MainWindow::binerToData(QString biner)
{
    bool ok;
    int sign = 1;
    QString data_real;
    QByteArray dataBin = biner.toUtf8();
    if(dataBin.length()==32) {
        if(dataBin.at(0)=='1')  sign =-1;                       // if bit 0 is 1 number is negative
        dataBin.remove(0,1);                                     // remove sign bit
    }
    QByteArray fraction =dataBin.right(23);   //get the fractional part
    double mantissa = 0;
    for(int i=0;i<fraction.length();i++)     // iterate through the array to claculate the fraction as a decimal.
        if(fraction.at(i)=='1')     mantissa += 1.0/(pow(2,i+1));
    int exponent = dataBin.left(dataBin.length()-23).toLongLong(&ok,2)-127;     //claculate the exponent
    data_real = QString::number( sign*pow(2,exponent)*(mantissa+1.0),'f', 9 );
//    data_decimal = QString::number( sign*pow(2,exponent)*(mantissa+1.0),'f', 0 );
    return data_real;
}
