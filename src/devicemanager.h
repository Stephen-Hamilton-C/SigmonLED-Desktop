#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>
#include <QtBluetooth>

class DeviceManager : public QObject
{
    Q_OBJECT
public:
    explicit DeviceManager(QObject *parent = nullptr);
    ~DeviceManager();

    QList<QBluetoothDeviceInfo*> getDiscoveredDevices();
    void connectDevice(QBluetoothDeviceInfo* device);
    void disconnectDevice();
    void write(QByteArray bytes);

signals:
    void onScanStarted();
    void onScanStopped();
    void onDeviceConnected(const QBluetoothDeviceInfo device);
    void onDeviceDisconnected(const QBluetoothDeviceInfo device);

public slots:
    void startScan();
    void stopScan();

private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo& deviceInfo);
    void scanError(QBluetoothDeviceDiscoveryAgent::Error error);
    void scanStopped();
    void connected();
    void disconnected();
    void serviceDiscovered(const QBluetoothUuid& service);
    void serviceDiscoveryFinished();
    void serviceStateChanged(QLowEnergyService::ServiceState state);

private:
    const QBluetoothUuid desiredServiceUUID = QBluetoothUuid("0000ffe0-0000-1000-8000-00805f9b34fb");
    const QBluetoothUuid desiredCharacteristicUUID = QBluetoothUuid("0000ffe1-0000-1000-8000-00805f9b34fb");

    QBluetoothDeviceDiscoveryAgent* _scanner;
    QBluetoothDeviceInfo* _currentDevice = nullptr;
    QLowEnergyController* _deviceController = nullptr;
    QLowEnergyService* _deviceService = nullptr;
    QLowEnergyCharacteristic* _deviceCharacteristic = nullptr;

    bool _scanning = false;
    bool _connected = false;
    QList<QBluetoothDeviceInfo*> _discoveredDevices = QList<QBluetoothDeviceInfo*>();
};

#endif // DEVICEMANAGER_H
