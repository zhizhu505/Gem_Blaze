#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QIcon>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleButtonClick();  // 处理按钮点击事件

private:
    void initializeBoard();  // 初始化棋盘
    void setupUI();          // 设置UI
    void updateUI();         // 更新UI
    void swapGems(int row1, int col1, int row2, int col2);  // 交换宝石

    Ui::MainWindow *ui;
    std::vector<std::vector<QPushButton*>> buttons;
    int board[7][7];  // 7x7 棋盘
    const int ROWS = 7;
    const int COLS = 7;

    // 用于记录用户选择的第一个和第二个宝石的位置
    bool firstSelected;
    int firstRow;
    int firstCol;
};

#endif // MAINWINDOW_H
