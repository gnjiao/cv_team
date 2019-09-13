#pragma once
#include <boost\archive\text_iarchive.hpp>
#include <boost\archive\text_oarchive.hpp>
struct BlurSmoothParam
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & kernelSize;
	}
public:
	int kernelSize = 3;
};
struct GaussianSmoothParam
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & kernelSize;
		ar & sigmaX;
		ar & sigmaY;
	}
public:
	int kernelSize = 3;
	double sigmaX = 0;
	double sigmaY = 0;
};
struct BoxSmoothParam
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & kernelSize;
		ar & normalized;
	}
public:
	int kernelSize = 3;
	bool normalized = true;
};
struct BilateralSmoothParam
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & sigmaColor;
		ar & sigmaSpace;
		ar & d;
	}
public:
	double sigmaColor = 25 * 2;
	double sigmaSpace = 25 / 2.0;
	int d = 25;
};
struct MedianSmoothParam
{
private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & kernelSize;
	}
public:
	int kernelSize = 3;
};