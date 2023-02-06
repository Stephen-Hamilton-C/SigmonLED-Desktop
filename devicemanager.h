#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>
#include <QtBluetooth>

class DeviceManager : public QObject
{
    Q_OBJECT
public:
    explicit DeviceManager(QObject *parent = nullptr);

signals:
    void onScanStarted();
    void onScanStopped();

public slots:
    void startScan();
    void stopScan();

private:
    const QBluetoothUuid desiredServiceUUID = QBluetoothUuid("0000ffe0-0000-1000-8000-00805f9b34fb");
    const QBluetoothUuid desiredCharacteristicUUID = QBluetoothUuid("0000ffe1-0000-1000-8000-00805f9b34fb");

    void deviceDiscovered(const QBluetoothDeviceInfo& deviceInfo);
    void scanError(QBluetoothDeviceDiscoveryAgent::Error error);
    void scanStopped();

    QBluetoothDeviceDiscoveryAgent* _scanner;

    bool _scanning = false;
    QList<QBluetoothDeviceInfo*> _discoveredDevices = QList<QBluetoothDeviceInfo*>();
};

#endif // DEVICEMANAGER_H
