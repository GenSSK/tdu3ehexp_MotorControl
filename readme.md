# モータ制御へようこそ
これは東京電機大学工学部電子システム工学実験Ⅱのモータ制御のプログラムです．

## プログラムの内容
このプログラムはみなさんが触る部分はC言語によって書かれています．
C言語のプログラミングに自信のない方は，プログラミングの授業を復習することをおすすめします．

このプログラムで制御に必要なファイルは3つです．
それらの役割について以下に解説を行います．

### main.cpp
以下は制御ループに入る前の前処理です．

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
    sleep(1);

    /* CSVを一度書き込む（初期値） */
    csvWriter(CurrentTime, ControlledTime);

ここで，制御の時間は

    double EndTime = 10.0;          //[sec] 終了時間

で設定できます．

次に制御ループの説明をします．

このループでは制御時間が設定時間に達するまでループを続けます．

    /*　終了時間まで制御するループ　*/
    while (CurrentTime < EndTime) {


    }

ループ内のif文はそれぞれ決められた時間に実行されます．
時間を取得→if文と比較することで，リアルタイム性を確保しています．
以下のプログラムでは1kHzで制御を実行，100HzでCSVを書き込み，1Hzでターミナルに出力を行っています．

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
        WroteTime = GetTime();
        csvWriter(CurrentTime, ControlledTime); //CSVに書き込みを行う関数
    }

    /*　現在時刻の取得　*/
    CurrentTime = GetTime();

    /*　1[sec]で実行　*/
    if (CurrentTime >= PrintTime + 1.0){
        PrintTime = GetTime();
        Print(CurrentTime);     //Terminalに出力する関数
    }

最後に制御を終了しプログラムを終わります．

    mbed.ch1 = 0.0;     //モータの指令値を0にする
    US.send(&mbed);     //UDPの送信

    UR.release();   //UDPの受信用ポートを閉める
    US.release();   //UDPの送信用ポートを閉める
    std::cout << "End, Control motor!" << std::endl;
    return -1;

以上がmain.cppの内容になります．

### myconfig.h
このヘッダーファイルでは使用するグローバル変数やクラスなどの宣言を行っています．
まず

    const char* FILE_NAME = "../Data/test.csv"; //ファイルネーム（実験ごとに書き換えないと上書きされる）

ではCSVのファイル名を決めます．
同一の名前のCSVがすでにあった場合，上書きされるので注意してください．
実験では多数のファイルを生成するので，ファイル名を見ただけでどの実験か判断できるようにファイル名を決めてください．

次に，モータ制御に使用される値の変数をまとめた構造体が以下となります．
制御について記述する場合はこの構造体を使用してください．

    /* モータ制御に関するすべての変数を一つにした構造体 */
    struct MotorInformation{
        double t = 0.0; //時間[s]
        double u = 0.0; //電流[A]
        double thm = 0.0;   //現在の角度[rad]
        double wm = 0.0;    //現在の角速度[rad/s]
        double am = 0.0;    //現在の角加速度[rad/s^2]
        double thmref = 0.0;    //目標の角度[rad]
        double wmref = 0.0; //目標の角速度[rad/s]
        double amref = 0.0; //目標の角加速度[rad/s^2]
        double thmPast = 0.0;   //1ステップ前の角度[rad]
        double wmPast = 0.0;    //1ステップ前の角速度[rad/s]
        double amPast = 0.0;    //1ステップ前の角加速度[rad/s^2]
        double kp = 0.0;    //Pゲイン[-]
        double kd = 0.0;    //Dゲイン[-]
        double ki = 0.0;    //Iゲイン[-]
        double e = 0.0; //角度誤差[rad]
        double e_d = 0.0; //角速度誤差[rad/s]
        double e_i = 0.0;   //角度誤差の積分[rad*s]
        double e_d_i = 0.0; //角速度誤差の積分[(rad/s)*s]
    };

次に構造体を初期化します．
ここでMIはモータ制御に使用し，MI_sendはグラフプロット用に使用されます．
制御を記述する際はMIを使用してください．

    MotorInformation MI;        //モータに関する構造体を初期化
    MotorInformation MI_send;   //グラフ描画用の構造体を初期化

以下はUDPの送受信を行うための宣言です．

    int count;  //Raspberry Pi -- mbed間の通信遅れを計算結果の格納する変数

    udpReceive UR{50000, "10.0.1.4"};   //UDPの受信を設定
    udpSend US{1235, "10.0.1.3"};       //UDPの送信を設定

    toMbed mbed;    //UDP送信用の構造体を初期化
    toPC pc;        //UDP受信用の構造体を初期化

以上がmyconfig.hの内容です．

### myfunc.h
このヘッダーファイルでは制御に使用する関数が宣言されています．

まずはじめに，

    /* 制御を行う関数 */
    void Control(double CurrentTime){
    
    }

は制御を行う関数です．
この関数はmain関数の制御ループにおいて1kHzの周期で呼ばれます．

この関数では，まずはじめにmbedからのUDPの受信が行われます．
これによってモータに装着れているエンコーダのパルスを受け取ります．

    int num = UR.receive(&pc, &mbed);   //UDPを受信


次に，関数内で使用される変数の定義が行われます．
    
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
    static double T1 = 0.001;           //LPFの時定数
    static double T2 = 0.01;            //LPFの時定数

次に，初回のみエンコーダの初期値を計算します．
これによって，初期値ズレによるモータの暴走を抑制します．

    /*　制御開始時にエンコーダパルスを記録（初期値記録）　*/
    if(FirstTime) {
        if (num > 0) {
            initial = (double) pc.tim4_pulse;
            FirstTime = false;
        }
    }

次に，モータの角度，角速度，角加速度の計算を行います．
ここでは，まずはじめにサンプリング時間（現在の時間から前回制御した時間の差分）を計算します．
そして，エンコーダのパルスから角度を計算します．
その後，角度をサンプリング時間で微分して角速度を計算，同様に角加速度を角速度から計算します．
ここで，エンコーダのパルス列は離散的であるため，LPFを用いて平滑化を行います．

    smp = CurrentTime - MI.t;   //実サンプリングの計算
    MI.t = CurrentTime; //現在の時間を格納
    thm_ = (double)(pc.tim4_pulse - initial) / cpr * 2 * M_PI;  //角度の計算
    MI.thm = ((2 * T1 - smp) / (2 * T1 + smp)) * MI.thmPast + (smp / (2 * T1 + smp)) * (thm_ + thm_old);    //角度にLPF
    wm_ = (MI.thm - MI.thmPast) / smp;  //角速度の計算
    MI.wm = ((2 * T2 - smp) / (2 * T2 + smp)) * MI.wmPast + (smp / (2 * T2 + smp)) * (wm_ + wm_old);    //角速度にLPF
    am_ = (MI.wm - MI.wmPast) / smp;    //角加速度の計算
    MI.am = ((2 * T2 - smp) / (2 * T2 + smp)) * MI.amPast + (smp / (2 * T2 + smp)) * (am_ + am_old);    //角速度にLPF
    MI.thmPast = MI.thm;    //角度の値を保持
    MI.wmPast = MI.wm;    //角速度の値を保持
    MI.amPast = MI.am;    //角加速度の値を保持
    thm_old = thm_; //角度LPF用の値を保持
    wm_old = wm_; //角速度LPF用の値を保持
    am_old = am_; //角加速度LPF用の値を保持

個々まで計算できたら，モータの角度，角速度，角加速度の情報を使って制御の記述を行います．
制御は以下に示す部分に記述してください．

    /*-----------------------------------ここから書いてください----------------------------------------------*/



    /*-----------------------------------ここまで書いてください----------------------------------------------*/


次に，モータの指令値が上限を超えないように値を制限して，UDPでmbedに指令値を送信します．

    /* 制御指令値は最大1~-1なので，制限を計算する */
    if (MI.u > 1.0){
        MI.u = 1.0;
    } else if (MI.u < -1.0){
        MI.u = -1.0;
    }
    mbed.ch1 = MI.u;    //UDP送信用変数に指令値を格納
    MI_send = MI;   //描画用変数に値を渡す

    US.send(&mbed);     //UDP送信
    count = mbed.checkCount - pc.returnCount;   //Raspberry Pi -- mbed間の通信遅れを計算


以上がControl関数の内容です．

また，他の関数はCSVを記録する関数，ターミナルに出力するための関数，現在の時間を取得する関数，グラフを描画するための関数があります．

    /* CSVを記録する関数 */
    void csvWriter(double CurrentTime, double OldTime){
    
    }

    /* Terminalに出力する関数（printf） */
    double Print(double CurrentTime){

    }

    /* 現在の時間を計算する関数 */
    double GetTime(){

    }

    /* グラフを描画するスレッド用の関数 */
    void *func_thread(void *arg) {

    }


以上がmyfunc.hの内容です．

## 実験で記述する場所
実験では
- モータ制御の時間を変更する
- 記録されるCSVファイルの名前を変更する
- 制御を記述する

ことが必要です．
以下にそれぞれの具体的な方法を説明します．

### モータ制御の時間を変更する
時間を変更する場合はmain.cpp内の`EndTime`を変更します．
たとえば，モータ制御の時間を5秒間としたいときは

    double EndTime = 5.0;

と変更します．

### 記録されるCSVファイルの名前を変更する
CSVのファイル名を変更する場合はmyconfig.h内の`FILE_NAME`を変更します．
たとえば，PID制御でKpゲインが1.0，Kdゲインが0.1，Kiゲインが0.01の場合は

    const char* FILE_NAME = "../Data/PIDControl_Kp1.0_Kd0.1_Ki0.01.csv";

と変更します．
ここでファイル名に`../Data/`を含める理由は，Dataフォルダ内にファイルを格納するためです．

### 制御を記述する
制御はmyfunc.hのControl関数内の以下に示す部分に必ず記述してください．
この場所以外で制御について記述すると，モータが正常に制御されない可能性があります．

    /*-----------------------------------ここから書いてください----------------------------------------------*/


    /*-----------------------------------ここまで書いてください----------------------------------------------*/

たとえば，モータへの指令値を0.5としたいならば，

    MI.u = 0.5;

と書いてください．
また，モータの目標角度をπ (rad)に設定したい場合は，

    MI.thmref = M_PI;

と書いてください．


## 速度制御をする場合
モータ制御実験の後半では速度制御を行います．
ここで，速度制御の場合はグラフの縦軸を角度から角速度に変更する必要があります．
そこで，Plotフォルダのpaint_class_bc.cppの一番下にある関数を一部変更します．

初期状態では

    /* 角度のプロット */
    GraphLabel("Time[sec]", "Angle[rad]", GLColor::Black);
    GraphPlot(0, MI_send.t, MI_send.thm, 1.0, GLColor::Blue);
    GraphPlot(1, MI_send.t, MI_send.thmref, 1.0, GLColor::Red);

    /* 角速度のプロット */
    //GraphLabel("Time[sec]", "Angler Velocity[rad/s]", GLColor::Black);
    //GraphPlot(0, MI_send.t, MI_send.wm, 1.0, GLColor::Blue);
    //GraphPlot(1, MI_send.t, MI_send.wmref, 1.0, GLColor::Red);

となっていますが，これを

    /* 角度のプロット */
    //GraphLabel("Time[sec]", "Angle[rad]", GLColor::Black);
    //GraphPlot(0, MI_send.t, MI_send.thm, 1.0, GLColor::Blue);
    //GraphPlot(1, MI_send.t, MI_send.thmref, 1.0, GLColor::Red);

    /* 角速度のプロット */
    GraphLabel("Time[sec]", "Angler Velocity[rad/s]", GLColor::Black);
    GraphPlot(0, MI_send.t, MI_send.wm, 1.0, GLColor::Blue);
    GraphPlot(1, MI_send.t, MI_send.wmref, 1.0, GLColor::Red);

と変更します．