#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QPushButton"
#include "QMessageBox"
#include "QPainter"
#include "QKeyEvent"
#include <vector>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget* wdg = new QWidget(this);
    button_new_game = new QPushButton("New Game", wdg);
    setCentralWidget(wdg);
    connect(button_new_game, SIGNAL(clicked(bool)), this, SLOT(button_new_event()));
    matrix.resize(3, std::vector<int>(3, 0));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::button_new_event()
{
    score1 = 0;
    score2 = 0;
    new_game();
    update();
}

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    for(int i = 0; i <= matrix.size(); ++i)
        for(int j = 0; j <= matrix.size(); ++j)
        {
            p.setPen(Qt::black);
            p.drawLine(start_pos + i * pixel_size, start_pos, start_pos + i*pixel_size, start_pos + matrix.size() * pixel_size);
            p.drawLine(start_pos, start_pos + j * pixel_size, start_pos + matrix.size() * pixel_size, start_pos + j * pixel_size);
            //if(matrix[i][j] == 1)
            if(i != matrix.size() && j != matrix.size())
            {
                if(matrix[i][j] == 1)
                {
                    p.setPen(Qt::red);
                    p.drawLine(start_pos + i * pixel_size, start_pos + j * pixel_size, start_pos + (i + 1) * pixel_size, start_pos + (j + 1) * pixel_size);
                    p.drawLine(start_pos + i * pixel_size, start_pos + (j + 1) * pixel_size, start_pos + (i + 1) * pixel_size, start_pos + j * pixel_size);
                }
                else if(matrix[i][j] == 2)
                {
                    p.setPen(Qt::blue);
                    p.drawEllipse(start_pos + i * pixel_size, start_pos + j * pixel_size, pixel_size, pixel_size);
                }
            }
        }
    p.drawText(start_pos + matrix.size() * pixel_size / 2, start_pos + (matrix.size() + 1) * pixel_size, QString("X:%1 | O:%2").arg(score1).arg(score2));
}

void MainWindow::mousePressEvent(QMouseEvent* e)
{
    if(e->pos().x() > start_pos && e->pos().x() < start_pos + matrix.size() * pixel_size &&
       e->pos().y() > start_pos && e->pos().y() < start_pos + matrix.size() * pixel_size)
    {
        int i = (e->pos().x() - start_pos) / pixel_size,
            j = (e->pos().y() - start_pos) / pixel_size;
        place(i, j);
    }
    update();
}

void MainWindow::new_game()
{
    for(int i = 0; i < matrix.size(); ++i)
        for(int j = 0; j < matrix.size(); ++j)
            matrix[i][j] = 0;
    turn = 1;
}

void MainWindow::place(int i, int j)
{
    if(matrix[i][j] != 0)
        return;
    matrix[i][j] = turn;
    if(check())
    {
        QString text = QString("Player %1 WON!").arg(turn);
        QMessageBox::warning(this, "GAME OVER", text);
        add_score();
        new_game();
        return;
    }
    change_turn();
}

void MainWindow::add_score()
{
    if(turn == 1)
        ++score1;
    else
        ++score2;
}

void MainWindow::change_turn()
{
    turn = (turn == 1 ? 2 : 1);
}

bool MainWindow::check()
{
    for(int i = 0; i < matrix.size(); ++i)
        for(int j = 0; j < matrix.size(); ++j)
        {
            if(check_row(i, j) ||
               check_col(i, j) ||
               check_diag(i, j))
                return true;
        }
    return false;
}

bool MainWindow::check_row(int i, int j)
{
    return i + 2 < matrix.size() && matrix[i][j] != 0 && matrix[i][j] == matrix[i+1][j] && matrix[i][j] == matrix[i+2][j];
}

bool MainWindow::check_col(int i, int j)
{
    return j + 2 < matrix.size() && matrix[i][j] != 0 && matrix[i][j] == matrix[i][j+1] && matrix[i][j] == matrix[i][j+2];
}

bool MainWindow::check_diag(int i, int j)
{
    if(matrix[i][j] != 0)
    {
        if(i == 0 && j == 0)
            return matrix[i][j] == matrix[i+1][j+1] && matrix[i][j] == matrix[i+2][j+2];
        else if(i == 0 && j == matrix.size() - 1)
            return matrix[i][j] == matrix[i+1][j-1] && matrix[i][j] == matrix[i+2][j-2];
    }
    return false;
}
