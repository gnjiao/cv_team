#pragma once
#include <boost\archive\text_iarchive.hpp>
#include <boost\archive\text_oarchive.hpp>
#include <boost\serialization\base_object.hpp>
#include <boost\serialization\vector.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "DataModel\BaseNode.h"

using namespace toolModel;

class ToolBlock : public toolModel::BaseNode
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<BaseNode>(*this);
		ar & m_children;
	}
public:
	ToolBlock()
	{
		m_nodeType = NodeType::Sequence;
		m_nodeName = typeid(*this).name();
	}
	~ToolBlock() {}
	std::string result;
	std::vector<BaseNode*> Children()const { return m_children; }
	void Reset();
	int NewKey()const;
	int MaxKey()const;
	void AddChild(BaseNode* node, bool isNew);
	void InsertChild(int index, BaseNode* node, bool isNew);
	void Swap(int left, int right);
	void RemoveChildByKey(int key);
	BaseNode* GetChild(int index);
	BaseNode* FindChildByKey(int key);
	void Clear();
	NodeState execute();
	BaseNode *clone();
	std::string getResult();
protected:
	std::vector<BaseNode*> m_children;
};