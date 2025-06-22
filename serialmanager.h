#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H
#include <QStringList>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include "mainwindow.h"
#include <qcustomplot.h>

enum FileType {
    PNG,
    PDF,
    BMP,
    JPG,
    VOID
};


class SerialManager
{
private:
    QElapsedTimer timer;
    QPen pens[15];
    QStringList portList;
    QSerialPort *port;
    int baudRate;
    int selectedIndex;
    MainWindow *mainWindow;
    QCustomPlot *plot;
    double xRange, yRange;
    bool autoRangeEnabled;

    int n_channels = {0};

    QRegularExpression fileTypeRegex;
    double time;
public:
    SerialManager(MainWindow *mw, QCustomPlot *newPlot);
    void refreshPorts();
    void initializePort(int portIndex, int baud);
    void setPortChanged(int newIndex);
    void updatePlot(std::array<double, 15> dataArr, int deltaT);
    void initializePlot();
    void setBaudRate(int baud);
    void setRange(double newXRange, double newYRange);
    void setAutoRange(bool value);
    void saveImage(QString fileName, FileType fileType);
    QPen initializePen(int channel);
    FileType getFileType(QString fileName);
};

#endif // SERIALMANAGER_H
