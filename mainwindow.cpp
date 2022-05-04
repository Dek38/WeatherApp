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
    QFile file;
    file.setFileName("city.list.json");
    file.open(QIODevice::ReadOnly);
    val = file.readAll();
    file.close();
    {
    /*QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());

    if (d.isArray())
    {
        qDebug() << "!!!!";
       QJsonArray ja = d.array();
       QJsonObject subtree;
       QString str;
       for (int i = 0; i < 100; i++)
       {
           subtree = ja.at(i).toObject();
           //if (subtree.value("country").toString() == "RU")
           {
               str = subtree.value("name").toString() + " " + QString::number(subtree.value("id").toInt());
                //qDebug() << str;
                ui->cityListWidget->addItem(str);
           }
       }
    }*/
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
    ui->currentCity->setText("Murino 524311");

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
            m_CurrentWeather = m_CurrentWeather.scaled(100, 100);
            ui->currentWeather->setText(ja.at(0).toObject().value("description").toString());
            ui->currentWeather->setPixmap(m_CurrentWeather);
        }
        else if (switchStr == "01n")
        {
            m_CurrentWeather = QPixmap("WeatherIcon\\moon.png");
            m_CurrentWeather = m_CurrentWeather.scaled(100, 100);
            ui->currentWeather->setText(ja.at(0).toObject().value("description").toString());
            ui->currentWeather->setPixmap(m_CurrentWeather);
        }
        else if (switchStr == "02d")
        {
            m_CurrentWeather = QPixmap("WeatherIcon\\cloudy-day.png");
            m_CurrentWeather = m_CurrentWeather.scaled(100, 100);
            ui->currentWeather->setText(ja.at(0).toObject().value("description").toString());
            ui->currentWeather->setPixmap(m_CurrentWeather);
        }
        else if (switchStr == "02n")
        {
            m_CurrentWeather = QPixmap("WeatherIcon\\cloudy-night.png");
            m_CurrentWeather = m_CurrentWeather.scaled(100, 100);
            ui->currentWeather->setText(ja.at(0).toObject().value("description").toString());
            ui->currentWeather->setPixmap(m_CurrentWeather);
        }
        else if (switchStr == "03d" || switchStr == "03n")
        {
            m_CurrentWeather = QPixmap("WeatherIcon\\cloud.png");
            m_CurrentWeather = m_CurrentWeather.scaled(100, 100);
            ui->currentWeather->setText(ja.at(0).toObject().value("description").toString());
            ui->currentWeather->setPixmap(m_CurrentWeather);
        }
        else if (switchStr == "04d" || switchStr == "04n")
        {
            m_CurrentWeather = QPixmap("WeatherIcon\\cloud(1).png");
            m_CurrentWeather = m_CurrentWeather.scaled(100, 100);
            ui->currentWeather->setText(ja.at(0).toObject().value("description").toString());
            ui->currentWeather->setPixmap(m_CurrentWeather);
        }
        else if (switchStr == "09d" || switchStr == "09n")
        {
            m_CurrentWeather = QPixmap("WeatherIcon\\rain.png");
            m_CurrentWeather = m_CurrentWeather.scaled(100, 100);
            ui->currentWeather->setText(ja.at(0).toObject().value("description").toString());
            ui->currentWeather->setPixmap(m_CurrentWeather);
        }
        else if (switchStr == "10d" || switchStr == "10n")
        {
            m_CurrentWeather = QPixmap("WeatherIcon\\weather.png");
            m_CurrentWeather = m_CurrentWeather.scaled(100, 100);
            ui->currentWeather->setText(ja.at(0).toObject().value("description").toString());
            ui->currentWeather->setPixmap(m_CurrentWeather);
        }
        else if (switchStr == "11d" || switchStr == "11n")
        {
            m_CurrentWeather = QPixmap("WeatherIcon\\thunderstorm.png");
            m_CurrentWeather = m_CurrentWeather.scaled(100, 100);
            ui->currentWeather->setText(ja.at(0).toObject().value("description").toString());
            ui->currentWeather->setPixmap(m_CurrentWeather);
        }
        else if (switchStr == "13d" || switchStr == "13n")
        {
            m_CurrentWeather = QPixmap("WeatherIcon\\snow.png");
            m_CurrentWeather = m_CurrentWeather.scaled(100, 100);
            ui->currentWeather->setText(ja.at(0).toObject().value("description").toString());
            ui->currentWeather->setPixmap(m_CurrentWeather);
        }
        else if (switchStr == "50d" || switchStr == "50n")
        {
            m_CurrentWeather = QPixmap("WeatherIcon\\wind.png");
            m_CurrentWeather = m_CurrentWeather.scaled(100, 100);
            ui->currentWeather->setText(ja.at(0).toObject().value("description").toString());
            ui->currentWeather->setPixmap(m_CurrentWeather);
        }

    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::TimeResolver(char charOfCurrentTime, QString& result)
{

    switch(charOfCurrentTime)
    {
        case '0': result = "WeatherIcon\\zero.png"; break;
        case '1': result = "WeatherIcon\\number-1.png"; break;
        case '2': result = "WeatherIcon\\number-2.png"; break;
        case '3': result = "WeatherIcon\\number-3.png"; break;
        case '4': result = "WeatherIcon\\number-four.png"; break;
        case '5': result = "WeatherIcon\\number-5.png"; break;
        case '6': result = "WeatherIcon\\six.png"; break;
        case '7': result = "WeatherIcon\\seven.png"; break;
        case '8': result = "WeatherIcon\\number-8.png"; break;
        case '9': result = "WeatherIcon\\number-9.png"; break;
        case 'a': result = "WeatherIcon\\letter-a.png"; break;
        case 'b': result = "WeatherIcon\\letter-b.png"; break;
        case 'c': result = "WeatherIcon\\letter-c.png"; break;
        case 'd': result = "WeatherIcon\\letter-d.png"; break;
        case 'e': result = "WeatherIcon\\letter-e.png"; break;
        case 'f': result = "WeatherIcon\\letter-f.png"; break;
        case 'g': result = "WeatherIcon\\letter-g.png"; break;
        case 'h': result = "WeatherIcon\\letter-h.png"; break;
        case 'i': result = "WeatherIcon\\letter-i.png"; break;
        case 'j': result = "WeatherIcon\\letter-j.png"; break;
        case 'k': result = "WeatherIcon\\letter-k.png"; break;
        case 'l': result = "WeatherIcon\\letter-l.png"; break;
        case 'm': result = "WeatherIcon\\letter-m.png"; break;
        case 'n': result = "WeatherIcon\\letter-n.png"; break;
        case 'o': result = "WeatherIcon\\letter-o.png"; break;
        case 'p': result = "WeatherIcon\\letter-p.png"; break;
        case 'q': result = "WeatherIcon\\letter-q.png"; break;
        case 'r': result = "WeatherIcon\\letter-r.png"; break;
        case 's': result = "WeatherIcon\\letter-s.png"; break;
        case 't': result = "WeatherIcon\\letter-t.png"; break;
        case 'u': result = "WeatherIcon\\letter-u.png"; break;
        case 'v': result = "WeatherIcon\\letter-v.png"; break;
        case 'w': result = "WeatherIcon\\letter-w.png"; break;
        case 'x': result = "WeatherIcon\\letter-x.png"; break;
        case 'y': result = "WeatherIcon\\letter-y.png"; break;
        case 'z': result = "WeatherIcon\\letter-z.png"; break;
        case 'A': result = "WeatherIcon\\letter-a.png"; break;
        case 'B': result = "WeatherIcon\\letter-b.png"; break;
        case 'C': result = "WeatherIcon\\letter-c.png"; break;
        case 'D': result = "WeatherIcon\\letter-d.png"; break;
        case 'E': result = "WeatherIcon\\letter-e.png"; break;
        case 'F': result = "WeatherIcon\\letter-f.png"; break;
        case 'G': result = "WeatherIcon\\letter-g.png"; break;
        case 'H': result = "WeatherIcon\\letter-h.png"; break;
        case 'I': result = "WeatherIcon\\letter-i.png"; break;
        case 'J': result = "WeatherIcon\\letter-j.png"; break;
        case 'K': result = "WeatherIcon\\letter-k.png"; break;
        case 'L': result = "WeatherIcon\\letter-l.png"; break;
        case 'M': result = "WeatherIcon\\letter-m.png"; break;
        case 'N': result = "WeatherIcon\\letter-n.png"; break;
        case 'O': result = "WeatherIcon\\letter-o.png"; break;
        case 'P': result = "WeatherIcon\\letter-p.png"; break;
        case 'Q': result = "WeatherIcon\\letter-q.png"; break;
        case 'R': result = "WeatherIcon\\letter-r.png"; break;
        case 'S': result = "WeatherIcon\\letter-s.png"; break;
        case 'T': result = "WeatherIcon\\letter-t.png"; break;
        case 'U': result = "WeatherIcon\\letter-u.png"; break;
        case 'V': result = "WeatherIcon\\letter-v.png"; break;
        case 'W': result = "WeatherIcon\\letter-w.png"; break;
        case 'X': result = "WeatherIcon\\letter-x.png"; break;
        case 'Y': result = "WeatherIcon\\letter-y.png"; break;
        case 'Z': result = "WeatherIcon\\letter-z.png"; break;
        case ':': result = "WeatherIcon\\colon.png"; break;
        default: result = ""; break;
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


    ui->lastUpdate->setText(std::ctime(&result));

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
    std::time_t result = std::time(nullptr);
    ui->lastUpdate->setText(std::ctime(&result));
    request.setUrl(QUrl("http://api.openweathermap.org/data/2.5/weather?id=" + m_cityId + "&mode=JSON&units=metric&appid=" + USER_ID + "&lang=ru"));
    manager->get(request);
    timer->start(1000);

}

