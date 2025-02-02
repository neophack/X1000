#ifndef __VIC_REG_H__
#define __VIC_REG_H__

#ifdef CONFIG_SOC_T10
#define VIC_SUPPORT_MIPI                0
#define VIC_DB_CFG		         0x0
#define DVP_DATA_POS			(1<<24)
#define DVP_RGB_ORDER			(1<<21)
#define DVP_RAW_ALIG			(1<<20)
#define DVP_DATA_TYPE			(17)
#define DVP_RAW8			(0<<DVP_DATA_TYPE)
#define DVP_RAW10			(1<<DVP_DATA_TYPE)
#define DVP_RAW12			(2<<DVP_DATA_TYPE)
#define DVP_YUV422_16BIT		(3<<DVP_DATA_TYPE)
#define DVP_RGB565_16BIT		(4<<DVP_DATA_TYPE)
#define DVP_BRG565_16BIT		(5<<DVP_DATA_TYPE)
#define DVP_YUV422_8BIT			(6<<DVP_DATA_TYPE)
#define DVP_RGB565_8BIT			(7<<DVP_DATA_TYPE)
#define DVP_HREF_MODE		(0 << 15)
#define DVP_HSYNC_MODE		(1 << 15)
#define DVP_SONY_MODE		(2 << 15)

#define DVP_TIMEING_MODE		(1<<15)
#define BT_INTF_WIDE			(1<<11)
#define BT_LINE_MODE			(1<<10)
#define BT_601_TIMING_MODE		(1<<9)
#define YUV_DATA_ORDER			(4)
#define UYVY				(0<<YUV_DATA_ORDER)
#define VYUY				(1<<YUV_DATA_ORDER)
#define YUYV				(2<<YUV_DATA_ORDER)
#define YVYU				(3<<YUV_DATA_ORDER)
#define FIRST_FIELD_TYPE		(1<<3)
#define INTERLACE_EN			(1<<2)
#define HSYN_POLAR			(1<<1)
#define VSYN_POLAR			(1<<0)

#define VIC_INPUT_HSYNC_BLANKING	0x04
#define VIC_INPUT_VSYNC_BLANKING	0x08

/* #define VIC_VCNT0                       0x0C */
/* #define VIC_VCNY1                       0x10 */
/* #define VIC_VCNY2                       0x14 */

/* #define VIC_INTF_TYPE                   0x1C */
/* #define INTF_TYPE_BT656			0x0 */
/* #define INTF_TYPE_BT601			0x1 */
/* #define INTF_TYPE_MIPI			0x2 */
/* #define INTF_TYPE_DVP			0x3 */
/* #define INTF_TYPE_BT1120		0x4 */

#define VIC_RESOLUTION	 	        0x0C
#define H_RESOLUTION			(1<<16)
#define V_RESOLUTION			(1)

#define VIC_H_BLANK_NUM		        (0x10)
#define VIC_V_BLANK_NUM		        (0x14)

#define VIC_AB_VALUE		        (0x18)
#define A_VALUE				(1<<16)
#define B_VALUE				(1)

#define VIC_GLOBAL_CFG             	(0x1c)
#define ISP_PRESET_MODE1		(0<<5)
#define ISP_PRESET_MODE2		(1<<5)
#define ISP_PRESET_MODE3		(2<<5)
#define VCKE_EN				(1<<4)
#define BLANK_EN			(2)
#define AB_MODE_SELECT			(0)

#define VIC_CONTROL			(0x20)
#define VIC_RESET			(1<<4)
#define GLB_SAFE_RST			(1<<3)
#define GLB_RST				(1<<2)
#define REG_ENABLE			(1<<1)
#define VIC_SRART			(1<<0)

#define ISP_VPLCK_GATE                  0x24
#define VIC_PIXEL			0x28
#define VIC_LINE			0x2c
#define VIC_STATE			0x30
#define VIC_OFIFO_COUNT			0x34
#define VIC_FLASH_STROBE		0x38
#define VIC_FIRST_CB			0x48
#define VIC_SECOND_CB			0x4c
#define VIC_THIRD_CB			0x50
#define VIC_FOURTH_CB			0x54
#define VIC_FIFTH_CB			0x58
#define VIC_SIXTH_CB			0x5c
#define VIC_SEVENTH_CB			0x60
#define VIC_EIGHTH_CB			0x64
#define CB_MODE0			0x68
#define CB_MODE1			0x6c
#define BK_NUM_CB1			0x70

#elif defined(CONFIG_SOC_T30) || defined(CONFIG_SOC_T20) || defined(CONFIG_SOC_X1630)
#define VIC_SUPPORT_MIPI                1
#define VIC_DB_CFG		        0x10
#define DVP_DATA_POS			(1<<24)
#define DVP_RGB_ORDER			(1<<21)
#define DVP_RAW_ALIG			(1<<20)
#define DVP_DATA_TYPE			(17)
#define DVP_RAW8			(0<<DVP_DATA_TYPE)
#define DVP_RAW10			(1<<DVP_DATA_TYPE)
#define DVP_RAW12			(2<<DVP_DATA_TYPE)
#define DVP_YUV422_16BIT		(3<<DVP_DATA_TYPE)
#define DVP_RGB565_16BIT		(4<<DVP_DATA_TYPE)
#define DVP_BRG565_16BIT		(5<<DVP_DATA_TYPE)
#define DVP_YUV422_8BIT			(6<<DVP_DATA_TYPE)
#define DVP_RGB565_8BIT			(7<<DVP_DATA_TYPE)

#define DVP_TIMEING_MODE		(1<<15)
#define DVP_SONY_MODE		        (2 << 15)
#define BT_INTF_WIDE			(1<<11)
#define BT_LINE_MODE			(1<<10)
#define BT_601_TIMING_MODE		(1<<9)
#define YUV_DATA_ORDER			(4)
#define UYVY				(0<<YUV_DATA_ORDER)
#define VYUY				(1<<YUV_DATA_ORDER)
#define YUYV				(2<<YUV_DATA_ORDER)
#define YVYU				(3<<YUV_DATA_ORDER)
#define FIRST_FIELD_TYPE		(1<<3)
#define INTERLACE_EN			(1<<2)
#define HSYN_POLAR			(1<<1)
#define VSYN_POLAR			(1<<0)

#define VIC_HCNT0                       0x04
#define VIC_HCNT1                       0x08

/* #define VIC_VCNT0                       0x0C */
/* #define VIC_VCNY1                       0x10 */
/* #define VIC_VCNY2                       0x14 */

#define VIC_FRM_ECC                     0x08
#define FRAME_ECC_EN		       (1<<0)
#define FRAME_ECC_MODE		       (1<<1)
#define VIC_INTF_TYPE                   0x0C
#define INTF_TYPE_BT656			0x0
#define INTF_TYPE_BT601			0x1
#define INTF_TYPE_MIPI			0x2
#define INTF_TYPE_DVP			0x3
#define INTF_TYPE_BT1120		0x4

#define VIC_RESOLUTION	 	        0x04
#define H_RESOLUTION			(1<<16)
#define V_RESOLUTION			(1)

/* #define VIC_H_BLANK_NUM		        (0x10) */
/* #define VIC_V_BLANK_NUM		        (0x14) */

#define VIC_IDI_TYPE                    (0x14)/* VIC_INPUT_MIPI */
#define MIPI_RAW8			0x0
#define MIPI_RAW10			0x1
#define MIPI_RAW12			0x2
#define MIPI_RGB555			0x3
#define MIPI_RGB565			0x4
#define MIPI_RGB666			0x5
#define MIPI_RGB888			0x6
#define MIPI_YUV422			0x7
#define MIPI_YUV422_10BIT		0x8

#define VIC_AB_VALUE		        (0x18)
#define A_VALUE				(1<<16)
#define B_VALUE				(1)

#define VIC_GLOBAL_CFG             	(0x50)
#define ISP_PRESET_MODE1		(0<<5)
#define ISP_PRESET_MODE2		(1<<5)
#define ISP_PRESET_MODE3		(2<<5)
#define VCKE_EN				(1<<4)
#define BLANK_EN			(2)
#define AB_MODE_SELECT			(0)

#define VIC_CONTROL			(0x0)
#define VIC_RESET			(1<<4)
#define GLB_SAFE_RST			(1<<3)
#define GLB_RST				(1<<2)
#define REG_ENABLE			(1<<1)
#define VIC_SRART			(1<<0)

#define ISP_VPLCK_GATE                  0x24
#define VIC_PIXEL			0x94
#define VIC_LINE			0x98
#define VIC_STATE			0x90
#define VIC_OFIFO_COUNT			0x9c
#define VIC_FLASH_STROBE		0x100
#define VIC_FIRST_CB			0xc0
#define VIC_SECOND_CB			0xc4
#define VIC_THIRD_CB			0xc8
#define VIC_FOURTH_CB			0xCC
#define VIC_FIFTH_CB			0xD0
#define VIC_SIXTH_CB			0xD4
#define VIC_SEVENTH_CB			0xD8
#define VIC_EIGHTH_CB			0xDC
#define CB_MODE0			0xb0
#define CB_MODE1			0xa0
#define BK_NUM_CB1			0xb4

#define VIC_INPUT_HSYNC_BLANKING 0x20
#define VIC_INPUT_VSYNC_BLANKING 0x3c

#define VIC_DMA_OUTPUT_MAX_WIDTH 2688
#endif/* CONFIG_SOC_T30 */

#define VIC_DMA_CONFIG			0x10000
#define VIC_DMA_RESOLUTION		0x10004
#define VIC_DMA_RESET			0x10008
#define VIC_DMA_Y_STRID			0x10014
#define VIC_DMA_Y_BUF0			0x10018
#define VIC_DMA_UV_STRID		0x10034
#define VIC_DMA_UV_BUF0			0x10038

#define TX_ISP_TOP_IRQ_CNT		0x20000
#define TX_ISP_TOP_IRQ_CNT1		0x20020
#define TX_ISP_TOP_IRQ_CNT2		0x20024
#define TX_ISP_TOP_IRQ_CLR_1	0x20004
#define TX_ISP_TOP_IRQ_CLR_ALL	0x20008
#define TX_ISP_TOP_IRQ_STA		0x2000C
#define TX_ISP_TOP_IRQ_OVF		0x20010
#define TX_ISP_TOP_IRQ_ENABLE	0x20014
#define TX_ISP_TOP_IRQ_MASK		0x2001c
#define TX_ISP_TOP_IRQ_ISP		0xffff
#define TX_ISP_TOP_IRQ_VIC		0x7f0000
#define TX_ISP_TOP_IRQ_ALL		0x7fffff

#endif/* __VIC_REG_H__ */
