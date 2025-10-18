/*!
 * @file    main.cpp
 * @brief   メイン関数
 * @author  Genki Sasaki
 * @date    2021/08/01
 * @version 1.4
 */

#include <iostream>
#include <unistd.h>
#include "udpSend.h"
#include "udpReceive.h"
#include "paint_conf.h"
#include "paint_class.h"
#include "myconfig.h"
#include "myfunc.h"

int main(int argc, char* argv[]) {
	std::cout << "Start, Control motor!" << std::endl;

	/* グラフプロット用のスレッド処理の開始 */
	pthread_t pthread;  //スレッドを初期化
	pthread_create(&pthread, NULL, func_thread, NULL);  //スレッドを作成・開始

	/*　変数の宣言　*/
	double EndTime = 10.0;          //[sec] 終了時間
	double CurrentTime = 0.0;       //[sec] 現在時間
	double ControlledTime = 0.0;    //[sec] 前回制御した時間
	double WroteTime = 0.0;         //[sec] 前回書き込んだ時間
	double PrintTime = 0.0;         //[sec] 前回出力した時間

	/* 制御まで一定時間待つ */
	sleep(3);

	/*　終了時間まで制御するループ　*/
	while (CurrentTime < EndTime) {

		/*　現在時刻の取得　*/
		CurrentTime = GetTime();

		/*　0.001[sec]で実行　*/
		if (CurrentTime >= ControlledTime + 0.001) {
			ControlledTime = GetTime();
			Control(CurrentTime);   //制御を行う関数
		}

		/*　現在時刻の取得　*/
		CurrentTime = GetTime();

		/*　0.01[sec]で実行　*/
		if (CurrentTime >= WroteTime + 0.01){
			csvWriter(CurrentTime, ControlledTime); //CSVに書き込みを行う関数
			WroteTime = GetTime();
		}

		/*　現在時刻の取得　*/
		CurrentTime = GetTime();

		/*　1[sec]で実行　*/
		if (CurrentTime >= PrintTime + 1.0){
			PrintTime = GetTime();
			Print(CurrentTime);     //Terminalに出力する関数
		}

		/* 終了フラグによる緊急停止 */
		if (EndFlag || kbhit() == 1)
			break;
	}

	mbed.ch1 = 0.0;     //モータの指令値を0にする
	US.send(&mbed);     //UDPの送信

	UR.release();   //UDPの受信用ポートを閉める
	US.release();   //UDPの送信用ポートを閉める
	std::cout << "End, Control motor!" << std::endl;
	return -1;
}
