# hsv_mask（任务1：HSV 分离通道 + 红色掩膜）

读取装甲板图片，转成 HSV，分离 H、S、V 三个通道分别弹窗显示，
再用 `inRange` 写阈值提取红色灯条，保存二值掩膜 `red_mask.png`。

## 目录结构

```text
hsv_mask/
├── CMakeLists.txt        # CMake 构建文件
├── images/
│   └── armor.png         # 装甲板图片
├── results/
│   ├── red_mask.png      # 程序运行后生成的红色二值掩膜
│   ├── H_channel.png     # H 通道
│   ├── S_channel.png     # S 通道
│   └── V_channel.png     # V 通道
└── src/
    └── main.cpp          # 源代码
```

## 环境准备（Ubuntu）

```bash
sudo apt install cmake build-essential libopencv-dev
```

本机使用的版本：OpenCV 4.6.0。

## 编译方法（命令行）

```bash
cmake -B build
cmake --build build
```

## 运行方法

必须在 hsv_mask 目录下运行（代码里用的是相对路径 images/armor.png）：

```bash
./build/HSV_MASK
```

运行后会弹出 H、S、V 三个通道窗口，最后弹出红色掩膜窗口，
按任意键关闭全部窗口。

## 代码思路

1. `imread` 读图片，`cvtColor(img, hsv, COLOR_BGR2HSV)` 转成 HSV。
2. `split` 把 HSV 拆成 3 个单通道图，用 `imshow` 分别显示 H、S、V。
3. 红色在 H 通道的 0 和 179 两头，所以写两段 `inRange`：
   一段取 0~10，一段取 170~179。
4. 两张黑白图直接用 `m1 + m2` 相加合并成一张掩膜。
5. `imshow` 显示掩膜，`imwrite` 保存到 `results/red_mask.png`。

## 运行结果

`results/red_mask.png` 中两根红色灯条变成白色，其余背景为黑色。
H 通道窗口可以看到灯条区域是高值，S、V 通道也能看出灯条的高饱和、高亮度。
