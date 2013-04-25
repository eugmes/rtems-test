#include "rx.h"
#include <rtems.h>
#include <bsp/irq.h>
#include <bsp/lm3s69xx.h>
#include <bsp/io.h>
#include <rtems/error.h>

#define MAX_PULSE_TIME 2500000

static struct rx_state current_state;
static rtems_name rx_sem;

static void rx_irq_handler(void *arg)
{
  static struct rx_state tmp_state;
  static uint8_t next_channel = 0xff;

  struct timespec diff, ts;

  rtems_clock_get_uptime(&ts);
  rtems_timespec_subtract(&tmp_state.last_update, &ts, &diff);
  memcpy(&tmp_state, &ts, sizeof(ts));

  if ((diff.tv_sec > 0) || (diff.tv_nsec > MAX_PULSE_TIME)) {
    next_channel = 0;
  } else if (next_channel < RX_NUM_CHANNELS) {
    tmp_state.values[next_channel] = diff.tv_nsec;
    next_channel++;
    
    if (next_channel == RX_NUM_CHANNELS)
      memcpy(&current_state, &tmp_state, sizeof(current_state));
  }

  volatile lm3s69xx_gpio *portd = LM3S69XX_GPIO(LM3S69XX_PORT_D);
  portd->icr = 1U << 3;
  rtems_semaphore_release(rx_sem);
}

void rx_init(void)
{
  rtems_semaphore_create(rtems_build_name('s', 'R', 'X', ' '), 0,
      RTEMS_DEFAULT_ATTRIBUTES, 1, &rx_sem);

  rtems_status_code sc = rtems_interrupt_handler_install(LM3S69XX_IRQ_GPIO_PORT_D,
      "PORT D",
      RTEMS_INTERRUPT_UNIQUE,
      rx_irq_handler,
      NULL);

  if (sc != RTEMS_SUCCESSFUL)
    rtems_panic("failed to install handler!\n");

  rtems_interrupt_level level;
  rtems_interrupt_disable(level);
  volatile lm3s69xx_gpio *portd = LM3S69XX_GPIO(LM3S69XX_PORT_D);
  portd->im |= (1U << 3);
  rtems_interrupt_enable(level);
}

void rx_get_state(struct rx_state *state)
{
  rtems_interrupt_level level;

  rtems_interrupt_disable(level);
  memcpy(state, &current_state, sizeof(current_state));
  rtems_interrupt_enable(level);
}
