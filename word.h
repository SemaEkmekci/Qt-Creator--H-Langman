#ifndef WORD_H
#define WORD_H

#include <QWidget>
#include <QLabel>
#include <QJsonObject>

class Letter : public QLabel
{
    Q_OBJECT
public:
    explicit Letter(QWidget *parent = 0);
    bool isItTrue;
public slots:
    void fileReadJson();

    QStringList randomLang();
private:
    QJsonObject jsonObject;
    QString randomKey;
    QStringList keys;
};

#endif // WORD_H
