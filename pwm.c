#include "pwm.h"
#include <bsp.h>
#include <stdio.h>
#include <stdlib.h>
#include <rtems/shell.h>
#include <bsp/lm3s69xx.h>
#include <bsp/syscon.h>
#include "lm3s69xx-pwm.h"

#define PWM_DIV_SETTING 4
#define PWM_FREQUENCY (LM3S69XX_SYSTEM_CLOCK / PWM_DIV_SETTING)
#define US_TO_PWM_TICKS(usecs) ((((uint64_t)PWM_FREQUENCY) * (usecs) / 1000000) - 1)

#define PWM_PERIOD_US 5000 // 200 Hz
#define PWM_WIDTH_NEUTRAL 1500 // Neutral position for servos
#define PWM_WIDTH_RANGE 1000

void set_width(unsigned int width)
{
  volatile lm3s69xx_pwm *pwm = LM3S69XX_PWM;
  volatile lm3s69xx_pwm_channel *chan0 = &pwm->chans[0];

  chan0->cmpa = US_TO_PWM_TICKS(PWM_PERIOD_US - width);
}

static int main_pwm(int argc, char **argv)
{
  if (argc != 2) {
    printf("%s <usecs>\n", argv[0]);
    return 1;
  }

  unsigned int w = strtoul(argv[1], NULL, 0);
  set_width(w);
  return 0;
}

rtems_shell_cmd_t Shell_PWM_Command = {
  "pwm",
  "pwm <usecs>",
  "user",
  main_pwm,
  NULL,
  NULL
};

void init_pwms(void)
{
  printf("Initializing PWM\n");
  lm3s69xx_syscon_enable_pwm_clock(true);
  lm3s69xx_syscon_set_pwmdiv(SYSCONRCC_PWMDIV_DIV4_VAL);

  volatile lm3s69xx_pwm *pwm = LM3S69XX_PWM;
  volatile lm3s69xx_pwm_channel *chan0 = &pwm->chans[0];

  chan0->ctl = 0; // FIXME
  chan0->gena = 0x8c; // FIXME
  chan0->load = US_TO_PWM_TICKS(PWM_PERIOD_US);
  chan0->cmpa = US_TO_PWM_TICKS((PWM_PERIOD_US - PWM_WIDTH_NEUTRAL));
  chan0->ctl = PWMCTL_ENABLE;
  pwm->enable = 1;
}
