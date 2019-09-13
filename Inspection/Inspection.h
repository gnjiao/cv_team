#pragma once
#include <boost\archive\text_iarchive.hpp>
#include <boost\archive\text_oarchive.hpp>
#include <boost\serialization\base_object.hpp>
#include "Acquisition\Acquisition.h"
#include <string>
#include "EntityManager.h"
#include "ToolBlock.h"

#define InspectionMGR EntityManager<Inspection>::Instance()
using namespace std;
class Inspection
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_key;
		ar & m_triggerSignal;
		ar & m_name;
		ar & m_cameraKey;
		ar & m_acquisition;
		ar & m_calibrationKey;
		ar & m_dwellTime;
		ar & m_displayId;
		ar & m_result;
	}
private:
	string config_dir = "CVTEAM_Config";
	int m_key = -1;
	string m_triggerSignal;
	string m_name;
	int m_cameraKey = -1;
	Acquisition* m_acquisition = nullptr;
	int m_calibrationKey = -1;
	int m_dwellTime = 0;
	int m_displayId = 0;
	string m_result;
public:
	Inspection() {
		m_acquisition = new Acquisition();
		m_toolBlock = new ToolBlock();
	}
	Inspection(int key)
	{
		m_key = key;
		m_acquisition = new Acquisition();
		m_toolBlock = new ToolBlock();
		SetTriggerSignal("t" + to_string(m_key));
		m_displayId = m_key;
		m_name = "Inspection" + to_string(m_key);
	}
	~Inspection()
	{
		if (m_acquisition)
		{
			delete m_acquisition;
			m_acquisition = nullptr;
		}
		if (m_toolBlock != nullptr)
		{
			delete m_toolBlock;
			m_toolBlock = nullptr;
		}
	}
	ToolBlock* m_toolBlock = nullptr;
	Inspection* SetTriggerSignal(string signal) { m_triggerSignal = signal; return this; }
	string GetTriggerSignal()const { return m_triggerSignal; }
	Inspection* SetDisplayId(int id) { m_displayId = id; return this; }
	int GetDisplayId()const { return m_displayId; }
	Inspection* SetName(string name) { m_name = name; return this; }
	string GetName()const { return m_name; }
	void MakeDir();
	string GetDir()const { return config_dir + "\\" + m_name; }
	string GetRefImageFileName()const { return config_dir + "\\" + m_name + "\\referenceImage.jpg"; }
	string GetToolBlockFileName()const { return config_dir + "\\" + m_name + "\\" + m_name + ".cvteam"; }
	Inspection* SetCameraKey(int key) { m_cameraKey = key; return this; }
	int GetCameraKey()const { return m_cameraKey; }
	Inspection* SetCalibrationKey(int key) { m_calibrationKey = key; return this; }
	int GetCalibrationKey()const { return m_calibrationKey; }
	Inspection* SetReuslt(string result) { m_result = result; return this; }
	string GetResult()const { return m_result; }
	Acquisition* GetAcquisition()const { return m_acquisition; }
	NodeState execute();
	int GetKey()const { return m_key; }
};