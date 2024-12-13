#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QIcon>
#include <QSize>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , firstSelected(false)  // 初始化选择状态为未选择
{
    ui->setupUi(this);

    // 初始化棋盘（通过数组初始化）
    initializeBoard();

    // 设置UI（创建按钮并添加到布局）
    setupUI();

    // 根据棋盘数据更新UI（显示宝石图标）
    updateUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeBoard() {
    // 使用固定数组初始化宝石类型
    int gemGrid[7][7] = {
        {1, 2, 3, 4, 5, 1, 2},
        {2, 3, 2, 5, 1, 2, 3},
        {3, 4, 4, 1, 2, 5, 4},
        {4, 5, 1, 2, 3, 4, 5},
        {1, 5, 4, 3, 2, 5, 1},
        {1, 2, 3, 4, 5, 1, 2},
        {2, 3, 4, 5, 1, 2, 3}
    };

    // 将 gemGrid 数组中的值复制到 board 数组中
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            board[row][col] = gemGrid[row][col];
        }
    }
}

void MainWindow::setupUI() {
    // 获取中央部件的布局，假设已经在 Qt Designer 中设置为 QGridLayout
    QGridLayout *gridLayout = qobject_cast<QGridLayout*>(ui->centralwidget->layout());
    if(!gridLayout) {
        // 如果没有设置为 QGridLayout，则创建一个
        gridLayout = new QGridLayout(ui->centralwidget);
        ui->centralwidget->setLayout(gridLayout);
    }

    // 调整布局间距和边距（可选）
    gridLayout->setSpacing(5);
    gridLayout->setContentsMargins(10, 10, 10, 10);

    // 初始化按钮二维数组
    buttons.resize(ROWS, std::vector<QPushButton*>(COLS, nullptr));

    // 创建按钮并添加到布局
    for(int row = 0; row < ROWS; ++row) {
        for(int col = 0; col < COLS; ++col) {
            QPushButton *button = new QPushButton(this);
            button->setFixedSize(70, 70); // 设置按钮大小
            button->setIconSize(QSize(60, 60)); // 设置图标大小
            button->setFlat(true); // 去除按钮边框

            // 去除焦点框
            button->setFocusPolicy(Qt::NoFocus);


            // 设置按钮的对象名称，便于后续识别
            button->setObjectName(QString("gemButton_%1_%2").arg(row).arg(col));

            // 连接按钮的点击信号到槽函数
            connect(button, &QPushButton::clicked, this, &MainWindow::handleButtonClick);

            // 将按钮添加到布局中
            gridLayout->addWidget(button, row, col);

            // 存储按钮指针
            buttons[row][col] = button;
        }
    }
}

void MainWindow::updateUI() {
    // 根据 board 数组更新UI显示宝石图标
    for(int row = 0; row < ROWS; ++row) {
        for(int col = 0; col < COLS; ++col) {
            int gemType = board[row][col];
            QString iconPath = QString(":/images/gem%1.jpg").arg(gemType);
            QIcon icon(iconPath);

            if (icon.isNull()) {
                qDebug() << "Failed to load icon:" << iconPath;
            }

            buttons[row][col]->setIcon(icon);
            //buttons[row][col]->setText(QString::number(gemType)); // 设置文本以确认变化
        }
    }
}

void MainWindow::handleButtonClick() {
    // 获取发送信号的按钮
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) {
        qDebug() << "Clicked button is null!";
        return;
    }

    // 获取按钮的对象名称，例如 "gemButton_2_3"
    QString objectName = clickedButton->objectName();
    QStringList parts = objectName.split("_");
    if (parts.size() != 3) {
        qDebug() << "Invalid object name:" << objectName;
        return;
    }

    bool ok1, ok2;
    int row = parts[1].toInt(&ok1);
    int col = parts[2].toInt(&ok2);
    if (!ok1 || !ok2) {
        qDebug() << "Invalid row or column in object name:" << objectName;
        return;
    }

    //qDebug() << "Button clicked at row:" << row << ", col:" << col;

    if (!firstSelected) {
        // 第一次点击，记录位置并高亮显示
        firstRow = row;
        firstCol = col;
        firstSelected = true;

        // 高亮第一个选择的按钮（例如，改变背景色）
        clickedButton->setStyleSheet("background-color: yellow;");
        //qDebug() << "First gem selected at (" << firstRow << "," << firstCol << ")";
    }
    else {
        // 第二次点击，记录位置并尝试交换
        int secondRow = row;
        int secondCol = col;

        // 如果点击的是同一个按钮，取消选择
        if (secondRow == firstRow && secondCol == firstCol) {
            buttons[firstRow][firstCol]->setStyleSheet(""); // 取消高亮
            firstSelected = false;
            //qDebug() << "Selection canceled. Same button clicked.";
            return;
        }

        //qDebug() << "Second gem selected at (" << secondRow << "," << secondCol << ")";

        // 交换宝石类型
        swapGems(firstRow, firstCol, secondRow, secondCol);

        // 更新UI
        updateUI();

        // 取消高亮
        buttons[firstRow][firstCol]->setStyleSheet("");
        firstSelected = false;
    }
}

void MainWindow::swapGems(int row1, int col1, int row2, int col2) {
    // 交换 board 数组中的宝石类型
    int temp = board[row1][col1];
    board[row1][col1] = board[row2][col2];
    board[row2][col2] = temp;

    // 打印交换后的棋盘状态（用于调试）
    //qDebug() << "Swapped (" << row1 << "," << col1 << ") with (" << row2 << "," << col2 << ")";
    /*for(int row = 0; row < ROWS; ++row) {
        QString rowStr;
        for(int col = 0; col < COLS; ++col) {
            rowStr += QString::number(board[row][col]) + " ";
        }
        qDebug() << rowStr;
    }*/
}
