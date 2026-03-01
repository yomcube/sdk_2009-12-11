#include "macros.h"
#include <revolution/mtx.h>
#include <math.h>
#include <context_rvl.h>

void C_VECAdd(CVecPtr a, CVecPtr b, VecPtr ab) {
    OSAssertMessage_Line(114, a, "VECAdd():  NULL VecPtr 'a' ");
    OSAssertMessage_Line(115, b, "VECAdd():  NULL VecPtr 'b' ");
    OSAssertMessage_Line(116, ab, "VECAdd():  NULL VecPtr 'ab' ");
    ab->x = a->x + b->x;
    ab->y = a->y + b->y;
    ab->z = a->z + b->z;
}

asm void PSVECAdd(register CVecPtr a, register CVecPtr b, register VecPtr ab) {
    psq_l f2, Vec.x(a), 0, 0
    psq_l f4, Vec.x(b), 0, 0
    ps_add f6, f2, f4
    psq_st f6, Vec.x(ab), 0, 0
    psq_l f3, Vec.z(a), 1, 0
    psq_l f5, Vec.z(b), 1, 0
    ps_add f7, f3, f5
    psq_st f7, Vec.z(ab), 1, 0
}


void C_VECSubtract(CVecPtr a, CVecPtr b, VecPtr a_b) {
    OSAssertMessage_Line(183, a, "VECSubtract():  NULL VecPtr 'a' ");
    OSAssertMessage_Line(184, b, "VECSubtract():  NULL VecPtr 'b' ");
    OSAssertMessage_Line(185, a_b, "VECSubtract():  NULL VecPtr 'a_b' ");
    a_b->x = a->x - b->x;
    a_b->y = a->y - b->y;
    a_b->z = a->z - b->z;
}

asm void PSVECSubtract(register CVecPtr a, register CVecPtr b, register VecPtr ab) {
    psq_l f2, Vec.x(a), 0, 0
    psq_l f4, Vec.x(b), 0, 0
    ps_sub f6, f2, f4
    psq_st f6, Vec.x(ab), 0, 0
    psq_l f3, Vec.z(a), 1, 0
    psq_l f5, Vec.z(b), 1, 0
    ps_sub f7, f3, f5
    psq_st f7, Vec.z(ab), 1, 0
}


void C_VECScale(CVecPtr src, VecPtr dst, f32 scale) {
    OSAssertMessage_Line(253, src, "VECScale():  NULL VecPtr 'src' ");
    OSAssertMessage_Line(254, dst, "VECScale():  NULL VecPtr 'dst' ");
    dst->x = src->x * scale;
    dst->y = src->y * scale;
    dst->z = src->z * scale;
}

void PSVECScale(register CVecPtr src, register VecPtr dst, register f32 scalar) {
    register f32 vxy, vz, rxy, rz;

    asm {
        psq_l vxy, 0x0(src), 0, 0
        psq_l vz, 0x8(src), 1, 0
        ps_muls0 rxy, vxy, scalar
        psq_st rxy, 0x0(dst), 0, 0
        ps_muls0 rz, vz, scalar
        psq_st rz, 0x8(dst), 1, 0
    }
}


void C_VECNormalize(CVecPtr src, VecPtr unit) {
    f32 mag;

    OSAssertMessage_Line(321, src, "VECNormalize():  NULL VecPtr 'src' ");
    OSAssertMessage_Line(322, unit, "VECNormalize():  NULL VecPtr 'unit' ");


    mag = (src->z * src->z) + ((src->x * src->x) + (src->y * src->y));
    OSAssertMessage_Line(327, 0.0f != mag, "VECNormalize():  zero magnitude vector ");

    mag = 1.0f/ sqrtf(mag);
    unit->x = src->x * mag;
    unit->y = src->y * mag;
    unit->z = src->z * mag;
}

void PSVECNormalize(register CVecPtr src, register VecPtr unit) {
    register f32 c_half = 0.5f;
    register f32 c_three = 3.0f;
    register f32 v1_xy;
    register f32 v1_z;
    register f32 xx_zz;
    register f32 xx_yy;
    register f32 sqsum;
    register f32 rsqrt;
    register f32 nwork0;
    register f32 nwork1;

    asm {
        psq_l v1_xy, 0x0(src), 0, 0
        ps_mul xx_yy, v1_xy, v1_xy
        psq_l v1_z, 0x8(src), 1, 0
        ps_madd xx_zz, v1_z, v1_z, xx_yy
        ps_sum0 sqsum, xx_zz, v1_z, xx_yy
        frsqrte rsqrt, sqsum
        fmuls nwork0, rsqrt, rsqrt
        fmuls nwork1, rsqrt, c_half
        fnmsubs nwork0, nwork0, sqsum, c_three
        fmuls rsqrt, nwork0, nwork1
        ps_muls0 v1_xy, v1_xy, rsqrt
        psq_st v1_xy, 0x0(unit), 0, 0
        ps_muls0 v1_z, v1_z, rsqrt
        psq_st v1_z, 0x8(unit), 1, 0
    }
}


f32 C_VECSquareMag(CVecPtr v) {
    f32 sqmag;

    OSAssertMessage_Line(411, v, "VECMag():  NULL VecPtr 'v' ");

    sqmag = (v->x * v->x) + (v->y * v->y) + (v->z * v->z);
    return sqmag;
}

f32 PSVECSquareMag(register CVecPtr v) {
    register f32 vxy, vzz, sqmag;

    asm {
        psq_l vxy, 0x0(v), 0, 0
        ps_mul vxy, vxy, vxy
        lfs vzz, 0x8(v)
        ps_madd sqmag, vzz, vzz, vxy
        ps_sum0 sqmag, sqmag, vxy, vxy
    }

    return sqmag;
}



f32 C_VECMag(CVecPtr v) {
    #if NDEBUG
        return sqrt(
            v->x * v->x +
            v->y * v->y +
            v->z * v->z
        );
    #else
        return sqrtf(C_VECSquareMag(v)); 
    #endif
}

f32 PSVECMag(register CVecPtr v) {
    register f32 vxy, vzz;
    register f32 sqmag, mag, rmag;
    register f32 nwork0, nwork1;
    register f32 c_three, c_half;
    

    c_half = 0.5f;

    asm {
        psq_l vxy, 0x0(v), 0, 0
        ps_mul vxy, vxy, vxy
        lfs vzz, 0x8(v)
        ps_madd sqmag, vzz, vzz, vxy
    }

    asm {
        ps_sum0 sqmag, sqmag, vxy, vxy
        fcmpu cr0, sqmag, nwork0
        beqlr
        frsqrte rmag, sqmag
    }

    c_three = 3.0f;

    asm {
        fmuls nwork0, rmag, rmag
        fmuls nwork1, rmag, c_half
        fnmsubs nwork0, nwork0, sqmag, c_three
        fmuls rmag, nwork0, nwork1
        fmuls mag, sqmag, rmag
    }

    return mag;
}


f32 C_VECDotProduct(CVecPtr a, CVecPtr b) {
    f32 dot;

    OSAssertMessage_Line(546, a, "VECDotProduct():  NULL VecPtr 'a' ");
    OSAssertMessage_Line(547, b, "VECDotProduct():  NULL VecPtr 'b' ");
    dot = (a->z * b->z) + ((a->x * b->x) + (a->y * b->y));
    return dot;
}

asm f32 PSVECDotProduct(register CVecPtr a, register CVecPtr b) {
    psq_l f2, Vec.y(a), 0, 0
    psq_l f3, Vec.y(b), 0, 0
    ps_mul f2, f2, f3
    psq_l f5, Vec.x(a), 0, 0
    psq_l f4, Vec.x(b), 0, 0
    ps_madd f3, f5, f4, f2
    ps_sum0 f1, f3, f2, f2
}


void C_VECCrossProduct(CVecPtr a, CVecPtr b, VecPtr axb) {
    Vec vTmp;

    OSAssertMessage_Line(608, a, "VECCrossProduct():  NULL VecPtr 'a' ");
    OSAssertMessage_Line(609, b, "VECCrossProduct():  NULL VecPtr 'b' ");
    OSAssertMessage_Line(610, axb, "VECCrossProduct():  NULL VecPtr 'axb' ");

    vTmp.x = (a->y * b->z) - (a->z * b->y);
    vTmp.y = (a->z * b->x) - (a->x * b->z);
    vTmp.z = (a->x * b->y) - (a->y * b->x);
    axb->x = vTmp.x;
    axb->y = vTmp.y;
    axb->z = vTmp.z;
}

asm void PSVECCrossProduct(register CVecPtr a, register CVecPtr b, register VecPtr axb) {
    psq_l f1, Vec.x(b), 0, 0
    lfs f2, Vec.z(a)
    psq_l f0, Vec.x(a), 0, 0
    ps_merge10 f6, f1, f1
    lfs f3, Vec.z(b)
    ps_mul f4, f1, f2
    ps_muls0 f7, f1, f0
    ps_msub f5, f0, f3, f4
    ps_msub f8, f0, f6, f7
    ps_merge11 f9, f5, f5
    ps_merge01 f10, f5, f8
    psq_st f9, Vec.x(axb), 1, 0
    ps_neg f10, f10
    psq_st f10, Vec.y(axb), 0, 0
}


void C_VECHalfAngle(CVecPtr a, CVecPtr b, VecPtr half) {
    Vec aTmp;
    Vec bTmp;
    Vec hTmp;

    OSAssertMessage_Line(713, a, "VECHalfAngle():  NULL VecPtr 'a' ");
    OSAssertMessage_Line(714, b, "VECHalfAngle():  NULL VecPtr 'b' ");
    OSAssertMessage_Line(715, half, "VECHalfAngle():  NULL VecPtr 'half' ");

    aTmp.x = -a->x;
    aTmp.y = -a->y;
    aTmp.z = -a->z;
    bTmp.x = -b->x;
    bTmp.y = -b->y;
    bTmp.z = -b->z;

    VECNormalize(&aTmp, &aTmp);
    VECNormalize(&bTmp, &bTmp);
    VECAdd(&aTmp, &bTmp, &hTmp);

    if (VECDotProduct(&hTmp, &hTmp) > 0.0f) {
        VECNormalize(&hTmp, half);
        return;
    }
    *half = hTmp;
}

void C_VECReflect(CVecPtr src, CVecPtr normal, VecPtr dst) {
    f32 cosA;
    Vec uI;
    Vec uN;

    OSAssertMessage_Line(769, src, "VECReflect():  NULL VecPtr 'src' ");
    OSAssertMessage_Line(770, normal, "VECReflect():  NULL VecPtr 'normal' ");
    OSAssertMessage_Line(771, dst, "VECReflect():  NULL VecPtr 'dst' ");

    uI.x = -src->x;
    uI.y = -src->y;
    uI.z = -src->z;

    VECNormalize(&uI, &uI);
    VECNormalize(normal, &uN);

    cosA = VECDotProduct(&uI, &uN);
    dst->x = (2.0f * uN.x * cosA) - uI.x;
    dst->y = (2.0f * uN.y * cosA) - uI.y;
    dst->z = (2.0f * uN.z * cosA) - uI.z;
    VECNormalize(dst, dst);
}


f32 C_VECSquareDistance(CVecPtr a, CVecPtr b) {
    Vec delta;

    delta.x = a->x - b->x;
    delta.y = a->y - b->y;
    delta.z = a->z - b->z;
    return (delta.x*delta.x) + (delta.y*delta.y) + (delta.z*delta.z);
}

f32 PSVECSquareDistance(register CVecPtr a, register CVecPtr b) {
    register f32 v0yz, v1yz, v0xy, v1xy, dyz, dxy;
    register f32 sqdist;

    asm {
        psq_l v0yz, 0x4(a), 0, 0
        psq_l v1yz, 0x4(b), 0, 0
        ps_sub dyz, v0yz, v1yz
        psq_l v0xy, 0x0(a), 0, 0
        psq_l v1xy, 0x0(b), 0, 0
        ps_mul dyz, dyz, dyz
        ps_sub dxy, v0xy, v1xy
        ps_madd sqdist, dxy, dxy, dyz
        ps_sum0 sqdist, sqdist, dyz, dyz
    }

    return sqdist;
}


f32 C_VECDistance(CVecPtr a, CVecPtr b) {
    #if NDEBUG
        Vec delta;

        delta.x = a->x - b->x;
        delta.y = a->y - b->y;
        delta.z = a->z - b->z;
        return sqrt((delta.x*delta.x) + (delta.y*delta.y) + (delta.z*delta.z));
    #else
        return sqrtf(C_VECSquareDistance(a, b));
    #endif
}

f32 PSVECDistance(register CVecPtr a, register CVecPtr b) {
    register f32 v0yz, v1yz, v0xy, v1xy, dyz, dxy;
    register f32 sqdist, rdist, dist;
    register f32 nwork0, nwork1;
    register f32 c_half, c_three;

    asm {
        psq_l v0yz, 0x4(a), 0, 0
        psq_l v1yz, 0x4(b), 0, 0
        ps_sub dyz, v0yz, v1yz
        psq_l v0xy, 0x0(a), 0, 0
        psq_l v1xy, 0x0(b), 0, 0
        ps_mul dyz, dyz, dyz
        ps_sub dxy, v0xy, v1xy
    }

    c_half  = 0.5f;

    asm {
        ps_madd sqdist, dxy, dxy, dyz
        ps_sum0 sqdist, sqdist, dyz, dyz
    }

    c_three = 3.0f;

    asm {
        frsqrte rdist, sqdist
        fmuls nwork0, rdist, rdist
        fmuls nwork1, rdist, c_half
        fnmsubs nwork0, nwork0, sqdist, c_three
        fmuls rdist, nwork0, nwork1
        fmuls dist, sqdist, rdist
    }

    return dist;
}





