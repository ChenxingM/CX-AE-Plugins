# C++20 特性使用指南

本项目使用 C++20 标准。以下是在 After Effects 插件开发中最有用的 C++20 特性。

## 已启用的 C++20 特性

### 1. Designated Initializers (指定初始化器)

**用途**：更清晰的结构体初始化

```cpp
// C++17 之前
ColorLinesInfo info;
info.targetColor = targetColor;
info.tolerance = tolerance;
info.fillMode = fillMode;
// ...

// C++20
ColorLinesInfo info {
    .targetColor = targetColor,
    .tolerance = tolerance,
    .fillMode = fillMode,
    .searchRadius = 5,
    .ignoreTransparent = true
};
```

### 2. constexpr 改进

**用途**：编译时常量计算，减少运行时开销

```cpp
// 编译时计算颜色距离阈值
constexpr PF_FpLong calculateMaxDistance(PF_FpLong tolerance) {
    return tolerance * 4.4167;  // sqrt(255^2 * 3)
}

// 编译时查找表生成
constexpr auto generateGaussianLUT() {
    std::array<PF_FpLong, 101> lut{};
    for (int i = 0; i <= 100; ++i) {
        lut[i] = std::exp(-i * i / 2000.0);
    }
    return lut;
}
```

### 3. Concepts (概念)

**用途**：类型约束，更清晰的模板接口

```cpp
#include <concepts>

// 定义像素类型概念
template<typename T>
concept PixelType = requires(T p) {
    { p.red } -> std::convertible_to<int>;
    { p.green } -> std::convertible_to<int>;
    { p.blue } -> std::convertible_to<int>;
    { p.alpha } -> std::convertible_to<int>;
};

// 通用像素处理函数
template<PixelType P>
void ProcessPixel(P* pixel, const ColorLinesInfo* info) {
    // 编译时保证 P 有 RGBA 成员
}
```

### 4. std::span (范围视图)

**用途**：安全的数组/缓冲区访问，无拷贝开销

```cpp
#include <span>

// 不需要传递指针+长度，一个 span 搞定
void ProcessRowSafe(std::span<PF_Pixel8> row, const ColorLinesInfo* info) {
    for (auto& pixel : row) {
        // 自动边界检查（debug 模式）
        ProcessPixel(&pixel, info);
    }
}

// 使用
PF_Pixel8* rowPtr = CX_GetRow8(world, y);
auto rowSpan = std::span(rowPtr, world->width);
ProcessRowSafe(rowSpan, info);
```

### 5. Ranges (范围库)

**用途**：简洁的数据处理管道

```cpp
#include <ranges>

// 过滤并处理符合条件的像素
auto validPixels = pixels
    | std::views::filter([](auto& p) { return p.alpha > 0; })
    | std::views::transform([](auto& p) { return processPixel(p); });

// 延迟计算，只在需要时才处理
for (auto& result : validPixels) {
    // ...
}
```

### 6. Lambda 改进

**C++20 的 lambda 支持模板参数**：

```cpp
// 泛型 lambda with explicit template
auto clamp = []<typename T>(T value, T min, T max) {
    return value < min ? min : (value > max ? max : value);
};

auto clamped8 = clamp(value, 0, 255);           // T = int
auto clamped16 = clamp(value, 0, PF_MAX_CHAN16); // T = int
auto clampedF = clamp(value, 0.0, 1.0);          // T = double
```

### 7. consteval (立即函数)

**用途**：强制编译时求值

```cpp
consteval int square(int x) {
    return x * x;
}

// 必须在编译时计算
constexpr int result = square(5);  // OK
// int runtime = square(n);  // 错误：n 不是编译时常量
```

### 8. [[likely]] / [[unlikely]] 属性

**用途**：优化分支预测

```cpp
PF_Boolean IsTargetColor(PF_Pixel8* pixel, ...) {
    if (pixel->alpha == 0) [[unlikely]] {
        return false;  // 编译器优化：假设很少执行
    }

    if (IsInRange(pixel)) [[likely]] {
        return true;   // 编译器优化：假设经常执行
    }

    return false;
}
```

### 9. using enum (枚举简化)

**用途**：简化枚举使用

```cpp
void SetFillMode(FillMode mode) {
    using enum FillMode;  // C++20

    switch (mode) {
        case FILL_MODE_NEAREST:   // 无需 FillMode:: 前缀
            // ...
            break;
        case FILL_MODE_AVERAGE:
            // ...
            break;
        case FILL_MODE_WEIGHTED:
            // ...
            break;
    }
}
```

### 10. std::source_location

**用途**：更好的日志和调试

```cpp
#include <source_location>

void LogError(const char* message,
              std::source_location loc = std::source_location::current()) {
    printf("[%s:%d:%s] %s\n",
           loc.file_name(),
           loc.line(),
           loc.function_name(),
           message);
}

// 自动捕获调用位置
LogError("Invalid parameter");
// 输出：[ColorLines.cpp:123:FillPixel8] Invalid parameter
```

## 推荐的现代化改进

### 当前代码可以改进的地方

1. **用 std::span 替代原始指针**
   ```cpp
   // 旧
   void ProcessRow(PF_Pixel8* row, A_long width);

   // 新
   void ProcessRow(std::span<PF_Pixel8> row);
   ```

2. **用 designated initializers 初始化结构**
   ```cpp
   // CXCommon.h 中的内联函数可以用 constexpr
   constexpr PF_FpLong CX_Clamp01(PF_FpLong value) {
       return value < 0.0 ? 0.0 : (value > 1.0 ? 1.0 : value);
   }
   ```

3. **枚举用 enum class（强类型）**
   ```cpp
   enum class FillMode : A_long {
       Nearest = 1,
       Average = 2,
       Weighted = 3
   };
   ```

## 性能提示

- **constexpr 函数**：编译时计算，零运行时开销
- **std::span**：零拷贝抽象，debug 模式有边界检查
- **Concepts**：编译时类型检查，无运行时成本
- **[[likely]]/[[unlikely]]**：帮助 CPU 分支预测，提升性能

## MSVC 2022 支持情况

✅ **完全支持**：
- Designated initializers
- constexpr/consteval
- Concepts
- std::span
- Ranges (部分)
- Lambda 模板
- [[likely]]/[[unlikely]]
- using enum
- std::source_location

⚠️ **部分支持**：
- Modules（AE SDK 不支持）
- Coroutines（不适用于 AE 插件）

## 迁移建议

逐步引入 C++20 特性：
1. ✅ 首先在新代码中使用 constexpr 和 designated initializers
2. ✅ 用 std::span 包装不安全的指针操作
3. ✅ 添加 [[likely]]/[[unlikely]] 到热点代码路径
4. 🔄 后续重构时逐步引入 concepts 和 ranges

## 参考资料

- [C++20 Reference](https://en.cppreference.com/w/cpp/20)
- [MSVC C++20 Features](https://docs.microsoft.com/en-us/cpp/overview/visual-cpp-language-conformance)
