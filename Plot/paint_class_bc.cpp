#include "paint_class.h"

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

extern MotorInformation MI_send;

namespace gl_setting {
    // 背景色
    void PaintClass::BackColor(void) {
        glClearColor(1.0, 1.0, 1.0, 1.0);
    }

    // テスト用 for test
    void PaintClass::DisplayDemo(void) {
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3d(1.0, show_set::x_a, 0.0);
        // ObjectColor("red");
        glBegin(GL_POLYGON);
        glVertex2d(-0.9, -0.9);
        glVertex2d(0.9, -0.9);
        glVertex2d(0.9, 0.9);
        glVertex2d(-0.9, 0.9);
        glEnd();

        glFlush();

        // 時間計測
        // chrono_time
        CountTime();
#ifdef __linux
        // for test
        // clock_gettime(CLOCK_REALTIME, &ts1);
        // show_set::time_load = static_cast<double>(ts1.tv_sec) + static_cast<double>(ts1.tv_nsec) * pow(10, -9);
        // show_set::time_now  = show_set::time_load - show_set::time_str;
        // std::cout << "time : " << show_set::time_now << std::endl;
#endif
    }

    // 横一次元
    void PaintClass::XBarDisplay(void) {
        //! 座標取得
        status_gl_bak[0] = status_gl_cal[0];

        // 初期化
        glClear(GL_COLOR_BUFFER_BIT);
        // // フレーム描画
        glLineWidth(2.0);
        ObjectFrameCube(0, 0, 4.2, 0.8, "Black");

        // ユーザー1の表示
        ObjectShapeCube(status_gl_bak[0].op_state[0][0], 0, 0.025, 0.55, "Chemical_Green");

        // 目標1の表示
        ObjectShapeCube(status_gl_bak[0].ref_state[0][0], -0.35, 0.03, 0.1, "Red");
        ObjectShapeCube(status_gl_bak[0].ref_state[0][0], 0.35, 0.03, 0.1, "Red");
        glFlush();
    }

    // 縦一次元
    void PaintClass::YBarDisplay(void) {
        // 座標取得
        status_gl_bak[0] = status_gl_cal[0];

        // 初期化
        glClear(GL_COLOR_BUFFER_BIT);
        // // フレーム描画
        glLineWidth(2.0);
        ObjectFrameCube(0, 0, 0.8, 2.6, "Black");

        // ユーザー1の表示
        ObjectShapeCube(0.0, status_gl_bak[0].op_state[1][0], 0.55, 0.025, "Chemical_Green");

        // 目標1の表示
        ObjectShapeCube(-0.35, status_gl_bak[0].ref_state[1][0], 0.1, 0.03, "Red");
        ObjectShapeCube(0.35, status_gl_bak[0].ref_state[1][0], 0.1, 0.03, "Red");
        glFlush();
    }

    // 二次元
    void PaintClass::BimanualDisp1of2(void) {
        // 座標取得
        status_gl_bak[0] = status_gl_cal[0];

        // リセット
        glClear(GL_COLOR_BUFFER_BIT);

        // フレーム描画
        glLineWidth(1.5);
        ObjectShapeCube(0, 0, 2.2, 2.2, "Gray");

        // if (show_set::time_load < 5) {
        //     ObjectShapeIsoscelesTriangle(0, 0, 0.5, 0.5, 60, 3, "Blue");
        // }
        // else if (show_set::time_load < 7) {
        //     ObjectFrameLemniscate(0, 0, "Blue");
        // }
        // else if (show_set::time_load < 9) {
        //     ObjectFrameLemniscate(0, 0, "Blue");
        // }
        // else if (show_set::time_load < 11) {
        //     ObjectFrameAstroid(0, 0, "Blue", 0, 0.8, 0.8, 1.0);
        // }
        // else if (show_set::time_load < 13) {
        //     ObjectFrameRoseCurve(0, 0, "Blue");
        // }
        // else if (show_set::time_load < 15) {
        //     ObjectGrid("Black");
        // }

        // 目標1の表示(Orange)
        ObjectShapeLoopCycle(status_gl_bak[0].ref_state[0][0], status_gl_bak[0].ref_state[1][0], 0, 0, 0.1, "Orange");

        // ユーザー1の表示
        ObjectShapeCycle(status_gl_bak[0].op_state[0][0], status_gl_bak[0].op_state[1][0], 0, 0, 0.06, "Pink");

        glFlush();
    }

    void PaintClass::BimanualReaching001(void) {
        // 座標取得
        status_gl_bak[0] = status_gl_cal[0];
        status_gl_bak[1] = status_gl_cal[1];

        // リセット
        glClear(GL_COLOR_BUFFER_BIT);

        // フレーム描画
        glLineWidth(1.5);
        ObjectShapeCube(0, 0, 2.2, 2.2, "Gray");

        // 目標1と2つなぐ(相対距離の計算が必要)
        ObjectColor("Purple", 0);
        glLineWidth(100);
        glBegin(GL_LINE_LOOP);
        glVertex2f(status_gl_bak[0].ref_state[0][0], status_gl_bak[0].ref_state[1][0]);
        glVertex2f(status_gl_bak[1].ref_state[0][0], status_gl_bak[1].ref_state[1][0]);
        glEnd();
        // ObjectShapeCube(0, 0, 1.0, 0.065, "Purple");

        // 目標1の表示(Orange)
        ObjectShapeCube(status_gl_bak[0].ref_state[0][0], status_gl_bak[0].ref_state[1][0], 0.1, 0.1, "Orange");
        ObjectFrameCube(status_gl_bak[0].ref_state[0][0], status_gl_bak[0].ref_state[1][0], 0.095, 0.1, "White");
        // 目標2の表示(cyan)
        ObjectShapeCube(status_gl_bak[1].ref_state[0][0], status_gl_bak[1].ref_state[1][0], 0.1, 0.1, "LimeGreen");
        ObjectFrameCube(status_gl_bak[1].ref_state[0][0], status_gl_bak[1].ref_state[1][0], 0.095, 0.1, "White");

        // ユーザー1の表示
        ObjectShapeCube(-0.4, -0.3, 0.06, 0.06, "Pink");

        // ユーザー2の表示
        ObjectShapeCube(0.4, -0.3, 0.06, 0.06, "DeepSkyBlue");

        glFlush();
    }

    void PaintClass::RealTimePlotDisplay(void){
        /*初期化*/
        glClear(GL_COLOR_BUFFER_BIT);

        /*フレーム描画*/
        ObjectGraphGrid(0.01, GLColor::Silver);
        ObjectGraphAxis(2.0, GLColor::Black);
        GraphTics(GLColor::Black);


        /* 角度のプロット */
        GraphLabel("Time[sec]", "Angle[rad]", GLColor::Black);
        GraphPlot(0, MI_send.t, MI_send.thm, 1.0, GLColor::Blue);
        GraphPlot(1, MI_send.t, MI_send.thmref, 1.0, GLColor::Red);

        /* 角速度のプロット */
//        GraphLabel("Time[sec]", "Angler Velocity[rad/s]", GLColor::Black);
//        GraphPlot(0, MI_send.t, MI_send.wm, 1.0, GLColor::Blue);
//        GraphPlot(1, MI_send.t, MI_send.wmref, 1.0, GLColor::Red);

        glFlush();
    }
}  // namespace gl_setting