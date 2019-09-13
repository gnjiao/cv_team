#pragma once
#include <boost\archive\text_iarchive.hpp>
#include <boost\archive\text_oarchive.hpp>

struct ThresholdFixParam
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & thresholdValue;
		ar & maxValue;
		ar & fixType;
	}
public:
	int thresholdValue = 100;
	int maxValue = 255;
	int fixType = 0;
};

struct ThresholdAutoParam
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & maxValue;
		ar & autoType;
		ar & fixType;
		ar & blockSize;
		ar & C;
	}
public:
	int maxValue = 255;
	int autoType = 0;
	int fixType = 0;
	int blockSize = 5;
	double C = 10;
};

struct ThresholdSegmentParam
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & baseValue;
		ar & maxValue;
	}
public:
	int baseValue = 100;
	int maxValue = 255;
};

struct ThresholdTwoPeakParam
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & maxValue;
		ar & fixType;
	}
public:
	int maxValue = 255;
	int fixType = 0;
};