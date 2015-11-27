#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

struct VFOStruct
{
    QString serialport;
    int serialportN;
    QString mode;
    int modeN;
    int port;
    int portN;
    double DDSclock;
    double fmax;
    double fmin;
    bool configured;
    bool multiplier;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setLimits();

    void StartHW();

private slots:
    void setNewFrequency(qint64 freq);

    void on_actionConfigure_triggered();

    void on_actionAbout_triggered();

    void on_actionAbout_Qt_triggered();

    void on_actionExit_triggered();

    void on_QuitPushButton_clicked();

private:
    Ui::MainWindow *ui;

    void ReadSettings();

    void WriteSettings();

};

#endif // MAINWINDOW_H
