# CMake 构建指南

CX-AE-Plugins 支持使用 CMake 进行跨 IDE 构建。

## 系统要求

- **CMake 3.20+**
- **Visual Studio 2022** (MSVC v143工具集)
- **After Effects SDK 25.6**
- **Ninja** (可选，用于CLion/VS Code)

## 快速开始

### 方法1：使用 CMake 预设 (推荐)

```bash
# 配置 Debug 构建
cmake --preset windows-debug

# 构建
cmake --build build/windows-debug

# 配置 Release 构建
cmake --preset windows-release
cmake --build build/windows-release
```

### 方法2：手动配置

```bash
# 创建构建目录
mkdir build && cd build

# 配置项目
cmake .. -G "Visual Studio 17 2022" -A x64

# 构建 Debug
cmake --build . --config Debug

# 构建 Release
cmake --build . --config Release
```

### 自定义 SDK 路径

如果 AE SDK 不在默认路径，可以指定：

```bash
cmake --preset windows-debug -DAE_SDK_PATH="D:/SDK/AfterEffectsSDK/Examples"
```

或编辑 `CMakePresets.json` 中的 `AE_SDK_PATH`。

## IDE 集成

### CLion

1. **打开项目**
   - File → Open → 选择项目根目录
   - CLion 会自动检测 CMakeLists.txt

2. **选择构建配置**
   - 打开 Settings → Build, Execution, Deployment → CMake
   - 预设配置会自动加载：
     - `windows-debug`
     - `windows-release`
     - `vs2022-debug`
     - `vs2022-release`

3. **构建**
   - Build → Build Project (Ctrl+F9)
   - 或点击右上角的 Build 按钮

4. **输出位置**
   - 编译的 `.aex` 文件在 `output/` 目录

### Visual Studio 2022

1. **打开 CMake 项目**
   - File → Open → CMake → 选择根目录的 `CMakeLists.txt`

2. **或使用传统 .sln**
   - 继续使用 `win/CX-AE-Plugins.sln`（两种方式并存）

3. **选择配置**
   - 工具栏选择配置：`vs2022-debug` 或 `vs2022-release`

4. **构建**
   - Build → Build All

### VS Code

1. **安装扩展**
   - CMake Tools
   - C/C++

2. **打开项目**
   - File → Open Folder → 选择项目根目录

3. **选择预设**
   - Ctrl+Shift+P → "CMake: Select Configure Preset"
   - 选择 `windows-debug` 或 `windows-release`

4. **构建**
   - Ctrl+Shift+P → "CMake: Build"
   - 或点击底部状态栏的 Build 按钮

## 添加新插件

在 `CMakeLists.txt` 底部添加：

```cmake
add_ae_plugin(cx_NewPlugin ${CMAKE_SOURCE_DIR}/plugins/cx_NewPlugin)
```

插件需要以下文件：
- `plugins/cx_NewPlugin/cx_NewPlugin.cpp`
- `plugins/cx_NewPlugin/cx_NewPlugin.h`
- `plugins/cx_NewPlugin/cx_NewPluginPiPL.r`

## PiPL 资源编译

CMake 自动处理 PiPL 资源编译流程：

1. 预处理 `.r` 文件 → `.rr`
2. 运行 `PiPLTool` → `.rrc`
3. 再次预处理 → `.rc`
4. RC 编译器生成资源

所有中间文件在 `build/` 目录中，不会污染源码树。

## 故障排查

### SDK 路径错误

```
CMake Error: After Effects SDK not found
```

**解决**：检查 `CMakePresets.json` 中的 `AE_SDK_PATH`，或使用 `-DAE_SDK_PATH=...` 参数。

### PiPLTool 未找到

```
PiPLTool.exe not found
```

**解决**：确保 SDK 路径正确，`PiPLTool.exe` 应在 `${AE_SDK_PATH}/Resources/` 下。

### Ninja 未安装 (CLion)

**解决**：
- 安装 Ninja：`choco install ninja` (Windows)
- 或改用 VS generator：修改 `CMakePresets.json` 中的 `generator`

## 与原有 Visual Studio 项目共存

- **CMake 构建**：使用 `CMakeLists.txt`
- **VS 原生构建**：继续使用 `win/CX-AE-Plugins.sln`

两者互不干扰，可根据需要选择：
- 开发调试 → CLion/VS Code (CMake)
- 发布构建 → Visual Studio (.sln)
