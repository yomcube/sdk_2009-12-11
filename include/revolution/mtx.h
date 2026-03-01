#ifndef RVL_SDK_MTX_TYPES_H
#define RVL_SDK_MTX_TYPES_H

/*******************************************************************************
 * headers
 */
#include <revolution/types.h>

/*******************************************************************************
 * types
 */
#ifdef __cplusplus
    extern "C" {
#endif

// Matrices

typedef f32 Mtx23[2][3];
typedef f32 Mtx[3][4];
typedef f32 Mtx43[4][3];
typedef f32 Mtx44[4][4];

typedef f32 (*Mtx23Ptr)[3];
typedef f32 (*MtxPtr)[4];
typedef f32 (*Mtx43Ptr)[3];
typedef f32 (*Mtx44Ptr)[4];

// not official names, just what i thought would make sense
typedef f32 const (*CMtx23Ptr)[3];
typedef f32 const (*CMtxPtr)[4];
typedef f32 const (*CMtx43Ptr)[3];
typedef f32 const (*CMtx44Ptr)[4];

// Vectors

// [SPQE7T]/ISpyD.elf:.debug_info::0xd64ea
typedef struct Vec2
{
	f32	x;	// size 0x04, offset 0x00
	f32	y;	// size 0x04, offset 0x04
} Vec2; // size 0x08

// [SPQE7T]/ISpyD.elf:.debug_info::0xd64bb
typedef struct Vec
{
	f32	x;	// size 0x04, offset 0x00
	f32	y;	// size 0x04, offset 0x04
	f32	z;	// size 0x04, offset 0x08
} Vec; // size 0x0c

// I think some of these names were in assert strings in debug MTX
typedef Vec2 *Vec2Ptr;
typedef Vec *VecPtr;

typedef Vec2 const *CVec2Ptr;
typedef Vec const *CVecPtr;

typedef struct Quaternion {
    f32 x, y, z, w;
} Quaternion;

// Matrices
#if !defined(NDEBUG)
# define MTXIdentity	C_MTXIdentity
# define MTXConcat		C_MTXConcat
# define MTXInverse		C_MTXInverse
# define MTXRotRad		C_MTXRotRad
# define MTXRotAxisRad	C_MTXRotAxisRad
# define MTXTrans		C_MTXTrans
# define MTXTransApply	C_MTXTransApply
# define MTXScale		C_MTXScale

# define MTXMultVec		C_MTXMultVec
#else
# define MTXIdentity	PSMTXIdentity
# define MTXConcat		PSMTXConcat
# define MTXInverse		PSMTXInverse
# define MTXRotRad		PSMTXRotRad
# define MTXRotAxisRad	PSMTXRotAxisRad
# define MTXTrans		PSMTXTrans
# define MTXTransApply	PSMTXTransApply
# define MTXScale		PSMTXScale

# define MTXMultVec		PSMTXMultVec
#endif

void PSMTXIdentity(Mtx m);
void C_MTXIdentity(Mtx m);

void PSMTXCopy(CMtxPtr src, MtxPtr dst);

void PSMTXConcat(CMtxPtr a, CMtxPtr b, MtxPtr ab);
void C_MTXConcat(CMtxPtr a, CMtxPtr b, MtxPtr ab);

BOOL PSMTXInverse(CMtxPtr src, MtxPtr inv);
BOOL C_MTXInverse(CMtxPtr src, MtxPtr inv);

void PSMTXRotRad(Mtx m, char axis, f32);
void C_MTXRotRad(Mtx m, char axis, f32);

void PSMTXRotAxisRad(MtxPtr, CVecPtr, f32);
void C_MTXRotAxisRad(MtxPtr, CVecPtr, f32);

void PSMTXTrans(Mtx m, f32, f32, f32);
void C_MTXTrans(Mtx m, f32, f32, f32);

void PSMTXTransApply(CMtxPtr src, MtxPtr dst, f32, f32, f32);
void C_MTXTransApply(CMtxPtr src, MtxPtr dst, f32, f32, f32);

void PSMTXScale(MtxPtr m, f32, f32, f32);
void C_MTXScale(MtxPtr m, f32, f32, f32);

void PSMTXMultVec(CMtxPtr m, CVecPtr src, VecPtr dst);
void C_MTXMultVec(CMtxPtr m, CVecPtr src, VecPtr dst);

// Vectors
// C versions
void C_VECAdd(CVecPtr a, CVecPtr b, VecPtr ab);
void C_VECSubtract(CVecPtr a, CVecPtr b, VecPtr ab);
void C_VECScale(CVecPtr src, VecPtr dst, f32 scale);
void C_VECNormalize(CVecPtr src, VecPtr unit);
f32 C_VECSquareMag(CVecPtr v);
f32 C_VECMag(CVecPtr v);
f32 C_VECDotProduct(CVecPtr a, CVecPtr b);
void C_VECCrossProduct(CVecPtr a, CVecPtr b, VecPtr ab);
f32 C_VECSquareDistance(CVecPtr a, CVecPtr b);
f32 C_VECDistance(CVecPtr a, CVecPtr b);

// PS versions
void PSVECAdd(CVecPtr a, CVecPtr b, VecPtr ab);
void PSVECSubtract(CVecPtr a, CVecPtr b, VecPtr ab);
void PSVECScale(CVecPtr src, VecPtr dst, f32 scale);
void PSVECNormalize(CVecPtr src, VecPtr dst);
f32 PSVECSquareMag(CVecPtr v);
f32 PSVECMag(CVecPtr v);
f32 PSVECDotProduct(CVecPtr a, CVecPtr b);
void PSVECCrossProduct(CVecPtr a, CVecPtr b, VecPtr ab);
f32 PSVECSquareDistance(CVecPtr a, CVecPtr b);
f32 PSVECDistance(CVecPtr a, CVecPtr b);

#if NDEBUG
#define VECAdd            PSVECAdd
#define VECSubtract       PSVECSubtract
#define VECScale          PSVECScale
#define VECNormalize      PSVECNormalize
#define VECSquareMag      PSVECSquareMag
#define VECMag            PSVECMag
#define VECDotProduct     PSVECDotProduct
#define VECCrossProduct   PSVECCrossProduct
#define VECSquareDistance PSVECSquareDistance
#define VECDistance       PSVECDistance
#else
#define VECAdd            C_VECAdd
#define VECSubtract       C_VECSubtract
#define VECScale          C_VECScale
#define VECNormalize      C_VECNormalize
#define VECSquareMag      C_VECSquareMag
#define VECMag            C_VECMag
#define VECDotProduct     C_VECDotProduct
#define VECCrossProduct   C_VECCrossProduct
#define VECSquareDistance C_VECSquareDistance
#define VECDistance       C_VECDistance
#endif

void C_VECHalfAngle(CVecPtr a, CVecPtr b, VecPtr half);
void C_VECReflect(CVecPtr src, CVecPtr normal, VecPtr dst);

#define VECHalfAngle C_VECHalfAngle
#define VECReflect   C_VECReflect

#ifdef __cplusplus
    }
#endif

#endif // RVL_SDK_MTX_TYPES_H