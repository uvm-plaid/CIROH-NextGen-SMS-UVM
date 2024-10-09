#include <SPI.h>
#include <RH_RF95.h>

#include "shared.h"

// Change this const to change board deploy to Tx or Rx.
const vtable_t tab = tx_vtable;

void setup() {
  tab.setup(rf95);
}

void loop() {
  tab.loop(rf95);
}