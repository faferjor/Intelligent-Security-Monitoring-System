# 智能安防监控系统

基于 Qt 6.5 + OpenCV + YOLOv8 构建的智能安防监控系统，采用 C/S 架构与多线程解耦设计。

## 功能特性

### 🎯 核心功能
- **实时目标检测**：基于 YOLOv8 实现行人、车辆、烟火等 80 类目标检测
- **C/S 架构设计**：支持多客户端同时连接，实现分布式监控
- **多线程处理**：摄像头采集、模型推理、网络通信独立线程，确保系统流畅性
- **异常报警**：检测到特定目标时自动触发报警并记录日志
- **区域入侵检测**：自定义警戒区域，检测非法入侵行为
- **越线检测**：设置警戒线，监控越线事件
- **人员聚集检测**：监控区域内人员数量，超过阈值自动报警
- **运动检测**：基于背景建模的运动检测

### 📷 视频功能
- **实时监控**：本地摄像头实时预览与远程传输
- **自动抓拍**：支持手动抓拍与异常事件自动抓拍
- **视频录制**：实时视频录制与保存
- **数据库记录**：使用 SQLite 存储检测历史数据

### 🎨 界面优化
- **QSS 样式美化**：专业美观的界面样式，支持多种按钮主题
- **分组框布局**：控件分类组织，界面清晰易用
- **响应式设计**：窗口自适应调整
- **多语言支持**：预留中英文切换接口

## 技术架构

### 技术栈
- **框架**：Qt 6.5.3（C++）
- **视觉库**：OpenCV 4.8.0
- **深度学习**：YOLOv8n（ONNX 模型）
- **网络通信**：Qt TCP Socket
- **数据库**：SQLite
- **多线程**：Qt Thread、信号槽机制

### 架构设计
- **MVC 架构**：模型-视图-控制器分离设计
- **模块化**：功能模块化，便于维护与扩展
- **解耦设计**：各模块通过信号槽通信，降低耦合度

## 项目结构

```
Intelligent-Security-Monitoring-System/
├── server/                 # 服务端代码
│   ├── server.pro         # 服务端项目配置
│   ├── styles.qss         # QSS 样式文件
│   ├── main.cpp           # 服务端入口
│   ├── mainwindow.h/cpp   # 主窗口界面
│   ├── tcpserver.h/cpp    # TCP 服务端实现
│   ├── detectormodel.h/cpp # YOLO 模型推理
│   └── database.h/cpp     # 数据库操作
├── client/                 # 客户端代码
│   ├── client.pro         # 客户端项目配置
│   ├── main.cpp           # 客户端入口
│   ├── mainwindow.h/cpp   # 主窗口界面
│   ├── tcpclient.h/cpp    # TCP 客户端实现
│   └── detectormodel.h/cpp # YOLO 模型推理
├── yolov8n.onnx           # YOLOv8 模型文件
├── coco.names             # COCO 类别名称
├── build.bat              # 一键编译脚本
└── README.md              # 项目说明文档
```

## 环境配置

### 依赖要求
- Qt 6.5.3 或更高版本
- OpenCV 4.8.0（需自行编译或下载预编译版本）
- MinGW 64 位编译器
- CMake 3.16+（如需自行编译 OpenCV）

### OpenCV 配置
1. 下载或编译 OpenCV 4.8.0
2. 修改 `server/server.pro` 和 `client/client.pro` 中的 OpenCV 路径：
```qmake
INCLUDEPATH += D:/opencv/build/install/include
LIBS += -LD:/opencv/build/lib \
    -lopencv_core480 \
    -lopencv_imgproc480 \
    -lopencv_highgui480 \
    -lopencv_imgcodecs480 \
    -lopencv_videoio480 \
    -lopencv_dnn480
```

### 模型文件配置
请确保以下文件位于项目根目录：
- `yolov8n.onnx` - YOLOv8 模型文件
- `coco.names` - COCO 类别名称文件

## 快速开始

### 1. 启动服务端
1. 编译服务端：`cd server && qmake && mingw32-make`
2. 运行服务端程序 `release/server.exe`
3. 点击 "Start Server" 按钮启动服务（默认端口 8888）
4. 点击 "Start Camera" 启动本地摄像头
5. 在设置中配置检测参数和告警阈值

### 2. 启动客户端
1. 编译客户端：`cd client && qmake && mingw32-make`
2. 运行客户端程序 `release/client.exe`
3. 输入服务端 IP 地址和端口（默认 127.0.0.1:8888）
4. 点击 "Connect" 连接服务端
5. 点击 "Start Camera" 启动本地摄像头

### 3. 功能使用
- **实时监控**：启动摄像头后自动显示实时画面
- **目标检测**：YOLOv8 自动检测画面中的目标并绘制 bounding box
- **区域入侵检测**：勾选 "Enable Area Detection" 并绘制检测区域
- **越线检测**：勾选 "Enable Line Crossing" 并绘制警戒线
- **人员聚集检测**：设置人数阈值，超过自动报警
- **运动检测**：勾选 "Enable Motion Detection" 启用运动检测
- **手动抓拍**：点击 "Capture" 按钮保存当前画面
- **视频录制**：服务端支持点击 "Record" 开始/停止录制
- **设置管理**：在服务端设置对话框中配置参数并保存/重置

## 模型文件

### YOLOv8n.onnx
- 下载地址：`https://github.com/ultralytics/assets/releases/download/v0.0.0/yolov8n.onnx`
- 模型大小：约 6MB
- 输入尺寸：640x640
- 输出：8400x85（80 类目标 + 5 个检测参数）

### coco.names
- 包含 80 个 COCO 数据集类别名称
- 下载地址：`https://raw.githubusercontent.com/pjreddie/darknet/master/data/coco.names`

## 一键编译

Windows 用户可以直接使用 `build.bat` 脚本一键编译服务端和客户端：
```batch
build.bat
```

编译完成后，可执行文件位于：
- 服务端：`server/release/server.exe`
- 客户端：`client/release/client.exe`

## 开发说明

### 代码规范
- 遵循 Qt 编码规范
- 使用驼峰命名法
- 类名首字母大写，函数名首字母小写
- 适当添加注释，提高代码可读性

### 扩展开发
- 添加新功能模块时遵循 MVC 架构
- 使用信号槽机制进行线程间通信
- 避免在 UI 线程中执行耗时操作
- 新增网络协议时需考虑兼容性

### QSS 样式定制
- 修改 `server/styles.qss` 文件自定义界面样式
- 支持多种按钮主题：普通按钮、危险按钮、警告按钮、成功按钮等
- 样式文件在程序启动时自动加载

## 常见问题

### Q: 无法加载模型文件
A: 请确保 `yolov8n.onnx` 和 `coco.names` 文件位于程序运行目录下，或在代码中指定完整路径。

### Q: OpenCV 链接错误
A: 请检查 `pro` 文件中的 OpenCV 路径是否正确，确保使用的 MinGW 版本与编译 OpenCV 时的版本一致。

### Q: 摄像头无法打开
A: 请确保摄像头未被其他程序占用，检查设备权限，或尝试修改摄像头索引（默认 0）。

### Q: 客户端无法连接服务端
A: 请检查网络连接，确保服务端已启动，端口未被防火墙阻止。

### Q: 检测效果不理想
A: 可以在服务端设置对话框中调整置信度阈值和 NMS 阈值，或使用更高精度的 YOLOv8s 模型。

## 版本历史

### v1.1.0（2026-06-01）
- ✅ 界面布局优化，使用分组框组织控件
- ✅ 添加 QSS 样式美化，多种按钮主题
- ✅ 实现语言切换功能（预留中英文接口）
- ✅ 添加区域入侵检测功能
- ✅ 实现越线检测功能
- ✅ 添加人员聚集检测功能
- ✅ 实现运动检测功能
- ✅ 集成 SQLite 数据库
- ✅ 优化客户端界面，去掉冗余控件
- ✅ 一键编译脚本

### v1.0.0（2026-05-26）
- 🎉 基础版本发布
- ✅ 实现核心目标检测功能
- ✅ 完成 C/S 架构通信
- ✅ 支持实时监控与报警
- ✅ 添加视频录制功能
- ✅ 实现手动抓拍功能

## 未来计划

- [ ] 支持更多语言切换
- [ ] 添加历史数据查询界面
- [ ] 实现远程配置管理
- [ ] 支持自定义检测模型
- [ ] 添加多人脸识别功能
- [ ] 实现视频回放功能
- [ ] 添加移动端 APP
- [ ] 支持云端存储

## 许可证

本项目基于 MIT 许可证开源，详见 LICENSE 文件。

## 致谢

感谢以下开源项目：
- [Qt](https://www.qt.io/) - 跨平台应用框架
- [OpenCV](https://opencv.org/) - 计算机视觉库
- [Ultralytics YOLOv8](https://github.com/ultralytics/ultralytics) - 目标检测模型

## 联系方式

如有问题或建议，欢迎提交 Issue 或联系开发者。
