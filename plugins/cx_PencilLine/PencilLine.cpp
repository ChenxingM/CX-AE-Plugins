/*
 * PencilLine.cpp
 * cx_PencilLine - Pencil Line Texture Effect for After Effects
 *
 * Extracts multiple target colors and applies pencil line texture processing.
 * Uses SmartFX architecture with multi-bit-depth support (8/16/32-bit).
 */

#include "PencilLine.h"
#include <cstdio>

// ============================================================================
// Color matching functions (checks against all enabled colors)
// Uses CX_IsTargetColor* from CXCommon.h
// ============================================================================

// Check if pixel matches any enabled target color (8-bit)
static inline bool IsTargetColor8(const PF_Pixel8* pixel, const PencilLineInfo* info) {
    for (A_long i = 0; i < info->colorCount; ++i) {
        const ColorEntry& entry = info->colors[i];
        if (!entry.enabled) continue;
        if (CX_IsTargetColor8(pixel, entry.color.red, entry.color.green, entry.color.blue, entry.toleranceSq)) {
            return true;
        }
    }
    return false;
}

// Check if pixel matches any enabled target color (16-bit)
static inline bool IsTargetColor16(const PF_Pixel16* pixel, const PencilLineInfo* info) {
    for (A_long i = 0; i < info->colorCount; ++i) {
        const ColorEntry& entry = info->colors[i];
        if (!entry.enabled) continue;
        if (CX_IsTargetColor16(pixel, entry.color.red, entry.color.green, entry.color.blue, entry.toleranceSq)) {
            return true;
        }
    }
    return false;
}

// Check if pixel matches any enabled target color (32-bit float)
static inline bool IsTargetColorFloat(const PF_PixelFloat* pixel, const PencilLineInfo* info) {
    for (A_long i = 0; i < info->colorCount; ++i) {
        const ColorEntry& entry = info->colors[i];
        if (!entry.enabled) continue;
        if (CX_IsTargetColorFloat(pixel, entry.color.red, entry.color.green, entry.color.blue, entry.toleranceSq)) {
            return true;
        }
    }
    return false;
}

// ============================================================================
// Pencil texture processing (placeholder - to be implemented)
// ============================================================================

static inline void ApplyPencilTexture8(
    PF_Pixel8* outP,
    const PF_Pixel8* inP,
    const PencilLineInfo* info,
    A_long x,
    A_long y)
{
    // Placeholder: Currently just copies the input
    // Will be replaced with pencil texture algorithm
    outP->alpha = inP->alpha;
    outP->red = inP->red;
    outP->green = inP->green;
    outP->blue = inP->blue;
}

static inline void ApplyPencilTexture16(
    PF_Pixel16* outP,
    const PF_Pixel16* inP,
    const PencilLineInfo* info,
    A_long x,
    A_long y)
{
    outP->alpha = inP->alpha;
    outP->red = inP->red;
    outP->green = inP->green;
    outP->blue = inP->blue;
}

static inline void ApplyPencilTextureFloat(
    PF_PixelFloat* outP,
    const PF_PixelFloat* inP,
    const PencilLineInfo* info,
    A_long x,
    A_long y)
{
    outP->alpha = inP->alpha;
    outP->red = inP->red;
    outP->green = inP->green;
    outP->blue = inP->blue;
}

// ============================================================================
// Main processing functions (per bit depth)
// ============================================================================

PF_Err ProcessPencilLine8(
    void*           refcon,
    A_long          x,
    A_long          y,
    PF_Pixel8*      inP,
    PF_Pixel8*      outP)
{
    PencilLineInfo* info = static_cast<PencilLineInfo*>(refcon);

    // Use inP directly - iterate callback provides correct input pixel
    bool isTargetColor = IsTargetColor8(inP, info);

    switch (info->outputMode) {
        case OUTPUT_MODE_LINE_ONLY:
            if (isTargetColor) {
                ApplyPencilTexture8(outP, inP, info, x, y);
            } else {
                outP->alpha = 0;
                outP->red = 0;
                outP->green = 0;
                outP->blue = 0;
            }
            break;

        case OUTPUT_MODE_BG_ONLY:
            if (isTargetColor) {
                outP->alpha = 0;
                outP->red = 0;
                outP->green = 0;
                outP->blue = 0;
            } else {
                *outP = *inP;
            }
            break;

        case OUTPUT_MODE_FULL:
        default:
            if (isTargetColor) {
                ApplyPencilTexture8(outP, inP, info, x, y);
            } else {
                *outP = *inP;
            }
            break;
    }

    return PF_Err_NONE;
}

PF_Err ProcessPencilLine16(
    void*           refcon,
    A_long          x,
    A_long          y,
    PF_Pixel16*     inP,
    PF_Pixel16*     outP)
{
    PencilLineInfo* info = static_cast<PencilLineInfo*>(refcon);

    // Use inP directly - iterate callback provides correct input pixel
    bool isTargetColor = IsTargetColor16(inP, info);

    switch (info->outputMode) {
        case OUTPUT_MODE_LINE_ONLY:
            if (isTargetColor) {
                ApplyPencilTexture16(outP, inP, info, x, y);
            } else {
                outP->alpha = 0;
                outP->red = 0;
                outP->green = 0;
                outP->blue = 0;
            }
            break;

        case OUTPUT_MODE_BG_ONLY:
            if (isTargetColor) {
                outP->alpha = 0;
                outP->red = 0;
                outP->green = 0;
                outP->blue = 0;
            } else {
                *outP = *inP;
            }
            break;

        case OUTPUT_MODE_FULL:
        default:
            if (isTargetColor) {
                ApplyPencilTexture16(outP, inP, info, x, y);
            } else {
                *outP = *inP;
            }
            break;
    }

    return PF_Err_NONE;
}

PF_Err ProcessPencilLineFloat(
    void*           refcon,
    A_long          x,
    A_long          y,
    PF_PixelFloat*  inP,
    PF_PixelFloat*  outP)
{
    PencilLineInfo* info = static_cast<PencilLineInfo*>(refcon);

    // Use inP directly - iterate callback provides correct input pixel
    bool isTargetColor = IsTargetColorFloat(inP, info);

    switch (info->outputMode) {
        case OUTPUT_MODE_LINE_ONLY:
            if (isTargetColor) {
                ApplyPencilTextureFloat(outP, inP, info, x, y);
            } else {
                outP->alpha = 0.0f;
                outP->red = 0.0f;
                outP->green = 0.0f;
                outP->blue = 0.0f;
            }
            break;

        case OUTPUT_MODE_BG_ONLY:
            if (isTargetColor) {
                outP->alpha = 0.0f;
                outP->red = 0.0f;
                outP->green = 0.0f;
                outP->blue = 0.0f;
            } else {
                *outP = *inP;
            }
            break;

        case OUTPUT_MODE_FULL:
        default:
            if (isTargetColor) {
                ApplyPencilTextureFloat(outP, inP, info, x, y);
            } else {
                *outP = *inP;
            }
            break;
    }

    return PF_Err_NONE;
}

// ============================================================================
// Plugin entry points
// ============================================================================

PF_Err GlobalSetup(
    PF_InData*      in_data,
    PF_OutData*     out_data)
{
    out_data->my_version = PF_VERSION(MAJOR_VERSION, MINOR_VERSION, BUG_VERSION, STAGE_VERSION, BUILD_VERSION);
    out_data->out_flags = PF_OutFlag_DEEP_COLOR_AWARE |
                          PF_OutFlag_SEND_UPDATE_PARAMS_UI;
    out_data->out_flags2 = PF_OutFlag2_FLOAT_COLOR_AWARE |
                           PF_OutFlag2_SUPPORTS_SMART_RENDER |
                           PF_OutFlag2_SUPPORTS_THREADED_RENDERING;
    return PF_Err_NONE;
}

// Helper function to add a single color's parameters
static PF_Err AddColorParams(
    PF_InData* in_data,
    A_long colorIndex,          // 1-based index
    PF_Boolean defaultEnabled,  // Whether checkbox is checked by default
    A_long diskIdEnabled,
    A_long diskIdColor,
    A_long diskIdTolerance)
{
    PF_Err err = PF_Err_NONE;
    PF_ParamDef def;
    char name[32];

    // Enabled checkbox
    AEFX_CLR_STRUCT(def);
    snprintf(name, sizeof(name), "Color %ld", colorIndex);
    PF_ADD_CHECKBOX(name, "", defaultEnabled, 0, diskIdEnabled);

    // Color picker
    AEFX_CLR_STRUCT(def);
    def.param_type = PF_Param_COLOR;
    snprintf(name, sizeof(name), "  Color %ld", colorIndex);
    PF_STRCPY(def.name, name);
    def.u.cd.value.red = 0;
    def.u.cd.value.green = 0;
    def.u.cd.value.blue = 0;
    def.u.cd.value.alpha = 255;
    def.u.cd.dephault = def.u.cd.value;
    def.uu.id = diskIdColor;
    if ((err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return err;

    // Tolerance slider
    AEFX_CLR_STRUCT(def);
    snprintf(name, sizeof(name), "  Tolerance %ld", colorIndex);
    def.param_type = PF_Param_FLOAT_SLIDER;
    PF_STRCPY(def.name, name);
    def.u.fs_d.valid_min = TOLERANCE_MIN;
    def.u.fs_d.valid_max = TOLERANCE_MAX;
    def.u.fs_d.slider_min = TOLERANCE_MIN;
    def.u.fs_d.slider_max = TOLERANCE_MAX;
    def.u.fs_d.value = DEFAULT_TOLERANCE;
    def.u.fs_d.dephault = DEFAULT_TOLERANCE;
    def.u.fs_d.precision = 1;
    def.uu.id = diskIdTolerance;
    if ((err = PF_ADD_PARAM(in_data, -1, &def)) != PF_Err_NONE) return err;

    return err;
}

PF_Err ParamsSetup(
    PF_InData*      in_data,
    PF_OutData*     out_data)
{
    PF_Err err = PF_Err_NONE;
    PF_ParamDef def;

    // Color Selection Group
    AEFX_CLR_STRUCT(def);
    def.flags = PF_ParamFlag_START_COLLAPSED;
    PF_ADD_TOPIC("Color Selection", DISK_ID_COLOR_GROUP);

    // Add all 16 color parameters (first color enabled by default)
    ERR(AddColorParams(in_data, 1,  TRUE,  DISK_ID_COLOR1_ENABLED,  DISK_ID_COLOR1,  DISK_ID_COLOR1_TOLERANCE));
    ERR(AddColorParams(in_data, 2,  FALSE, DISK_ID_COLOR2_ENABLED,  DISK_ID_COLOR2,  DISK_ID_COLOR2_TOLERANCE));
    ERR(AddColorParams(in_data, 3,  FALSE, DISK_ID_COLOR3_ENABLED,  DISK_ID_COLOR3,  DISK_ID_COLOR3_TOLERANCE));
    ERR(AddColorParams(in_data, 4,  FALSE, DISK_ID_COLOR4_ENABLED,  DISK_ID_COLOR4,  DISK_ID_COLOR4_TOLERANCE));
    ERR(AddColorParams(in_data, 5,  FALSE, DISK_ID_COLOR5_ENABLED,  DISK_ID_COLOR5,  DISK_ID_COLOR5_TOLERANCE));
    ERR(AddColorParams(in_data, 6,  FALSE, DISK_ID_COLOR6_ENABLED,  DISK_ID_COLOR6,  DISK_ID_COLOR6_TOLERANCE));
    ERR(AddColorParams(in_data, 7,  FALSE, DISK_ID_COLOR7_ENABLED,  DISK_ID_COLOR7,  DISK_ID_COLOR7_TOLERANCE));
    ERR(AddColorParams(in_data, 8,  FALSE, DISK_ID_COLOR8_ENABLED,  DISK_ID_COLOR8,  DISK_ID_COLOR8_TOLERANCE));
    ERR(AddColorParams(in_data, 9,  FALSE, DISK_ID_COLOR9_ENABLED,  DISK_ID_COLOR9,  DISK_ID_COLOR9_TOLERANCE));
    ERR(AddColorParams(in_data, 10, FALSE, DISK_ID_COLOR10_ENABLED, DISK_ID_COLOR10, DISK_ID_COLOR10_TOLERANCE));
    ERR(AddColorParams(in_data, 11, FALSE, DISK_ID_COLOR11_ENABLED, DISK_ID_COLOR11, DISK_ID_COLOR11_TOLERANCE));
    ERR(AddColorParams(in_data, 12, FALSE, DISK_ID_COLOR12_ENABLED, DISK_ID_COLOR12, DISK_ID_COLOR12_TOLERANCE));
    ERR(AddColorParams(in_data, 13, FALSE, DISK_ID_COLOR13_ENABLED, DISK_ID_COLOR13, DISK_ID_COLOR13_TOLERANCE));
    ERR(AddColorParams(in_data, 14, FALSE, DISK_ID_COLOR14_ENABLED, DISK_ID_COLOR14, DISK_ID_COLOR14_TOLERANCE));
    ERR(AddColorParams(in_data, 15, FALSE, DISK_ID_COLOR15_ENABLED, DISK_ID_COLOR15, DISK_ID_COLOR15_TOLERANCE));
    ERR(AddColorParams(in_data, 16, FALSE, DISK_ID_COLOR16_ENABLED, DISK_ID_COLOR16, DISK_ID_COLOR16_TOLERANCE));

    PF_END_TOPIC(DISK_ID_COLOR_GROUP_END);

    // Pencil Texture Group
    AEFX_CLR_STRUCT(def);
    def.flags = PF_ParamFlag_START_COLLAPSED;
    PF_ADD_TOPIC("Pencil Texture", DISK_ID_TEXTURE_GROUP);

    // Line Width
    AEFX_CLR_STRUCT(def);
    PF_ADD_SLIDER("Line Width",
                  LINE_WIDTH_MIN, LINE_WIDTH_MAX,
                  LINE_WIDTH_MIN, LINE_WIDTH_MAX,
                  DEFAULT_LINE_WIDTH,
                  DISK_ID_LINE_WIDTH);

    // Line Density
    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX("Line Density",
                         LINE_DENSITY_MIN, LINE_DENSITY_MAX,
                         LINE_DENSITY_MIN, LINE_DENSITY_MAX,
                         DEFAULT_LINE_DENSITY,
                         PF_Precision_TENTHS,
                         0,
                         0,
                         DISK_ID_LINE_DENSITY);

    // Texture Strength
    AEFX_CLR_STRUCT(def);
    PF_ADD_FLOAT_SLIDERX("Texture Strength",
                         TEXTURE_STRENGTH_MIN, TEXTURE_STRENGTH_MAX,
                         TEXTURE_STRENGTH_MIN, TEXTURE_STRENGTH_MAX,
                         DEFAULT_TEXTURE_STRENGTH,
                         PF_Precision_TENTHS,
                         0,
                         0,
                         DISK_ID_TEXTURE_STRENGTH);

    PF_END_TOPIC(DISK_ID_TEXTURE_GROUP_END);

    // Output Group
    AEFX_CLR_STRUCT(def);
    def.flags = PF_ParamFlag_START_COLLAPSED;
    PF_ADD_TOPIC("Output", DISK_ID_OUTPUT_GROUP);

    // Output Mode
    AEFX_CLR_STRUCT(def);
    PF_ADD_POPUP("Output Mode",
                 3,
                 OUTPUT_MODE_FULL,
                 "Full|Line Only|Background Only",
                 DISK_ID_OUTPUT_MODE);

    PF_END_TOPIC(DISK_ID_OUTPUT_GROUP_END);

    out_data->num_params = PENCILLINE_NUM_PARAMS;

    return err;
}

PF_Err UpdateParameterUI(
    PF_InData*      in_data,
    PF_OutData*     out_data,
    PF_ParamDef*    params[])
{
    PF_Err err = PF_Err_NONE;

    AEFX_SuiteScoper<PF_ParamUtilsSuite3> paramSuite = AEFX_SuiteScoper<PF_ParamUtilsSuite3>(
        in_data, kPFParamUtilsSuite, kPFParamUtilsSuiteVersion3, out_data);

    // Update enabled state for each color's Color and Tolerance based on checkbox
    for (A_long i = 1; i <= MAX_COLORS; ++i) {
        A_long enabledIdx = COLOR_ENABLED_PARAM(i);
        A_long colorIdx = COLOR_PARAM(i);
        A_long toleranceIdx = COLOR_TOLERANCE_PARAM(i);

        // Check if this color is enabled (checkbox checked)
        PF_Boolean isEnabled = params[enabledIdx]->u.bd.value;

        // Update color picker enabled state
        PF_ParamDef paramCopy = *params[colorIdx];
        if (isEnabled) {
            paramCopy.ui_flags &= ~PF_PUI_DISABLED;
        } else {
            paramCopy.ui_flags |= PF_PUI_DISABLED;
        }
        ERR(paramSuite->PF_UpdateParamUI(in_data->effect_ref, colorIdx, &paramCopy));

        // Update tolerance slider enabled state
        paramCopy = *params[toleranceIdx];
        if (isEnabled) {
            paramCopy.ui_flags &= ~PF_PUI_DISABLED;
        } else {
            paramCopy.ui_flags |= PF_PUI_DISABLED;
        }
        ERR(paramSuite->PF_UpdateParamUI(in_data->effect_ref, toleranceIdx, &paramCopy));
    }

    return err;
}

PF_Err PreRender(
    PF_InData*              in_data,
    PF_OutData*             out_data,
    PF_PreRenderExtra*      extra)
{
    PF_Err err = PF_Err_NONE;
    PF_RenderRequest req = extra->input->output_request;
    PF_CheckoutResult in_result;

    // Allocate custom data for processing info
    AEFX_SuiteScoper<PF_HandleSuite1> handleSuite = AEFX_SuiteScoper<PF_HandleSuite1>(
        in_data, kPFHandleSuite, kPFHandleSuiteVersion1, out_data);
    PF_Handle infoH = handleSuite->host_new_handle(sizeof(PencilLineInfo));
    if (!infoH) return PF_Err_OUT_OF_MEMORY;

    PencilLineInfo* info = reinterpret_cast<PencilLineInfo*>(
        handleSuite->host_lock_handle(infoH));

    // Initialize info structure
    memset(info, 0, sizeof(PencilLineInfo));

    // Fixed 16 colors
    info->colorCount = MAX_COLORS;

    // Checkout all color parameters
    for (A_long i = 0; i < MAX_COLORS; ++i) {
        PF_ParamDef enabledParam, colorParam, toleranceParam;
        AEFX_CLR_STRUCT(enabledParam);
        AEFX_CLR_STRUCT(colorParam);
        AEFX_CLR_STRUCT(toleranceParam);

        A_long colorNum = i + 1;  // 1-based

        ERR(PF_CHECKOUT_PARAM(in_data, COLOR_ENABLED_PARAM(colorNum), in_data->current_time,
                              in_data->time_step, in_data->time_scale, &enabledParam));
        ERR(PF_CHECKOUT_PARAM(in_data, COLOR_PARAM(colorNum), in_data->current_time,
                              in_data->time_step, in_data->time_scale, &colorParam));
        ERR(PF_CHECKOUT_PARAM(in_data, COLOR_TOLERANCE_PARAM(colorNum), in_data->current_time,
                              in_data->time_step, in_data->time_scale, &toleranceParam));

        if (!err) {
            info->colors[i].enabled = enabledParam.u.bd.value;
            info->colors[i].color = colorParam.u.cd.value;
            info->colors[i].tolerance = toleranceParam.u.fs_d.value;

            // Precompute squared tolerance using common helper
            info->colors[i].toleranceSq = CX_ToleranceToDistSq(info->colors[i].tolerance);
        }

        PF_CHECKIN_PARAM(in_data, &enabledParam);
        PF_CHECKIN_PARAM(in_data, &colorParam);
        PF_CHECKIN_PARAM(in_data, &toleranceParam);
    }

    // Checkout texture and output parameters
    PF_ParamDef lineWidthParam, lineDensityParam, textureStrengthParam, outputModeParam;
    AEFX_CLR_STRUCT(lineWidthParam);
    AEFX_CLR_STRUCT(lineDensityParam);
    AEFX_CLR_STRUCT(textureStrengthParam);
    AEFX_CLR_STRUCT(outputModeParam);

    ERR(PF_CHECKOUT_PARAM(in_data, PENCILLINE_LINE_WIDTH, in_data->current_time,
                          in_data->time_step, in_data->time_scale, &lineWidthParam));
    ERR(PF_CHECKOUT_PARAM(in_data, PENCILLINE_LINE_DENSITY, in_data->current_time,
                          in_data->time_step, in_data->time_scale, &lineDensityParam));
    ERR(PF_CHECKOUT_PARAM(in_data, PENCILLINE_TEXTURE_STRENGTH, in_data->current_time,
                          in_data->time_step, in_data->time_scale, &textureStrengthParam));
    ERR(PF_CHECKOUT_PARAM(in_data, PENCILLINE_OUTPUT_MODE, in_data->current_time,
                          in_data->time_step, in_data->time_scale, &outputModeParam));

    if (!err) {
        info->lineWidth = lineWidthParam.u.sd.value;
        info->lineDensity = lineDensityParam.u.fs_d.value;
        info->textureStrength = textureStrengthParam.u.fs_d.value;
        info->outputMode = outputModeParam.u.pd.value;
    }

    PF_CHECKIN_PARAM(in_data, &lineWidthParam);
    PF_CHECKIN_PARAM(in_data, &lineDensityParam);
    PF_CHECKIN_PARAM(in_data, &textureStrengthParam);
    PF_CHECKIN_PARAM(in_data, &outputModeParam);

    // Request input checkout
    req.preserve_rgb_of_zero_alpha = TRUE;
    ERR(extra->cb->checkout_layer(in_data->effect_ref,
                                  PENCILLINE_INPUT,
                                  PENCILLINE_INPUT,
                                  &req,
                                  in_data->current_time,
                                  in_data->time_step,
                                  in_data->time_scale,
                                  &in_result));

    // Set max result rect
    UnionLRect(&in_result.result_rect, &extra->output->result_rect);
    UnionLRect(&in_result.max_result_rect, &extra->output->max_result_rect);

    // Store custom data handle
    extra->output->pre_render_data = infoH;
    handleSuite->host_unlock_handle(infoH);

    return err;
}

PF_Err SmartRender(
    PF_InData*              in_data,
    PF_OutData*             out_data,
    PF_SmartRenderExtra*    extra)
{
    PF_Err err = PF_Err_NONE;
    AEFX_SuiteScoper<PF_HandleSuite1> handleSuite = AEFX_SuiteScoper<PF_HandleSuite1>(
        in_data, kPFHandleSuite, kPFHandleSuiteVersion1, out_data);

    // Get pre-render data
    PencilLineInfo* info = reinterpret_cast<PencilLineInfo*>(
        handleSuite->host_lock_handle(reinterpret_cast<PF_Handle>(extra->input->pre_render_data)));

    if (!info) {
        return PF_Err_BAD_CALLBACK_PARAM;
    }

    // Checkout input layer
    PF_EffectWorld* input_worldP = nullptr;
    ERR(extra->cb->checkout_layer_pixels(in_data->effect_ref, PENCILLINE_INPUT, &input_worldP));

    if (!err && input_worldP) {
        // Get output world
        PF_EffectWorld* output_worldP = nullptr;
        ERR(extra->cb->checkout_output(in_data->effect_ref, &output_worldP));

        if (!err && output_worldP) {
            // Determine pixel format and iterate
            PF_PixelFormat format = PF_PixelFormat_INVALID;
            AEFX_SuiteScoper<PF_WorldSuite2> wsP = AEFX_SuiteScoper<PF_WorldSuite2>(
                in_data, kPFWorldSuite, kPFWorldSuiteVersion2, out_data);
            ERR(wsP->PF_GetPixelFormat(input_worldP, &format));

            if (!err) {
                switch (format) {
                    case PF_PixelFormat_ARGB128:
                        {
                            AEFX_SuiteScoper<PF_iterateFloatSuite2> iterSuite = AEFX_SuiteScoper<PF_iterateFloatSuite2>(
                                in_data, kPFIterateFloatSuite, kPFIterateFloatSuiteVersion2, out_data);
                            ERR(iterSuite->iterate(
                                in_data,
                                0,
                                output_worldP->height,
                                input_worldP,
                                nullptr,
                                info,
                                ProcessPencilLineFloat,
                                output_worldP));
                        }
                        break;

                    case PF_PixelFormat_ARGB64:
                        {
                            AEFX_SuiteScoper<PF_iterate16Suite2> iterSuite = AEFX_SuiteScoper<PF_iterate16Suite2>(
                                in_data, kPFIterate16Suite, kPFIterate16SuiteVersion2, out_data);
                            ERR(iterSuite->iterate(
                                in_data,
                                0,
                                output_worldP->height,
                                input_worldP,
                                nullptr,
                                info,
                                ProcessPencilLine16,
                                output_worldP));
                        }
                        break;

                    case PF_PixelFormat_ARGB32:
                    default:
                        {
                            AEFX_SuiteScoper<PF_Iterate8Suite2> iterSuite = AEFX_SuiteScoper<PF_Iterate8Suite2>(
                                in_data, kPFIterate8Suite, kPFIterate8SuiteVersion2, out_data);
                            ERR(iterSuite->iterate(
                                in_data,
                                0,
                                output_worldP->height,
                                input_worldP,
                                nullptr,
                                info,
                                ProcessPencilLine8,
                                output_worldP));
                        }
                        break;
                }
            }
        }
    }

    // Checkin input layer
    ERR(extra->cb->checkin_layer_pixels(in_data->effect_ref, PENCILLINE_INPUT));

    // Unlock handle
    handleSuite->host_unlock_handle(reinterpret_cast<PF_Handle>(extra->input->pre_render_data));

    return err;
}

// ============================================================================
// Main entry point
// ============================================================================

// Helper macro for version string
#ifndef MACRO_STR
#define MACRO_STR_IMPL(x) #x
#define MACRO_STR(x) MACRO_STR_IMPL(x)
#endif

extern "C" DllExport PF_Err EffectMain(
    PF_Cmd          cmd,
    PF_InData*      in_data,
    PF_OutData*     out_data,
    PF_ParamDef*    params[],
    PF_LayerDef*    output,
    void*           extra)
{
    PF_Err err = PF_Err_NONE;

    switch (cmd) {
        case PF_Cmd_ABOUT:
            PF_STRCPY(out_data->return_msg, PLUGIN_NAME " v"
                MACRO_STR(MAJOR_VERSION) "." MACRO_STR(MINOR_VERSION) "\r"
                PLUGIN_DESCRIPTION);
            break;

        case PF_Cmd_GLOBAL_SETUP:
            err = GlobalSetup(in_data, out_data);
            break;

        case PF_Cmd_PARAMS_SETUP:
            err = ParamsSetup(in_data, out_data);
            break;

        case PF_Cmd_UPDATE_PARAMS_UI:
            err = UpdateParameterUI(in_data, out_data, params);
            break;

        case PF_Cmd_SMART_PRE_RENDER:
            err = PreRender(in_data, out_data, static_cast<PF_PreRenderExtra*>(extra));
            break;

        case PF_Cmd_SMART_RENDER:
            err = SmartRender(in_data, out_data, static_cast<PF_SmartRenderExtra*>(extra));
            break;

        default:
            break;
    }

    return err;
}
