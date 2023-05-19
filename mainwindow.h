
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QTimer>
#include <QTime>
#include <iostream>
#include <list>
#include <utility>
#include <algorithm>
#include <random>
#include <QDateTime>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool two_cards_reveal = false;
    std::list<std::pair<int,int>> links;
    QList<QLabel> cards;
    std::pair<int,int> pair_compare;
    int theme;
    int difficulty;
    void reset();
    void win();
    void resetButton(QPushButton* button);
    void link_card(int nb_card);
    void updateTimer();
    void stok(int pos);
    bool paire_compare();

public slots:
    void init();
    void timing();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QTime currentTime;
};

#endif // MAINWINDOW_H
