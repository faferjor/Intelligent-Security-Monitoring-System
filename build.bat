@echo off
echo 正在构建智能安防系统...
echo.

echo 1. 构建服务端...
cd server
qmake server.pro
mingw32-make -j4
if %errorlevel% equ 0 (
    echo 服务端构建成功！
) else (
    echo 服务端构建失败！
    pause
    exit /b 1
)
cd ..
echo.

echo 2. 构建客户端...
cd client
qmake client.pro
mingw32-make -j4
if %errorlevel% equ 0 (
    echo 客户端构建成功！
) else (
    echo 客户端构建失败！
    pause
    exit /b 1
)
cd ..
echo.

echo 3. 复制模型文件...
copy /Y yolov8n.onnx server
copy /Y coco.names server
copy /Y yolov8n.onnx client
copy /Y coco.names client
echo 模型文件复制完成！
echo.

echo ========================================
echo 智能安防系统构建完成！
echo ========================================
echo.
echo 运行说明：
echo 1. 启动服务端：server\release\server.exe
echo 2. 启动客户端：client\release\client.exe
echo.
pause