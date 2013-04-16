#include "gyroscope.h"
#include <rtems.h>
#include <rtems/error.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <bsp/io.h>
#include <rtems/libi2c.h>
#include "l3g4200d.h"

#define SSI_SS_PIN LM3S3749_GPIO_PIN(LM3S3749_PORT_E, 1)

#define NUM_OFFSET_SAMPLES 100

struct gyroscope_values {
  int16_t x;
  int16_t y;
  int16_t z;
};

struct gyroscope_data {
  int minor;
  struct gyroscope_values offset;
};

struct gyroscope_data gyro_data;

rtems_task gyroscope_task(rtems_task_argument arg)
{
  printf("gyroscope task started\n");

  for (;;) {
    rtems_task_wake_after(5 * rtems_clock_get_ticks_per_second());
  }
}

/**
 * Checks WHO_AM_I register content of L34200D gyroscope.
 */
static void check_whoami(int minor)
{
  unsigned char reg;

  l3g4200d_read_registers(minor, L3G4200D_WHO_AM_I, &reg, 1);

  if (reg != L3G4200D_WHO_AM_I_VALUE)
    rtems_panic("Invalid L3G4200 WHO AM I value");
}

/**
 * Loads default gyroscope settings for L3G4200D.
 */
static void init_l3g4200d(rtems_device_minor_number minor)
{
  unsigned char r;

  r = L3G4200D_CRTL2_HPCF(8);
  l3g4200d_write_registers(minor, L3G4200D_CRTL2, &r, 1);
  r = 0;
  l3g4200d_write_registers(minor, L3G4200D_CRTL3, &r, 1);
  r = L3G4200D_CRTL4_FS(1) | L3G4200D_CRTL4_BDU;
  l3g4200d_write_registers(minor, L3G4200D_CRTL4, &r, 1);
  r = L3G4200D_CRTL5_HPEN;
  l3g4200d_write_registers(minor, L3G4200D_CRTL5, &r, 1);
  r = L3G4200D_CRTL1_PD | L3G4200D_CRTL1_ZEN | L3G4200D_CRTL1_YEN | L3G4200D_CRTL1_XEN;
  l3g4200d_write_registers(minor, L3G4200D_CRTL1, &r, 1);
}

static void read_gyroscope_values(rtems_device_minor_number minor, struct gyroscope_values *vals)
{
  l3g4200d_read_registers(minor, L3G4200D_OUT_X_L, (unsigned char *)vals, 6);
}

static void read_gyroscope_values_polled(rtems_device_minor_number minor, struct gyroscope_values *vals)
{
  unsigned char status;

  do {
    l3g4200d_read_registers(minor, L3G4200D_STATUS, &status, 1);
  } while ((status & L3G4200D_STATUS_ZYXDA) == 0);
  
  read_gyroscope_values(minor, vals);
}

static void measure_offset(rtems_device_minor_number minor, struct gyroscope_values *vals)
{
  int32_t x_acc = 0;
  int32_t y_acc = 0;
  int32_t z_acc = 0;

  for (int i = 0; i < NUM_OFFSET_SAMPLES; i++) {
    read_gyroscope_values_polled(minor, vals);

    x_acc += vals->x;
    y_acc += vals->y;
    z_acc += vals->z;
  }

  vals->x = x_acc / NUM_OFFSET_SAMPLES;
  vals->y = y_acc / NUM_OFFSET_SAMPLES;
  vals->z = z_acc / NUM_OFFSET_SAMPLES;
}

void gyroscope_init(int spi_bus_num)
{
  printf("Initializing gyroscope...\n");

  lm3s3749_gpio_set_pin(SSI_SS_PIN, true);
  lm3s3749_gpio_digital_enable(SSI_SS_PIN, true);

  rtems_device_minor_number minor = RTEMS_LIBI2C_MAKE_MINOR(spi_bus_num, SSI_SS_PIN);

  check_whoami(minor);
  init_l3g4200d(minor);

  measure_offset(minor, &gyro_data.offset);
  printf("Gyroscope offset: %"PRId16", %"PRId16", %"PRId16"\n",
      gyro_data.offset.x, gyro_data.offset.y, gyro_data.offset.z);

  rtems_id task_id;

  rtems_task_create(rtems_build_name('G', 'Y', 'R', 'O'), 1,
      RTEMS_MINIMUM_STACK_SIZE, RTEMS_DEFAULT_MODES,
      RTEMS_DEFAULT_ATTRIBUTES, &task_id);

  rtems_task_start(task_id, gyroscope_task, 0);
}
