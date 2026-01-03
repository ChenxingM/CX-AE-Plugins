/*
	CXCommon.h

	CX Animation Tools - Shared Utilities
	Common functions and definitions used across all CX plugins

	Copyright (c) 2025 CX Animation Tools
*/

#pragma once
#ifndef CX_COMMON_H
#define CX_COMMON_H

#include "AEConfig.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_Macros.h"

#ifdef AE_OS_WIN
	#include <Windows.h>
#endif

// ============================================================================
// Version Info
// ============================================================================

#define CX_TOOLS_VERSION_MAJOR	1
#define CX_TOOLS_VERSION_MINOR	0
#define CX_TOOLS_CATEGORY		"CX Animation Tools"

// ============================================================================
// Common Utility Macros
// ============================================================================

// Branchless min/max
#define CX_MIN(a, b) ((a) < (b) ? (a) : (b))
#define CX_MAX(a, b) ((a) > (b) ? (a) : (b))
#define CX_CLAMP(val, lo, hi) CX_MAX((lo), CX_MIN((hi), (val)))

// ============================================================================
// Common Inline Functions
// ============================================================================

// Clamp functions for different bit depths
static inline A_u_char CX_ClampByte(PF_FpLong value) {
	return (A_u_char)(value < 0 ? 0 : (value > 255 ? 255 : value));
}

static inline A_u_short CX_Clamp16(PF_FpLong value) {
	return (A_u_short)(value < 0 ? 0 : (value > PF_MAX_CHAN16 ? PF_MAX_CHAN16 : value));
}

static inline PF_FpLong CX_Clamp01(PF_FpLong value) {
	return value < 0.0 ? 0.0 : (value > 1.0 ? 1.0 : value);
}

// Pixel access helpers
static inline PF_Pixel8* CX_GetRow8(PF_EffectWorld *world, A_long y) {
	return (PF_Pixel8*)((char*)world->data + y * world->rowbytes);
}

static inline PF_Pixel16* CX_GetRow16(PF_EffectWorld *world, A_long y) {
	return (PF_Pixel16*)((char*)world->data + y * world->rowbytes);
}

static inline PF_PixelFloat* CX_GetRowFloat(PF_EffectWorld *world, A_long y) {
	return (PF_PixelFloat*)((char*)world->data + y * world->rowbytes);
}

// Rectangle union helper
static inline void CX_UnionLRect(const PF_LRect *src, PF_LRect *dst) {
	if (src->left < dst->left) dst->left = src->left;
	if (src->top < dst->top) dst->top = src->top;
	if (src->right > dst->right) dst->right = src->right;
	if (src->bottom > dst->bottom) dst->bottom = src->bottom;
}

// ============================================================================
// RGB <-> HSL Conversion
// ============================================================================

static inline PF_FpLong CX_HueToRGB(PF_FpLong p, PF_FpLong q, PF_FpLong t) {
	if (t < 0.0) t += 1.0;
	else if (t > 1.0) t -= 1.0;

	if (t < 0.166666667) return p + (q - p) * 6.0 * t;
	if (t < 0.5) return q;
	if (t < 0.666666667) return p + (q - p) * (0.666666667 - t) * 6.0;
	return p;
}

static inline void CX_RGBtoHSL(PF_FpLong r, PF_FpLong g, PF_FpLong b,
                               PF_FpLong *h, PF_FpLong *s, PF_FpLong *l) {
	PF_FpLong maxVal = r > g ? (r > b ? r : b) : (g > b ? g : b);
	PF_FpLong minVal = r < g ? (r < b ? r : b) : (g < b ? g : b);
	PF_FpLong delta = maxVal - minVal;

	*l = (maxVal + minVal) * 0.5;

	if (delta < 0.00001) {
		*h = 0.0;
		*s = 0.0;
	} else {
		*s = (*l > 0.5) ? delta / (2.0 - maxVal - minVal) : delta / (maxVal + minVal);
		if (maxVal == r) {
			*h = (g - b) / delta + (g < b ? 6.0 : 0.0);
		} else if (maxVal == g) {
			*h = (b - r) / delta + 2.0;
		} else {
			*h = (r - g) / delta + 4.0;
		}
		*h *= 0.166666667;
	}
}

static inline void CX_HSLtoRGB(PF_FpLong h, PF_FpLong s, PF_FpLong l,
                               PF_FpLong *r, PF_FpLong *g, PF_FpLong *b) {
	if (s < 0.00001) {
		*r = *g = *b = l;
	} else {
		PF_FpLong q = l < 0.5 ? l * (1.0 + s) : l + s - l * s;
		PF_FpLong p = 2.0 * l - q;
		*r = CX_HueToRGB(p, q, h + 0.333333333);
		*g = CX_HueToRGB(p, q, h);
		*b = CX_HueToRGB(p, q, h - 0.333333333);
	}
}

#endif // CX_COMMON_H
