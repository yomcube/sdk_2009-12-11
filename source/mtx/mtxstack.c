#include "__internal/__NULL.h"
#include "revolution/types.h"
#include <revolution/mtx.h>
#include <context_rvl.h>

void MTXInitStack(MTXStack* sPtr, u32 numMtx) {
    OSAssertMessage_Line(80, sPtr, "MTXInitStack():  NULL MtxStackPtr 'sPtr' ");
    OSAssertMessage_Line(81, sPtr->stackBase, "MTXInitStack():  'sPtr' contains a NULL ptr to stack memory ");
    OSAssertMessage_Line(82, numMtx, "MTXInitStack():  'numMtx' is 0 ");

    sPtr->numMtx = numMtx;
    sPtr->stackPtr = 0;
}

MtxPtr MTXPush(MTXStack* sPtr, const Mtx m) {
    OSAssertMessage_Line(110, sPtr, "MTXPush():  NULL MtxStackPtr 'sPtr' ");
    OSAssertMessage_Line(111, sPtr->stackBase, "MTXPush():  'sPtr' contains a NULL ptr to stack memory ");
    OSAssertMessage_Line(112, m, "MTXPush():  NULL MtxPtr 'm' ");

    if (sPtr->stackPtr == nullptr) {
        sPtr->stackPtr = sPtr->stackBase;
        MTXCopy(m, sPtr->stackPtr);
    } else {
        OSAssertMessage_Line(127, ((((s32)sPtr->stackPtr - (s32)sPtr->stackBase) / 16) / 3) < (sPtr->numMtx - 1), "MTXPush():  stack overflow ");
        MTXCopy(m, sPtr->stackPtr + 3);
        sPtr->stackPtr += 3;
    }

    return sPtr->stackPtr;
}

MtxPtr MTXPushFwd(MTXStack* sPtr, const Mtx m) {
    OSAssertMessage_Line(163, sPtr, "MTXPushFwd():  NULL MtxStackPtr 'sPtr' ");
    OSAssertMessage_Line(164, sPtr->stackBase, "MTXPushFwd():  'sPtr' contains a NULL ptr to stack memory ");
    OSAssertMessage_Line(165, m, "MTXPushFwd():  NULL MtxPtr 'm' ");

    if (sPtr->stackPtr == NULL) {
        sPtr->stackPtr = sPtr->stackBase;
        MTXCopy(m, sPtr->stackPtr);
    } else {
        OSAssertMessage_Line(180, ((((s32)sPtr->stackPtr - (s32)sPtr->stackBase) / 16) / 3) < (sPtr->numMtx - 1), "MTXPushFwd():  stack overflow");
        MTXConcat(sPtr->stackPtr, m, sPtr->stackPtr + 3);
        sPtr->stackPtr += 3;
    }

    return sPtr->stackPtr;
}

MtxPtr MTXPushInv(MTXStack* sPtr, const Mtx m) {
    Mtx mInv;
    OSAssertMessage_Line(222, sPtr, "MTXPushInv():  NULL MtxStackPtr 'sPtr' ");
    OSAssertMessage_Line(223, sPtr->stackBase, "MTXPushInv():  'sPtr' contains a NULL ptr to stack memory ");
    OSAssertMessage_Line(224, m, "MTXPushInv():  NULL MtxPtr 'm' ");

    MTXInverse(m, mInv);
    if (sPtr->stackPtr == NULL) {
        sPtr->stackPtr = sPtr->stackBase;
        MTXCopy(mInv, sPtr->stackPtr);
    } else {
        OSAssertMessage_Line(242, ((((s32)sPtr->stackPtr - (s32)sPtr->stackBase) / 16) / 3) < (sPtr->numMtx - 1), "MTXPushInv():  stack overflow");
        MTXConcat(mInv, sPtr->stackPtr, sPtr->stackPtr + 3);
        sPtr->stackPtr += 3;
    }

    return sPtr->stackPtr;
}

MtxPtr MTXPushInvXpose(MTXStack* sPtr, const Mtx m) {
    Mtx mIT;
    OSAssertMessage_Line(285, sPtr, "MTXPushInvXpose():  NULL MtxStackPtr 'sPtr' ");
    OSAssertMessage_Line(286, sPtr->stackBase, "MTXPushInvXpose():  'sPtr' contains a NULL ptr to stack memory ");
    OSAssertMessage_Line(287, m, "MTXPushInvXpose():  NULL MtxPtr 'm' ");

    MTXInverse(m, mIT);
    MTXTranspose(mIT, mIT);
    if (sPtr->stackPtr == NULL) {
        sPtr->stackPtr = sPtr->stackBase;
        MTXCopy(mIT, sPtr->stackPtr);
    } else {
        OSAssertMessage_Line(306, ((((s32)sPtr->stackPtr - (s32)sPtr->stackBase) / 16) / 3) < (sPtr->numMtx - 1), "MTXPushInvXpose():  stack overflow ");
        MTXConcat(sPtr->stackPtr, mIT, sPtr->stackPtr + 3);
        sPtr->stackPtr += 3;
    }

    return sPtr->stackPtr;
}

MtxPtr MTXPop(MTXStack* sPtr) {
    OSAssertMessage_Line(334, sPtr, "MTXPop():  NULL MtxStackPtr 'sPtr' ");
    OSAssertMessage_Line(335, sPtr->stackBase, "MTXPop():  'sPtr' contains a NULL ptr to stack memory ");
    
    if (sPtr->stackPtr == NULL) {
        return NULL;
    }

    if (sPtr->stackBase == sPtr->stackPtr) {
        sPtr->stackPtr = NULL;
        return NULL;
    }

    sPtr->stackPtr -= 3;
    return sPtr->stackPtr;
}

MtxPtr MTXGetStackPtr(const MTXStack* sPtr) {
    OSAssertMessage_Line(372, sPtr, "MTXGetStackPtr():  NULL MtxStackPtr 'sPtr' ");
    OSAssertMessage_Line(373, sPtr->stackBase, "MTXGetStackPtr():  'sPtr' contains a NULL ptr to stack memory ");
    return sPtr->stackPtr;
}
