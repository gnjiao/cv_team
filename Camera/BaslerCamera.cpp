#include "BaslerCamera.h"
BaslerCamera::BaslerCamera(void)
{
	//m_bImageUpdate = false;
	m_imageData = nullptr;
	m_imageSize = 0;
}
BaslerCamera::~BaslerCamera(void)
{
	if (m_imageData != nullptr)
	{
		delete m_imageData;
		m_imageData = nullptr;
	}
	Pylon::PylonTerminate();
}
Pylon::DeviceInfoList_t BaslerCamera::m_devices;
Pylon::CTlFactory* BaslerCamera::m_tlFactory = NULL;
int BaslerCamera::EnumerateDevices(std::vector<std::string>& IDs)
{
	Pylon::PylonInitialize();
	// Get the transport layer factory.
	m_tlFactory = &Pylon::CTlFactory::GetInstance();
	// Get all devices and exit application if no device is found.
	if (m_tlFactory->EnumerateDevices(m_devices) == 0)
	{
		return 1;
	}
	for (size_t i = 0; i < m_devices.size(); ++i)
	{
		if ((std::string)(m_devices[i].GetVendorName().c_str()) == "Basler")
		{
			IDs.push_back(m_devices[i].GetSerialNumber().c_str());
		}
	}
	return 0;
}
int BaslerCamera::Open()
{
	if (m_id.empty())
	{
		return 1;
	}
	if (m_devices.empty())
	{
		Pylon::PylonInitialize();
		// Get the transport layer factory.
		m_tlFactory = &Pylon::CTlFactory::GetInstance();
		// Get all devices and exit application if no device is found.
		if (m_tlFactory->EnumerateDevices(m_devices) == 0)
		{
			return 1;
		}
	}
	// Create and attach all Pylon Devices.
	for (size_t i = 0; i < m_devices.size(); ++i)
	{
		if (m_id.compare(m_devices[i].GetSerialNumber().c_str()) == 0)
		{
			if (m_camera.IsPylonDeviceAttached())
			{
				Close();
			}
			m_camera.Attach(m_tlFactory->CreateDevice(m_devices[i]));
			std::cout << this << std::endl;
			/*std::cout << m_camera.IsPylonDeviceAttached() << std::endl;*/
		}
	}
	if (!m_camera.IsPylonDeviceAttached())
	{
		return 3;
	}
	m_camera.RegisterConfiguration(new Pylon::CSoftwareTriggerConfiguration, Pylon::RegistrationMode_ReplaceAll, Pylon::Cleanup_Delete);
	m_camera.RegisterImageEventHandler(this, Pylon::RegistrationMode_Append, Pylon::Ownership_ExternalOwnership);
	if (!m_camera.IsOpen())
	{
		m_camera.Open();
	}
	return 0;
}
bool BaslerCamera::IsOpen()
{
	return m_camera.IsOpen();
}
void BaslerCamera::Close()
{
	if (m_camera.IsPylonDeviceAttached())
	{
		m_camera.Close();
		m_camera.DestroyDevice();
		m_camera.DetachDevice();
	}
	return;
}
void BaslerCamera::OnImageGrabbed(Pylon::CInstantCamera& camera, const Pylon::CGrabResultPtr& ptrGrabResult)
{
	if (ptrGrabResult->GrabSucceeded())
	{
		if (m_imageSize != ptrGrabResult->GetImageSize())
		{
			m_imageSize = ptrGrabResult->GetImageSize();
			if (m_imageData != NULL)
			{
				delete[] m_imageData;
			}
			m_imageData = new unsigned char[m_imageSize];
		}
		m_mtx.lock();
		memcpy_s(m_imageData, m_imageSize, ptrGrabResult->GetBuffer(), m_imageSize);
		if (!m_isContinous)
		{
			m_current_sts = true;
		}
		m_mtx.unlock();
	}
	return;
}
int BaslerCamera::GetImageWith(int& width)
{
	if (!m_camera.IsOpen())
	{
		return 1;
	}
	width = m_camera.Width.GetValue();
	return 0;
}
int BaslerCamera::GetImageHeight(int& height)
{
	if (!m_camera.IsOpen())
	{
		return 1;
	}
	height = m_camera.Height.GetValue();
	return 0;
}
const unsigned char * const BaslerCamera::GetImageData(int& imageSize)
{
	if (!m_camera.IsOpen())
	{
		return NULL;
	}
	if (m_imageData == NULL)
	{
		return NULL;
	}
	// if (!m_bImageUpdate)
	// {
	// return NULL;
	// }
	if (m_imageSize < 1)
	{
		return NULL;
	}
	// if (imageData == NULL)
	// {
	// return 5;
	// }
	// imageData = new unsigned char[m_imageSize];
	// memcpy_s(imageData, m_imageSize, m_imageData, m_imageSize);
	imageSize = m_imageSize;
	// m_bImageUpdate = false;
	return m_imageData;
}
int BaslerCamera::ExcuteSoftwareTrigger()
{
	if (!m_camera.IsOpen())
	{
		return 1;
	}
	m_camera.ExecuteSoftwareTrigger();
	return 0;
}
int BaslerCamera::SetTriggerMode(const int mode)
{
	if (!m_camera.IsOpen())
	{
		return 1;
	}
	m_isContinous = false;
	switch (mode)
	{
	case 1:
		m_isContinous = true;
		m_camera.TriggerSelector.SetValue(Basler_GigECamera::TriggerSelector_FrameStart);
		m_camera.TriggerMode.SetValue(Basler_GigECamera::TriggerMode_Off);
		if (!m_camera.IsGrabbing())
		{
			m_camera.StartGrabbing(Pylon::GrabStrategy_OneByOne, Pylon::GrabLoop_ProvidedByInstantCamera);
		}
		//m_camera.StartGrabbing(Pylon::GrabStrategy_OneByOne, Pylon::GrabLoop_ProvidedByInstantCamera);
		m_triggerMode = 1;
		break;
	case 2:
		m_camera.TriggerSelector.SetValue(Basler_GigECamera::TriggerSelector_FrameStart);
		m_camera.TriggerMode.SetValue(Basler_GigECamera::TriggerMode_On);
		m_camera.TriggerSource.SetValue(Basler_GigECamera::TriggerSource_Software);
		if (!m_camera.IsGrabbing())
		{
			m_camera.StartGrabbing(Pylon::GrabStrategy_OneByOne, Pylon::GrabLoop_ProvidedByInstantCamera);
		}
		//m_camera.StartGrabbing(Pylon::GrabStrategy_OneByOne, Pylon::GrabLoop_ProvidedByInstantCamera);
		m_triggerMode = 2;
		break;
	case 3:
		m_camera.TriggerSelector.SetValue(Basler_GigECamera::TriggerSelector_FrameStart);
		m_camera.TriggerMode.SetValue(Basler_GigECamera::TriggerMode_On);
		m_camera.TriggerSource.SetValue(Basler_GigECamera::TriggerSource_Line1);
		m_camera.TriggerActivation.SetValue(Basler_GigECamera::TriggerActivation_RisingEdge);
		if (!m_camera.IsGrabbing())
		{
			m_camera.StartGrabbing(Pylon::GrabStrategy_OneByOne, Pylon::GrabLoop_ProvidedByInstantCamera);
		}
		//m_camera.StartGrabbing(Pylon::GrabStrategy_OneByOne, Pylon::GrabLoop_ProvidedByInstantCamera);
		m_triggerMode = 3;
		break;
	case 4:
		m_camera.TriggerSelector.SetValue(Basler_GigECamera::TriggerSelector_FrameStart);
		m_camera.TriggerMode.SetValue(Basler_GigECamera::TriggerMode_On);
		m_camera.TriggerSource.SetValue(Basler_GigECamera::TriggerSource_Line1);
		m_camera.TriggerActivation.SetValue(Basler_GigECamera::TriggerActivation_FallingEdge);
		if (!m_camera.IsGrabbing())
		{
			m_camera.StartGrabbing(Pylon::GrabStrategy_OneByOne, Pylon::GrabLoop_ProvidedByInstantCamera);
		}
		//m_camera.StartGrabbing(Pylon::GrabStrategy_OneByOne, Pylon::GrabLoop_ProvidedByInstantCamera);
		m_triggerMode = 4;
		break;
	default:
		break;
	}
	if (mode != 1)
	{
		m_isContinous = false;
	}
	return 0;
}
int BaslerCamera::GetExposureTime(int& exposureTime)
{
	if (!m_camera.IsOpen())
	{
		return 1;
	}
	exposureTime = m_camera.ExposureTimeRaw.GetValue();
	return 0;
}
int BaslerCamera::GetGain(int& gain)
{
	if (!m_camera.IsOpen())
	{
		return 1;
	}
	gain = m_camera.GainRaw.GetValue();
	return 0;
}
int BaslerCamera::GetTriggerMode(int& mode)
{
	if (!m_camera.IsOpen())
	{
		return 1;
	}
	mode = m_triggerMode;
	return 0;
}
int BaslerCamera::SetExposureTime(const int exposureTime)
{
	if (!m_camera.IsOpen())
	{
		return 1;
	}
	if (exposureTime < m_camera.ExposureTimeAbs.GetMin() || exposureTime > m_camera.ExposureTimeAbs.GetMax())
	{
		return 2;
	}
	m_camera.ExposureTimeRaw.SetValue(exposureTime);
	return 0;
}
int BaslerCamera::SetGain(const int gain)
{
	if (!m_camera.IsOpen())
	{
		return 1;
	}
	if (gain < m_camera.GainRaw.GetMin() || gain > m_camera.GainRaw.GetMax())
	{
		return 2;
	}
	m_camera.GainRaw.SetValue(gain);
	return 0;
}