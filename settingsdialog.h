#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

    static VFOStruct vfo;

private slots:
    void on_DeviceComboBox_currentIndexChanged(int index);

    void on_ClockDoubleSpinBox_editingFinished();

    void on_MinFrequencyDoubleSpinBox_editingFinished();

    void on_MaxFrequencyDoubleSpinBox_editingFinished();

    void on_SerialComboBox_currentIndexChanged(int index);

    void on_buttonBox_accepted();

    void on_MultiplierCheckBox_clicked(bool checked);

private:
    Ui::SettingsDialog *ui;

    void SetInitialValues();

    void UpdateWindow();

    int fillPortsInfo();

};

#endif // SETTINGSDIALOG_H
