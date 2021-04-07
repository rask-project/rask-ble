#pragma once

#include <QObject>
#include <QLowEnergyService>
#include <QLowEnergyCharacteristic>

namespace Rask::BLE {

class Service : public QObject
{
    Q_PROPERTY(QList<QObject *> characteristics READ getCharacteristics NOTIFY characteristicsChanged)
    Q_PROPERTY(QString name READ getName NOTIFY serviceChanged)
    Q_PROPERTY(QString uuid READ getUuid NOTIFY serviceChanged)
    Q_PROPERTY(QString type READ getType NOTIFY serviceChanged)
    Q_PROPERTY(Status status READ getStatus WRITE setStatus NOTIFY statusChanged)
    Q_OBJECT

public:
    Service(QObject *parent = nullptr);
    Service(QLowEnergyService *service, QObject *parent = nullptr);

    enum class Status
    {
        InvalidService = 0,
        DiscoveryRequired,
        DiscoveringDetails,
        ServiceDiscovered,
        LocalService
    };
    Q_ENUM(Status)

    QList<QObject *> getCharacteristics() const;
    QString getName() const;
    QString getUuid() const;
    QString getType() const;
    Status getStatus() const;

public slots:
    void setService(QLowEnergyService *service);
    void details();
    void write(int key, const QString &value);
    void write(QObject *obj, const QString &value);
    void notify(QObject *obj);
    void setStatus(Status status);

private slots:
    void stateChanged(QLowEnergyService::ServiceState state);
    void addCharacteristics();

signals:
    void serviceChanged();
    void characteristicsChanged();
    void statusChanged();

private:
    QLowEnergyService *m_service;
    QList<QObject *> m_characteristics;
    QString m_name;
    QString m_uuid;
    QString m_type;
    Status m_status;

    void configure();
};

} // Rask::BLE
