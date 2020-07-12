#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client.h"
#include <QPushButton>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>
#include <QFile>
#define defaultXMLPath "test.xml"

//Структура данных кнопок
struct buttons
{
    //Кнопка
    QPushButton* btn = nullptr;
    //Таймер для обновления
    QTimer* tmr = nullptr;
    //Ip
    QString ip;
    //Название
    QString name;
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(char *argv[], QWidget *parent = nullptr);
    ~MainWindow();
signals:

public slots:
    //Процедура обработки изменения цвета кнопок
    void function();

private:
    //Путь к xml файлу
    QString xmlPath;
    //Создание клиента
    Client* client = nullptr;
    Ui::MainWindow *ui;
    //Вектор кнопок
    QVector<buttons*>* btnInfoLst;
    //Процедура считывания xml файла
    void readXML();
};
#endif // MAINWINDOW_H
