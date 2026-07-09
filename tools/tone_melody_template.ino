// tone_melody_template.ino — 喇叭單體(無源蜂鳴器)旋律範例
// 搭配課程站「單體作曲台」使用：
//   1. 在作曲台按琴鍵作曲，按「複製代碼」
//   2. 貼到下面取代 NOTE_COUNT 與 MELODY 兩段
//   3. 燒錄即可循環播放
//
// 接線：單體(+) ── 100~220Ω 電阻 ── D2
//       單體(-) ── GND
// （單體阻抗低，一定要串電阻限流；想更大聲改用電晶體如 S8050 驅動）
// 適用 Arduino Uno / LinkIt 7697 等支援 tone() 的開發板

const int SPEAKER_PIN = 2;

// ↓↓↓ 用「單體作曲台」產生的代碼取代這兩段 ↓↓↓
const int NOTE_COUNT = 8;
const int MELODY[NOTE_COUNT][2] = {   // {頻率 Hz, 長度 ms}，頻率 0 = 休止符
  {262,  400}, {294,  400}, {330,  400}, {349,  400},
  {392,  400}, {440,  400}, {494,  400}, {523,  800},
};
// ↑↑↑ ------------------------------------------ ↑↑↑

void setup() {
  pinMode(SPEAKER_PIN, OUTPUT);
}

void loop() {
  for (int i = 0; i < NOTE_COUNT; i++) {
    int freq = MELODY[i][0];
    int ms   = MELODY[i][1];
    if (freq > 0) tone(SPEAKER_PIN, freq);
    delay(ms - 25);          // 提早 25ms 停音，讓連續同音之間有顆粒感
    noTone(SPEAKER_PIN);
    delay(25);
  }
  delay(1000);               // 播完整首，停一秒再循環
}
