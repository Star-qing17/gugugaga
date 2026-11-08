# armor_detect（任务2：视频装甲板灯条与四角点识别）

读取装甲板视频，逐帧提取红色灯条，用最大外接矩形框住两根灯条，
再求灯条的旋转矩形，取长边的两个端点作为装甲板的四个角点并依次连线，
最后把结果保存成视频。

## 目录结构

```text
armor_detect/
├── CMakeLists.txt              # CMake 构建文件
├── video/
│   └── armor.mp4               # 输入视频
├── results/
│   ├── armor_result.mp4        # 程序输出的结果视频（mp4v 编码）
│   ├── armor_result_h264.mp4   # 转成 H.264 后的结果视频，Windows 可直接播放
│   └── armor_result.jpg        # 从结果视频里截取的一帧，方便放在报告里
└── src/
    └── main.cpp                # 源代码
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

必须在 armor_detect 目录下运行（代码里用的是相对路径 video/armor.mp4）：

```bash
./build/ARMOR_DETECT
```

运行后弹出 `result` 窗口显示识别结果，视频播完自动结束，
结果保存在 `results/armor_result.mp4`。

## 代码思路

1. `VideoCapture` 逐帧读视频，`VideoWriter` 把处理后的帧写成新视频。
2. 每帧转 HSV，用和任务1一样的两段 `inRange` 提取红色灯条掩膜，
   两张黑白图用 `m1 + m2` 合并。
3. `connectedComponentsWithStats` 找出所有白色连通域，
   比较 `CC_STAT_AREA` 取面积最大的两个，就是左右两根灯条。
4. 用 `CC_STAT_LEFT/TOP/WIDTH/HEIGHT` 组成 `Rect`，用黄色画出灯条的最大外接矩形。
5. 把每个灯条内部的白色像素收集成点集，用 `minAreaRect` 求最小外接旋转矩形，
   用 `rr.points(fp)` 取出 4 个顶点。
6. 旋转矩形有两条长边和两条短边，先比较边长判断哪两条是长边；
   左灯条取中点更靠左的那条长边，右灯条取中点更靠右的那条长边，
   长边的两个端点就是灯条两端的尖角，也就是装甲板的四个角点。
7. 四个角点按顺序用绿色线连接，再画一个绿色十字标出装甲板中心。
8. 每帧 `imshow` 显示并写入结果视频，最后 `release` 释放。

## 运行结果

- 黄色矩形：框住左右两根灯条的最大外接矩形。
- 绿色四边形：装甲板的四个角点，取的是灯条两端外侧的尖角，灯条倾斜时也能贴合。
- 绿色十字：四个角点的平均位置，即装甲板中心。
- 效果见 `results/armor_result_h264.mp4` 和 `results/armor_result.jpg`。
