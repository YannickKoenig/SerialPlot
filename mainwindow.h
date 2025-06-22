#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void addComboboxEntry(QString key, QVariant value);
    void resetCombobox();
    void addToMonitor(QString string);
    void setLastPacketText(QString string);
    void initializeBaudComboBox();
    void initializeSliders();

    ~MainWindow();

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_baudComboBox_currentIndexChanged(int index);

    void on_checkBox_checkStateChanged(const Qt::CheckState &arg1);

    void on_autoRangeCheckBox_checkStateChanged(const Qt::CheckState &arg1);

    void on_yRangeSlider_rangeChanged(int min, int max);

    void on_yRangeSlider_sliderMoved(int position);

    void on_xRangeSlider_sliderMoved(int position);

    void on_pushButton_2_clicked();

    void on_exportImageButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
