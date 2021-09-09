/**
  * @file paint_class.h
  * @author s.koizumi
  * @brief 描画の設定
  * @version 0.1
  * @date 2020-07-29
  * @details 描画の設定，オブジェクト動作はglobalでpaint_conf.hに記載してある
  * @note paint_class.cpp : 描画の全体設定記述
  * @note paint_class_bc.cpp : 描画するオブジェクト
  * @note paint_class_event.cpp : openglのイベントの設定
  */

#ifndef GL_TASK_PAINT_H
#define GL_TASK_PAINT_H

#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <iostream>
#include <istream>
#include <sstream>
#include <chrono>
#include <cstring>
#include <string.h>
#include <stdlib.h>
#include <cmath>
#include <GL/glut.h>

#include "paint_conf.h"

using namespace std;
using namespace std::chrono;

#ifdef __linux
extern system_clock::time_point paint_tim1_str, paint_tim1_load;
extern struct timespec ts1;
#endif

namespace gl_setting {

    class PaintClass {
      private:
        /* data */
        system_clock::time_point p_tim_str, p_tim_load;

        static inline GraphSize gs;
        static inline GraphInfo gi;
        static inline GraphInfo giBase;
        static inline GraphData gd[256];

        static inline int PlotType;

        //! windowの枚数
        int Window_num_ = 1;
        //! 各windowのid
        int* WindowID_ = new int[Window_num_];

        //! 描画モード
        int mode_ = -1;
        //! windowサイズ
        int WSx_ = 350, WSy_ = 350;

        // 色(10進数)
        constexpr static double kBase_rdg_ = 255.0;

        static void DisplayString(float xPosition, float yPosition, GLColor, char *str, void *font);

      public:
        PaintClass(int mode_set = -1);
        ~PaintClass();

        /**
        * @brief OpenGL version check
        * @note version checkしたらプログラムは終了する
        */
        void VersionCheck(int argc, char* argv[]);

        /**
        * @brief 時間測定
        * 
        */
        static void CountTime(void);

        /**
        * @brief 描画全体設定
        * 
        * @param argc 
        * @param argv 
        */
        void DrawingSetting(int argc, char* argv[]);

        /**
        * @brief 描画全体設定 sample
        * 
        * @param argc 
        * @param argv 
        * @param mode_size 
        */
        void DisplayDemo01(int argc, char* argv[], int mode_size);

        /**
         * @brief ybarTask
         * 
         * @param argc 
         * @param argv 
         * @param mode_size 
         */
        void DisplayDemo02(int argc, char* argv[], int mode_size);

        /**
         * @brief RealTimePlot
         *
         * @param argc
         * @param argv
         * @param mode_size
         */
        void RealTimePlot(int argc, char* argv[], int mode_size);

        /**
        * @brief 描画全体設定 sample
        * 
        * @param argc 
        * @param argv 
        * @param mode_size 
        */
        void DisplayBimanualTask001(int argc, char* argv[], int mode_size);

        // Setting Display

        /**
        * @brief windowのサイズ設定
        * 
        * @param num 
        */
        void WindowSizeSet(int num);

        /**
        * @brief modeに応じた表示描画選択
        * 
        * @param num 
        */
        void DrawingSelection(int num);

        /**
        * @brief windowの縦横比を1:1
        * 
        * @param w 
        * @param h 
        */
        static void ResizeWindow(int w, int h);

        // Setting Event

        /**
        * @brief マウスのクリック時の座標出力
        * 
        * @param x 
        * @param y 
        */
        static void MouseClick(int x, int y);

        /**
        * @brief マウスの座標出力
        * 
        * @param x 
        * @param y 
        */
        static void MouseOver(int x, int y);

        /**
        * @brief キー入力時の更新
        * 
        * @param key 
        * @param x 
        * @param y 
        */
        static void KeyBoardPush(unsigned char key, int x, int y);

        /**
        * @brief glutMainLoop()の終了
        * 
        */
        static void FinishMainloop();

        /**
        * @brief 出力画面の更新
        * 
        * @param value 
        */
        static void DrawingUpdate(int value);

        // Setting Object

        /**
        * @brief 色の選択
        * 
        * @param str 色の名前かRGBを10進数で入力
        * @param color_manual 色のRGBをマニュアルで入力するかどうか(0:OFF, 1:ON)
        */
        static void ObjectColor(const char* str, int color_manual);

        /**
        * @brief 色の選択
        *
        * @param enum 色の名前をGLColorから選択
        */
        static void ObjectColor(GLColor);

        /**
        * @brief 塗りつぶし四角の出力
        * 
        * @param type_x xの座標
        * @param type_y yの座標
        * @param ax xの辺の長さ
        * @param by yの辺の長さ
        * @param color 色の選択:文字入力かマニュアル(glColor4f)入力
        * @param color_manual 色のRGBをマニュアルで入力するかどうか(0:OFF, 1:ON)
        * 
        * @note colorは color_manual==0 の場合 color="Red", color_manual==1 の場合 color="255,0,0,0"
        */
        static void ObjectShapeCube(double type_x, double type_y, double ax, double by, const char* color = "black", int color_manual = 0);

        /**
        * @brief フレームのみの四角の出力
        * 
        * @param type_x xの座標
        * @param type_y yの座標
        * @param ax xの辺の長さ
        * @param by yの辺の長さ
        * @param color 色の選択:文字入力かマニュアル入力
        * @param color_manual 色のRGBをマニュアルで入力するかどうか(0:OFF, 1:ON)
        */
        static void ObjectFrameCube(double type_x, double type_y, double ax, double by, const char* color, int color_manual = 0, double bar_width = 3);

        /**
        * @brief 塗りつぶし円の出力
        * 
        * @param type_x 
        * @param type_y 
        * @param ax 
        * @param by 
        * @param type_r 
        * @param color 色の選択:文字入力かマニュアル入力
        * @param color_manual 色のRGBをマニュアルで入力するかどうか(0:OFF, 1:ON)
        */
        static void ObjectShapeCycle(double type_x, double type_y, double ax, double by, double type_r, const char* color, int color_manual = 0);

        /**
        * @brief フレームのみの円の出力
        * 
        * @param type_x 
        * @param type_y 
        * @param ax 
        * @param by 
        * @param type_r 
        * @param color 色の選択:文字入力かマニュアル入力
        * @param color_manual 色のRGBをマニュアルで入力するかどうか(0:OFF, 1:ON)
        */
        static void ObjectFrameCycle(double type_x, double type_y, double ax, double by, double type_r, const char* color, int color_manual = 0);

        /**
        * @brief ドーナッツの出力
        * 
        * @param type_x 
        * @param type_y 
        * @param ax 
        * @param by 
        * @param type_r 
        * @param color 色の選択:文字入力かマニュアル入力
        * @param color_manual 色のRGBをマニュアルで入力するかどうか(0:OFF, 1:ON)
        */
        static void ObjectShapeLoopCycle(double type_x, double type_y, double ax, double by, double type_r, const char* color, int color_manual = 0);

        /**
         * @brief 三角
         * 
         * @param type_x 
         * @param type_y 
         * @param height 
         * @param bottom 
         * @param direct 向き(degree)
         * @param bar_width 
         * @param color 
         * @param color_manual 
         */
        static void ObjectShapeIsoscelesTriangle(double type_x, double type_y, double height, double bottom, double direct, int bar_width, const char* color, int color_manual = 0);

        /**
         * @brief 三角フレーム
         * 
         * @param type_x 
         * @param type_y 
         * @param height 
         * @param bottom 
         * @param direct 向き(degree)
         * @param color 
         * @param color_manual 
         */
        static void ObjectFrameIsoscelesTriangle(double type_x, double type_y, double height, double bottom, double direct, int bar_width, const char* color, int color_manual = 0);

        /**
         * @brief Lemniscate(レムニスケート)
         * 
         * @param type_x 
         * @param type_y 
         * @param color 
         * @param color_manual 
         * @param ax 
         * @param by 
         * @param type_r 
         */
        static void ObjectFrameLemniscate(double type_x, double type_y, const char* color, int color_manual = 0, double ax = 0.6, double by = 0.6, double type_r = 0.6);

        /**
         * @brief Astroid(アステロイド)
         * 
         * @param type_x 
         * @param type_y 
         * @param color 
         * @param color_manual 
         * @param ax 
         * @param by 
         * @param amp 振幅
         */
        static void ObjectFrameAstroid(double type_x, double type_y, const char* color, int color_manual = 0, double ax = 0.6, double by = 0.6, double amp = 0.8);

        /**
         * @brief Chrysanthemum(キク)
         * 
         * @param type_x 
         * @param type_y 
         * @param color 
         * @param color_manual 
         * @param ax 
         * @param by 
         */
        static void ObjectFrameChrysanthemum(double type_x, double type_y, const char* color, int color_manual = 0, double ax = 0.6, double by = 0.6);

        /**
         * @brief EpiCycloid(外サイクロイド)
         * 
         * @param type_x 
         * @param type_y 
         * @param color 
         * @param color_manual 
         * @param a1 振幅
         * @param a2 
         * @param a3 
         * @param w1 角周波数
         * @param w2 
         */
        static void ObjectFrameEpiCycloid(double type_x, double type_y, const char* color, int color_manual = 0, double a1 = 0.5, double a2 = 0.05, double a3 = 0.2, double w1 = 1, double w2 = 1);

        /**
         * @brief Lissajous(リサージュ図)
         * 
         * @param type_x 
         * @param type_y 
         * @param color 
         * @param color_manual 
         * @param a1 振幅
         * @param a2 
         * @param w1 角周波数
         * @param w2 
         */
        static void ObjectFrameLissajous(double type_x, double type_y, const char* color, int color_manual = 0, double a1 = 0.5, double a2 = 0.5, double w1 = 0.2, double w2 = 0.3);

        /**
         * @brief RoseCurve(正葉曲線)
         * 
         * @param type_x 
         * @param type_y 
         * @param color 
         * @param color_manual 
         * @param n 
         * @param k 
         * @param m 
         * @param a1 
         */
        static void ObjectFrameRoseCurve(double type_x, double type_y, const char* color, int color_manual = 0, int n = 2, double k = 1, int m = 2, double a1 = 1);

        /**
         * @brief gridの描画
         * 
         * @param color 
         * @param color_manual 
         */
        static void ObjectGraphGrid(float LineWidth, GLColor);

        /**
         * @brief グラフの軸の描画
         *
         * @param color
         * @param color_manual
         */
        static void ObjectGraphAxis(float LineWidth, GLColor);

        /**
         * @brief グラフの軸の値
         *
         * @param
         * @param
         */
        static void GraphTics(GLColor);

        /**
         * @brief グラフの軸ラベル
         *
         * @param
         * @param
         */
        static void GraphLabel(char *xAxis, char *yAxis, GLColor);

        /**
         * @brief グラフのプロット
         *
         * @param
         * @param
         */
        static void GraphPlot(int GraphNum, double x, double y, float LineWidth, GLColor);

        /**
         * @brief グラフのプロット点を計算
         *
         * @param
         * @param
         */
        static GraphData GraphPlotPoint(GraphData gd);

        /**
        * @brief 背景色の設定
        * 
        */
        static void BackColor(void);

        /**
        * @brief 描画設定 sample
        * 
        */
        static void DisplayDemo(void);

        /**
        * @brief 描画の内容(オブジェクト含む)
        * @details 縦方向のタスク
        * @note グローバル構造体(status_gl_cal)で値取得
        */
        static void XBarDisplay(void);

        /**
         * @brief 描画の内容(オブジェクト含む)
         * @details 横方向のタスク
         * @note グローバル構造体(status_gl_cal)で値取得
         */
        static void YBarDisplay(void);

        /**
         * @brief 描画の内容(オブジェクト含む)
         * @details 両手の縦タスク
         * @note グローバル構造体(status_gl_cal)で値取得
         */
        static void BimanualDisp1of2(void);

        /**
         * @brief 描画の内容(オブジェクト含む)
         * @details 両手の縦タスク
         * @note グローバル構造体(status_gl_cal)で値取得
         */
        static void BimanualReaching001(void);

        /**
         * @brief 描画の内容(オブジェクト含む)
         * @details リアルタイムプロット
         * @note グローバル構造体(status_gl_cal)で値取得
         */
        static void RealTimePlotDisplay(void);

        /**
         * @brief カンマ削除
         * 
         * @param input 入力データアドレス
         * @param delimiter 削除文字
         * @return vector<string> 
         */
        static std::vector<string> split(string& input, char delimiter);
    };

}  // namespace gl_setting

#endif  //GL_TASK_PAINT_H