#include "device.h"
#include "service.h"

#include <QBluetoothAddress>
#include <QDebug>

namespace Rask::BLE {

Device::Device(QObject *parent) :
    QObject(parent),
    m_controller(nullptr),
    m_randomAddress(false),
    m_discoveringServices(false),
    m_status(Status::Disconnected)
{}

Device::Device(const QBluetoothDeviceInfo &info, QObject *parent) :
    QObject(parent),
    m_info(info),
    m_controller(nullptr),
    m_randomAddress(false),
    m_discoveringServices(false),
    m_status(Status::Disconnected)
{}

Device::~Device()
{
    qDebug() << "Device deleted" << getAddress();
    qDeleteAll(m_services);
    delete  m_controller;
    m_services.clear();
}

void Device::setDeviceInfo(const QBluetoothDeviceInfo &info)
{
    m_info = info;
    emit deviceChanged();
}

QString Device::getName() const
{
    return m_info.name();
}

QString Device::getAddress() const
{
    return m_info.address().toString();
}

bool Device::getRandomAddress() const
{
    return m_randomAddress;
}

bool Device::getDiscoveringServices() const
{
    return m_discoveringServices;
}

Device::Status Device::getStatus() const
{
    return m_status;
}

QList<QObject *> Device::getServices() const
{
    return m_services;
}

void Device::connect()
{
    disconnect();
    setStatus(Status::Connecting);

    m_controller = QLowEnergyController::createCentral(m_info);
    QObject::connect(m_controller, &QLowEnergyController::connected, this, &Device::connected);
    QObject::connect(m_controller, QOverload<QLowEnergyController::Error>::of(&QLowEnergyController::error), this, &Device::deviceError);
    QObject::connect(m_controller, &QLowEnergyController::disconnected, this, [&]() { setStatus(Status::Disconnected); });
    QObject::connect(m_controller, &QLowEnergyController::serviceDiscovered, this, &Device::serviceDiscovered);
    QObject::connect(m_controller, &QLowEnergyController::discoveryFinished, this, [&]() { setDiscoveringServices(false); });

    if (m_randomAddress)
        m_controller->setRemoteAddressType(QLowEnergyController::RandomAddress);
    else
        m_controller->setRemoteAddressType(QLowEnergyController::PublicAddress);
    m_controller->connectToDevice();
}

void Device::disconnect()
{
    if (m_controller != nullptr) {
        m_controller->disconnectFromDevice();
        delete m_controller;
        m_controller = nullptr;
        setStatus(Status::Disconnected);
    }
}

void Device::setRandomAddress(bool randomAddress)
{
    if (m_randomAddress == randomAddress)
        return;

    m_randomAddress = randomAddress;
    emit randomAddressChanged();
}

void Device::setDiscoveringServices(bool discoveringServices)
{
    if (m_discoveringServices == discoveringServices)
        return;

    m_discoveringServices = discoveringServices;
    emit discoveringServicesChanged();
}

void Device::setStatus(Device::Status status)
{
    if (m_status == status)
        return;

    m_status = status;
    emit statusChanged();
}

void Device::deviceError(QLowEnergyController::Error error)
{
    if (error != QLowEnergyController::Error::ConnectionError || m_status == Status::Connecting)
        setStatus(Status::Disconnected);
    qCritical() << "Error:" << error << m_controller->errorString();
}

void Device::connected()
{
    setDiscoveringServices(true);
    qDeleteAll(m_services);

    m_controller->discoverServices();
    setStatus(Status::Connected);
}

void Device::serviceDiscovered(const QBluetoothUuid &serviceUuid)
{
    qDebug() << getAddress() << serviceUuid;
    QLowEnergyService *service = m_controller->createServiceObject(serviceUuid);
    if (service == nullptr) {
        qCritical() << "Error: Cannot create service for uuid" << serviceUuid;
        return;
    }

    m_services.append(new Service(service));
    emit servicesChanged();
}

}
