#include "raskble_plugin.h"

#include <QtQml>
#include "controller.h"
#include "device.h"
#include "service.h"
#include "characteristic.h"

void RaskBlePlugin::registerTypes(const char *uri)
{
    qmlRegisterType<Rask::BLE::Controller>(uri, 1, 0, "BLEController");
    qmlRegisterType<Rask::BLE::Device>(uri, 1, 0, "BLEDevice");
    qmlRegisterType<Rask::BLE::Service>(uri, 1, 0, "BLEService");
    qmlRegisterType<Rask::BLE::Characteristic>(uri, 1, 0, "BLECharacteristic");
}
