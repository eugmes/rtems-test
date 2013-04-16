#ifndef L3G4200D_H
#define L3G4200D_H
#include <rtems.h>

#define L3G4200D_WHO_AM_I 0x0f
#define L3G4200D_CRTL1 0x20
#define L3G4200D_CRTL2 0x21
#define L3G4200D_CRTL3 0x22
#define L3G4200D_CRTL4 0x23
#define L3G4200D_CRTL5 0x24
#define L3G4200D_REFERENCE 0x25
#define L3G4200D_OUT_TEMP 0x26
#define L3G4200D_STATUS 0x27
#define L3G4200D_OUT_X_L 0x28
#define L3G4200D_OUT_X_H 0x29
#define L3G4200D_OUT_Y_L 0x2a
#define L3G4200D_OUT_Y_H 0x2b
#define L3G4200D_OUT_Z_L 0x2c
#define L3G4200D_OUT_Z_H 0x2d
#define L3G4200D_FIFO_CTRL 0x2e
#define L3G4200D_FIFO_SRC 0x2f
#define L3G4200D_INT1_CFG 0x30
#define L3G4200D_INT1_SRC 0x31
#define L3G4200D_INT1_TSH_XH 0x32
#define L3G4200D_INT1_TSH_XL 0x33
#define L3G4200D_INT1_TSH_YH 0x34
#define L3G4200D_INT1_TSH_YL 0x35
#define L3G4200D_INT1_TSH_ZH 0x36
#define L3G4200D_INT1_TSH_ZL 0x37
#define L3G4200D_INT1_DURATION 0x38

#define L3G4200D_WHO_AM_I_VALUE 0xd3

#define L3G4200D_CRTL1_DR(val) ((val) << 6)
#define L3G4200D_CRTL1_DR_MSK  (0x3 << 6)
#define L3G4200D_CRTL1_BW(val) ((val) << 4)
#define L3G4200D_CRTL1_BW_MSK  (0x3 << 4)
#define L3G4200D_CRTL1_PD  (1 << 3)
#define L3G4200D_CRTL1_ZEN (1 << 2)
#define L3G4200D_CRTL1_YEN (1 << 1)
#define L3G4200D_CRTL1_XEN (1 << 0)

#define L3G4200D_CRTL2_HPM(val) (val << 4)
#define L3G4200D_CRTL2_HPM_MSK  (0x3 << 4)
#define L3G4200D_CRTL2_HPCF(val) (val)
#define L3G4200D_CRTL2_HPCF_MSK  0xf

#define L3G4200D_CRTL3_I1_INT1 (1 << 7)
#define L3G4200D_CRTL3_I1_BOOT (1 << 6)
#define L3G4200D_CRTL3_H_LACTIVE (1 << 5)
#define L3G4200D_CRTL3_PP_OD (1 << 4)
#define L3G4200D_CRTL3_I2_DRDY (1 << 3)
#define L3G4200D_CRTL3_I2_WTM (1 << 2)
#define L3G4200D_CRTL3_I2_ORUN (1 << 1)
#define L3G4200D_CRTL3_I2_EMPTY (1 << 0)

#define L3G4200D_CRTL4_BDU (1 << 7)
#define L3G4200D_CRTL4_BLE (1 << 6)
#define L3G4200D_CRTL4_FS(val) ((val) << 4)
#define L3G4200D_CRTL4_FS_MSK (0x3 << 4)
#define L3G4200D_CRTL4_ST(val) ((val) << 1)
#define L3G4200D_CRTL4_ST_MSK (0x3 << 1)
#define L3G4200D_CRTL4_SIM 1

#define L3G4200D_CRTL5_BOOT (1 << 7)
#define L3G4200D_CRTL5_FIFO_EN (1 << 6)
#define L3G4200D_CRTL5_HPEN (1 << 4)
#define L3G4200D_CRTL5_INT1_SEL(val) (val)
#define L3G4200D_CRTL5_INT1_SEL_MSK 0xf

#define L3G4200D_STATUS_ZYXOR (1 << 7)
#define L3G4200D_STATUS_ZOR (1 << 6)
#define L3G4200D_STATUS_YOR (1 << 6)
#define L3G4200D_STATUS_XOR (1 << 6)
#define L3G4200D_STATUS_ZYXDA (1 << 7)
#define L3G4200D_STATUS_ZDA (1 << 6)
#define L3G4200D_STATUS_YDA (1 << 6)
#define L3G4200D_STATUS_XDA (1 << 6)

#ifdef __cplusplus
extern "C" {
#endif

void l3g4200d_read_registers(rtems_device_minor_number minor, uint32_t reg, unsigned char *buf, int n);
void l3g4200d_write_registers(rtems_device_minor_number minor, uint32_t reg, const unsigned char *buf, int n);

#ifdef __cplusplus
}
#endif

#endif
