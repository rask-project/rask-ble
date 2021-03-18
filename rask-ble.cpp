#include "rask-ble.h"

#include <iostream>

// #include <esp_bt.h>
// #include <esp_gap_ble_api.h>
// #include <esp_gattc_api.h>
// #include <esp_gatt_defs.h>
// #include <esp_bt_main.h>
// #include <esp_gatt_common_api.h>
// #include <esp_log.h>

namespace Rask {
namespace BLE {

BLEController::BLEController()
{
    // ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));
    // esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
}

} // namespace BLE
} // namespace Rask
