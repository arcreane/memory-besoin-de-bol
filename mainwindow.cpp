
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this,SLOT(time()));
    timer->start(5000);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::updateTimer()
{
    currentTime = currentTime.addSecs(1);
    ui->label_2->setText(currentTime.toString("hh:mm:ss"));
}

void MainWindow::timing()
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);

    QFont font("Arial", 20);
    ui->label_2->setFont(font);
    ui->label_2->setText("00:00:00");

    currentTime.setHMS(0, 0, 0);

    timer->start(1000);
}

void MainWindow::init()
{
    timer->stop();
    ui->label_3->setText("");
    timing();
    qDebug() << this->links;
    int cnt;
    if(ui->theme->checkedButton()->text() == "Bière")
        theme = 2;
    else if(ui->theme->checkedButton()->text() == "Voiture")
        theme = 1;
    else if(ui->theme->checkedButton()->text() == "Eau")
        theme = 0;
    else
        QMessageBox::information(this,"ERROR", "une erreur est survenue sur le groupe de bouton THEME");

    if(ui->difficult->checkedButton()->text() == "Difficile")
    {
        reset();
        link_card(32);
        cnt = 0;

        for(int i = 0; i < 32; i++)
        {
            int target = i+1;
            auto foundPair = std::find_if(links.begin(), links.end(), [target](const std::pair<int, int>& pair) {
                return pair.first == target || pair.second == target;
            });
            std::size_t index = std::distance(links.begin(), foundPair);
            QPushButton *card = new QPushButton(tr("card_%1").arg(i+1));
            if(i%8 == 0 && i != 0)
                cnt++;
            ui->gridLayout_2->addWidget(card, cnt, i-8*cnt);
            connect(card, &QPushButton::clicked, this, [=](){
                card->setText("");
                QString s_img = "";
                if (this->theme == 0){
                    s_img = QStringLiteral(":/Image/eau/e%1.png").arg(index+1);
                }
                else if (this->theme == 1){
                    s_img = QStringLiteral(":/Image/voiture/v%1.png").arg(index+1);
                }
                else if (this->theme == 2){
                    s_img = QStringLiteral(":/Image/biere/b%1.png").arg(index+1);}
                card->setIcon(QIcon(QPixmap(s_img)));
                card->setIconSize(QSize(100,100));
                win();
            });
        }
    }
    else if(ui->difficult->checkedButton()->text() == "Moyen")
    {
        cnt = 0;

        reset();
        link_card(16);
        for (int i = 0; i < 16; i++)
        {
            int target = i+1;
            auto foundPair = std::find_if(links.begin(), links.end(), [target](const std::pair<int, int>& pair) {
                return pair.first == target || pair.second == target;
            });
            std::size_t index = std::distance(links.begin(), foundPair);

            QPushButton* card = new QPushButton(tr("card_ %1").arg(i + 1));
            if (i % 8 == 0 && i != 0)
                cnt++;
            ui->gridLayout_2->addWidget(card, cnt, i - 8 * cnt);
            connect(card, &QPushButton::clicked, this, [=]() {
                    card->setText("");
                QString s_img = "";
                if (this->theme == 0){
                    s_img = QStringLiteral(":/Image/eau/e%1.png").arg(index+1);
                }
                else if (this->theme == 1){
                    s_img = QStringLiteral(":/Image/voiture/v%1.png").arg(index+1);
                }
                else if (this->theme == 2){
                    s_img = QStringLiteral(":/Image/biere/b%1.png").arg(index+1);}
                card->setIcon(QIcon(QPixmap(s_img)));
                card->setIconSize(QSize(100, 100));
                win();
            });
        }
    }
    else if(ui->difficult->checkedButton()->text() == "Facile")
    {
        cnt = 0;

        reset();
        link_card(8);
        for(int i = 0; i < 8; i++)
        {
            int target = i+1;
            auto foundPair = std::find_if(links.begin(), links.end(), [target](const std::pair<int, int>& pair) {
                return pair.first == target || pair.second == target;
            });
            std::size_t index = std::distance(links.begin(), foundPair);

            QPushButton *card = new QPushButton(tr("card_%1").arg(i+1));
            if(i%8 == 0 && i != 0)
                cnt++;
            ui->gridLayout_2->addWidget(card, cnt, i-8*cnt);
            connect(card, &QPushButton::clicked, this, [=](){
                card->setText("");
                QString s_img = "";
                if (this->theme == 0){
                    s_img = QStringLiteral(":/Image/eau/e%1.png").arg(index+1);
                }
                else if (this->theme == 1){
                    s_img = QStringLiteral(":/Image/voiture/v%1.png").arg(index+1);
                }
                else if (this->theme == 2){
                    s_img = QStringLiteral(":/Image/biere/b%1.png").arg(index+1);
                }

                card->setIcon(QIcon(QPixmap(s_img)));
                card->setIconSize(QSize(100,100));
                this->stok(i+1);

                if (this->pair_compare.first != 0 && this->pair_compare.second != 0)
                    this->paire_compare();
                win();
            });
        }
    }
    else
        QMessageBox::information(this,"ERROR", "une erreur est survenue sur le groupe de bouton DIFFICULT");
}

void MainWindow::reset()
{
    QLayout* layout = ui->gridLayout_2->layout();
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        QWidget* widget = item->widget();
        if (widget != nullptr) {
            layout->removeWidget(widget);
            delete widget;
        }
        delete item;
    }
}

void MainWindow::link_card(int nb_card)
{
    std::list<int> disponibles;


    for (int i = 1; i <= nb_card; i++)
    {
        disponibles.push_back(i);
    }


    std::random_device rd;
    std::mt19937 rng(rd());


    std::vector<int> disponiblesVec(disponibles.begin(), disponibles.end());
    std::shuffle(disponiblesVec.begin(), disponiblesVec.end(), rng);
    disponibles.assign(disponiblesVec.begin(), disponiblesVec.end());


    for (int i = 0; i <= nb_card%8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (disponibles.empty())
            {
                break;
            }

            int premier = disponibles.front();
            disponibles.pop_front();

            if (disponibles.empty())
            {
                break;
            }

            int deuxieme = disponibles.front();
            disponibles.pop_front();

            this->links.push_back(std::make_pair(premier, deuxieme));
        }

        disponiblesVec.assign(disponibles.begin(), disponibles.end());
        std::shuffle(disponiblesVec.begin(), disponiblesVec.end(), rng);
        disponibles.assign(disponiblesVec.begin(), disponiblesVec.end());
    }
    for (const auto &paire : links)
    {
        qDebug() << "Premier élément : " << paire.first << ", Deuxième élément : " << paire.second;
    }
    std::cout << std::endl;
}

bool MainWindow::paire_compare(){

    for(const auto& paire : links) {
        if (paire == this->pair_compare)
        {
            this->pair_compare.first = 0;
            this->pair_compare.second = 0;
            return true;
        }
        else
        {
            this->pair_compare.first = 0;
            this->pair_compare.second = 0;
            return false;
        }
    }
    return false;
}

void MainWindow::stok(int pos)
{
    if (this->pair_compare.first == 0)
        this->pair_compare.first = pos;
    else if (this->pair_compare.second == 0)
        this->pair_compare.second = pos;
}

void MainWindow::win()
{
    bool win = true;
    QGridLayout* gridLayout = ui->gridLayout_2;

    QList<QPushButton*> buttonList;

    for (int i = 0; i < gridLayout->count(); ++i) {
        QLayoutItem* item = gridLayout->itemAt(i);
        if (item) {
            if (QPushButton* button = qobject_cast<QPushButton*>(item->widget())) {

                buttonList.append(button);

            }
        }
    }

    for(QPushButton* button : buttonList)
    {
        if(button->icon().isNull())
        {
            win = false;
            break;
        }
    }

    if(win == true)
    {
        timer->stop();
        QFont font("Arial", 40);
        ui->label_3->setFont(font);
        ui->label_3->setText("GAGNÉ !!!");
    }

}

