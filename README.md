# 雷射切割機在專題上的應用 — 研習課程站

2026-07-07 教師研習的完整紀錄站:三分鐘精華(站內直播)、五章課程講義、
失敗路徑、格狀鉸鏈互動計算器(嵌入 [lattice-hinge-designer](https://github.com/henrychao521/lattice-hinge-designer))、
4,805 句逐字稿全文搜尋(可深連結到完整版影片時間點)。

🌐 https://henrychao521.github.io/laser-cut-course/

## 影片上架

長片走 YouTube(未列出),上傳後把 ID 填入 `data/config.js`:

| 檔案(本機 /Volumes/128G/laser-film/) | 長度 | 填入欄位 |
|---|---|---|
| 成片_完整教學.mp4(5.8GB) | 4:21:31 | `YT_IDS.full` |
| 成片_合併版.mp4(1.7GB,內嵌字幕) | 1:12:08 | `YT_IDS.merged` |

填入後自動啟用:章節跳轉按鈕、逐字稿搜尋結果 ▶ 深連結。

## 產製說明

影片與逐字稿由 AI 管線自動產出(video-toolkit:VLM 素材盤點 → whisper 逐字稿
剪輯 → 自動字幕 → 出口 QC);站內精華版已完成臉部遮罩。
`data/transcript.js` 內的 `f` 欄位是句子在完整版剪輯時間軸上的映射秒數。
