#ifndef _MONITOR_DATA_H_
#define _MONITOR_DATA_H_

#include <string>
#include "Geom2d.h"

// Container for monitor-specific information
class MonitorData
{
	public:		
		static MonitorData GetMonitorData(int iMonitorIndex);

		bool IsAttached() const				{ return m_bAttached; }
		const std::string& GetName() const	{ return m_strName; }
		Point2d<int> GetPosition() const	{ return m_pos; }
		Size2d<int> GetSize() const		{ return m_size; }

	public:		
		~MonitorData();

	private:
		MonitorData();

		bool m_bAttached;
		std::string m_strName;
		Point2d<int> m_pos;
		Size2d<int> m_size;
};


#endif // _MONITOR_DATA_H_
