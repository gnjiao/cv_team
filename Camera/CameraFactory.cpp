#include "CameraFactory.h"
CameraFactory::CameraFactory(void)
{
}
CameraFactory::~CameraFactory(void)
{
}
std::shared_ptr<CameraImp> CameraFactory::CreateCamera(const CameraVendor vendor)
{
	std::shared_ptr<CameraImp> cameraImp = nullptr;
	switch (vendor)
	{
	case CameraVendor::Basler:
		cameraImp = std::make_shared<BaslerCamera>();
		break;
	case CameraVendor::Hik:
		//cameraImp = std::make_shared<HikCamera>();
		break;
	}
	cameraImp->SetVendor(vendor);
	return cameraImp;
}