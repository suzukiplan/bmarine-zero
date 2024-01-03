# WIP: Battle Marine for VGS-Zero

「Battle Marine」（バトルマリーン）は固定画面弾幕海戦シューティングゲームです。

2013 年にスマートフォン向けゲームとしてリリース、その後 2019 年に [ファミコンに移植](https://github.com/suzukiplan/battle-marine-fc) されました。

今回、VGS-Zero 対応ソフト第1弾としてリニューアル移植。

潜水艦、魚、カニを魚雷で DESTROY THEM ALL!

鳥のう○こに気をつけろ!!

## How to Use (WIP)

### Required Hardware

本ゲームの起動には次のハードウェアが必要です:

- RaspberryPi Zero 2W
  - [https://www.switch-science.com/products/7600](https://www.switch-science.com/products/7600)
  - [https://www.amazon.co.jp/dp/B0B55MFH1D/](https://www.amazon.co.jp/dp/B0B55MFH1D/)
- HDMI ケーブル (mini HDMI Type C → HDMI Type A)
  - [https://www.amazon.co.jp/dp/B08R7BVL7T/](https://www.amazon.co.jp/dp/B08R7BVL7T/)
- USB ジョイパッド（D-Pad+A/B+Start/Select）+ 変換アダプタ
  - [https://www.amazon.co.jp/dp/B07M7SYX11/](https://www.amazon.co.jp/dp/B07M7SYX11/)
  - [https://www.amazon.co.jp/dp/B08BNFKCYM/](https://www.amazon.co.jp/dp/B08BNFKCYM/)
- USB 電源
  - [https://www.amazon.co.jp/dp/B09T3C758Q/](https://www.amazon.co.jp/dp/B09T3C758Q/)
- micro SD カード
  - [https://www.amazon.co.jp/gp/aw/d/B08PTP6KKS/](https://www.amazon.co.jp/gp/aw/d/B08PTP6KKS/)
  - 最大 20MB 程度の空き容量が必要です（ゲーム本体: 最大16MB、ファームウェア: 約4MB）
- テレビなど（以下の条件のもの）
  - HDMI入力対応
  - リフレッシュレート60Hz
  - 解像度 240x192 ピクセル以上
  - オーディオ出力対応

> ジョイパッドは上記リストのものでなくても問題ありませんが、デフォルトのキー配置が想定通りでない場合があります。
>
> キー配置は [コチラ](https://github.com/suzukiplan/vgszero?tab=readme-ov-file#configsys) のドキュメントを参考にして [config.sys](./image/config.sys) を編集してください。

### Launch Game

[image](./image) ディレクトリ配下のファイルを FAT32 フォーマットされた SD カードのルートディレクトリへ配置して、RaspberryPi Zero 2W に挿入して電源投入すればゲームが起動します。

### Joypad Usage

![joypad.png](./joypad.png)

- START ボタン: ゲーム開始・ポーズ・再開
- カーソルキー: プレイヤー移動
- B ボタン: 魚雷を発射
- A ボタン: ジャンプ

## Game Play

TODO
