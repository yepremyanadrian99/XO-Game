#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    int pixel_size = 50;
    int start_pos = 80;
    int turn = 1;
    int score1 = 0, score2 = 0;

    std::vector<std::vector<int>> matrix;
    QPushButton* button_new_game;

private slots:
    void button_new_event();

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);

    void new_game();

    void place(int, int);
    void change_turn();
    void add_score();
    bool check();
    bool check_row(int, int);
    bool check_col(int, int);
    bool check_diag(int, int);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
