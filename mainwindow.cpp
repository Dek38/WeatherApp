#include "mainwindow.h"
#include "ui_mainwindow.h"


class CurrentWeather
{

private:
    int m_cityId;
    int m_tempr;
    int m_humd;
    int m_windSpeed;
    std::time_t m_timeStamp;
public:
    CurrentWeather();
    ~CurrentWeather();


};




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QString val;

    ui->setupUi(this);
    connect (this, &MainWindow::quitSignal, this, &MainWindow::on_actionQuit_triggered);

    try
    {
        QFile file;
        file.setFileName("city.list.json");
        if (!file.open(QIODevice::ReadOnly))
        {
            throw(QString("Can't open file"));
        }
        val = file.readAll();
        file.close();
    }
    catch(QString mess)
    {
        qDebug() << mess;
        return;
    }
    {
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());

    if (d.isArray())
    {
        qDebug() << "!!!!";
       QJsonArray ja = d.array();
       QJsonObject subtree;
       QString str;
       for (int i = 0; i < ja.size(); i++)
       {
           subtree = ja.at(i).toObject();
           //if (subtree.value("country").toString() == "RU")
           {
               str = subtree.value("name").toString() + " " + QString::number(subtree.value("id").toInt());
                //qDebug() << str;
                ui->cityListWidget->addItem(str);
           }
       }
    }
    }
    qDebug()<<ui->cityListWidget->count();
    m_TempImage = QPixmap("WeatherIcon\\celsius.png");
    m_TempImage = m_TempImage.scaled(100, 100);
    ui->TempLabel->setPixmap(m_TempImage);

    m_HumdImage = QPixmap("WeatherIcon\\humidity.png");
    m_HumdImage = m_HumdImage.scaled(100, 100);
    ui->HumidLabel->setPixmap(m_HumdImage);

    m_WindSpeedImage = QPixmap("WeatherIcon\\windsock.png");
    m_WindSpeedImage = m_WindSpeedImage.scaled(100, 100);
    ui->WindSpeedLabel->setPixmap(m_WindSpeedImage);

    for (unsigned char i = 0; i < 6; i++)
    {
        m_LabelsForDate[i].setPixmap(m_LettersOfDate[i]);
        ui->dateLayout->addWidget(&m_LabelsForDate[i]);
    }

    for (unsigned char i = 0; i < 8; i++)
    {
        m_LabelsForTime[i].setPixmap(m_LettersOfTime[i]);
        ui->timeLayout->addWidget(&m_LabelsForTime[i]);
    }

    val.resize(1);

    manager = new QNetworkAccessManager(this);

    timer = new QTimer(this);
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::managerFinished);
    connect(timer, &QTimer::timeout, this , &MainWindow::slotTimerTimeout);

    request.setUrl(QUrl("http://api.openweathermap.org/data/2.5/weather?id=3333218&mode=JSON&units=metric&appid=" + USER_ID + "&lang=ru"));
    manager->get(request);
    timer->start(1000);

}

void MainWindow::managerFinished(QNetworkReply *reply)
{
    if (reply->error())
    {
        qDebug() << reply->errorString();
        return;
    }
    else
    {
        QString answer = reply->readAll();
        //qDebug() << answer;
        QJsonDocument d = QJsonDocument::fromJson(answer.toUtf8());
        QJsonObject subtree = d.object();
        ui->currentCity->setText(subtree.value("name").toString());
        subtree = subtree.value("main").toObject();
        //qDebug() << (subtree.value("temp").toDouble())

        ui->Temp->setText(QString::number(subtree.value("temp").toDouble()));
        ui->Humd->setText(QString::number(subtree.value("humidity").toDouble()));
        ui->WindSpeed->setText(QString::number(d.object().value("wind").toObject().value("speed").toDouble()));
        QJsonArray ja = d.object().value("weather").toArray();
        QString switchStr = ja.at(0).toObject().value("icon").toString();

        if (switchStr == "01d")
        {
            m_CurrentWeather = QPixmap("WeatherIcon\\sun.png");
        }
        else if (switchStr == "01n")
        {
            m_CurrentWeather = QPixmap("WeatherIcon\\moon.png");
        }
        else if (switchStr == "02d")
        {
            m_CurrentWeather = QPixmap("WeatherIcon\\cloudy-day.png");
        }
        else if (switchStr == "02n")
        {
            m_CurrentWeather = QPixmap("WeatherIcon\\cloudy-night.png");
        }
        else if (switchStr == "03d" || switchStr == "03n")
        {
            m_CurrentWeather = QPixmap("WeatherIcon\\cloud.png");
        }
        else if (switchStr == "04d" || switchStr == "04n")
        {
            m_CurrentWeather = QPixmap("WeatherIcon\\cloud(1).png");
        }
        else if (switchStr == "09d" || switchStr == "09n")
        {
            m_CurrentWeather = QPixmap("WeatherIcon\\rain.png");
        }
        else if (switchStr == "10d" || switchStr == "10n")
        {
            m_CurrentWeather = QPixmap("WeatherIcon\\weather.png");
        }
        else if (switchStr == "11d" || switchStr == "11n")
        {
            m_CurrentWeather = QPixmap("WeatherIcon\\thunderstorm.png");
        }
        else if (switchStr == "13d" || switchStr == "13n")
        {
            m_CurrentWeather = QPixmap("WeatherIcon\\snow.png");
        }
        else if (switchStr == "50d" || switchStr == "50n")
        {
            m_CurrentWeather = QPixmap("WeatherIcon\\wind.png");
        }
        m_CurrentWeather = m_CurrentWeather.scaled(100, 100);
        ui->currentWeather->setText(ja.at(0).toObject().value("description").toString());
        ui->currentWeather->setPixmap(m_CurrentWeather);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::TimeResolver(char charOfCurrentTime, QString& result)
{
    if ((charOfCurrentTime != ':') && (charOfCurrentTime != ' '))
    {
        result = "WeatherIcon\\" + QString(charOfCurrentTime) + ".png";
    }
    else if (charOfCurrentTime == ':')
    {
        result = "WeatherIcon\\colon.png";
    }
    else
    {
        result = "";
    }
}


void MainWindow::slotTimerTimeout()
{
    static unsigned char count = 0;
    if (count > 10)
    {
        request.setUrl(QUrl("http://api.openweathermap.org/data/2.5/weather?id=" + m_cityId + "&mode=JSON&units=metric&appid=" + USER_ID + "&lang=ru"));
        manager->get(request);
        count = 0;
    }
    else
    {
        count++;
    }

    std::time_t result = std::time(nullptr);
    std::string currentTime (std::ctime(&result));

    for (unsigned char i = 4; i < 19; i++)
    {
        QString str;
        TimeResolver(currentTime[i], str);
        if ((i >= 4) && (i < 10))
        {
            unsigned char j = i - 4;
            if (str == "")
            {
                m_LettersOfTime[j] = QPixmap();
            }
            else
            {
                m_LettersOfDate[j] = QPixmap(str).scaled(25, 25);
            }
            m_LabelsForDate[j].setPixmap(m_LettersOfDate[j]);

        }
        else if ((i >= 11) && (i < 19))
        {
            unsigned char j = i - 11;

            m_LettersOfTime[j] = QPixmap(str).scaled(25, 25);
            m_LabelsForTime[j].setPixmap(m_LettersOfTime[j]);
        }
    }

}



void MainWindow::on_actionQuit_triggered()
{
    qApp->quit();
}

void MainWindow::on_cityListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    timer->stop();
    int begin = 0;
    QString str = item->text();
    do
    {
        begin = str.indexOf(" ");
        str = str.mid(begin + 1, str.size());
        qDebug() << str.toInt();
    }
    while (str[0].isDigit() == false);

    m_cityId = str;

    ui->currentCity->setText(item->text());
    request.setUrl(QUrl("http://api.openweathermap.org/data/2.5/weather?id=" + m_cityId + "&mode=JSON&units=metric&appid=" + USER_ID + "&lang=ru"));
    manager->get(request);
    timer->start(1000);

}

