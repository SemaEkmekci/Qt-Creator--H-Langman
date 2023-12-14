#include "word.h"
//#include <fstream>
//#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

Letter::Letter(QWidget *parrent):QLabel(parrent)
{
    isItTrue = false;

}

void Letter::fileReadJson()
{
    QFile file(":/new/files/info.json");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (!doc.isNull()){
           jsonObject = doc.object();
            qDebug() << "Json";
           keys = jsonObject.keys();
        }
        else
        { qDebug() << "JSON dosyası ayrıştırılamadı.";}
    }
    else
    {
        qDebug() << "JSON dosyası açılamadı.";
    }
}

QStringList Letter::randomLang()
{
    QStringList questionInfo;
    int control = 1; //Aynı kelime tekrarı

    while(control){

   if(keys.size() != 0)
    {      qDebug() << "Randoom" << keys.size();
           int randomIndex = rand() % keys.size();
           randomKey = keys[randomIndex];
           qDebug() << "Key" << keys.size();
           if(keys.contains(randomKey))
           {
               control = 0;
               QJsonObject info = jsonObject[randomKey].toObject();
               int id = info["id"].toInt();
               QString clue = info["clue"].toString();
               QString infoText = info["info"].toString();

               questionInfo.append(randomKey);
               questionInfo.append(clue);
               questionInfo.append(infoText);

               qDebug() << "Key: " << randomKey;
               qDebug() << "ID: " << id;
               qDebug() << "Clue: " << clue;
               qDebug() << "Info: " << infoText;
               keys.removeOne(randomKey);
               qDebug() << "SONRAKİ KEY" << keys.size();

           }
    }
    else if(keys.size() == 0){ //keys.size() = 0 ise oyun kazanılmıştır.
           qDebug() << "Tebrikler Kazandınız";
           questionInfo.append("KAZANDINIIZ!!!");
           return questionInfo;

    }
    }
    return questionInfo;
}




