#ifndef __JZ_VPU_REG_H__
#define __JZ_VPU_REG_H__

#define DEVICES_VPU_NAME "/dev/jz-vpu"

#define VPU_BASE 0x13200000
#define VPU_SIZE 0x000FFFFF
#define CPM_BASE 0x10000000
#define CPM_SIZE 0x00001000
/********************************************
  CPM (CPM)
*********************************************/
#define REG_CPM_SRBC 0xC4
#define CPM_VPU_SR           (0x1<<31)
#define CPM_VPU_STP          (0x1<<30)
#define CPM_VPU_ACK          (0x1<<29)

/********************************************
  SCH (Scheduler)
*********************************************/
#define TCSM_FLUSH           0xc0000
#define REG_SCH_GLBC         0x00000
#define SCH_GLBC_SLDE        (0x1<<31)
#ifdef JZM_V2_TLB
# define SCH_TLBE_JPGC       (0x1<<26)
# define SCH_TLBE_DBLK       (0x1<<25)
# define SCH_TLBE_SDE        (0x1<<24)
# define SCH_TLBE_EFE        (0x1<<23)
# define SCH_TLBE_VDMA       (0x1<<22)
# define SCH_TLBE_MCE        (0x1<<21)
#else
# define SCH_GLBC_TLBE       (0x1<<30)
# define SCH_GLBC_TLBINV     (0x1<<29)
#endif
#define SCH_INTE_ACFGERR     (0x1<<20)
#define SCH_INTE_TLBERR      (0x1<<18)
#define SCH_INTE_BSERR       (0x1<<17)
#define SCH_INTE_ENDF        (0x1<<16)
#define SCH_GLBC_HIMAP       (0x1<<15)
#define SCH_GLBC_HIAXI       (0x1<<9)
#define SCH_GLBC_EPRI0       (0x0<<7)
#define SCH_GLBC_EPRI1       (0x1<<7)
#define SCH_GLBC_EPRI2       (0x2<<7)
#define SCH_GLBC_EPRI3       (0x3<<7)

#define REG_SCH_TLBA         0x00030

#ifdef JZM_V2_TLB
# define REG_SCH_TLBC        0x00050
# define SCH_TLBC_VPN        (0xFFFFF000)
# define SCH_TLBC_RIDX(idx)  (((idx) & 0xFF)<<4)
# define SCH_TLBC_INVLD      (0x1<<1)
# define SCH_TLBC_RETRY      (0x1<<0)

# define REG_SCH_TLBV        0x00054
# define SCH_TLBV_CNM(cnm)   (((cnm) & 0xFFF)<<16)
# define SCH_TLBV_GCN(gcn)   (((gcn) & 0xFFF)<<0)
# define SCH_TLBV_RCI_MC     (0x1<<30)
# define SCH_TLBV_RCI_EFE    (0x1<<31)
#endif

#define REG_SCH_STAT         0x00034

#define REG_SCH_SLDE0        0x00040
#define REG_SCH_SLDE1        0x00044
#define REG_SCH_SLDE2        0x00048
#define REG_SCH_SLDE3        0x0004C
#define SCH_SLD_VTAG(val)    (((val) & 0xFFF)<<20)
#define SCH_SLD_MASK(val)    (((val) & 0xFFF)<<8)
#define SCH_SLD_VLD          (0x1<<0)

#define REG_SCH_SCHC         0x00060
#define SCH_CH1_PCH(ch)      (((ch) & 0x3)<<0)
#define SCH_CH2_PCH(ch)      (((ch) & 0x3)<<8)
#define SCH_CH3_PCH(ch)      (((ch) & 0x3)<<16)
#define SCH_CH4_PCH(ch)      (((ch) & 0x3)<<24)
#define SCH_CH1_PE           (0x1<<2)
#define SCH_CH2_PE           (0x1<<10)
#define SCH_CH3_PE           (0x1<<18)
#define SCH_CH4_PE           (0x1<<26)
#define SCH_CH1_GS0          (0x0<<3)
#define SCH_CH1_GS1          (0x1<<3)
#define SCH_CH2_GS0          (0x0<<11)
#define SCH_CH2_GS1          (0x1<<11)
#define SCH_CH3_GS0          (0x0<<19)
#define SCH_CH3_GS1          (0x1<<19)
#define SCH_CH4_GS0          (0x0<<27)
#define SCH_CH4_GS1          (0x1<<27)

#define REG_SCH_BND          0x00064
#define SCH_CH1_HID(hid)     (((hid) & 0xF)<<16)
#define SCH_CH2_HID(hid)     (((hid) & 0xF)<<20)
#define SCH_CH3_HID(hid)     (((hid) & 0xF)<<24)
#define SCH_CH4_HID(hid)     (((hid) & 0xF)<<28)
#define SCH_BND_G0F1         (0x1<<0)
#define SCH_BND_G0F2         (0x1<<1)
#define SCH_BND_G0F3         (0x1<<2)
#define SCH_BND_G0F4         (0x1<<3)
#define SCH_BND_G1F1         (0x1<<4)
#define SCH_BND_G1F2         (0x1<<5)
#define SCH_BND_G1F3         (0x1<<6)
#define SCH_BND_G1F4         (0x1<<7)
#define SCH_DEPTH(val)       (((val-1) & 0xF)<<8)

#define REG_SCH_SCHG0        0x00068
#define REG_SCH_SCHG1        0x0006C
#define REG_SCH_SCHE1        0x00070
#define REG_SCH_SCHE2        0x00074
#define REG_SCH_SCHE3        0x00078
#define REG_SCH_SCHE4        0x0007C

#define DSA_SCH_CH1          (VPU_BASE | REG_SCH_SCHE1)
#define DSA_SCH_CH2          (VPU_BASE | REG_SCH_SCHE2)
#define DSA_SCH_CH3          (VPU_BASE | REG_SCH_SCHE3)
#define DSA_SCH_CH4          (VPU_BASE | REG_SCH_SCHE4)
/********************************************
  VDMA (VPU general-purpose DMA)
*********************************************/
#define REG_VDMA_LOCK        0x10000
#define REG_VDMA_UNLK        0x10004
#define REG_VMDA_TRIG        0x10008
#define VDMA_ACFG_RUN        (0x1)
#define VDMA_DESC_RUN        (0x3)
#define VDMA_ACFG_CLR        (0x8)
#define VDMA_ACFG_SAFE       (0x4)
#define VDMA_ACFG_DHA(a)     (((unsigned int)(a)) & 0xFFFFFF80)
#define VDMA_DESC_DHA(a)     (((unsigned int)(a)) & 0xFFFF0)

#define REG_VDMA_TASKST      0x1000C
#define VDMA_ACFG_ERR        (0x1<<3)
#define VDMA_ACFG_END        (0x1<<2)
#define VDMA_DESC_END        (0x1<<1)
#define VDMA_VPU_BUSY        (0x1<<0)

#define VDMA_DESC_EXTSEL     (0x1<<0)
#define VDMA_DESC_TLBSEL     (0x1<<1)
#define VDMA_DESC_LK         (0x1<<31)

#define VDMA_ACFG_VLD        (0x1<<31)
#define VDMA_ACFG_TERM       (0x1<<30)
#define VDMA_ACFG_IDX(a)     (((unsigned int)(a)) & 0xFFFFC)

#define GEN_VDMA_ACFG(chn, reg, lk, val)                 \
({*chn++ = val;                                          \
  *chn++ = (VDMA_ACFG_VLD | (lk) | VDMA_ACFG_IDX(reg));  \
})

/****************************************************************
  JPGC (jpeg codec)
*****************************************************************/
#define REG_JPGC_TRIG        0xE0000
#define REG_JPGC_GLBI        0xE0004
#define REG_JPGC_STAT        0xE0008
#define REG_JPGC_BSA         0xE000C
#define REG_JPGC_P0A         0xE0010
#define REG_JPGC_P1A         0xE0014
#define REG_JPGC_P2A         0xE0018
#define REG_JPGC_P3A         0xE001C
#define REG_JPGC_NMCU        0xE0028
#define REG_JPGC_NRSM        0xE002C
#define REG_JPGC_P0C         0xE0030
#define REG_JPGC_P1C         0xE0034
#define REG_JPGC_P2C         0xE0038
#define REG_JPGC_P3C         0xE003C
#define REG_JPGC_MCUS        0xE0064
#define REG_JPGC_ZIGM0       0xE1000
#define REG_JPGC_ZIGM1       0xE1100
#define REG_JPGC_HUFB        0xE1200
#define REG_JPGC_HUFM        0xE1300
#define REG_JPGC_QMEM        0xE1400
#define REG_JPGC_HUFE        0xE1800
#define REG_JPGC_HUFS        0xE1800

#define JPGC_CORE_OPEN      (0x1<<0)
#define JPGC_BS_TRIG        (0x1<<1)
#define JPGC_PP_TRIG        (0x1<<2)
#define JPGC_TERM           (0x1<<3)
#define JPGC_RSTER_MD       (0x1<<8)


/********************************************
  EFE (Encoder Front End)
*********************************************/
#define REG_EFE_CTRL         0x40000
#define EFE_TSE(en)          (((en) & 0x1)<<31)
#define EFE_FMVP(en)         (((en) & 0x1)<<30)
#define EFE_ID_X264          (0x0<<14)
#define EFE_ID_JPEG          (0x1<<14)
#define EFE_ID_VP8           (0x2<<14)
#define EFE_X264_QP(qp)      (((qp) & 0x3F)<<8)
#define EFE_VP8_QTB(qtb)     (((qtb) & 0x7f)<<22)
#define EFE_VP8_QIDX(qp)     (((qp) & 0x3F)<<8)
#define EFE_VP8_LF(lf)       ((lf & 0x3F)<<16)
#define EFE_DBLK_EN          (0x1<<5)
#define EFE_SLICE_TYPE(a)    (((a) & 0x1)<<4)
#define EFE_PLANE_TILE       (0x0<<2)
#define EFE_PLANE_420P       (0x1<<2)
#define EFE_PLANE_NV12       (0x2<<2)
#define EFE_PLANE_NV21       (0x3<<2)
#define EFE_EN               (0x1<<1)
#define EFE_RUN              (0x1<<0)

#define REG_EFE_GEOM         0x40004
#define EFE_FST_MBY(mb)      (((mb) & 0xFF)<<24)
#define EFE_FST_MBX(mb)      (((0/*FIXME*/) & 0xFF)<<16)
#define EFE_LST_MBY(mb)      (((mb) & 0xFF)<<8)
#define EFE_LST_MBX(mb)      (((mb) & 0xFF)<<0)
#define EFE_JPGC_LST_MBY(mb) (((mb) & 0xFFFF)<<16)
#define EFE_JPGC_LST_MBX(mb) ((mb) & 0xFFFF)

#define REG_EFE_COEF_BA      0x4000C
#define REG_EFE_RAWY_SBA     0x40010
#define REG_EFE_RAWC_SBA     0x40014
#define REG_EFE_RAWU_SBA     0x40014
#define REG_EFE_TOPMV_BA     0x40018
#define REG_EFE_TOPPA_BA     0x4001C
#define REG_EFE_MECHN_BA     0x40020
#define REG_EFE_MAUCHN_BA    0x40024
#define REG_EFE_DBLKCHN_BA   0x40028
#define REG_EFE_SDECHN_BA    0x4002C
#define REG_EFE_RAW_DBA      0x40030
#define REG_EFE_RAWV_SBA     0x40034

#define REG_EFE_ROI_MAX_QP       0x40040
#define REG_EFE_ROI_BASE_INFO0   0x40044
#define REG_EFE_ROI_BASE_INFO1   0x40048
#define REG_EFE_ROI_POS_INFO0    0x4004C
#define REG_EFE_ROI_POS_INFO1    0x40050
#define REG_EFE_ROI_POS_INFO2    0x40054
#define REG_EFE_ROI_POS_INFO3    0x40058
#define REG_EFE_ROI_POS_INFO4    0x4005C
#define REG_EFE_ROI_POS_INFO5    0x40060
#define REG_EFE_ROI_POS_INFO6    0x40064
#define REG_EFE_ROI_POS_INFO7    0x40068
#define REG_EFE_RAW_STRD     0x40038
#define EFE_RAW_STRDY(y)     (((y) & 0xFFFF)<<16)
#define EFE_RAW_STRDC(c)     (((c) & 0xFFFF)<<0)

#define REG_EFE_DBG_INFO     0x4003C
#define EFE_DBG_EN           (0x1<<31)
#define EFE_DBG_BP_MBX(x)    (((x) & 0xFFF)<<0)
#define EFE_DBG_BP_MBY(y)    (((y) & 0xFFF)<<16)

#define REG_EFE_MVRP         0x40100
#define REG_EFE_SSAD         0x40108
#define REG_EFE_DCS          0x4010C
#define EFE_DCS_CLR(th)      (0x1<<(th & 0xF))
#define EFE_DCS_EN(en)       (((en) & 0x1)<<16)
#define EFE_DCS_RT(rt)       (((rt) & 0xF)<<20)
#define EFE_DCS_OTH(oth)     (((oth) & 0xF)<<24)
#define REG_EFE_STAT         0x40110

#define    WAIT_COMPLETE     0
#define    LOCK              1
#define    UNLOCK            2
#define    FLUSH_CACHE       3

#define CMD_VPU_CACHE           100
#define CMD_VPU_PHY             101
#define CMD_VPU_DMA_NOTLB       102
#define CMD_VPU_DMA_TLB         103
#define CMD_VPU_CLEAN_WAIT_FLAG 104
#define CMD_VPU_RESET           105
#define CMD_VPU_SET_REG         106
#define CMD_WAIT_COMPLETE 110

struct vpu_struct{
    unsigned int vpu_base;
    unsigned int cpm_base;
    int vpu_fd;
};

void* vpu_init();
void vpu_exit(void *vpu);
int jz_start_hw_compress(void *handle,unsigned int des_va,unsigned int des_pa);
#endif    /* __JZ_VPU_REG_H__ */
