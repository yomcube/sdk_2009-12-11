#ifndef CONTEXT_RVL_H
#define CONTEXT_RVL_H

#include <macros.h>

#include <revolution/types.h>

// TODO: migrate all code using this context into using normal bte headers
#if !defined(IS_BTE)
# include <context_bte.h>
#endif

#ifdef __cplusplus
	extern "C" {
#endif

/* Contains the context of the other Revolution SDK libraries that the WPAD
 * library needs to compile.
 *
 * This is not the full context; the other half of the context is in
 * <context_bte.h>.
 *
 * Ideally, this file's usages should be replaced with the headers in your
 * project that declare these symbols.
 */

#if !defined(NDEBUG)
# define OSAssertMessage_FileLine(file_, line_, exp_, ...)	\
	(void)((exp_) || (OSPanic(file_, line_, __VA_ARGS__), 0))
#else
# define OSAssertMessage_FileLine(file_, line_, exp_, ...)	\
	((void)0)
#endif

// defined in terms of OSAssertMessage_FileLine
#define OSAssertMessage_Line(line_, exp_, ...)	\
	OSAssertMessage_FileLine(__FILE__, line_, exp_, __VA_ARGS__)
#define OSAssert_Line(line_, exp_)	\
	OSAssertMessage_FileLine(__FILE__, line_, exp_, "Failed assertion " #exp_)

#define ASSERT(exp_) ((void)0)

// OSError
#define OSError_FileLine(file_, line_, ...)	\
	OSPanic(file_, line_, __VA_ARGS__)

// defined in terms of OSError_FileLine
#define OSError_Line(line_, ...)	\
	OSError_FileLine(__FILE__, line_, __VA_ARGS__)

// OS

typedef s64 OSTime;
typedef u32 OSTick;

typedef u8 OSAppType;
enum OSAppType_et
{
	OS_APP_TYPE_IPL		= 0x40,
	OS_APP_TYPE_DVD		= 0x80,
	OS_APP_TYPE_CHANNEL	= 0x81,
};

extern BOOL __OSInIPL;
extern OSTime __OSStartTime;

void OSRegisterVersion(char const *version);
char const *OSGetAppGamename(void);
OSAppType OSGetAppType(void);

#if defined(NDEBUG)
# define OSPhysicalToCached(addr)	(void *)((u32)(addr) + 0x80000000)
# define OSPhysicalToUncached(addr)	(void *)((u32)(addr) + 0xC0000000)
# define OSCachedToPhysical(addr)	(void *)((u32)(addr) - 0x80000000)
# define OSUncachedToPhysical(addr)	(void *)((u32)(addr) - 0xC0000000)
#endif // defined(NDEBUG)

void *(OSPhysicalToCached)(void *addr);
void *(OSPhysicalToUncached)(void *addr);
void *(OSCachedToPhysical)(void *addr);
void *(OSUncachedToPhysical)(void *addr);

typedef struct OSAlarm OSAlarm;
typedef struct OSContext OSContext;

typedef void OSAlarmHandler(OSAlarm *alarm, OSContext *context);

// [SPQE7T]/ISpyD.elf:.debug_info::0x2950d7
struct OSAlarm
{
	OSAlarmHandler	*handler;	// size 0x04, offset 0x00
	u32				tag;		// size 0x04, offset 0x04
	OSTime			fire;		// size 0x08, offset 0x08
	OSAlarm			*prev;		// size 0x04, offset 0x10
	OSAlarm			*next;		// size 0x04, offset 0x14
	OSTime			period;		// size 0x08, offset 0x18
	OSTime			start;		// size 0x08, offset 0x20
	void			*userData;	// size 0x04, offset 0x28
	/* 4 bytes padding */
}; // size 0x30

void OSCreateAlarm(OSAlarm *alarm);
void OSSetAlarm(OSAlarm *alarm, OSTime tick, OSAlarmHandler handler);
void OSSetPeriodicAlarm(OSAlarm *alarm, OSTime tick, OSTime period,
                        OSAlarmHandler handler);
void OSCancelAlarm(OSAlarm *alarm);
void OSSetAlarmUserData(OSAlarm *alarm, void *userData);
void *OSGetAlarmUserData(const OSAlarm *alarm);

#define OSGetAlarmUserDataAny(type_, alarm_)	\
	((type_)(OSGetAlarmUserData(alarm_)))

#define OSSetAlarmUserDataAny(alarm_, data_)	\
	OSSetAlarmUserData(alarm_, (void *)(data_))

void DCFlushRange(void const *buf, u32 len);
void DCInvalidateRange(void const *buf, u32 len);
void ICInvalidateRange(void const *buf, u32 len);

typedef void OSFiber(/* unspecified */);

// [SPQE7T]/ISpyD.elf:.debug_info::0x3135
struct OSContext
{
	register_t	gpr[32];			// size 0x080, offset 0x000
	register_t	cr;					// size 0x004, offset 0x080
	register_t	lr;					// size 0x004, offset 0x084
	register_t	ctr;				// size 0x004, offset 0x088
	register_t	xer;				// size 0x004, offset 0x08c
	f64			fpr[32];			// size 0x100, offset 0x090
	u32			fpscr_pad;			// size 0x004, offset 0x190
	register_t	fpscr;				// size 0x004, offset 0x194
	register_t	srr0;				// size 0x004, offset 0x198
	register_t	srr1;				// size 0x004, offset 0x19c
	u16			mode;				// size 0x002, offset 0x1a0
	u16			state;				// size 0x002, offset 0x1a2
	register_t	gqr[8];				// size 0x020, offset 0x1a4
	u32			psf_pad;			// size 0x004, offset 0x1c4
	f64			psf[32];			// size 0x100, offset 0x1c8
}; // size 0x2c8

void OSSwitchFiber(OSFiber *fiber, void *stack);
void OSSwitchFiberEx(register_t arg0, register_t arg1, register_t arg2,
                     register_t arg3, OSFiber *fiber, void *stack);

byte4_t OSCalcCRC32(void const *data, u32 length);

ATTR_WEAK void OSReport(char const *msg, ...);
ATTR_WEAK void OSPanic(char const *file, int line, char const *msg, ...);

// Initialization
static inline void OSInitFastCast(void)
{
	// TODO: name (verb)

#if defined(__MWERKS__)
	asm // function-level asm not allowed for functions marked inline
	{
		// GQR2 = u8
		li		r3, 4
		oris	r3, r3, 4
		mtspr	0x392, r3

		// GQR3 = u16
		li		r3, 5
		oris	r3, r3, 5
		mtspr	0x393, r3

		// GQR4 = s8
		li		r3, 6
		oris	r3, r3, 6
		mtspr	0x394, r3

		// GQR5 = s16
		li		r3, 7
		oris	r3, r3, 7
		mtspr	0x395, r3
	}
#endif
}

static inline u8 __OSf32tou8(register f32 in)
{
	f32 a;
	register f32 *ptr = &a;
	u8 r;

#if defined(__MWERKS__)
	asm { psq_st in, 0(ptr), true, 2 };
#else
# pragma unused(in)
#endif

	r = *(u8 *)ptr;

	return r;
}

static inline u16 __OSf32tou16(register f32 in)
{
	f32 a;
	register f32 *ptr = &a;
	u16 r;

#if defined(__MWERKS__)
	asm { psq_st in, 0(ptr), true, 3 };
#else
# pragma unused(in)
#endif

	r = *(u16 *)ptr;

	return r;
}

static inline s16 __OSf32tos16(register f32 in)
{
	f32 a;
	register f32 *ptr = &a;
	s16 r;

#if defined(__MWERKS__)
	asm { psq_st in, 0(ptr), true, 5 };
#else
# pragma unused(in)
#endif

	r = *(s16 *)ptr;

	return r;
}

static inline f32 __OSu16tof32(register u16 const *arg)
{
	register f32 ret;

#if defined(__MWERKS__)
	asm { psq_l ret, 0(arg), true, 3 };
#else
# pragma unused(arg)
	ret = 0;
#endif

	return ret;
}

static inline f32 __OSs16tof32(register s16 const *arg)
{
	register f32 ret;

#if defined(__MWERKS__)
	asm { psq_l ret, 0(arg), true, 5 };
#else
# pragma unused(arg)
	ret = 0;
#endif

	return ret;
}

static inline void OSf32tou8(f32 const *in, u8 *out)
{
	*out = __OSf32tou8(*in);
}

static inline void OSf32tou16(f32 const *in, u16 *out)
{
	*out = __OSf32tou16(*in);
}

static inline void OSf32tos16(f32 const *in, s16 *out)
{
	*out = __OSf32tos16(*in);
}

static inline void OSs16tof32(s16 const *in, f32 *out)
{
	*out = __OSs16tof32(in);
}

static inline void OSu16tof32(u16 const *in, f32 *out)
{
	*out = __OSu16tof32(in);
}

#define OS_FONT_SIZE_ANSI	0x020120
#define OS_FONT_SIZE_SJIS	0x120f00

typedef u16 OSFontEncode;
enum OSFontEncode_et
{
	OS_FONT_ENCODE_ANSI,
	OS_FONT_ENCODE_SJIS,
	OS_FONT_ENCODE_2,
	OS_FONT_ENCODE_UTF8,
	OS_FONT_ENCODE_UTF16,
	OS_FONT_ENCODE_UTF32,

	OS_FONT_ENCODE_MAX,

	OS_FONT_ENCODE_INVALID = -1
};

// [SC5PGN]/build/libs/Debug/slamWiiD.a:ut_RomFont.o(1)::.debug_info::0x296 [original object]
typedef struct OSFontHeader
{
	u16	fontType;		// size 0x02, offset 0x00
	u16	firstChar;		// size 0x02, offset 0x02
	u16	lastChar;		// size 0x02, offset 0x04
	u16	invalChar;		// size 0x02, offset 0x06
	u16	ascent;			// size 0x02, offset 0x08
	u16	descent;		// size 0x02, offset 0x0a
	u16	width;			// size 0x02, offset 0x0c
	u16	leading;		// size 0x02, offset 0x0e
	u16	cellWidth;		// size 0x02, offset 0x10
	u16	cellHeight;		// size 0x02, offset 0x12
	u32	sheetSize;		// size 0x04, offset 0x14
	u16	sheetFormat;	// size 0x02, offset 0x18
	u16	sheetColumn;	// size 0x02, offset 0x1a
	u16	sheetRow;		// size 0x02, offset 0x1c
	u16	sheetWidth;		// size 0x02, offset 0x1e
	u16	sheetHeight;	// size 0x02, offset 0x20
	u16	widthTable;		// size 0x02, offset 0x22
	u32	sheetImage;		// size 0x04, offset 0x24
	u32	sheetFullSize;	// size 0x04, offset 0x28
	u8	c0;				// size 0x01, offset 0x2c
	u8	c1;				// size 0x01, offset 0x2d
	u8	c2;				// size 0x01, offset 0x2e
	u8	c3;				// size 0x01, offset 0x2f

} OSFontHeader; // size 0x30

BOOL OSInitFont(OSFontHeader *font);
OSFontEncode OSGetFontEncode(void);
char const *OSGetFontWidth(char const *str, s32 *widthOut);
char const *OSGetFontTexture(char const *str, void **texOut, s32 *xOut,
                             s32 *yOut, s32 *widthOut);

BOOL OSEnableInterrupts(void);
BOOL OSDisableInterrupts(void);
BOOL OSRestoreInterrupts(BOOL status);

typedef struct OSMutex OSMutex;

// [SPQE7T]/ISpyD.elf:.debug_info::0x333a
typedef struct OSMutexLink
{
	OSMutex	*next;	// size 0x04, offset 0x00
	OSMutex	*prev;	// size 0x04, offset 0x04
} OSMutexLink; // size 0x08

typedef int OSShutdownPass;
enum OSShutdownPass_et
{
	OS_SHUTDOWN_PASS_FIRST,
	OS_SHUTDOWN_PASS_SECOND,
};

typedef u32 OSShutdownEvent;
enum OSShutdownEvent_et
{
	OS_SHUTDOWN_EVENT_FATAL,
	OS_SHUTDOWN_EVENT_1,
	OS_SHUTDOWN_EVENT_SHUTDOWN,
	OS_SHUTDOWN_EVENT_3,
	OS_SHUTDOWN_EVENT_RESTART,
	OS_SHUTDOWN_EVENT_RETURN_TO_MENU,
	OS_SHUTDOWN_EVENT_LAUNCH_APP,
};

typedef BOOL OSShutdownFunction(OSShutdownPass pass, OSShutdownEvent event);

typedef struct OSShutdownFunctionInfo OSShutdownFunctionInfo;
struct OSShutdownFunctionInfo
{
	OSShutdownFunction		*func;	// size 0x04, offset 0x00	/* name known from asserts */
	u32						prio;	// size 0x04, offset 0x04
	OSShutdownFunctionInfo	*next;	// size 0x04, offset 0x08
	OSShutdownFunctionInfo	*prev;	// size 0x04, offset 0x0c
}; // size 0x10

typedef struct OSShutdownFunctionQueue
{
	OSShutdownFunctionInfo	*head;	// size 0x04, offset 0x00
	OSShutdownFunctionInfo	*tail;	// size 0x04, offset 0x04
} OSShutdownFunctionQueue; // size 0x08

extern BOOL __OSIsReturnToIdle;

void OSRegisterShutdownFunction(OSShutdownFunctionInfo *info);

#define OSSendMessageAny(msgQueue_, msg_, flags_)	\
	OSSendMessage(msgQueue_, (OSMessage)(msg_), flags_)

#define OSReceiveMessageAny(msgQueue_, msgOut_, flags_)	\
	OSReceiveMessage(msgQueue_, (OSMessage *)(msgOut_), flags_)

#define OSJamMessageAny(msgQueue_, msg_, flags_)	\
	OSJamMessage(msgQueue_, (OSMessage)(msg_), flags_)

#define OSReadMessageAs(msg_, type_)	\
	((type_)(msg_))

typedef void *OSMessage;

typedef u32 OSMessageFlags;
enum OSMessageFlags_et
{
	OS_MESSAGE_NO_FLAGS			= 0,

	OS_MESSAGE_FLAG_PERSISTENT	= 1 << 0,
};

typedef struct OSThread OSThread;

typedef enum OSThreadState
{
	OS_THREAD_STATE_EXITED		= 0,
	OS_THREAD_STATE_READY		= 1 << 0,
	OS_THREAD_STATE_RUNNING		= 1 << 1,
	OS_THREAD_STATE_SLEEPING	= 1 << 2,
	OS_THREAD_STATE_MORIBUND	= 1 << 3,
} OSThreadState;

typedef u16 OSThreadFlags;
enum OSThreadFlags_et
{
	OS_THREAD_NO_FLAGS	= 0,

	OS_THREAD_DETACHED	= 1 << 0,
};

typedef void OSSwitchThreadCallback(OSThread *curThread, OSThread *newThread);
typedef void *OSThreadFunction(void *arg);

// [SPQE7T]/ISpyD.elf:.debug_info::0x2fb2
typedef struct OSThreadQueue
{
	OSThread	*head;	// size 0x04, offset 0x00
	OSThread	*tail;	// size 0x04, offset 0x04
} OSThreadQueue; // size 0x08

// [SPQE7T]/ISpyD.elf:.debug_info::0x328b
typedef struct OSThreadLink
{
	OSThread	*next;	// size 0x04, offset 0x00
	OSThread	*prev;	// size 0x04, offset 0x04
} OSThreadLink; // size 0x08

// [SPQE7T]/ISpyD.elf:.debug_info::0x32cf
typedef struct OSMutexQueue
{
	OSMutex	*head;	// size 0x04, offset 0x00
	OSMutex	*tail;	// size 0x04, offset 0x04
} OSMutexQueue; // size 0x08

// [SPQE7T]/ISpyD.elf:.debug_info::0x2feb
struct OSThread
{
	OSContext		context;		// size 0x2c8, offset 0x000
	u16				state;			// size 0x002, offset 0x2c8
	u16				attr;			// size 0x002, offset 0x2ca
	s32				suspend;		// size 0x004, offset 0x2cc
	s32				priority;		// size 0x004, offset 0x2d0
	s32				base;			// size 0x004, offset 0x2d4
	void			*val;			// size 0x004, offset 0x2d8
	OSThreadQueue	*queue;			// size 0x004, offset 0x2dc
	OSThreadLink	link;			// size 0x008, offset 0x2e0
	OSThreadQueue	queueJoin;		// size 0x008, offset 0x2e8
	OSMutex			*mutex;			// size 0x004, offset 0x2f0
	OSMutexQueue	queueMutex;		// size 0x008, offset 0x2f4
	OSThreadLink	linkActive;		// size 0x008, offset 0x2fc
	u8				*stackBase;		// size 0x004, offset 0x304
	u32				*stackEnd;		// size 0x004, offset 0x308
	s32				error;			// size 0x004, offset 0x30c
	void			*specific[2];	// size 0x008, offset 0x310
}; // size 0x318

void OSInitThreadQueue(OSThreadQueue *queue);
BOOL OSCreateThread(OSThread *thread, OSThreadFunction *func, void *funcArg,
                   void *stackBegin, u32 stackSize, int prio,
                   OSThreadFlags flags);
BOOL OSJoinThread(OSThread *thread, void *val);
s32 OSResumeThread(OSThread *thread);
void OSSleepThread(OSThreadQueue *queue);
void OSWakeupThread(OSThreadQueue *queue);

// [SPQE7T]/ISpyD.elf:.debug_info::0x23508c
typedef struct OSMessageQueue
{
	OSThreadQueue	queueSend;		// size 0x08, offset 0x00
	OSThreadQueue	queueReceive;	// size 0x08, offset 0x08
	OSMessage		*msgArray;		// size 0x04, offset 0x10
	s32				msgCount;		// size 0x04, offset 0x14
	s32				firstIndex;		// size 0x04, offset 0x18
	s32				usedCount;		// size 0x04, offset 0x1c
} OSMessageQueue; // size 0x20

void OSInitMessageQueue(OSMessageQueue *msgQueue, OSMessage *buffer,
                        int capacity);
BOOL OSSendMessage(OSMessageQueue *msgQueue, OSMessage msg,
                   OSMessageFlags flags);
BOOL OSReceiveMessage(OSMessageQueue *msgQueue, OSMessage *msgOut,
                      OSMessageFlags flags);
BOOL OSJamMessage(OSMessageQueue *msgQueue, OSMessage msg,
                  OSMessageFlags flags);

// [SPQE7T]/ISpyD.elf:.debug_info::0x2f63
struct OSMutex
{
	OSThreadQueue	queue;		// size 0x08, offset 0x00
	OSThread		*thread;	// size 0x04, offset 0x08
	s32				count;		// size 0x04, offset 0x0c
	OSMutexLink		link;		// size 0x08, offset 0x10
}; // size 0x18

void OSInitMutex(OSMutex *mutex);

extern u32 __OSBusClock AT_ADDRESS(0x800000f8);

#define OS_TIMER_CLOCK	(__OSBusClock / 4)

// clang-format off
#define OSTicksToSeconds(ticks)			((ticks)        / (OS_TIMER_CLOCK         )       )
#define OSTicksToMilliseconds(ticks)	((ticks)        / (OS_TIMER_CLOCK / 1000  )       )
#define OSTicksToMicroseconds(ticks)	((ticks) * 8    / (OS_TIMER_CLOCK / 125000)       )
#define OSTicksToNanoseconds(ticks)		((ticks) * 8000 / (OS_TIMER_CLOCK / 125000)       )

#define OSSecondsToTicks(sec)			((  sec)        * (OS_TIMER_CLOCK         )       )
#define OSMillisecondsToTicks(msec)		(( msec)        * (OS_TIMER_CLOCK / 1000  )       )
#define OSMicrosecondsToTicks(usec)		(( usec)        * (OS_TIMER_CLOCK / 125000) / 8   )
#define OSNanosecondsToTicks(nsec)		(( nsec)        * (OS_TIMER_CLOCK / 125000) / 8000)

#define OSDiffTick(tick1, tick0)		((OSTick)(tick1) - (OSTick)(tick0))
// clang-format on

OSTime OSGetTime(void);
OSTick OSGetTick(void);
OSTime __OSGetSystemTime(void);

// TODO: is there a way to make this work with an object with address declaration?
#define CURRENT_AFH_CHANNEL_PHYSICAL_ADDR ((void *)(0x31a2))

// AI

BOOL AICheckInit(void);
void AIInit(void *stack);

// ARC

typedef s32 ARCEntryNum;
#define ARC_ENTRY_NUM_INVALID	(-1)

// [SPQE7T]/ISpyD.elf:.debug_info::0x119a90
typedef struct ARCHandle
{
	void		*archiveStartAddr;	// size 0x04, offset 0x00
	void		*FSTStart;			// size 0x04, offset 0x04
	void		*fileStart;			// size 0x04, offset 0x08
	u32			entryNum;			// size 0x04, offset 0x0c // more accurately entryCount
	const char	*FSTStringStart;	// size 0x04, offset 0x10
	u32			FSTLength;			// size 0x04, offset 0x14
	u32			currDir;			// size 0x04, offset 0x18 // more accurately what ARCDir calls entryNum
} ARCHandle; // size 0x1c

// [SPQE7T]/ISpyD.elf:.debug_info::0x368e0b
typedef struct ARCFileInfo
{
	ARCHandle	*handle;		// size 0x04, offset 0x00
	u32			startOffset;	// size 0x04, offset 0x04
	u32			length;			// size 0x04, offset 0x08
} ARCFileInfo; // size 0x0c

// [SPQE7T]/ISpyD.elf:.debug_info::0x3749e4
typedef struct ARCDir
{
	ARCHandle	*handle;	// size 0x04, offset 0x00
	u32			entryNum;	// size 0x04, offset 0x04
	u32			location;	// size 0x04, offset 0x08
	u32			next;		// size 0x04, offset 0x0c
} ARCDir; // size 0x10

// [SPQE7T]/ISpyD.elf:.debug_info::0x374a3e
typedef struct ARCDirEntry
{
	ARCHandle	*handle;	// size 0x04, offset 0x00
	u32			entryNum;	// size 0x04, offset 0x04
	BOOL		isDir;		// size 0x04, offset 0x08
	const char	*name;		// size 0x04, offset 0x0c
} ARCDirEntry; // size 0x10

BOOL ARCInitHandle(void *bin, ARCHandle *handle);
BOOL ARCOpen(ARCHandle *handle, const char *filename, ARCFileInfo *af);
BOOL ARCFastOpen(ARCHandle *handle, int entrynum, ARCFileInfo *af);
s32 ARCConvertPathToEntrynum(ARCHandle *handle, const char *path);
void *ARCGetStartAddrInMem(ARCFileInfo *af);
u32 ARCGetLength(ARCFileInfo *af);
BOOL ARCClose(ARCFileInfo *af);
BOOL ARCChangeDir(ARCHandle *handle, const char *dirname);
BOOL ARCOpenDir(ARCHandle *handle, const char *dirname, ARCDir *dir);
BOOL ARCReadDir(ARCDir *dir, ARCDirEntry *direntry);
BOOL ARCCloseDir(ARCDir *dir);

// AX

#define AX_PRIORITY_STACKS	32	/* name known from asserts */

#define AX_PRIORITY_MAX		(AX_PRIORITY_STACKS - 1)

// [SPQE7T]/ISpyD.elf:.debug_info::0x36a6f8
typedef void AXFrameCallback(void);

// [SPQE7T]/ISpyD.elf:.debug_info::0x36c40e
typedef void AXAuxCallback(void *data, void *context);

typedef enum AXMixerCtrlFlags
{
	AX_MIXER_CTRL_L			= 1 << 0,
	AX_MIXER_CTRL_R			= 1 << 1,
	AX_MIXER_CTRL_DELTA		= 1 << 2,
	AX_MIXER_CTRL_S			= 1 << 3,
	AX_MIXER_CTRL_DELTA_S	= 1 << 4,

	AX_MIXER_CTRL_A_L		= 1 << 16,
	AX_MIXER_CTRL_A_R		= 1 << 17,
	AX_MIXER_CTRL_A_DELTA	= 1 << 18,
	AX_MIXER_CTRL_A_S		= 1 << 19,
	AX_MIXER_CTRL_A_DELTA_S	= 1 << 20,

	AX_MIXER_CTRL_B_L		= 1 << 21,
	AX_MIXER_CTRL_B_R		= 1 << 22,
	AX_MIXER_CTRL_B_DELTA	= 1 << 23,
	AX_MIXER_CTRL_B_S		= 1 << 24,
	AX_MIXER_CTRL_B_DELTA_S	= 1 << 25,

	AX_MIXER_CTRL_C_L		= 1 << 26,
	AX_MIXER_CTRL_C_R		= 1 << 27,
	AX_MIXER_CTRL_C_DELTA	= 1 << 28,
	AX_MIXER_CTRL_C_S		= 1 << 29,
	AX_MIXER_CTRL_C_DELTA_S	= 1 << 30,
} AXMixerCtrlFlags;

typedef u32 AXVPBSrcType;
enum AXVPBSrcType_et
{
	AX_SRC_TYPE_0,
	AX_SRC_TYPE_1,
	AX_SRC_TYPE_2,
	AX_SRC_TYPE_3,
	AX_SRC_TYPE_4TAP_16K,	/* name known from asserts */
};

typedef u32 AXVPBSyncFlags;
enum AXVPBSyncFlags_et
{
	AX_VPB_SYNC_FLAG_SRC_TYPE				= 1 <<  0,
	AX_VPB_SYNC_FLAG_MIXER_CTRL				= 1 <<  1,
	AX_VPB_SYNC_FLAG_STATE					= 1 <<  2,
	AX_VPB_SYNC_FLAG_TYPE					= 1 <<  3,
	AX_VPB_SYNC_FLAG_MIX					= 1 <<  4,
	AX_VPB_SYNC_FLAG_ITD					= 1 <<  5,
	AX_VPB_SYNC_FLAG_ITD_TARGET				= 1 <<  6,
	AX_VPB_SYNC_FLAG_DPOP					= 1 <<  7,
	AX_VPB_SYNC_FLAG_VE						= 1 <<  8,
	AX_VPB_SYNC_FLAG_VE_DELTA				= 1 <<  9,
	AX_VPB_SYNC_FLAG_ADDR					= 1 << 10,
	AX_VPB_SYNC_FLAG_ADDR_LOOP_FLAG			= 1 << 11,
	AX_VPB_SYNC_FLAG_ADDR_LOOP_ADDR			= 1 << 12,
	AX_VPB_SYNC_FLAG_ADDR_END_ADDR			= 1 << 13,
	AX_VPB_SYNC_FLAG_ADDR_CURRENT_ADDR		= 1 << 14,
	AX_VPB_SYNC_FLAG_ADPCM					= 1 << 15,
	AX_VPB_SYNC_FLAG_SRC					= 1 << 16,
	AX_VPB_SYNC_FLAG_SRC_RATIO				= 1 << 17,
	AX_VPB_SYNC_FLAG_ADPCM_LOOP				= 1 << 18,
	AX_VPB_SYNC_FLAG_LPF					= 1 << 19,
	AX_VPB_SYNC_FLAG_LPF_COEFS				= 1 << 20,
	AX_VPB_SYNC_FLAG_BIQUAD					= 1 << 21,
	AX_VPB_SYNC_FLAG_BIQUAD_COEFS			= 1 << 22,
	AX_VPB_SYNC_FLAG_RMT_ON					= 1 << 23,
	AX_VPB_SYNC_FLAG_RMT_MIXER_CTRL			= 1 << 24,
	AX_VPB_SYNC_FLAG_RMT_MIX				= 1 << 25,
	AX_VPB_SYNC_FLAG_RMT_DPOP				= 1 << 26,
	AX_VPB_SYNC_FLAG_RMT_SRC				= 1 << 27,
	AX_VPB_SYNC_FLAG_RMT_IIR				= 1 << 28,
	AX_VPB_SYNC_FLAG_RMT_IIR_LPF_COEFS		= 1 << 29,
	AX_VPB_SYNC_FLAG_RMT_IIR_BIQUAD_COEFS	= 1 << 30,
	AX_VPB_SYNC_FLAG_FULL_PB				= 1 << 31
};

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x82853
typedef void AXVPBCallback(void *p);

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x82a39
typedef struct _AXPBMIX
{
	u16	vL;				// size 0x02, offset 0x00
	u16	vDeltaL;		// size 0x02, offset 0x02
	u16	vR;				// size 0x02, offset 0x04
	u16	vDeltaR;		// size 0x02, offset 0x06
	u16	vAuxAL;			// size 0x02, offset 0x08
	u16	vDeltaAuxAL;	// size 0x02, offset 0x0a
	u16	vAuxAR;			// size 0x02, offset 0x0c
	u16	vDeltaAuxAR;	// size 0x02, offset 0x0e
	u16	vAuxBL;			// size 0x02, offset 0x10
	u16	vDeltaAuxBL;	// size 0x02, offset 0x12
	u16	vAuxBR;			// size 0x02, offset 0x14
	u16	vDeltaAuxBR;	// size 0x02, offset 0x16
	u16	vAuxCL;			// size 0x02, offset 0x18
	u16	vDeltaAuxCL;	// size 0x02, offset 0x1a
	u16	vAuxCR;			// size 0x02, offset 0x1c
	u16	vDeltaAuxCR;	// size 0x02, offset 0x1e
	u16	vS;				// size 0x02, offset 0x10
	u16	vDeltaS;		// size 0x02, offset 0x12
	u16	vAuxAS;			// size 0x02, offset 0x14
	u16	vDeltaAuxAS;	// size 0x02, offset 0x16
	u16	vAuxBS;			// size 0x02, offset 0x18
	u16	vDeltaAuxBS;	// size 0x02, offset 0x1a
	u16	vAuxCS;			// size 0x02, offset 0x1c
	u16	vDeltaAuxCS;	// size 0x02, offset 0x1e
} AXPBMIX; // size 0x30

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x82c05
typedef struct _AXPBITD
{
	u16	flag;			// size 0x02, offset 0x00
	u16	bufferHi;		// size 0x02, offset 0x02
	u16	bufferLo;		// size 0x02, offset 0x04
	u16	shiftL;			// size 0x02, offset 0x06
	u16	shiftR;			// size 0x02, offset 0x08
	u16	targetShiftL;	// size 0x02, offset 0x0a
	u16	targetShiftR;	// size 0x02, offset 0x0c
} AXPBITD; // size 0x0e

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x82c9a
typedef struct _AXPBDPOP
{
	s16	aL;		// size 0x02, offset 0x00
	s16	aAuxAL;	// size 0x02, offset 0x02
	s16	aAuxBL;	// size 0x02, offset 0x04
	s16	aAuxCL;	// size 0x02, offset 0x06
	s16	aR;		// size 0x02, offset 0x08
	s16	aAuxAR;	// size 0x02, offset 0x0a
	s16	aAuxBR;	// size 0x02, offset 0x0c
	s16	aAuxCR;	// size 0x02, offset 0x0e
	s16	aS;		// size 0x02, offset 0x10
	s16	aAuxAS;	// size 0x02, offset 0x12
	s16	aAuxBS;	// size 0x02, offset 0x14
	s16	aAuxCS;	// size 0x02, offset 0x16
} AXPBDPOP; // size 0x18

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x82d6b
typedef struct _AXPBVE
{
	u16	currentVolume;	// size 0x02, offset 0x00
	s16	currentDelta;	// size 0x02, offset 0x02
} AXPBVE; // size 0x04

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x82da9
typedef struct _AXPBADDR
{
	u16	loopFlag;			// size 0x02, offset 0x00
	u16	format;				// size 0x02, offset 0x02
	u16	loopAddressHi;		// size 0x02, offset 0x04
	u16	loopAddressLo;		// size 0x02, offset 0x06
	u16	endAddressHi;		// size 0x02, offset 0x08
	u16	endAddressLo;		// size 0x02, offset 0x0a
	u16	currentAddressHi;	// size 0x02, offset 0x0c
	u16	currentAddressLo;	// size 0x02, offset 0x0e
} AXPBADDR; // size 0x10

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x825d6
typedef struct _AXPBADPCM
{
	u16	a[8][2];	// size 0x20, offset 0x00
	u16	gain;		// size 0x02, offset 0x20
	u16	pred_scale;	// size 0x02, offset 0x22
	u16	yn1;		// size 0x02, offset 0x24
	u16	yn2;		// size 0x02, offset 0x26
} AXPBADPCM; // size 0x28

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x82e72
typedef struct _AXPBSRC
{
	u16	ratioHi;			// size 0x02, offset 0x00
	u16	ratioLo;			// size 0x02, offset 0x02
	u16	currentAddressFrac;	// size 0x02, offset 0x04
	u16	last_samples[4];	// size 0x08, offset 0x06
} AXPBSRC; // size 0x0e

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x82652
typedef struct _AXPBADPCMLOOP
{
	u16	loop_pred_scale;	// size 0x02, offset 0x00
	u16	loop_yn1;			// size 0x02, offset 0x02
	u16	loop_yn2;			// size 0x02, offset 0x04
} AXPBADPCMLOOP; // size 0x06

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x82ee9
typedef struct _AXPBLPF
{
	u16	on;		// size 0x02, offset 0x00
	u16	yn1;	// size 0x02, offset 0x02
	u16	a0;		// size 0x02, offset 0x04
	u16	b0;		// size 0x02, offset 0x06
} AXPBLPF; // size 0x08

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x82f2e
typedef struct _AXPBBIQUAD
{
	u16	on;		// size 0x02, offset 0x00
	u16	xn1;	// size 0x02, offset 0x02
	u16	xn2;	// size 0x02, offset 0x04
	u16	yn1;	// size 0x02, offset 0x06
	u16	yn2;	// size 0x02, offset 0x08
	u16	b0;		// size 0x02, offset 0x0a
	u16	b1;		// size 0x02, offset 0x0c
	u16	b2;		// size 0x02, offset 0x0e
	u16	a1;		// size 0x02, offset 0x10
	u16	a2;		// size 0x02, offset 0x12
} AXPBBIQUAD; // size 0x14

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x82fc7
typedef struct _AXPBRMTMIX
{
	u16	vMain0;			// size 0x02, offset 0x00
	u16	vDeltaMain0;	// size 0x02, offset 0x02
	u16	vAux0;			// size 0x02, offset 0x04
	u16	vDeltaAux0;		// size 0x02, offset 0x06
	u16	vMain1;			// size 0x02, offset 0x08
	u16	vDeltaMain1;	// size 0x02, offset 0x0a
	u16	vAux1;			// size 0x02, offset 0x0c
	u16	vDeltaAux1;		// size 0x02, offset 0x0e
	u16	vMain2;			// size 0x02, offset 0x10
	u16	vDeltaMain2;	// size 0x02, offset 0x12
	u16	vAux2;			// size 0x02, offset 0x14
	u16	vDeltaAux2;		// size 0x02, offset 0x16
	u16	vMain3;			// size 0x02, offset 0x18
	u16	vDeltaMain3;	// size 0x02, offset 0x1a
	u16	vAux3;			// size 0x02, offset 0x1c
	u16	vDeltaAux3;		// size 0x02, offset 0x1e
} AXPBRMTMIX; // size 0x20

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x8310a
typedef struct _AXPBRMTDPOP
{
	s16	aMain0;	// size 0x02, offset 0x00
	s16	aMain1;	// size 0x02, offset 0x02
	s16	aMain2;	// size 0x02, offset 0x04
	s16	aMain3;	// size 0x02, offset 0x06
	s16	aAux0;	// size 0x02, offset 0x08
	s16	aAux1;	// size 0x02, offset 0x0a
	s16	aAux2;	// size 0x02, offset 0x0c
	s16	aAux3;	// size 0x02, offset 0x0e
} AXPBRMTDPOP; // size 0x10

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x831a2
typedef struct _AXPBRMTSRC
{
	u16	currentAddressFrac;	// size 0x02, offset 0x00
	u16	last_samples[4];	// size 0x08, offset 0x02
} AXPBRMTSRC; // size 0x0a

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x831f8
typedef union __AXPBRMTIIR
{
	AXPBLPF		lpf;	// size 0x08
	AXPBBIQUAD	biquad;	// size 0x14
} __AXPBRMTIIR; // size 0x14

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x8286e
typedef struct _AXPB
{
	u16				nextHi;			// size 0x002, offset 0x000 (0x028)
	u16				nextLo;			// size 0x002, offset 0x002 (0x02a)
	u16				currHi;			// size 0x002, offset 0x004 (0x02c)
	u16				currLo;			// size 0x002, offset 0x006 (0x02e)
	u16				srcSelect;		// size 0x002, offset 0x008 (0x030)
	u16				coefSelect;		// size 0x002, offset 0x00a (0x032)
	u32				mixerCtrl;		// size 0x004, offset 0x00c (0x034)
	u16				state;			// size 0x002, offset 0x010 (0x038)
	u16				type;			// size 0x002, offset 0x012 (0x03a)
	AXPBMIX			mix;			// size 0x030, offset 0x014 (0x03c)
	AXPBITD			itd;			// size 0x00e, offset 0x044 (0x06c)
	AXPBDPOP		dpop;			// size 0x018, offset 0x052 (0x07a)
	AXPBVE			ve;				// size 0x004, offset 0x06a (0x092)
	AXPBADDR		addr;			// size 0x010, offset 0x06e (0x096)
	AXPBADPCM		adpcm;			// size 0x028, offset 0x07e (0x0a6)
	AXPBSRC			src;			// size 0x00e, offset 0x0a6 (0x0ce)
	AXPBADPCMLOOP	adpcmLoop;		// size 0x006, offset 0x0b4 (0x0dc)
	AXPBLPF			lpf;			// size 0x008, offset 0x0ba (0x0e2)
	AXPBBIQUAD		biquad;			// size 0x014, offset 0x0c2 (0x0ea)
	u16				remote;			// size 0x002, offset 0x0d6 (0x0fe)
	u16				rmtMixerCtrl;	// size 0x002, offset 0x0d8 (0x100)
	AXPBRMTMIX		rmtMix;			// size 0x020, offset 0x0da (0x102)
	AXPBRMTDPOP		rmtDpop;		// size 0x010, offset 0x0fa (0x122)
	AXPBRMTSRC		rmtSrc;			// size 0x00a, offset 0x10a (0x132)
	__AXPBRMTIIR	rmtIIR;			// size 0x014, offset 0x114 (0x13c)
	byte2_t			pad[12];
} AXPB; // size 0x140

// [SGLEA4]/GormitiDebug.elf:.debug_info::0x82773
typedef struct _AXVPB
{
	void			*next;			// size 0x004, offset 0x000
	void			*prev;			// size 0x004, offset 0x004
	void			*next1;			// size 0x004, offset 0x008
	u32				priority;		// size 0x004, offset 0x00c
	AXVPBCallback	*callback;		// size 0x004, offset 0x010
	register_t		userContext;	// size 0x004, offset 0x014
	u32				index;			// size 0x004, offset 0x018
	AXVPBSyncFlags	sync;			// size 0x004, offset 0x01c
	u32				depop;			// size 0x004, offset 0x020
	void			*itdBuffer;		// size 0x004, offset 0x024
	AXPB			pb;				// size 0x140, offset 0x028
} AXVPB; // size 0x168

void AXInit(void);
BOOL AXIsInit(void);
AXVPB *AXAcquireVoice(u32 priority, AXVPBCallback *cb, register_t userContext);
void AXFreeVoice(AXVPB *p);
AXFrameCallback *AXRegisterCallback(AXFrameCallback *cb);
void AXRegisterAuxACallback(AXAuxCallback *cb, void *context);
void AXGetAuxACallback(AXAuxCallback **cbOut, void **contextOut);

u16 AXGetAuxAReturnVolume(void);
u16 AXGetAuxBReturnVolume(void);
u16 AXGetAuxCReturnVolume(void);
void AXSetAuxAReturnVolume(u16 volume);
void AXSetAuxBReturnVolume(u16 volume);
void AXSetAuxCReturnVolume(u16 volume);

// AXFX

// [SPQE7T]/ISpyD.elf:.debug_info::0x36bdec
typedef void *AXFXAllocFunc(u32 /* explicitly unnamed */);

// [SPQE7T]/ISpyD.elf:.debug_info::0x36be0f
typedef void AXFXFreeFunc(void */* explicitly unnamed */);

// [SPQE7T]/ISpyD.elf:.debug_info::0x36c3ae
typedef struct AXFX_BUS
{
	s32	*left;		// size 0x04, offset 0x00
	s32	*right;		// size 0x04, offset 0x04
	s32	*surround;	// size 0x04, offset 0x08
} AXFX_BUS; // size 0x0c

// [SPQE7T]/ISpyD.elf:.debug_info::0x36bec8
typedef struct AXFX_REVERBHI_EXP
{
	f32			*earlyLine[3];				// size 0x00c, offset 0x000
	u32			earlyPos[3];				// size 0x00c, offset 0x00c
	u32			earlyLength;				// size 0x004, offset 0x018
	u32			earlyMaxLength;				// size 0x004, offset 0x01c
	f32			earlyCoef[3];				// size 0x00c, offset 0x020
	f32			*preDelayLine[3];			// size 0x00c, offset 0x02c
	u32			preDelayPos;				// size 0x004, offset 0x038
	u32			preDelayLength;				// size 0x004, offset 0x03c
	u32			preDelayMaxLength;			// size 0x004, offset 0x040
	f32			*combLine[3][3];			// size 0x024, offset 0x044
	u32			combPos[3];					// size 0x00c, offset 0x068
	u32			combLength[3];				// size 0x00c, offset 0x074
	u32			combMaxLength[3];			// size 0x00c, offset 0x080
	f32			combCoef[3];				// size 0x00c, offset 0x08c
	f32			*allpassLine[3][2];			// size 0x018, offset 0x098
	u32			allpassPos[2];				// size 0x008, offset 0x0b0
	u32			allpassLength[2];			// size 0x008, offset 0x0b8
	u32			allpassMaxLength[2];		// size 0x008, offset 0x0c0
	f32			*lastAllpassLine[3];		// size 0x00c, offset 0x0c8
	u32			lastAllpassPos[3];			// size 0x00c, offset 0x0d4
	u32			lastAllpassLength[3];		// size 0x00c, offset 0x0e0
	u32			lastAllpassMaxLength[3];	// size 0x00c, offset 0x0ec
	f32			allpassCoef;				// size 0x004, offset 0x0f8
	f32			lastLpfOut[3];				// size 0x00c, offset 0x0fc
	f32			lpfCoef;					// size 0x004, offset 0x108
	u32			active;						// size 0x004, offset 0x10c
	u32			earlyMode;					// size 0x004, offset 0x110
	f32			preDelayTimeMax;			// size 0x004, offset 0x114
	f32			preDelayTime;				// size 0x004, offset 0x118
	u32			fusedMode;					// size 0x004, offset 0x11c
	f32			fusedTime;					// size 0x004, offset 0x120
	f32			coloration;					// size 0x004, offset 0x124
	f32			damping;					// size 0x004, offset 0x128
	f32			crosstalk;					// size 0x004, offset 0x12c
	f32			earlyGain;					// size 0x004, offset 0x130
	f32			fusedGain;					// size 0x004, offset 0x134
	AXFX_BUS	*busIn;						// size 0x004, offset 0x138
	AXFX_BUS	*busOut;					// size 0x004, offset 0x13c
	f32			outGain;					// size 0x004, offset 0x140
	f32			sendGain;					// size 0x004, offset 0x144
} AXFX_REVERBHI_EXP; // size 0x148

// [SPQE7T]/ISpyD.elf:.debug_info::0x36be2c
typedef struct AXFX_REVERBHI
{
	AXFX_REVERBHI_EXP	reverbInner;	// size 0x148, offset 0x000
	f32					coloration;		// size 0x004, offset 0x148
	f32					mix;			// size 0x004, offset 0x14c
	f32					time;			// size 0x004, offset 0x150
	f32					damping;		// size 0x004, offset 0x154
	f32					preDelay;		// size 0x004, offset 0x158
	f32					crosstalk;		// size 0x004, offset 0x15c
} AXFX_REVERBHI; // size 0x160

void AXFXSetHooks(AXFXAllocFunc *alloc, AXFXFreeFunc *free);
void AXFXGetHooks(AXFXAllocFunc **allocOut, AXFXFreeFunc **freeOut);
BOOL AXFXReverbHiInit(AXFX_REVERBHI *reverbHi);
BOOL AXFXReverbHiShutdown(AXFX_REVERBHI *reverbHi);

// Available for use as the argument cb to AXRegisterAuxCallback
void AXFXReverbHiCallback(void *data, void *context);

// MTX

// GX


// IPC

typedef s32 IPCResult;

IPCResult IPCCltInit(void);

extern u32 volatile __IPCRegs[] AT_ADDRESS(0xcd000000);

inline u32 ACRReadReg(u32 reg)
{
	return __IPCRegs[reg >> 2];
}

inline void ACRWriteReg(u32 reg, u32 val)
{
	__IPCRegs[reg >> 2] = val;
}

// MEM

typedef struct MEMiHeapHead MEMiHeapHead;
typedef struct MEMAllocator MEMAllocator;

// [SPQE7T]/ISpyD.elf:.debug_info::0x113386
typedef void *MEMAllocatorAllocFunc(MEMAllocator *pAllocator, u32 size);

// [SPQE7T]/ISpyD.elf:.debug_info::0x1133bb
typedef void MEMAllocatorFreeFunc(MEMAllocator *pAllocator, void *memBlock);

typedef struct MEMLink
{
	void	*prevObject;	// size 0x04, offset 0x00
	void	*nextObject;	// size 0x04, offset 0x04
} MEMLink; // size 0x08

typedef struct MEMList
{
	void	*headObject;	// size 0x04, offset 0x00
	void	*tailObject;	// size 0x04, offset 0x04
	u16		numObjects;		// size 0x02, offset 0x08
	u16		offset;			// size 0x02, offset 0x0a
} MEMList; // size 0x0c

// TODO(DWARF)
struct MEMiHeapHead
{
	u32		signature;	// size 0x04, offset 0x00
	MEMLink	link;		// size 0x08? offset 0x04
	MEMList	childList;	// size 0x0c? offset 0x0c
	void	*heapStart;	// size 0x04, offset 0x18
	void	*heapEnd;	// size 0x04, offset 0x1c
	OSMutex	mutex;		// size 0x18, offset 0x20

	// TODO(DWARF)
	union /* explicitly untagged */
	{
		u32	val;	// size 0x04

		// TODO(DWARF)
		struct /* explicitly untagged */
		{
			u32	_reserved	: 24;	// size 24, offset  0
			u32	optFlag		: 8;	// size 8,  offset 24
		} fields; // size 0x04
	} attribute;
};

// [SPQE7T]/ISpyD.elf:.debug_info::0x113345
typedef struct MEMAllocatorFunc
{
	MEMAllocatorAllocFunc	*pfAlloc;	// size 0x04, offset 0x00
	MEMAllocatorFreeFunc	*pfFree;	// size 0x04, offset 0x04
} MEMAllocatorFunc; // size 0x08

// TODO(DWARF)
struct MEMAllocator
{
	MEMAllocatorFunc	const *pFunc;	// size 0x04, offset 0x00
	void				*pHeap;			// size 0x04, offset 0x04
	u32					heapParam1;		// size 0x04, offset 0x08
	u32					heapParam2;		// size 0x04, offset 0x0c
}; // size 0x10

void *MEMAllocFromAllocator(MEMAllocator *allocator, u32 size);
void MEMFreeToAllocator(MEMAllocator *allocator, void *block);
void MEMInitAllocatorForExpHeap(MEMAllocator *allocator, MEMiHeapHead *heap,
                                s32 align);

MEMiHeapHead *MEMCreateExpHeapEx(void *start, u32 size, u16 opt);
MEMiHeapHead *MEMDestroyExpHeap(void *heap);
static inline MEMiHeapHead *MEMCreateExpHeap(void *startAddress, u32 size)
{
	return MEMCreateExpHeapEx(startAddress, size, 0);
}

// NAND

#define FS_MAX_PATH	64

typedef s32 NANDResult;
enum NANDResult_et
{
	NAND_RESULT_OK				= 0,

	NAND_RESULT_FATAL_ERROR		= -128,
	NAND_RESULT_UNKNOWN			= -64,

	NAND_RESULT_ACCESS			= -1,
	NAND_RESULT_ALLOC_FAILED	= -2,
	NAND_RESULT_BUSY			= -3,
	NAND_RESULT_CORRUPT			= -4,
	NAND_RESULT_ECC_CRIT		= -5,
	NAND_RESULT_EXISTS			= -6,

	NAND_RESULT_INVALID			= -8,
	NAND_RESULT_MAXBLOCKS		= -9,
	NAND_RESULT_MAXFD			= -10,
	NAND_RESULT_MAXFILES		= -11,
	NAND_RESULT_NOEXISTS		= -12,
	NAND_RESULT_NOTEMPTY		= -13,
	NAND_RESULT_OPENFD			= -14,
	NAND_RESULT_AUTHENTICATION	= -15,
	NAND_RESULT_MAXDEPTH		= -16,

#define NAND_ESUCCESS	NAND_RESULT_OK
#define NAND_EFATAL		NAND_RESULT_FATAL_ERROR
};

typedef u8 NANDOpenMode;
enum NANDOpenMode_et
{
	NAND_OPEN_NONE,

	NAND_OPEN_READ,
	NAND_OPEN_WRITE,
	NAND_OPEN_RW,
};

typedef enum NANDSeekMode
{
	NAND_SEEK_SET,
	NAND_SEEK_CUR,
	NAND_SEEK_END
} NANDSeekMode;

typedef struct NANDCommandBlock NANDCommandBlock;
typedef void NANDAsyncCallback(NANDResult result, NANDCommandBlock *block);

// [SPQE7T]/ISpyD.elf:.debug_info::0x283b92
typedef struct NANDFileInfo
{
	s32		fileDescriptor;			// size 0x04, offset 0x00
	s32		origFd;					// size 0x04, offset 0x04
	char	origPath[FS_MAX_PATH];	// size 0x40, offset 0x08
	char	tmpPath[FS_MAX_PATH];	// size 0x40, offset 0x48
	u8		accType;				// size 0x01, offset 0x88
	u8		stage;					// size 0x01, offset 0x89
	u8		mark;					// size 0x01, offset 0x8a
	// 1 byte padding
} NANDFileInfo; // size 0x8c

// [SPQE7T]/ISpyD.elf:.debug_info::0x2837aa
struct NANDCommandBlock
{
	void		*userData;				// size 0x04, offset 0x00
	void		*callback;				// size 0x04, offset 0x04 // not NANDAsyncCallback *?
	void		*fileInfo;				// size 0x04, offset 0x08 // not NANDFileInfo *?
	void		*bytes;					// size 0x04, offset 0x0c
	void		*inodes;				// size 0x04, offset 0x10
	void		*status;				// size 0x04, offset 0x14
	u16			ownerId;				// size 0x02, offset 0x18
	u16			groupId;				// size 0x02, offset 0x1a
	u8			nextStage;				// size 0x01, offset 0x1c
	/* 3 bytes padding */
	u32			attr;					// size 0x04, offset 0x20
	u32			ownerAcc;				// size 0x04, offset 0x24
	u32			groupAcc;				// size 0x04, offset 0x28
	u32			othersAcc;				// size 0x04, offset 0x2c
	u32			num;					// size 0x04, offset 0x30
	char		absPath[FS_MAX_PATH];	// size 0x40, offset 0x34
	u32			*length;				// size 0x04, offset 0x74
	u32			*pos;					// size 0x04, offset 0x78
	int			state;					// size 0x04, offset 0x7c
	void		*copyBuf;				// size 0x04, offset 0x80
	u32			bufLength;				// size 0x04, offset 0x84
	u8			*type;					// size 0x04, offset 0x88
	u32			uniqNo;					// size 0x04, offset 0x8c
	u32			reqBlocks;				// size 0x04, offset 0x90
	u32			reqInodes;				// size 0x04, offset 0x94
	u32			*answer;				// size 0x04, offset 0x98
	u32			homeBlocks;				// size 0x04, offset 0x9c
	u32			homeInodes;				// size 0x04, offset 0xa0
	u32			userBlocks;				// size 0x04, offset 0xa4
	u32			userInodes;				// size 0x04, offset 0xa8
	u32			workBlocks;				// size 0x04, offset 0xac
	u32			workInodes;				// size 0x04, offset 0xb0
	char		const **dir;			// size 0x04, offset 0xb4
	int			simpleFlag;				// size 0x04, offset 0xb8
}; // size 0xbc

NANDResult NANDReadAsync(NANDFileInfo *info, void *buf, u32 len,
                         NANDAsyncCallback *callback, NANDCommandBlock *block);
NANDResult NANDWriteAsync(NANDFileInfo *info, void const *buf, u32 len,
                          NANDAsyncCallback *callback, NANDCommandBlock *block);
NANDResult NANDSeekAsync(NANDFileInfo *info, s32 offset, NANDSeekMode whence,
                         NANDAsyncCallback *callback, NANDCommandBlock *block);

NANDResult NANDInit(void);

NANDResult NANDOpenAsync(char const *path, NANDFileInfo *info, NANDOpenMode mode,
                         NANDAsyncCallback *callback, NANDCommandBlock *block);
NANDResult NANDCloseAsync(NANDFileInfo *info, NANDAsyncCallback *callback,
                          NANDCommandBlock *block);

// SC

typedef u32 SCStatus;
enum SCStatus_et
{
	SC_STATUS_OK,	/* name known from asserts */
	SC_STATUS_BUSY,
	SC_STATUS_FATAL,
	SC_STATUS_PARSE,
};

typedef void SCFlushCallback(SCStatus status);

void SCInit(void);
SCStatus SCCheckStatus(void);

void SCFlushAsync(SCFlushCallback *cb);

typedef u8 SCSoundMode;
enum SCSoundMode_et
{
	SC_SND_MONO,
	SC_SND_STEREO,
	SC_SND_SURROUND
};

typedef u8 SCSensorBarPos;
enum SCSensorBarPos_et
{
	SC_SENSOR_BAR_BOTTOM,
	SC_SENSOR_BAR_TOP,
};

#if !defined(IS_BTE) // irrelevant, and causes errors right now
typedef struct SCBtDeviceInfo // basic dev info?
{
	BD_ADDR					devAddr;	// size 0x06, offset 0x00
	struct small_dev_info	small;		// size 0x40, offset 0x06
} SCBtDeviceInfo; // size 0x46

typedef struct SCBtDeviceInfoArray
{
	u8				num;			// size 0x001, offset 0x000	/* name known from asserts */

	/* NOTE: contents of the devices member are actually
	 *
	 * SCBtDeviceInfo	registered[WUD_MAX_DEV_ENTRY_FOR_STD];
	 * SCBtDeviceInfo	active[WUD_MAX_CHANNELS];
	 *
	 * but functions in WUD act as if it's only one buffer (see
	 * __wudSecurityEventStackCallback, case BTA_DM_LINK_DOWN_EVT)
	 */
	SCBtDeviceInfo	devices[14];	// size 0x3d4, offset 0x001

	SCBtDeviceInfo	wbc;			// size 0x046, offset 0x3d5
	SCBtDeviceInfo	at_0x41b;		// size 0x046, offset 0x41b
} SCBtDeviceInfoArray; // size 0x461

typedef struct SCBtCmpDevInfo // complex dev info?
{
	BD_ADDR					devAddr;	// size 0x06, offset 0x00
	struct small_dev_info	small;		// size 0x40, offset 0x06
	LINK_KEY				linkKey;	// size 0x10, offset 0x30 // ? or just a buffer
} SCBtCmpDevInfo; // size 0x56

typedef struct SCBtCmpDevInfoArray
{
	u8				num;			// size 0x001, offset 0x000 /* name known from asserts */
	SCBtCmpDevInfo	devices[6];		// size 0x204, offset 0x001
} SCBtCmpDevInfoArray; // size 0x205

SCSoundMode SCGetSoundMode(void);
void SCGetBtDeviceInfoArray(SCBtDeviceInfoArray *array);
BOOL SCSetBtDeviceInfoArray(SCBtDeviceInfoArray const *array);
void SCGetBtCmpDevInfoArray(SCBtCmpDevInfoArray *array);
BOOL SCSetBtCmpDevInfoArray(SCBtCmpDevInfoArray const *array);
#endif
u32 SCGetBtDpdSensibility(void);
BOOL SCSetBtDpdSensibility(u8 sens);
u8 SCGetWpadMotorMode(void);
BOOL SCSetWpadMotorMode(u8 mode);
SCSensorBarPos SCGetWpadSensorBarPosition(void);
u8 SCGetWpadSpeakerVolume(void);
BOOL SCSetWpadSpeakerVolume(u8 vol);

typedef s8 SCProductGameRegion;
enum SCProductGameRegion_et
{
	SC_PRD_GAME_REG_JP,
	SC_PRD_GAME_REG_US,
	SC_PRD_GAME_REG_EU,
	SC_PRD_GAME_REG_DEBUG,

	SC_PRD_GAME_REG_KR,
	SC_PRD_GAME_REG_CN,

	SC_PRD_GAME_REG_MAX,
	SC_PRD_GAME_REG_NULL	= -1
};

SCProductGameRegion SCGetProductGameRegion(void);

// USB

typedef void (*USBCallback)(IPCResult result, void *arg);

IPCResult IUSB_OpenLib(void);
IPCResult IUSB_CloseLib(void);
IPCResult IUSB_OpenDeviceIds(char const *interface, u16 vid, u16 pid,
                             IPCResult *resultOut);
IPCResult IUSB_CloseDeviceAsync(s32 fd, USBCallback callback,
                                void *callbackArg);
IPCResult IUSB_ReadIntrMsgAsync(s32 fd, u32 endpoint, u32 length, void *buffer,
                                USBCallback callback, void *callbackArg);
IPCResult IUSB_ReadBlkMsgAsync(s32 fd, u32 endpoint, u32 length, void *buffer,
                               USBCallback callback, void *callbackArg);
IPCResult IUSB_WriteBlkMsgAsync(s32 fd, u32 endpoint, u32 length,
                                void const *buffer, USBCallback callback,
                                void *callbackArg);
IPCResult IUSB_WriteCtrlMsgAsync(s32 fd, u8 requestType, u8 request, u16 value,
                                 u16 index, u16 length, void *buffer,
                                 USBCallback callback, void *callbackArg);

// VI

void __VIResetRFIdle(void);
void VISetBlack(BOOL black);
void VIFlush(void);

#ifdef __cplusplus
	}
#endif

#endif // CONTEXT_RVL_H
