#pragma once
#include <mutex>
#include <string>
#include <vector>
enum CameraVendor
{
	Basler,
	Hik
};
class CameraBase
{
public:
	CameraBase() {}
	~CameraBase() {}
	void SetVendor(CameraVendor vendor) { m_vendor = vendor; }
	CameraVendor GetVendor()const { return m_vendor; }
	virtual std::string GetVendorString()const = 0;
	void SetId(std::string id) { m_id = id; }
	std::string GetId()const { return m_id; }
	virtual int Open() = 0;
	virtual bool IsOpen() = 0;
	virtual void Update()
	{
		if (m_isContinous)
		{
			m_isTriggered = false;
		}
		if (m_last_sts == false && m_current_sts == true)
		{
			m_isTriggered = true;
		}
		else if (m_last_sts == true && m_current_sts == true)
		{
			m_isTriggered = false;
			m_current_sts = false;
		}
		else
		{
			m_isTriggered = false;
		}
		m_last_sts = m_current_sts;
		if (m_isContinous)
		{
			m_isTriggered = false;
		}
	}
	bool IsTriggered()const { return m_isTriggered; }
	virtual const unsigned char* const GetImageData(int& imageSize) = 0;
	virtual int GetImageWidth(int& width) = 0;
	virtual int GetImageHeight(int& height) = 0;
	virtual int GetExposureTime(int& exposureTime) = 0;
	virtual int GetGain(int& gain) = 0;
	virtual int GetTriggerMode(int& mode) = 0;
	virtual int SetExposureTime(const int exposureTime) = 0;
	virtual int SetGain(const int gain) = 0;
	virtual int SetTriggerMode(const int mode) = 0; 
	virtual int ExcuteSoftwareTrigger() = 0;
	virtual void Close() = 0;
protected:
	CameraVendor m_vendor;
	std::string m_id;
	int m_triggerMode = 0;
	bool m_last_sts = false;
	bool m_current_sts = false;
	bool m_isTriggered = false;
	bool m_isContinous = false;
};