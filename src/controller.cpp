#include "controller.h"
#include "constants.h"

#include <QDebug>

namespace Rask::BLE {

Controller::Controller(QObject *parent) :
    QObject(parent),
    m_deviceDiscoveryAgent(std::make_unique<QBluetoothDeviceDiscoveryAgent>(parent)),
    m_discovering(false)
{
    m_deviceDiscoveryAgent->setLowEnergyDiscoveryTimeout(Constants::bleDiscoveryTimeout);
    connect(m_deviceDiscoveryAgent.get(), &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &Controller::newDeviceDiscovered);
    connect(m_deviceDiscoveryAgent.get(), QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::error), this, &Controller::deviceDiscoverError);
    connect(m_deviceDiscoveryAgent.get(), &QBluetoothDeviceDiscoveryAgent::finished, this, &Controller::deviceDiscoveryFinished);
}

bool Controller::getDiscovering() const
{
    return m_discovering;
}

QList<QObject *> Controller::getDevices() const
{
    return m_devices;
}

void Controller::setDiscovering(bool discovering)
{
    if (m_discovering == discovering)
        return;

    m_discovering = discovering;
    emit discoveringChanged();
}

void Controller::startDiscovering()
{
    setDiscovering(true);
    qDeleteAll(m_devices);
    m_devices.clear();
    emit devicesChanged();
    m_deviceDiscoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

void Controller::newDeviceDiscovered(const QBluetoothDeviceInfo &info)
{
    qDebug() << "New device detected: " << info.address() << info.name();
}

void Controller::deviceDiscoverError(QBluetoothDeviceDiscoveryAgent::Error /*error*/)
{}

void Controller::deviceDiscoveryFinished()
{
    const auto devices = m_deviceDiscoveryAgent->discoveredDevices();
    for (const auto &info : devices)
        if ((info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) != 0U)
            m_devices.append(new Device(info));
    emit devicesChanged();
    setDiscovering(false);
}

} // Rask::BLE
