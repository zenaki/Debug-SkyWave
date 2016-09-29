#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QStringList Convert_Biner();
    QString decode_base64(QString data);
    QString hex_to_bin(char dat);
    QString hex_to_bin_conversion(QString hex);
    QString format_5cut_32get(QString biner);
    QString binerToData(QString biner);

private slots:
    void on_pbBiner_clicked();
    void on_pbData_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
