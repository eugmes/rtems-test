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

#define RED_LED_PIN LM3S3749_GPIO_PIN(LM3S3749_PORT_D, 7)
#define GREEN_LED_PIN LM3S3749_GPIO_PIN(LM3S3749_PORT_D, 6)

#define LM3S3749_PIN_PWM(port, pin)               \
  {                                               \
    .pin_first = LM3S3749_GPIO_PIN(port, pin),    \
    .pin_last = LM3S3749_GPIO_PIN(port, pin),     \
    .digital = LM3S3749_GPIO_DIGITAL_ENABLE,      \
    .alternate = LM3S3749_GPIO_AF_ENABLE,         \
    .analog = LM3S3749_GPIO_ANALOG_DISABLE,       \
    .dir = LM3S3749_GPIO_DIRECTION_OUTPUT,        \
    .otype = LM3S3749_GPIO_OTYPE_PUSH_PULL,       \
    .drive = LM3S3749_GPIO_DRIVE_2MA,             \
    .pull = LM3S3749_GPIO_NO_PULL,                \
    .slr = LM3S3749_GPIO_NO_SLEW_RATE_CONTROL     \
  }

static const lm3s3749_gpio_config gpio_config[] = {
  LM3S3749_PIN_LED(LM3S3749_PORT_D, 6),
  LM3S3749_PIN_LED(LM3S3749_PORT_D, 7),

  LM3S3749_PIN_PWM(LM3S3749_PORT_F, 0), // PWM0
  LM3S3749_PIN_PWM(LM3S3749_PORT_F, 1), // PWM1
  LM3S3749_PIN_PWM(LM3S3749_PORT_H, 0), // PWM2
  LM3S3749_PIN_PWM(LM3S3749_PORT_H, 1), // PWM3
  LM3S3749_PIN_PWM(LM3S3749_PORT_F, 2), // PWM4
  LM3S3749_PIN_PWM(LM3S3749_PORT_F, 3), // PWM5
  LM3S3749_PIN_PWM(LM3S3749_PORT_G, 6), // PWM6
  LM3S3749_PIN_PWM(LM3S3749_PORT_G, 7), // PWM7
  {
    .pin_first = SSI_SS_PIN,
    .pin_last = SSI_SS_PIN,
    .digital = LM3S3749_GPIO_DIGITAL_DISABLE,
    .alternate = LM3S3749_GPIO_AF_DISABLE,
    .analog = LM3S3749_GPIO_ANALOG_DISABLE,
    .dir = LM3S3749_GPIO_DIRECTION_OUTPUT,
    .otype = LM3S3749_GPIO_OTYPE_PUSH_PULL,
    .drive = LM3S3749_GPIO_DRIVE_2MA,
    .pull = LM3S3749_GPIO_NO_PULL,
    .slr = LM3S3749_GPIO_NO_SLEW_RATE_CONTROL
  }
};

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

  lm3s3749_gpio_set_pin(pin, state);
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

rtems_task Init(rtems_task_argument arg)
{
  printf("Pre-boot delay...\n");

  pre_boot_delay(10);

  printf("Initializing the test...\n");

  lm3s3749_gpio_set_config_array(gpio_config,
      sizeof(gpio_config) / sizeof(gpio_config[0]));

  rtems_libi2c_initialize();
  int bus_no = rtems_libi2c_register_bus(NULL, lm3s3749_ssi_1);

  gyroscope_init(bus_no);
  init_pwms();
  printf("done\n");

  lm3s3749_gpio_set_pin(RED_LED_PIN, true);
  lm3s3749_gpio_set_pin(GREEN_LED_PIN, true);

  rtems_shell_main_loop(NULL);

  rtems_task_delete(RTEMS_SELF);
}
