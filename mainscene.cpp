#include "mainscene.h"
#include "./ui_mainscene.h"
#include<QIcon>
#include<QPainter>
#include "mypushbutton.h"
#include<QDebug>
#include<QTimer>
MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);

    //配置主场景

    //设置固定大小
    setFixedSize(320,588);

    //设置图标
    setWindowIcon(QIcon(":/res/Coin0001.png"));

    //设置标题
    setWindowTitle("翻金币主场景");

    //退出按钮实现
    connect(ui->actionquit,&QAction::triggered,this,[=](){
        this->close();
    });

    //开始按钮
    MyPushButton * startBtn = new MyPushButton(":/res/MenuSceneStartButton.png");
    startBtn->setParent(this);
    startBtn->move(this->width() * 0.5 - startBtn->width() * 0.5,this->height() * 0.7);

    //实例化选择关卡场景
    chooseScene = new ChooseLevelScene;

    //监听选择关卡的返回按钮的信号
    connect(chooseScene,&ChooseLevelScene::chooseSceneBack,this,[=](){
        //将选择关卡场景隐藏掉
        chooseScene->hide();
        //重新显示主场景
        this->show();
    });

    connect(startBtn,&QPushButton::clicked,this,[=](){
        //qDebug()<<"点击了开始";
        //做一个弹跳特效
        startBtn->zoom1();
        startBtn->zoom2();

        //延时进入到选择关卡场景中
        QTimer::singleShot(500,this,[=](){
            //进入选择关卡场景中
            //自身隐藏
            this->hide();
            //显示选择关卡场景
            chooseScene->show();
        });


    });

}

//重写paintEvent事件 画背景图
void MainScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //画背景图标
    pix.load(":/res/Title.png");

    //缩放图标
    pix = pix.scaled(pix.width()*0.5,pix.height()*0.5);

    painter.drawPixmap(10,30,pix);
}




MainScene::~MainScene()
{
    delete ui;
}

