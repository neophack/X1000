/*
 * sc031gs Camera Driver
 *
 * Copyright (C) 2018, Ingenic Semiconductor Inc.
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

//#define DEBUG 1
#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/videodev2.h>
#include <media/v4l2-chip-ident.h>
#include <media/v4l2-subdev.h>
#include <media/soc_camera.h>
#include <media/soc_mediabus.h>

#define DEFAULT_WIDTH		640
#define DEFAULT_HEIGHT		128

#define CHIP_ID_SC031GS 	0x0031
#define CHIP_ID_HIGH_REG    0x3107
#define CHIP_ID_LOW_REG		0x3108
#define CHIP_ID_HIGH        0x00
#define CHIP_ID_LOW			0x31

#if 0
#define V4L2_CID_CHD_GETREG 			((0x00980000|0x900)+91) 
#define V4L2_CID_CHD_SETREG 			((0x00980000|0x900)+92)
#define V4L2_CHD_VERMIRROR_SETREG 	((0x00980000|0x900)+93) // Vertical inversion control [SET]
#define V4L2_CHD_VERMIRROR_GETREG 	((0x00980000|0x900)+94) // Vertical inversion control [GET]
#define V4L2_CHD_HORMIRROR_SETREG 	((0x00980000|0x900)+95) // Horizontal inversion control [SET]
#define V4L2_CHD_HORMIRROR_GETREG 	((0x00980000|0x900)+96) // Horizontal inversion control [GET]
#endif
/*
 * Struct
 */
struct regval_list {
	u16 reg_num;
    unsigned char value;
};

/* Supported resolutions */
enum sc031gs_width {
    W_VGA   = 640,
};

enum sc031gs_height {
    H_VGA   = 128,
};

struct sc031gs_win_size {
    char *name;
    enum sc031gs_width width;
    enum sc031gs_height height;
    const struct regval_list *regs;
};


struct sc031gs_priv {
    struct v4l2_subdev subdev;
    struct v4l2_ctrl_handler ctrl_handler;
    enum v4l2_mbus_pixelcode cfmt_code;
    struct v4l2_mbus_framefmt mf;
    const struct sc031gs_win_size *win;

    unsigned char power_en;
	unsigned char gain;
    unsigned int  exposure;
    unsigned char exposure_mode;
    unsigned char flag_vflip:1;
    unsigned char flag_hflip:1;

    struct work_struct resume_work;
};

static inline int sensor_i2c_master_send(struct i2c_client *client, const char *buf ,int len)
{
	int ret;
	struct i2c_adapter *adap=client->adapter;
	struct i2c_msg msg;

	msg.addr = client->addr;
	msg.flags = client->flags & I2C_M_TEN;
	msg.len = len;
	msg.buf = (char *)buf;
	ret = i2c_transfer(adap, &msg, 1);
	
	return (ret == 1) ? len : ret;
}

static inline int sensor_i2c_master_recv(struct i2c_client *client, char *buf ,int len)
{
	struct i2c_adapter *adap = client->adapter;
	struct i2c_msg msg;
	int ret;

	msg.addr = client->addr;
	msg.flags = client->flags & I2C_M_TEN;
	msg.flags |= I2C_M_RD;
	msg.len = len;
	msg.buf = buf;
	ret = i2c_transfer(adap, &msg, 1);

	/* If everything went ok (i.e. 1 msg transmitted), return #bytes
	   transmitted, else error code. */
	return (ret == 1) ? len : ret;
}

static inline int sc031gs_write_reg(struct i2c_client * client, u16 reg, unsigned char val)
{
	unsigned char msg[3];
	int ret;

	reg = cpu_to_be16(reg);

	memcpy(&msg[0], &reg, 2);
	memcpy(&msg[2], &val, 1);

	ret = sensor_i2c_master_send(client, msg, 3);

	if (ret < 0){
		printk("RET < 0\n");
		return ret;
	}
	if (ret < 3){
		printk("RET < 3\n");
		return -EIO;
	}

	return 0;
}

static inline int sc031gs_read_reg(struct i2c_client *client, u16 reg)
{
	int ret;
	unsigned char retval;
	u16 r = cpu_to_be16(reg);

	ret = sensor_i2c_master_send(client, (unsigned char *)&r, 2);

	if (ret < 0)
		return ret;
	if (ret != 2)
		return -EIO;

	ret = sensor_i2c_master_recv(client, &retval, 1);
	if (ret < 0)
		return ret;
	if (ret != 1)
		return -EIO;
	
	return retval;
}

/*
 * Registers settings
 */
#define ENDMARKER {0xffff, 0xff}

static const struct regval_list sc031gs_init_regs[] = {
	// 24M input 64M pclk
	{0x0100, 0x00},
	{0x300f, 0x0f},
	{0x3018, 0x1f},
	{0x3019, 0xff},
	{0x301c, 0xb4},
	{0x301f, 0x01},
	{0x3028, 0x82},
	{0x3220, 0x10},
	{0x3223, 0x50},
	{0x3250, 0xf0},
	{0x3251, 0x02},
	{0x3254, 0x02},
	{0x3255, 0x07},
	{0x3304, 0x48},
	{0x3306, 0x48},
	{0x3309, 0x68},
	{0x330b, 0xe0},
	{0x330c, 0x18},
	{0x330f, 0x20},
	{0x3310, 0x10},
	{0x3314, 0x3a},
	{0x3315, 0x38},
	{0x3316, 0x48},
	{0x3317, 0x20},
	{0x3329, 0x3c},
	{0x332d, 0x3c},
	{0x332f, 0x40},
	{0x3335, 0x44},
	{0x3344, 0x44},
	{0x335b, 0x80},
	{0x335f, 0x80},
	{0x3366, 0x06},
	{0x3385, 0x31},
	{0x3387, 0x51},
	{0x3389, 0x01},
	{0x33b1, 0x03},
	{0x33b2, 0x06},
	{0x3621, 0xa4},
	{0x3622, 0x05},
	{0x3624, 0x47},
	{0x3630, 0x46},
	{0x3631, 0x48},
	{0x3633, 0x52},
	{0x3635, 0x18},
	{0x3636, 0x25},
	{0x3637, 0x89},
	{0x3638, 0x0f},
	{0x3639, 0x08},
	{0x363a, 0x00},
	{0x363b, 0x48},
	{0x363c, 0x06},
	{0x363d, 0x00},
	{0x363e, 0xf8},
	{0x3640, 0x01},//0x00}, Bit[1:0]: PCLK DLY 2ns/step
	{0x3641, 0x01},
	{0x36e9, 0x44},
	{0x36ea, 0x36},
	{0x36eb, 0x1a},
	{0x36ec, 0x0a},
	{0x36ed, 0x13},
	{0x36f9, 0x44},
	{0x36fa, 0x20},
	{0x3908, 0x91},
	{0x3d08, 0x01}, // polarity: bit[0] pclk  bit[1] VSYNC  bit[2] HSYNC 
	{0x3e01, 0x12},
	{0x3e02, 0x30},
	{0x3e06, 0x0c},
	{0x4500, 0x59},
	{0x4501, 0xc4},
	{0x4809, 0x01},
	{0x4837, 0x1f},
	{0x5011, 0x00},

	{0x3208, 0x02},//-
	{0x3209, 0x80},// 输出窗口宽度
	{0x320a, 0x00},//-
	{0x320b, 0x80},// 输出窗口高度
	{0x320c, 0x03},//-
	{0x320d, 0x6e},// 行长 一行数据的个数
	{0x320e, 0x00},//-
	{0x320f, 0xa0},// 帧长 一帧图像的行数   fps=pclk/(行长 * 帧长)
	{0x3210, 0x00},//-
	{0x3211, 0x08},// 输出窗口列起始位置
//	{0x3212, 0x00},//-
//	{0x3213, 0x04},// 输出窗口行起始位置
	{0x3202, 0x00},
	{0x3203, 0xb8},
	{0x3206, 0x01},
	{0x3207, 0x47},
	{0x3252, 0x00},
	{0x3253, 0x9b},	

	{0x0100, 0x01},
	{0x4418, 0x08},
	{0x4419, 0x8a},
	// gain
//	{0x3e08, 0x42},
//	{0x3e09, 0x42},
//	{0x3314, 0x42}, // gain
//	{0x3317, 0x20}, // gain

	ENDMARKER,
};

static const struct regval_list sc031gs_vga_regs[] = {
    ENDMARKER,
};


#define SC031GS_SIZE(n, w, h, r) \
    {.name = n, .width = w , .height = h, .regs = r }

static const struct sc031gs_win_size sc031gs_supported_win_sizes[] = {
    SC031GS_SIZE("VGA", W_VGA, H_VGA, sc031gs_vga_regs),
};

static const struct regval_list sc031gs_enable_output_regs[] = {
    ENDMARKER,
};

static const struct regval_list sc031gs_disable_output_regs[] = {
    ENDMARKER,
};

static const struct regval_list sc031gs_enable_auto_exposure_regs[] = {
    ENDMARKER,
};

static const struct regval_list sc031gs_disable_auto_exposure_regs[] = {
    ENDMARKER,
};

static enum v4l2_mbus_pixelcode sc031gs_codes[] = {
    V4L2_MBUS_FMT_Y8_1X8,
/*  V4L2_MBUS_FMT_YUYV8_2X8,
    V4L2_MBUS_FMT_YUYV8_1_5X8,
    V4L2_MBUS_FMT_JZYUYV8_1_5X8,*/
};

/*
 * General functions
 */
static struct sc031gs_priv *to_sc031gs(const struct i2c_client *client)
{
    return container_of(i2c_get_clientdata(client), struct sc031gs_priv, subdev);
}

static int sc031gs_write_array(struct i2c_client *client, const struct regval_list *vals)
{
    int ret;

    while ((vals->reg_num != 0xffff) || (vals->value != 0xff)) {
        ret = sc031gs_write_reg(client, vals->reg_num, vals->value);
        if (ret < 0) {
            return ret;
        }
//		printk("write array: 0x%x, 0x%x\n", vals->reg_num, vals->value);
        vals++;
    }
    return 0;
}

static int sc031gs_read_array(struct i2c_client  *client, struct regval_list *vals)
{
	unsigned char myreg;

	while ((vals->reg_num != 0xffff) || (vals->value != 0xff)) {
		myreg = sc031gs_read_reg(client, vals->reg_num);
		printk("read array: 0x%x, 0x%x\n", vals->reg_num, vals->value);
		vals++;
	}

	return 0;
}

static void sc031gs_stream_on(struct sc031gs_priv* priv)
{
    struct i2c_client  *client = v4l2_get_subdevdata(&priv->subdev);

    sc031gs_write_array(client, sc031gs_enable_output_regs);
}

static void sc031gs_stream_off(struct sc031gs_priv* priv)
{
    struct i2c_client  *client = v4l2_get_subdevdata(&priv->subdev);

    sc031gs_write_array(client, sc031gs_disable_output_regs);
}

/*
 * soc_camera_ops functions
 */
static int sc031gs_s_stream(struct v4l2_subdev *sd, int enable)
{
    struct i2c_client  *client = v4l2_get_subdevdata(sd);
    struct sc031gs_priv *priv = to_sc031gs(client);

    if (enable) {
        sc031gs_stream_on(priv);
    } else {
        sc031gs_stream_off(priv);
    }
    return 0;
}


static int sc031gs_g_chip_ident(struct v4l2_subdev *sd,
                       struct v4l2_dbg_chip_ident *id)
{
    return CHIP_ID_SC031GS;
}

// 曝光时间设置
static int ctrl_inttime_update(struct i2c_client *client, unsigned int u32IntTime)        //u32IntTime单位是行
{       
	unsigned char u8Reg0x3e00, u8Reg0x3e01, u8Reg0x3e02;
	
    u32IntTime *= 16;     
	u8Reg0x3e00 = (u32IntTime >>  16) & 0x0F;
    u8Reg0x3e01 = (u32IntTime >>  8)  & 0xFF;		
    u8Reg0x3e02 = u32IntTime & 0xFF;				
	 
	sc031gs_write_reg(client, 0x3e00, u8Reg0x3e00);
	sc031gs_write_reg(client, 0x3e01, u8Reg0x3e01);
	sc031gs_write_reg(client, 0x3e02, u8Reg0x3e02);

	return;                                                                                           
}

static int s_ctrl_gain_handle(struct i2c_client *client, unsigned char gain)
{
	unsigned char Reg0x3e08, Reg0x3e09; // 模拟增益
//	unsigned char Reg0x3e06, Reg0x3e07; // 数字增益，调节也有效果，此处未加

	Reg0x3e08 = 0x03;
	
	if(gain < 16){
		Reg0x3e09  = 0x10 + gain;		
	}
	else if(gain < 32){
		Reg0x3e08 |= 0x04; 
		Reg0x3e09  = 0x10 + (gain - 16);		
	}
	else if(gain < 48){
		Reg0x3e08 |= 0x0C; 
		Reg0x3e09  = 0x10 + (gain - 32);		
	}
	else if(gain < 64){
		Reg0x3e08 |= 0x1C; 
		Reg0x3e09  = 0x10 + (gain - 48);
	}
	
	sc031gs_write_reg(client, 0x3e08, Reg0x3e08);
	sc031gs_write_reg(client, 0x3e09, Reg0x3e09);

	if(gain < 16){
		sc031gs_write_reg(client, 0x3314, 0x42);
		sc031gs_write_reg(client, 0x3317, 0x20);
	}
	else{
		sc031gs_write_reg(client, 0x3314, 0x4f);
		sc031gs_write_reg(client, 0x3317, 0x0f);
	}	

printk("gain = %d, 0x3e08=0x%x 0x3e09=0x%x\n", gain, Reg0x3e08, Reg0x3e09);

	return 0;
}

static int s_ctrl_auto_exposure_handle(struct i2c_client *client, int mode)
{
    int ret;

    switch(mode) {
    case V4L2_EXPOSURE_AUTO:
        ret = sc031gs_write_array(client, sc031gs_enable_auto_exposure_regs);
        if (ret < 0) {
            dev_err(&client->dev,"Failed to write enable_auto_exposure regs\n");
            return -1;
        }
        break;

    case V4L2_EXPOSURE_MANUAL:
        ret = sc031gs_write_array(client, sc031gs_disable_auto_exposure_regs);
        if (ret < 0) {
            dev_err(&client->dev,"Failed to write disable_auto_exposure regs\n");
            return -1;
        }
        break;

    case V4L2_EXPOSURE_SHUTTER_PRIORITY:
    case V4L2_EXPOSURE_APERTURE_PRIORITY:
        dev_err(&client->dev,"Unsupport exposure cid: %d\n",mode);
        return -1;
    default:
        dev_err(&client->dev,"Unknow exposure cid: %d\n",mode);
        return -1;
    }
    return 0;
}

static int s_ctrl_exposure_handle(struct i2c_client *client, int value)
{
    return 0;
}

static int s_ctrl_mirror_vflip(struct i2c_client *client, int value)
{
    return 0;
}

static int s_ctrl_mirror_hflip(struct i2c_client *client, int value)
{
    return 0;
}

#if 0
#define V4L2_VCTL_BRIGHTNESS				((0x00980000 | 0x900)+0)	// 亮度
#define V4L2_VCTL_CONTRAST					((0x00980000 | 0x900)+1)	// 对比度
#define V4L2_VCTL_SATURATION				((0x00980000 | 0x900)+2) 	// 饱和
#define V4L2_VCTL_HUE						((0x00980000 | 0x900)+3) 	// 色调
#define V4L2_VCTL_WHITE_BALANCE				((0x00980000 | 0x900)+26)	// 白平衡温度
#define V4L2_VCTL_GAMMA						((0x00980000 | 0x900)+16)	// 伽马
#define V4L2_VCTL_GAIN						((0x00980000 | 0x900)+19)	// 增益
#define V4L2_VCTL_SHARPNESS					((0x00980000 | 0x900)+27)	// 清晰度
#define V4L2_VCTL_BACKLIGHT 				((0x00980000 | 0x900)+28)	// 背光补偿
#endif

static int sc031gs_g_volatile_ctrl_do(struct i2c_client *client, struct v4l2_control *ctrl)
{
    int ret = 0;

    switch (ctrl->id) {
	case V4L2_CID_CHD_GETREG:	// add by liangyh 获取sensor 寄存器的值
	case V4L2_CID_CHD_SETREG:
		break;
	case V4L2_CID_GAIN:
		break; 
    default:
        dev_err(&client->dev, "Has no V4L2 CID: 0x%x ", ctrl->id);
        return -EINVAL;
    }

    return ret;
}

static int sc031gs_handler_g_volatile_ctrl(struct v4l2_ctrl *ctrl)
{
    int ret;
    struct sc031gs_priv *priv   = container_of(ctrl->handler, struct sc031gs_priv, ctrl_handler);
    struct v4l2_subdev *sd      = &priv->subdev;
    struct i2c_client  *client  = v4l2_get_subdevdata(sd);
    struct v4l2_control control;

    control.id = ctrl->id;

    ret = sc031gs_g_volatile_ctrl_do(client, &control);
    if (ret < 0) {
        dev_err(&client->dev, "Failed to g ctrl\n");
        return -1;
    }
    ctrl->val = control.value;
    return 0;
}

#if 0 // add by liangyh, may no use
static int sc031gs_try_ctrl_do(struct i2c_client *client, struct v4l2_control *ctrl)
{
    int ret = 0;

    switch (ctrl->id) {
	case V4L2_CID_CHD_GETREG:	// add by liangyh 获取sensor 寄存器的值
	case V4L2_CID_CHD_SETREG:
		break;
	case V4L2_CID_GAIN:
		break; 
    default:
        dev_err(&client->dev, "Has no V4L2 CID: 0x%x ", ctrl->id);
        return -EINVAL;
    }

    return ret;
}

static int sc031gs_handler_try_ctrl(struct v4l2_ctrl *ctrl)
{
    int ret;
    struct sc031gs_priv *priv   = container_of(ctrl->handler, struct sc031gs_priv, ctrl_handler);
    struct v4l2_subdev *sd      = &priv->subdev;
    struct i2c_client  *client  = v4l2_get_subdevdata(sd);
    struct v4l2_control control;

    control.id = ctrl->id;

    ret = sc031gs_try_ctrl_do(client, &control);
    if (ret < 0) {
        dev_err(&client->dev, "Failed to g ctrl\n");
        return -1;
    }
    ctrl->val = control.value;
    return 0;
}

#endif

static int sc031gs_s_ctrl_do(struct i2c_client *client, struct v4l2_control *ctrl)
{
    int ret = 0;
	unsigned short reg = (ctrl->value >> 8) & 0xFFFF;
	unsigned char wtval = (ctrl->value)&0xFF;
	unsigned char rdval;

    switch (ctrl->id) {
	case V4L2_CID_CHD_SETREG:	// add by liangyh 设置sensor 寄存器的值
		if(reg == 0xFFFF){
			s_ctrl_gain_handle(client, wtval);
			break;
		}
		else{
			printk("[sc031gs s_ctrl] reg_addr: %#x data: %x\n", reg, wtval);
			return sc031gs_write_reg(client, reg, wtval);
		}
	case V4L2_CID_CHD_GETREG:	// add by liangyh 获取sensor 寄存器的值
		rdval = sc031gs_read_reg(client, reg);
		ctrl->value = rdval;
		printk("[sc031gs g_ctrl] reg_addr: %#x data: %x\n", reg, rdval);
		return 0;
	case V4L2_CID_GAIN:
		s_ctrl_gain_handle(client, (ctrl->value & 0xFF));
		break;
    case V4L2_CID_EXPOSURE_AUTO:
        dev_err(&client->dev,"V4L2_CID_EXPOSURE_AUTO.\n");
        ret = s_ctrl_auto_exposure_handle(client, ctrl->value);
        if (ret < 0) {
            dev_err(&client->dev,"Failed to set exposure mode.\n");
        }
        break;
    case V4L2_CID_EXPOSURE:
        dev_err(&client->dev,"V4L2_CID_EXPOSURE.\n");
        ret = s_ctrl_exposure_handle(client,ctrl->value);
        if (ret < 0) {
            dev_err(&client->dev,"Failed to set exposure\n");
        }
        break;

    case V4L2_CID_VFLIP:
        dev_err(&client->dev,"V4L2_CID_VFLIP.\n");
        ret = s_ctrl_mirror_vflip(client, ctrl->value);
        if (ret < 0) {
            dev_err(&client->dev,"Failed to set h mirror.\n");
        }
        break;

    case V4L2_CID_HFLIP:
        dev_err(&client->dev,"V4L2_CID_HFLIP.\n");
        ret = s_ctrl_mirror_hflip(client, ctrl->value);
        if (ret < 0) {
            dev_err(&client->dev,"Failed to set h mirror.\n");
        }
        break;

    default:
        dev_err(&client->dev, "no V4L2 CID: 0x%x ", ctrl->id);
        return -EINVAL;
    }

    return ret;
}

static void set_ctrl_mode(struct sc031gs_priv *priv, unsigned int cid, unsigned int mode)
{
    switch (cid) {
	case V4L2_CID_GAIN:
		priv->gain = mode;
		break;
    case V4L2_CID_EXPOSURE_AUTO:
        priv->exposure_mode = mode;
        break;
    case V4L2_CID_EXPOSURE:
        priv->exposure = mode;
        break;
    case V4L2_CID_VFLIP:
        priv->flag_vflip = mode ? 1 : 0;
        break;
    case V4L2_CID_HFLIP:
        priv->flag_hflip = mode ? 1 : 0;
        break;
    default:
        break;
    }
}

static int sc031gs_handler_s_ctrl(struct v4l2_ctrl *ctrl)
{
    int ret;
    struct v4l2_control control;
    struct sc031gs_priv *priv = container_of(ctrl->handler, struct sc031gs_priv, ctrl_handler);
    struct v4l2_subdev *sd = &priv->subdev;
    struct i2c_client  *client = v4l2_get_subdevdata(sd);

    dev_info(&client->dev, "VIDIOC_S_CTRL id=%x  value=%d\n", ctrl->id, ctrl->val);
    control.id    = ctrl->id;
    control.value = ctrl->val;

    ret = sc031gs_s_ctrl_do(client, &control);
    if (ret < 0) {
        dev_err(&client->dev, "Set ctrl error!\n");
        return -1;
    }

	// return value to user  --add by liangyh start
	if(ctrl->id == V4L2_CID_CHD_SETREG || ctrl->id == V4L2_CID_CHD_GETREG){
		ctrl->val = control.value;
	}
	// add by liangyh end
	
    set_ctrl_mode(priv, control.id, control.value);
    return 0;
}



/* Select the nearest higher resolution for capture */
static const struct sc031gs_win_size *sc031gs_select_win(u32 *width, u32 *height)
{
    int i, default_size = ARRAY_SIZE(sc031gs_supported_win_sizes) - 1;

    for (i = 0; i < ARRAY_SIZE(sc031gs_supported_win_sizes); i++) {
        if (sc031gs_supported_win_sizes[i].width  >= *width &&
            sc031gs_supported_win_sizes[i].height >= *height) {
            *width = sc031gs_supported_win_sizes[i].width;
            *height = sc031gs_supported_win_sizes[i].height;
            return &sc031gs_supported_win_sizes[i];
        }
    }

    *width = sc031gs_supported_win_sizes[default_size].width;
    *height = sc031gs_supported_win_sizes[default_size].height;
    return &sc031gs_supported_win_sizes[default_size];
}

static int sc031gs_sensor_init(struct v4l2_subdev *sd)
{
    int ret;
    struct i2c_client  *client = v4l2_get_subdevdata(sd);

    /* initialize the sensor with default data */
    ret = sc031gs_write_array(client, sc031gs_init_regs);
    if (ret < 0) {
        dev_err(&client->dev, "%s: Error %d", __func__, ret);
        return -1;
    }

//	sc031gs_read_array(client, sc031gs_init_regs);
	
    dev_info(&client->dev, "%s: Init default", __func__);
	
    return 0;
}


static int sc031gs_g_fmt(struct v4l2_subdev *sd,
                 struct v4l2_mbus_framefmt *mf)
{
    struct i2c_client  *client = v4l2_get_subdevdata(sd);
    struct sc031gs_priv *priv   = to_sc031gs(client);

    mf->width      = priv->win->width;
    mf->height     = priv->win->height;
    mf->code       = priv->cfmt_code;

    mf->colorspace = V4L2_COLORSPACE_JPEG;
    mf->field      = V4L2_FIELD_NONE;

    return 0;
}

static int sc031gs_s_fmt(struct v4l2_subdev *sd,
                 struct v4l2_mbus_framefmt *mf)
{
    /* current do not support set format, use unify format yuv422i */
    int ret;
    struct i2c_client  *client = v4l2_get_subdevdata(sd);
    struct sc031gs_priv *priv   = to_sc031gs(client);

    priv->win = sc031gs_select_win(&mf->width, &mf->height);
    /* set size win */
    ret = sc031gs_write_array(client, priv->win->regs);
    if (ret < 0) {
        dev_err(&client->dev, "%s: Error\n", __func__);
        return ret;
    }

    memcpy(&priv->mf, mf, sizeof(struct v4l2_mbus_framefmt));
	
    return 0;
}

static int sc031gs_try_fmt(struct v4l2_subdev *sd,
                   struct v4l2_mbus_framefmt *mf)
{
    const struct sc031gs_win_size *win;
    struct i2c_client *client = v4l2_get_subdevdata(sd);
    /*
     * select suitable win
     */
    win = sc031gs_select_win(&mf->width, &mf->height);

    if(mf->field == V4L2_FIELD_ANY) {
        mf->field = V4L2_FIELD_NONE;
    }
#if 0
	else if (mf->field != V4L2_FIELD_NONE) {
        dev_err(&client->dev, "Field type invalid.\n");
        return -ENODEV;
    }
#endif
    switch (mf->code) {
    case V4L2_MBUS_FMT_Y8_1X8:
 /*	case V4L2_MBUS_FMT_YUYV8_2X8:
    case V4L2_MBUS_FMT_YUYV8_1_5X8:
    case V4L2_MBUS_FMT_JZYUYV8_1_5X8: */
        mf->colorspace = V4L2_COLORSPACE_JPEG;
        break;

    default:
        mf->code = V4L2_MBUS_FMT_YUYV8_2X8;
        break;
    }

    return 0;
}

static int sc031gs_enum_fmt(struct v4l2_subdev *sd, unsigned int index,
                            enum v4l2_mbus_pixelcode *code)
{
    if (index >= ARRAY_SIZE(sc031gs_codes))
        return -EINVAL;

    *code = sc031gs_codes[index];
    return 0;
}


static int sc031gs_video_probe(struct i2c_client *client)
{
	unsigned char retval_high = 0, retval_low = 0;
	int ret = 0;

    struct soc_camera_subdev_desc *ssdd = soc_camera_i2c_to_desc(client);

    ret = soc_camera_power_on(&client->dev, ssdd);
    if (ret < 0) {
        return ret;
    }

	/* check and show product ID and manufacturer ID */
	retval_high = sc031gs_read_reg(client, CHIP_ID_HIGH_REG);
	if (retval_high != CHIP_ID_HIGH) {
		dev_err(&client->dev, "read sensor %s chip_id high %x is error\n", client->name, retval_high);
		return -1;
	}
	retval_low = sc031gs_read_reg(client, CHIP_ID_LOW_REG);
	if (retval_low != CHIP_ID_LOW) {
		dev_err(&client->dev, "read sensor %s chip_id low %x is error\n", client->name, retval_low);
		return -1;
	}
	dev_info(&client->dev, "read sensor %s id high:0x%x,low:%x successed!\n", client->name, retval_high, retval_low);

	soc_camera_power_off(&client->dev, ssdd);

    return 0;
}

static int sc031gs_s_power(struct v4l2_subdev *sd, int on)
{
    int ret;
    struct v4l2_ctrl* ctrl;
    struct i2c_client *client = v4l2_get_subdevdata(sd);
    struct sc031gs_priv *priv   = to_sc031gs(client);
    struct soc_camera_subdev_desc *ssdd = soc_camera_i2c_to_desc(client);

    dev_info(&client->dev,"s_power %s\n", on?"on":"off");

    ret = soc_camera_set_power(&client->dev, ssdd, on);

    if (on) {
        ret = sc031gs_sensor_init(sd);
        if (ret < 0) {
            dev_err(&client->dev, "Failed to init sensor.\n");
            soc_camera_set_power(&client->dev, ssdd, !on);
            return -1;
        }
    } 
	else {
        ctrl = v4l2_ctrl_find(priv->subdev.ctrl_handler,
                              V4L2_CID_EXPOSURE_AUTO);
        if (ctrl) {
            priv->exposure_mode = V4L2_EXPOSURE_AUTO;
            ctrl->cur.val       = V4L2_EXPOSURE_AUTO;
        }

        ctrl = v4l2_ctrl_find(priv->subdev.ctrl_handler,
                              V4L2_CID_EXPOSURE);
        if (ctrl) {
            priv->exposure      = 0;
            ctrl->cur.val       = 0;
        }

        ctrl = v4l2_ctrl_find(priv->subdev.ctrl_handler,
                              V4L2_CID_VFLIP);
        if (ctrl) {
            priv->flag_vflip     = 0;
            ctrl->cur.val        = 0;
        }

        ctrl = v4l2_ctrl_find(priv->subdev.ctrl_handler,
                              V4L2_CID_HFLIP);
        if (ctrl) {
            priv->flag_hflip     = 0;
            ctrl->cur.val        = 0;
        }
    }
    priv->power_en = on;
    return 0;
}

static int sc031gs_g_mbus_config(struct v4l2_subdev *sd,
                          struct v4l2_mbus_config *cfg)
{

    struct i2c_client *client = v4l2_get_subdevdata(sd);
    struct soc_camera_subdev_desc *ssdd = soc_camera_i2c_to_desc(client);

    cfg->flags = V4L2_MBUS_PCLK_SAMPLE_RISING | V4L2_MBUS_MASTER |
                 V4L2_MBUS_VSYNC_ACTIVE_HIGH  | V4L2_MBUS_HSYNC_ACTIVE_HIGH |
                 V4L2_MBUS_DATA_ACTIVE_HIGH;
    cfg->type  = V4L2_MBUS_PARALLEL;

    cfg->flags = soc_camera_apply_board_flags(ssdd, cfg);

    return 0;
}


static int sc031gs_enum_framesizes(struct v4l2_subdev *sd,
                          struct v4l2_mbus_config *cfg)
{
	return 0;
}


static int sc031gs_enum_frameintervals(struct v4l2_subdev *sd,
                          struct v4l2_mbus_config *cfg)
{
	return 0;
}

static struct v4l2_subdev_core_ops sc031gs_subdev_core_ops = {
    .s_power      = sc031gs_s_power,
    .g_chip_ident = sc031gs_g_chip_ident,
};

static struct v4l2_subdev_video_ops sc031gs_subdev_video_ops = {
    .s_stream      = sc031gs_s_stream,
    .g_mbus_fmt    = sc031gs_g_fmt,
    .s_mbus_fmt    = sc031gs_s_fmt,
    .try_mbus_fmt  = sc031gs_try_fmt,
    .enum_mbus_fmt = sc031gs_enum_fmt,
    .g_mbus_config = sc031gs_g_mbus_config,
    .enum_framesizes = sc031gs_enum_framesizes,
    .enum_frameintervals = sc031gs_enum_frameintervals,
};

static struct v4l2_subdev_ops sc031gs_subdev_ops = {
    .core   = &sc031gs_subdev_core_ops,
    .video  = &sc031gs_subdev_video_ops,
};


static struct v4l2_ctrl_ops sc031gs_v4l2_ctrl_ops = {
    .g_volatile_ctrl = sc031gs_handler_g_volatile_ctrl,
//	.try_ctrl		 = sc031gs_handler_try_ctrl,
    .s_ctrl          = sc031gs_handler_s_ctrl,
};

static int sc031gs_ctrl_handler_init(struct sc031gs_priv *priv)
{
    struct v4l2_ctrl* ctrl;
    struct i2c_client *client = v4l2_get_subdevdata(&priv->subdev);

    v4l2_ctrl_handler_init(priv->subdev.ctrl_handler, 16);

    ctrl = v4l2_ctrl_new_std(priv->subdev.ctrl_handler,
                      &sc031gs_v4l2_ctrl_ops, V4L2_CID_CHD_GETREG, 0, 0xFFFFFF, 1, 0);
    if (!ctrl) {
        dev_err(&client->dev, "Failed to add ctrl: V4L2_CID_CHD_GETREG\n");
    }

    ctrl = v4l2_ctrl_new_std(priv->subdev.ctrl_handler,
                      &sc031gs_v4l2_ctrl_ops, V4L2_CID_CHD_SETREG, 0, 0xFFFFFF, 1, 0);
    if (!ctrl) {
        dev_err(&client->dev, "Failed to add ctrl: V4L2_CID_CHD_SETREG\n");
    }

    ctrl = v4l2_ctrl_new_std(priv->subdev.ctrl_handler,
                      &sc031gs_v4l2_ctrl_ops, V4L2_CID_GAIN, 0, 63, 1, 1);
    if (!ctrl) {
        dev_err(&client->dev, "Failed to add ctrl: V4L2_CID_GAIN\n");
    }

    ctrl = v4l2_ctrl_new_std(priv->subdev.ctrl_handler,
                      &sc031gs_v4l2_ctrl_ops, V4L2_CID_HFLIP, 0, 1, 1, 0);
    if (!ctrl) {
        dev_err(&client->dev, "Failed to add ctrl: V4L2_CID_HFLIP\n");
    }

    ctrl = v4l2_ctrl_new_std(priv->subdev.ctrl_handler,
                      &sc031gs_v4l2_ctrl_ops, V4L2_CID_VFLIP, 0, 1, 1, 0);
    if (!ctrl) {
        dev_err(&client->dev, "Failed to add ctrl: V4L2_CID_VFLIP\n");
    }

    ctrl = v4l2_ctrl_new_std_menu(priv->subdev.ctrl_handler,
                      &sc031gs_v4l2_ctrl_ops, V4L2_CID_EXPOSURE_AUTO,
                      V4L2_EXPOSURE_APERTURE_PRIORITY, 0, V4L2_EXPOSURE_AUTO);
    if (!ctrl) {
        dev_err(&client->dev, "Failed to add ctrl: V4L2_CID_EXPOSURE_AUTO\n");
    }

    ctrl = v4l2_ctrl_new_std(priv->subdev.ctrl_handler,
                      &sc031gs_v4l2_ctrl_ops, V4L2_CID_EXPOSURE, 0, 4096, 1, 0);
    if (!ctrl) {
        dev_err(&client->dev, "Failed to add ctrl: V4L2_CID_EXPOSURE\n");
    }

    return 0;
}

/*
 * i2c_driver functions
 */


static void resume_handle(struct work_struct *data)
{
    int ret;
    struct v4l2_ctrl ctrl;
    struct sc031gs_priv* priv =
                    container_of(data, struct sc031gs_priv, resume_work);
    struct i2c_client  *client = v4l2_get_subdevdata(&priv->subdev);

    ret = sc031gs_sensor_init(&priv->subdev);
    if (ret < 0) {
        dev_err(&client->dev, "Failed to init sensor.\n");
    }

    ret = sc031gs_s_fmt(&priv->subdev, &priv->mf);
    if (ret < 0) {
        dev_err(&client->dev, "Failed to s sensor fmt.\n");
    }

    ctrl.handler = &priv->ctrl_handler;

    if (priv->exposure_mode != V4L2_EXPOSURE_AUTO) {
        ctrl.id  = V4L2_CID_EXPOSURE_AUTO;
        ctrl.val = priv->exposure_mode;
        sc031gs_handler_s_ctrl(&ctrl);
        if (priv->exposure_mode == V4L2_EXPOSURE_MANUAL) {
            ctrl.id  = V4L2_CID_EXPOSURE;
            ctrl.val = priv->exposure;
            sc031gs_handler_s_ctrl(&ctrl);
        }
    }

    if (priv->flag_vflip) {
        ctrl.id  = V4L2_CID_VFLIP;
        ctrl.val = priv->flag_vflip;
        sc031gs_handler_s_ctrl(&ctrl);
    }

    if (priv->flag_hflip) {
        ctrl.id  = V4L2_CID_HFLIP;
        ctrl.val = priv->flag_hflip;
        sc031gs_handler_s_ctrl(&ctrl);
    }

    sc031gs_stream_on(priv);
}



static int sc031gs_probe(struct i2c_client *client,
                  const struct i2c_device_id *did)
{
    int ret;
    struct sc031gs_priv *priv;
    struct soc_camera_subdev_desc *ssdd = soc_camera_i2c_to_desc(client);
    struct i2c_adapter *adapter = to_i2c_adapter(client->dev.parent);
    int default_wight  = DEFAULT_WIDTH;
    int default_height = DEFAULT_HEIGHT;

    dev_info(&client->dev, "Probe ...\n");

    if (!ssdd) {
        dev_err(&client->dev, "sc031gs: missing platform data!\n");
        return -EINVAL;
    }

    if(!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE | I2C_FUNC_SMBUS_BYTE_DATA)) {
        dev_err(&client->dev, "client not i2c capable\n");
        return -ENODEV;
    }

    priv = kzalloc(sizeof(struct sc031gs_priv), GFP_KERNEL);
    if (!priv) {
        dev_err(&adapter->dev, "Failed to allocate memory for private data!\n");
        return -ENOMEM;
    }

    priv->win                 = sc031gs_select_win(&default_wight, &default_height);
    priv->cfmt_code           = V4L2_MBUS_FMT_Y8_1X8; //only y
    priv->subdev.ctrl_handler = &priv->ctrl_handler;

    priv->power_en            = 0;
	priv->gain				  = 1;
    priv->exposure_mode       = V4L2_EXPOSURE_AUTO;
    priv->flag_vflip          = 0;
    priv->flag_hflip          = 0;

    ret = sc031gs_video_probe(client);
    if (ret) {
        kfree(priv);
        return -1;
    }

    sc031gs_ctrl_handler_init(priv);
    v4l2_i2c_subdev_init(&priv->subdev, client, &sc031gs_subdev_ops);

    INIT_WORK(&priv->resume_work, resume_handle);

    dev_info(&client->dev, "Probe successed\n");
    return 0;
}

static int sc031gs_remove(struct i2c_client *client)
{
    struct sc031gs_priv *priv = to_sc031gs(client);

    v4l2_ctrl_handler_free(&priv->ctrl_handler);
    kfree(priv);

    return 0;
}

static int sc031gs_suspend(struct i2c_client *client, pm_message_t mesg)
{
    return 0;
}

static int sc031gs_resume(struct i2c_client *client)
{
    int ret;
    struct sc031gs_priv *priv = to_sc031gs(client);
    struct soc_camera_subdev_desc *ssdd = soc_camera_i2c_to_desc(client);

    if (!priv->power_en) {
        return 0;
    }

    ret = soc_camera_set_power(&client->dev, ssdd, priv->power_en);
    if (ret < 0) {
        dev_err(&client->dev, "Failed to set power.\n");
        return -1;
    }

    schedule_work(&priv->resume_work);

    return 0;
}

static const struct i2c_device_id sc031gs_id[] = {
    { "sc031gs",  0 },
    { }
};


MODULE_DEVICE_TABLE(i2c, sc031gs_id);

static struct i2c_driver sc031gs_i2c_driver = {
    .driver = {
        .name = "sc031gs",
    },
    .suspend  = sc031gs_suspend,
    .resume   = sc031gs_resume,
    .probe    = sc031gs_probe,
    .remove   = sc031gs_remove,
    .id_table = sc031gs_id,
};

/*
 * Module functions
 */
static int __init sc031gs_module_init(void)
{
    return i2c_add_driver(&sc031gs_i2c_driver);
}

static void __exit sc031gs_module_exit(void)
{
    i2c_del_driver(&sc031gs_i2c_driver);
}

module_init(sc031gs_module_init);
module_exit(sc031gs_module_exit);

MODULE_DESCRIPTION("camera sensor sc031gs driver");
MODULE_AUTHOR("qipengzhen <aric.pzqi@ingenic.com>,Monk <rongjin.su@ingenic.com>");
MODULE_LICENSE("GPL");
