#include "mycoin.h"
#include<QDebug>
#include<QTimer>
MyCoin::MyCoin(QString btnImg)
{
    QPixmap pix;
    bool ret = pix.load(btnImg);
    if(!ret)
    {
        QString str = QString("图片 %1 加载失败").arg(btnImg);
        qDebug()<<str;
        return;
    }
    this->setFixedSize(pix.width(),pix.height());
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(),pix.height()));

    //初始化定时器对象
    timer1 = new QTimer(this);
    timer2 = new QTimer(this);

    //监听正面翻反面的信号并反转硬币
    connect(timer1,&QTimer::timeout,this,[=](){
        QPixmap pix;
        QString str = QString(":/res/Coin000%1").arg(this->min++);
        pix.load(str);

        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("QPushButton{border:0px;}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));
        //判断 如果翻完了 将min重置为1
        if(this->min > this->max)
        {
            this->min = 1;
            isAnimation = false;//停止做动画
            timer1->stop();
        }
    });

    //监听反面翻正面的信号并反转硬币
    connect(timer2,&QTimer::timeout,this,[=](){
        QPixmap pix;
        QString str = QString(":/res/Coin000%1").arg(this->max--);
        pix.load(str);

        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("QPushButton{border:0px;}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));
        //判断 如果翻完了 将min重置为1
        if(this->max < this->min)
        {
            this->max = 8;
            isAnimation = false;//停止做动画
            timer2->stop();
        }
    });



}

//改变正反面标志的方法
void MyCoin::changeFlag()
{
    //如果是正面 翻成反面
    if(this->flag)
    {
        //开始正面翻反面的定时器
        timer1->start(30);
        isAnimation = true;//开始做动画了
        this->flag = false;
    }
    else //反面翻正面
    {
        timer2->start(30);
        isAnimation = true;//开始做动画了
        this->flag = true;
    }
}

//重写 按下 动画完成前不让再次点击
void MyCoin::mousePressEvent(QMouseEvent *e)
{
    if(this->isAnimation || this->isWin)//如果正在进行动画 那么什么指令就都不传了
    {
        return;
    }
    else//交给父类 父类该处理一些其他的东西
    {
        QPushButton::mousePressEvent(e);
    }
}
