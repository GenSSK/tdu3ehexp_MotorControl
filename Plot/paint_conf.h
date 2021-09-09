/**
 * @file paint_conf.h
 * @author your name (you@domain.com)
 * @brief global val for paint
 * @version 0.1
 * @date 2020-07-29
 */
#ifndef GL_TASK_CONFIG_H
#define GL_TASK_CONFIG_H

/**
 * @def MEMBER_SIZE
 * @brief 配列数確認
 */
#define MEMBER_SIZE(type, member) (sizeof(((type*)0)->member))

#define _USE_MATH_DEFINES

#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>

namespace test_set {
    //! joystickの使用ON,OFF
    extern int js_mode;
}  // namespace test_set

/**
 * @brief グローバル時間管理
 * 
 */
namespace show_set {
    //! window の枚数
    extern int window_num;
    // テスト用色変化変数
    extern double x_a;
    //! サンプリング時間
    extern double smp_disp;
    //! 時間取得(start)
    extern double time_str;
    //! 時間取得(load)
    extern double time_load;
    //! 時間取得(now)
    extern double time_now;
}  // namespace show_set

/**
 * @brief グローバル状態保持
 * 
 */
namespace show_status {

    // @brief 追従目標の状態[buffer(0が最新), (x,y), (x,dx,ddx,dddx)]
    extern double ref_bufstatus[10][2][4];

    // extern double ref1_x, ref1_y;
    // extern double obj_x, obj_y;
}  // namespace show_status

/**
 * @brief グローバルフラグ
 * 
 */
namespace flag {
    extern int input_st;
    extern int pseu_en;

    // tobii 1:on/ 0:off
    extern int tobii_set;

    // 終了変数
    extern int all_prog_fin;
    // 終了時間
    extern int fin_time;

    // 1回の描画(0:待ち時間,1:次の描画に移行)
    extern int confir_disp;

    // 位置誤差小さい -> アシスト切る : 1
    extern int stop_ass[2];

}  // namespace flag

/**
 * @struct Status_GL
 * @brief グローバル描画用の状態格納
 */
typedef struct Status_GL {
    double ref_state[4][4] = {0};  // [x,y,yaw,pitch][x, dx, ddx, dddx]
    double op_state[4][5]  = {0};  // [x, dx, ddx, dddx, js]
} status_gl_s;

// 状態保持bak: backup用
extern status_gl_s status_gl_bak[];

// 状態保持cal: calculate用
extern status_gl_s status_gl_cal[];

namespace csvinfo_set {
    extern std::string date;
    extern std::string time;
    extern std::string name;
    extern std::string subject_name;
}  // namespace csvinfo_set

namespace user_status {
    // 被験者の名前
    extern std::string name;
    // 実験の有無
    extern int hand_memo;
    // 利き手
    extern int dominant_hand;
    // 実験で使用する手(利き手 : 0,非利き手 : 1, 仮想 : 2)
    extern int test_hand;
    // 実験回数
    extern int test_num;
    // 視線の位置 左上[0,0], 右下[-1,1]
    extern float eyes_position[100][2];
}  // namespace user_status

namespace file_names {
    // clion
    extern std::string search_clion_dir;
    // result
    extern std::string result_str;
    extern std::string dominant_str;
    extern std::string test_hand_str;
    extern std::stringstream data_add_date;
    extern std::stringstream raw_add_date;
    extern std::stringstream nn_weight;
    extern std::stringstream data_data;
    extern std::stringstream raw_data;
    extern std::stringstream nn_weight_data;

}  // namespace file_names


typedef struct {
    float xLimLeft = -1.8;
    float xLimRight = 2.3;
    float yLimTop = 1.2;
    float yLimBottom = -1.1;
    int xGridNum = 20;
    int yGridNum = 20;
    float xAxisWidth;
    float yAxisWidth;
} GraphSize;

typedef struct {
    float xlimMax = 5;
    float xlimMin = 0;
    float ylimMax = 10;
    float ylimMin = -10;
    int xtics = 5;
    int ytics = 5;
    float xticsOffset = 0.2;
    float yticsOffset = 0.35;
    float xlabelOffset = 0.38;
    float ylabelOffset = 0.15;
} GraphInfo;

typedef struct {
    std::vector<double> x;
    std::vector<double> y;
} GraphData;

enum GLColor{
    Black,
    White,
    Blue,
    Green,
    Purple,
    Red,
    Pink,
    Orange,
    Yellow,
    Magenta,
    Cyan,
    DeepSkyBlue,
    Teal,
    Lime,
    LimeGreen,
    Gray,
    Silver,
    Paleturquoise,
    Chemical_Red,
    Chemical_Green,
    Chemical_Blue,
};

enum PlotTypeSelect{
    xFix_yFix_Positive,
    xFix_yFix_PositiveNegative,
    xReset_yFix_Positive,
    xReset_yFix_PositiveNegative,
    xDynamix_yFix_Positive,
    xDynamic_yFix_PositiveNegative,
    xFix_yDynamic_Positive,
    xFix_yDynamic_PositiveNegative,
    xReset_yDynamic_Positive,
    xReset_yDynamic_PositiveNegative,
    xDynamix_yDynamic_Positive,
    xDynamic_yDynamic_PositiveNegative,
    xFix_yMax_Positive,
    xFix_yMax_PositiveNegative,
    xReset_yMax_Positive,
    xReset_yMax_PositiveNegative,
    xDynamix_yMax_Positive,
    xDynamic_yMax_PositiveNegative,
};

#endif  //GL_TASK_CONFIG_H
