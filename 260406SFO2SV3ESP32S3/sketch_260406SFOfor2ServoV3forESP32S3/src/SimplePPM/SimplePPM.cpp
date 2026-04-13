#include "SimplePPM.h"

static SimplePPM* g_simpleInstance = nullptr;

void IRAM_ATTR simplePpmISR() {
  if (g_simpleInstance) {
    g_simpleInstance->handleISR();
  }
}

SimplePPM::SimplePPM(uint8_t pin, uint8_t channels)
  : _pin(pin),
    _channelsCount(channels),
    _currentChannel(0),
    _lastMicros(0),
    _lastFrameMicros(0),
    _newFrameFlag(false),
    _irqCount(0),
    _lastWidth(0)
{
  _channels = (volatile uint16_t*)malloc(_channelsCount * sizeof(uint16_t));
  for (uint8_t i = 0; i < _channelsCount; i++) {
    _channels[i] = 1500;
  }
}

void SimplePPM::begin() {
  g_simpleInstance = this;

  pinMode(_pin, INPUT_PULLUP);
  attachInterrupt(_pin, simplePpmISR, RISING);
}

void IRAM_ATTR SimplePPM::handleISR() {
  uint32_t now = micros();
  uint32_t w = now - _lastMicros;
  _lastMicros = now;
  _lastWidth = w;
  _irqCount++;

  // シンクパルス
  if (w > SYNC_PULSE_US) {
    _currentChannel = 0;
    _lastFrameMicros = now;
    _newFrameFlag = true;
    return;
  }

  // 通常チャンネル幅
  if (w >= MIN_PULSE_US && w <= MAX_PULSE_US) {
    if (_currentChannel < _channelsCount) {
      _channels[_currentChannel] = (uint16_t)w;
      _currentChannel++;
    }
  }
}

uint16_t SimplePPM::latestValidChannelValue(uint8_t ch, uint16_t defaultValue) {
  if (ch >= _channelsCount) return defaultValue;

  uint16_t v;
  noInterrupts();
  v = _channels[ch];
  interrupts();

  if (v < MIN_PULSE_US || v > MAX_PULSE_US) return defaultValue;
  return v;
}

uint32_t SimplePPM::latestUpdate() {
  uint32_t t;
  noInterrupts();
  t = _lastFrameMicros;
  interrupts();
  return t;
}

bool SimplePPM::isNewFrame() {
  bool f;
  noInterrupts();
  f = _newFrameFlag;
  _newFrameFlag = false;
  interrupts();
  return f;
}

uint32_t SimplePPM::irqCount() {
  uint32_t c;
  noInterrupts();
  c = _irqCount;
  interrupts();
  return c;
}

uint32_t SimplePPM::lastWidth() {
  uint32_t w;
  noInterrupts();
  w = _lastWidth;
  interrupts();
  return w;
}