//
// Created by Genki Sasaki on 2021/08/01.
// これは変数を宣言するヘッダファイルです． v1.3
//

#ifndef TDU3EHEXP_MOTORCONTROL_MYCONFIG_H
#define TDU3EHEXP_MOTORCONTROL_MYCONFIG_H

const char* FILE_NAME = "../Data/test.csv"; //ファイルネーム（実験ごとに書き換えないと上書きされる）

/* モータ制御に関するすべての変数を一つにした構造体 */
struct MotorInformation{
    double t = 0.0;         //時間[s]
    double u = 0.0;         //電流[A]
    double thm = 0.0;       //現在の角度[rad]
    double wm = 0.0;        //現在の角速度[rad/s]
    double am = 0.0;        //現在の角加速度[rad/s^2]
    double thmref = 0.0;    //目標の角度[rad]
    double wmref = 0.0;     //目標の角速度[rad/s]
    double amref = 0.0;     //目標の角加速度[rad/s^2]
    double thmPast = 0.0;   //1ステップ前の角度[rad]
    double wmPast = 0.0;    //1ステップ前の角速度[rad/s]
    double amPast = 0.0;    //1ステップ前の角加速度[rad/s^2]
    double kp = 0.0;        //Pゲイン[-]
    double kd = 0.0;        //Dゲイン[-]
    double ki = 0.0;        //Iゲイン[-]
    double e = 0.0;         //角度誤差[rad]
    double e_d = 0.0;       //角速度誤差[rad/s]
    double e_i = 0.0;       //角度誤差の積分[rad*s]
    double e_d_i = 0.0;     //角速度誤差の積分[(rad/s)*s]
};

MotorInformation MI;        //モータに関する構造体を初期化
MotorInformation MI_send;   //グラフ描画用の構造体を初期化

int count;              //Raspberry Pi -- mbed間の通信遅れを計算結果の格納する変数
bool EndFlag = false;  //モータ制御を終了するためのフラグ

udpReceive UR{50000, "10.0.1.4"};   //UDPの受信を設定
udpSend US{1235, "10.0.1.3"};       //UDPの送信を設定

toMbed mbed;    //UDP送信用の構造体を初期化
toPC pc;        //UDP受信用の構造体を初期化

#endif //TDU3EHEXP_MOTORCONTROL_MYCONFIG_H
