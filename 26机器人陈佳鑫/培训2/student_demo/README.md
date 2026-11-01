# student_demo

实现一个 `Student` 类，用于存储学生信息（姓名、学号、分数），提供信息打印和修改成绩功能。
代码分文件组织，使用 CMake 构建，并将 Student 类打包成静态库后链接使用。

## 目录结构

```text
student_demo/
├── CMakeLists.txt        # CMake 构建文件
├── includes/
│   └── student.hpp       # Student 类声明（头文件）
├── tools/
│   └── student.cpp       # Student 类成员函数实现（静态库）
└── src/
    └── main.cpp          # main 函数，测试 Student 类
```

## 编译方法（命令行）

```bash
cmake -B build
cmake --build build
```

## 运行方法

```bash
./build/STUDENT.exe
```

按提示依次输入：姓名、学号、分数，再输入要修改的新分数。

## 运行结果示例

```text
请输入学生名字，学号，分数：
小明 1 100
学生信息如下：
姓名：小明
学号：1
分数：100
请输入你要修改的分数：
95
小明的分数修改完毕。
学生信息如下：
姓名：小明
学号：1
分数：95
```

## 构建说明

- `add_library(TOOLS STATIC tools/student.cpp)`：将类实现编译为静态库 TOOLS
- `add_executable(STUDENT src/main.cpp)`：编译主程序
- `target_link_libraries(STUDENT TOOLS)`：主程序链接静态库
