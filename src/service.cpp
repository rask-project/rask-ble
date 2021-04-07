#include "service.h"
#include "characteristic.h"

namespace Rask::BLE {

Service::Service(QObject *parent) :
    QObject(parent),
    m_service(nullptr),
    m_status(Status::DiscoveryRequired)
{}

Service::Service(QLowEnergyService *service, QObject *parent) :
    QObject(parent),
    m_service(service),
    m_status(Status::DiscoveryRequired)
{
    configure();
}

QList<QObject *> Service::getCharacteristics() const
{
    return m_characteristics;
}

QString Service::getName() const
{
    if (m_service == nullptr)
        return QLatin1String();
    return m_service->serviceName();
}

QString Service::getUuid() const
{
    if (m_service == nullptr)
        return QLatin1String();
    return m_service->serviceUuid().toString(QUuid::WithoutBraces);
}

QString Service::getType() const
{
    if (m_service == nullptr)
        return QLatin1String();

    QString result;
    if ((m_service->type() & QLowEnergyService::PrimaryService) != 0U)
        result += QStringLiteral("primary");
    else
        result += QStringLiteral("secondary");

    if ((m_service->type() & QLowEnergyService::IncludedService) != 0U)
        result += QStringLiteral(" included");

    result.prepend('<').append('>');

    return result;
}

Service::Status Service::getStatus() const
{
    return m_status;
}

void Service::setService(QLowEnergyService *service)
{
    m_service = service;
    configure();
    emit serviceChanged();
}

void Service::details()
{
    qDeleteAll(m_characteristics);
    m_characteristics.clear();
    emit characteristicsChanged();

    m_service->discoverDetails();
}

void Service::write(int key, const QString &value)
{
    write(m_characteristics[key], value);
}

void Service::write(QObject *obj, const QString &value)
{
    auto characteristic = qobject_cast<Characteristic *>(obj)->getCharacteristic();
    if (!characteristic.isValid()) {
        qCritical() << "Characteristic invalid:" << characteristic.uuid();
        return;
    }

    m_service->writeCharacteristic(characteristic, value.toUtf8());
}

void Service::notify(QObject *obj)
{
    auto *objChar = qobject_cast<Characteristic *>(obj);
    auto characteristic = objChar->getCharacteristic();
    if (!characteristic.isValid()) {
        qCritical() << "Characteristic invalid:" << characteristic.uuid();
        return;
    }

    QLowEnergyDescriptor notification(characteristic.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration));
    if (!notification.isValid()) {
        qCritical() << "Characteristic Notifications invalid:" << notification.uuid();
        return;
    }

    const auto notify = !objChar->getNotification();
    m_service->writeDescriptor(notification, notify ? QByteArray::fromHex("0100") : QByteArray::fromHex("0000"));
    objChar->setNotification(notify);
}

void Service::setStatus(Service::Status status)
{
    if (m_status == status)
        return;

    m_status = status;
    emit statusChanged();
}

void Service::stateChanged(QLowEnergyService::ServiceState state)
{
    switch (state) {
    case QLowEnergyService::ServiceState::DiscoveringServices: setStatus(Status::DiscoveringDetails); break;
    default: setStatus(static_cast<Status>(state)); break;
    }

    if (m_status != Status::ServiceDiscovered)
        return;

    addCharacteristics();
}

void Service::addCharacteristics()
{
    for (auto &characterisitc : m_service->characteristics())
        m_characteristics.append(new Characteristic(characterisitc, this));
    emit characteristicsChanged();
}

void Service::configure()
{
    m_service->setParent(this);
    connect(m_service, &QLowEnergyService::stateChanged, this, &Service::stateChanged);
    connect(m_service, &QLowEnergyService::characteristicChanged, this, [&](const QLowEnergyCharacteristic &info, const QByteArray &value)
    {
        qDebug() << "Characteristic changed:" << info.uuid() << value;
    });
    connect(m_service, &QLowEnergyService::characteristicWritten, this, [&](const QLowEnergyCharacteristic &characteristic, const QByteArray &newValue)
    {
        qDebug() << "Written" << characteristic.uuid() << newValue;
    });
    connect(m_service, QOverload<QLowEnergyService::ServiceError>::of(&QLowEnergyService::error), this, [&](QLowEnergyService::ServiceError error)
    {
        qDebug() << "Service Error:" << error;
    });
}

} // Rask::BLE
