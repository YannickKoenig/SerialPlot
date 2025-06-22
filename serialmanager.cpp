#include "serialmanager.h"
#include <QRandomGenerator>

SerialManager::SerialManager(MainWindow* mw, QCustomPlot *newPlot) : fileTypeRegex(R"(.*\.([^.]+)$)"){
    mainWindow = mw;
    port = nullptr;
    plot = newPlot;
    initializePlot();

    xRange = 10;
    yRange = 10;
    autoRangeEnabled = true;
    //time = QDateTime::currentSecsSinceEpoch();
    time = 0;
    timer.start();

}

void SerialManager::setRange(double newXRange, double newYRange)
{
    xRange = newXRange;
    yRange = newYRange;
}

void SerialManager::setAutoRange(bool value)
{
    autoRangeEnabled = value;
}

void SerialManager::refreshPorts()
{
    mainWindow->resetCombobox();
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &portInfo : serialPortInfos) {
        portList.append(QString(portInfo.portName()));
        mainWindow->addComboboxEntry(QString(portInfo.portName() + " " + portInfo.manufacturer()  + " "  +portInfo.description()), QVariant(portInfo.portName()));
    }

}
void SerialManager::initializePort(int portIndex, int baud)
{
    if(port != nullptr) // Close open port
    {
        port->close();
    }

    QString portName = portList.at(portIndex);
    mainWindow->setWindowTitle("SerialPlot @ " + portName);
    port = new QSerialPort(mainWindow);
    port->setPortName(portName);
    port->setBaudRate(QSerialPort::Baud115200);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);
    if (port->open(QIODevice::ReadWrite)) {
        qDebug() << "Serial port opened successfully.";
    } else {
        QMessageBox::critical(mainWindow, "Error", "An error occurred while opening the port: " + port->errorString());
    }
    MainWindow::connect(port, &QSerialPort::readyRead, mainWindow, [=]() {
        QByteArray data = port->readAll();
        qDebug() << "Data received:" << data;
        QString dataString = QString(data);



        if(dataString == "\r\n" || dataString == "" || dataString == "\n"){
            return;
        } else if (!dataString.contains("\r\n")) {
             if(!dataString.contains("\r")) {
                dataString.append("\r");
             }
            if(!dataString.contains("\n")) {
                dataString.append("\n");
            }
        }
        dataString = dataString.replace("\r", "");
        QStringList elements = dataString.split(",");
        std::array<double, 15> dataArr;

        for(int i = 0; i<15; i++) {
            if (i >= elements.size()) break;
            dataArr[i] = elements.at(i).toDouble();
            if(i > n_channels) {
                n_channels = i;
                plot->addGraph();
                plot->graph(n_channels)->setPen(initializePen(n_channels));
            }
        }

        mainWindow->addToMonitor(dataString);
        qDebug() << "nch: " << n_channels <<"\n";
        mainWindow->setLastPacketText("Last data: " + dataString);
        updatePlot(dataArr, timer.elapsed());
        timer.restart();
    });

}

void SerialManager::updatePlot(std::array<double, 15> dataArr, int deltaT) {

    time += deltaT;
    double time_s = time /1000.0;

    for(int i = 0; i < (n_channels+1); i++) {

        plot->graph(i)->addData(time_s, dataArr.at(i));
        if(autoRangeEnabled)
        {
            plot->graph(i)->rescaleAxes(true);
        } else {
            plot->yAxis->setRange(-yRange, yRange);
            plot->xAxis->setRange(time_s-xRange, time_s);
        }
    }

    plot->replot();
    time_s++;
}



QPen SerialManager::initializePen(int channel) {
    QPen pen;
    pens[channel] = pen;
    pen.setWidth(5);
    pen.setColor(QColor(QRandomGenerator::global()->bounded(256),
                        QRandomGenerator::global()->bounded(128),
                        QRandomGenerator::global()->bounded(52)));
    return pen;

}

void SerialManager::initializePlot()
{

    plot->addGraph();
    plot->graph(0)->setPen(initializePen(0));
    plot->xAxis->setLabel("t in s");
    plot->yAxis->setLabel("y");


}

void SerialManager::setBaudRate(int baud)
{
    baudRate = baud;
    initializePort(selectedIndex, baud);

}



void SerialManager::setPortChanged(int newIndex) {
    selectedIndex = newIndex;
    initializePort(newIndex, baudRate);
}

FileType SerialManager::getFileType(QString fileName) {

    QRegularExpressionMatch match = fileTypeRegex.match(fileName);
    if(!match.hasMatch())
    {

        return FileType::VOID;
    }
    QString matchStr = match.captured(1);
    if(matchStr == "pdf") {
        return FileType::PDF;
    } else     if(matchStr == "png") {
        return FileType::PNG;
    } else     if(matchStr == "bmp") {
        return FileType::BMP;
    } else    if(matchStr == "jpg") {
        return FileType::JPG;
    } else {
        return FileType::VOID;
    }

}


void SerialManager::saveImage(QString fileName, FileType fileType)
{
    switch(fileType)
    {
        case FileType::BMP: {
            plot->saveBmp(fileName);
            break;
        }

        case FileType::PDF: {
            plot->savePdf(fileName);
            break;
        }

        case FileType::PNG: {
            plot->savePng(fileName);
            break;
        }

        case FileType::JPG: {
            plot->saveJpg(fileName);
            break;
        }

    }
}


