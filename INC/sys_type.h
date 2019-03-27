/*
H********************************************************************************************************
*                                      Weight System Firmware
*
*                    (c) Copyright 2010-2014 ACEA Biosciences, Inc.
*                    All rights reserved.  Protected by international copyright laws.
*
* File      :  sys_type.h
* By        :  AlexShi
* Email     :  shiweining123@gmail.com
* Version   :  V1.00.00
* Compiler  :  IAR Embedded Workbench for ARM V7.x
*********************************************************************************************************
* Note(s)   :
*
*
*
*********************************************************************************************************
*/

#ifndef  SYSONCE_INFRA_SYS_TYPE_H
#define  SYSONCE_INFRA_SYS_TYPE_H

//#include <includes.h>

/************************************** BASE DATA TYPE DEFINITION **************************************/
// typedef                      void           VOID;       /* void                                       */
// typedef                      char           CHAR8;      /*                                            */
// typedef             unsigned char           UCHAR8;     /*                                            */

// typedef             unsigned char           BYTE;       /*                                            */
// typedef             unsigned short  int     HWORD;      /*                                            */
// typedef             unsigned long           WORD;       /*                                            */
// typedef             unsigned long   long    DWORD;      /*                                            */

// typedef               signed char           CHAR;       /*                                            */
// typedef               signed short  int     SHORT;      /*                                            */
// typedef               signed long           LONG;       /*                                            */
// typedef               signed long   long    LONGLONG;   /*                                            */

// typedef             unsigned int            UINT;       /*                                            */
// typedef             unsigned char           UINT8;      /*                                            */
// typedef             unsigned short  int     UINT16;     /*                                            */
// typedef             unsigned long   int     UINT32;     /*                                            */
// typedef             unsigned long   long    UINT64;     /*                                            */

// typedef               signed int            INT;        /*                                            */
// typedef               signed char           INT8;       /*                                            */
// typedef               signed short  int     INT16;      /*                                            */
// typedef               signed long   int     INT32;      /*                                            */
// typedef               signed long   long    INT64;      /*                                            */
/************************************** 8-bit DATA TYPE DEFINITION *************************************/
// typedef             unsigned char           INT8U;      /*          Unsigned  8 bit Quantity          */
// typedef               signed char           INT8S;      /*            Signed  8 bit Quantity          */
// typedef             unsigned char   const   INT8UC;     /*          Unsigned  8 bit Constant          */
// typedef               signed char   const   INT8SC;     /*            Signed  8 bit Constant          */
// typedef volatile    unsigned char           VINT8U;     /* Volatile Unsigned  8 bit Quantity          */
// typedef volatile      signed char           VINT8S;     /* Volatile   Signed  8 bit Quantity          */
// typedef volatile    unsigned char   const   VINT8UC;    /* Volatile Unsigned  8 bit Constant          */
// typedef volatile      signed char   const   VINT8SC;    /* Volatile   Signed  8 bit Constant          */
/************************************* 16-bit DATA TYPE DEFINITION *************************************/
// typedef             unsigned short          INT16U;     /*          Unsigned 16 bit Quantity          */
// typedef               signed short          INT16S;     /*            Signed 16 bit Quantity          */
// typedef             unsigned short  const   INT16UC;    /*          Unsigned 16 bit Constant          */
// typedef               signed short  const   INT16SC;    /*            Signed 16 bit Constant          */
// typedef volatile    unsigned short          VINT16U;    /* Volatile Unsigned 16 bit Quantity          */
// typedef volatile      signed short          VINT16S;    /* Volatile   Signed 16 bit Quantity          */
// typedef volatile    unsigned short  const   VINT16UC;   /* Volatile Unsigned 16 bit Constant          */
// typedef volatile      signed short  const   VINT16SC;   /* Volatile   Signed 16 bit Constant          */
/************************************* 32-bit DATA TYPE DEFINITION *************************************/
// typedef             unsigned long           INT32U;     /*          Unsigned 32 bit Quantity          */
// typedef               signed long           INT32S;     /*            Signed 32 bit Quantity          */
// typedef             unsigned long   const   INT32UC;    /*          Unsigned 32 bit Constant          */
// typedef               signed long   const   INT32SC;    /*            Signed 32 bit Constant          */
// typedef volatile    unsigned long           VINT32U;    /* Volatile Unsigned 32 bit Quantity          */
// typedef volatile      signed long           VINT32S;    /* Volatile   Signed 32 bit Quantity          */
// typedef volatile    unsigned long   const   VINT32UC;   /* Volatile Unsigned 32 bit Constant          */
// typedef volatile      signed long   const   VINT32SC;   /* Volatile   Signed 32 bit Constant          */
/************************************* 64-bit DATA TYPE DEFINITION *************************************/
typedef          unsigned long long         INT64U;     /*          Unsigned 64 bit Quantity          */
typedef            signed long long         INT64S;     /*            Signed 64 bit Quantity          */
typedef          unsigned long long const   INT64UC;    /*          Unsigned 64 bit Constant          */
typedef            signed long long const   INT64SC;    /*            Signed 64 bit Constant          */
typedef volatile unsigned long long         VINT64U;    /* Volatile Unsigned 64 bit Quantity          */
typedef volatile   signed long long         VINT64S;    /* Volatile   Signed 64 bit Quantity          */
typedef volatile unsigned long long const   VINT64UC;   /* Volatile Unsigned 64 bit Constant          */
typedef volatile   signed long long const   VINT64SC;   /* Volatile   Signed 64 bit Constant          */
/************************************* Float DATA TYPE DEFINITION **************************************/
// typedef float                               FP32;       /* Single precision floating point            */
// typedef double                              FP64;       /* Double precision floating point            */
/*******************************************************************************************************/
// typedef            void        CPU_VOID;
// typedef            char        CPU_CHAR;                /*  8-bit character                           */
// typedef  unsigned  char        CPU_BOOLEAN;             /*  8-bit boolean or logical                  */
// typedef  unsigned  char        CPU_INT08U;              /*  8-bit unsigned integer                    */
// typedef    signed  char        CPU_INT08S;              /*  8-bit   signed integer                    */
// typedef  unsigned  short       CPU_INT16U;              /* 16-bit unsigned integer                    */
// typedef    signed  short       CPU_INT16S;              /* 16-bit   signed integer                    */
// typedef  unsigned  int         CPU_INT32U;              /* 32-bit unsigned integer                    */
// typedef    signed  int         CPU_INT32S;              /* 32-bit   signed integer                    */
// typedef  unsigned  long  long  CPU_INT64U;              /* 64-bit unsigned integer                    */
// typedef    signed  long  long  CPU_INT64S;              /* 64-bit   signed integer                    */
// typedef            float       CPU_FP32;                /* 32-bit floating point                      */
// typedef            double      CPU_FP64;                /* 64-bit floating point                      */
// typedef  volatile  CPU_INT08U  CPU_REG08;               /*  8-bit register                            */
// typedef  volatile  CPU_INT16U  CPU_REG16;               /* 16-bit register                            */
// typedef  volatile  CPU_INT32U  CPU_REG32;               /* 32-bit register                            */
typedef  volatile  CPU_INT64U  CPU_REG64;               /* 64-bit register                            */
// typedef            void      (*CPU_FNCT_VOID)(void  );  /* See Note #2a.                              */
// typedef            void      (*CPU_FNCT_PTR )(void *);  /* See Note #2b.                              */
/*******************************************************************************************************/
typedef         signed  char    cpu_int8s_t;
typedef         signed short    cpu_int16s_t;
typedef         signed  long    cpu_int32s_t;
typedef     signed long long    cpu_int64s_t;
typedef       unsigned  char    cpu_bool_t;
typedef       unsigned  char    cpu_int8u_t;
typedef       unsigned short    cpu_int16u_t;
typedef       unsigned  long    cpu_int32u_t;
typedef   unsigned long long    cpu_int64u_t;
typedef                float    cpu_fp32_t;
typedef               double    cpu_fp64_t;

typedef          signed long    cpu_base_t;
typedef        unsigned long    cpu_ubase_t;
typedef           cpu_base_t    cpu_err_t;
typedef           cpu_base_t    cpu_flag_t;
typedef           cpu_base_t    cpu_off_t;
typedef          cpu_ubase_t    cpu_tick_t;
typedef          cpu_ubase_t    cpu_size_t;

typedef       unsigned  char    bool_t;
typedef         signed  char    int8s_t;    /*            Signed  8 bit Quantity          */
typedef       unsigned  char    int8u_t;    /*          Unsigned  8 bit Quantity          */
typedef         signed short    int16s_t;   /*            Signed 16 bit Quantity          */
typedef       unsigned short    int16u_t;   /*          Unsigned 16 bit Quantity          */
typedef         signed  long    int32s_t;   /*            Signed 32 bit Quantity          */
typedef       unsigned  long    int32u_t;   /*          Unsigned 32 bit Quantity          */
typedef    signed long  long    int64s_t;   /*            Signed 64 bit Quantity          */
typedef  unsigned long  long    int64u_t;   /*          Unsigned 64 bit Quantity          */
typedef                float    float32_t;  /* Single precision floating point            */
typedef               double    float64_t;  /* Double precision floating point            */

typedef enum bool {
    FALSE   = 0,    TRUE    = !FALSE,
    NO      = 0,    YES     = !NO,
    Disable = 0,    Enable  = !Disable,
    STOP    = 0,    RUN     = !STOP,
    ACTIVE  = 0,    INACTIVE= !ACTIVE,
    INVALID = 0,    VALID   = !INACTIVE
} BOOL;

typedef union {
    INT8S   Sbyte;
    INT8U   Ubyte;
    struct {
        INT8U   b0  :1;
        INT8U   b1  :1;
        INT8U   b2  :1;
        INT8U   b3  :1;
        INT8U   b4  :1;
        INT8U   b5  :1;
        INT8U   b6  :1;
        INT8U   b7  :1;
    } Bits;
} UNION8;

typedef union {
    INT16S  Shw;
    INT16U  Uhw;
    INT8S   Sbyte[2];
    INT8U   Ubyte[2];
    struct {
        INT8U   b0  :1;
        INT8U   b1  :1;
        INT8U   b2  :1;
        INT8U   b3  :1;
        INT8U   b4  :1;
        INT8U   b5  :1;
        INT8U   b6  :1;
        INT8U   b7  :1;
        INT8U   b8  :1;
        INT8U   b9  :1;
        INT8U   b10 :1;
        INT8U   b11 :1;
        INT8U   b12 :1;
        INT8U   b13 :1;
        INT8U   b14 :1;
        INT8U   b15 :1;
    } Bits;
} UNION16;

typedef union {
    INT32S  Sword;
    INT32U  Uword;
    INT16S    Shw[2];
    INT16U    Uhw[2];
    INT8S   Sbyte[4];
    INT8U   Ubyte[4];
    FP32    fp32;
    struct {
        INT8U   b0  :1;
        INT8U   b1  :1;
        INT8U   b2  :1;
        INT8U   b3  :1;
        INT8U   b4  :1;
        INT8U   b5  :1;
        INT8U   b6  :1;
        INT8U   b7  :1;
        INT8U   b8  :1;
        INT8U   b9  :1;
        INT8U   b10 :1;
        INT8U   b11 :1;
        INT8U   b12 :1;
        INT8U   b13 :1;
        INT8U   b14 :1;
        INT8U   b15 :1;
        INT8U   b16 :1;
        INT8U   b17 :1;
        INT8U   b18 :1;
        INT8U   b19 :1;
        INT8U   b20 :1;
        INT8U   b21 :1;
        INT8U   b22 :1;
        INT8U   b23 :1;
        INT8U   b24 :1;
        INT8U   b25 :1;
        INT8U   b26 :1;
        INT8U   b27 :1;
        INT8U   b28 :1;
        INT8U   b29 :1;
        INT8U   b30 :1;
        INT8U   b31 :1;
    } Bits;
} UNION32;

typedef union {
    INT64S  Sdword;
    INT64U  Udword;
    INT32S  Sword[2];
    INT32U  Uword[2];
    INT16S    Shw[4];
    INT16U    Uhw[4];
    INT8S   Sbyte[8];
    INT8U   Ubyte[8];
    FP64    fp64;
    struct {
        INT8U   b0  :1;
        INT8U   b1  :1;
        INT8U   b2  :1;
        INT8U   b3  :1;
        INT8U   b4  :1;
        INT8U   b5  :1;
        INT8U   b6  :1;
        INT8U   b7  :1;
        INT8U   b8  :1;
        INT8U   b9  :1;
        INT8U   b10 :1;
        INT8U   b11 :1;
        INT8U   b12 :1;
        INT8U   b13 :1;
        INT8U   b14 :1;
        INT8U   b15 :1;
        INT8U   b16 :1;
        INT8U   b17 :1;
        INT8U   b18 :1;
        INT8U   b19 :1;
        INT8U   b20 :1;
        INT8U   b21 :1;
        INT8U   b22 :1;
        INT8U   b23 :1;
        INT8U   b24 :1;
        INT8U   b25 :1;
        INT8U   b26 :1;
        INT8U   b27 :1;
        INT8U   b28 :1;
        INT8U   b29 :1;
        INT8U   b30 :1;
        INT8U   b31 :1;
        INT8U   b32 :1;
        INT8U   b33 :1;
        INT8U   b34 :1;
        INT8U   b35 :1;
        INT8U   b36 :1;
        INT8U   b37 :1;
        INT8U   b38 :1;
        INT8U   b39 :1;
        INT8U   b40 :1;
        INT8U   b41 :1;
        INT8U   b42 :1;
        INT8U   b43 :1;
        INT8U   b44 :1;
        INT8U   b45 :1;
        INT8U   b46 :1;
        INT8U   b47 :1;
        INT8U   b48 :1;
        INT8U   b49 :1;
        INT8U   b50 :1;
        INT8U   b51 :1;
        INT8U   b52 :1;
        INT8U   b53 :1;
        INT8U   b54 :1;
        INT8U   b55 :1;
        INT8U   b56 :1;
        INT8U   b57 :1;
        INT8U   b58 :1;
        INT8U   b59 :1;
        INT8U   b60 :1;
        INT8U   b61 :1;
        INT8U   b62 :1;
        INT8U   b63 :1;
    } Bits;
} UNION64;

typedef union {
                INT64S          *SdwordPtr;
                INT64U          *UdwordPtr;
                INT32S          *SwordPtr;
                INT32U          *UwordPtr;
                INT16S          *ShwPtr;
                INT16U          *UhwPtr;
                INT8S           *SbytePtr;
                INT8U           *UbytePtr;
} UnionPtr;

typedef union {
    volatile    INT64S          *SdwordPtr;
    volatile    INT64U          *UdwordPtr;
    volatile    INT32S          *SwordPtr;
    volatile    INT32U          *UwordPtr;
    volatile    INT16S          *ShwPtr;
    volatile    INT16U          *UhwPtr;
    volatile    INT8S           *SbytePtr;
    volatile    INT8U           *UbytePtr;
} UnionVPtr;

typedef union {
                INT64S  const   *SdwordPtr;
                INT64U  const   *UdwordPtr;
                INT32S  const   *SwordPtr;
                INT32U  const   *UwordPtr;
                INT16S  const   *ShwPtr;
                INT16U  const   *UhwPtr;
                INT8S   const   *SbytePtr;
                INT8U   const   *UbytePtr;
} UnionCPtr;

typedef union {
    volatile    INT64S  const   *SdwordPtr;
    volatile    INT64U  const   *UdwordPtr;
    volatile    INT32S  const   *SwordPtr;
    volatile    INT32U  const   *UwordPtr;
    volatile    INT16S  const   *ShwPtr;
    volatile    INT16U  const   *UhwPtr;
    volatile    INT8S   const   *SbytePtr;
    volatile    INT8U   const   *UbytePtr;
} UnionCVPtr;

typedef struct {
                INT64S          *SdwordPtr;
                INT64U          *UdwordPtr;
                INT32S          *SwordPtr;
                INT32U          *UwordPtr;
                INT16S          *ShwPtr;
                INT16U          *UhwPtr;
                INT8S           *SbytePtr;
                INT8U           *UbytePtr;
} StructPtr;

typedef struct {
    volatile    INT64S          *SdwordPtr;
    volatile    INT64U          *UdwordPtr;
    volatile    INT32S          *SwordPtr;
    volatile    INT32U          *UwordPtr;
    volatile    INT16S          *ShwPtr;
    volatile    INT16U          *UhwPtr;
    volatile    INT8S           *SbytePtr;
    volatile    INT8U           *UbytePtr;
} StructVPtr;

typedef struct {
                INT64S  const   *SdwordPtr;
                INT64U  const   *UdwordPtr;
                INT32S  const   *SwordPtr;
                INT32U  const   *UwordPtr;
                INT16S  const   *ShwPtr;
                INT16U  const   *UhwPtr;
                INT8S   const   *SbytePtr;
                INT8U   const   *UbytePtr;
} StructCPtr;

typedef struct {
    volatile    INT64S  const   *SdwordPtr;
    volatile    INT64U  const   *UdwordPtr;
    volatile    INT32S  const   *SwordPtr;
    volatile    INT32U  const   *UwordPtr;
    volatile    INT16S  const   *ShwPtr;
    volatile    INT16U  const   *UhwPtr;
    volatile    INT8S   const   *SbytePtr;
    volatile    INT8U   const   *UbytePtr;
} StructCVPtr;

#endif  /* SYSONCE_INFRA_SYS_TYPE_H */


/*******************************************************************************************************/
/*************************************** M A C R O-D E C L A R E ***************************************/
/*******************************************************************************************************/
#ifndef  SYSONCE_INFRA_SYS_MACRO_H
#define  SYSONCE_INFRA_SYS_MACRO_H

/*
*********************************************************************************************************
*                                             Macros Version
*********************************************************************************************************
*/

#define  MACROS_VERSION                                    100u


/*
*********************************************************************************************************
*                                          STANDARD DEFINES
*********************************************************************************************************
*/
/*
#define  DEF_DISABLED                                      0u
#define  DEF_ENABLED                                       1u

#define  DEF_FALSE                                         0u
#define  DEF_TRUE                                          1u

#define  DEF_NO                                            0u
#define  DEF_YES                                           1u

#define  DEF_OFF                                           0u
#define  DEF_ON                                            1u

#define  DEF_CLR                                           0u
#define  DEF_SET                                           1u

#define  DEF_LOW                                           0u
#define  DEF_HIGH                                          1u

#define  DEF_ACTIVE                                        0u
#define  DEF_INACTIVE                                      1u

#define  DEF_FAIL                                          0u
#define  DEF_OK                                            1u
*/

#define  DEF_DOWN                                          0u
#define  DEF_UP                                            1u

#define  DEF_UNLOCK                                        0u
#define  DEF_LOCK                                          1u

#define  DEF_STOP                                          0u
#define  DEF_RUN                                           1u

#define  DEF_IN                                            0u
#define  DEF_OUT                                           1u

// ------------------------------------------------------------------------------------------------------
// Allow creation of procedure-like macros that are a single statement,
// and must be followed by a semi-colon

#define DBG_MACRO_START do {
#define DBG_MACRO_END   } while (0)

#define DBG_EMPTY_STATEMENT DBG_MACRO_START DBG_MACRO_END

#define DBG_UNUSED_PARAM( _type_, _name_ )  \
DBG_MACRO_START                             \
    _type_ __tmp1 = (_name_);               \
    _type_ __tmp2 = __tmp1;                 \
    __tmp1 = __tmp2;                        \
DBG_MACRO_END


/*
*********************************************************************************************************
*                                            Bit Mask Macros
*********************************************************************************************************
*/

#define  DEF_BIT_NONE_MASK                          0x00000000uL        // 0
#define  DEF_BIT_ALL_MASK                           0xFFFFFFFFuL        //
#define  DEF_BIT00_MASK                             0x00000001uL        // 1 << 0
#define  DEF_BIT01_MASK                             0x00000002uL        // 1 << 1
#define  DEF_BIT02_MASK                             0x00000004uL        // 1 << 2
#define  DEF_BIT03_MASK                             0x00000008uL        // 1 << 3
#define  DEF_BIT04_MASK                             0x00000010uL        // 1 << 4
#define  DEF_BIT05_MASK                             0x00000020uL        // 1 << 5
#define  DEF_BIT06_MASK                             0x00000040uL        // 1 << 6
#define  DEF_BIT07_MASK                             0x00000080uL        // 1 << 7
#define  DEF_BIT08_MASK                             0x00000100uL        // 1 << 8
#define  DEF_BIT09_MASK                             0x00000200uL        // 1 << 9
#define  DEF_BIT10_MASK                             0x00000400uL        // 1 << 10
#define  DEF_BIT11_MASK                             0x00000800uL        // 1 << 11
#define  DEF_BIT12_MASK                             0x00001000uL        // 1 << 12
#define  DEF_BIT13_MASK                             0x00002000uL        // 1 << 13
#define  DEF_BIT14_MASK                             0x00004000uL        // 1 << 14
#define  DEF_BIT15_MASK                             0x00008000uL        // 1 << 15
#define  DEF_BIT16_MASK                             0x00010000uL        // 1 << 16
#define  DEF_BIT17_MASK                             0x00020000uL        // 1 << 17
#define  DEF_BIT18_MASK                             0x00040000uL        // 1 << 18
#define  DEF_BIT19_MASK                             0x00080000uL        // 1 << 19
#define  DEF_BIT20_MASK                             0x00100000uL        // 1 << 20
#define  DEF_BIT21_MASK                             0x00200000uL        // 1 << 21
#define  DEF_BIT22_MASK                             0x00400000uL        // 1 << 22
#define  DEF_BIT23_MASK                             0x00800000uL        // 1 << 23
#define  DEF_BIT24_MASK                             0x01000000uL        // 1 << 24
#define  DEF_BIT25_MASK                             0x02000000uL        // 1 << 25
#define  DEF_BIT26_MASK                             0x04000000uL        // 1 << 26
#define  DEF_BIT27_MASK                             0x08000000uL        // 1 << 27
#define  DEF_BIT28_MASK                             0x10000000uL        // 1 << 28
#define  DEF_BIT29_MASK                             0x20000000uL        // 1 << 29
#define  DEF_BIT30_MASK                             0x40000000uL        // 1 << 30
#define  DEF_BIT31_MASK                             0x80000000uL        // 1 << 31


/*
*********************************************************************************************************
*                                               Size Macros
*********************************************************************************************************
*/

#define  DEF_SIZE_1K                                0x00000400          // Size:  1K
#define  DEF_SIZE_2K                                0x00000800          // Size:  2K
#define  DEF_SIZE_4K                                0x00001000          // Size:  4K
#define  DEF_SIZE_8K                                0x00002000          // Size:  8K
#define  DEF_SIZE_16K                               0x00004000          // Size: 16K
#define  DEF_SIZE_32K                               0x00008000          // Size: 32K
#define  DEF_SIZE_64K                               0x00010000          // Size: 64K
#define  DEF_SIZE_128K                              0x00020000          // Size:128K
#define  DEF_SIZE_256K                              0x00040000          // Size:256K
#define  DEF_SIZE_512K                              0x00080000          // Size:512K
#define  DEF_SIZE_1M                                0x00100000          // Size:  1M
#define  DEF_SIZE_2M                                0x00200000          // Size:  2M
#define  DEF_SIZE_4M                                0x00400000          // Size:  4M
#define  DEF_SIZE_8M                                0x00800000          // Size:  8M
#define  DEF_SIZE_16M                               0x01000000          // Size: 16M
#define  DEF_SIZE_32M                               0x02000000          // Size: 32M
#define  DEF_SIZE_64M                               0x04000000          // Size: 64M
#define  DEF_SIZE_128M                              0x08000000          // Size:128M
#define  DEF_SIZE_256M                              0x10000000          // Size:256M
#define  DEF_SIZE_512M                              0x20000000          // Size:512M
#define  DEF_SIZE_1G                                0x40000000          // Size:  1G


/*
*********************************************************************************************************
*                                             Macros Constant
*********************************************************************************************************
*/

#define DEF_FLOAT_DELTA                             0.000001
#define DEF_PI                                      3.1415926535897932384626433832795f
#define DEF_2PI                                     6.283185307179586476925286766559f
#define DEF_RADIAN2DEGREE                          (180.0f/DEF_PI)
#define DEF_DEGREE2RADIAN                          (DEF_PI/180.0f)

#define SECTION(x)                      @ x
#define UNUSED
#define USED
#define ALIGN(n)                        PRAGMA(data_alignment=n)
#define __INLINE__                      inline

#ifndef NULL
#define NULL                    ((void *)0)
#endif

#define	PUBLIC
#define PROTECTED
#define PRIVATE                 static

/* --------------------------------------------------------------------------------------------------- */
/* 得到指定地址上的一个字节 */
#define MEM_B(addr)             (*((INT8U *)(addr)))
/* 得到指定地址上的一个字 */
#define MEM_W(addr)             (*((INT16U *)(addr)))
/* --------------------------------------------------------------------------------------------------- */
/* 得到一个变量的地址(byte宽度)    */
#define BYTEPTR(var)            ((INT8U *)  (void *)&(var))
/* 得到一个变量的地址(hw宽度)      */
#define HWPTR(var)              ((INT16U *) (void *)&(var))
/* 得到一个变量的地址(word宽度)    */
#define WORDPTR(var)            ((INT32U *) (void *)&(var))
/* 得到一个变量的地址(dword宽度)   */
#define DWORDPTR(var)           ((INT64U *) (void *)&(var))
/* --------------------------------------------------------------------------------------------------- */
/* 按照LSB格式把两个字节转化为一个Word */
#define FLIPW(ray)              ((((INT16U)(ray)[0]) << 8) + (ray)[1])
/* 按照LSB格式把一个Word转化为两个字节 */
#define FLOPW(ray, val)         \
do {                            \
    (ray)[0] = ((val) / 256);   \
    (ray)[1] = ((val) & 0xFF);  \
} while(0)
/* --------------------------------------------------------------------------------------------------- */
/* 将一半字拆分成2字节 */
#define MSB(var)                (((INT8U *)&(var))[1])
#define LSB(var)                (((INT8U *)&(var))[0])
/* 将一字拆分成2半字 */
#define MSW(var)                (((INT16U *)&(var))[1])
#define LSW(var)                (((INT16U *)&(var))[0])
/* 将一字拆分成4字节 */
#define MSB0W(var)              (((INT8U *)&(var))[3])     /* Most  significant byte of 1st rank of \a var. */
#define MSB1W(var)              (((INT8U *)&(var))[2])     /* Most  significant byte of 2nd rank of \a var. */
#define MSB2W(var)              (((INT8U *)&(var))[1])     /* Most  significant byte of 3rd rank of \a var. */
#define MSB3W(var)              (((INT8U *)&(var))[0])     /* Most  significant byte of 4th rank of \a var. */
#define LSB3W(var)              MSB0W(var)                 /* Least significant byte of 4th rank of \a var. */
#define LSB2W(var)              MSB1W(var)                 /* Least significant byte of 3rd rank of \a var. */
#define LSB1W(var)              MSB2W(var)                 /* Least significant byte of 2nd rank of \a var. */
#define LSB0W(var)              MSB3W(var)                 /* Least significant byte of 1st rank of \a var. */
#define MSB0(var)               (((INT8U *)&(var))[3])     /* Most  significant byte of 1st rank of \a var. */
#define MSB1(var)               (((INT8U *)&(var))[2])     /* Most  significant byte of 2nd rank of \a var. */
#define MSB2(var)               (((INT8U *)&(var))[1])     /* Most  significant byte of 3rd rank of \a var. */
#define MSB3(var)               (((INT8U *)&(var))[0])     /* Most  significant byte of 4th rank of \a var. */
#define LSB0(var)               MSB3(var)                  /* Least significant byte of 4th rank of \a var. */
#define LSB1(var)               MSB2(var)                  /* Least significant byte of 3rd rank of \a var. */
#define LSB2(var)               MSB1(var)                  /* Least significant byte of 2nd rank of \a var. */
#define LSB3(var)               MSB0(var)                  /* Least significant byte of 1st rank of \a var. */
/* --------------------------------------------------------------------------------------------------- */
#define Swap8(var)              ((INT8U) (((INT8U) (var) >> 4) | ((INT8U) (var) << 4)))
#define Swap16(var)             ((INT16U)(((INT16U)(var) >> 8) | ((INT16U)(var) << 8)))
#define Swap32(var)             ((INT32U)(((INT32U)Swap16((INT32U)(var) >> 16)) | ((INT32U)Swap16((INT32U)(var)) << 16)))
#define Swap64(var)             ((INT64U)(((INT64U)Swap32((INT64U)(var) >> 32)) | ((INT64U)Swap32((INT64U)(var)) << 32)))
#define swap8(var)              Swap8(var)
#define swap16(var)             Swap16(var)
#define swap32(var)             Swap32(var)
#define swap64(var)             Swap64(var)
/* --------------------------------------------------------------------------------------------------- */
/* \brief Reads the bits of a value specified by a given bit-mask.
 *
 * \param value Value to read bits from.
 * \param mask  Bit-mask indicating bits to read.
 *
 * \return Read bits.
 */
#define RdBits(value, mask)             ((value) & (mask))
/* \brief Writes the bits of a C lvalue specified by a given bit-mask.
 *
 * \param lvalue  C lvalue to write bits to.
 * \param mask    Bit-mask indicating bits to write.
 * \param bits    Bits to write.
 *
 * \return Resulting value with written bits.
 */
#define WrBits(lvalue, mask, bits)      ((lvalue) = ((lvalue) & ~(mask)) | ((bits) & (mask)))
/* \brief Tests the bits of a value specified by a given bit-mask.
 *
 * \param value Value of which to test bits.
 * \param mask  Bit-mask indicating bits to test.
 *
 * \return \c 1 if at least one of the tested bits is set, else \c 0.
 */
#define TestBits(value, mask)           (Rd_bits(value, mask) != 0)
/* \brief Clears the bits of a C lvalue specified by a given bit-mask.
 *
 * \param lvalue  C lvalue of which to clear bits.
 * \param mask    Bit-mask indicating bits to clear.
 *
 * \return Resulting value with cleared bits.
 */
#define ClrBits(lvalue, mask)           ((lvalue) &= ~(mask))
/* \brief Sets the bits of a C lvalue specified by a given bit-mask.
 *
 * \param lvalue  C lvalue of which to set bits.
 * \param mask    Bit-mask indicating bits to set.
 *
 * \return Resulting value with set bits.
 */
#define SetBits(lvalue, mask)           ((lvalue) |= (mask))
/* \brief Toggles the bits of a C lvalue specified by a given bit-mask.
 *
 * \param lvalue  C lvalue of which to toggle bits.
 * \param mask    Bit-mask indicating bits to toggle.
 *
 * \return Resulting value with toggled bits.
 */
#define TglBits(lvalue, mask)           ((lvalue) ^= (mask))
/* --------------------------------------------------------------------------------------------------- */
/* 将一个字母转换为大写 */    /* ' ' = 0x20 */
#define DEF_UPPER_CASE(ch)             (((ch) >= 'a' && (ch) <= 'z') ? ((ch) - ' ') : (ch))
/* 将一个字母转换为小写 */    /* ' ' = 0x20 */
#define DEF_LOWER_CASE(ch)             (((ch) >= 'A' && (ch) <= 'Z') ? ((ch) + ' ') : (ch))
/* --------------------------------------------------------------------------------------------------- */
/* 判断数字是否是2的幂 */
#define IS2POWER(num)                  (((num)>0)?(((num)&((num)-1))?DEF_FALSE:DEF_TRUE):DEF_FALSE)
/* 判断字符是不是10进值的数字 */
#define IS_DEC_NUMBER(ch)               ((ch) >= '0' && (ch) <= '9')
/* 判断字符是不是16进值的数字 */
#define IS_HEX_NUMBER(ch)              (((ch) >= '0' && (ch) <= '9') || \
                                        ((ch) >= 'A' && (ch) <= 'F') || \
                                        ((ch) >= 'a' && (ch) <= 'f'))
/* --------------------------------------------------------------------------------------------------- */
/* 防止溢出的一个方法 */
#define INC_SAT(val)                    (val = ((val)+1 > (val)) ? (val) : (val)+1)
/* 返回一个比X大的最接近的8的倍数的数 */
#define RND8(x)                         ((((x) + 7) / 8 ) * 8 )
/* --------------------------------------------------------------------------------------------------- */
/* 得到一个field在结构体(struct)中的偏移量 */
#define FPOS(type, field)               ((INT32U) &((type *) 0)->field)
/* 得到一个结构体中field所占用的字节数 */
#define FSIZ(type, field)               sizeof(((type *) 0)->field)
/* --------------------------------------------------------------------------------------------------- */
/* 返回数组元素的个数 */
#define ARR_SIZE(arr)                   (sizeof(arr)/sizeof((arr)[0]))
#define lengthof(array)                 (sizeof(array) / sizeof(*(array)))
#define endof(array)                    (&(array)[lengthof(array) - 1])
/* --------------------------------------------------------------------------------------------------- */
/* 返回一个无符号数n尾的值MOD_BY_POWER_OF_TWO(X,n)=X%(2^n) */
#define MOD_BY_POWER_OF_TWO(val, mod_by)    ((INT32U)(val) & (INT32U)((mod_by)-1))
/* --------------------------------------------------------------------------------------------------- */
/* 对于IO空间映射在存储空间的结构,输入输出处理 */
#define DEF_INPUT_BYTE(port)            (*((CPU_REG08   *)(port)))
#define DEF_INPUT_HW(port)              (*((CPU_REG16   *)(port)))
#define DEF_INPUT_WORD(port)            (*((CPU_REG32   *)(port)))
#define DEF_OUTPUT_BYTE(port, val)      (*((CPU_REG08   *)(port))   = ((INT8U)(val)))
#define DEF_OUTPUT_HW(port, val)        (*((CPU_REG16   *)(port))   = ((INT16U)(val)))
#define DEF_OUTPUT_WORD(port, val)      (*((CPU_REG32   *)(port))   = ((INT32U)(val)))
/* --------------------------------------------------------------------------------------------------- */
/* 使用#把宏参数变为一个字符串,用##把两个宏参数贴合在一起. */
#define _STR(s)                         #s
#define STR(s)                         _STR(s)
#define _CONS(a,b)                      int(a##e##b)
#define CONS(a,b)                      _CONS(a, b)
#define CONCAT_(a,b)                    a##b
#define CONCAT(a,b)                     CONCAT_(a,b)
#define STRINGIFY_(s)                   #s
#define STRINGIFY(s)                    STRINGIFY_(s)

/* 填充结构 */
#define FILL(x)                         {x, #x}
/* 得到一个数值类型所对应的字符串缓冲大小 */
#define _TYPE_BUF_SIZE(type)            sizeof(#type)
#define TYPE_BUF_SIZE(type)            _TYPE_BUF_SIZE(type)
/* --------------------------------------------------------------------------------------------------- */


#endif  /* SYSONCE_INFRA_SYS_MACRO_H */

/*
*********************************************************************************************************
*                                               No More!
*********************************************************************************************************
*/
