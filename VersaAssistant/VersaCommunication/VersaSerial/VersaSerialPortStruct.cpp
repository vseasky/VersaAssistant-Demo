#include "VersaSerialPortStruct.h"

void VersaSerialPortStruct::setSerialPortName(QString portName)
{
	mSerialPortName = portName;
	emit serialPortNameChanged(mSerialPortName);
}

void VersaSerialPortStruct::setSerialBaudRate(int baudRate)
{
	mSerialBaudRate = baudRate;
	emit serialBaudRateChanged(mSerialBaudRate);
}

void VersaSerialPortStruct::setSerialStopBits(QSerialPort::StopBits stopBits)
{
	mSerialStopBits = stopBits;
	emit serialStopBitsChanged(mSerialStopBits);
}

void VersaSerialPortStruct::setSerialDataBits(QSerialPort::DataBits dataBits)
{
	mSerialDataBits = dataBits;
	emit serialDataBitsChanged(mSerialDataBits);
}

void VersaSerialPortStruct::setSerialParrity(QSerialPort::Parity Parrity)
{
	mSerialParrity = Parrity;
	emit serialParrityChanged(mSerialParrity);
}

void VersaSerialPortStruct::setSerialFlowControl(QSerialPort::FlowControl flowControl)
{
	mSerialFlowControl = flowControl;
	emit serialFlowControlChanged(flowControl);
}