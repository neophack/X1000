#ifndef __JZM_JPEG_ENC_H__
#define __JZM_JPEG_ENC_H__
#define HUFFENC_LEN    (384)        /* Huffman encoder  table lenth */
#define QMEM_LEN       (256)        /* Quantization table lenth */
#define HUFNUM         (1)          /* Huffman encode table number */
#define QTNUM          (3)          /* Quantization table number */
#define YUV420P0C      (0x30)       /* component 0 configure information NBLK<<4| QT<<2| HA<<1| HD */
#define YUV420P1C      (0x07)       /* component 1 configure information NBLK<<4| QT<<2| HA<<1| HD */
#define YUV420P2C      (0x07)       /* component 2 configure information NBLK<<4| QT<<2| HA<<1| HD */
#define YUV420PVH      (0x0a<<16)   /* component vertical/horizontal size of MCU:P3H P3V P2H P2V P1H P1V P0H P0V */
#define JPGC_RSM       (0x1<<2)     /* JPGC rstart marker enable signal */
#define JPGC_SPEC      (0x0<<1)     /* YUV420 mode */
#define JPGC_UNIV      (0x1<<1)     /* YUV444 or YUV422 mode */
#define JPGC_EN        (0x1)        /* JPGC enable signal */
#define OPEN_CLOCK     (0x1)        /* open the core clock */
#define JPGC_NCOL      (0x2<<4)     /* color numbers of a MCU minus 1,it always 2 for YUV color space */
#define STAT_CLEAN     (0x0)        /* clean the STAT register */
#define CORE_RST       (0x1<<6)     /* JPGC core reset ,high active */
#define JPGC_EFE       (0x1<<8)     /* JPGC EFE source */
#define YUV_YUY2       (0x1<<29)    /* YUY-YUY2 mode */
#define VRAM_RAWY_BA   (VPU_BASE | 0xF0000)
#define VRAM_RAWC_BA   (VRAM_RAWY_BA + 256)

static const uint32_t huffenc[HUFNUM][HUFFENC_LEN] = {
    {
        0x100,  0x101,  0x204,  0x30b,  0x41a,  0x678,  0x7f8,  0x9f6,
        0xf82,  0xf83,  0x30c,  0x41b,  0x679,  0x8f6,  0xaf6,  0xf84,
        0xf85,  0xf86,  0xf87,  0xf88,  0x41c,  0x7f9,  0x9f7,  0xbf4,
        0xf89,  0xf8a,  0xf8b,  0xf8c,  0xf8d,  0xf8e,  0x53a,  0x8f7,
        0xbf5,  0xf8f,  0xf90,  0xf91,  0xf92,  0xf93,  0xf94,  0xf95,
        0x53b,  0x9f8,  0xf96,  0xf97,  0xf98,  0xf99,  0xf9a,  0xf9b,
        0xf9c,  0xf9d,  0x67a,  0xaf7,  0xf9e,  0xf9f,  0xfa0,  0xfa1,
        0xfa2,  0xfa3,  0xfa4,  0xfa5,  0x67b,  0xbf6,  0xfa6,  0xfa7,
        0xfa8,  0xfa9,  0xfaa,  0xfab,  0xfac,  0xfad,  0x7fa,  0xbf7,
        0xfae,  0xfaf,  0xfb0,  0xfb1,  0xfb2,  0xfb3,  0xfb4,  0xfb5,
        0x8f8,  0xec0,  0xfb6,  0xfb7,  0xfb8,  0xfb9,  0xfba,  0xfbb,
        0xfbc,  0xfbd,  0x8f9,  0xfbe,  0xfbf,  0xfc0,  0xfc1,  0xfc2,
        0xfc3,  0xfc4,  0xfc5,  0xfc6,  0x8fa,  0xfc7,  0xfc8,  0xfc9,
        0xfca,  0xfcb,  0xfcc,  0xfcd,  0xfce,  0xfcf,  0x9f9,  0xfd0,
        0xfd1,  0xfd2,  0xfd3,  0xfd4,  0xfd5,  0xfd6,  0xfd7,  0xfd8,
        0x9fa,  0xfd9,  0xfda,  0xfdb,  0xfdc,  0xfdd,  0xfde,  0xfdf,
        0xfe0,  0xfe1,  0xaf8,  0xfe2,  0xfe3,  0xfe4,  0xfe5,  0xfe6,
        0xfe7,  0xfe8,  0xfe9,  0xfea,  0xfeb,  0xfec,  0xfed,  0xfee,
        0xfef,  0xff0,  0xff1,  0xff2,  0xff3,  0xff4,  0xff5,  0xff6,
        0xff7,  0xff8,  0xff9,  0xffa,  0xffb,  0xffc,  0xffd,  0xffe,
        0x30a,  0xaf9,  0xfff,  0xfff,  0xfff,  0xfff,  0xfff,  0xfff,
        0xfd0,  0xfd1,  0xfd2,  0xfd3,  0xfd4,  0xfd5,  0xfd6,  0xfd7,
        0x101,  0x204,  0x30a,  0x418,  0x419,  0x538,  0x678,  0x8f4,
        0x9f6,  0xbf4,  0x30b,  0x539,  0x7f6,  0x8f5,  0xaf6,  0xbf5,
        0xf88,  0xf89,  0xf8a,  0xf8b,  0x41a,  0x7f7,  0x9f7,  0xbf6,
        0xec2,  0xf8c,  0xf8d,  0xf8e,  0xf8f,  0xf90,  0x41b,  0x7f8,
        0x9f8,  0xbf7,  0xf91,  0xf92,  0xf93,  0xf94,  0xf95,  0xf96,
        0x53a,  0x8f6,  0xf97,  0xf98,  0xf99,  0xf9a,  0xf9b,  0xf9c,
        0xf9d,  0xf9e,  0x53b,  0x9f9,  0xf9f,  0xfa0,  0xfa1,  0xfa2,
        0xfa3,  0xfa4,  0xfa5,  0xfa6,  0x679,  0xaf7,  0xfa7,  0xfa8,
        0xfa9,  0xfaa,  0xfab,  0xfac,  0xfad,  0xfae,  0x67a,  0xaf8,
        0xfaf,  0xfb0,  0xfb1,  0xfb2,  0xfb3,  0xfb4,  0xfb5,  0xfb6,
        0x7f9,  0xfb7,  0xfb8,  0xfb9,  0xfba,  0xfbb,  0xfbc,  0xfbd,
        0xfbe,  0xfbf,  0x8f7,  0xfc0,  0xfc1,  0xfc2,  0xfc3,  0xfc4,
        0xfc5,  0xfc6,  0xfc7,  0xfc8,  0x8f8,  0xfc9,  0xfca,  0xfcb,
        0xfcc,  0xfcd,  0xfce,  0xfcf,  0xfd0,  0xfd1,  0x8f9,  0xfd2,
        0xfd3,  0xfd4,  0xfd5,  0xfd6,  0xfd7,  0xfd8,  0xfd9,  0xfda,
        0x8fa,  0xfdb,  0xfdc,  0xfdd,  0xfde,  0xfdf,  0xfe0,  0xfe1,
        0xfe2,  0xfe3,  0xaf9,  0xfe4,  0xfe5,  0xfe6,  0xfe7,  0xfe8,
        0xfe9,  0xfea,  0xfeb,  0xfec,  0xde0,  0xfed,  0xfee,  0xfef,
        0xff0,  0xff1,  0xff2,  0xff3,  0xff4,  0xff5,  0xec3,  0xff6,
        0xff7,  0xff8,  0xff9,  0xffa,  0xffb,  0xffc,  0xffd,  0xffe,
        0x100,  0x9fa,  0xfff,  0xfff,  0xfff,  0xfff,  0xfff,  0xfff,
        0xfd0,  0xfd1,  0xfd2,  0xfd3,  0xfd4,  0xfd5,  0xfd6,  0xfd7,
        0x100,  0x202,  0x203,  0x204,  0x205,  0x206,  0x30e,  0x41e,
        0x53e,  0x67e,  0x7fe,  0x8fe,  0xfff,  0xfff,  0xfff,  0xfff,
        0x100,  0x101,  0x102,  0x206,  0x30e,  0x41e,  0x53e,  0x67e,
        0x7fe,  0x8fe,  0x9fe,  0xafe,  0xfff,  0xfff,  0xfff,  0xfff
    }
};

static const uint32_t qmem[QTNUM][QMEM_LEN] = {
    //normal quantization table
    {
        0x0100, 0x0155, 0x0155, 0x0a49, 0x0155, 0x0b33, 0x0100, 0x0a49,
        0x0a49, 0x0a49, 0x09c7, 0x09c7, 0x0100, 0x00cd, 0x0955, 0x08cd,
        0x093b, 0x0955, 0x12e9, 0x12e9, 0x0955, 0x251f, 0x11c7, 0x11af,
        0x0911, 0x08cd, 0x1a35, 0x113b, 0x2421, 0x1111, 0x1a35, 0x113b,
        0x1a49, 0x1a49, 0x0880, 0x19c7, 0x10b2, 0x10d2, 0x0880, 0x10f1,
        0x10ba, 0x10ea, 0x1a49, 0x1a49, 0x10cd, 0x1095, 0x231f, 0x10ba,
        0x1955, 0x229d, 0x193b, 0x193b, 0x193b, 0x2421, 0x10d2, 0x223f,
        0x2219, 0x2249, 0x10a4, 0x1911, 0x10b2, 0x2d05, 0x193b, 0x10a4,
        0x09c7, 0x09c7, 0x09c7, 0x0955, 0x12e9, 0x0955, 0x1155, 0x093b,
        0x093b, 0x1155, 0x10a4, 0x23e1, 0x1a49, 0x23e1, 0x10a4, 0x10a4,
        0x10a4, 0x10a4, 0x10a4, 0x10a4, 0x10a4, 0x10a4, 0x10a4, 0x10a4,
        0x10a4, 0x10a4, 0x10a4, 0x10a4, 0x10a4, 0x10a4, 0x10a4, 0x10a4,
        0x10a4, 0x10a4, 0x10a4, 0x10a4, 0x10a4, 0x10a4, 0x10a4, 0x10a4,
        0x10a4, 0x10a4, 0x10a4, 0x10a4, 0x10a4, 0x10a4, 0x10a4, 0x10a4,
        0x10a4, 0x10a4, 0x10a4, 0x10a4, 0x10a4, 0x10a4, 0x10a4, 0x10a4,
        0x10a4, 0x10a4, 0x10a4, 0x10a4, 0x10a4, 0x10a4, 0x10a4, 0x10a4,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
    },
    //finer quantization table
    {
        0x0155, 0x0200, 0x0200, 0x0b33, 0x0200, 0x0200, 0x0155, 0x0b33,
        0x0b33, 0x0b33, 0x0155, 0x0155, 0x0155, 0x0155, 0x0100, 0x093b,
        0x09c7, 0x0100, 0x0a49, 0x0a49, 0x0100, 0x0911, 0x12e9, 0x0955,
        0x09c7, 0x093b, 0x11c7, 0x0080, 0x11af, 0x11af, 0x11c7, 0x0080,
        0x11c7, 0x08f1, 0x08cd, 0x08ba, 0x1a49, 0x1155, 0x08cd, 0x08c3,
        0x1a5f, 0x08c3, 0x08f1, 0x11c7, 0x251f, 0x23e1, 0x251f, 0x1a5f,
        0x1a35, 0x1111, 0x0880, 0x0880, 0x0880, 0x11af, 0x1155, 0x10ea,
        0x19bb, 0x10f1, 0x2421, 0x19bb, 0x1a49, 0x2421, 0x0880, 0x1111,
        0x0155, 0x0155, 0x0155, 0x0100, 0x0a49, 0x0100, 0x0911, 0x09c7,
        0x09c7, 0x0911, 0x1111, 0x08c3, 0x11c7, 0x08c3, 0x1111, 0x1111,
        0x1111, 0x1111, 0x1111, 0x1111, 0x1111, 0x1111, 0x1111, 0x1111,
        0x1111, 0x1111, 0x1111, 0x1111, 0x1111, 0x1111, 0x1111, 0x1111,
        0x1111, 0x1111, 0x1111, 0x1111, 0x1111, 0x1111, 0x1111, 0x1111,
        0x1111, 0x1111, 0x1111, 0x1111, 0x1111, 0x1111, 0x1111, 0x1111,
        0x1111, 0x1111, 0x1111, 0x1111, 0x1111, 0x1111, 0x1111, 0x1111,
        0x1111, 0x1111, 0x1111, 0x1111, 0x1111, 0x1111, 0x1111, 0x1111,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
    },
    //finer quantization table
    {
        0x02ab, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x02ab, 0x0400,
        0x0400, 0x0400, 0x02ab, 0x02ab, 0x02ab, 0x02ab, 0x02ab, 0x0b33,
        0x0200, 0x02ab, 0x02ab, 0x02ab, 0x02ab, 0x0155, 0x0200, 0x0b33,
        0x0200, 0x0b33, 0x0a49, 0x0155, 0x0a49, 0x0a49, 0x0a49, 0x0155,
        0x0a49, 0x0155, 0x0a49, 0x0100, 0x00cd, 0x09c7, 0x0a49, 0x0100,
        0x00cd, 0x0100, 0x0155, 0x0a49, 0x09c7, 0x0955, 0x09c7, 0x00cd,
        0x00cd, 0x12e9, 0x12e9, 0x12e9, 0x12e9, 0x0a49, 0x09c7, 0x0955,
        0x093b, 0x0955, 0x12e9, 0x093b, 0x00cd, 0x12e9, 0x12e9, 0x12e9,
        0x02ab, 0x02ab, 0x02ab, 0x02ab, 0x02ab, 0x02ab, 0x0155, 0x0200,
        0x0200, 0x0155, 0x12e9, 0x0100, 0x0a49, 0x0100, 0x12e9, 0x12e9,
        0x12e9, 0x12e9, 0x12e9, 0x12e9, 0x12e9, 0x12e9, 0x12e9, 0x12e9,
        0x12e9, 0x12e9, 0x12e9, 0x12e9, 0x12e9, 0x12e9, 0x12e9, 0x12e9,
        0x12e9, 0x12e9, 0x12e9, 0x12e9, 0x12e9, 0x12e9, 0x12e9, 0x12e9,
        0x12e9, 0x12e9, 0x12e9, 0x12e9, 0x12e9, 0x12e9, 0x12e9, 0x12e9,
        0x12e9, 0x12e9, 0x12e9, 0x12e9, 0x12e9, 0x12e9, 0x12e9, 0x12e9,
        0x12e9, 0x12e9, 0x12e9, 0x12e9, 0x12e9, 0x12e9, 0x12e9, 0x12e9,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
    }
};

#endif// __JZM_JPEG_ENC_H__
