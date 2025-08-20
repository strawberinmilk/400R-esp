# パーツスペック

## 全体を通したスペック

#### 電源側
- 電圧
  - 想定値: 12~14.5V
  - 動作値: 8.7~15V 
    - DCDC5V下限8V+D電圧降下0.7V ~ DCDC上限15V
  - 理論値: 8.7~15.7V
    - DCDC 上限電圧 15V+逆流保護 D 電圧降下 0.7V
- 電流
  - 逆流保護ダイオード上限5A
  - TODO: それ以前にヒューズボックスのヒューズが切れるので保護を入れる

#### DCDC配下
- 12V系電流
  - 1.5A
- 5V系電流
  - 理論値は6A

## パーツごとのスペック

### DCDC(12V)

[入出力 2.5V~15V 最大 1.5A 同期整流式昇降圧型スイッチング電源モジュールキット LTC3111 使用](https://akizukidenshi.com/catalog/g/g111212/)

- 入力
  - 2.5-15V
  - 昇降圧
  - 非絶縁
- 出力
  - 12V(可変抵抗で指定)
  - 1.5A
- TODO 事項
  - カットアウト電圧の指定
    - 5V系に合わせるなら8V、理想は蓄電池保護程度

### DCDC(5V)
[MYMGK00506ERSR 使用 5V 出力 最大 6A 大電流 DCDC コンバーターモジュールキット](https://akizukidenshi.com/catalog/g/g115108)
- 入力
  - 8-15V
  - 降圧
  - 非絶縁
- 出力
  - 5V
  - 6A
- 効率
  - 95%

### 逆流保護ダイオード
[ファストリカバリー・ダイオード 400V5A ER504](https://akizukidenshi.com/catalog/g/g102230/)

- 耐圧
  - 400V
- 平均順電流
  - 5A
- ピーク順電流
  - 150A

### ディスプレイ

[ST7032 搭載 16x2 文字 I2C LCD](https://akizukidenshi.com/catalog/g/g112486/)

### 入力デバイス
- [ロータリーエンコーダー](https://akizukidenshi.com/catalog/g/g106357/)

- [プッシュボタンスイッチ](https://akizukidenshi.com/catalog/g/g104583/)
  - 色は任意(上記データは白)