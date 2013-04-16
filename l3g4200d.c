#include "l3g4200d.h"
#include <rtems/libi2c.h>

void l3g4200d_read_registers(rtems_device_minor_number minor, uint32_t reg, unsigned char *buf, int n)
{
  unsigned char start_byte = reg | 0xc0;

  rtems_libi2c_start_write_bytes(minor, &start_byte, 1);
  rtems_libi2c_read_bytes(minor, buf, n);
  rtems_libi2c_send_stop(minor);
}

void l3g4200d_write_registers(rtems_device_minor_number minor, uint32_t reg, const unsigned char *buf, int n)
{
  unsigned char start_byte = reg | 0x40;

  rtems_libi2c_start_write_bytes(minor, &start_byte, 1);
  rtems_libi2c_write_bytes(minor, buf, n);
  rtems_libi2c_send_stop(minor);
}
