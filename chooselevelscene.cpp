#include "chooselevelscene.h"
#include<QMenuBar>
#include<QPainter>
#include "mypushbutton.h"
#include<QDebug>
#include<QTimer>
#include<QLabel>
#include "playscene.h"
ChooseLevelScene::ChooseLevelScene(QWidget *parent)
    : QMainWindow{parent}
{
    //配置选择关卡场景
    this->setFixedSize(320,588);

    //设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));

    //设置标题
    this->setWindowTitle("选择关卡场景");

    //创建菜单栏
    QMenuBar * bar = menuBar();
    setMenuBar(bar);

    //创建开始菜单
    QMenu * startMenu = bar->addMenu("开始");

    //创建退出 菜单项
    QAction * quitAction = startMenu->addAction("退出");

    //点击退出 实现退出游戏
    connect(quitAction,&QAction::triggered,this,[=](){
        this->close();
    });

    //返回按钮
    MyPushButton * backBtn = new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    //返回按钮放到窗口中
    backBtn->setParent(this);
    //返回按钮放到窗口右下角
    backBtn->move(this->width() - backBtn->width(),this->height() - backBtn->height());

    //点击返回 实现
    connect(backBtn,&QPushButton::clicked,this,[=](){
        //qDebug()<<"点击返回按钮";

        //延时返回
        QTimer::singleShot(500,this,[=](){
            //告诉主场景 我返回了，主场景监听ChooseLevelScene的返回按钮
            emit this->chooseSceneBack();
        });
    });


    //创建选择关卡的按钮
    for(int i=0;i<20;i++)
    {
        MyPushButton * menuBtn = new MyPushButton(":/res/LevelIcon.png");
        //将关卡选择按钮放到窗口中
        menuBtn->setParent(this);
        //将按钮移动到它应该在的位置
        menuBtn->move(25 + i%4 * 70,130 + i/4 * 70);

        //监听每个按钮的点击事件
        connect(menuBtn,&MyPushButton::clicked,this,[=](){
            QString str = QString("您选择的是第 %1 关").arg(i+1);
            qDebug()<<str;

            //进入到游戏场景
            this->hide();
            play = new PlayScene(i+1);//创建游戏场景
            play->show();//显示游戏场景
            connect(play,&PlayScene::chooseSceneBack,this,[=](){
                this->show();
                delete play;
                play = NULL;

            });
        });

        //按钮上显示文字 用显示文字的标签部件
        QLabel * label = new QLabel;
        label->setParent(this);
        //设置label大小
        label->setFixedSize(menuBtn->width(),menuBtn->height());
        label->setText(QString::number(i+1));
        label->move(25 + i%4 * 70,130 + i/4 * 70);
        //设置label上的文字对齐方式 设置水平居中和垂直居中
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        //设置让鼠标进行穿透
        label->setAttribute(Qt::WA_TransparentForMouseEvents);
    }



}

//重写绘图事件
void ChooseLevelScene::paintEvent(QPaintEvent *)
{
    //加载背景
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/OtherSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //加载标题图标
    pix.load(":/res/Title.png");
    painter.drawPixmap((this->width()-pix.width()) * 0.5,30,pix.width(),pix.height(),pix);
}






