#include "mgos.h"
#include "mgos_arduino_dallas_temp.h"
#include "mgos_arduino_onewire.h"

#define ONE_WIRE_BUS 2
OneWire *ow;
DallasTemperature *dt;
int deviceCount = 0;
float tempC;

void get_temp_reading_cb()
{
  mgos_arduino_dt_request_temperatures(dt);
  // Display temperature from each sensor
  for (int i = 0; i < deviceCount; i++)
  {
    tempC = mgos_arduino_dt_get_tempc_by_index(dt, i);
    LOG(LL_INFO, ("TCU: Temparture of  found : %.2f from %i device", tempC, i));
  }
}

enum mgos_app_init_result mgos_app_init(void)
{
  ow = mgos_arduino_onewire_create(ONE_WIRE_BUS);
  dt = mgos_arduino_dt_create(ow);
  mgos_arduino_dt_begin(dt);
  deviceCount = mgos_arduino_dt_get_device_count(dt);
  LOG(LL_INFO, ("TCU: Devices found : %i", deviceCount));
  mgos_set_timer(
      1000,
      MGOS_TIMER_REPEAT,
      get_temp_reading_cb,
      NULL);
  return MGOS_APP_INIT_SUCCESS;
}
