#pragma once
#include "DataModel\ToolDataMap.h"

using namespace toolModel;

using namespace std;

ToolDataMap::ToolDataMap()
{

}





ToolDataMap::~ToolDataMap()
{

}



ToolDataMap* ToolDataMap::Add(ToolData<int>* i, ToolDataType type)
{

	if (i)i->SetIndex(vectorInt.size());

	vectorInt.push_back(i);

	return this;

}



ToolDataMap* ToolDataMap::Add(ToolData<double>* d, ToolDataType type)
{

	switch (type)

	{

	case toolModel::eDouble:

		if (d)d->SetIndex(vectorDouble.size());

		vectorDouble.push_back(d);

		break;

	case toolModel::eLength:

		if (d)d->SetIndex(vectorLength.size());

		vectorLength.push_back(d);

		break;

	case toolModel::eArea:

		if (d)d->SetIndex(vectorArea.size());

		vectorArea.push_back(d);

		break;

	case toolModel::eScore:

		if (d)d->SetIndex(vectorScore.size());

		vectorScore.push_back(d);

		break;

	default:

		break;

	}

	return this;

}



ToolDataMap* ToolDataMap::Add(ToolData<bool>* b, ToolDataType type)
{

	if (b)b->SetIndex(vectorBool.size());

	vectorBool.push_back(b);

	return this;

}



ToolDataMap* ToolDataMap::Add(ToolData<std::string>* s, ToolDataType type)
{

	if (s)s->SetIndex(vectorString.size());

	vectorString.push_back(s);

	return this;

}



ToolDataMap* ToolDataMap::Add(ToolData<cv::Mat>* m, ToolDataType type)
{

	switch (type)

	{

	case toolModel::eMat:

		if (m)m->SetIndex(vectorMat.size());

		vectorMat.push_back(m);

		break;

	case toolModel::eRefMat:

		if (m)m->SetIndex(vectorRefMat.size());

		vectorRefMat.push_back(m);

		break;

	case toolModel::eRegion:

		if (m)m->SetIndex(vectorRegion.size());

		vectorRegion.push_back(m);

		break;

	case toolModel::eFeaturePoints:

		if (m)m->SetIndex(vectorFeaturePoints.size());

		vectorFeaturePoints.push_back(m);

		break;

	}

	return this;

}



ToolDataMap* toolModel::ToolDataMap::Add(ToolData<Angle>* a, ToolDataType type)
{

	if (a)a->SetIndex(vectorPoint.size());

	vectorAngle.push_back(a);

	return this;

}



ToolDataMap* toolModel::ToolDataMap::Add(ToolData<Point2D>* p, ToolDataType type)
{

	if (p)p->SetIndex(vectorPoint.size());

	vectorPoint.push_back(p);

	return this;

}



ToolDataMap* toolModel::ToolDataMap::Add(ToolData<Line2D>* l, ToolDataType type)
{

	if (l)l->SetIndex(vectorLine.size());

	vectorLine.push_back(l);

	return this;

}



ToolDataMap* toolModel::ToolDataMap::Add(ToolData<Circle2D>* c, ToolDataType type)
{

	if (c)c->SetIndex(vectorCircle.size());

	vectorCircle.push_back(c);

	return this;

}



ToolDataMap* toolModel::ToolDataMap::Add(ToolData<AffineTrans>* a, ToolDataType type)
{

	if (a)a->SetIndex(vectorAffineTrans.size());

	vectorAffineTrans.push_back(a);

	return this;

}