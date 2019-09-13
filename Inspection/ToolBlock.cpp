#include "ToolBlock.h"
#include "DataModel\BaseOperator.h"
void ToolBlock::Reset()
{
	for each (auto var in m_children)
	{
		var->Reset();
	}
}
int ToolBlock::NewKey() const
{
	return this->MaxKey() + 1;
}
int ToolBlock::MaxKey() const
{
	int maxKey = -1;
	for each (auto var in m_children)
	{
		if (var->GetNodeKey() > maxKey)
		{
			maxKey = var->GetNodeKey();
		}
	}
	return maxKey;
}
void ToolBlock::AddChild(BaseNode* node, bool isNew)
{
	if (isNew)
	{
		node->SetNodeKey(this->NewKey());
	}
	this->m_children.push_back(node);
}
void ToolBlock::InsertChild(int index, BaseNode* node, bool isNew)
{
	std::vector<BaseNode*>::iterator it;
	if (index >= this->m_children.size())
		it = m_children.end();
	else
		it = this->m_children.begin() + index;
	if (isNew)
	{
		node->SetNodeKey(this->NewKey());
	}
	this->m_children.insert(it, node);
}
void ToolBlock::Swap(int left, int right)
{
	std::swap(m_children[left], m_children[right]);
}
void ToolBlock::RemoveChildByKey(int key)
{
	for (size_t i = 0; i < m_children.size(); i++)
	{
		if (m_children[i]->GetNodeKey() == key)
		{
			std::vector<BaseNode*>::iterator it = m_children.begin() + i;
			/*delete m_children[i];
			m_children[i] == nullptr;*/
			m_children.erase(it);
		}
	}
}
BaseNode* ToolBlock::GetChild(int index)
{
	return m_children.at(index);
}
BaseNode* ToolBlock::FindChildByKey(int key)
{
	for (size_t i = 0; i < m_children.size(); i++)
	{
		if (m_children[i]->GetNodeKey() == key)
		{
			return m_children[i];
		}
	}
	return nullptr;
}
void ToolBlock::Clear()
{
	this->m_children.clear();
}
NodeState ToolBlock::execute()
{
	result = "";
	m_state = NodeState::Running;
	for each (auto var in m_children)
	{
		if (var->execute() != NodeState::Success)
		{
			m_state = NodeState::Failure;
			result = "NG";
			return NodeState::Failure;
		}
		string temp = ((BaseOperator*)var)->getResultString();
		if (0 == result.size())
		{
			result = temp;
			continue;
		}
		if (temp.size() > 0)
		{
			result += +"," + temp;
		}
	}
	m_state = NodeState::Success;
	return m_state;
}
BaseNode * ToolBlock::clone()
{
	ToolBlock *newnode = new ToolBlock;
	for each (auto var in m_children)
	{
		newnode->AddChild(var->clone(), true);
	}
	return newnode;
}
std::string ToolBlock::getResult()
{
	return result;
}