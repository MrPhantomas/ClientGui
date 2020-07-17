#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QColor>
#include <QPalette>
#include <QThread>
#include <QDebug>
#include <QHostAddress>
#include <QVBoxLayout>

MainWindow::MainWindow(char *argv[], QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStringList lst = QString(argv[0]).split("/");
    client =  new Client(lst[lst.size()-1]);
    btnInfoLst = new QVector<buttons*>;
    xmlPath = defaultXMLPath;
    //readXML();

    QWidget* widjet = new QWidget(ui->centralwidget);
    widjet->setGeometry(20,20,150,150);
    widjet->setStyleSheet("background-color:rgb(0,0,250);");

    QVBoxLayout* vLayout = new QVBoxLayout(widjet);
    //QLabel* lbl_1 = new QLabel(widjet->layout());


    QPushButton* btn_1 = new QPushButton(widjet);
    btn_1->setGeometry(10,10,30,40);
    btn_1->setStyleSheet("background-color:rgb(250,0,0);");
    QPushButton* btn_2 = new QPushButton(widjet);
    btn_2->setGeometry(10,10,30,40);
    btn_2->setStyleSheet("background-color:rgb(250,0,0);");
    QPushButton* btn_3 = new QPushButton(widjet);
    btn_3->setGeometry(10,10,30,40);
    btn_3->setStyleSheet("background-color:rgb(250,0,0);");




    QWidget* widjet2 = new QWidget(ui->centralwidget);
    widjet2->setGeometry(190,20,150,150);
    widjet2->setStyleSheet("background-color:rgb(0,0,250);");
    QPushButton* btn2 = new QPushButton(widjet2);
    btn2->setGeometry(10,10,30,40);
    btn2->setStyleSheet("background-color:rgb(250,0,0);");
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

    QLabel* test = static_cast<QLabel *>(ui->widgettst->children().at(4));
    test->setText("12345");
    test->setStyleSheet("background-color:rgb(0,250,0); \n border-radius:5px;");
    QLabel* test1 = static_cast<QLabel *>(ui->widgettst->children().at(3));
    test1->setText("12345");
    test1->setStyleSheet("background-color:rgb(0,0,250); \n border-radius:5px;");
    QLabel* test2 = static_cast<QLabel *>(ui->widgettst->children().at(2));
    test2->setText("12345");
    test2->setStyleSheet("background-color:rgb(250,0,0); \n border-radius:5px;");
    ui->widgettst->setStyleSheet("background-color:rgb(127,127,127); \n border-radius:5px;");
    //QPalette tstPalette =  test->palette();
    //tstPalette.setColor(test->backgroundRole(), QColor(0,250,0));
    //tstPalette.setColor(test->foregroundRole(), QColor(0,250,0));
    //test->setPalette(tstPalette);
    //ui->widgettst->setStyleSheet("background-color:rgb(0,250,0);");
    //ui->widgettst->setStyleSheet("border-radius:5px");
    //QPalette tstPalette2 =  ui->widgettst->palette();
    //tstPalette2.setColor(QPalette::Base, QColor(250,250,0));
    //ui->widgettst->setPalette(tstPalette2);

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

                        //QWidget* widjet = new QWidget;
                        //widjet->setGeometry(x,y,xsize,ysize);
                        //widjet->layout()->addWidget(new QPushButton);
                        //new QPushButton(widjet->window());
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
