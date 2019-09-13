#pragma once
#include <boost\archive\text_iarchive.hpp>
#include <boost\archive\text_oarchive.hpp>
#include <boost\serialization\map.hpp>
#include <map>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <direct.h>
using namespace std;
template<class TEntity>
class EntityManager
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_map;
	}
private:
	map<int, TEntity*> m_map;
	std::string config_dir = "CVTEAM_Config";
public:
	EntityManager() {}
	~EntityManager() {}
	static EntityManager* Instance()
	{
		static EntityManager instance;
		return &instance;
	}
	TEntity* Insert(int key, TEntity* pEntity)
	{
		m_map.insert(make_pair(key, pEntity));
		return pEntity;
	}
	void Remove(int key)
	{
		map<int, TEntity*>::iterator it = m_map.find(key);
		if (it != m_map.end())
		{
			delete it->second;
			it->second = nullptr;
			m_map.erase(m_map.find(key));
		}
	}
	TEntity* FindBy(int key)const
	{
		map<int, TEntity*>::const_iterator it = m_map.find(key);
		if (it == m_map.end())
		{
			return nullptr;
		}
		return it->second;
	}
	vector<TEntity*> FindAll()const
	{
		vector<TEntity*> entities;
		for (map<int, TEntity*>::const_iterator it = m_map.begin(); it != m_map.end(); it++)
		{
			entities.push_back(it->second);
		}
		return entities;
	}
	void Clear()
	{
		map<int, TEntity*>::iterator it = m_map.begin();
		while (it != m_map.end())
		{
			delete it->second;
			it->second = nullptr;
			m_map.erase(it++);
		}
	}
	int Size()const
	{
		return m_map.size();
	}
	int MaxKey()const
	{
		int max = -1;
		for (map<int, TEntity*>::const_iterator it = m_map.begin(); it != m_map.end(); it++)
		{
			if (it->first < 100)
			{
				if (max < it->first)
				{
					max = it->first;
				}
			}
		}
		return max;
	}
	void Save()
	{
		_mkdir(config_dir.c_str());
		string m_fileName = config_dir + "\\" + typeid(TEntity).name();
		std::ofstream ofs;
		ofs.open(m_fileName.c_str(), ofstream::out);
		if (ofs)
		{
			boost::archive::text_oarchive oa(ofs);
			oa << m_map;
		}
		ofs.close();
	}
	void Load()
	{
		_mkdir(config_dir.c_str());
		string m_fileName = config_dir + "\\" + typeid(TEntity).name();
		std::ifstream ifs(m_fileName);
		if (ifs)
		{
			boost::archive::text_iarchive ia(ifs);
			ia >> m_map;
		}
		ifs.close();
	}
};