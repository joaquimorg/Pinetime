#define CST816S_REG_DATA                0x00
#define CST816S_REG_GESTURE_ID          0x01
#define CST816S_REG_FINGER_NUM          0x02
#define CST816S_REG_XPOS_H              0x03
#define CST816S_REG_XPOS_L              0x04
#define CST816S_REG_YPOS_H              0x05
#define CST816S_REG_YPOS_L              0x06
#define CST816S_REG_BPC0H               0xB0
#define CST816S_REG_BPC0L               0xB1
#define CST816S_REG_BPC1H               0xB2
#define CST816S_REG_BPC1L               0xB3
#define CST816S_REG_POWER_MODE          0xA5
#define CST816S_REG_CHIP_ID             0xA7
#define CST816S_REG_PROJ_ID             0xA8
#define CST816S_REG_FW_VERSION          0xA9
#define CST816S_REG_MOTION_MASK         0xEC
#define CST816S_REG_IRQ_PULSE_WIDTH     0xED
#define CST816S_REG_NOR_SCAN_PER        0xEE
#define CST816S_REG_MOTION_S1_ANGLE     0xEF
#define CST816S_REG_LP_SCAN_RAW1H       0xF0
#define CST816S_REG_LP_SCAN_RAW1L       0xF1
#define CST816S_REG_LP_SCAN_RAW2H       0xF2
#define CST816S_REG_LP_SCAN_RAW2L       0xF3
#define CST816S_REG_LP_AUTO_WAKEUP_TIME 0xF4
#define CST816S_REG_LP_SCAN_TH          0xF5
#define CST816S_REG_LP_SCAN_WIN         0xF6
#define CST816S_REG_LP_SCAN_FREQ        0xF7
#define CST816S_REG_LP_SCAN_I_DAC       0xF8
#define CST816S_REG_AUTOSLEEP_TIME      0xF9
#define CST816S_REG_IRQ_CTL             0xFA
#define CST816S_REG_DEBOUNCE_TIME       0xFB
#define CST816S_REG_LONG_PRESS_TIME     0xFC
#define CST816S_REG_IOCTL               0xFD
#define CST816S_REG_DIS_AUTO_SLEEP      0xFE

#define CST816S_GESTURE_NONE          (0x00)
#define CST816S_GESTURE_SLIDE_UP      (0x01)
#define CST816S_GESTURE_SLIDE_DOWN    (0x02)
#define CST816S_GESTURE_SLIDE_LEFT    (0x03)
#define CST816S_GESTURE_SLIDE_RIGHT   (0x04)
#define CST816S_GESTURE_CLICK         (0x05)
#define CST816S_GESTURE_DOUBLE_CLICK  (0x0B)
#define CST816S_GESTURE_LONG_PRESS    (0x0C)

#define CST816S_MOTION_EN_CON_LR      (1<<2)
#define CST816S_MOTION_EN_CON_UR      (1<<1)
#define CST816S_MOTION_EN_DCLICK      (1<<0)

#define CST816S_IRQ_EN_TEST           (1<<7)
#define CST816S_IRQ_EN_TOUCH          (1<<6)
#define CST816S_IRQ_EN_CHANGE         (1<<5)
#define CST816S_IRQ_EN_MOTION         (1<<4)
#define CST816S_IRQ_ONCE_WLP          (1<<0)

#define CST816S_IOCTL_SOFT_RTS        (1<<2)
#define CST816S_IOCTL_IIC_OD          (1<<1)
#define CST816S_IOCTL_EN_1V8          (1<<0)

#define CST816S_POWER_MODE_SLEEP          (0x03)
#define CST816S_POWER_MODE_EXPERIMENTAL   (0x05)