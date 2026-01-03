# CX AE Plugins

After Effects 插件工具集，为动画摄影设计。

## 插件列表

| 插件 | 说明 | 状态 |
|------|------|------|
| [cx_ColorLines](plugins/cx_ColorLines/) | 主线提取与填充 | ✅ 完成 |

## 项目结构

```
CX-AE-Plugins/
├── shared/                    # 共享代码（所有插件通用）
│   └── CXCommon.h
├── plugins/                   # 各插件源码
│   └── cx_ColorLines/
│       ├── ColorLines.h
│       ├── ColorLines.cpp
│       └── ColorLinesPiPL.r
├── win/                       # Windows 构建文件
│   ├── CX-AE-Plugins.sln      # 主解决方案
│   └── cx_ColorLines/
│       └── cx_ColorLines.vcxproj
├── docs/                      # 文档
│   ├── BUILD.md
│   └── DEVELOPMENT.md
└── output/                    # 编译输出 (.aex 文件)
```

## 构建要求

- **Visual Studio 2022** (v143 工具集)
- **After Effects SDK** (25.6 或更高版本)
- **Windows 10 SDK**

## 快速开始

1. 打开 `win/CX-AE-Plugins.sln`
2. 配置 SDK 路径（如需要，在 vcxproj 中修改 `AE_SDK_PATH`）
3. 选择 Release|x64 配置
4. 构建解决方案
5. 将 `output/*.aex` 复制到 AE 插件目录

## 添加新插件

1. 在 `plugins/` 下创建新目录 `cx_NewPlugin/`
2. 添加源文件：`NewPlugin.h`, `NewPlugin.cpp`, `NewPluginPiPL.r`
3. 在 `win/` 下创建 `cx_NewPlugin/cx_NewPlugin.vcxproj`
4. 将新项目添加到 `CX-AE-Plugins.sln`

## 作者

Ma Chenxing
