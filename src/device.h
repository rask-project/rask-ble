#pragma once

#include <QObject>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>

namespace Rask::BLE {

class Device : public QObject
{
    Q_PROPERTY(QString name READ getName NOTIFY deviceChanged)
    Q_PROPERTY(QString address READ getAddress NOTIFY deviceChanged)
    Q_PROPERTY(Status status READ getStatus WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(bool randomAddress READ getRandomAddress WRITE setRandomAddress NOTIFY randomAddressChanged)
    Q_PROPERTY(bool discoveringServices READ getDiscoveringServices WRITE setDiscoveringServices NOTIFY discoveringServicesChanged)
    Q_PROPERTY(QList<QObject *> services READ getServices NOTIFY servicesChanged)
    Q_OBJECT

public:
    Device(QObject *parent = nullptr);
    Device(const QBluetoothDeviceInfo &info, QObject *parent = nullptr);
    ~Device();

    enum class Status
    {
        Disconnected,
        Connecting,
        Connected
    };
    Q_ENUM(Status)

    void setDeviceInfo(const QBluetoothDeviceInfo &info);

    QString getName() const;
    QString getAddress() const;
    bool getRandomAddress() const;
    bool getDiscoveringServices() const;
    Status getStatus() const;
    QList<QObject *> getServices() const;

public slots:
    void connect();
    void disconnect();
    void setRandomAddress(bool randomAddress);
    void setDiscoveringServices(bool discoveringServices);
    void setStatus(Status status);

private slots:
    void deviceError(QLowEnergyController::Error error);
    void connected();
    void serviceDiscovered(const QBluetoothUuid &serviceUuid);

signals:
    void deviceChanged();
    void nameChanged();
    void randomAddressChanged();
    void discoveringServicesChanged();
    void statusChanged();
    void servicesChanged();

private:
    QBluetoothDeviceInfo m_info;
    QLowEnergyController *m_controller;
    bool m_randomAddress;
    bool m_discoveringServices;
    Status m_status;
    QList<QObject *> m_services;
};

}

