# M5Faces3_Keyboard

M5Stack の Faces Keyboard3（Faces3, A003-V3）をキー入力として読み取る最小サンプルです。
[M5Stack/examples/Face/KEYBOARD](https://github.com/m5stack/M5Stack/tree/master/examples/Face/KEYBOARD) を
[M5Faces](https://github.com/m5stack/M5Faces) ライブラリと M5Unified を使って Faces3 向けに移植しています。

## 内容

- Faces Keyboard3 のキー入力をI2C経由で読み取り、画面とシリアルに表示します
- Backspace / Delete キーで入力文字を正しく削除できます
- 入力位置に点滅するカーソルを表示します

## 対応ボード（PlatformIO env）

- m5stack-core2 (default)
- m5stack-cores3
- m5stack-grey
- m5stack-fire
- m5stack-core-esp32

## 必要なもの

- 上記いずれかの M5Stack 本体
- Faces Keyboard3 (A003-V3)
- [PlatformIO](https://platformio.org/)

## ビルド・書き込み

```sh
pio run -e m5stack-core2 -t upload
```

## License

[MIT](LICENSE)
