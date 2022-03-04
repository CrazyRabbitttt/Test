#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <cmath>
#include <unistd.h>
using namespace  std;

const int NORMAL = 0;
const int TIME_OUT = 1;
const int THREE_DUP_ACK = 2;


const int MSS = 100;        //单个报文段的长度：100B

int ssthresh = 3 * MSS;     //初始时候的阈值
int cwnd = 1 * MSS;         //拥塞窗口：初始慢启动的时候就是1 MSS
int Index = 1;              //循环的轮数
int transData = 0;          //设置的总的传递的字节
int sumData;                //本次需要传递的总数据量的大小

int isCrowd(int cwnd) {
    int p = cwnd / MSS * 2;         //产生拥塞的概率
    if (rand() % 101 < p) {
        if (rand() % 101 < 50) {
            return  TIME_OUT;           //超时
        }else {
            return THREE_DUP_ACK;       //冗余ACK
        }
    }else return NORMAL;
}

//开始进行传输
void StartTrans() {
    bool isSlowStart = true;

    transData += cwnd;
    if (cwnd < ssthresh) {      //小于阈值，慢启动
        cout << "---------第" << Index++ << "轮传输-----------\n";
        cout << "慢启动：拥塞窗口cwnd=" << cwnd / MSS << ",慢启动门限ssthresh=" << ssthresh / MSS << endl;
    }else {                     //拥塞避免
        cout<<"***********************第"<<Index++<<"轮传输***********************"<<endl;
        cout<<"拥塞避免:拥塞窗口cwnd="<<cwnd/MSS<<",慢启动门限ssthresh="<<ssthresh/MSS<<endl;
        isSlowStart=false;
    }

    //本轮的数据传输已经结束，判断是否是拥塞
    int crow = isCrowd(cwnd);
    if (!crow) {            //没有发生拥塞
        if (isSlowStart) {  //还是慢启动
            cout << "检测到网络正常，将窗口进行加倍!\n";
            cwnd *= 2;
        }else {             //拥塞避免
            cout << "检测到网络正常，将窗口进行线性增加\n";
            cwnd+=MSS*(1.0*MSS/cwnd)*cwnd/MSS;
        }
    }else if (crow == TIME_OUT) {   //超时
            transData -= cwnd;
            //启动慢启动
            cout << "重传计数器超时，回退慢启动！阈值减为当前窗口一半，窗口置1\n";
            ssthresh = cwnd / 2;
            cwnd = 1 * MSS;
    } else if (crow = THREE_DUP_ACK) {  //冗余ACK
            cout << "收到了三个重复的ACK，采用快速恢复，阈值减半，窗口设置新的阈值\n";
            ssthresh = cwnd / 2;
            cwnd = ssthresh;
    }else {
        cout << "判断拥塞异常！返回异常值：" << crow << endl;
        exit(1);
    }

    if (transData < sumData)
        cout << "已经传输了：" << transData << "B, 还剩" << sumData - transData << "B\n";
    else {
        cout << "传输完毕！\n";
        return ;
    }

    cout << endl;
    sleep(1 * (cwnd % 3) + 1);
    StartTrans();
}

int main()
{
    srand(time(0));     //initialize seed

    cout << "输入本次传送数据量的大小（单位：字节B）";
    cin >> sumData;
    cout << "-----------------开始传输--------------------\n";

    StartTrans();
    return 0;
}

