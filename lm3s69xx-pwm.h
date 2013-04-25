#ifndef LM323749_PWM_H
#define LM323749_PWM_H
#include <bsp/utility.h>

#define LM3S69XX_UART_BASE 0x40028000

#define LM3S69XX_PWM ((volatile lm3s69xx_pwm *)LM3S69XX_UART_BASE)

typedef struct {
#define PWMCTL_LATCH BSP_BIT32(18)
#define PWMCTL_MINFLTPER BSP_BIT32(17)
#define PWMCTL_FLTSRC BSP_BIT32(16)
#define PWMCTL_DBFALLUPD(val) BSP_FLD32(val, 14, 15)
#define PWMCTL_DBRISEUPD(val) BSP_FLD32(val, 12, 13)
#define PWMCTL_DBCTLUPD(val) BSP_FLD32(val, 10, 11)
#define PWMCTL_GENBUPD(val) BSP_FLD32(val, 8, 9)
#define PWMCTL_GENAUPD(val) BSP_FLD32(val, 6, 7)
#define PWMCTL_CMPBUPD BSP_BIT32(5)
#define PWMCTL_CMPAUPD BSP_BIT32(4)
#define PWMCTL_LOADUPD BSP_BIT32(3)
#define PWMCTL_DEBUG BSP_BIT32(2)
#define PWMCTL_MODE BSP_BIT32(1)
#define PWMCTL_ENABLE BSP_BIT32(0)
  uint32_t ctl;

#define PWMINTEN_TRCMPBD BSP_BIT32(13)
#define PWMINTEN_TRCMPBU BSP_BIT32(12)
#define PWMINTEN_TRCMPAD BSP_BIT32(11)
#define PWMINTEN_TRCMPAU BSP_BIT32(10)
#define PWMINTEN_TRCNTLOAD BSP_BIT32(9)
#define PWMINTEN_TRCNTZERO BSP_BIT32(8)
#define PWMINTEN_INTCMPBD BSP_BIT32(5)
#define PWMINTEN_INTCMPBU BSP_BIT32(4)
#define PWMINTEN_INTCMPAD BSP_BIT32(3)
#define PWMINTEN_INTCMPAU BSP_BIT32(2)
#define PWMINTEN_INTCNTLOAD BSP_BIT32(1)
#define PWMINTEN_INTCNTZERO BSP_BIT32(0)
  uint32_t inten;
  uint32_t ris;
  uint32_t isc;
  uint32_t load;
  uint32_t count;
  uint32_t cmpa;
  uint32_t cmpb;
  uint32_t gena;
  uint32_t genb;
  uint32_t dbct;
  uint32_t dbrise;
  uint32_t dbfall;
  uint32_t fltsrc0;
  uint32_t reserved;
  uint32_t minfiltper;
} lm3s69xx_pwm_channel;

typedef struct {
  uint32_t fltsen;
  uint32_t fltstat0;
  uint32_t reserved[30];
} lm3s69xx_pwm_fault;

typedef struct {
  uint32_t ctl;
  uint32_t sync;

  uint32_t enable;
  uint32_t invert;
  uint32_t fault;

  uint32_t inten;
  uint32_t ris;
  uint32_t isc;
  uint32_t status;

  uint32_t faultval;

  uint32_t reserved_0[6];

  lm3s69xx_pwm_channel chans[4];

  uint32_t reserved_1[432];

  lm3s69xx_pwm_fault faults[4];
} lm3s69xx_pwm;

#endif
