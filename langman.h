#ifndef LANGMAN_H
#define LANGMAN_H

#include <QMainWindow>
#include <QTimer>
#include <QtMultimedia>
#include <QAudioOutput>
#include <QPushButton>
#include "word.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Langman; }
QT_END_NAMESPACE

class Langman : public QMainWindow
{
    Q_OBJECT

public:
    Langman(QWidget *parent = nullptr);
    ~Langman();


private slots:
    void backgroundSound();
    void startGameBtn_Clicked();
    void startGameBtn_2_Clicked();
    void howToPlayBtn_Clicked();
    void backBtn_Clicked();
    void nextLevelBtnClicked();
    void buttonHover();
    void volumeBtn();
    void sarkiBitti();
    void backBtn();
    void randomUserName();
    void rightGuessVoice();
    void wrongGuessVoice();
    void toWinVoice();
    void toLoseVoice();
    void trueWordVoice();
    void trueWord();
    void keyPress();
    void keyPressEvent(QKeyEvent *event);
    void gameScreen();
    void getLetter();
    void toWinandtoLose();
    void showClue();
    void showClueLetter();
    void showInfo();
    void manDrawNotVisible();
    void manDraw(int count);


private:
    Ui::Langman *ui;
    QMediaPlayer *player; //Arka plan sesi için
    QMediaPlayer *player2; //Buton sesleri için
    QMediaPlayer *player3; //Klavye sesleri için
    QMediaPlayer *player4; //Game Over için
    QAudioOutput *audioOutput;
    QAudioOutput *audioOutput_2; //Arka plans sesi hariç diğer seslerin düzeyini ayarlamak için
    Letter *let; // Letter sınıfından nesne türetiyorum.
    QTimer *timer;
    QLabel *letter;
    QLabel *line;
    QStringList falseLetter; //Yanlış harfleri ekranda göstermek için
    QString falseLets = "";
    QString trueLets = "";
    int trueCount; //Kelime bilindi mi?
    int length; //Kelimenin uzunluğu
    bool isSoundOn = true;
    int clueCount = 2;
    int score = 0 ;
    int falseCount = 0; //Adam çizmek için yanlış harf sayacı
};
#endif // LANGMAN_H
