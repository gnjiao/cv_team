#pragma once
#include <memory>
#include <mutex>
#include <vector>
#include <string>
#include "CameraImp.h"
#include "CameraFactory.h"
#include "Inspection\EntityManager.h"
using namespace std;
#define CameraManager EntityManager<Camera>::Instance()
//#define CameraManager Camera::Instance()
enum CameraTriggerMode
{
	Continue = 1,
	Soft = 2,
	PLS = 3,
	PLF = 4
};
class Camera
{
private:
	std::shared_ptr<CameraImp> m_cameraImp = nullptr;
	std::string m_name;
	CameraVendor m_vendor = CameraVendor::Basler;
	std::string m_id;
	std::string m_clientIpAddress;
	CameraTriggerMode m_triggerMode = CameraTriggerMode::PLS;
	bool m_isGrabContinous = false;
	int m_exposureTime = 0;
	int m_gain = 0;
	int m_key;
public:
	Camera()
	{
	}
	Camera(int key)
	{
		m_key = key;
	}
	~Camera()
	{
		m_cameraImp = nullptr;
	}
	//static shared_ptr<Camera>Instance();
	std::shared_ptr<CameraImp> GetCameraImp()const;
	void SetName(std::string name);
	std::string GetName()const;
	void SetVendor(CameraVendor vendor);
	CameraVendor GetVendor()const;
	std::string GetVendorString()const;
	void SetId(std::string id);
	std::string GetId()const;
	void SetClientIpAddress(std::string ip);
	std::string GetClientIpAddress()const;
	void SetTriggerMode(CameraTriggerMode mode);
	CameraTriggerMode GetTriggerMode()const;
	bool IsGrabContinous()const;
	void SetExposureTime(int time);
	int GetExposureTime()const;
	void SetGain(int gain);
	int GetGain()const;
	void SoftTrigger();
	void InitCameraImp();
	void SetSelfTriggerMode();
	void SetSelfExposureTime();
	void SetSelfGain();
	void InitCameraParams();
	void Update();
};