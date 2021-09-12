#include "paint_conf.h"

namespace test_set {
    //! joystickの使用ON,OFF
    int js_mode = -1;
}  // namespace test_set

/**
 * @brief 描画プログラムの変数
 * 
 */
namespace show_set {
    int window_num   = 0;
    double x_a       = 0.0;
    double smp_disp  = 0.1;
    double time_str  = 0.0;
    double time_load = 0.0;
    double time_now  = 0.0;
}  // namespace show_set

namespace show_status {
    double ref_bufstatus[10][2][4] = {0};
    // 操作対象の初期位置
    // double obj_x = 0.8, obj_y = 0.2;
    // 目標対象の初期位置
    // double ref1_x = 0.5, ref1_y = 0.2;
}  // namespace show_status

namespace flag {
    //!
    int input_st = -1;
    int pseu_en  = -1;

    //! 終了変数
    int all_prog_fin = 0;
    //! 終了時間
    int fin_time = 80;

    //! 1回の描画(0:待ち時間,1:次の描画に移行)
    int confir_disp = -1;

    //! 位置誤差小さい -> アシスト切る : 1
    int stop_ass[2] = {0};

}  // namespace flag

//! 状態保持(bak: 書き込み用, cal: 読み込み用)
status_gl_s status_gl_bak[4], status_gl_cal[4];

//! csvに書き込む要素
namespace csvinfo_set {
    std::string date;
    std::string time;
    std::string name;
    std::string subject_name;
}  // namespace csvinfo_set

namespace user_status {
    // 被験者の名前
    std::string name;
    // 実験の有無
    int hand_memo;
    // 利き手
    int dominant_hand;
    // 実験で使用する手(利き手 : 0,非利き手 : 1, 仮想 : 2)
    int test_hand;
    // 実験回数
    int test_num;
    // 視線の位置 左上[0,0], 右下[-1,1]
    float eyes_position[100][2] = {0};
}  // namespace user_status

namespace file_names {
    // clion
    std::string search_clion_dir = "cmake-build-debug";
    // result
    std::string result_str = "result";
    std::string dominant_str;
    std::string test_hand_str;
    std::stringstream data_add_date;
    std::stringstream raw_add_date;
    std::stringstream nn_weight;
    std::stringstream data_data;
    std::stringstream raw_data;
    std::stringstream nn_weight_data;

}  // namespace file_names