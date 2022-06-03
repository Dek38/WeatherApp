#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QListWidgetItem>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPixmap>
#include <QTimer>
#include <ctime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QJsonArray>
#include <csignal>
#include <QLabel>

#include "userSettings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();




private slots:
    void on_actionQuit_triggered();
    void slotTimerTimeout();
    void on_cityListWidget_itemDoubleClicked(QListWidgetItem *item);
    void managerFinished(QNetworkReply *reply);

private:
    QPixmap m_TempImage;
    QPixmap m_HumdImage;
    QPixmap m_WindSpeedImage;
    QPixmap m_CurrentWeather;
    QPixmap m_LettersOfTime [8];
    QPixmap m_LettersOfDate [6];
    QLabel m_LabelsForTime [8];
    QLabel m_LabelsForDate [6];


    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QByteArray buffer;
    QString m_cityId = "3333218";
    Ui::MainWindow *ui;
    QTimer *timerConnectTimeout;
    QTimer *timer;

    void TimeResolver(char charOfCurrentTime, QString& result);

signals:
    void quitSignal();
};
#endif // MAINWINDOW_H
