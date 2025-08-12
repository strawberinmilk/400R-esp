# 400R-esp

Nissan Skyline RV37 400R 用の ESP32 ベース車載制御システム

## 📋 概要

このプロジェクトは、ESP32 マイコンを使用して Nissan Skyline RV37 400R の各種電装品を制御するシステムです。ロータリーエンコーダと LCD ディスプレイによる直感的なユーザーインターフェースを提供し、フットライト等の制御を行います。

## 🚗 対象車両

- **車種**: Nissan Skyline RV37 400R
- **年式**: 2019
- **エンジン**: VR30DDTT

## ⚙️ ハードウェア仕様

### マイコン

- **ESP32 DevKit V1** (ESP32-WROOM-32)

### 電圧変換
- **MYMGK00506ERSR使用 5V出力 最大6A 大電流DCDCコンバーターモジュールキット** 
  - URL: https://akizukidenshi.com/catalog/g/g115108  
  - 販売コード: 115108

### ディスプレイ

- **ST7032 搭載 16x2 文字 I2C LCD**
  - データシート: `dataSheet/st7032.pdf`
  - リンク: https://akizukidenshi.com/catalog/g/g112486/
  - 販売コード: 112486

### 入力デバイス

- **ロータリーエンコーダー** (24クリックタイプ)
  - リンク: https://akizukidenshi.com/catalog/g/g106357/
  - 販売コード: 106357
- **プッシュボタンスイッチ**

### 出力デバイス

- **フットライト** (PWM 制御)
  - 12V テープ LED(ピンク)

## 🏗️ ソフトウェア構成

### アーキテクチャ

```
src/
├── main.cpp                    # メインプログラム
├── config/                     # 設定・定数管理
│   ├── pinConfig.h/.cpp       # ピン配置定義
├── interface/                  # ハードウェア抽象化層
│   ├── display.h/.cpp         # LCD制御
│   ├── encoder.h/.cpp         # エンコーダー制御
│   └── button.h/.cpp          # ボタン制御
├── output/                     # 出力制御層
│   └── footLight.h/.cpp       # フットライト制御
└── setting/                    # UI・設定管理層
    ├── BaseSetting.h          # 設定基底クラス
    ├── manager.h/.cpp         # 設定マネージャー
    └── feature/               # 各種機能
        ├── standbyMode.h/.cpp
        ├── modeSelect.h/.cpp
        ├── FootLightModeSetting.h/.cpp
        └── footLightVolumeSetting.h/.cpp
```

### 設計パターン

- **継承ベースの設計**: `BaseSetting`抽象クラスによる統一インターフェース
- **状態管理パターン**: `SettingManager`による機能切り替え
- **ハードウェア抽象化**: インターフェース層によるハードウェア依存の分離

## 🔧 開発環境

### 必要なソフトウェア

- [PlatformIO IDE](https://platformio.org/platformio-ide) または [PlatformIO CLI](https://platformio.org/install/cli)
- [VS Code](https://code.visualstudio.com/) (推奨)

### 依存ライブラリ

```ini
lib_deps =
    madhephaestus/ESP32Encoder@^0.11.8
    asukiaaa/ST7032_asukiaaa@^1.0.5
```

## 🚀 ビルド・デプロイ手順

### 1. リポジトリのクローン

```bash
git clone https://github.com/strawberinmilk/400R-esp.git
cd 400R-esp
```

### 2. ビルド

```bash
platformio run
```

### 3. アップロード

ESP32 を COM6 ポートに接続後：

```bash
platformio run --target upload
```

### 4. シリアルモニター

```bash
platformio device monitor --baud 115200
```

## 📟 使用方法

### 基本操作

1. **電源投入**: "SKYLINE RV37 400R Welcome!" が表示
2. **メニュー選択**: ロータリーエンコーダーで項目選択
3. **決定**: SELECT_SW ボタンで選択実行
4. **戻る**: 各機能から SELECT_SW ボタンでスタンバイモードに戻る

### 機能一覧

#### 🔦 フットライト制御

**Volume 設定**

- 明度調整 (0-255 段階)
- PWM 制御による滑らかな調光

**Mode 設定**

- `OFF`: 消灯
- `ON`: 常時点灯
- `Auto Side Brake`: サイドブレーキ連動 (実装予定)
- `Auto Illumi`: イルミネーション連動 (実装予定)

## 🛠️ 開発・カスタマイズ

### 新機能の追加

1. `src/setting/feature/` に新しいクラスを作成
2. `BaseSetting` を継承
3. `start()`, `update()`, `cleanup()` を実装
4. `SettingManager` に登録

### 例: 新機能追加

```cpp
class NewFeature : public BaseSetting {
public:
    NewFeature() { name = "New Feature"; }
    void start() override { /* 初期化処理 */ }
    void update() override { /* 更新処理 */ }
    void cleanup() override { /* 終了処理 */ }
};
```

## 📈 今後の拡張予定

- [ ] 設定の不揮発性メモリ保存 (EEPROM/SPIFFS)
- [ ] OTA (Over-The-Air) 更新機能
- [ ] CAN 通信対応
- [ ] 追加センサー対応 (温度、電圧監視等)
- [ ] スマートフォン連携 (Bluetooth/Wi-Fi)
- [ ] ログ機能とデータ解析

## 🔒 安全性について

### 車載使用時の注意事項

⚠️ **重要**: このシステムを車両に実装する際は、以下の安全基準を遵守してください：

- 運転中の操作は避け、停車時のみ設定変更を行う
- 電源系統の適切な保護回路を実装する
- 車両の基本安全システムに影響しない配線を行う
- 定期的な動作確認とメンテナンスを実施する

## 🐛 トラブルシューティング

### よくある問題

**Q: LCD が表示されない**
A: I2C 接続を確認。SDA/SCL ピンとプルアップ抵抗をチェック

**Q: エンコーダーが反応しない**
A: エンコーダーのピン接続とチャタリング除去回路を確認

**Q: ビルドエラーが発生する**
A: PlatformIO の依存ライブラリを最新版に更新

```bash
platformio lib update
```

## 📄 ライセンス

このプロジェクトは [MIT License](LICENSE) の下で公開されています。

## 🤝 コントリビューション

プルリクエストやイシューの報告を歓迎します！

### 開発ガイドライン

1. コードスタイルは既存に合わせる
2. コメントは日本語で記述
3. 変更には適切なテストを含める
4. コミットメッセージは簡潔で明確に

## 📞 サポート

質問やサポートが必要な場合は、[Issues](https://github.com/strawberinmilk/400R-esp/issues) でお気軽にお問い合わせください。

---

**Project Status**: ✅ Active Development  
**Last Updated**: 2025 年 8 月 10 日  
**Version**: 1.0.0

Made with ❤️ for RV37 400R owners
