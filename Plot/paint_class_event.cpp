#include "paint_class.h"

namespace gl_setting {
    // 以下出力画面サイズの設定

    // 画面サイズ
    void PaintClass::WindowSizeSet(int mode_) {
        if (mode_ == -1) {
            WSx_ = 400;
            WSy_ = 300;
        }
        else if (mode_ == 0) {
            WSx_ = 400;
            WSy_ = 300;
        }
        else if (mode_ == 33) {
            WSx_ = 500;
            WSy_ = 500;
        }
        else if (mode_ == 35) {
            WSx_ = 1000;
            WSy_ = 1000;
        }
        else if (mode_ == 1) {
            WSx_ = 1000;
            WSy_ = 600;
        }
    }

    // 画面出力選択
    void PaintClass::DrawingSelection(int mode_) {
        if (mode_ == -1) {
            glutDisplayFunc(DisplayDemo);
        }
        else if (mode_ == 1) {
            glutDisplayFunc(YBarDisplay);
        }
        else if (mode_ == 3) {
            glutDisplayFunc(BimanualDisp1of2);
        }
    }

    // 以下イベント処理

    // 画面出力時のサイズ変更時の更新
    void PaintClass::ResizeWindow(int w, int h) {
        // windowサイズからw,hを取得
        // cout << "w : " << w << ", h : " << h << endl;
        // ウィンドウ全体をビューポートにする
        glViewport(0, 0, w, h);
        // 変換行列の初期化
        glLoadIdentity();
        gluPerspective(90.0, (double)w / (double)h, 1.0, 100.0);
        // カメラの視点(x,y,z)
        glTranslated(0.0, 0.0, -1.5);

        // スクリーン上の表示領域をビューポートの大きさに比例させる
        // glOrtho(-w / 700.0, w / 700.0, -h / 700.0, h / 700.0, -1.0, 1.0); // -1 ~ 1
    }

    // 画面出力時のマウス操作+click_on時の更新
    void PaintClass::MouseClick(int x, int y) {
        cout << "X = " << x << " : Y = " << y << endl;
    }

    // 画面出力時のマウス操作+click_off時の更新
    void PaintClass::MouseOver(int x, int y) {
        cout << "X = " << x << " : Y = " << y << endl;
    }

    // 画面出力時のキー入力時の更新
    void PaintClass::KeyBoardPush(unsigned char key, int x, int y) {
        // タスク終了
        if (key == ' ') {
            FinishMainloop();
        }
    }

    // glmainloopをs終了
    void PaintClass::FinishMainloop() {
        // 複数画面時
        for (int loop = 0; loop < show_set::window_num; ++loop) {
            // window削除
            glutDestroyWindow(loop + 1);
        }
        // 1回の描画
        flag::confir_disp = 1;
        // 他のプログラムループ終了フラグ
        flag::all_prog_fin = 1;
        // MainLoop終了
//        glutLeaveMainLoop();
    }

    // 出力画面の更新
    void PaintClass::DrawingUpdate(int value) {
        show_set::x_a += 0.01;
        if (show_set::x_a > 1.0) {
            show_set::x_a = 0.0;
        }

        // 複数画面
        for (int loop = 0; loop < show_set::window_num; loop++) {
            // 更新ウィンドウの変更
            // glutSetWindow(WindowID_[loop]);
            glutSetWindow(loop + 1);
            //再描画 (※display()関数を呼び出す関数 )
            glutPostRedisplay();
        }

        // 1回の描画
        flag::confir_disp = 1;

        glutTimerFunc((int)(show_set::smp_disp * 1000), DrawingUpdate, 0);
    }

    // 以下作成図のショートカット

    void PaintClass::ObjectShapeCube(double type_x, double type_y, double ax, double by, const char* color, int color_manual) {
        glLineWidth(5);
        ObjectColor(color, color_manual);
        glBegin(GL_POLYGON);
        glVertex2d(type_x - ax / 2, type_y + by / 2);  // 左上座標
        glVertex2d(type_x + ax / 2, type_y + by / 2);  // 右上
        glVertex2d(type_x + ax / 2, type_y - by / 2);  // 右下
        glVertex2d(type_x - ax / 2, type_y - by / 2);  // 左下
        glEnd();
    }

    void PaintClass::ObjectFrameCube(double type_x, double type_y, double ax, double by, const char* color, int color_manual, double bar_width) {
        ObjectColor(color, color_manual);
        glLineWidth(bar_width);
        glBegin(GL_LINE_LOOP);
        glVertex2d(type_x - ax / 2, type_y + by / 2);  // 左上座標
        glVertex2d(type_x + ax / 2, type_y + by / 2);  // 右上
        glVertex2d(type_x + ax / 2, type_y - by / 2);  // 右下
        glVertex2d(type_x - ax / 2, type_y - by / 2);  // 左下
        glEnd();
    }

    void PaintClass::ObjectShapeCycle(double type_x, double type_y, double ax, double by, double type_r, const char* color, int color_manual) {
        glLineWidth(5);
        ObjectColor(color, color_manual);
        glBegin(GL_POLYGON);
        double cycle_n    = 360;
        double cycle_rate = 0.0;
        double cycle_x = 0.0, cycle_y = 0.0;
        // 円を描画
        for (int i = 0; i < cycle_n; i += 15) {
            // 座標を計算
            cycle_rate = (double)i / cycle_n;
            cycle_x    = type_x + type_r * (sin(i * M_PI / 180.0) - cos(i * M_PI / 180.0));
            cycle_y    = type_y + type_r * (cos(i * M_PI / 180.0) + sin(i * M_PI / 180.0));
            glVertex3f((float)cycle_x, (float)cycle_y, 0.0);
        }
        glEnd();
    }

    void PaintClass::ObjectFrameCycle(double type_x, double type_y, double ax, double by, double type_r, const char* color, int color_manual) {
        ObjectColor(color, color_manual);
        glBegin(GL_LINE_LOOP);
        double cycle_n    = 360;
        double cycle_rate = 0.0;
        double cycle_x = 0.0, cycle_y = 0.0;
        // 円を描画
        for (int i = 0; i < cycle_n; i += 15) {
            // 座標を計算
            cycle_rate = (double)i / cycle_n;
            cycle_x    = type_x + type_r * (sin(i * M_PI / 180.0) - cos(i * M_PI / 180.0));
            cycle_y    = type_y + type_r * (cos(i * M_PI / 180.0) + sin(i * M_PI / 180.0));
            glVertex3f((float)cycle_x, (float)cycle_y, 0.0);
        }
        glEnd();
    }

    void PaintClass::ObjectShapeLoopCycle(double type_x, double type_y, double ax, double by, double type_r, const char* color, int color_manual) {
        glLineWidth(5);
        glBegin(GL_POLYGON);
        ObjectColor(color, color_manual);
        double cycle_n    = 360;
        double cycle_rate = 0.0;
        double cycle_x = 0.0, cycle_y = 0.0;
        // 円を描画
        for (int i = 0; i < cycle_n; i += 15) {
            // 座標を計算
            cycle_rate = (double)i / cycle_n;
            cycle_x    = type_x + type_r * (sin(i * M_PI / 180.0) - cos(i * M_PI / 180.0));
            cycle_y    = type_y + type_r * (cos(i * M_PI / 180.0) + sin(i * M_PI / 180.0));
            glVertex3f((float)cycle_x, (float)cycle_y, 0.0);
        }
        glEnd();
    }

    void PaintClass::ObjectShapeIsoscelesTriangle(double type_x, double type_y, double height, double bottom, double direct, int bar_width, const char* color, int color_manual) {
        ObjectColor(color, color_manual);
        glLineWidth(bar_width);
        double angle2rad = direct * M_PI / 2 / 90;
        glBegin(GL_LINE_LOOP);
        // 向き
        glVertex2d(type_x + height * std::cos(angle2rad), type_y + height * std::sin(angle2rad));
        // 底辺
        glVertex2d(type_x + bottom / 2 * std::cos(angle2rad + 30), type_y + bottom / 2 * std::sin(angle2rad + 30));
        glVertex2d(type_x + bottom / 2 * std::cos(angle2rad - 30), type_y + bottom / 2 * std::sin(angle2rad - 30));
        glEnd();
    }

    void PaintClass::ObjectFrameIsoscelesTriangle(double type_x, double type_y, double height, double bottom, double direct, int bar_width, const char* color, int color_manual) {
        ObjectColor(color, color_manual);
        glLineWidth(bar_width);
        double angle2rad = direct * M_PI / 2 / 90;
        glBegin(GL_LINE_LOOP);
        // 向き
        glVertex2d(type_x + height * std::cos(angle2rad), type_y + height * std::sin(angle2rad));
        // 底辺
        glVertex2d(type_x + bottom / 2 * std::cos(angle2rad + 30), type_y + bottom / 2 * std::sin(angle2rad + 30));
        glVertex2d(type_x + bottom / 2 * std::cos(angle2rad - 30), type_y + bottom / 2 * std::sin(angle2rad - 30));
        glEnd();
    }

    void PaintClass::ObjectFrameLemniscate(double type_x, double type_y, const char* color, int color_manual, double ax, double by, double type_r) {
        ObjectColor(color, color_manual);
        glBegin(GL_LINE_LOOP);
        double cycle_n    = 360;
        double cycle_rate = 0.0;
        double cycle_x = 0.0, cycle_y = 0.0;
        // 円を描画
        for (int i = 0; i < cycle_n; i += 15) {
            // 座標を計算
            cycle_rate = (double)i / cycle_n;
            cycle_x    = type_x + type_r * (sin(i * M_PI / 180.0));
            cycle_y    = type_y + type_r * (sin(2.0 * i * M_PI / 180.0));
            glVertex3f((float)cycle_x, (float)cycle_y, 0.0);
        }
        glEnd();
    }

    void PaintClass::ObjectFrameAstroid(double type_x, double type_y, const char* color, int color_manual, double ax, double by, double amp) {
        ObjectColor(color, color_manual);
        glBegin(GL_LINE_LOOP);
        double cycle_n    = 360;
        double cycle_rate = 0.0;
        double cycle_x = 0.0, cycle_y = 0.0;
        // 円を描画
        for (int i = 0; i < cycle_n; i += 15) {
            // 座標を計算
            cycle_rate = (double)i / cycle_n;
            cycle_x    = type_x + amp * std::pow(ax * std::cos(i * M_PI / 180.0), 3);
            cycle_y    = type_y + amp * std::pow(by * std::sin(i * M_PI / 180.0), 3);
            glVertex3f((float)cycle_x, (float)cycle_y, 0.0);
        }
        glEnd();
    }

    void PaintClass::ObjectFrameChrysanthemum(double type_x, double type_y, const char* color, int color_manual, double ax, double by) {
        ObjectColor(color, color_manual);
        glBegin(GL_LINE_LOOP);
        double cycle_n    = 1800;
        double cycle_rate = 0.0;
        double cycle_x = 0.0, cycle_y = 0.0;
        // 円を描画
        for (int i = 0; i < cycle_n; i += 15) {
            // 座標を計算
            cycle_rate = (double)i / cycle_n;
            cycle_x    = type_x + ax * std::sin(ax * i * M_PI / 180.0) * std::cos(i * M_PI / 180.0);
            cycle_y    = type_y + by * std::sin(by * i * M_PI / 180.0) * std::sin(i * M_PI / 180.0);
            glVertex3f((float)cycle_x, (float)cycle_y, 0.0);
        }
        glEnd();
    }

    void PaintClass::ObjectFrameEpiCycloid(double type_x, double type_y, const char* color, int color_manual, double a1, double a2, double a3, double w1, double w2) {
        ObjectColor(color, color_manual);
        glBegin(GL_LINE_LOOP);
        double cycle_n    = 360;
        double cycle_rate = 0.0;
        double cycle_x = 0.0, cycle_y = 0.0;
        // 円を描画
        for (int i = 0; i < cycle_n; i += 5) {
            // 座標を計算
            cycle_rate = (double)i / cycle_n;
            cycle_x    = type_x + ((a1 + a2) * std::cos(w1 * i * M_PI / 180.0) - a2 * std::cos(((a1 + a2) / a2) * i * M_PI / 180.0));
            cycle_y    = type_x + ((a1 + a2) * std::sin(w2 * i * M_PI / 180.0) - a2 * std::sin(((a1 + a2) / a2) * i * M_PI / 180.0));
            glVertex3f((float)cycle_x, (float)cycle_y, 0.0);
        }
        glEnd();
    }

    void PaintClass::ObjectFrameLissajous(double type_x, double type_y, const char* color, int color_manual, double a1, double a2, double w1, double w2) {
        ObjectColor(color, color_manual);
        glBegin(GL_LINE_LOOP);
        double cycle_n    = 3600;
        double cycle_rate = 0.0;
        double cycle_x = 0.0, cycle_y = 0.0;
        // 円を描画
        for (int i = 0; i < cycle_n; i += 15) {
            // 座標を計算
            cycle_rate = (double)i / cycle_n;
            cycle_x    = type_x + a1 * std::sin(w1 * i * M_PI / 180.0);
            cycle_y    = type_x + a2 * std::sin(w2 * i * M_PI / 180.0);
            glVertex3f((float)cycle_x, (float)cycle_y, 0.0);
        }
        glEnd();
    }

    void PaintClass::ObjectFrameRoseCurve(double type_x, double type_y, const char* color, int color_manual, int n, double k, int m, double a1) {
        ObjectColor(color, color_manual);
        glBegin(GL_LINE_LOOP);
        double cycle_n    = 360;
        double cycle_rate = 0.0;
        double cycle_x = 0.0, cycle_y = 0.0;
        double radius = 0.0;
        if (k > 2) {
            cycle_n = 36000;
        }
        // 円を描画
        for (int i = 0; i < cycle_n; i++) {
            // 座標を計算
            cycle_rate = i * M_PI / 180.0;
            radius     = a1 * std::pow(std::sin(n * cycle_rate / k), m);
            cycle_x    = type_x + radius * std::cos(cycle_rate);
            cycle_y    = type_x + radius * std::sin(cycle_rate);
            glVertex3f((float)cycle_x, (float)cycle_y, 0.0);
        }
        glEnd();
    }

    void PaintClass::ObjectGraphGrid(float LineWidth, GLColor Color) {
        ObjectColor(Color);
        glLineWidth(LineWidth);

        for (double i = 0; i < gs.xGridNum + 1; i++) {
            // x
            glBegin(GL_LINES);
            glVertex2d(gs.xLimLeft + ((gs.xLimRight - gs.xLimLeft) / (double)gs.xGridNum) * i, gs.yLimTop);
            glVertex2d(gs.xLimLeft + ((gs.xLimRight - gs.xLimLeft) / ((double)gs.xGridNum - 0.0)) * i, gs.yLimBottom);
            glEnd();
        }

        for (double i = 0; i < gs.yGridNum + 1; i++) {
            // y
            glBegin(GL_LINES);
            glVertex2d(gs.xLimLeft, gs.yLimBottom + ((gs.yLimTop - gs.yLimBottom) / (double)gs.yGridNum) * i);
            glVertex2d(gs.xLimRight, gs.yLimBottom + ((gs.yLimTop - gs.yLimBottom) / (double)gs.yGridNum) * i);
            glEnd();
        }
    }

    void PaintClass::ObjectGraphAxis(float LineWidth, GLColor Color){
        ObjectColor(Color);
        glLineWidth(LineWidth);
        /*yAxis*/
        glBegin(GL_LINES);
        glVertex2d(gs.xLimLeft, gs.yLimTop);
        glVertex2d(gs.xLimLeft, gs.yLimBottom);
        glEnd();

        /*xAxis*/
        if (PlotType == 0 || PlotType == 2 || PlotType == 4 || PlotType == 6 || PlotType == 8
            || PlotType == 10 || PlotType == 12 || PlotType == 14 || PlotType == 16){
            glBegin(GL_LINES);
            glVertex2d(gs.xLimLeft, gs.yLimBottom);
            glVertex2d(gs.xLimRight, gs.yLimBottom);
            glEnd();
        } else {
//            float yResize = (gi.ylimMax - gi.ylimMin) / (gs.yLimTop - gs.yLimBottom);
//            float yOffset = (gs.yLimTop + gs.yLimBottom) / 2;
//            float ratio = gi.ylimMax / gi.ylimMin;
//            float zero = 0.00001 / yResize + yOffset;
            glBegin(GL_LINES);
//            glVertex2d(gs.xLimLeft, zero);
//            glVertex2d(gs.xLimRight, zero);
            glVertex2d(gs.xLimLeft, (gs.yLimTop + gs.yLimBottom) / 2);
            glVertex2d(gs.xLimRight, (gs.yLimTop + gs.yLimBottom) / 2);
            glEnd();
        }

    }

    void PaintClass::GraphTics(GLColor Color){
        float xticsCalc;
        float yticsCalc;
        char buf[20];
        /* xAxis */
        for (int i = 0; i < gi.xtics; ++i) {
            xticsCalc = gi.xlimMin + ((gi.xlimMax - gi.xlimMin) / ((float)gi.xtics - 1) * (float)i);
            sprintf(buf, "%.1f", xticsCalc);
            DisplayString(gs.xLimLeft + ((gs.xLimRight - gs.xLimLeft) / (float)(gi.xtics - 1)) * i - 0.17,
                          gs.yLimBottom - gi.xticsOffset,
                          Color, buf, GLUT_BITMAP_TIMES_ROMAN_24);
        }
        /* yAxis */
        if (PlotType == 0 || PlotType == 2 || PlotType == 4 || PlotType == 6 || PlotType == 8
            || PlotType == 10 || PlotType == 12 || PlotType == 14 || PlotType == 16){
            for (int i = 0; i < gi.ytics; ++i) {
                yticsCalc = gi.ylimMax / (gi.ytics - 1) * i;
                sprintf(buf, "%5.0f", yticsCalc);
                DisplayString(gs.xLimLeft - gi.yticsOffset,
                              (gs.yLimBottom + ((gs.yLimTop - gs.yLimBottom) / (float)(gi.ytics - 1)) * i) - 0.05,
                              Color, buf, GLUT_BITMAP_TIMES_ROMAN_24);
            }
        } else {
            for (int i = 0; i < gi.ytics; ++i) {
                yticsCalc = (gi.ylimMax - gi.ylimMin) / (gi.ytics - 1) * i + gi.ylimMin;
                sprintf(buf, "%5.2f", yticsCalc);
                DisplayString(gs.xLimLeft - gi.yticsOffset,
                              (gs.yLimBottom + ((gs.yLimTop - gs.yLimBottom) / (float)(gi.ytics - 1)) * i) - 0.05,
                              Color, buf, GLUT_BITMAP_TIMES_ROMAN_24);
            }
        }
    }

    void PaintClass::GraphLabel(char* xAxis, char* yAxis, GLColor Color) {
        DisplayString(gs.xLimRight + gs.xLimLeft - 0.5, gs.yLimBottom - gi.xlabelOffset, Color, xAxis, GLUT_BITMAP_TIMES_ROMAN_24);
        DisplayString(gs.xLimLeft - 0.2, gs.yLimTop + gi.ylabelOffset, Color, yAxis, GLUT_BITMAP_TIMES_ROMAN_24);
    }

    void PaintClass::GraphPlot(int n, double x, double y, float LineWidth, GLColor Color) {
        if(x > gi.xlimMax){
            if (PlotType == 2 || PlotType == 3 || PlotType == 8 || PlotType == 9 || PlotType == 14
                || PlotType == 15){
                gi.xlimMax += giBase.xlimMax;
                gi.xlimMin += giBase.xlimMax;
                gd[n].x.clear();
                gd[n].y.clear();
            } else if (PlotType == 4 || PlotType == 5 || PlotType == 10 || PlotType == 11 || PlotType == 14
                     || PlotType == 16 || PlotType == 17){
                gi.xlimMax = x;
                gi.xlimMin = x - giBase.xlimMax;
                gd[n].x.erase(gd[n].x.begin());
                gd[n].y.erase(gd[n].y.begin());
            }
        }
        gd[n].x.push_back(x);
        gd[n].y.push_back(y);

        double MaxVal = 0;
        double MinVal = 0;
        for (int i = 0; i < gd[n].y.size(); ++i) {
            if (MaxVal < gd[n].y[i])
                MaxVal = gd[n].y[i];
            if (MinVal > gd[n].y[i])
                MinVal = gd[n].y[i];
        }
        if (PlotType == 6 || PlotType == 7 || PlotType == 8 || PlotType == 9 || PlotType == 10
            || PlotType == 11){
            if (n == 0){
                gi.ylimMax = MaxVal;
                gi.ylimMin = MinVal;
            } else {
                if (gi.ylimMax < MaxVal)
                    gi.ylimMax = MaxVal;
                if (gi.ylimMin > MinVal)
                    gi.ylimMin = MinVal;
            }
        }
        if (PlotType == 13 || PlotType == 14 || PlotType == 15 || PlotType == 16 || PlotType == 17) {
            if (gi.ylimMax < MaxVal)
                gi.ylimMax = MaxVal;
            if (gi.ylimMin > MinVal)
                gi.ylimMin = MinVal;
        }

        GraphData gdResized = GraphPlotPoint(gd[n]);
        ObjectColor(Color);
        if (gdResized.x.size() > 1) {
            for (int i = 1; i < gdResized.x.size(); ++i) {
                if(gdResized.x[i] >= gs.xLimLeft && gdResized.x[i] <= gs.xLimRight) {
                    glBegin(GL_LINES);
                    glVertex2d(gdResized.x[i - 1], gdResized.y[i - 1]);
                    glVertex2d(gdResized.x[i], gdResized.y[i]);
                    glEnd();
                }
            }
        }
    }

    GraphData PaintClass::GraphPlotPoint(GraphData gd) {
        GraphData gdRecalc;
        float xResize = (gi.xlimMax - gi.xlimMin) / (gs.xLimRight - gs.xLimLeft);
        float yResize = (gi.ylimMax - gi.ylimMin) / (gs.yLimTop - gs.yLimBottom);
        float xOffset = gs.xLimLeft;
        float yOffset = (gs.yLimTop + gs.yLimBottom) / 2;

        for (int i = 0; i < gd.x.size(); ++i) {
            gdRecalc.x.push_back((gd.x[i] - gi.xlimMin) / xResize + xOffset);
            gdRecalc.y.push_back(gd.y[i] / yResize + yOffset);
        }

        return gdRecalc;
    }

    void PaintClass::DisplayString(float xPosition, float yPosition, GLColor Color, char *str, void *font){
/*        FontTypes
            GLUT_BITMAP_8_BY_13
            GLUT_BITMAP_9_BY_15
            GLUT_BITMAP_TIMES_ROMAN_10
            GLUT_BITMAP_TIMES_ROMAN_24
            GLUT_BITMAP_HELVETICA_10
            GLUT_BITMAP_HELVETICA_12
            GLUT_BITMAP_HELVETICA_18
       */
        ObjectColor(Color);
        glRasterPos2f(xPosition,yPosition);
        for(int i = 0; i < strlen(str); i++) // 一文字ずつ描画
            glutBitmapCharacter(font, str[i]); // 一文字描画
    }

    void PaintClass::ObjectColor(GLColor color) {
        switch ((int)color) {
            case 0: glColor3f(0.0, 0.0, 0.0);
                break;
            case 1: glColor3f(1.0, 1.0, 1.0);
                break;
            case 2: glColor3f(0.0, 0.0, 1.0);
                break;
            case 3: glColor3f(0.0, 0.5, 0.0);
                break;
            case 4: glColor3f(0.5, 0.0, 1.0);
                break;
            case 5: glColor3f(1.0, 0.0, 0.0);
                break;
            case 6: glColor4f(1.0f, 0.65f, 0.65f, 0.5f);
                break;
            case 7: glColor3f(1.0, 0.5, 0.0);
                break;
            case 8: glColor3f(1.0, 1.0, 0.0);
                break;
            case 9: glColor3f(1.0, 0.0, 1.0);
                break;
            case 10: glColor3f(0.0, 1.0, 1.0);
                break;
            case 11: glColor3f(0 / kBase_rdg_, 191 / kBase_rdg_, 255 / kBase_rdg_);
                break;
            case 12: glColor3f(0.0, 0.5, 0.5);
                break;
            case 13: glColor3f(0.0, 1.0, 0.0);
                break;
            case 14: glColor3f(50 / 255.0, 205 / 255.0, 50 / 255.0);
                break;
            case 15: glColor3d(0.8, 0.8, 0.8);
                break;
            case 16: glColor3d(192 / 255.0, 192 / 255.0, 192 / 255.0);
                break;
            case 17: glColor3d((double)175 / 255, 238.0 / 255.0, 238.0 / 255.0);
                break;
            case 18: glColor4f(1.5, 0.0, 0.0, 0.0);
                break;
            case 19: glColor4f(0.0, 1.5, 0.0, 0.0);
                break;
            case 20: glColor4f(0.0, 0.0, 1.5, 0.0);
                break;
            default: std::cout << "ObjectColor Error : Nothing Paint Color." << std::endl;
                break;
        }
    }

    void PaintClass::ObjectColor(const char* str, int color_manual) {
        if (color_manual == 1) {
            static std::vector<string> color_rgb_str;
            static double color_rgb[4];
            std::string str2(str);

            color_rgb_str = split(str2, ',');
            for (int j = 0; j < 4; j++) {
                color_rgb[j] = std::stod(color_rgb_str[j]);
            }
            glColor4f(color_rgb[0] / kBase_rdg_, color_rgb[1] / kBase_rdg_, color_rgb[2] / kBase_rdg_, color_rgb[3]);
        }
        else if (color_manual == 0) {
            // if (str == "black"){
            //     glColor3f(0.0, 0.0, 0.0);
            // }

            if (strcmp(str, "Black") == 0) {
                glColor3f(0.0, 0.0, 0.0);
            }
            else if (strcmp(str, "White") == 0) {
                glColor3f(1.0, 1.0, 1.0);
            }
            else if (strcmp(str, "Blue") == 0) {
                glColor3f(0.0, 0.0, 1.0);
            }
            else if (strcmp(str, "Green") == 0) {
                glColor3f(0.0, 0.5, 0.0);
            }
            else if (strcmp(str, "Purple") == 0) {
                glColor3f(0.5, 0.0, 1.0);
            }
            else if (strcmp(str, "Red") == 0) {
                glColor3f(1.0, 0.0, 0.0);
            }
            else if (strcmp(str, "Pink") == 0) {
                //glColor3f(1.0f, 0.65f, 0.65f);
                glColor4f(1.0f, 0.65f, 0.65f, 0.5f);
            }
            else if (strcmp(str, "Orange") == 0) {
                glColor3f(1.0, 0.5, 0.0);
            }
            else if (strcmp(str, "Yellow") == 0) {
                glColor3f(1.0, 1.0, 0.0);
            }
            else if (strcmp(str, "Magenta") == 0) {
                glColor3f(1.0, 0.0, 1.0);
            }
            else if (strcmp(str, "Cyan") == 0) {
                glColor3f(0.0, 1.0, 1.0);
            }
            else if (strcmp(str, "DeepSkyBlue") == 0) {
                glColor3f(0 / kBase_rdg_, 191 / kBase_rdg_, 255 / kBase_rdg_);
            }
            else if (strcmp(str, "Teal") == 0) {
                glColor3f(0.0, 0.5, 0.5);
            }
            else if (strcmp(str, "Lime") == 0) {
                glColor3f(0.0, 1.0, 0.0);
            }
            else if (strcmp(str, "LimeGreen") == 0) {
                glColor3f(50 / 255.0, 205 / 255.0, 50 / 255.0);
            }
            else if (strcmp(str, "Gray") == 0) {
                glColor3d(0.8, 0.8, 0.8);
            }
            else if (strcmp(str, "Silver") == 0) {
                glColor3d(192 / 255.0, 192 / 255.0, 192 / 255.0);
            }
            else if (strcmp(str, "Paleturquoise") == 0) {
                glColor3d((double)175 / 255, 238.0 / 255.0, 238.0 / 255.0);
            }
            else if (strcmp(str, "Chemical_Red") == 0) {
                glColor4f(1.5, 0.0, 0.0, 0.0);
            }
            else if (strcmp(str, "Chemical_Green") == 0) {
                glColor4f(0.0, 1.5, 0.0, 0.0);
            }
            else if (strcmp(str, "Chemical_Blue") == 0) {
                glColor4f(0.0, 0.0, 1.5, 0.0);
            }
            else {
                std::cout << "ObjectColor Error : Nothing Paint Color." << std::endl;
            }
        }
        else {
            std::cout << "ObjectColor Error : Nothing Select Color Manual." << std::endl;
        }
    }

    vector<string> PaintClass::split(string& input, char delimiter) {
        istringstream stream(input);
        string field;
        vector<string> result;
        while (getline(stream, field, delimiter)) {
            result.push_back(field);
        }
        return result;
    }
}  // namespace gl_setting