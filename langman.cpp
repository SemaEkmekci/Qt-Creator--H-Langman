#include "langman.h"
#include "ui_langman.h"
#include <QMessageBox>
#include <fstream>
#include <QHBoxLayout>
#include <QLabel>
#include <QStringList>

QVector<QLabel*> letterLabels;
QVector<QLabel*> lineLabels;
QStringList questionInfo;

Langman::Langman(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Langman)
{
    ui->setupUi(this);
    manDrawNotVisible();
    ui->stackedWidget->setCurrentIndex(0);  //Anasayfa
    backgroundSound();
    timer = new QTimer(this); //Arka planda çalan şarkı bitince tekrar dönsün diye oluşturdum.
    timer->start(100000);

    isSoundOn = true;


    connect(ui->startGame, &QPushButton::clicked, this, &Langman::startGameBtn_Clicked);
    connect(ui->howToPlay, &QPushButton::clicked, this, &Langman::howToPlayBtn_Clicked);
    connect(ui->backHowBtn, &QPushButton::clicked, this, &Langman::newGameBtn);
    connect(ui->backGameBtn, &QPushButton::clicked, this, &Langman::newGameBtn);
    connect(ui->startNewGame, &QPushButton::clicked, this, &Langman::newGameBtn);
    connect(ui->nextLevel, &QPushButton::clicked, this, &Langman::nextLevelBtnClicked);

    connect(ui->startGame_2, &QPushButton::clicked, this, &Langman::startGameBtn_2_Clicked);
    connect(ui->sugges, &QPushButton::clicked, this, &Langman::randomUserName);


    connect(ui->volumeBtn, &QPushButton::clicked, this, &Langman::volumeBtn);
    connect(ui->startGame, &QPushButton::clicked, this, &Langman::buttonHover);
    connect(ui->howToPlay, &QPushButton::clicked, this, &Langman::buttonHover);
    connect(ui->backHowBtn, &QPushButton::clicked, this, &Langman::buttonHover);
    connect(ui->backGameBtn, &QPushButton::clicked, this, &Langman::buttonHover);
    connect(ui->clue, &QPushButton::clicked, this, &Langman::buttonHover);
    //connect(ui->clue, &QPushButton::clicked, this, &Langman::showClue);

    connect(ui->backBtn, &QPushButton::clicked, this, &Langman::buttonHover);
    connect(ui->backBtn, &QPushButton::clicked, this, &Langman::backBtn);
   //connect(ui->backBtn, &QPushButton::clicked, this, &Langman::buttonHover);

    connect(ui->startGame_2, &QPushButton::clicked, this, &Langman::buttonHover);
    connect(ui->name, &QLineEdit::textChanged, this, &Langman::keyPress); //Oyuna başla sayfasındaki isim alanına isim girilirken klavye sesi çıkması için
    connect(ui->sugges, &QPushButton::clicked, this, &Langman::buttonHover);
    connect(timer, SIGNAL(timeout()),this,SLOT(sarkiBitti()));
}

Langman::~Langman()
{
    delete ui;
}

void Langman::backgroundSound()
{
    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.3);
    player->setSource(QUrl("qrc:/new/media/spook.mp3"));
    player->play();
    audioOutput_2 = new QAudioOutput;
    audioOutput_2->setVolume(1);
}

void Langman::startGameBtn_Clicked()
{
    ui->warning->setText("");
    ui->name->setText("Adınızı Giriniz");
    ui->stackedWidget->setCurrentIndex(1); //Oyuna Giriş Ekranı
    ui->win->setVisible(false);

}

void Langman::startGameBtn_2_Clicked()
{
    qDebug() << "text:" << ui->name->text();
    if(ui->name->text() == "" || ui->name->text() == "Adınızı Giriniz") //isim kontrolü
    {
        ui->warning->setText("Bu İsim Geçerli Değil ❗");
    }
    else if(ui->name->text().length() > 20)
    {
        ui->warning->setText("İsim 20 karakterden\nfazla olamaz ❗");
    }

    else
    {
        qDebug() << "text:" << ui->name->text();
        ui->nameLabel->setText("Oyuncu: " + ui->name->text());
        ui->clueInfo->setText("");
        ui->score->setText("Skor: 0");
        score = 0;
        if(clueCount != 0){
            ui->clue->setDisabled(false);}
        else{
            ui->clue->setDisabled(true);
        }

        ui->stackedWidget->setCurrentIndex(2); //Oyun Ekranı
        gameScreen(); //Letter sınıfına gidip json dosyasından veri alıyorum.
        getLetter();
    }

}

void Langman::howToPlayBtn_Clicked()
{
    ui->stackedWidget->setCurrentIndex(5); //Nasıl Oynanır Ekranı
}

void Langman::newGameBtn()
{
    ui->stackedWidget->setCurrentIndex(0); //Anasayfa
}

void Langman::nextLevelBtnClicked()
{

   /* trueLets=""; //Oyunda kullanılan harfleri sıfırlıyorum. Çünkü oyun yeniden açılmış gibi oluyor.
    ui->word->setText("");
    for (QLabel* label : letterLabels) //Kelime ile işim bittiğinde listemi temizliyorum.
    {
        delete label;
    }
    qDebug() << "AAAAAAA ";

    for (QLabel* label : lineLabels) //Kelime ile işim bittiğinde listemi temizliyorum.
    {
        delete label;
    }
    lineLabels.clear();
    falseLetter.clear();
    letterLabels.clear();*/
    qDebug() << "BBBBBBB ";
    ui->stackedWidget->setCurrentIndex(2);
    getLetter();
}

void Langman::buttonHover()
{
    player2 = new QMediaPlayer;
    //audioOutput = new QAudioOutput;
    player2->setAudioOutput(audioOutput_2);
    player2->setSource(QUrl("qrc:/new/media/mouse-click.mp3"));
    //audioOutput->setVolume(50);
    player2->play();
}

void Langman::volumeBtn()
{
    if (isSoundOn) {
        player->stop();
        ui->volumeBtn->setIcon(QIcon(":/new/image/volumeDisabled.png"));
        isSoundOn = false;
    } else {
        player->play();
        ui->volumeBtn->setIcon(QIcon(":/new/image/volume.png"));
        isSoundOn = true;
    }
}


void Langman::sarkiBitti()
{

    if (isSoundOn) {
        qDebug() << "aaaaaaa: ";
        //player->stop();
       // player = new QMediaPlayer;
       // player->setAudioOutput(audioOutput);
        //player->setSource(QUrl("qrc:/new/media/spook.mp3"));
       // audioOutput->setVolume(50);
       // player->play();
        player->stop();
        player->setPosition(0);
        player->play();
    }
}



void Langman::backBtn()
{

    if (QMessageBox::question(this, "", "Oyundan çekilmek istiyor musunuz?", QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
        {
        ui->stackedWidget->setCurrentIndex(0);
        /*clueCount = 2;
        for (QLabel* label : letterLabels) //Kelime ile işim bittiğinde listemi temizliyorum.
        {
            delete label;
        }
        letterLabels.clear();
        for (QLabel* label : lineLabels) //Kelime ile işim bittiğinde listemi temizliyorum.
        {
            delete label;
        }
        lineLabels.clear();
        falseLetter.clear();
        trueLets=""; //Oyunda kullanılan harfleri sıfırlıyorum. Çünkü oyun yeniden açılmış gibi oluyor.
        ui->word->setText("");
        manDrawNotVisible(); // Adamı ekranda görünmez hale getiriyorum.*/
        gameReset();
    }
}

void Langman::randomUserName()
{

        QFile readFile(":/new/files/userName.txt");
        if(readFile.open(QIODevice::ReadOnly | QIODevice::Text)){
            int lineNum = 1;
            int random = rand() % 25 + 1;  //1 ile 25 arasında bir sayı üretiyorum. Çünkü isim dosyasında 25 satır var.
            QTextStream in(&readFile);
            QString line;
            while(!in.atEnd()){   // Bu döngü random üretilen sayısının olduğu satıra gelince sonlanacak.
                line = in.readLine();
                lineNum++;

                if(lineNum == random)
                {
                    break;
                }
            }
            readFile.close();
            qDebug() << "line: " << line;
            ui->name->setText(line);
            readFile.close();

        }
        else
        {
            qDebug() << "Üzgünüm isim yok";
        }
}

void Langman::rightGuessVoice()
{
        player3 = new QMediaPlayer;
        player3->setAudioOutput(audioOutput_2);
        player3->setSource(QUrl("qrc:/new/media/correct-choice.mp3"));
        // audioOutput->setVolume(50);
        player3->play();


}

void Langman::wrongGuessVoice()
{
        player3 = new QMediaPlayer;
        player3->setAudioOutput(audioOutput_2);
        player3->setSource(QUrl("qrc:/new/media/negative_beeps.mp3"));
        // audioOutput->setVolume(50);
        player3->play();
}

void Langman::toWinVoice()
{
        player3 = new QMediaPlayer;
        player3->setAudioOutput(audioOutput_2);
        player3->setSource(QUrl("qrc:/new/media/success-trumpets.mp3"));
        //audioOutput_2->setVolume(50);
        player3->play();
}

void Langman::toLoseVoice()
{
        player3 = new QMediaPlayer;
        player3->setAudioOutput(audioOutput_2);
        player3->setSource(QUrl("qrc:/new/media/wahwahsadtrombone.mp3"));
        //audioOutput_2->setVolume(50);
        player3->play();
}

void Langman::trueWordVoice()
{
        player3 = new QMediaPlayer;
        player3->setAudioOutput(audioOutput_2);
        player3->setSource(QUrl("qrc:/new/media/success.mp3"));
        //audioOutput_2->setVolume(50);
        player3->play();
}



void Langman::trueWord()
{
    trueWordVoice();
    QTimer::singleShot(500, [=]() {  //Ekranda girilen son harfi de bir süre görebilmek için biraz bekletiyorum.
    ui->stackedWidget->setCurrentIndex(3);
    trueLets=""; //Oyunda kullanılan harfleri sıfırlıyorum. Çünkü oyun yeniden açılmış gibi oluyor.
    ui->word->setText("");

    for (QLabel* label : lineLabels) //Kelime ile işim bittiğinde listemi temizliyorum.
    {
            delete label;
    }
    for (QLabel* label : letterLabels) //Kelime ile işim bittiğinde listemi temizliyorum.
    {
            delete label;
    }
    qDebug() << "AAAAAAA ";
    lineLabels.clear();
    falseLetter.clear();
    letterLabels.clear();
    showInfo();

    });

}

void Langman::keyPress() //Klavye tuşları sesleri için
{
        qDebug() << "Tuşa: basıldı: ";
        player3 = new QMediaPlayer;
        player3->setAudioOutput(audioOutput_2);
        player3->setSource(QUrl("qrc:/new/media/mech-keyboard.mp3"));
       // audioOutput->setVolume(50);
        player3->play();
}




void Langman::keyPressEvent(QKeyEvent *event)
{

        int currentPageIndex = ui->stackedWidget->currentIndex();

        falseLets = "";

        qDebug() << "Tuş" << currentPageIndex;
        if (currentPageIndex == 2) //Klavyeden sadece harfler alınacak
        {
            if(trueCount == length)
            {   //Eğer kelime bilindiyse fonksiyondan çık.
                return;
            }
            if(event->text().length() == 1 && (event->text().at(0).isLetter() || event->text() == '#' || event->text() == '+' ) ){
            keyPress(); //Klavye'den sadece harfler, '+' ve '#' karakterini almak için
            bool isFound = questionInfo[0].contains(event->text().toUpper());
            qDebug() << "isFound" << isFound;

            if (isFound) {

                qDebug() << "Girilen Harf: " <<event->text();
                if(!(trueLets.contains(event->text().toUpper()))){
                rightGuessVoice();
                score += 10;
                trueLets+=event->text().toUpper();
                ui->score->setText("Skor: " + QString::number(score));
                for (int i = 0; i < letterLabels.size(); i++)
                {
                    if (questionInfo[0][i] == event->text().toUpper())
                    {
                        letterLabels[i]->setVisible(true);
                        trueCount++;

                        qDebug() <<"trueCount" <<trueCount;
                        if(trueCount == length)
                        {

                            trueWord();
                            return;
                        }
                    }
                }}
            }else if(!(falseLetter.contains(event->text().toUpper()))) //Kullanılan harf tekrar kullanılmaması için
            {
                falseCount++;
                qDebug() << "falseCount: " << falseCount;
                if(falseCount == 12) //Oyun bitti KAYBEDİLDİ.
                {
                    toLoseVoice(); //Kaybedilirse bu ses çalcak
                    ui->info_2->setText(questionInfo[2]); //Son kelimenin bilgisi ekranda belirecek.
                    ui->score_2->setText("Skor: " + QString::number(score)); // Skor bilgisi ekranda belirecek.
                    ui->stackedWidget->setCurrentIndex(4);
                    gameReset();
                    return;
                }

                wrongGuessVoice();
                score-=2;
                ui->score->setText("Skor: " + QString::number(score));
                falseLetter.append(event->text().toUpper());
                for (QString falseLet : falseLetter)
                {
                    falseLets += " " + falseLet;
                }
                ui->word->setText(falseLets);
                if(falseCount <= 12){ //Kullanıcının 12 defa hata yapma hakkı var.
                manDraw(falseCount);
                }
            }
            }
            }
}


void Langman::gameScreen()
{
    let = new Letter(this);
    let->fileReadJson();
}

void Langman::getLetter()
{
    qDebug() << "Çalıştı";
    questionInfo = let->randomLang();
    qDebug() << "Çalıştı2";
    for (QLabel* label : letterLabels)  //kelime ile işim bittiğinde listemi temizliyorum ki ekranda görünmesin ve crashed olmasın.
    {
        delete label;
    }
    letterLabels.clear();
    if(questionInfo.size() == 1){ //Eğer questionInfo boyutu 1 ' e eşitse yarışma kazanılmıştır.
            toWin();
        return;
    }
    trueCount = 0;
    length = questionInfo[0].length();
    int geometry = 100;
    int geometryLine = 100;
    for (int i = 0; i < length; i++)
    {
            qDebug() << i;
            //QString labelName = "letter_" + QString::number(length);
            letter = new QLabel(this); 
            letter->setGeometry(geometry, 300, 100, 100);
            geometry+=60;
            letter->setText(questionInfo[0][i]);
            letter->setStyleSheet("color:white; font-family: 'Papyrus', monospace; font-size: 40px;");
            letter->font();
            layout()->addWidget(letter);
            letter->setVisible(false);
            letterLabels.append(letter);

            line = new QLabel(this);
            line->setStyleSheet("border-image:url(:/new/image/line.png);");
            line->setGeometry(geometryLine, 380, 50, 30);
            geometryLine+=60;
            lineLabels.append(line);
            layout()->addWidget(line);
    }

    showClue();
}

void Langman::toWin()
{
     qDebug() << "Çalıştı";
     questionInfo = let->randomLang();
     qDebug() << "Çalıştı2";
     ui->clueInfo->setText(""); //Oyun kazanıldıktan veya kaybedildikten sonra Ekranda ipucu görünmeyecek
     /*for (QLabel* label : letterLabels)  //kelime ile işim bittiğinde listemi temizliyorum ki ekranda görünmesin ve crashed olmasın.
     {
        delete label;
     }
     letterLabels.clear();*/

     length = questionInfo[0].length();
     int geometry = 100;
     int geometryLine = 100;
     for (int i = 0; i < length; i++)
     {
        qDebug() << i;
        //QString labelName = "letter_" + QString::number(length);
        letter = new QLabel(this);
        letter->setGeometry(geometry, 300, 100, 100);
        geometry+=60;
        letter->setText(questionInfo[0][i]);
        letter->setStyleSheet("color:white; font-family: 'Papyrus', monospace; font-size: 40px;");
        letter->font();
        layout()->addWidget(letter);
        letterLabels.append(letter);

        line = new QLabel(this);
        line->setStyleSheet("border-image:url(:/new/image/line.png);");
        line->setGeometry(geometryLine, 380, 50, 30);
        geometryLine+=60;
        lineLabels.append(line);
        layout()->addWidget(line);
     }
     if(questionInfo[0] == "KAZANDINIIZ!!!"){
        ui->win->setVisible(false);
        toWinVoice();

     }
     /*else{
        toLoseVoice(); //Kaybedilirse bu ses çalcak
     }*/
}

void Langman::showClue()
{
    ui->clueInfo->setText(questionInfo[1]);
}

void Langman::showClueLetter()
{
    ui->clue->setDisabled(true);
    clueCount--;
    qDebug() << clueCount;
}

void Langman::showInfo()
{
    ui->info->setText(questionInfo[2]);
}

void Langman::manDrawNotVisible()
{
    falseCount = 0;
    ui->man_1->setVisible(false);
    ui->man_2->setVisible(false);
    ui->man_3->setVisible(false);
    ui->man_4->setVisible(false);
    ui->man_5->setVisible(false);
    ui->man_6->setVisible(false);
    ui->man_7->setVisible(false);
    ui->man_8->setVisible(false);
    ui->man_9->setVisible(false);
    ui->man_10->setVisible(false);
    ui->man_11->setVisible(false);
    ui->man_12->setVisible(false);
    ui->eyes->setVisible(false);
    ui->mouth->setVisible(false);
}

void Langman::manDraw(int count)
{

    if(count == 1)
    {
        ui->man_1->setVisible(true);
    }
    else if(count == 2)
    {
        ui->man_2->setVisible(true);
    }
    else if(count == 3)
    {
        ui->man_3->setVisible(true);
    }
    else if(count == 4)
    {
        ui->man_4->setVisible(true);
    }
    else if(count == 5)
    {
        ui->man_5->setVisible(true);
    }
    else if(count == 6)
    {
        ui->man_6->setVisible(true);
    }
    else if(count == 7)
    {
        ui->man_7->setVisible(true);
        ui->eyes->setVisible(true);
        ui->mouth->setVisible(true);
    }
    else if(count == 8)
    {
        ui->man_8->setVisible(true);
    }
    else if(count == 9)
    {
        ui->man_9->setVisible(true);
    }
    else if(count == 10)
    {
        ui->man_10->setVisible(true);
    }
    else if(count == 11)
    {
        ui->man_11->setVisible(true);
    }
    else if(count == 12)
    {
        ui->man_12->setVisible(true);
    }

}

void Langman::gameReset()
{
    clueCount = 2;
    for (QLabel* label : letterLabels) //Kelime ile işim bittiğinde listemi temizliyorum.
    {
        delete label;
    }
    letterLabels.clear();
    for (QLabel* label : lineLabels) //Kelime ile işim bittiğinde listemi temizliyorum.
    {
        delete label;
    }
    lineLabels.clear();
    falseLetter.clear();
    trueLets=""; //Oyunda kullanılan harfleri sıfırlıyorum. Çünkü oyun yeniden açılmış gibi oluyor.
    ui->word->setText("");
    falseCount = 0;
    manDrawNotVisible(); // Adamı ekranda görünmez hale getiriyorum.

}
/*
void Langman::newGameBtn()
{
    ui->stackedWidget->setCurrentIndex(0);
}
*/





