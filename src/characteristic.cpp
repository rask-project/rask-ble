#include "characteristic.h"
#include "constants.h"

#include <QLowEnergyService>
#include <QLowEnergyDescriptor>

namespace Rask::BLE {

Characteristic::Characteristic(QObject *parent) :
    QObject(parent)
{}

Characteristic::Characteristic(const QLowEnergyCharacteristic &characteristic, QObject *parent) :
    QObject(parent),
    m_characteristic(characteristic),
    m_notification(false)
{}

QString Characteristic::getName() const
{
    QString name = m_characteristic.name();
    if (!name.isEmpty())
        return name;

    const QList<QLowEnergyDescriptor> descriptors = m_characteristic.descriptors();
    for (const QLowEnergyDescriptor &descriptor : descriptors) {
        if (descriptor.type() == QBluetoothUuid::CharacteristicUserDescription) {
            name = descriptor.value();
            break;
        }
    }

    if (name.isEmpty())
        name = QStringLiteral("Unkown");
    return name;
}

QString Characteristic::getUuid() const
{
    return m_characteristic.uuid().toString(QBluetoothUuid::WithoutBraces);
}

QString Characteristic::getValue() const
{
    QByteArray value = m_characteristic.value();
    if (value.isEmpty()) {
        return QStringLiteral("<none>");
    }
    return value.toHex();
}

QString Characteristic::getHandle() const
{
    return QStringLiteral("0x%1").arg(QString::number(m_characteristic.handle(), Constants::baseHex));
}

QString Characteristic::getPermission() const
{
    QStringList properties;
    uint permission = m_characteristic.properties();
    if ((permission & QLowEnergyCharacteristic::Read) != 0U) properties << QStringLiteral("Read");
    if ((permission & QLowEnergyCharacteristic::Write) != 0U) properties << QStringLiteral("Write");
    if ((permission & QLowEnergyCharacteristic::Notify) != 0U) properties << QStringLiteral("Notify");
    if ((permission & QLowEnergyCharacteristic::Indicate) != 0U) properties << QStringLiteral("Indicate");
    if ((permission & QLowEnergyCharacteristic::ExtendedProperty) != 0U) properties << QStringLiteral("ExtendedProperty");
    if ((permission & QLowEnergyCharacteristic::Broadcasting) != 0U) properties << QStringLiteral("Broadcasting");
    if ((permission & QLowEnergyCharacteristic::WriteNoResponse) != 0U) properties << QStringLiteral("WriteNoResponse");
    if ((permission & QLowEnergyCharacteristic::WriteSigned) != 0U) properties << QStringLiteral("WriteSigned");
    return properties.join(QStringLiteral(", "));
}

bool Characteristic::getNotification() const
{
    return m_notification;
}

QLowEnergyCharacteristic &Characteristic::getCharacteristic()
{
    return m_characteristic;
}

void Characteristic::setCharacteristic(const QLowEnergyCharacteristic &characteristic)
{
    m_characteristic = characteristic;
    emit characteristicChanged();
}

bool Characteristic::hasPermission(Characteristic::PropertyType property)
{
    return (m_characteristic.properties() & static_cast<QLowEnergyCharacteristic::PropertyType>(property)) != 0U;
}

void Characteristic::setNotification(bool notification)
{
    if (m_notification == notification)
        return;

    m_notification = notification;
    emit notificationChanged();
}

}
