#include "Camera.h"
std::shared_ptr<CameraImp> Camera::GetCameraImp() const
{
	return m_cameraImp;
}
void Camera::SetName(std::string name)
{
	m_name = name;
}
std::string Camera::GetName()const
{
	return m_name;
}
void Camera::SetVendor(CameraVendor vendor)
{
	m_vendor = vendor;
}
CameraVendor Camera::GetVendor()const
{
	return m_vendor;
}
std::string Camera::GetVendorString()const
{
	if (m_cameraImp == nullptr)
	{
		return "Null";
	}
	return m_cameraImp->GetVendorString();
}
void Camera::SetId(std::string id)
{
	m_id = id;
	if (m_cameraImp == nullptr)
	{
		return;
	}
	m_cameraImp->SetId(m_id);
}
std::string Camera::GetId()const
{
	return m_id;
}
void Camera::SetClientIpAddress(std::string ip)
{
	m_clientIpAddress = ip;
}
std::string Camera::GetClientIpAddress()const
{
	return m_clientIpAddress;
}
void Camera::SetTriggerMode(CameraTriggerMode mode)
{
	if (m_cameraImp == nullptr)
	{
		return;
	}
	if (mode == CameraTriggerMode::Continue)
	{
		m_isGrabContinous = true;
		m_cameraImp->SetTriggerMode(CameraTriggerMode::Continue);
	}
	else
	{
		m_triggerMode = mode;
		m_isGrabContinous = false;
		m_cameraImp->SetTriggerMode(m_triggerMode);
	}
}
CameraTriggerMode Camera::GetTriggerMode()const
{
	return m_triggerMode;
}
bool Camera::IsGrabContinous()const
{
	return m_isGrabContinous;
}
void Camera::SetExposureTime(int time)
{
	m_exposureTime = time;
	if (m_cameraImp == nullptr)
	{
		return;
	}
	m_cameraImp->SetExposureTime(m_exposureTime);
}
int Camera::GetExposureTime()const
{
	return m_exposureTime;
}
void Camera::SetGain(int gain)
{
	m_gain = gain;
	if (m_cameraImp == nullptr)
	{
		return;
	}
	m_cameraImp->SetGain(m_gain);
}
int Camera::GetGain()const
{
	return m_gain;
}
void Camera::SoftTrigger()
{
	if (m_cameraImp == nullptr)
	{
		return;
	}
	/*this->SetTriggerMode(CameraTriggerMode::Soft);*/
	this->m_cameraImp->ExcuteSoftwareTrigger();
	/*this->SetSelfTriggerMode();*/
}
void Camera::InitCameraImp()
{
	m_cameraImp = CameraFactory::Instance()->CreateCamera(m_vendor);
	m_cameraImp->SetVendor(m_vendor);
	m_cameraImp->SetId(m_id);
	//bool b = m_cameraImp->IsOpen();
	//if (m_cameraImp->IsOpen())
	// return;
	m_cameraImp->Open();
}
void Camera::SetSelfTriggerMode()
{
	if (m_cameraImp == nullptr)
	{
		return;
	}
	m_cameraImp->SetTriggerMode(m_triggerMode);
}
void Camera::SetSelfExposureTime()
{
	if (m_cameraImp == nullptr)
	{
		return;
	}
	m_cameraImp->SetExposureTime(m_exposureTime);
}
void Camera::SetSelfGain()
{
	if (m_cameraImp == nullptr)
	{
		return;
	}
	m_cameraImp->SetGain(m_gain);
}
void Camera::InitCameraParams()
{
	if (m_cameraImp == nullptr)
	{
		return;
	}
	m_cameraImp->SetTriggerMode(m_triggerMode);
	m_cameraImp->SetExposureTime(m_exposureTime);
	m_cameraImp->SetGain(m_gain);
}
void Camera::Update()
{
	if (m_cameraImp == nullptr)
	{
		return;
	}
	m_cameraImp->Update();
}