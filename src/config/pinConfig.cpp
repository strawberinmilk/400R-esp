#include "pinConfig.h"

// I2C LCDディスプレイ
const int LCD_ADDRESS = 0x3E;
// const int SDA_PIN = 21;
// const int SCL_PIN = 22;

// ボタン
const int SELECT_SW_PIN = 19;
// const int UN_USED_BUTTON_PIN = 18;

// エンコーダ
const int ENCODER_A_PIN = 32;
const int ENCODER_B_PIN = 33;

// フットライト出力
const int FOOTLIGHT_PIN = 17;
const int PWM_CH = 0; // pin17 foot light PWM channel

// ハートライト出力
const int HEARTLIGHT_PIN = 4;
const int HEARTLIGHT_PWM_CH = 1; // pin4 heart light PWM channel

// イルミ線入力
const int ILLUMI_INPUT_PIN = 16;

// サイドブレーキ線入力
const int HAND_BRAKE_INPUT_PIN = 15;

// DAC対応ピン 配線済み
// const int DAC_1_PIN = 25;
// const int DAC_2_PIN = 26;