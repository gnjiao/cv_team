#pragma once
#include <vector>
#include <memory>
#include "BaslerCamera.h"
//#include "HikCamera.h"
class CameraFactory
{
private:
	CameraFactory(void);
	~CameraFactory(void);
public:
	static CameraFactory* Instance()
	{
		static CameraFactory instance;
		return &instance;
	}
	std::shared_ptr<CameraImp> CreateCamera(const CameraVendor vendor);//
};