#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QColor>
#include <QPalette>
#include <QThread>
#include <QDebug>
#include <QHostAddress>
MainWindow::MainWindow(char *argv[], QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStringList lst = QString(argv[0]).split("/");
    client =  new Client(lst[lst.size()-1]);
    btnInfoLst = new QVector<buttons*>;
    xmlPath = defaultXMLPath;
    readXML();

}

MainWindow::~MainWindow()
{
    delete ui;
}

//Процедура обработки изменения цвета кнопок
void MainWindow::function()
{
    QTimer* timer = static_cast<QTimer *>(QObject::sender());

    //Получения индекса кнопки массиве
    int i;
    for(i = 0; i < btnInfoLst->count() ; i++)
    {
        if(btnInfoLst->at(i)->tmr->timerId() == timer->timerId())
            break;
    }
    if(i==btnInfoLst->count())return;


    QHostAddress ip(btnInfoLst->at(i)->ip);
    QPalette myPalette =  btnInfoLst->at(i)->btn->palette();
    QDateTime* lastDtm = client->lastMessageTime(ip);
    int interval;
    if(lastDtm!=nullptr) interval = QDateTime::currentDateTime().toMSecsSinceEpoch() - lastDtm->toMSecsSinceEpoch();
    //Изменение цвета кнопки
    if(lastDtm==nullptr?true:interval>60000)
        myPalette.setColor(  btnInfoLst->at(i)->btn->backgroundRole(), QColor(137,129,118) );
    else if(interval>20000)
        myPalette.setColor(  btnInfoLst->at(i)->btn->backgroundRole(), QColor(250,0,0) );
    else
        myPalette.setColor(  btnInfoLst->at(i)->btn->backgroundRole(), QColor(0,250,0) );
    btnInfoLst->at(i)->btn->setPalette( myPalette );

}

//Чтение xml
void MainWindow::readXML()
{
    QFile file(xmlPath);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QXmlStreamReader xmlReader;
        xmlReader.setDevice(&file);
        while(!xmlReader.atEnd())
        {
            if(xmlReader.name() == "catalog" && xmlReader.isStartElement())
            {
                foreach(const QXmlStreamAttribute &attr, xmlReader.attributes())
                {
                    if(attr.name().toString() == "width")
                        this->setFixedWidth(attr.value().toInt());
                    else if(attr.name().toString() == "height")
                        this->setFixedHeight(attr.value().toInt());
                }

                while(!(xmlReader.name() == "catalog" && xmlReader.isEndElement()))
                {
                    if(xmlReader.name() == "object")
                    {
                        int x,y,xsize,ysize;
                        QString name, ip;
                        while(!(xmlReader.name() == "object" && xmlReader.isEndElement()))
                        {
                            if(xmlReader.isStartElement())
                            {
                                if(xmlReader.name() == "x")
                                    x = xmlReader.readElementText().toInt();
                                else if(xmlReader.name() == "y")
                                    y = xmlReader.readElementText().toInt();
                                else if(xmlReader.name() == "xsize")
                                    xsize = xmlReader.readElementText().toInt();
                                else if(xmlReader.name() == "ysize")
                                    ysize = xmlReader.readElementText().toInt();
                                else if(xmlReader.name() == "name")
                                    name = xmlReader.readElementText();
                                else if(xmlReader.name() == "ip")
                                    ip = xmlReader.readElementText();
                            }
                            xmlReader.readNext();
                        }
                        //QMessageBox::warning(this,"",ip+"");
                        buttons* strt = new buttons;
                        strt->btn = new QPushButton(ui->centralwidget);
                        strt->btn->setGeometry(x,y,xsize,ysize);
                        strt->btn->setText(name + " " + ip);
                        strt->tmr = new QTimer;
                        strt->tmr->setInterval(2000);
                        connect(strt->tmr, SIGNAL(timeout()), this, SLOT(function()));
                        strt->tmr->start();
                        strt->ip = ip;
                        strt->name = name;
                        btnInfoLst->push_back(strt);
                    }
                    xmlReader.readNext();
                }
                xmlReader.readNext();
            }
            xmlReader.readNext();
        }
    }
    else
    {
        QMessageBox::warning(this,"","файл не открывается");
    }
}
