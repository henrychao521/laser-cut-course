// 0707_midi — 優化版（2026-07-09）
// 研習成品「雷切音樂動態裝置」完整程式:三顆掃動伺服 + 連續旋轉伺服 + 單體旋律 + LED
// 變更：
//   1. 移除 DFPlayer Mini MP3 模組（SoftwareSerial / DFRobotDFPlayerMini 皆不再需要）
//   2. 喇叭單體直接接 D2，用 tone() 播放「梅岡寄情」（315021 同學譜曲，循環播放）
//   3. 全程改用 millis() 非阻塞式寫法：播音樂、掃動伺服機、LED 閃爍互不干擾
//
// 接線：
//   單體(+) → D2（建議串 100~220Ω 電阻限流；想要大聲一點可改用電晶體驅動）
//   單體(-) → GND
//   伺服機維持原本 D8 / D9 / D10 / D12
//
// 旋律陣列可用本課程站「單體作曲台」直接產生後貼換。

#include <Servo.h>

const int SPEAKER_PIN = 2;

/* ---------- 旋律（{頻率 Hz, 長度 ms}，頻率 0 = 休止符） ----------
   「梅岡寄情」（詞曲：李子恆），由 315021 同學協助譜曲
   一遍約 33 秒共 56 音，播完自動從頭循環，
   原稿譜了兩遍完全相同，已精簡為一遍 */
const int NOTE_COUNT = 56;
const int MELODY[NOTE_COUNT][2] = {
  { 392,   300},  // G4
  { 392,   600},  // G4
  { 392,   300},  // G4
  { 440,   600},  // A4
  { 523,   600},  // C5
  { 440,   600},  // A4
  { 392,  1200},  // G4
  { 330,   600},  // E4
  { 294,   300},  // D4
  { 294,   600},  // D4
  { 330,   300},  // E4
  { 294,   600},  // D4
  { 262,   600},  // C4
  { 220,  1200},  // A3
  { 262,   300},  // C4
  { 262,   600},  // C4
  { 220,   300},  // A3
  { 262,   600},  // C4
  { 440,   600},  // A4
  { 392,  1200},  // G4
  { 330,   600},  // E4
  { 294,   300},  // D4
  { 294,   600},  // D4
  { 330,   300},  // E4
  { 294,   300},  // D4
  { 262,   300},  // C4
  { 220,   600},  // A3
  { 262,  1200},  // C4
  { 330,   600},  // E4
  { 392,   600},  // G4
  { 392,   600},  // G4
  { 330,   300},  // E4
  { 294,   300},  // D4
  { 262,  1200},  // C4
  { 294,   600},  // D4
  { 330,   600},  // E4
  { 392,   600},  // G4
  { 392,   600},  // G4
  { 440,   600},  // A4
  { 392,  1200},  // G4
  { 392,   600},  // G4
  { 440,   600},  // A4
  { 523,  1200},  // C5
  { 440,   600},  // A4
  { 392,   300},  // G4
  { 392,   600},  // G4
  { 440,   300},  // A4
  { 392,   600},  // G4
  { 330,   600},  // E4
  { 294,   300},  // D4
  { 294,   600},  // D4
  { 330,   300},  // E4
  { 294,   300},  // D4
  { 262,   300},  // C4
  { 220,   600},  // A3
  { 262,  1200},  // C4
};

// ---------- 三顆來回掃動的伺服機 ----------
const int SWEEP_COUNT = 3;
const int SWEEP_PINS[SWEEP_COUNT] = {8, 9, 10};
int  angles[SWEEP_COUNT]  = {10, 60, 110};   // 原本的 a / b / c
int  minAng[SWEEP_COUNT]  = {1, 1, 1};
int  maxAng[SWEEP_COUNT]  = {178, 179, 178};
int  dirs[SWEEP_COUNT]    = { +1, -1, +1 };  // 原本的 aa / bb / cc
Servo sweepServos[SWEEP_COUNT];
Servo servo5;                                 // D12：連續旋轉（原 90-6 = 84）

const unsigned long SERVO_INTERVAL = 20;  // 每 20ms 走 1 度（原本 delay(10)×2 的速度）
const unsigned long LED_INTERVAL   = 10;  // 原本 10ms 亮 / 10ms 暗

unsigned long lastServo = 0;
unsigned long lastLed   = 0;
bool ledOn = false;

int  noteIndex = 0;
unsigned long noteStart = 0;
bool notePlaying = false;

void startNote() {
  noteStart = millis();
  int freq = MELODY[noteIndex][0];
  if (freq > 0) {
    tone(SPEAKER_PIN, freq);
    notePlaying = true;
  } else {
    noTone(SPEAKER_PIN);
    notePlaying = false;
  }
}

void updateMelody() {
  unsigned long elapsed = millis() - noteStart;
  unsigned long dur = MELODY[noteIndex][1];
  // 提早 25ms 停音，讓連續同音之間有斷開的顆粒感
  if (notePlaying && elapsed + 25 >= dur) {
    noTone(SPEAKER_PIN);
    notePlaying = false;
  }
  if (elapsed >= dur) {
    noteIndex = (noteIndex + 1) % NOTE_COUNT;  // 播完整首自動循環
    startNote();
  }
}

void updateServos() {
  if (millis() - lastServo < SERVO_INTERVAL) return;
  lastServo = millis();
  for (int i = 0; i < SWEEP_COUNT; i++) {
    angles[i] += dirs[i];
    if (angles[i] >= maxAng[i]) dirs[i] = -1;
    if (angles[i] <= minAng[i]) dirs[i] = +1;
    sweepServos[i].write(angles[i]);
  }
}

void updateLed() {
  if (millis() - lastLed < LED_INTERVAL) return;
  lastLed = millis();
  ledOn = !ledOn;
  digitalWrite(LED_BUILTIN, ledOn ? HIGH : LOW);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SPEAKER_PIN, OUTPUT);

  for (int i = 0; i < SWEEP_COUNT; i++) {
    sweepServos[i].attach(SWEEP_PINS[i]);
    sweepServos[i].write(angles[i]);
  }
  servo5.attach(12);
  servo5.write(84);   // 連續旋轉伺服機：慢速轉動（原 90-6）

  startNote();        // 開始播放旋律
}

void loop() {
  updateMelody();
  updateServos();
  updateLed();
}
