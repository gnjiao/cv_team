#pragma once
#include <boost\archive\text_iarchive.hpp>
#include <boost\archive\text_oarchive.hpp>

struct DilateParam
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & structElementShape;
		ar & structElementSize;
		ar & iteration;
	}
public:
	int structElementShape = 0;
	int structElementSize = 1;
	int iteration = 1;
};

struct ErodeParam
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & structElementShape;
		ar & structElementSize;
		ar & iteration;
	}
public:
	int structElementShape = 0;
	int structElementSize = 1;
	int iteration = 1;
};

struct AdvancedMorphologyParam
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & morphType;
		ar & structElementShape;
		ar & structElementSize;
		ar & iteration;
	}
public:
	int morphType = 0;
	int structElementShape = 0;
	int structElementSize = 1;
	int iteration = 1;
};