# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

CX-AE-Plugins is a collection of After Effects plugins designed for animation photography workflows. All plugins use the After Effects SDK 25.6 with the SmartFX architecture for multi-bit-depth support (8-bit, 16-bit, 32-bit float) and multi-threaded rendering.

## Build Commands

### CMake (Cross-IDE) - Recommended

**Quick build with presets:**
```bash
cmake --preset windows-release
cmake --build build/windows-release
```

**Custom SDK path:**
```bash
cmake --preset windows-release -DAE_SDK_PATH="D:/path/to/ae_sdk/Examples"
```

**Available presets:**
- `windows-debug` / `windows-release` - Ninja generator for CLion/VS Code
- `vs2022-debug` / `vs2022-release` - Visual Studio generator

**IDE-specific:**
- **CLion**: File → Open project root → Auto-detects CMakeLists.txt → Select preset → Build
- **VS Code**: Open folder → Select preset (Ctrl+Shift+P: CMake: Select Configure Preset) → Build
- **Visual Studio 2022**: File → Open → CMake → Select CMakeLists.txt

See [docs/CMAKE.md](docs/CMAKE.md) for detailed CMake usage.

### Visual Studio (Traditional)
- **Open solution**: `win/CX-AE-Plugins.sln`
- **Build all**: Select Release|x64 configuration, Build > Build Solution (F7)
- **Output location**: Compiled `.aex` files appear in `output/`

### Build Requirements
- Visual Studio 2022 (v143 toolset) with C++20 support
- CMake 3.20+ (for CMake builds)
- After Effects SDK 25.6 (default path: `C:\Users\tammc\Documents\0_Develop\ae25.6_61.64bit.AfterEffectsSDK\Examples`)
- Windows 10 SDK
- Ninja (optional, for CLion/VS Code)

### Language Standard
- **C++20** (ISO/IEC 14882:2020)
- See [docs/CPP20.md](docs/CPP20.md) for recommended C++20 features to use

### Plugin Installation
Copy `output/*.aex` to After Effects plugins directory:
`C:\Program Files\Adobe\Adobe After Effects 2025\Support Files\Plug-ins\`

## Architecture

### Multi-Plugin Repository Structure
```
CX-AE-Plugins/
├── shared/              # Common utilities (CXCommon.h)
├── plugins/             # Individual plugin source directories
│   └── cx_ColorLines/  # Plugin-specific code
├── win/                 # Windows build files
│   ├── CX-AE-Plugins.sln
│   └── cx_ColorLines/   # Per-plugin vcxproj
├── docs/                # Documentation
└── output/              # Build output (.aex files)
```

### Shared Components (CXCommon.h)
The `shared/CXCommon.h` file contains utilities used across all plugins:
- **Bit-depth clamping**: `CX_ClampByte()`, `CX_Clamp16()`, `CX_Clamp01()` for 8/16/32-bit
- **Row pointer access**: `CX_GetRow8()`, `CX_GetRow16()`, `CX_GetRowFloat()` for efficient pixel iteration
- **RGB/HSL conversion**: `CX_RGBtoHSL()`, `CX_HSLtoRGB()` for color space operations
- **Common macros**: `CX_MIN()`, `CX_MAX()`, `CX_CLAMP()`

All plugins should use these utilities instead of implementing their own.

### SmartFX Plugin Architecture
All plugins use the SmartFX architecture (not traditional Render):
- **Entry point**: `EffectMain()` handles commands via switch statement
- **Key commands**:
  - `PF_Cmd_GLOBAL_SETUP`: Declare bit-depth and threading support
  - `PF_Cmd_PARAMS_SETUP`: Define UI parameters
  - `PF_Cmd_SMART_PRE_RENDER`: Request input checkout, allocate custom data
  - `PF_Cmd_SMART_RENDER`: Execute rendering via iterate callbacks
- **Bit-depth handling**: Detect pixel format with `PF_GetPixelFormat()`, dispatch to separate 8/16/32-bit processing functions
- **Multi-pass processing**: Use custom data structures (like `ColorLinesInfo`) passed via extra->output_request_receipt for complex algorithms requiring multiple iterations

### PiPL Resource System
Each plugin requires a `.r` PiPL resource file compiled via custom build step:
1. Preprocessor expands `.r` to `.rr`
2. PiPLTool converts `.rr` to `.rrc`
3. Preprocessor generates final `.rc` for resource compiler
4. Declares plugin metadata: name, category, version, flags (DEEP_COLOR_AWARE, FLOAT_COLOR_AWARE, SUPPORTS_SMART_RENDER, SUPPORTS_THREADED_RENDERING)

### Parameter System
- **Parameter IDs**: Enum with unique sequential IDs (PLUGIN_INPUT, PLUGIN_PARAM_1, etc.)
- **Disk IDs**: Separate enum for persistent project compatibility (never reuse retired IDs)
- **Parameter groups**: Use `PF_ADD_TOPIC()`/`PF_END_TOPIC()` for collapsible UI sections
- **Reading parameters**: In SmartFX, use `PF_CHECKOUT_PARAM()` at current_time, not params[] array

## Adding a New Plugin

### Using CMake (Recommended)

1. **Create plugin directory**: `plugins/cx_NewPlugin/`
2. **Create source files**:
   - `NewPlugin.h` - Parameter enums, data structures, function declarations
   - `NewPlugin.cpp` - EffectMain implementation, rendering logic
   - `NewPluginPiPL.r` - PiPL resource definition
3. **Register in CMake**: Add to `CMakeLists.txt`:
   ```cmake
   add_ae_plugin(cx_NewPlugin ${CMAKE_SOURCE_DIR}/plugins/cx_NewPlugin)
   ```
4. **Include shared header**: `#include "CXCommon.h"` for common utilities
5. **Build**: `cmake --build build/windows-release`

The `add_ae_plugin()` function automatically handles:
- Source file compilation
- PiPL resource processing
- Include paths setup
- .aex output generation

### Using Visual Studio Projects

1. Follow steps 1-2 above
2. **Create VS project**: `win/cx_NewPlugin/cx_NewPlugin.vcxproj`
   - Copy existing vcxproj, update paths and GUIDs
   - Configure include paths: `$(CX_PLUGINS_ROOT)\shared`, plugin directory, SDK paths
   - Add custom build step for PiPL compilation
3. **Add to solution**: Open `CX-AE-Plugins.sln`, Add Existing Project

## cx_ColorLines Implementation Details

### Core Algorithm: Multi-Pass Color Line Processing
1. **Pass 1 (FillAndMask)**: Detect target color pixels, fill with neighbor sampling, build line mask
2. **Pass 2 (BlurPass)**: Apply Gaussian blur only within masked line regions

### Fill Modes
- **Nearest**: Ring-based search (1 to radius) with early termination when adjacent pixel found
- **Average**: Equal-weighted average of all valid neighbors in radius
- **Weighted**: Distance-inverse weighted average (`weight = 1/(dist+0.1)`)

### Performance Optimizations (docs/DEVELOPMENT.md section 9)
- **Precomputed lookup tables**: Distance weights and Gaussian weights (avoid sqrt/exp per pixel)
- **Squared distance comparison**: Compare `distSq <= toleranceSq` instead of `dist <= tolerance`
- **Row pointer caching**: Get row pointer once, use pointer arithmetic for pixel access
- **Ring search optimization**: NEAREST mode searches outward from pixel, stops at first valid neighbor
- **Multiplication vs division**: Precompute `invWeight = 1/totalWeight`, multiply channels

### Coordinate Mapping
- Uses `x_offset`/`y_offset` in ColorLinesInfo for proper coordinate translation between request extent and input world
- Always map output coordinates to source world coordinates when accessing neighbors

### Edge Protection
Skip processing pixels within `searchRadius` pixels of image edges to prevent neighbor lookup out-of-bounds errors.

## Chinese Language Usage

README and development docs are in Chinese (Traditional Chinese character set). This is intentional for the target user base. Maintain Chinese for user-facing documentation and comments in docs/, but code comments should remain in English.
