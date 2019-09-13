#include "FindCircleOperator.h"
#include <QDebug>
#include <Windows.h>

FindCircleOperator::FindCircleOperator()
{
	m_nodeName = "findCircle";
	m_showText = "findCircle";
	m_caliper = new Calipers();
	m_findcircle = new FindCircle();
}
FindCircleOperator::~FindCircleOperator()
{
	delete output_circle;
	delete output_center;
	delete output_radius;
	delete output_diameter;
	delete output_circumference;
	delete m_caliper;
	delete m_findcircle;
}
void FindCircleOperator::initOutput()
{
	output_circle = createOutput<geo::Circle2D>(toolModel::eCircle)->SetDisplayEnabled(true);
	output_center = createOutput<geo::Point2D>(toolModel::ePoint)->SetDisplayEnabled(true);
	output_radius = createOutput<double>(toolModel::eLength);
	output_diameter = createOutput<double>(toolModel::eLength);
	output_circumference = createOutput<double>(toolModel::eLength);
	output_area = createOutput<double>(toolModel::eArea);
	output_circle->SetShowText("FindCircle-Circle");
	output_center->SetShowText("FindCircle-Center");
	output_radius->SetShowText("FindCircle-Radius");
	output_diameter->SetShowText("FindCircle-Diameter");
	output_circumference->SetShowText("FindCircle-Circumference");
	output_area->SetShowText("FindCircle-Area");
}
void FindCircleOperator::registerOutput()
{
	output()->Add(output_circle, ToolDataType::eCircle);
	output()->Add(output_center, ToolDataType::ePoint);
	output()->Add(output_radius, ToolDataType::eLength);
	output()->Add(output_diameter, ToolDataType::eLength);
	output()->Add(output_circumference, ToolDataType::eLength);
	output()->Add(output_area, ToolDataType::eArea);
	//output_circle->SetShowText(TOOLBOXMGR->findCircle.map[TOOLBOXMGR->findCircle.circle]);
	//output_center->SetShowText(TOOLBOXMGR->findCircle.map[TOOLBOXMGR->findCircle.center]);
	//output_radius->SetShowText(TOOLBOXMGR->findCircle.map[TOOLBOXMGR->findCircle.radius]);
	//output_diameter->SetShowText(TOOLBOXMGR->findCircle.map[TOOLBOXMGR->findCircle.diameter]);
	//output_circumference->SetShowText(TOOLBOXMGR->findCircle.map[TOOLBOXMGR->findCircle.circumference]);
	//output_area->SetShowText(TOOLBOXMGR->findCircle.map[TOOLBOXMGR->findCircle.area]);
}
toolModel::NodeState FindCircleOperator::execute()
{
	if (input_mat == nullptr)
	{
		m_state = NodeState::Idel;
		return m_state;
	}
	if (m_divideRects.size() == 0)
	{
		m_state = NodeState::Idel;
		return NodeState::Idel;
	}
	m_state = NodeState::Running;
	m_errorCode = ErrorCode::Null;

	vector<geo::RotRect2D> rects = m_divideRects;
	if (input_affineTrans != nullptr)
	{
		AffineTrans trans = input_affineTrans->GetValue();
		rects.clear();
		for each (auto var in m_divideRects)
		{
			rects.push_back(var.TranslateBy(trans.Translation.ToVector2D())
				.RotateBy(trans.Center, trans.Rotation));
		}
	}
	m_state = test(input_mat->GetValue(), rects);
	return m_state;
}
string FindCircleOperator::getResultString()
{
	string str;
	if (output_circle->GetOutputEnabled() && output_circle->GetOutputChecked())
	{
		str += transCircle2D(output_circle->GetValue()).ToString();
	}
	if (output_center->GetOutputEnabled() && output_center->GetOutputChecked())
	{
		if ("" != str)
			str += ",";
		str += transPoint2D(output_center->GetValue()).ToString();
	}
	if (output_radius->GetOutputEnabled() && output_radius->GetOutputChecked())
	{
		if ("" != str)
			str += ",";
		str += to_string(transValue(output_radius->GetValue()));
	}
	if (output_diameter->GetOutputEnabled() && output_diameter->GetOutputChecked())
	{
		if ("" != str)
			str += ",";
		str += to_string(transValue(output_diameter->GetValue()));
	}
	if (output_circumference->GetOutputEnabled() && output_circumference->GetOutputChecked())
	{
		if ("" != str)
			str += ",";
		str += to_string(transValue(output_circumference->GetValue()));
	}
	if (output_area->GetOutputEnabled() && output_area->GetOutputChecked())
	{
		if ("" != str)
			str += ",";
		str += to_string(transValue(output_area->GetValue()));
	}
	return str;
}
BaseNode * FindCircleOperator::clone()
{
	FindCircleOperator *newnode = new FindCircleOperator();
	newnode->setPolarProperty(this->getPolarProperty());
	newnode->setResultType(this->getResultType());
	newnode->setGradThresh(this->getGradThresh());
	newnode->setFitType(this->getFitType());
	newnode->setMaxIter(this->getMaxIter());
	newnode->setDistThresh(this->getDistThresh());
	newnode->setDivideCounts(this->getDivideCounts());
	newnode->setCaliperWidth(this->getCaliperWidth());
	newnode->setCaliperLength(this->getCaliperLength());
	newnode->setDivideRects(this->getDivideRects());
	newnode->setDeleteCount(this->getDeleteCount());
	newnode->input_mat = createClone<cv::Mat>(this->input_mat);
	newnode->input_refMat = createClone<cv::Mat>(this->input_refMat);
	newnode->input_affineTrans = createClone<AffineTrans>(this->input_affineTrans);
	newnode->initOutput();
	newnode->setCoreParams();
	return newnode;
}
toolModel::NodeState FindCircleOperator::test(cv::Mat src, vector<geo::RotRect2D> rects)
{
	cv::Mat srcImage;
	if (src.channels() > 1)
	{
		cv::cvtColor(src, srcImage, cv::COLOR_RGB2GRAY);
	}
	else
	{
		src.copyTo(srcImage);
	}
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	m_state = _test(srcImage, rects);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	setDuration(duration_cast<chrono::microseconds>(t2 - t1).count() / 1000.0);
	return m_state;
}
toolModel::NodeState FindCircleOperator::_test(cv::Mat src, vector<geo::RotRect2D> rects)
{
	if (!src.data)
	{
		m_errorCode = ErrorCode::NG_Mat_Null;
		return NodeState::Failure;
	}
	
	m_pointsOK.clear();
	m_pointsNG.clear();
	vector<cv::Point2d> inPoints; 
	vector<vector<cv::Point2d>> allPoints; 
	vector<cv::Point2d> outPoints(3);
	vector<cv::Point2d> fitPoints;
	m_caliper->SetImage(src);
	setCoreParams();
	geo::Point2D limitCorner = geo::Point2D(src.cols, src.rows); int rtn;
	int realDeleteCounts = m_deleteCount;
	allPoints.clear();
	for (int i = 0; i < m_divideCounts; i++)
	{
		if (rects[i].Point0().X() >= 0
			&& rects[i].Point0().X() < limitCorner.X()
			&& rects[i].Point1().X() >= 0
			&& rects[i].Point1().X() < limitCorner.X()
			&& rects[i].Point2().X() >= 0
			&& rects[i].Point2().X() < limitCorner.X()
			&& rects[i].Point3().X() >= 0
			&& rects[i].Point3().X() < limitCorner.X()
			&& rects[i].Point0().Y() >= 0
			&& rects[i].Point0().Y() < limitCorner.Y()
			&& rects[i].Point1().Y() >= 0
			&& rects[i].Point1().Y() < limitCorner.Y()
			&& rects[i].Point2().Y() >= 0
			&& rects[i].Point2().Y() < limitCorner.Y()
			&& rects[i].Point3().Y() >= 0
			&& rects[i].Point3().Y() < limitCorner.Y()
			)
		{
			inPoints.clear();
			outPoints.clear();
			inPoints.push_back(cv::Point2d(rects[i].Point0().X(), rects[i].Point0().Y()));
			inPoints.push_back(cv::Point2d(rects[i].Point1().X(), rects[i].Point1().Y()));
			inPoints.push_back(cv::Point2d(rects[i].Point2().X(), rects[i].Point2().Y()));
			inPoints.push_back(cv::Point2d(rects[i].Point3().X(), rects[i].Point3().Y()));
			m_caliper->SetImage(src);
			rtn = m_caliper->Caliper(inPoints, outPoints);
			allPoints.push_back(outPoints);
			if (rtn == 0)
			{
				fitPoints.push_back(outPoints[2]);
			}
			else { realDeleteCounts--; }
		}
		else { realDeleteCounts--; }
	}
	if (fitPoints.size() < allPoints.size() / 2)
	{
		m_errorCode = ErrorCode::NG_Caliper_Count;
		return NodeState::Failure;
	}
	//m_findcircle->FitCircle(fitPoints, m_circle_para, m_pointsOK, m_pointsNG, m_deleteCount);
	if (realDeleteCounts < 0) { realDeleteCounts = 0; }
	m_findcircle->FitCircle(fitPoints, m_circle_para, m_pointsOK, m_pointsNG, realDeleteCounts);
	Circle2D c = geo::Circle2D(Point2D(m_circle_para.x, m_circle_para.y), m_circle_para.z);
	//output
	output_area->SetValue(c.Area());
	output_center->SetValue(c.Center());
	output_circle->SetValue(c);
	output_circumference->SetValue(c.Circumference());
	output_diameter->SetValue(c.Diameter());
	output_radius->SetValue(c.Radius());
	m_errorCode = ErrorCode::Null;
	return NodeState::Success;
}
vector<geo::Point2D> FindCircleOperator::getPointsOK()
{
	int counts = m_pointsOK.size();
	vector<geo::Point2D> ptsOK;
	ptsOK.clear();
	for (int i = 0; i < counts; i++)
	{
		ptsOK.push_back(geo::Point2D(m_pointsOK[i].x, m_pointsOK[i].y));
		qDebug() << "pointOK X:" << m_pointsOK[i].x << "pointOK Y:" << m_pointsOK[i].y;
	}
	return ptsOK;
}
vector<geo::Point2D> FindCircleOperator::getPointsNG()
{
	int counts = m_pointsNG.size();
	vector<geo::Point2D> ptsNG;
	ptsNG.clear();
	for (int i = 0; i < counts; i++)
	{
		ptsNG.push_back(geo::Point2D(m_pointsNG[i].x, m_pointsNG[i].y));
	}
	return ptsNG;
}
void FindCircleOperator::resetOutputValue()
{
	if (nullptr != output_circle)
		output_circle->SetValue(Circle2D());
	if (nullptr != output_center)
		output_center->SetValue(Point2D());
	if (nullptr != output_radius)
		output_radius->SetValue(0);
	if (nullptr != output_diameter)
		output_diameter->SetValue(0);
	if (nullptr != output_circumference)
		output_circumference->SetValue(0);
	if (nullptr != output_area)
		output_area->SetValue(0);
}