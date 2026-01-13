/*
 * PencilLine.h
 * cx_PencilLine - Pencil Line Texture Effect for After Effects
 *
 * Extracts multiple target colors and applies pencil line texture processing.
 * Supports up to 16 colors, each with individual enable/disable and tolerance.
 */

#pragma once

#include "AEConfig.h"
#include "entry.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_Macros.h"
#include "Param_Utils.h"
#include "AE_EffectCBSuites.h"
#include "String_Utils.h"
#include "AE_GeneralPlug.h"
#include "AEGP_SuiteHandler.h"
#include "AEFX_SuiteHelper.h"
#include "Smart_Utils.h"

#include "CXCommon.h"

#ifdef AE_OS_WIN
    #include <Windows.h>
#endif

// Plugin identification
#define PLUGIN_NAME             "cx_PencilLine"
#define PLUGIN_MATCH_NAME       "CX PencilLine"
#define PLUGIN_CATEGORY         "CX Animation Tools"
#define PLUGIN_DESCRIPTION      "Pencil line texture effect for animation photography"

// Version info
#define MAJOR_VERSION           1
#define MINOR_VERSION           0
#define BUG_VERSION             0
#define STAGE_VERSION           PF_Stage_DEVELOP
#define BUILD_VERSION           1

// Maximum number of colors supported
constexpr A_long MAX_COLORS = 16;

// Output modes
enum OutputMode {
    OUTPUT_MODE_FULL = 1,       // Full image with processed lines
    OUTPUT_MODE_LINE_ONLY,      // Only extracted lines with texture
    OUTPUT_MODE_BG_ONLY         // Only background (no lines)
};

// Parameter IDs (UI order)
// Each color has 3 params: Enabled (checkbox), Color, Tolerance
enum {
    PENCILLINE_INPUT = 0,

    // Color Selection Group
    PENCILLINE_COLOR_GROUP,

    // Color 1-16 (each has: Enabled, Color, Tolerance)
    PENCILLINE_COLOR1_ENABLED,
    PENCILLINE_COLOR1,
    PENCILLINE_COLOR1_TOLERANCE,

    PENCILLINE_COLOR2_ENABLED,
    PENCILLINE_COLOR2,
    PENCILLINE_COLOR2_TOLERANCE,

    PENCILLINE_COLOR3_ENABLED,
    PENCILLINE_COLOR3,
    PENCILLINE_COLOR3_TOLERANCE,

    PENCILLINE_COLOR4_ENABLED,
    PENCILLINE_COLOR4,
    PENCILLINE_COLOR4_TOLERANCE,

    PENCILLINE_COLOR5_ENABLED,
    PENCILLINE_COLOR5,
    PENCILLINE_COLOR5_TOLERANCE,

    PENCILLINE_COLOR6_ENABLED,
    PENCILLINE_COLOR6,
    PENCILLINE_COLOR6_TOLERANCE,

    PENCILLINE_COLOR7_ENABLED,
    PENCILLINE_COLOR7,
    PENCILLINE_COLOR7_TOLERANCE,

    PENCILLINE_COLOR8_ENABLED,
    PENCILLINE_COLOR8,
    PENCILLINE_COLOR8_TOLERANCE,

    PENCILLINE_COLOR9_ENABLED,
    PENCILLINE_COLOR9,
    PENCILLINE_COLOR9_TOLERANCE,

    PENCILLINE_COLOR10_ENABLED,
    PENCILLINE_COLOR10,
    PENCILLINE_COLOR10_TOLERANCE,

    PENCILLINE_COLOR11_ENABLED,
    PENCILLINE_COLOR11,
    PENCILLINE_COLOR11_TOLERANCE,

    PENCILLINE_COLOR12_ENABLED,
    PENCILLINE_COLOR12,
    PENCILLINE_COLOR12_TOLERANCE,

    PENCILLINE_COLOR13_ENABLED,
    PENCILLINE_COLOR13,
    PENCILLINE_COLOR13_TOLERANCE,

    PENCILLINE_COLOR14_ENABLED,
    PENCILLINE_COLOR14,
    PENCILLINE_COLOR14_TOLERANCE,

    PENCILLINE_COLOR15_ENABLED,
    PENCILLINE_COLOR15,
    PENCILLINE_COLOR15_TOLERANCE,

    PENCILLINE_COLOR16_ENABLED,
    PENCILLINE_COLOR16,
    PENCILLINE_COLOR16_TOLERANCE,

    PENCILLINE_COLOR_GROUP_END,

    // Pencil Texture Group (placeholder for future parameters)
    PENCILLINE_TEXTURE_GROUP,
    PENCILLINE_LINE_WIDTH,
    PENCILLINE_LINE_DENSITY,
    PENCILLINE_TEXTURE_STRENGTH,
    PENCILLINE_TEXTURE_GROUP_END,

    // Output Group
    PENCILLINE_OUTPUT_GROUP,
    PENCILLINE_OUTPUT_MODE,
    PENCILLINE_OUTPUT_GROUP_END,

    PENCILLINE_NUM_PARAMS
};

// Disk IDs (persistent, never reuse)
enum {
    DISK_ID_COLOR_GROUP = 1,

    // Color 1-16 disk IDs (each color uses 3 consecutive IDs)
    DISK_ID_COLOR1_ENABLED = 10,
    DISK_ID_COLOR1,
    DISK_ID_COLOR1_TOLERANCE,

    DISK_ID_COLOR2_ENABLED = 20,
    DISK_ID_COLOR2,
    DISK_ID_COLOR2_TOLERANCE,

    DISK_ID_COLOR3_ENABLED = 30,
    DISK_ID_COLOR3,
    DISK_ID_COLOR3_TOLERANCE,

    DISK_ID_COLOR4_ENABLED = 40,
    DISK_ID_COLOR4,
    DISK_ID_COLOR4_TOLERANCE,

    DISK_ID_COLOR5_ENABLED = 50,
    DISK_ID_COLOR5,
    DISK_ID_COLOR5_TOLERANCE,

    DISK_ID_COLOR6_ENABLED = 60,
    DISK_ID_COLOR6,
    DISK_ID_COLOR6_TOLERANCE,

    DISK_ID_COLOR7_ENABLED = 70,
    DISK_ID_COLOR7,
    DISK_ID_COLOR7_TOLERANCE,

    DISK_ID_COLOR8_ENABLED = 80,
    DISK_ID_COLOR8,
    DISK_ID_COLOR8_TOLERANCE,

    DISK_ID_COLOR9_ENABLED = 90,
    DISK_ID_COLOR9,
    DISK_ID_COLOR9_TOLERANCE,

    DISK_ID_COLOR10_ENABLED = 100,
    DISK_ID_COLOR10,
    DISK_ID_COLOR10_TOLERANCE,

    DISK_ID_COLOR11_ENABLED = 110,
    DISK_ID_COLOR11,
    DISK_ID_COLOR11_TOLERANCE,

    DISK_ID_COLOR12_ENABLED = 120,
    DISK_ID_COLOR12,
    DISK_ID_COLOR12_TOLERANCE,

    DISK_ID_COLOR13_ENABLED = 130,
    DISK_ID_COLOR13,
    DISK_ID_COLOR13_TOLERANCE,

    DISK_ID_COLOR14_ENABLED = 140,
    DISK_ID_COLOR14,
    DISK_ID_COLOR14_TOLERANCE,

    DISK_ID_COLOR15_ENABLED = 150,
    DISK_ID_COLOR15,
    DISK_ID_COLOR15_TOLERANCE,

    DISK_ID_COLOR16_ENABLED = 160,
    DISK_ID_COLOR16,
    DISK_ID_COLOR16_TOLERANCE,

    DISK_ID_COLOR_GROUP_END = 200,

    DISK_ID_TEXTURE_GROUP = 210,
    DISK_ID_LINE_WIDTH,
    DISK_ID_LINE_DENSITY,
    DISK_ID_TEXTURE_STRENGTH,
    DISK_ID_TEXTURE_GROUP_END,

    DISK_ID_OUTPUT_GROUP = 220,
    DISK_ID_OUTPUT_MODE,
    DISK_ID_OUTPUT_GROUP_END
};

// Helper macros to get parameter IDs for color N (1-based)
#define COLOR_ENABLED_PARAM(n) (PENCILLINE_COLOR1_ENABLED + ((n) - 1) * 3)
#define COLOR_PARAM(n)         (PENCILLINE_COLOR1 + ((n) - 1) * 3)
#define COLOR_TOLERANCE_PARAM(n) (PENCILLINE_COLOR1_TOLERANCE + ((n) - 1) * 3)

// Default parameter values
constexpr PF_FpLong DEFAULT_TOLERANCE = 0.0;
constexpr A_long DEFAULT_LINE_WIDTH = 2;
constexpr PF_FpLong DEFAULT_LINE_DENSITY = 50.0;
constexpr PF_FpLong DEFAULT_TEXTURE_STRENGTH = 50.0;

// Parameter ranges
constexpr PF_FpLong TOLERANCE_MIN = 0.0;
constexpr PF_FpLong TOLERANCE_MAX = 100.0;
constexpr A_long LINE_WIDTH_MIN = 1;
constexpr A_long LINE_WIDTH_MAX = 20;
constexpr PF_FpLong LINE_DENSITY_MIN = 0.0;
constexpr PF_FpLong LINE_DENSITY_MAX = 100.0;
constexpr PF_FpLong TEXTURE_STRENGTH_MIN = 0.0;
constexpr PF_FpLong TEXTURE_STRENGTH_MAX = 100.0;

// Single color entry for processing
struct ColorEntry {
    PF_Boolean enabled;
    PF_Pixel color;
    PF_FpLong tolerance;
    A_long toleranceSq;     // Precomputed squared tolerance
};

// Processing info structure passed between PreRender and SmartRender
struct PencilLineInfo {
    // Color selection parameters
    A_long colorCount;              // Active color count (always 16)
    ColorEntry colors[MAX_COLORS];  // All color entries

    // Pencil texture parameters
    A_long lineWidth;
    PF_FpLong lineDensity;
    PF_FpLong textureStrength;

    // Output mode
    A_long outputMode;
};

// Function declarations
extern "C" {
    DllExport PF_Err EffectMain(
        PF_Cmd          cmd,
        PF_InData*      in_data,
        PF_OutData*     out_data,
        PF_ParamDef*    params[],
        PF_LayerDef*    output,
        void*           extra);
}

// Internal functions
PF_Err GlobalSetup(
    PF_InData*      in_data,
    PF_OutData*     out_data);

PF_Err ParamsSetup(
    PF_InData*      in_data,
    PF_OutData*     out_data);

PF_Err UpdateParameterUI(
    PF_InData*      in_data,
    PF_OutData*     out_data,
    PF_ParamDef*    params[]);

PF_Err PreRender(
    PF_InData*              in_data,
    PF_OutData*             out_data,
    PF_PreRenderExtra*      extra);

PF_Err SmartRender(
    PF_InData*              in_data,
    PF_OutData*             out_data,
    PF_SmartRenderExtra*    extra);
