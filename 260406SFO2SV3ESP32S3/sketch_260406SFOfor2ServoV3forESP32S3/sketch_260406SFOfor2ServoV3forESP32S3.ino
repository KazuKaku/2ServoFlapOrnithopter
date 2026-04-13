// 2260409 Version3 SFO on Seeed studio ESP32S3 for 2 servo CODE by K.Kakuta
// ESP32 + SimplePPM 版（PPM入力: GPIO3, 右サーボ: GPIO5, 左サーボ: GPIO6）

#include <Arduino.h>
#include <ESP32Servo.h>
#include "src/SimplePPM/SimplePPM.h"
//#include "src/PPMReader/PPMReader.h"// <PPMReader.h>
// PPM入力ピン
const uint8_t PPM_INPUT_PIN = 3;   // D2相当 (GPIO3)
const int channelAmount = 6;
SimplePPM ppm(PPM_INPUT_PIN, channelAmount);

// サーボ出力ピン
const uint8_t servo_left_pin  = 6;  // D5 (GPIO6)
const uint8_t servo_right_pin = 5;  // D4 (GPIO5)

Servo servo_left, servo_right;

// ここから下は元コードと同じ変数定義
volatile int elevator = 0;
volatile int flapamp = 0;
volatile float delaytime = 100;  // Servo speed low-increase / high-decrease (micro second)

float elapsed_time = 0;
float dt;
unsigned long current_time, prev_time;

volatile int ch3value = 1000;  // Ch3
volatile int ch1value = 1500;  // Ch1
volatile int ch2value = 1500;  // Ch2
volatile int ch4value = 1500;  // Ch4
volatile int ch5value = 1500;  // Ch5

static int servo_comm1 = 0;  // Left or Right Servo high point and low point
static int servo_comm2 = 0;  // Left or Right Servo high point and low point

volatile int rudder = 0;
float glide_deg = 0;         // Gliding angle 0=0 degree 500=90degree
static float servo_zero1 = 0; // flap angle adjust
static float servo_zero2 = 0; // flap angle adjust

void setup() {
  Serial.begin(9600);

  // サーボ初期化
  pinMode(servo_left_pin, OUTPUT);
  pinMode(servo_right_pin, OUTPUT);

  servo_left.attach(servo_left_pin, 1000, 2000);
  servo_right.attach(servo_right_pin, 1000, 2000);

  // PPM受信開始
  ppm.begin();

  // RX起動待ち
  delay(2000);
}

void loop() {
  prev_time = current_time;
  current_time = micros();
  dt = (current_time - prev_time) / 1000000.0;

  elapsed_time = elapsed_time + dt;

  // SimplePPM から各チャネルを取得
  // もともとの CH番号に合わせてインデックスを合わせています
  ch3value = ppm.latestValidChannelValue(2, ch3value);  // Ch3
  ch1value = ppm.latestValidChannelValue(0, ch1value);  // Ch1
  ch2value = ppm.latestValidChannelValue(1, ch2value);  // Ch2
  ch4value = ppm.latestValidChannelValue(3, ch4value);  // Ch4
  ch5value = ppm.latestValidChannelValue(4, ch5value);  // Ch5

  // 1000〜2000の範囲に軽くクリップしておくと安心
  auto clamp = [](int v) {
    if (v < 1000) return 1000;
    if (v > 2000) return 2000;
    return v;
  };
  ch1value = clamp(ch1value);
  ch2value = clamp(ch2value);
  ch3value = clamp(ch3value);
  ch4value = clamp(ch4value);
  ch5value = clamp(ch5value);

  // ここから先は元コードと同じロジック
  rudder   = (int)(ch1value - 1500);                    // Ch1  Flap angle incline-- AileronStick
  elevator = (int)(ch2value - 1500);                    // Ch2 Flap Angle bilateral UP&Down
  flapamp  = (int)(ch4value - 1500);                    // Ch4 Right and left Flap angle difference
  delaytime = (int)((ch5value - 950) / 5);              // Ch5 Flapping frequency

  if (ch3value > 1080) {
    // フラッピング ON
    if (elapsed_time < delaytime / 1000.0f) {
      servo_comm1 = (int)((ch3value - 1000) / 2 + 1500 + rudder - elevator + servo_zero1 + flapamp);
      servo_comm2 = (int)(1000 + (2000 - ((ch3value - 1000) / 2 + 1500)) + rudder + elevator - servo_zero2 + flapamp);

      servo_left.writeMicroseconds(servo_comm1);
      servo_right.writeMicroseconds(servo_comm2);
    }

    if ((elapsed_time > delaytime / 1000.0f) &&
        (elapsed_time < (delaytime + delaytime) / 1000.0f)) {

      servo_comm1 = (int)((ch3value - 1000) / 2 + 1500 + rudder + elevator + servo_zero1 - flapamp);
      servo_comm2 = (int)(1000 + (2000 - ((ch3value - 1000) / 2 + 1500)) + rudder - elevator - servo_zero2 - flapamp);

      // ここは左右を入れ替えるコードだったので、そのまま維持
      servo_left.writeMicroseconds(servo_comm2);
      servo_right.writeMicroseconds(servo_comm1);
    }
  } else {
    // フラッピング OFF（滑空モード）
    servo_comm1 = (int)(1500 + rudder - elevator + glide_deg);
    servo_comm2 = (int)(1500 + rudder + elevator - glide_deg);

    servo_left.writeMicroseconds(servo_comm1);
    servo_right.writeMicroseconds(servo_comm2);
  }

  if (elapsed_time > (delaytime + delaytime) / 1000.0f) {
    // 1周期終了
    elapsed_time = 0;
  }
}