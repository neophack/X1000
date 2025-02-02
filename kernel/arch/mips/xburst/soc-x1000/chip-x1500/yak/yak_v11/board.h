#ifndef __BOARD_H__
#define __BOARD_H__

#include <gpio.h>
#include <soc/gpio.h>


/*
 * Keys
 */
/* #define GPIO_HOME_KEY                GPIO_PD(18) */
/* #define ACTIVE_LOW_HOME              1 */

/* #define GPIO_VOLUMEDOWN_KEY          GPIO_PD(18) */
/* #define ACTIVE_LOW_VOLUMEDOWN        0 */

#define GPIO_ENDCALL_KEY                GPIO_PB(31)
#define ACTIVE_LOW_ENDCALL              0



/*
 * WiFi
 */
#ifdef CONFIG_BCMDHD_1_141_66

#define GPIO_WIFI_RST_N                 GPIO_PC(17)
#define GPIO_WIFI_WAKE                  GPIO_PC(16)

/*
 * Bluetooth
 */
#define BLUETOOTH_UPORT_NAME            "ttyS0"
#define GPIO_BT_REG_ON                  GPIO_PC(18)
#define GPIO_BT_WAKE                    GPIO_PC(20)
#define GPIO_BT_INT                     GPIO_PC(19)
#define GPIO_BT_UART_RTS                GPIO_PC(13)

#endif

#ifdef CONFIG_HALLEY2_MINI_CORE_V10

//notice:here there is a problem,but slove in the future!!!
#define GPIO_WIFI_WAKEUP                GPIO_PC(16)
#define GPIO_WIFI_RST_N                 GPIO_PC(17)

#endif

/*
 * MSC0
 */
#define GPIO_SD0_CD_N                   -1

/*
 * WiFi LED
 */
#ifdef CONFIG_LEDS_GPIO
#define	WL_LED_R                        -1
#define	WL_LED_G                        -1
#define	WL_LED_B                        -1
#endif

/*
 * SPI
 */
#ifdef CONFIG_SPI_GPIO
#define GPIO_SPI_SCK                    GPIO_PA(26)
#define GPIO_SPI_MOSI                   GPIO_PA(29)
#define GPIO_SPI_MISO                   GPIO_PA(28)
#endif

#if defined(CONFIG_JZ_SPI) || defined(CONFIG_JZ_SFC)
#define SPI_CHIP_ENABLE                 GPIO_PA(27)
#endif

/*
 * USB
 */
/*#define GPIO_USB_ID            	    GPIO_PC(21)*/
#define GPIO_USB_ID_LEVEL               LOW_ENABLE
#ifdef CONFIG_BOARD_HAS_NO_DETE_FACILITY
#define GPIO_USB_DETE                   -1 /*GPIO_PC(22)*/
#define GPIO_USB_DETE_LEVEL             LOW_ENABLE
#else
#define GPIO_USB_DETE                   GPIO_PD(3)
#define GPIO_USB_DETE_LEVEL             LOW_ENABLE
#endif
#define GPIO_USB_DRVVBUS                GPIO_PB(25)
#define GPIO_USB_DRVVBUS_LEVEL          HIGH_ENABLE

#ifdef CONFIG_JZ_USB_REMOTE_WKUP_PIN
#define GPIO_USB_SWITCH                 GPIO_PD(2)
#define GPIO_USB_SWITCH_LEVEL           HIGH_ENABLE

#define GPIO_USB_REMOTE_WKUP_DETE       GPIO_PD(1)
#define GPIO_USB_REMOTE_WKUP_DETE_LEVEL LOW_ENABLE
#endif

/*
 * Audio
 */
#define GPIO_HP_MUTE                    -1  /*hp mute gpio*/
#define GPIO_HP_MUTE_LEVEL              -1  /*vaild level*/

#ifdef CONFIG_HALLEY2_MINI_CORE_V10
#define GPIO_SPEAKER_EN                 GPIO_PC(23)         /*speaker enable gpio*/
#define GPIO_SPEAKER_EN_LEVEL           0
#else
#define GPIO_SPEAKER_EN                 -1         /*speaker enable gpio*/
#define GPIO_SPEAKER_EN_LEVEL           0
#endif

#define GPIO_SPEAKER_MUTE               (-1)
#define GPIO_SPEAKER_MUTE_EN_LEVEL      (1)

#define GPIO_AMP_POWER                  (-1)
#define GPIO_AMP_POWER_LEVEL            (1)

#define GPIO_AMP_MUTE                   (-1)                /* amp mute */
#define GPIO_AMP_MUTE_LEVEL             (1)

#define GPIO_HANDSET_EN                 -1  /*handset enable gpio*/
#define GPIO_HANDSET_EN_LEVEL           -1

#define GPIO_HP_DETECT                  -1/*hp detect gpio*/
#define GPIO_HP_INSERT_LEVEL            1
#define GPIO_MIC_SELECT                 -1  /*mic select gpio*/
#define GPIO_BUILDIN_MIC_LEVEL          -1  /*builin mic select level*/
#define GPIO_MIC_DETECT                 -1
#define GPIO_MIC_INSERT_LEVEL           -1
#define GPIO_MIC_DETECT_EN              -1  /*mic detect enable gpio*/
#define GPIO_MIC_DETECT_EN_LEVEL        -1 /*mic detect enable gpio*/

#define HP_SENSE_ACTIVE_LEVEL           1
#define HOOK_ACTIVE_LEVEL               -1

/*
 * GMAC
 */
#ifdef CONFIG_JZ_MAC

#ifndef CONFIG_MDIO_GPIO

    #ifdef CONFIG_JZGPIO_PHY_RESET
    #define GMAC_PHY_PORT_GPIO          GPIO_PC(23)
    #define GMAC_PHY_ACTIVE_HIGH        1
    #define GMAC_CRLT_PORT              GPIO_PORT_B
    #define GMAC_CRLT_PORT_PINS         (0x7 << 7)
    #define GMAC_CRTL_PORT_INIT_FUNC    GPIO_FUNC_1
    #define GMAC_CRTL_PORT_SET_FUNC     GPIO_OUTPUT0
    #define GMAC_PHY_DELAYTIME          10
    #endif

#else /* CONFIG_MDIO_GPIO */

    #define MDIO_MDIO_MDC_GPIO          GPIO_PF(13)
    #define MDIO_MDIO_GPIO              GPIO_PF(14)
#endif

#endif /* CONFIG_JZ4775_MAC */

/*
 * I2C
 */
#ifndef CONFIG_I2C0_V12_JZ
#define GPIO_I2C0_SDA                   GPIO_PB(24)
#define GPIO_I2C0_SCK                   GPIO_PB(23)
#endif
#ifndef CONFIG_I2C1_V12_JZ
#define GPIO_I2C1_SDA                   GPIO_PC(27)
#define GPIO_I2C1_SCK                   GPIO_PC(26)
#endif
#ifndef CONFIG_I2C2_V12_JZ
#define GPIO_I2C2_SDA                   GPIO_PD(1)
#define GPIO_I2C2_SCK                   GPIO_PD(0)
#endif

/*
 * Sensor
 */
#ifdef CONFIG_SENSORS_BMA2X2
#define GPIO_GSENSOR_INTR               GPIO_PB(2)
#endif

/*
 * Camera
 */
#if defined(CONFIG_VIDEO_JZ_CIM_HOST_V13) || defined(CONFIG_JZ_CIM_CORE)
#define FRONT_CAMERA_INDEX                  (0)
#define BACK_CAMERA_INDEX                   (1)
#define FRONT_CAMERA_SENSOR_RESET           (-1)
#define FRONT_CAMERA_SENSOR_PWDN            GPIO_PB(21)

#ifdef CONFIG_SOC_CAMERA_OV2640
#define FRONT_CAMERA_VDD_EN                 GPIO_PB(5)
#else
#define FRONT_CAMERA_VDD_EN                 (-1)
#endif

#define FRONT_CAMERA_SENSOR_RESET_LEVEL     0
#define FRONT_CAMERA_SENSOR_PWDN_LEVEL      0
#define FRONT_CAMERA_VDD_EN_LEVEL           1

#define FRONT_CAMERA_IR_POWER_EN            (-1)
#define FRONT_CAMERA_IR_POWER_EN__LEVEL     (1)

#endif

/*
 * EFUSE
 */
#define GPIO_EFUSE_VDDQ                 (-ENODEV)/* GPIO_PB(27)	*//* EFUSE must be -ENODEV or a gpio */

/*
 * PMU (RN5T567)
 */
#ifdef CONFIG_REGULATOR_RN5T567
#define PMU_IRQ_N                       -1
#define PMU_SLP_N                       -1
#define SLP_PIN_DISABLE_VALUE           1
#endif /* CONFIG_REGULATOR_RN5T567 */

/*
 * PWM BEEPER
 */
#ifdef CONFIG_INPUT_PWM_BEEPER
#define BEEPER_PORT_PWM_ID              0
#endif

#endif /* __BOARD_H__ */
