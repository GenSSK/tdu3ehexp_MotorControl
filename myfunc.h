/*!
 * @file    myfunc.h
 * @brief   関数を宣言するヘッダファイル
 * @author  Genki Sasaki
 * @date    2021/08/01
 * @version 1.5
 */

#ifndef TDU3EHEXP_MOTORCONTROL_MYFUNC_H
#define TDU3EHEXP_MOTORCONTROL_MYFUNC_H

#include <fstream>
#include <ctime>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <math.h>
#include <fcntl.h>
#include <pthread.h>
#include <termios.h>

/*!
 * @brief	制御を行う関数
 * @param[in] CurrentTime	現在の時間(sec)
 * */

void Control(double CurrentTime){
	int num = UR.receive(&pc, &mbed);   //UDPを受信
	static bool FirstTime = true;       //初回起動のみ実行するためのフラグ
	static int initial = 0;             //制御開始時のエンコーダパルスを記録する変数
	static double thm_ = 0.0;           //角度LPF計算用変数
	static double thm_old = 0.0;
	static double wm_ = 0.0;            //角速度LPF計算用変数
	static double wm_old = 0.0;
	static double am_ = 0.0;            //角加速度LPF計算用変数
	static double am_old = 0.0;
	static double smp = 0.0;            //実サンプリング時間[sec]
	static double cpr = 1024 * 4 * 3.7; //一周あたりのエンコーダパルス数p[pulses]
	static double wc = 100;             //カットオフ周波数[rad/s]
	static double t = wc * 0.001;       //LPFの時定数
	static int ResponseCount = 0;       //モータのレスポンスをカウント
	static int OverSpeedCount = 0;      //角速度制限のカウント
	/*　制御開始時にエンコーダパルスを記録（初期値記録）　*/
	if(FirstTime) {
		if (num > 0) {
			initial = pc.tim4_pulse;
			FirstTime = false;
		}
	}

	/* 角度・角速度・角加速度の計算 */
	smp = CurrentTime - MI.t;   //実サンプリングの計算
	MI.t = CurrentTime; //現在の時間を格納
	MI.thm = (double)(pc.tim4_pulse - initial) / cpr * 2 * M_PI;  //角度の計算
	wm_ = (MI.thm - MI.thmPast) / smp;  //角速度の計算
	MI.wm = ((2 - t) / (2 + t)) * MI.wm + t / (2 + t) * (wm_ + wm_old);    //角速度にLPF
	am_ = (MI.wm - MI.wmPast) / smp;    //角加速度の計算
	MI.am = ((2 - t) / (2 + t)) * MI.am + t / (2 + t) * (am_ + am_old);    //角加速度にLPF
	MI.thmPast = MI.thm;    //角度の値を保持
	MI.wmPast = MI.wm;    //角速度の値を保持
	MI.amPast = MI.am;    //角加速度の値を保持
	thm_old = thm_; //角度LPF用の値を保持
	wm_old = wm_; //角速度LPF用の値を保持
	am_old = am_; //角加速度LPF用の値を保持

	/* 一定速度に達したら制御を終了する */
	if(fabs(MI.wm) > 40) {
		OverSpeedCount++;
		if (OverSpeedCount > 300){
			EndFlag = true;
			std::cout << "Over Speed!!" << std::endl;
		}
	} else {
		OverSpeedCount = 0;
	}

	/* エンコーダの異常検知 */
	if(fabs(MI.wm) < 0.15 && fabs(MI.u) > 0.05){
		ResponseCount++;
		if (ResponseCount > 300){
			EndFlag = true;
			std::cout << "Encoder is dead!!" << std::endl;
		}
	} else {
		ResponseCount = 0;
	}

	/*-----------------------------------ここから書いてください----------------------------------------------*/

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

/*!
 * @brief	CSVを記録する関数
 * @param[in] CurrentTime	現在の時間(sec)
 * @param[in] OldTime	前回の時間(sec)
 * */
void csvWriter(double CurrentTime, double OldTime){
	static int Data_num = 13; //CSVに書き込むデータの数
	static std::vector<std::vector<double>> Data(Data_num, std::vector<double>(0)); //CSVに書き込むデータを格納する変数

	/* 正常終了時にCSVを生成する */
	if(EndFlag) {
		std::ofstream OutputFile;
		/* ラベルを書き込む（一行目） */
		OutputFile.open(FILE_NAME, std::ios::out);  //指定されたファイルネームでCSVを作成
		OutputFile << "CurrentTime[sec],Current[A],Voltage[V],TargetAngle[rad],Angle[rad],TargetVelocity[rad/s],Velocity[rad/s]"
					  ",TargetAcceleration[rad/s^2],Acceleration[rad/s^2]"
					  ",kp[-],kd[-],ki[-],e_i[rad*s]";
		OutputFile << std::endl;

		/* 値を書き込む */
		for (int i = 0; i < Data[0].size(); ++i) {
			for (int j = 0; j < Data_num; ++j) {
				OutputFile << Data[j][i];
				if (j != Data_num - 1) {
					OutputFile << ",";
				}
			}
			OutputFile << std::endl;
		}
		OutputFile.close();
		std::cout << "CSV Write Complete!!" << std::endl;

		return;
	}

	/* 値をDataに書き込む */
	Data[0].push_back(MI.t);
	Data[1].push_back(MI.u);
	Data[2].push_back(MI.u * 24);
	Data[3].push_back(MI.thmref);
	Data[4].push_back(MI.thm);
	Data[5].push_back(MI.wmref);
	Data[6].push_back(MI.wm);
	Data[7].push_back(MI.amref);
	Data[8].push_back(MI.am);
	Data[9].push_back(MI.kp);
	Data[10].push_back(MI.kd);
	Data[11].push_back(MI.ki);
	Data[12].push_back(MI.e_i);

}

/*!
 * @brief	Terminalに出力する関数（printf）
 * @param[in] CurrentTime	現在の時間(sec)
 * */
double Print(double CurrentTime){
	std::cout << "Time = ";
	std::cout << CurrentTime;
	std::cout << " (sec)";
	std::cout << std::endl;
}

/*!
 * @brief	現在の時間を計算する関数
 * @return	現在時間(sec)
 * */
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

/*!
 * @brief	グラフを描画するスレッド用の関数
 * @param[in] *arg 任意
 * */
void *func_thread(void *arg) {
	char *argv_[] = {"Plot"};
	int argc_ = 1;
	glutInit(&argc_, argv_);
	gl_setting::PaintClass plt;
	plt.RealTimePlot(argc_, argv_, 1);
}

/*!
 * @brief	getchを行う関数
 * @return	読み込んだ文字
 * */
int getch() {
	struct termios oldt, newt;
	int ch;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}

/*!
 * @brief	_kbhitを行う関数
 * @retval	true	キー押下
 * @retval	false	何もなし
 * */
int kbhit() {
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if (ch != EOF) {
		ungetc(ch, stdin);
		return 1;
	}

	return 0;
}
#endif //TDU3EHEXP_MOTORCONTROL_MYFUNC_H
