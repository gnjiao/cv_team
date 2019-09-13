#pragma once

#include <boost\archive\text_iarchive.hpp>

#include <boost\archive\text_oarchive.hpp>

#include <boost\serialization\base_object.hpp>

#include <string>

#include "Inspection\Inspection.h"
/*
******每一个算子算出来的数据
******
******
******
******
******
******
*/

namespace toolModel

{
	enum ToolDataType

	{

		eInt,

		eDouble,

		eBool,

		eString,

		eMat,

		ePoint,

		eLine,

		eCircle,

		eLength,

		eArea,

		eAngle,

		eScore,

		eAffineTrans,

		eRefMat,

		eFeaturePoints,

		eRegion

	};



	template<typename T>

	class ToolData

	{

	private:

		friend class boost::serialization::access;

		template<class Archive>

		void serialize(Archive & ar, const unsigned int version)

		{

			ar & m_inspectionKey;

			ar & m_nodeKey;

			ar & m_dataName;

			ar & m_dataType;

			ar & m_dataKey;

			ar & m_index;

			ar & m_showText;

			ar & m_displayEnabled;

			ar & m_displayChecked;

			ar & m_outputEnabled;

			ar & m_outputChecked;

			ar & m_color_r;

			ar & m_color_g;

			ar & m_color_b;

		}



	private:

		int m_inspectionKey = -1;

		int m_nodeKey = -1;

		std::string m_dataName;

		ToolDataType m_dataType;

		int m_dataKey = -1;

		int m_index = 0;

		std::string m_showText = "";

		T m_value;

		bool m_displayEnabled = false;

		bool m_displayChecked = true;

		bool m_outputEnabled = false;

		bool m_outputChecked = false;

		int m_color_r = 255;

		int m_color_g = 0;

		int m_color_b = 0;


	public:

		ToolData() {}

		~ToolData() {}



		ToolData* SetInspectionKey(int key) { m_inspectionKey = key; return this; }

		int GetInspectionKey()const { return m_inspectionKey; }



		ToolData* SetNodeKey(int key) { m_nodeKey = key; return this; }

		int GetNodeKey()const { return m_nodeKey; }



		ToolData* SetDataName(std::string name) { m_dataName = name; return this; }

		std::string GetDataName()const { return m_dataName; }



		ToolData* SetDataType(ToolDataType type) { m_dataType = type; return this; }

		ToolDataType GetDataType()const { return m_dataType; }



		ToolData* SetDataKey(int dataKey) { m_dataKey = dataKey; return this; }

		int GetDataKey()const { return m_dataKey; }



		ToolData* SetIndex(int index) { m_index = index; return this; }

		int GetIndex()const { return m_index; }



		ToolData* SetValue(T value) { m_value = value; return this; }

		T GetValue()const { return m_value; }



		ToolData* SetDisplayEnabled(bool b) { m_displayEnabled = b; return this; }

		bool GetDisplayEnabled()const { return m_displayEnabled; }



		ToolData* SetDisplayChecked(bool b) { m_displayChecked = b; return this; }

		bool GetDisplayChecked()const { return m_displayChecked; }



		ToolData* SetOutputEnabled(bool b) { m_outputEnabled = b; return this; }

		bool GetOutputEnabled()const { return m_outputEnabled; }



		ToolData* SetOutputChecked(bool b) { m_outputChecked = b; return this; }

		bool GetOutputChecked()const { return m_outputChecked; }

		ToolData* SetColor(int r, int g, int b) { m_color_b = b;m_color_g = g ;m_color_r = r; return this; }
		int GetColorR() { return m_color_r; }
		int GetColorG() { return m_color_g; }
		int GetColorB() { return m_color_b; }

		std::string GetName()const

		{

			string str = "";

			switch (m_dataType)

			{

			case toolModel::eInt:

				str = "Int";

				break;

			case toolModel::eDouble:

				str = "Double";

				break;

			case toolModel::eBool:

				str = "Bool";

				break;

			case toolModel::eString:

				str = "String";

				break;

			case toolModel::eMat:

				str = "Mat";

				break;

			case toolModel::ePoint:

				str = "Point";

				break;

			case toolModel::eLine:

				str = "Line";

				break;

			case toolModel::eCircle:

				str = "Circle";

				break;

			case toolModel::eLength:

				str = "Length";

				break;

			case toolModel::eArea:

				str = "Area";

				break;

			case toolModel::eAngle:

				str = "Angle";

				break;

			case toolModel::eScore:

				str = "Score";

				break;

			case toolModel::eAffineTrans:

				str = "AffineTrans";

				break;

			case toolModel::eRefMat:

				str = "RefMat";

				break;

			case toolModel::eFeaturePoints:

				str = "FeaturePoints";

				break;

			case toolModel::eRegion:

				str = "Region";

				break;



			}

			str += to_string(m_index);

			return str;

		}
		ToolData* SetShowText(std::string text) { m_showText = text; return this; }

		std::string GetShowText()const { return m_showText; }

		std::string GetNodeShowText()const

		{

			std::string nodeText = InspectionMGR->FindBy(m_inspectionKey)->m_toolBlock->FindChildByKey(m_nodeKey)->GetNodeShowText();

			std::string str = nodeText + to_string(m_nodeKey) + " -> ";

			if (m_showText != "")

				return str + m_showText;

			return str + nodeText;
			
		}

	};

}



