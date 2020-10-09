#include "console.h"
#include "mainwindow.h"
#include <QDebug>
#include <QScrollBar>
#include <QBitArray>
#include <bitset>
#include <iostream>
#include <QCharRef>

Console::Console(QWidget *parent) :
    QPlainTextEdit(parent)
{
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::white);
    p.setColor(QPalette::Text, Qt::black);
    setPalette(p);
}

void Console::putData(const QByteArray &data, char encoding)
{

    if (encoding == 'H'){
       insertPlainText(data.toHex() + 'h');
    }
    if (encoding == 'A'){
       insertPlainText(data);
    }
    if (encoding == 'B'){
       QString bin;
       std::bitset<8> x(data.toInt());
       for (int i = 8; i>0;i--)
       {
            bin.append(QString::number(int(x[i])));
       }
       qDebug() << bin;
       insertPlainText(bin+'b');
    }
}


void Console::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Backspace:
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Up:
    case Qt::Key_Down:
        break;
    default:
        QPlainTextEdit::keyPressEvent(e);
        emit getData(e->text().toLocal8Bit());
    }
}
