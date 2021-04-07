#pragma once

#include <QObject>
#include <QLowEnergyCharacteristic>

namespace Rask::BLE {

class Characteristic : public QObject
{
    Q_PROPERTY(QString name READ getName NOTIFY characteristicChanged)
    Q_PROPERTY(QString uuid READ getUuid NOTIFY characteristicChanged)
    Q_PROPERTY(QString value READ getValue NOTIFY characteristicChanged)
    Q_PROPERTY(QString handle READ getHandle NOTIFY characteristicChanged)
    Q_PROPERTY(QString permission READ getPermission NOTIFY characteristicChanged)
    Q_PROPERTY(bool notification READ getNotification WRITE setNotification NOTIFY notificationChanged)
    Q_OBJECT

public:
    explicit Characteristic(QObject *parent = nullptr);
    Characteristic(const QLowEnergyCharacteristic &characteristic, QObject *parent = nullptr);

    enum class PropertyType {
        Unknown = 0x00,
        Broadcasting = 0x01,
        Read = 0x02,
        WriteNoResponse = 0x04,
        Write = 0x08,
        Notify = 0x10,
        Indicate = 0x20,
        WriteSigned = 0x40,
        ExtendedProperty = 0x80
    };
    Q_ENUM(PropertyType)

    QString getName() const;
    QString getUuid() const;
    QString getValue() const;
    QString getHandle() const;
    QString getPermission() const;
    bool getNotification() const;

public slots:
    QLowEnergyCharacteristic &getCharacteristic();
    void setCharacteristic(const QLowEnergyCharacteristic &characteristic);
    bool hasPermission(PropertyType property);
    void setNotification(bool notification);

signals:
    void characteristicChanged();
    void notificationChanged();

private:
    QLowEnergyCharacteristic m_characteristic;
    bool m_notification;
};

}
