#include "MonitorData.h"
#include <windows.h> // Todo: Remove! MonitorData should use IWindow functions
#include <wingdi.h>

// g++ doesn't define this in its wingdi.h
#ifndef DISPLAY_DEVICE_ATTACHED_TO_DESKTOP
#define DISPLAY_DEVICE_ATTACHED_TO_DESKTOP 0x00000001
#endif

MonitorData::MonitorData() : m_bAttached(false), m_strName("INVALID"), m_pos(-1,-1)
{
}

MonitorData::~MonitorData()
{
}

MonitorData MonitorData::GetMonitorData(int iMonitorIndex/*=0*/)
{
	// Grab display device data on the selected monitor
	DISPLAY_DEVICE dispDevice;
	dispDevice.cb = sizeof(dispDevice);
	if ( !EnumDisplayDevices(NULL, iMonitorIndex, &dispDevice, 0) )
		return MonitorData();

	// Grab the device mode data for the selected monitor
	DEVMODE devMode;
	devMode.dmSize = sizeof(devMode);	
	if ( !EnumDisplaySettings(dispDevice.DeviceName, ENUM_CURRENT_SETTINGS, &devMode) )
		return MonitorData();

	// Create and return instance
	MonitorData monData;
	monData.m_bAttached = dispDevice.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP;
	monData.m_strName = dispDevice.DeviceName;
	monData.m_pos = Point2d<int>(devMode.dmPosition.x, devMode.dmPosition.y);
	monData.m_size = Size2d<int>(devMode.dmPelsWidth, devMode.dmPelsHeight);
	
	return monData;
}
