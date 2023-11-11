#include "playscene.h"
#include<QDebug>
#include<QMenuBar>
#include<QPainter>
#include "mypushbutton.h"
#include<QTimer>
#include<QLabel>
#include "mycoin.h"
#include "dataconfig.h"
#include<QPropertyAnimation>
PlayScene::PlayScene(int levelNum)
{
    QString str=QString("进入了第 %1 关").arg(levelNum);
    qDebug()<<str;
    this->levelIndex=levelNum;

    //初始化游戏场景
    //设置固定大小
    this->setFixedSize(320,588);
    //设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));
    this->setWindowTitle("翻金币场景");

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
        qDebug()<<"翻金币场景中点击了返回按钮";

        //延时返回
        QTimer::singleShot(500,this,[=](){
            //告诉主场景 我返回了，主场景监听ChooseLevelScene的返回按钮
            emit this->chooseSceneBack();
        });
    });

    //显示当前的关卡数
    QLabel * label = new QLabel(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    QString str1 = QString("Level: %1").arg(this->levelIndex);
    //将字体设置到标签控件中
    label->setFont(font);
    label->setText(str1);
    label->setGeometry(30,this->height() - 50,120,50);


    //初始化每个关卡的二维数组
    dataConfig config;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            this->gameArray[i][j] = config.mData[this->levelIndex][i][j];
        }
    }


    //胜利图片显示
    QLabel* winLabel = new QLabel;
    QPixmap tmpPix;
    tmpPix.load(":/res/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->setParent(this);
    winLabel->move( (this->width() - tmpPix.width())*0.5 , -tmpPix.height());



    //显示金币背景图案
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            //绘制背景图片
            QLabel * label = new QLabel;
            label->setGeometry(0,0,50,50);
            label->setPixmap(QPixmap(":/res/BoardNode(1).png"));
            label->setParent(this);
            label->move(57 + i*50,200 + j*50);

            //创建金币
            if(this->gameArray[i][j]==1)
            {
                //显示金币
                str = ":/res/Coin0001.png";
            }
            else
            {
                //显示银币
                str = ":/res/Coin0008.png";
            }
            MyCoin * coin = new MyCoin(str);
            coin->setParent(this);
            coin->move(59 + i*50,204 + j*50);

            //给金币的属性赋值
            coin->posX = i;
            coin->posY = j;
            coin->flag = this->gameArray[i][j];//1 为正面 金币  0 为反面 银币

            //将金币放入到金币的二维数组 以便于后期的维护
            coinBtn[i][j]=coin;

            //点击金币 进行反转
            connect(coin,&MyCoin::clicked,this,[=](){
                //点击按钮 将所有的按钮先都禁用
                for(int i=0;i<4;i++)
                {
                    for(int j=0;j<4;j++)
                    {
                        this->coinBtn[i][j]->isWin = true;
                    }
                }

                coin->changeFlag();
                this->gameArray[i][j] = this->gameArray[i][j]==1 ? 0 : 1;

                //翻转周围的硬币，延时翻转
                //周围的右侧金币翻转的条件
                QTimer::singleShot(300,this,[=](){
                    if(coin->posX+1 < 4)
                    {
                        coinBtn[coin->posX+1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX+1][coin->posY] = this->gameArray[coin->posX+1][coin->posY]==1 ? 0 : 1;
                    }
                    //周围的左侧金币翻转的条件
                    if(coin->posX-1 >= 0)//周围的右侧金币翻转的条件
                    {
                        coinBtn[coin->posX-1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX-1][coin->posY] = this->gameArray[coin->posX-1][coin->posY]==1 ? 0 : 1;
                    }
                    //周围的下侧金币翻转的条件
                    if(coin->posY+1 < 4)
                    {
                        coinBtn[coin->posX][coin->posY+1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY+1] = this->gameArray[coin->posX][coin->posY+1]==1 ? 0 : 1;
                    }
                    //周围的上侧金币翻转的条件
                    if(coin->posY-1 >= 0)//周围的右侧金币翻转的条件
                    {
                        coinBtn[coin->posX][coin->posY-1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY-1] = this->gameArray[coin->posX][coin->posY-1]==1 ? 0 : 1;
                    }

                    //翻完所有金币后 将周围所有金币解开禁用
                    for(int i=0;i<4;i++)
                    {
                        for(int j=0;j<4;j++)
                        {
                            this->coinBtn[i][j]->isWin = false;
                        }
                    }


                    //判断是否胜利
                    this->isWin = true;
                    for(int i=0;i<4;i++)
                    {
                        for(int j=0;j<4;j++)
                        {
                            if(coinBtn[i][j]->flag == false)
                            {
                                this->isWin = false;
                                break;
                            }
                        }
                    }
                    if(this->isWin == true)
                    {
                        //胜利了
                        qDebug()<<"游戏胜利";
                        //将所有按钮的胜利标志改为true，如果再次点击按钮，直接return，不做响应
                        for(int i=0;i<4;i++)
                        {
                            for(int j=0;j<4;j++)
                            {
                                if(coinBtn[i][j]->flag == false)
                                {
                                    coinBtn[i][j]->isWin = true;
                                }
                            }
                        }

                        //将胜利的图片移动下来
                        QPropertyAnimation * animation = new QPropertyAnimation(winLabel,"geometry");
                        //设置时间间隔
                        animation->setDuration(1000);
                        //设置开始位置
                        animation->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
                        //设置结束位置
                        animation->setEndValue(QRect(winLabel->x(),winLabel->y() + 114,winLabel->width(),winLabel->height()));
                        //设置缓和曲线
                        animation->setEasingCurve(QEasingCurve(QEasingCurve::OutBounce));
                        //执行动画
                        animation->start();
                    }
                });
            });
        }
    }




}

//重写painterEvent事件
void PlayScene::paintEvent(QPaintEvent *)
{
    //创建背景
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //加载标题图标
    pix.load(":/res/Title.png");
    pix = pix.scaled(pix.width()*0.5,pix.height()*0.5);
    painter.drawPixmap(10,30,pix.width(),pix.height(),pix);


}
