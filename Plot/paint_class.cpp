#include "paint_class.h"

// 描画時間計測
system_clock::time_point paint_tim1_str, paint_tim1_load;

namespace gl_setting {

    // コンストラクタ, 描画数の決定, mode_の設定
    PaintClass::PaintClass(int mode_set) {
        mode_ = mode_set;
        // 画面の枚数
        int w_num   = 5;
        Window_num_ = w_num;
        // 画面数名の初期化
        memset(WindowID_, 0, w_num * sizeof(int));
        // 描画初期時間取得
#ifdef __linux
        p_tim_str      = chrono::high_resolution_clock::now();
        paint_tim1_str = chrono::high_resolution_clock::now();
#elif _WIN32
        p_tim_str           = chrono::system_clock::now();
        paint_tim1_str      = chrono::system_clock::now();
#endif
    }

    // デコンストラクタ
    PaintClass::~PaintClass() {
        // delete[] WindowID_;
    }

    // versionチェック
    void PaintClass::VersionCheck(int argc, char* argv[]) {
        glutInit(&argc, argv);
        glutCreateWindow("GLUT");
        cout << "version: " << glGetString(GL_VERSION) << endl;
        cout << "renderer: " << glGetString(GL_RENDERER) << endl;
        cout << "vendor: " << glGetString(GL_VENDOR) << endl;
        cout << "VersionCheck: finish program." << endl;
        exit(0);
    }

    // 時間測定
    void PaintClass::CountTime() {
#ifdef __linux
        paint_tim1_load     = chrono::high_resolution_clock::now();
        show_set::time_load = (static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(paint_tim1_load - paint_tim1_str).count() / std::pow(10, 9)));
#elif _WIN32
        paint_tim1_load     = chrono::system_clock::now();
        show_set::time_load = (static_cast<double>(chrono::duration_cast<chrono::nanoseconds>(paint_tim1_load - paint_tim1_str).count() / std::pow(10, 9)));
#endif
        cout << "chrono_time : " << show_set::time_load << endl;
    }

    // 描画設定
    void PaintClass::DrawingSetting(int argc, char* argv[]) {
        // 初期化
        glutInit(&argc, argv);
        // window1
        Window_num_ = 1;
        // 画面表示位置
        glutInitWindowPosition(600, 100);
        // 描画画面サイズ
        WindowSizeSet(mode_);
        glutInitWindowSize(WSx_, WSy_);
        // ウィンドウモード(default) + window 表示
        glutInitDisplayMode(GLUT_RGBA);
        // ID振り分け
        WindowID_[0] = glutCreateWindow("Window1");
        // 描画モード選択
        DrawingSelection(mode_);
        // 背景色
        BackColor();
        // GLのversion チェック
        // VersionCheck();
        // ウィンドサイズが変更された場合,関数呼び出し
        glutReshapeFunc(ResizeWindow);

        // // window2
        // Window_num_ = 2;
        // // 画面表示位置
        // glutInitWindowPosition(500, 100);
        // // 描画画面サイズ
        // WindowSizeSet(mode_);
        // glutInitWindowSize(WSx_, WSy_);
        // // ウィンドウモード(default) + window 表示
        // glutInitDisplayMode(GLUT_RGBA);
        // WindowID_[1] = glutCreateWindow("Window2");
        // // 描画モード選択
        // disp_size_out(mode_);
        // // 背景色
        // BackColor();
        // // GLのversion チェック
        // // VersionCheck();
        // // ウィンドサイズが変更された場合,関数呼び出し
        // glutReshapeFunc(ResizeWindow);

        show_set::window_num = Window_num_;

        // 以下各windowと共有
        // マウス入力で関数呼び出し;
        // glutMotionFunc(MouseClick);
        // glutPassiveMotionFunc(MouseOver);
        // 任意の入力で関数を呼びだす
        glutKeyboardFunc(KeyBoardPush);
        // 任意時間[msec]で関数を呼び出す
        glutTimerFunc((int)(show_set::smp_disp * pow(10, 3)), DrawingUpdate, 0);
        int Board;
        glutMainLoop();
    }

    void PaintClass::DisplayDemo01(int argc, char* argv[], int mode_size) {
        // 初期化
        glutInit(&argc, argv);
        // window1
        Window_num_ = 1;
        // 画面表示位置
        glutInitWindowPosition(1000, 100);
        // 描画画面サイズ
        // WindowSizeSet(mode_);
        WindowSizeSet(mode_size);
        glutInitWindowSize(WSx_, WSy_);
        // ウィンドウモード(default) + window 表示
        glutInitDisplayMode(GLUT_RGBA);
        // ID振り分け
        WindowID_[0] = glutCreateWindow("Window1");
        // 描画モード選択
        // glutDisplayFunc(YBarDisplay);
        glutDisplayFunc(BimanualDisp1of2);
        // 背景色
        BackColor();
        // GLのversion チェック
        // VersionCheck();
        // ウィンドサイズが変更された場合,関数呼び出し
        glutReshapeFunc(ResizeWindow);

        // // window2
        Window_num_ = 2;
        // 画面表示位置
        glutInitWindowPosition(400, 100);
        // 描画画面サイズ
        WindowSizeSet(mode_size);
        glutInitWindowSize(WSx_, WSy_);
        // ウィンドウモード(default) + window 表示
        glutInitDisplayMode(GLUT_RGBA);
        WindowID_[1] = glutCreateWindow("Window2");
        // 描画モード選択
        DrawingSelection(mode_);
        // 背景色
        BackColor();
        // GLのversion チェック
        // VersionCheck();
        // ウィンドサイズが変更された場合,関数呼び出し
        glutReshapeFunc(ResizeWindow);
        // glutmainloop終了オプション
//        glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

        // window の枚数
        show_set::window_num = Window_num_;

        // 以下各windowと共有
        // マウス入力で関数呼び出し;
        // glutMotionFunc(MouseClick);
        // glutPassiveMotionFunc(MouseOver);
        // 任意の入力で関数を呼びだす
        glutKeyboardFunc(KeyBoardPush);
        // 任意時間[msec]で関数を呼び出す
        glutTimerFunc((int)(show_set::smp_disp * pow(10, 3)), DrawingUpdate, 0);

        glutMainLoop();
    }

    void PaintClass::DisplayDemo02(int argc, char* argv[], int mode_size) {
        // 初期化
        glutInit(&argc, argv);
        // window1
        Window_num_ = 1;
        // 画面表示位置
        glutInitWindowPosition(1000, 100);
        // 描画画面サイズ
        // WindowSizeSet(mode_);
        WindowSizeSet(mode_size);
        glutInitWindowSize(WSx_, WSy_);
        // ウィンドウモード(default) + window 表示
        glutInitDisplayMode(GLUT_RGBA);
        // ID振り分け
        WindowID_[0] = glutCreateWindow("Window1");
        // 描画モード選択
        // glutDisplayFunc(YBarDisplay);
        glutDisplayFunc(BimanualDisp1of2);
        // 背景色
        BackColor();
        // GLのversion チェック
        // VersionCheck();
        // ウィンドサイズが変更された場合,関数呼び出し
        glutReshapeFunc(ResizeWindow);
        // glutmainloop終了オプション
//        glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

        // window の枚数
        show_set::window_num = Window_num_;

        // 以下各windowと共有
        // マウス入力で関数呼び出し;
        // glutMotionFunc(MouseClick);
        // glutPassiveMotionFunc(MouseOver);
        // 任意の入力で関数を呼びだす
        glutKeyboardFunc(KeyBoardPush);
        // 任意時間[msec]で関数を呼び出す
        glutTimerFunc((int)(show_set::smp_disp * pow(10, 3)), DrawingUpdate, 0);

        glutMainLoop();
    }

    void PaintClass::DisplayBimanualTask001(int argc, char* argv[], int mode_size) {
        // 初期化
        glutInit(&argc, argv);
        // window1
        Window_num_ = 1;
        // 画面表示位置
        glutInitWindowPosition(800, 100);
        // 描画画面サイズ
        // WindowSizeSet(mode_);
        WindowSizeSet(mode_size);
        glutInitWindowSize(WSx_, WSy_);
        // ウィンドウモード(default) + window 表示
        glutInitDisplayMode(GLUT_RGBA);
        // ID振り分け
        WindowID_[0] = glutCreateWindow("Window1");
        // 描画モード選択
        // glutDisplayFunc(YBarDisplay);
        glutDisplayFunc(BimanualReaching001);
        // 背景色
        BackColor();
        // GLのversion チェック
        // VersionCheck();
        // ウィンドサイズが変更された場合,関数呼び出し
        glutReshapeFunc(ResizeWindow);

        // glutmainloop終了オプション
//        glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

        // window の枚数
        show_set::window_num = Window_num_;

        // 以下各windowと共有
        // マウス入力で関数呼び出し;
        // glutMotionFunc(MouseClick);
        // glutPassiveMotionFunc(MouseOver);
        // 任意の入力で関数を呼びだす
        glutKeyboardFunc(KeyBoardPush);
        // 任意時間[msec]で関数を呼び出す
        glutTimerFunc((int)(show_set::smp_disp * pow(10, 3)), DrawingUpdate, 0);

        glutMainLoop();
    }

    void PaintClass::RealTimePlot(int argc, char* argv[], int mode_size){
        /*初期化*/
        Window_num_ = 1;    //Window number
        glutInitWindowPosition(600, 100);   //画面表示位置
        WindowSizeSet(mode_size);   //描画画面サイズ
        glutInitWindowSize(WSx_, WSy_); //サイズ決定
        glutInitDisplayMode(GLUT_RGBA); // ウィンドウモード(default) + window 表示
        WindowID_[0] = glutCreateWindow("Plot"); // ID振り分け
        PlotType = (int)PlotTypeSelect::xDynamic_yFix_PositiveNegative; //プロット手法を選択 PlotTypeSelect::を使用する

        /*描画モード*/
        glutDisplayFunc(RealTimePlotDisplay);// 描画モード選択
        BackColor(); // 背景色の設定
        glutReshapeFunc(ResizeWindow);// ウィンドサイズが変更された場合,関数呼び出し
//        glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);// glutmainloop終了オプション
        show_set::window_num = Window_num_; // window の枚数

        // 以下各windowと共有
        // マウス入力で関数呼び出し;
        // glutMotionFunc(MouseClick);
        // glutPassiveMotionFunc(MouseOver);
        // 任意の入力で関数を呼びだす
        glutKeyboardFunc(KeyBoardPush);
        // 任意時間[msec]で関数を呼び出す
        DrawingUpdate(0);
        
        glutMainLoop();
    };
}  // namespace gl_setting