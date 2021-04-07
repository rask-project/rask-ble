#pragma once

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>

#include <map>
#include <memory>

#include "device.h"

namespace Rask::BLE {

class Controller : public QObject
{
    Q_PROPERTY(bool discovering READ getDiscovering WRITE setDiscovering NOTIFY discoveringChanged)
    Q_PROPERTY(QList<QObject *> devices READ getDevices NOTIFY devicesChanged)
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);

    bool getDiscovering() const;
    QList<QObject *> getDevices() const;

public slots:
    void setDiscovering(bool discovering);
    void startDiscovering();

signals:
    void discoveringChanged();
    void devicesChanged();

private:
    std::unique_ptr<QBluetoothDeviceDiscoveryAgent> m_deviceDiscoveryAgent;
    bool m_discovering;
    QList<QObject*> m_devices;

    static void newDeviceDiscovered(const QBluetoothDeviceInfo &info);
    void deviceDiscoverError(QBluetoothDeviceDiscoveryAgent::Error error);
    void deviceDiscoveryFinished();
};

} // Rask::BLE
