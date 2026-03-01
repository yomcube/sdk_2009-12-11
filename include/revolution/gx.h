#ifndef RVL_SDK_GX_H
#define RVL_SDK_GX_H
#include <revolution/types.h>
#include <revolution/mtx.h>
#include <context_rvl.h>

/* Reference:
 * Patent US 6,937,245 B1
 * https://patents.google.com/patent/US6937245B1
 * section 13, line 4
 */
typedef u8 GXBool;
#define GX_TRUE		1
#define GX_FALSE	0

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x48f80e
typedef enum _GXAlphaOp
{
	GX_AOP_AND,
	GX_AOP_OR,
	GX_AOP_XOR,
	GX_AOP_XNOR,

	GX_MAX_ALPHAOP
} GXAlphaOp;

// [SPQE7T]/ISpyD.elf:.debug_info::0x21232f
typedef enum _GXAnisotropy
{
	GX_ANISO_1,
	GX_ANISO_2,
	GX_ANISO_4,

	GX_MAX_ANISOTROPY
} GXAnisotropy;

// NOTE: older GX version (Feb 24 2009 00:57:49)
// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x10044
typedef enum _GXAttnFn
{
	GX_AF_SPEC,
	GX_AF_SPOT,

	GX_AF_NONE
} GXAttnFn;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x438266
typedef enum _GXAttr
{
	GX_VA_PNMTXIDX,

	GX_VA_TEX0MTXIDX,
	GX_VA_TEX1MTXIDX,
	GX_VA_TEX2MTXIDX,
	GX_VA_TEX3MTXIDX,
	GX_VA_TEX4MTXIDX,
	GX_VA_TEX5MTXIDX,
	GX_VA_TEX6MTXIDX,
	GX_VA_TEX7MTXIDX,

	GX_VA_POS,
	GX_VA_NRM,

	GX_VA_CLR0,
	GX_VA_CLR1,

	GX_VA_TEX0,
	GX_VA_TEX1,
	GX_VA_TEX2,
	GX_VA_TEX3,
	GX_VA_TEX4,
	GX_VA_TEX5,
	GX_VA_TEX6,
	GX_VA_TEX7,

	GX_POS_MTX_ARRAY,
	GX_NRM_MTX_ARRAY,
	GX_TEX_MTX_ARRAY,
	GX_LIGHT_ARRAY,

	GX_VA_NBT,

	GX_VA_MAX_ATTR,
	GX_VA_NULL = 0xff
} GXAttr;

// [SPQE7T]/ISpyD.elf:.debug_info::0x268dc0
typedef enum _GXAttrType
{
	GX_NONE,

	GX_DIRECT,

	GX_INDEX8,
	GX_INDEX16
} GXAttrType;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x45d222
typedef enum _GXBlendFactor
{
	GX_BL_ZERO,
	GX_BL_ONE,

	GX_BL_SRCCLR,
	GX_BL_INVSRCCLR,

	GX_BL_SRCALPHA,
	GX_BL_INVSRCALPHA,

	GX_BL_DSTALPHA,
	GX_BL_INVDSTALPHA,

	GX_BL_DSTCLR		= GX_BL_SRCCLR,
	GX_BL_INVDSTCLR		= GX_BL_INVSRCCLR
} GXBlendFactor;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x48f667
typedef enum _GXBlendMode
{
	GX_BM_NONE,

	GX_BM_BLEND,
	GX_BM_LOGIC,
	GX_BM_SUBTRACT,

	GX_MAX_BLENDMODE
} GXBlendMode;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x4070a5
typedef enum _GXChannelID
{
	GX_COLOR0,
	GX_COLOR1,

	GX_ALPHA0,
	GX_ALPHA1,

	GX_COLOR0A0,
	GX_COLOR1A1,

	GX_COLOR_ZERO,

	GX_ALPHA_BUMP,
	GX_ALPHA_BUMPN,

	GX_COLOR_NULL = 0xff
} GXChannelID;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x401562
typedef enum _GXColorSrc
{
	GX_SRC_REG,
	GX_SRC_VTX
} GXColorSrc;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x45d66e
typedef enum _GXCompare
{
	GX_NEVER,

	GX_LESS,
	GX_EQUAL,
	GX_LEQUAL,
	GX_GREATER,
	GX_NEQUAL,
	GX_GEQUAL,

	GX_ALWAYS
} GXCompare;

// [SPQE7T]/ISpyD.elf:.debug_info::0x268cce
typedef enum _GXCompCnt
{
	GX_POS_XY = 0,
	GX_POS_XYZ,

	GX_NRM_XYZ = 0,
	GX_NRM_NBT,
	GX_NRM_NBT3,

	GX_CLR_RGB = 0,
	GX_CLR_RGBA,

	GX_TEX_S = 0,
	GX_TEX_ST
} GXCompCnt;

// [SPQE7T]/ISpyD.elf:.debug_info::0x25e1b3
typedef enum _GXCompType
{
	GX_U8,
	GX_S8,
	GX_U16,
	GX_S16,
	GX_F32,

	GX_RGB565 = 0,
	GX_RGB8,
	GX_RGBX8,
	GX_RGBA4,
	GX_RGBA6,
	GX_RGBA8
} GXCompType;

// [SPQE7T]/ISpyD.elf:.debug_info::0x286e6d
typedef enum _GXCullMode
{
	GX_CULL_NONE,
	GX_CULL_FRONT,
	GX_CULL_BACK,
	GX_CULL_ALL
} GXCullMode;

// [SJXE41]/LynWiiRetail.elf:.debug_info::0xd838
typedef enum _GXDiffuseFn
{
	GX_DF_NONE,

	GX_DF_SIGN,
	GX_DF_CLAMP
} GXDiffuseFn;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x48fc70
typedef enum _GXIndTexAlphaSel
{
	GX_ITBA_OFF,

	GX_ITBA_S,
	GX_ITBA_T,
	GX_ITBA_U,

	GX_MAX_ITBALPHA
} GXIndTexAlphaSel;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x48fb29
typedef enum _GXIndTexBiasSel
{
	GX_ITB_NONE,

	GX_ITB_S,
	GX_ITB_T,
	GX_ITB_ST,
	GX_ITB_U,
	GX_ITB_SU,
	GX_ITB_TU,
	GX_ITB_STU,

	GX_MAX_ITBIAS
} GXIndTexBiasSel;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x48fac0
typedef enum _GXIndTexFormat
{
	GX_ITF_8,
	GX_ITF_5,
	GX_ITF_4,
	GX_ITF_3,

	GX_MAX_ITFORMAT
} GXIndTexFormat;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x48ef62
typedef enum _GXIndTexMtxID
{
	GX_ITM_OFF,

	GX_ITM_0,
	GX_ITM_1,
	GX_ITM_2,

	GX_ITM_S0 = 5,
	GX_ITM_S1,
	GX_ITM_S2,

	GX_ITM_T0 = 9,
	GX_ITM_T1,
	GX_ITM_T2
} GXIndTexMtxID;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x490652
typedef enum _GXIndTexScale
{
	GX_ITS_1,
	GX_ITS_2,
	GX_ITS_4,
	GX_ITS_8,
	GX_ITS_16,
	GX_ITS_32,
	GX_ITS_64,
	GX_ITS_128,
	GX_ITS_256,

	GX_MAX_ITSCALE
} GXIndTexScale;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x48fa37
typedef enum _GXIndTexStageID
{
	GX_INDTEXSTAGE0,
	GX_INDTEXSTAGE1,
	GX_INDTEXSTAGE2,
	GX_INDTEXSTAGE3,

	GX_MAX_INDTEXSTAGE
} GXIndTexStageID;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x48fbd5
typedef enum _GXIndTexWrap
{
	GX_ITW_OFF,

	GX_ITW_256,
	GX_ITW_128,
	GX_ITW_64,
	GX_ITW_32,
	GX_ITW_16,
	GX_ITW_0,

	GX_MAX_ITWRAP
} GXIndTexWrap;

// [SPQE7T]/ISpyD.elf:.debug_info::0x1eeb49
typedef enum _GXLightID
{
	GX_LIGHT0		= 1 << 0,
	GX_LIGHT1		= 1 << 1,
	GX_LIGHT2		= 1 << 2,
	GX_LIGHT3		= 1 << 3,
	GX_LIGHT4		= 1 << 4,
	GX_LIGHT5		= 1 << 5,
	GX_LIGHT6		= 1 << 6,
	GX_LIGHT7		= 1 << 7,

	GX_MAX_LIGHT	= 1 << 8,
	GX_LIGHT_NULL	= 0
} GXLightID;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x48f6dc
typedef enum _GXLogicOp
{
	GX_LO_CLEAR,

	GX_LO_AND,
	GX_LO_REVAND,
	GX_LO_COPY,
	GX_LO_INVAND,
	GX_LO_NOOP,
	GX_LO_XOR,
	GX_LO_OR,
	GX_LO_NOR,
	GX_LO_EQUIV,
	GX_LO_INV,
	GX_LO_REVOR,
	GX_LO_INVCOPY,
	GX_LO_INVOR,
	GX_LO_NAND,

	GX_LO_SET
} GXLogicOp;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x455fb4
typedef enum _GXPrimitive
{
	GX_POINTS			= 0xb8,	// 0b1011'1000

	GX_LINES			= 0xa8,	// 0b1010'1000
	GX_LINESTRIP		= 0xb0,	// 0b1011'0000

	GX_TRIANGLES		= 0x90,	// 0x1001'0000
	GX_TRIANGLESTRIP	= 0x98,	// 0x1001'1000
	GX_TRIANGLEFAN		= 0xa0,	// 0x1010'0000

	GX_QUADS			= 0x80	// 0x1000'0000
} GXPrimitive;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x490518
typedef enum _GXTevAlphaArg
{
	GX_CA_APREV,

	GX_CA_A0,
	GX_CA_A1,
	GX_CA_A2,

	GX_CA_TEXA,
	GX_CA_RASA,

	GX_CA_KONST,
	GX_CA_ZERO,
	GX_CA_ONE
} GXTevAlphaArg;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x48fe17
typedef enum _GXTevBias
{
	GX_TB_ZERO,
	GX_TB_ADDHALF,
	GX_TB_SUBHALF,

	GX_MAX_TEVBIAS
} GXTevBias;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x162cf3
typedef enum _GXTevColorArg
{
	GX_CC_CPREV,
	GX_CC_APREV,

	GX_CC_C0,
	GX_CC_A0,
	GX_CC_C1,
	GX_CC_A1,
	GX_CC_C2,
	GX_CC_A2,

	GX_CC_TEXC,
	GX_CC_TEXA,
	GX_CC_RASC,
	GX_CC_RASA,

	GX_CC_ONE,
	GX_CC_HALF,
	GX_CC_KONST,
	GX_CC_ZERO,

	GX_CC_TEXRRR,
	GX_CC_TEXGGG,
	GX_CC_TEXBBB,

	GX_CC_QUARTER	= GX_CC_KONST
} GXTevColorArg;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x48f87f
typedef enum _GXTevColorChan
{
	GX_CH_RED,
	GX_CH_GREEN,
	GX_CH_BLUE,
	GX_CH_ALPHA
} GXTevColorChan;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x490211
typedef enum _GXTevKColorSel
{
	GX_TEV_KCSEL_8_8,
	GX_TEV_KCSEL_7_8,
	GX_TEV_KCSEL_6_8,
	GX_TEV_KCSEL_5_8,
	GX_TEV_KCSEL_4_8,
	GX_TEV_KCSEL_3_8,
	GX_TEV_KCSEL_2_8,
	GX_TEV_KCSEL_1_8,

	GX_TEV_KCSEL_1 = GX_TEV_KCSEL_8_8,
	GX_TEV_KCSEL_3_4 = GX_TEV_KCSEL_6_8,
	GX_TEV_KCSEL_1_2 = GX_TEV_KCSEL_4_8,
	GX_TEV_KCSEL_1_4 = GX_TEV_KCSEL_2_8,

	GX_TEV_KCSEL_K0 = 12,
	GX_TEV_KCSEL_K1,
	GX_TEV_KCSEL_K2,
	GX_TEV_KCSEL_K3,

	GX_TEV_KCSEL_K0_R,
	GX_TEV_KCSEL_K1_R,
	GX_TEV_KCSEL_K2_R,
	GX_TEV_KCSEL_K3_R,
	GX_TEV_KCSEL_K0_G,
	GX_TEV_KCSEL_K1_G,
	GX_TEV_KCSEL_K2_G,
	GX_TEV_KCSEL_K3_G,
	GX_TEV_KCSEL_K0_B,
	GX_TEV_KCSEL_K1_B,
	GX_TEV_KCSEL_K2_B,
	GX_TEV_KCSEL_K3_B,
	GX_TEV_KCSEL_K0_A,
	GX_TEV_KCSEL_K1_A,
	GX_TEV_KCSEL_K2_A,
	GX_TEV_KCSEL_K3_A
} GXTevKColorSel;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x48ff5c2
typedef enum _GXTevKAlphaSel
{
	GX_TEV_KASEL_8_8,
	GX_TEV_KASEL_7_8,
	GX_TEV_KASEL_6_8,
	GX_TEV_KASEL_5_8,
	GX_TEV_KASEL_4_8,
	GX_TEV_KASEL_3_8,
	GX_TEV_KASEL_2_8,
	GX_TEV_KASEL_1_8,

	GX_TEV_KASEL_1 = GX_TEV_KASEL_8_8,
	GX_TEV_KASEL_3_4 = GX_TEV_KASEL_6_8,
	GX_TEV_KASEL_1_2 = GX_TEV_KASEL_4_8,
	GX_TEV_KASEL_1_4 = GX_TEV_KASEL_2_8,

	GX_TEV_KASEL_K0_R = 16,
	GX_TEV_KASEL_K1_R,
	GX_TEV_KASEL_K2_R,
	GX_TEV_KASEL_K3_R,
	GX_TEV_KASEL_K0_G,
	GX_TEV_KASEL_K1_G,
	GX_TEV_KASEL_K2_G,
	GX_TEV_KASEL_K3_G,
	GX_TEV_KASEL_K0_B,
	GX_TEV_KASEL_K1_B,
	GX_TEV_KASEL_K2_B,
	GX_TEV_KASEL_K3_B,
	GX_TEV_KASEL_K0_A,
	GX_TEV_KASEL_K1_A,
	GX_TEV_KASEL_K2_A,
	GX_TEV_KASEL_K3_A
} GXTevKAlphaSel;

// NOTE: older GX version (Feb 24 2009 00:57:49)
// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x107e0
typedef enum _GXTevKColorID
{
	GX_KCOLOR0,
	GX_KCOLOR1,
	GX_KCOLOR2,
	GX_KCOLOR3,

	GX_MAX_KCOLOR
} GXTevKColorID;

// [SPQE7T]/ISpyD.elf:.debug_info::0x1f105e
typedef enum _GXTevMode
{
	GX_MODULATE,
	GX_DECAL,
	GX_BLEND,
	GX_REPLACE,
	GX_PASSCLR
} GXTevMode;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x48fce7
typedef enum _GXTevOp
{
	GX_TEV_ADD,
	GX_TEV_SUB,

	GX_TEV_COMP_R8_GT = 8,
	GX_TEV_COMP_R8_EQ,
	GX_TEV_COMP_GR16_GT,
	GX_TEV_COMP_GR16_EQ,
	GX_TEV_COMP_BGR24_GT,
	GX_TEV_COMP_BGR24_EQ,
	GX_TEV_COMP_RGB8_GT,
	GX_TEV_COMP_RGB8_EQ,

	GX_TEV_COMP_A8_GT = GX_TEV_COMP_RGB8_GT,
	GX_TEV_COMP_A8_EQ = GX_TEV_COMP_RGB8_EQ
} GXTevOp;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x48fef1
typedef enum _GXTevRegID
{
	GX_TEVPREV,

	GX_TEVREG0,
	GX_TEVREG1,
	GX_TEVREG2,

	GX_MAX_TEVREG
} GXTevRegID;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x48fe77
typedef enum _GXTevScale
{
	GX_TEV_SCALE_0,
	GX_TEV_SCALE_1,
	GX_TEV_SCALE_2,
	GX_TEV_SCALE_3
} GXTevScale;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x163019
typedef enum _GXTevStageID
{
	GX_TEVSTAGE0,
	GX_TEVSTAGE1,
	GX_TEVSTAGE2,
	GX_TEVSTAGE3,
	GX_TEVSTAGE4,
	GX_TEVSTAGE5,
	GX_TEVSTAGE6,
	GX_TEVSTAGE7,
	GX_TEVSTAGE8,
	GX_TEVSTAGE9,
	GX_TEVSTAGE10,
	GX_TEVSTAGE11,
	GX_TEVSTAGE12,
	GX_TEVSTAGE13,
	GX_TEVSTAGE14,
	GX_TEVSTAGE15,

	GX_MAX_TEVSTAGE
} GXTevStageID;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x4905c4
typedef enum _GXTevSwapSel
{
	GX_TEV_SWAP0,
	GX_TEV_SWAP1,
	GX_TEV_SWAP2,
	GX_TEV_SWAP3,

	GX_MAX_TEVSWAP
} GXTevSwapSel;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x14a955
typedef enum _GXTexCoordID
{
	GX_TEXCOORD0,
	GX_TEXCOORD1,
	GX_TEXCOORD2,
	GX_TEXCOORD3,
	GX_TEXCOORD4,
	GX_TEXCOORD5,
	GX_TEXCOORD6,
	GX_TEXCOORD7,

	GX_MAX_TEXCOORD,
	GX_TEXCOORD_NULL = 0xff
} GXTexCoordID;

// [SPQE7T]/ISpyD.elf:.debug_info::0x1176fc
typedef enum _GXTexFilter
{
	GX_NEAR,
	GX_LINEAR,

	GX_NEAR_MIP_NEAR,
	GX_LIN_MIP_NEAR,

	GX_NEAR_MIP_LIN,
	GX_LIN_MIP_LIN
} GXTexFilter;

// [SPQE7T]/ISpyD.elf:.debug_info::0x28a5e5
typedef enum _GXTexFmt
{
	GX_TF_I4		=  0,
	GX_TF_I8		=  1,
	GX_TF_IA4		=  2,
	GX_TF_IA8		=  3,

	GX_TF_RGB565	=  4,
	GX_TF_RGB5A3	=  5,
	GX_TF_RGBA8		=  6,

	GX_TF_CMPR		= 14,

	GX_CTF_R4		= 32,
	GX_CTF_RA4		= 34,
	GX_CTF_RA8		= 35,
	GX_CTF_YUVA8	= 38,

	GX_CTF_A8		= 39,
	GX_CTF_R8		= 40,
	GX_CTF_G8		= 41,
	GX_CTF_B8		= 42,
	GX_CTF_RG8		= 43,
	GX_CTF_GB8		= 44,

	GX_TF_Z8		= 17,
	GX_TF_Z16		= 19,
	GX_TF_Z24X8		= 22,

	GX_CTF_Z4		= 48,
	GX_CTF_Z8M		= 57,
	GX_CTF_Z8L		= 58,
	GX_CTF_Z16L		= 60,

	GX_TF_A8		= GX_CTF_A8
} GXTexFmt;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x14ab0b
typedef enum _GXTexGenSrc
{
	GX_TG_POS,

	GX_TG_NRM,
	GX_TG_BINRM,
	GX_TG_TANGENT,

	GX_TG_TEX0,
	GX_TG_TEX1,
	GX_TG_TEX2,
	GX_TG_TEX3,
	GX_TG_TEX4,
	GX_TG_TEX5,
	GX_TG_TEX6,
	GX_TG_TEX7,
	GX_TG_TEXCOORD0,
	GX_TG_TEXCOORD1,
	GX_TG_TEXCOORD2,
	GX_TG_TEXCOORD3,
	GX_TG_TEXCOORD4,
	GX_TG_TEXCOORD5,
	GX_TG_TEXCOORD6,

	GX_TG_COLOR0,
	GX_TG_COLOR1
} GXTexGenSrc;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x14aa2f
typedef enum _GXTexGenType
{
	GX_TG_MTX3x4,
	GX_TG_MTX2x4,

	GX_TG_BUMP0,
	GX_TG_BUMP1,
	GX_TG_BUMP2,
	GX_TG_BUMP3,
	GX_TG_BUMP4,
	GX_TG_BUMP5,
	GX_TG_BUMP6,
	GX_TG_BUMP7,

	GX_TG_SRTG
} GXTexGenType;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x2b93e8
typedef enum _GXTexMapID
{
	GX_TEXMAP0,
	GX_TEXMAP1,
	GX_TEXMAP2,
	GX_TEXMAP3,
	GX_TEXMAP4,
	GX_TEXMAP5,
	GX_TEXMAP6,
	GX_TEXMAP7,

	GX_MAX_TEXMAP,
	GX_TEXMAP_NULL	= 0xff,
	GX_TEX_DISABLE	= 256
} GXTexMapID;

// [SPQE7T]/ISpyD.elf:.debug_info::0x2122a1
typedef enum _GXTexWrapMode
{
	GX_CLAMP,
	GX_REPEAT,
	GX_MIRROR,

	GX_MAX_TEXWRAPMODE
} GXTexWrapMode;

// [SPQE7T]/ISpyD.elf:.debug_info::0x288777
typedef enum _GXTlutFmt
{
	GX_TL_IA8,
	GX_TL_RGB565,
	GX_TL_RGB5A3,

	GX_MAX_TLUTFMT
} GXTlutFmt;

// [SPQE7T]/ISpyD.elf:.debug_info::0x2689ea
typedef enum _GXVtxFmt
{
	GX_VTXFMT0,
	GX_VTXFMT1,
	GX_VTXFMT2,
	GX_VTXFMT3,
	GX_VTXFMT4,
	GX_VTXFMT5,
	GX_VTXFMT6,
	GX_VTXFMT7,

	GX_MAX_VTXFMT
} GXVtxFmt;

/* Patent US 6,664,958 B1
 * [2] https://patents.google.com/patent/US6664958B1
 * section 15, lines 20-25
 */
typedef enum _GXZTexOp
{
	GX_ZT_DISABLE,
	GX_ZT_ADD,
	GX_ZT_REPLACE, // NOTE: misspelled as GZ_ZT_REPLACE in patent

	GX_MAX_ZTEXOP
} GXZTexOp;

// [SGLEA4]/GormitiDebug.elf:.debug_info::0xba05c
typedef struct _GXColor
{
	u8	r;	// size 0x01, offset 0x00
	u8	g;	// size 0x01, offset 0x01
	u8	b;	// size 0x01, offset 0x02
	u8	a;	// size 0x01, offset 0x03
} GXColor; // size 0x04

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x4764f4
typedef struct _GXColorS10
{
	s16	r;	// size 0x02, offset 0x00
	s16	g;	// size 0x02, offset 0x02
	s16	b;	// size 0x02, offset 0x04
	s16	a;	// size 0x02, offset 0x06
} GXColorS10; // size 0x08

// [SPQE7T]/ISpyD.elf:.debug_info::0x11894d
typedef struct _GXTexObj
{
	// i guess they didn't want to expose the internals
	byte4_t	dummy[8];
} GXTexObj; // size 0x20

// [SPQE7T]/ISpyD.elf:.debug_info::0x1eb04c
typedef struct _GXTlutObj
{
	byte4_t	dummy[3];
} GXTlutObj; // size 0x0c

void GXClearVtxDesc(void);
int GXGetTexObjFmt(GXTexObj *);
u16 GXGetTexObjHeight(GXTexObj *);
u16 GXGetTexObjWidth(GXTexObj *);
GXTexWrapMode GXGetTexObjWrapS(GXTexObj *);
GXTexWrapMode GXGetTexObjWrapT(GXTexObj *);
void *GXGetTexObjUserData(GXTexObj *);
void GXInitTexObj(GXTexObj *, void *, u16, u16, GXTexFmt, GXTexWrapMode, GXTexWrapMode, GXBool);
void GXInitTexObjCI(GXTexObj *, void *, u16, u16, GXTexFmt, GXTexWrapMode,
                    GXTexWrapMode, GXBool, u32);
void GXInitTexObjLOD(GXTexObj *, GXTexFilter, GXTexFilter,
                     f32, f32, f32, GXBool,
                     GXBool, GXAnisotropy);
void GXInitTexObjTlut(GXTexObj *, u32);
void GXInitTexObjWrapMode(GXTexObj *, GXTexWrapMode, GXTexWrapMode);
void GXInitTexObjUserData(GXTexObj *, void *);
void GXInitTlutObj(GXTlutObj *, void *, GXTlutFmt, u16);
void GXLoadPosMtxImm(Mtx, int);
void GXLoadTexMtxImm(const Mtx, u32, GXTexGenType);
void GXLoadTexObj(GXTexObj *, int);
void GXLoadTlut(GXTlutObj *, u32);
void GXSetAlphaCompare(GXCompare, u8, GXAlphaOp, GXCompare, u8);
void GXSetAlphaUpdate(GXBool);
void GXSetBlendMode(GXBlendMode, GXBlendFactor, GXBlendFactor, GXLogicOp);
void GXSetChanCtrl(GXChannelID, GXBool, GXColorSrc, GXColorSrc, GXLightID, GXDiffuseFn, GXAttnFn);
void GXSetChanMatColor(GXChannelID, GXColor);
void GXSetColorUpdate(GXBool);
void GXSetCullMode(GXCullMode);
void GXSetCurrentMtx(int);
void GXSetDispCopyGamma(int);
void GXSetFog(int, f32, f32, f32, f32, GXColor);
void GXSetIndTexCoordScale(GXIndTexStageID, GXIndTexScale, GXIndTexScale);
void GXSetIndTexMtx(GXIndTexMtxID, const Mtx23, s8);
void GXSetIndTexOrder(GXIndTexStageID, GXTexCoordID, GXTexMapID);
void GXSetLineWidth(u8, int);
void GXSetNumChans(u8);
void GXSetNumIndStages(u8);
void GXSetNumTevStages(u8);
void GXSetNumTexGens(u8);
void GXSetTevAlphaIn(GXTevStageID, GXTevAlphaArg, GXTevAlphaArg, GXTevAlphaArg, GXTevAlphaArg);
void GXSetTevAlphaOp(GXTevStageID, GXTevOp, GXTevBias, GXTevScale, u8, GXTevRegID);
void GXSetTevColor(GXTevRegID, GXColor);
void GXSetTevColorIn(GXTevStageID, GXTevColorArg, GXTevColorArg, GXTevColorArg, GXTevColorArg);
void GXSetTevColorOp(GXTevStageID, GXTevOp, GXTevBias, GXTevScale, u8, GXTevRegID);
void GXSetTevColorS10(GXTevRegID, GXColorS10);
void GXSetTevDirect(GXTevStageID);
void GXSetTevIndirect(GXTevStageID, GXIndTexStageID, GXIndTexFormat, GXIndTexBiasSel, GXIndTexMtxID, GXIndTexWrap, GXIndTexWrap, GXBool, GXBool, GXIndTexAlphaSel);
void GXSetTevKAlphaSel(GXTevStageID, GXTevKAlphaSel);
void GXSetTevKColor(GXTevKColorID, GXColor);
void GXSetTevKColorSel(GXTevStageID, GXTevKColorSel);
void GXSetTevOp(GXTevStageID, GXTevMode);
void GXSetTevOrder(GXTevStageID, GXTexCoordID, GXTexMapID, GXChannelID);
void GXSetTevSwapMode(GXTevStageID, GXTevSwapSel, GXTevSwapSel);
void GXSetTevSwapModeTable(GXTevSwapSel, GXTevColorChan, GXTevColorChan, GXTevColorChan, GXTevColorChan);
void GXSetTexCoordCylWrap(int, u8, u8);
void GXSetTexCoordGen2(GXTexCoordID, GXTexGenType, GXTexGenSrc, u32, GXBool, u32);
void GXSetTexCoordScaleManually(int, u8, u16, u16);
void GXSetVtxAttrFmt(GXVtxFmt, GXAttr, GXCompCnt, GXCompType, u8);
void GXSetVtxDesc(GXAttr, GXAttrType);
void GXSetZCompLoc(u8);
void GXSetZMode(GXBool, GXCompare, GXBool);
void GXSetZTexture(GXZTexOp op, GXTexFmt fmt, u32 bias);
void GXInitTexObjFilter(GXTexObj *, u32, u32);

// ---

static inline void GXSetTexCoordGen(GXTexCoordID dst_coord, GXTexGenType func,
                                    GXTexGenSrc src_param, u32 mtx)
{
	GXSetTexCoordGen2(dst_coord, func, src_param, mtx, FALSE, 0x7d);
}

void GXBegin(GXPrimitive, GXVtxFmt, u16);

static inline void GXEnd(void)
{
#if !defined(NDEBUG)
	extern GXBool __GXinBegin;

	if (!__GXinBegin)
	{
		OSError_FileLine("GXGeometry.h", 127,
		                 "GXEnd: called without a GXBegin");
	}

	__GXinBegin = FALSE;
#endif // !defined(NDEBUG)
}

union WGPipe
{
	u16	u16;	// size 0x02
	u32	u32;	// size 0x04
	f32	f32;	// size 0x04
}; // size 0x04

extern volatile union WGPipe __WGPipe AT_ADDRESS(0xcc008000);

extern void GXPosition2f32(f32 x, f32 y);
#if defined(NDEBUG)
inline void GXPosition2f32(f32 x, f32 y)
{
	__WGPipe.f32 = x;
	__WGPipe.f32 = y;
}
#endif

extern void GXPosition3f32(f32 x, f32 y, f32 z);
#if defined(NDEBUG)
inline void GXPosition3f32(f32 x, f32 y, f32 z)
{
	__WGPipe.f32 = x;
	__WGPipe.f32 = y;
	__WGPipe.f32 = z;
}
#endif

extern void GXColor1u32(u32 color);
#if defined(NDEBUG)
inline void GXColor1u32(u32 color)
{
	__WGPipe.u32 = color;
}
#endif

extern void GXTexCoord2u16(u16 u, u16 v);
#if defined(NDEBUG)
inline void GXTexCoord2u16(u16 u, u16 v)
{
	__WGPipe.u16 = u;
	__WGPipe.u16 = v;
}
#endif

extern void GXTexCoord2f32(f32 u, f32 v);
#if defined(NDEBUG)
inline void GXTexCoord2f32(f32 u, f32 v)
{
	__WGPipe.f32 = u;
	__WGPipe.f32 = v;
}
#endif

#endif // RVL_SDK_GX_H
