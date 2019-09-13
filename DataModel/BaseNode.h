#pragma once

#include <boost\archive\text_iarchive.hpp>

#include <boost\archive\text_oarchive.hpp>

#include <boost\serialization\base_object.hpp>

#include <chrono>

#include <string>

#include <typeinfo>

/*
******每一个算子都是一个节点，这个是基础节点，被其他节点继承
******
******
******
******
******
******
*/

using namespace std::chrono;

namespace toolModel

{

	enum NodeState

	{

		Idel,

		Success,

		Failure,

		Running

	};



	enum NodeType

	{

		Base,

		Parent,

		Sequence,

		Selector,

		For,

		IfElse,

		Inverse,

		Leaf

	};



	class BaseNode

	{

	private:

		friend class boost::serialization::access;

		template<class Archive>

		void serialize(Archive & ar, const unsigned int version)

		{

			ar & m_nodeKey;

			ar & m_nodeType;

			ar & m_nodeName;

		}



	public:

		BaseNode()

		{

			m_nodeType = NodeType::Base;

			m_nodeName = typeid(*this).name();

		}

		BaseNode(int key)

		{

			m_nodeKey = key;

			m_nodeType = NodeType::Base;

			m_nodeName = typeid(*this).name();

		}

		~BaseNode() {}



		void SetNodeKey(int key) { m_nodeKey = key; }



		int GetNodeKey()const { return m_nodeKey; }



		NodeType GetNodeType()const { return m_nodeType; }



		std::string GetNodeName()const { return m_nodeName; }



		std::string GetNodeShowText()const

		{

			if (m_showText == "")

				return m_nodeName;

			return m_showText;

		}



		NodeState m_state = NodeState::Idel;



		NodeState GetState()const { return m_state; }



		virtual void Reset() { m_state = NodeState::Idel; }



		virtual NodeState execute() = 0;



		virtual BaseNode* clone() = 0;



		void setDuration(double d) { m_duration = d; }



		double getDuration()const { return m_duration; }



	private:

		int m_nodeKey = -1;

		double m_duration = 0;



	protected:

		NodeType m_nodeType;

		std::string m_nodeName;

		std::string m_showText;

	};

}
