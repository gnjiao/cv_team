#pragma once
#include <boost\archive\text_iarchive.hpp>
#include <boost\archive\text_oarchive.hpp>
#include <boost\serialization\base_object.hpp>
#include <opencv2\core\core.hpp>
#include <iostream>
#include <thread>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include "Inspection\EntityManager.h"
#define AcquisitionManager EntityManager<Acquisition>::Instance()

using namespace std;

class Acquisition
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_key;
		ar & m_cameraKey;
		ar & m_exposure;
		ar & m_gain;
	}
private:
	int m_key = -1;
	int m_cameraKey = -1;
	int m_exposure = 100;
	int m_gain = 0;
	cv::Mat m_referenceMat;
	cv::Mat m_currentMat;
public:
	Acquisition() {}
	Acquisition(int key) { m_key = key; }
	~Acquisition() {}
	Acquisition* SetCameraKey(int key) { m_cameraKey = key; return this; }
	int GetCameraKey()const { return m_cameraKey; }
	Acquisition* SetExposure(int exposure) { m_exposure = exposure; return this; }
	int GetExposure()const { return m_exposure; }
	Acquisition* SetGain(int gain) { m_gain = gain; return this; }
	int GetGain()const { return m_gain; }
	Acquisition* SetCurrentMat(cv::Mat image) { m_currentMat = image; return this; }
	cv::Mat GetCurrentMat()const { return m_currentMat; }
	Acquisition* SetReferenceMat(cv::Mat image) { m_referenceMat = image; return this; }
	cv::Mat GetReferenceMat()const { return m_referenceMat; }
	Acquisition* SaveReferenceMat(string fileName);
	cv::Mat LoadReferenceMat(string fileName);
	cv::Mat FromFile(string fileName);
	cv::Mat FromCamera();
};

