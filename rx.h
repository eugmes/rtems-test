#ifndef RX_H
#define RX_H
#include <rtems.h>
#include <rtems/timespec.h>

#define RX_NUM_CHANNELS 5

struct rx_state {
   struct timespec last_update;
   long values[RX_NUM_CHANNELS];
};

void rx_init(void);
void rx_get_state(struct rx_state *state);

#endif
