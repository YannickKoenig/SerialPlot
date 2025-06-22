#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "serialmanager.h"
#include <QFileDialog>

SerialManager *serialManager;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serialManager = new SerialManager(this, ui->widget);
    serialManager->refreshPorts();
    initializeBaudComboBox();
    initializeSliders();


}
void MainWindow::initializeSliders()
{
    ui->xRangeSlider->setVisible(false);
    ui->yRangeSlider->setVisible(false);
    ui->xRangeLabel->setVisible(false);
    ui->yRangeLabel->setVisible(false);



}

void MainWindow::initializeBaudComboBox() {
    ui->baudComboBox->addItem("115200 Baud", 115200);
    ui->baudComboBox->addItem("57600 Baud", 57600);
    ui->baudComboBox->addItem("38400 Baud", 38400);
    ui->baudComboBox->addItem("19200 Baud", 19200);
    ui->baudComboBox->addItem("9600 Baud", 9600);
    ui->baudComboBox->addItem("4800 Baud", 4800);
    ui->baudComboBox->addItem("2400 Baud", 2400);
    ui->baudComboBox->addItem("1200 Baud", 1200);

}

void MainWindow::resetCombobox()
{
    ui->comboBox->clear();
}

void MainWindow::addComboboxEntry(QString key, QVariant value)
{
    ui->comboBox->addItem(key, value);

}

void MainWindow::addToMonitor(QString string) {
    ui->textBrowser->append(string);
}
void MainWindow::setLastPacketText(QString string) {
    ui->lastDataLabel->setText(string);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    serialManager->setPortChanged(index);
}


void MainWindow::on_baudComboBox_currentIndexChanged(int index)
{
    serialManager->setBaudRate(ui->baudComboBox->currentData().toInt());
}


void MainWindow::on_checkBox_checkStateChanged(const Qt::CheckState &arg1)
{

}


void MainWindow::on_autoRangeCheckBox_checkStateChanged(const Qt::CheckState &arg1)
{
    if(arg1==Qt::CheckState::Checked)
    {
        ui->xRangeSlider->setVisible(false);
        ui->yRangeSlider->setVisible(false);
        serialManager->setAutoRange(true);
        ui->xRangeLabel->setVisible(false);
        ui->yRangeLabel->setVisible(false);
    } else {

        ui->xRangeSlider->setVisible(true);
        ui->yRangeSlider->setVisible(true);
        serialManager->setAutoRange(false);
        ui->xRangeLabel->setVisible(true);
        ui->yRangeLabel->setVisible(true);
    }
}


void MainWindow::on_yRangeSlider_sliderMoved(int position)
{
    int xRange = ui->xRangeSlider->sliderPosition();
    serialManager->setRange(xRange*xRange*0.01, position*position*0.01);
    serialManager->setAutoRange(false);
}


void MainWindow::on_xRangeSlider_sliderMoved(int position)
{

    int yRange = ui->yRangeSlider->sliderPosition();
    serialManager->setRange((position*position*0.01), yRange*yRange*0.01);

    serialManager->setAutoRange(false);
}


void MainWindow::on_yRangeSlider_rangeChanged(int min, int max)
{

}

void MainWindow::on_pushButton_2_clicked(){

}


void MainWindow::on_exportImageButton_clicked()
{

    QString filename = QFileDialog::getSaveFileName(
        this,
        QObject::tr("Export Image"),
        QDir::currentPath()+"/plot.png",
        QObject::tr("PNG (*.png);; PDF (*.pdf);; BMP (*.bmp);; JPEG (*.jpg)"));

    FileType fileType = serialManager->getFileType(filename);
    if(fileType == FileType::VOID) {
        QMessageBox::critical(this, "Error", "No file type selected");
        return;
    }
    serialManager->saveImage(filename, fileType);
}

