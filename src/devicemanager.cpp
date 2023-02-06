#include "devicemanager.h"
#include <QDebug>

DeviceManager::DeviceManager(QObject *parent)
  : QObject{parent}
{
     _scanner = new QBluetoothDeviceDiscoveryAgent(this);
     _scanner->setLowEnergyDiscoveryTimeout(15000);

     connect(_scanner, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &DeviceManager::deviceDiscovered);
     connect(_scanner, &QBluetoothDeviceDiscoveryAgent::errorOccurred, this, &DeviceManager::scanError);
     connect(_scanner, &QBluetoothDeviceDiscoveryAgent::finished, this, &DeviceManager::scanStopped);
     connect(_scanner, &QBluetoothDeviceDiscoveryAgent::canceled, this, &DeviceManager::scanStopped);
}

QList<QBluetoothDeviceInfo*> DeviceManager::getDiscoveredDevices()
{
    return _discoveredDevices;
}

void DeviceManager::connectDevice(QBluetoothDeviceInfo* device)
{
    qDebug() << "First disconnect from current device...";
    disconnectDevice();

    qDebug() << "Connecting...";
    _currentDevice = device;
    _deviceController = QLowEnergyController::createCentral(*device, this);

    connect(_deviceController, &QLowEnergyController::connected, this, &DeviceManager::connected);
    connect(_deviceController, &QLowEnergyController::disconnected, this, &DeviceManager::disconnected);

    _deviceController->connectToDevice();
}

void DeviceManager::disconnectDevice()
{
    if(_deviceController == nullptr) return;

    qDebug() << "Disconnecting...";
    _deviceController->disconnectFromDevice();
}

void DeviceManager::write(QByteArray bytes)
{
    if(!_connected) return;
    if(_deviceCharacteristic == nullptr) return;
    if(!_deviceCharacteristic->isValid()) return;
    if(_deviceService == nullptr) return;

    qDebug() << "Writing " << bytes;
    _deviceService->writeCharacteristic(*_deviceCharacteristic, bytes, QLowEnergyService::WriteWithoutResponse);
}

void DeviceManager::startScan()
{
    qDebug() << "Starting scan...";
    _discoveredDevices.clear();
    _scanner->start(QBluetoothDeviceDiscoveryAgent::DiscoveryMethod::LowEnergyMethod);
    _scanning = true;
    emit onScanStarted();
}

void DeviceManager::stopScan()
{
    qDebug() << "Stopping scan...";
    _scanner->stop();
}

void DeviceManager::deviceDiscovered(const QBluetoothDeviceInfo& deviceInfo)
{
    qDebug() << "Discovered device: " << deviceInfo.name();
    if(deviceInfo.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration
            && deviceInfo.serviceUuids().contains(desiredServiceUUID)) {
        qDebug() << "Device is BLE and has the serial service";
        _discoveredDevices.append(new QBluetoothDeviceInfo(deviceInfo));
    }
}

void DeviceManager::scanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    qDebug() << "Error while scanning:";
    switch(error) {
        case QBluetoothDeviceDiscoveryAgent::Error::InputOutputError:
            qDebug() << "IO Error";
            break;
        case QBluetoothDeviceDiscoveryAgent::Error::InvalidBluetoothAdapterError:
            qDebug() << "Invalid Bluetooth Adapter";
            break;
        case QBluetoothDeviceDiscoveryAgent::Error::LocationServiceTurnedOffError:
            qDebug() << "Location Services are disabled";
            break;
        case QBluetoothDeviceDiscoveryAgent::Error::NoError:
            qDebug() << "There is no error...";
            break;
        case QBluetoothDeviceDiscoveryAgent::Error::PoweredOffError:
            qDebug() << "Powered off";
            break;
        case QBluetoothDeviceDiscoveryAgent::Error::UnknownError:
            qDebug() << "Unknown error";
            break;
        case QBluetoothDeviceDiscoveryAgent::Error::UnsupportedDiscoveryMethod:
            qDebug() << "Unsupported Discovery Method";
            break;
        case QBluetoothDeviceDiscoveryAgent::Error::UnsupportedPlatformError:
            qDebug() << "Unsupported Platform";
            break;
    }

    scanStopped();
}

void DeviceManager::scanStopped()
{
    qDebug() << "Scan has been stopped";
    _scanning = false;
    emit onScanStopped();
}

void DeviceManager::connected()
{
    qDebug() << "Device has been connected";
    // Do this once service has been discovered
    // emit onDeviceConnected(QBluetoothDeviceInfo(*_currentDevice));

    connect(_deviceController, &QLowEnergyController::serviceDiscovered, this, &DeviceManager::serviceDiscovered);
    connect(_deviceController, &QLowEnergyController::discoveryFinished, this, &DeviceManager::serviceDiscoveryFinished);
    _deviceController->discoverServices();
}

void DeviceManager::disconnected()
{
    qDebug() << "Device has been disconnected";
    _connected = false;
    emit onDeviceDisconnected(QBluetoothDeviceInfo(*_currentDevice));
    delete _currentDevice;
    _currentDevice = nullptr;
    delete _deviceController;
    _deviceController = nullptr;
    delete _deviceService;
    _deviceService = nullptr;
    delete _deviceCharacteristic;
    _deviceCharacteristic = nullptr;
}

void DeviceManager::serviceDiscovered(const QBluetoothUuid& service)
{
    qDebug() << "Service discovered";
    if(service == desiredServiceUUID) {
        qDebug() << "The discovered service is the serial service";
        disconnect(_deviceController, &QLowEnergyController::serviceDiscovered, this, &DeviceManager::serviceDiscovered);
        disconnect(_deviceController, &QLowEnergyController::discoveryFinished, this, &DeviceManager::serviceDiscoveryFinished);

        _deviceService = _deviceController->createServiceObject(service, this);
        connect(_deviceService, &QLowEnergyService::stateChanged, this, &DeviceManager::serviceStateChanged);
    }
}

void DeviceManager::serviceDiscoveryFinished()
{
    if(!_connected) {
        qDebug() << "Connect failed - Could not find Serial service";
        disconnectDevice();
    }
}

void DeviceManager::serviceStateChanged(QLowEnergyService::ServiceState state)
{
    if(state == QLowEnergyService::RemoteServiceDiscovered) {
        qDebug() << "Remote Service Discovered";
        QLowEnergyCharacteristic qleChar = _deviceService->characteristic(desiredCharacteristicUUID);
        if(!qleChar.isValid()) return;
        disconnect(_deviceService, &QLowEnergyService::stateChanged, this, &DeviceManager::serviceStateChanged);

        qDebug() << "Characteristic is valid, device is now ready.";
        _deviceCharacteristic = &qleChar;

        _connected = true;
        emit onDeviceConnected(QBluetoothDeviceInfo(*_currentDevice));
    }
}
