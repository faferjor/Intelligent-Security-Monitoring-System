@echo off
echo Copying YOLOv8 model files to server and client build directories...

:: 获取当前目录
set "CURRENT_DIR=%~dp0"

:: 定义模型文件路径
set "MODEL_FILE=%CURRENT_DIR%yolov8n.onnx"
set "CLASSES_FILE=%CURRENT_DIR%coco.names"

:: 检查模型文件是否存在
if not exist "%MODEL_FILE%" (
    echo ERROR: yolov8n.onnx not found in current directory!
    echo Please download it from: https://github.com/ultralytics/assets/releases/download/v0.0.0/yolov8n.onnx
    pause
    exit /b 1
)

if not exist "%CLASSES_FILE%" (
    echo ERROR: coco.names not found in current directory!
    echo Please download it from: https://raw.githubusercontent.com/pjreddie/darknet/master/data/coco.names
    pause
    exit /b 1
)

:: 复制到服务端构建目录
echo.
echo Copying to server build directories...

:: 尝试常见的构建目录结构
for /d %%d in ("%CURRENT_DIR%server\build*", "%CURRENT_DIR%server\*Debug", "%CURRENT_DIR%server\*Release") do (
    if exist "%%d" (
        echo Found server build directory: %%d
        
        :: 复制到 debug 子目录
        if exist "%%d\debug" (
            copy "%MODEL_FILE%" "%%d\debug\" >nul
            copy "%CLASSES_FILE%" "%%d\debug\" >nul
            echo Copied to: %%d\debug
        )
        
        :: 复制到 release 子目录
        if exist "%%d\release" (
            copy "%MODEL_FILE%" "%%d\release\" >nul
            copy "%CLASSES_FILE%" "%%d\release\" >nul
            echo Copied to: %%d\release
        )
        
        :: 直接复制到构建目录根
        copy "%MODEL_FILE%" "%%d\" >nul
        copy "%CLASSES_FILE%" "%%d\" >nul
        echo Copied to: %%d
    )
)

:: 复制到客户端构建目录
echo.
echo Copying to client build directories...

for /d %%d in ("%CURRENT_DIR%client\build*", "%CURRENT_DIR%client\*Debug", "%CURRENT_DIR%client\*Release") do (
    if exist "%%d" (
        echo Found client build directory: %%d
        
        :: 复制到 debug 子目录
        if exist "%%d\debug" (
            copy "%MODEL_FILE%" "%%d\debug\" >nul
            copy "%CLASSES_FILE%" "%%d\debug\" >nul
            echo Copied to: %%d\debug
        )
        
        :: 复制到 release 子目录
        if exist "%%d\release" (
            copy "%MODEL_FILE%" "%%d\release\" >nul
            copy "%CLASSES_FILE%" "%%d\release\" >nul
            echo Copied to: %%d\release
        )
        
        :: 直接复制到构建目录根
        copy "%MODEL_FILE%" "%%d\" >nul
        copy "%CLASSES_FILE%" "%%d\" >nul
        echo Copied to: %%d
    )
)

:: 复制到项目根目录（备用）
echo.
echo Copying to project root directory...
copy "%MODEL_FILE%" "%CURRENT_DIR%" >nul
copy "%CLASSES_FILE%" "%CURRENT_DIR%" >nul
echo Copied to: %CURRENT_DIR%

echo.
echo Done! Model files have been copied to all detected build directories.
echo.
echo Press any key to exit...
pause >nul