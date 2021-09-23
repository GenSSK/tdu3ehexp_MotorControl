//
// Created by Genki Sasaki on 2021/08/01.
// これは関数を宣言するヘッダファイルです． v1.3
//

#ifndef TDU3EHEXP_MOTORCONTROL_MYFUNC_H
#define TDU3EHEXP_MOTORCONTROL_MYFUNC_H

#include <fstream>
#include <math.h>
#include <pthread.h>

/* 制御を行う関数 1kHz毎に呼ばれる */
void Control(double CurrentTime){
    int num = UR.receive(&pc, &mbed);   //UDPを受信
    static bool FirstTime = true;       //初回起動のみ実行するためのフラグ
    static int initial = 0;             //制御開始時のエンコーダパルスを記録する変数
    static double wm_ = 0.0;            //角速度LPF計算用変数
    static double wm_old = 0.0;
    static double am_ = 0.0;            //角加速度LPF計算用変数
    static double am_old = 0.0;
    static double smp = 0.0;            //実サンプリング時間[sec]
    static double cpr = 1024 * 4 * 3.7; //一周あたりのエンコーダパルス数p[pulses]
    static double T1 = 0.001;           //LPFの時定数
    static double T2 = 0.01;            //LPFの時定数
    static int OverSpeedCount = 0;      //角速度制限のカウント
    static int ResponseCount = 0;       //モータのレスポンスをカウント
    /*　制御開始時にエンコーダパルスを記録（初期値記録）　*/
    if(FirstTime) {
        if (num > 0) {
            initial = (double) pc.tim4_pulse;
            FirstTime = false;
        }
    }

    /* 角度・角速度・角加速度の計算 */
    smp = CurrentTime - MI.t;   //実サンプリングの計算
    MI.t = CurrentTime; //現在の時間を格納
    MI.thm = (double)(pc.tim4_pulse - initial) / cpr * 2 * M_PI;  //角度の計算
    wm_ = (MI.thm - MI.thmPast) / smp;  //角速度の計算
    MI.wm = ((2 * T2 - smp) / (2 * T2 + smp)) * MI.wmPast + (smp / (2 * T2 + smp)) * (wm_ + wm_old);    //角速度にLPF
    am_ = (MI.wm - MI.wmPast) / smp;    //角加速度の計算
    MI.am = ((2 * T2 - smp) / (2 * T2 + smp)) * MI.amPast + (smp / (2 * T2 + smp)) * (am_ + am_old);    //角加速度にLPF
    MI.thmPast = MI.thm;    //角度の値を保持
    MI.wmPast = MI.wm;    //角速度の値を保持
    MI.amPast = MI.am;    //角加速度の値を保持
    wm_old = wm_; //角速度LPF用の値を保持
    am_old = am_; //角加速度LPF用の値を保持

    /* 一定速度に達したら制御を終了する */
    if(fabs(MI.wm) > 40) {
        OverSpeedCount++;
        if (OverSpeedCount > 300){
            EndFlag = true;
            std::cout << "Over Speed!! FORCE STOP" << std::endl;
        }
    } else {
        OverSpeedCount = 0;
    }

    /* エンコーダの異常検知 */
    if(fabs(MI.wm) < 0.15 && fabs(MI.u) > 0.05){
        ResponseCount++;
        if (ResponseCount > 300){
            EndFlag = true;
            std::cout << "Encoder is dead!! FORCE STOP" << std::endl;
        }
    } else {
        ResponseCount = 0;
    }

    /*-----------------------------------ここから書いてください----------------------------------------------*/

    /* 目標値 */
    MI.thmref = M_PI * 2;
    MI.wmref = 0.0;

    /* ゲイン */
    MI.kp = 0.1;
    MI.kd = 0.0;
    MI.ki = 0.0;

    MI.e = MI.thmref - MI.thm; // 角度誤差
    MI.e_d = MI.wmref - MI.wm; // 角速度誤差
    MI.e_i += MI.e * smp; // 定常偏差の蓄積 (目標値 - 現在)

    MI.u = MI.kp * MI.e + MI.kd * MI.e_d + MI.ki * MI.e_i;
    
    /*-----------------------------------ここまで書いてください----------------------------------------------*/
    /* 制御指令値は最大1.0~-1.0なので，制限を計算する */
    if (MI.u > 1.0){
        MI.u = 1.0;
    } else if (MI.u < -1.0){
        MI.u = -1.0;
    }
    mbed.ch1 = MI.u;    //UDP送信用変数に指令値を格納
    MI_send = MI;   //描画用変数に値を渡す

    US.send(&mbed);     //UDP送信
    count = mbed.checkCount - pc.returnCount;   //Raspberry Pi -- mbed間の通信遅れを計算
}

/* CSVを記録する関数 */
void csvWriter(double CurrentTime, double OldTime){
    static bool FirstTime = true;
    static std::ofstream OutputFile;

    /* 初回実行時にラベルを書き込む（一行目） */
    if(FirstTime) {
        OutputFile.open(FILE_NAME, std::ios::out);  //指定されたファイルネームでCSVを作成
        OutputFile << "CurrentTime[sec],Current[A],Voltage[V],TargetAngle[rad],Angle[rad],TargetVelocity[rad/s],Velocity[rad/s]"
                      ",TargetAcceleration[rad/s^2],Acceleration[rad/s^2]"
                      ",kp[-],kd[-],ki[-],e_i[rad*s]";
        OutputFile << std::endl;
        FirstTime = false;
    }

        /* 値をCSVに書き込む */
        OutputFile << MI.t;
        OutputFile << "," << MI.u;
        OutputFile << "," << MI.u * 24;
        OutputFile << "," << MI.thmref;
        OutputFile << "," << MI.thm;
        OutputFile << "," << MI.wmref;
        OutputFile << "," << MI.wm;
        OutputFile << "," << MI.amref;
        OutputFile << "," << MI.am;
        OutputFile << "," << MI.kp;
        OutputFile << "," << MI.kd;
        OutputFile << "," << MI.ki;
        OutputFile << "," << MI.e_i;
        OutputFile << std::endl;

}

/* Terminalに出力する関数（printf） */
double Print(double CurrentTime){
    std::cout << "Time = ";
    std::cout << CurrentTime;
    std::cout << " (sec)";
    std::cout << std::endl;
}

/* 現在の時間を計算する関数 */
double GetTime(){
    static int i = 0;
    /* 時間関連の変数 */
    static struct timespec s_time, e_time;
    if(i == 0) {
        clock_gettime(CLOCK_MONOTONIC, &s_time);
        i++;
    }
    clock_gettime(CLOCK_MONOTONIC, &e_time);
    double time = ((double)e_time.tv_sec - (double)s_time.tv_sec) + ((double)e_time.tv_nsec - (double)s_time.tv_nsec) * 1.0E-9;
    return time;
}

/* グラフを描画するスレッド用の関数 */
void *func_thread(void *arg) {
    char *argv_[] = {"Plot"};
    int argc_ = 1;
    glutInit(&argc_, argv_);
    gl_setting::PaintClass plt;
    plt.RealTimePlot(argc_, argv_, 1);
}
#endif //TDU3EHEXP_MOTORCONTROL_MYFUNC_H
