#pragma once
#include <Arduino.h>

class SimplePPM {
public:
  // pin: PPM入力ピン, channels: 読みたいch数
  SimplePPM(uint8_t pin, uint8_t channels);

  // setup() から呼ぶ初期化
  void begin();

  void IRAM_ATTR handleISR();

  // ch値取得
  uint16_t latestValidChannelValue(uint8_t ch, uint16_t defaultValue = 1500);
  uint32_t latestUpdate();
  bool isNewFrame();

  uint32_t irqCount();
  uint32_t lastWidth();

private:
  uint8_t _pin;
  uint8_t _channelsCount;

  volatile uint16_t* _channels;
  volatile uint8_t _currentChannel;
  volatile uint32_t _lastMicros;
  volatile uint32_t _lastFrameMicros;
  volatile bool _newFrameFlag;

  volatile uint32_t _irqCount;
  volatile uint32_t _lastWidth;

  static const uint16_t SYNC_PULSE_US  = 4500;  // シンク判定
  static const uint16_t MIN_PULSE_US   = 800;
  static const uint16_t MAX_PULSE_US   = 2200;
};

void IRAM_ATTR simplePpmISR();