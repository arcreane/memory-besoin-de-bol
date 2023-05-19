
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
    // Start the timer
    timer->start(1000);
}

void MainWindow::init()
{
    timer->stop();
    ui->label_3->setText("");
    timing();
    int cnt;
    if(ui->theme->checkedButton()->text() == "Bière")
        theme = 2;
    else if(ui->theme->checkedButton()->text() == "Soft")
        theme = 1;
    else if(ui->theme->checkedButton()->text() == "Eau")
        theme = 0;
    else
        QMessageBox::information(this,"ERROR", "une erreur est survenue sur le groupe de bouton THEME");

    if(ui->difficult->checkedButton()->text() == "Difficile")
    {
        reset();
        cnt = 0;
        for(int i = 0; i < 32; i++)
        {
            QPushButton *card = new QPushButton(tr("card_%1").arg(i+1));
            if(i%8 == 0 && i != 0)
                cnt++;
            ui->gridLayout_2->addWidget(card, cnt, i-8*cnt);
            connect(card, &QPushButton::clicked, this, [=](){
                card->setText("");
                card->setIcon(QIcon(QPixmap(":/Image/biere/b1.png")));
                card->setIconSize(QSize(100,100));
                win();
            });
        }
    }
    else if(ui->difficult->checkedButton()->text() == "Moyen")
    {
        cnt = 0;
        reset();
        for (int i = 0; i < 16; i++)
        {
            QPushButton* card = new QPushButton(tr("card_ %1").arg(i + 1));
            if (i % 8 == 0 && i != 0)
                cnt++;
            ui->gridLayout_2->addWidget(card, cnt, i - 8 * cnt);
            connect(card, &QPushButton::clicked, this, [=]() {
                    card->setText("");
                    card->setIcon(QIcon(QPixmap(":/Image/biere/b1.png")));
                    card->setIconSize(QSize(100, 100));
                    win();
            });
        }
    }
    else if(ui->difficult->checkedButton()->text() == "Facile")
    {
        cnt = 0;
        reset();
        for(int i = 0; i < 8; i++)
        {
            QPushButton *card = new QPushButton(tr("card_%1").arg(i+1));
            if(i%8 == 0 && i != 0)
                cnt++;
            ui->gridLayout_2->addWidget(card, cnt, i-8*cnt);
            connect(card, &QPushButton::clicked, this, [=](){
                card->setText("");
                card->setIcon(QIcon(QPixmap(":/Image/biere/b1.png")));
                card->setIconSize(QSize(100,100));
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

void MainWindow::link_card(std::list<std::pair<int,int>> link[1])
{
    std::list<int> disponibles;

    // Remplir la liste des nombres disponibles avec les entiers de 1 à 16
    for (int i = 1; i <= 16; i++)
    {
        disponibles.push_back(i);
    }

    // Créer un générateur de nombres aléatoires
    std::random_device rd;
    std::mt19937 rng(rd());

    // Mélanger la liste des nombres disponibles de manière aléatoire
    std::vector<int> disponiblesVec(disponibles.begin(), disponibles.end());
    std::shuffle(disponiblesVec.begin(), disponiblesVec.end(), rng);
    disponibles.assign(disponiblesVec.begin(), disponiblesVec.end());

    // Créer les 8 listes avec les paires d'entiers
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (disponibles.empty())
            {
                break; // Sortir de la boucle si la liste est vide
            }

            int premier = disponibles.front();
            disponibles.pop_front();

            if (disponibles.empty())
            {
                break; // Sortir de la boucle si la liste est vide
            }

            int deuxieme = disponibles.front();
            disponibles.pop_front();

            link[1].push_back(std::make_pair(premier, deuxieme));
        }

        // Réinitialiser la liste des nombres disponibles pour la prochaine liste
        disponiblesVec.assign(disponibles.begin(), disponibles.end());
        std::shuffle(disponiblesVec.begin(), disponiblesVec.end(), rng);
        disponibles.assign(disponiblesVec.begin(), disponiblesVec.end());
    }
}

bool MainWindow::pair_compare(int pos){
    if (this->p_compare.first != 0 && this->p_compare.second != 0){
        this->p_compare.first = 0;
        this->p_compare.second = 0;
    }
    else if (this->p_compare.first != 0)
        this->p_compare.second = pos;
    else if (this->p_compare.second != 0)
    this->p_compare.second = pos;

    for(const auto& paire : links) {
     if (paire == this->p_compare)
         return true;
     else
         return false;

    }
    return false;
    }

void MainWindow::win()
{
    bool win = true;
    QGridLayout* gridLayout = ui->gridLayout_2; // Replace 'gridLayout_2' with your actual grid layout object

    QList<QPushButton*> buttonList;

    for (int i = 0; i < gridLayout->count(); ++i) {
        QLayoutItem* item = gridLayout->itemAt(i);
        if (item) {
            if (QPushButton* button = qobject_cast<QPushButton*>(item->widget())) {
                // Found a QPushButton in the grid layout
                buttonList.append(button);
                // Perform any desired operations on the button
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

