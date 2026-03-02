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

// MTX
// C version
void C_MTXIdentity(Mtx m);
void C_MTXCopy(const Mtx src, Mtx dst);
void C_MTXConcat(const Mtx a, const Mtx b, Mtx ab);
void C_MTXConcatArray(const Mtx a, const Mtx* srcBase, Mtx* dstBase, u32 count);
void C_MTXTranspose(const Mtx src, Mtx xPose);
u32 C_MTXInverse(const Mtx src, Mtx inv);
u32 C_MTXInvXpose(const Mtx src, Mtx invX);
void C_MTXRotRad(Mtx m, char axis, f32 rad);
void C_MTXRotTrig(Mtx m, char axis, f32 sinA, f32 cosA);
void C_MTXRotAxisRad(Mtx m, const Vec* axis, f32 rad);
void C_MTXTrans(Mtx m, f32 xT, f32 yT, f32 zT);
void C_MTXTransApply(const Mtx src, Mtx dst, f32 xT, f32 yT, f32 zT);
void C_MTXScale(Mtx m, f32 xS, f32 yS, f32 zS);
void C_MTXScaleApply(const Mtx src, Mtx dst, f32 xS, f32 yS, f32 zS);
void C_MTXQuat(Mtx m, const Quaternion* q);
void C_MTXReflect(Mtx m, const Vec* p, const Vec* n);

// PS version
void PSMTXIdentity(Mtx m);
void PSMTXCopy(const Mtx src, Mtx dst);
void PSMTXConcat(const Mtx a, const Mtx b, Mtx ab);
void PSMTXConcatArray(const Mtx a, const Mtx* srcBase, Mtx* dstBase, u32 count);
void PSMTXTranspose(const Mtx src, Mtx xPose);
u32 PSMTXInverse(const Mtx src, Mtx inv);
u32 PSMTXInvXpose(const Mtx src, Mtx invX);
void PSMTXRotRad(Mtx m, char axis, f32 rad);
void PSMTXRotTrig(Mtx m, char axis, f32 sinA, f32 cosA);
void PSMTXRotAxisRad(Mtx m, const Vec* axis, f32 rad);
void PSMTXTrans(Mtx m, f32 xT, f32 yT, f32 zT);
void PSMTXTransApply(const Mtx src, Mtx dst, f32 xT, f32 yT, f32 zT);
void PSMTXScale(Mtx m, f32 xS, f32 yS, f32 zS);
void PSMTXScaleApply(const Mtx src, Mtx dst, f32 xS, f32 yS, f32 zS);
void PSMTXQuat(Mtx m, const Quaternion* q);
void PSMTXReflect(Mtx m, const Vec* p, const Vec* n);

#ifdef DEBUG
#define MTXIdentity   C_MTXIdentity
#define MTXCopy       C_MTXCopy
#define MTXConcat     C_MTXConcat
#define MTXInverse    C_MTXInverse
#define MTXTranspose  C_MTXTranspose
#define MTXInverse    C_MTXInverse
#define MTXInvXpose   C_MTXInvXpose
#define MTXRotRad     C_MTXRotRad
#define MTXRotTrig    C_MTXRotTrig
#define MTXRotAxisRad C_MTXRotAxisRad
#define MTXTrans      C_MTXTrans
#define MTXTransApply C_MTXTransApply
#define MTXScale      C_MTXScale
#define MTXScaleApply C_MTXScaleApply
#define MTXQuat       C_MTXQuat
#define MTXReflect    C_MTXReflect
#else
#define MTXIdentity   PSMTXIdentity
#define MTXCopy       PSMTXCopy
#define MTXConcat     PSMTXConcat
#define MTXInverse    PSMTXInverse
#define MTXTranspose  PSMTXTranspose
#define MTXInverse    PSMTXInverse
#define MTXInvXpose   PSMTXInvXpose
#define MTXRotRad     PSMTXRotRad
#define MTXRotTrig    PSMTXRotTrig
#define MTXRotAxisRad PSMTXRotAxisRad
#define MTXTrans      PSMTXTrans
#define MTXTransApply PSMTXTransApply
#define MTXScale      PSMTXScale
#define MTXScaleApply PSMTXScaleApply
#define MTXQuat       PSMTXQuat
#define MTXReflect    PSMTXReflect
#endif

// C versions only
void C_MTXLookAt(Mtx m, const Vec* camPos, const Vec* camUp, const Vec* target);
void C_MTXLightFrustum(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 scaleS, f32 scaleT, f32 transS, f32 transT);
void C_MTXLightPerspective(Mtx m, f32 fovY, f32 aspect, f32 scaleS, f32 scaleT, f32 transS, f32 transT);
void C_MTXLightOrtho(Mtx m, f32 t, f32 b, f32 l, f32 r, f32 scaleS, f32 scaleT, f32 transS, f32 transT);

#define MTXLookAt           C_MTXLookAt
#define MTXLightFrustum     C_MTXLightFrustum
#define MTXLightPerspective C_MTXLightPerspective
#define MTXLightOrtho       C_MTXLightOrtho


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

// MTXVEC
// C versions
void C_MTXMultVec(const Mtx m, const Vec* src, Vec* dst);
void C_MTXMultVecArray(const Mtx m, const Vec* srcBase, Vec* dstBase, u32 count);
void C_MTXMultVecSR(const Mtx m, const Vec* src, Vec* dst);
void C_MTXMultVecArraySR(const Mtx m, const Vec* srcBase, Vec* dstBase, u32 count);

// PS versions
void PSMTXMultVec(const Mtx m, const Vec* src, Vec* dst);
void PSMTXMultVecArray(const Mtx m, const Vec* srcBase, Vec* dstBase, u32 count);
void PSMTXMultVecSR(const Mtx m, const Vec* src, Vec* dst);
void PSMTXMultVecArraySR(const Mtx m, const Vec* srcBase, Vec* dstBase, u32 count);

#ifdef DEBUG
#define MTXMultVec        C_MTXMultVec
#define MTXMultVecArray   C_MTXMultVecArray
#define MTXMultVecSR      C_MTXMultVecSR
#define MTXMultVecArraySR C_MTXMultVecArraySR
#else
#define MTXMultVec        PSMTXMultVec
#define MTXMultVecArray   PSMTXMultVecArray
#define MTXMultVecSR      PSMTXMultVecSR
#define MTXMultVecArraySR PSMTXMultVecArraySR
#endif

#ifdef __cplusplus
    }
#endif

#endif // RVL_SDK_MTX_TYPES_H