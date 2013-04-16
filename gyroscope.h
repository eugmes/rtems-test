#ifndef GYROSCOPE_H
#define GYROSCOPE_H
#include <bsp/io.h>

#define SSI_SS_PIN LM3S3749_GPIO_PIN(LM3S3749_PORT_E, 1)

void gyroscope_init(int spi_bus_num);

#endif
