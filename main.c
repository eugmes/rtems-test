#include <stdio.h>
#include <inttypes.h>
#include <rtems.h>
#include <rtems/shell.h>
#include <bsp.h>
#include <bsp/io.h>
#include <bsp/ssi.h>
#include <rtems/libi2c.h>
#include "l3g4200d.h"
#include "gyroscope.h"
#include "pwm.h"
#include <rtems/monitor.h>
#include <bsp/bootcard.h>
#include "rx.h"

#define RED_LED_PIN LM3S69XX_GPIO_PIN(LM3S69XX_PORT_D, 7)
#define GREEN_LED_PIN LM3S69XX_GPIO_PIN(LM3S69XX_PORT_D, 6)

#define LM3S69XX_PIN_PWM(port, pin)               \
  {                                               \
    .pin_first = LM3S69XX_GPIO_PIN(port, pin),    \
    .pin_last = LM3S69XX_GPIO_PIN(port, pin),     \
    .digital = LM3S69XX_GPIO_DIGITAL_ENABLE,      \
    .alternate = LM3S69XX_GPIO_AF_ENABLE,         \
    .analog = LM3S69XX_GPIO_ANALOG_DISABLE,       \
    .dir = LM3S69XX_GPIO_DIRECTION_OUTPUT,        \
    .otype = LM3S69XX_GPIO_OTYPE_PUSH_PULL,       \
    .drive = LM3S69XX_GPIO_DRIVE_2MA,             \
    .pull = LM3S69XX_GPIO_NO_PULL,                \
    .slr = LM3S69XX_GPIO_NO_SLEW_RATE_CONTROL     \
  }

#define LM3S69XX_PIN_INPUT(port, pin)             \
  {                                               \
    .pin_first = LM3S69XX_GPIO_PIN(port, pin),    \
    .pin_last = LM3S69XX_GPIO_PIN(port, pin),     \
    .digital = LM3S69XX_GPIO_DIGITAL_ENABLE,      \
    .alternate = LM3S69XX_GPIO_AF_DISABLE,        \
    .analog = LM3S69XX_GPIO_ANALOG_DISABLE,       \
    .dir = LM3S69XX_GPIO_DIRECTION_INPUT,         \
    .otype = LM3S69XX_GPIO_OTYPE_PUSH_PULL,       \
    .drive = LM3S69XX_GPIO_DRIVE_2MA,             \
    .pull = LM3S69XX_GPIO_PULL_DOWN,              \
    .slr = LM3S69XX_GPIO_NO_SLEW_RATE_CONTROL     \
  }

static const lm3s69xx_gpio_config gpio_config[] = {
  LM3S69XX_PIN_LED(LM3S69XX_PORT_D, 6),
  LM3S69XX_PIN_LED(LM3S69XX_PORT_D, 7),

  LM3S69XX_PIN_INPUT(LM3S69XX_PORT_D, 3),
  LM3S69XX_PIN_INPUT(LM3S69XX_PORT_F, 5),
  LM3S69XX_PIN_INPUT(LM3S69XX_PORT_A, 6),
  LM3S69XX_PIN_INPUT(LM3S69XX_PORT_A, 7),

  LM3S69XX_PIN_PWM(LM3S69XX_PORT_F, 0), // PWM0
  LM3S69XX_PIN_PWM(LM3S69XX_PORT_F, 1), // PWM1
  LM3S69XX_PIN_PWM(LM3S69XX_PORT_H, 0), // PWM2
  LM3S69XX_PIN_PWM(LM3S69XX_PORT_H, 1), // PWM3
  LM3S69XX_PIN_PWM(LM3S69XX_PORT_F, 2), // PWM4
  LM3S69XX_PIN_PWM(LM3S69XX_PORT_F, 3), // PWM5
  LM3S69XX_PIN_PWM(LM3S69XX_PORT_G, 6), // PWM6
  LM3S69XX_PIN_PWM(LM3S69XX_PORT_G, 7), // PWM7
  {
    .pin_first = SSI_SS_PIN,
    .pin_last = SSI_SS_PIN,
    .digital = LM3S69XX_GPIO_DIGITAL_DISABLE,
    .alternate = LM3S69XX_GPIO_AF_DISABLE,
    .analog = LM3S69XX_GPIO_ANALOG_DISABLE,
    .dir = LM3S69XX_GPIO_DIRECTION_OUTPUT,
    .otype = LM3S69XX_GPIO_OTYPE_PUSH_PULL,
    .drive = LM3S69XX_GPIO_DRIVE_2MA,
    .pull = LM3S69XX_GPIO_NO_PULL,
    .slr = LM3S69XX_GPIO_NO_SLEW_RATE_CONTROL
  }
};

void rtems_monitor_reset_cmd(int argc, char **argv, const rtems_monitor_command_arg_t*dummy1, bool dummy2)
{
  bsp_reset();
}

static int main_led(int argc, char **argv)
{
  if (argc != 3) {
    printf("%s (green|red) (on|off)\n", argv[0]);
    return 1;
  }

  int pin;

  if (!strcmp(argv[1], "red"))
    pin = RED_LED_PIN;
  else if (!strcmp(argv[1], "green"))
    pin = GREEN_LED_PIN;
  else
    return 1;

  bool state;

  if (!strcmp(argv[2], "on"))
    state = false;
  else if (!strcmp(argv[2], "off"))
    state = true;
  else
    return 1;

  lm3s69xx_gpio_set_pin(pin, state);
  return 0;
}

rtems_shell_cmd_t Shell_LED_Command = {
  "led",
  "led (green|red) (on|off)",
  "user",
  main_led,
  NULL,
  NULL
};

static void pre_boot_delay(int seconds)
{
  time_t initial = rtems_clock_get_uptime_seconds();

  while (rtems_clock_get_uptime_seconds() - initial < 10)
      /* wait */;
}

void set_width(unsigned int width);

rtems_task rx_demo_task(rtems_task_argument arg)
{
  for (;;) {
    struct rx_state st;

    rx_get_state(&st);
#if 0
    printf("\r1: % 5ld 2: % 5ld 3: % 5ld 4: % 5ld 5: % 5ld",
        (st.values[0] - 1500000L) / 1000,
        (st.values[1] - 1500000L) / 1000,
        (st.values[2] - 1500000L) / 1000,
        (st.values[3] - 1500000L) / 1000,
        (st.values[4] - 1500000L) / 1000);
    fflush(stdout);
#endif
    long pwm = st.values[0] / 1000;
    if ((pwm > 400) && (pwm < 2300))
        set_width(pwm);

    rtems_task_wake_after(10);
  }
}


rtems_task Init(rtems_task_argument arg)
{
  printf("Pre-boot delay...\n");

  pre_boot_delay(10);

  printf("Initializing the test...\n");

  lm3s69xx_gpio_set_config_array(gpio_config,
      sizeof(gpio_config) / sizeof(gpio_config[0]));

  rtems_libi2c_initialize();
  int bus_no = rtems_libi2c_register_bus(NULL, lm3s69xx_ssi_1);

  gyroscope_init(bus_no);
  init_pwms();
  rx_init();
  printf("done\n");

  lm3s69xx_gpio_set_pin(RED_LED_PIN, true);
  lm3s69xx_gpio_set_pin(GREEN_LED_PIN, true);

  rtems_name rx_demo_id;

  rtems_task_create(rtems_build_name('R', 'X', ' ', ' '), 1,
      RTEMS_MINIMUM_STACK_SIZE, RTEMS_DEFAULT_MODES,
      RTEMS_DEFAULT_ATTRIBUTES, &rx_demo_id);
  rtems_task_start(rx_demo_id, &rx_demo_task, 0);

  rtems_shell_main_loop(NULL);

  rtems_task_delete(RTEMS_SELF);
}
