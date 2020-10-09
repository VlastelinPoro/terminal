#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

void MainWindow::createwidgets ( )
{
    QGridLayout *lCalcLayout = new QGridLayout;
    setLayout ( lCalcLayout );
    pushButtonConnect = new QPushButton ( "connect" );
    pushButtonDisconnect = new QPushButton ( "disconnect" );
    pushButtonDisconnect->setEnabled(false);

    label1 = new QLabel ("COM name");
    label2 = new QLabel ("Baud rate");
    label3 = new QLabel ("Data bits");
    label4 = new QLabel ("Stop bits");
    label5 = new QLabel ("Encoding");

    Comname = new QComboBox ();
    Baudrate = new QComboBox ();
    Stopbits = new QComboBox ();
    Databits = new QComboBox ();
    Encoding = new QComboBox ();

    Comname->addItem(QStringLiteral("COM1"));
    Comname->addItem(QStringLiteral("COM2"));
    Comname->addItem(QStringLiteral("COM3"));
    Comname->addItem(QStringLiteral("COM4"));

    Baudrate->addItem(QStringLiteral("2400"), QSerialPort::Baud2400);
    Baudrate->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);

    Databits->addItem(QStringLiteral("5"), QSerialPort::Data5);
    Databits->addItem(QStringLiteral("6"), QSerialPort::Data6);
    Databits->addItem(QStringLiteral("7"), QSerialPort::Data7);
    Databits->addItem(QStringLiteral("8"), QSerialPort::Data8);
    Databits->setCurrentIndex(3);

    Stopbits->addItem(QStringLiteral("1"), QSerialPort::OneStop);
    Stopbits->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    Encoding->addItem(QStringLiteral("ASCII"));
    Encoding->addItem(QStringLiteral("BIN"));
    Encoding->addItem(QStringLiteral("HEX"));

    lCalcLayout->addWidget ( label1, 0 , 0);
    lCalcLayout->addWidget ( label2, 0 , 1);
    lCalcLayout->addWidget ( label3, 0 , 2);
    lCalcLayout->addWidget ( label4, 0 , 3);
    lCalcLayout->addWidget ( label5, 0 , 4);
    lCalcLayout->addWidget ( Comname, 1 , 0);
    lCalcLayout->addWidget ( Baudrate, 1 , 1);
    lCalcLayout->addWidget ( Stopbits, 1 , 2);
    lCalcLayout->addWidget ( Databits, 1 , 3);
    lCalcLayout->addWidget ( Encoding, 1 , 4);
    lCalcLayout->addWidget ( pushButtonConnect, 1 , 6);
    lCalcLayout->addWidget ( pushButtonDisconnect, 1 , 7);
    lCalcLayout->addWidget( m_console, 2 , 0 , 3 , 8);
}

MainWindow::MainWindow ( QWidget * parent ) :
    QWidget ( parent ),
    port(new QSerialPort),
    m_console(new Console)
{
    setWindowTitle ( " Terminal MIEM edition " );
    createwidgets( );
    connect(port, &QSerialPort::readyRead, this, &MainWindow::readData);
    connect(m_console, &Console::getData, this, &MainWindow::writeData);
    connect(pushButtonConnect, &QPushButton::clicked, this, &MainWindow::OpenPort);
    connect(pushButtonDisconnect, &QPushButton::clicked, this, &MainWindow::ClosePort);
}

MainWindow::~MainWindow()
{

}


void MainWindow::writeData(const QByteArray &data)
{
    port->write(data);
}

void MainWindow::readData()
{
    const QByteArray data = port->readAll();
    char encoding;
    QString encod = Encoding->currentText();
    if (encod == "ASCII") encoding = 'A';
    if (encod == "HEX") encoding = 'H';
    if (encod == "BIN") encoding = 'B';
    m_console->putData(data,encoding);
}

void MainWindow::ClosePort()
{
    if (port->isOpen())
        port->close();
    pushButtonConnect->setEnabled(true);
    pushButtonDisconnect->setEnabled(false);
    Stopbits->setEnabled(true);
    Baudrate->setEnabled(true);
    Comname->setEnabled(true);
    Databits->setEnabled(true);
    m_console->setEnabled(false);
}

void MainWindow::OpenPort()
{
    port->setPortName(Comname->currentText());
    port->setBaudRate(Baudrate->itemData(Baudrate->currentIndex()).toInt());
    port->setDataBits(static_cast<QSerialPort::DataBits>(Databits->itemData(Databits->currentIndex()).toInt()));
    port->setStopBits(static_cast<QSerialPort::StopBits>(Stopbits->itemData(Stopbits->currentIndex()).toInt()));
    port->open(QSerialPort::ReadWrite);
    pushButtonConnect->setEnabled(false);
    pushButtonDisconnect->setEnabled(true);
    Stopbits->setEnabled(false);
    Baudrate->setEnabled(false);
    Comname->setEnabled(false);
    Databits->setEnabled(false);
    m_console->setEnabled(true);
}
