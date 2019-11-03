#pragma once
#include "CameraImp.h"
#include "pylon/PylonIncludes.h"
#include <pylon/gige/BaslerGigEInstantCamera.h>
#include <pylon/gige/_BaslerGigECameraParams.h>
#include <pylon/gige/BaslerGigEInstantCamera.h>
class BaslerCamera :
	public Pylon::CImageEventHandler, public CameraImp
{
public:
	BaslerCamera(void);
	~BaslerCamera(void);
	static int EnumerateDevices(std::vector<std::string>& IDs);
	std::string GetVendorString()const { return "Basler"; }
	int Open();
	bool IsOpen();
	const unsigned char* const GetImageData(int& imageSize);
	int GetImageWith(int& width);
	int GetImageHeight(int& height);
	int GetExposureTime(int& exposureTime);
	int GetGain(int& gain);
	int GetTriggerMode(int& mode);
	int SetExposureTime(const int exposureTime);
	int SetGain(const int gain);
	int SetTriggerMode(const int mode);
	int ExcuteSoftwareTrigger();
	void Close();
private:
	Pylon::CBaslerGigEInstantCamera m_camera;
	static Pylon::DeviceInfoList_t m_devices;
	static Pylon::CTlFactory* m_tlFactory;
	unsigned char* m_imageData;
	int m_imageSize;
	std::mutex m_mtx;
	//bool m_bImageUpdate;
	virtual void OnImageGrabbed(Pylon::CInstantCamera& camera, const Pylon::CGrabResultPtr& ptrGrabResult);
};