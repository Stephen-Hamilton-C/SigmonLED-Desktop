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

void DeviceManager::startScan()
{
    qDebug() << "Starting scan...";
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
