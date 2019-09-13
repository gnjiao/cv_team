#pragma once
#include <fstream>
#include <boost\serialization\export.hpp>
#include <boost\serialization\vector.hpp>
#include "DataModel\BaseNode.h"
#include "DataModel\ToolDataMap.h"
#include "Inspection\Inspection.h"
#include "geo/Line2D.h"
#include "geo/Circle2D.h"
#include <opencv.hpp>

using namespace geo;

namespace toolModel
{
	class BaseOperator : public BaseNode
	{
	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & boost::serialization::base_object<BaseNode>(*this);
			ar & m_inspectionKey;
			ar & m_maxDataKey;
		}
	public:
		BaseOperator()
		{
			m_output = new ToolDataMap();
		}
		~BaseOperator()
		{
			delete m_output;
		}
		void setInspectionKey(int key) { m_inspectionKey = key; }
		int getInspectionKey()const { return m_inspectionKey; }
		void save()
		{
			Inspection* inspection = InspectionMGR->FindBy(m_inspectionKey);
			if (inspection == nullptr)
				return;
			string fileName = inspection->GetToolBlockFileName();
			std::ofstream ofs;
			ofs.open(fileName.c_str(), ofstream::out);
			if (ofs)
			{
				boost::archive::text_oarchive oa(ofs);
				oa << inspection->m_toolBlock;
			}
			ofs.close();
		}
		string nodeDirectory()const
		{
			return InspectionMGR->FindBy(m_inspectionKey)->GetDir() + "\\"
				+ this->GetNodeName() + to_string(this->GetNodeKey());
		}
		virtual BaseNode* clone() = 0;
		virtual NodeState execute() = 0;
		virtual std::string getResultString() = 0;
		virtual void resetOutputValue() = 0;
		ToolDataMap* output()const { return m_output; }
		virtual void initOutput() = 0;
		virtual void registerOutput() = 0;
		virtual void setCoreParams() = 0;
		virtual void drawFeatures(cv::Mat);
		int maxDataKey()const { return m_maxDataKey; }
		Point2D transPoint2D(Point2D it);
		Line2D transLine2D(Line2D it);
		Circle2D transCircle2D(Circle2D it);
		double transValue(double it);
		double transArea(double it);
	protected:
		template<typename T>
		ToolData<T>* createOutput(ToolDataType dataType)
		{
			ToolData<T>* data = new ToolData<T>();
			data->SetInspectionKey(m_inspectionKey)
				->SetNodeKey(this->GetNodeKey())
				->SetDataName(this->GetNodeName())
				->SetDataType(dataType);
			data->SetDataKey(m_maxDataKey);
			m_output->Add(data, dataType);
			m_maxDataKey++;
			switch (dataType)
			{
			case ToolDataType::eInt:
			case ToolDataType::eDouble:
			case ToolDataType::eBool:
			case ToolDataType::eArea:
			case ToolDataType::eString:
			case ToolDataType::ePoint:
			case ToolDataType::eLine:
			case ToolDataType::eCircle:
			case ToolDataType::eLength:
			case ToolDataType::eAngle:
				data->SetOutputEnabled(true);
				break;
			default:
				break;
			}
			switch (dataType)
			{
			case ToolDataType::ePoint:
			case ToolDataType::eLine:
			case ToolDataType::eCircle:
				data->SetDisplayEnabled(true);
				data->SetDisplayChecked(false);
				break;
			default:
				break;
			}
			return data;
		}
		template<typename T>
		ToolData<T>* createClone(ToolData<T> *orinal)
		{
			if (orinal == nullptr)
				return nullptr;
			ToolData<T>* data = new ToolData<T>();
			*data = *orinal;
			return data;
		}
	private:
		int m_inspectionKey = -1;
		ToolDataMap* m_output = nullptr;
		int m_maxDataKey = 0;
	};
	enum MatType
	{
		Reference,
		Current
	};
}