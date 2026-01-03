# cx_ColorLines

色线提取与填充插件，用于动画摄影后期处理。

## 功能

- **色线检测**：检测指定颜色的线条
- **邻近色填充**：用周围像素颜色填充线条
- **颜色调整**：亮度、对比度、饱和度调整
- **采样模糊**：线条区域内的高斯模糊

## 参数

| 参数 | 说明 |
|------|------|
| Target Color | 目标线条颜色 |
| Color Tolerance | 颜色容差 (0-100%) |
| Fill Mode | 填充模式：Nearest / Average / Weighted |
| Search Radius | 搜索半径 (1-50 px) |
| Ignore Transparent | 是否忽略透明像素 |
| Sample Blur | 采样模糊量 |
| Brightness/Contrast/Saturation | 颜色调整 |
| Output Mode | 输出模式：Full / Lines Only / BG Only |

## 详细开发文档

参见 [docs/DEVELOPMENT.md](../../docs/DEVELOPMENT.md)
