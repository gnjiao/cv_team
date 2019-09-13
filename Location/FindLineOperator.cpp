#include "FindLineOperator.h"
using namespace geo;
FindLineOperator::FindLineOperator()
{
	m_nodeName = "findLine";
	m_showText = "findLine";
	m_caliper = new Calipers();
	m_findline = new FindLine();
	m_aimRect = RotRect2D(Point2D(100, 100), 30, 60,Eigen::Vector2d(0, 1));
	m_errorCode = FindLineOperator::ErrorCode::Null;
}
FindLineOperator::~FindLineOperator()
{
	delete output_midPoint;
	delete output_startPoint;
	delete output_endPoint;
	delete output_line;
	delete output_length;
	delete m_caliper;
	delete m_findline;
}
vector<Point2D> FindLineOperator::getPointsOK()
{
	int counts = m_pointsOK.size();
	vector<Point2D> ptsOK;
	ptsOK.clear();
	for (int i = 0; i < counts; i++)
	{
		ptsOK.push_back(Point2D(m_pointsOK[i].x, m_pointsOK[i].y));
	}
	return ptsOK;
}
vector<Point2D> FindLineOperator::getPointsNG()
{
	int counts = m_pointsNG.size();
	vector<Point2D> ptsNG;
	ptsNG.clear();
	for (int i = 0; i < counts; i++)
	{
		ptsNG.push_back(Point2D(m_pointsNG[i].x, m_pointsNG[i].y));
	}
	return ptsNG;
}
void FindLineOperator::setPointsOK(vector<Point2D> ptsOK)
{
	vector<cv::Point2d> temp;
	for (size_t i = 0; i < ptsOK.size(); i++)
	{
		temp.push_back(cv::Point2d(ptsOK[i].X(), ptsOK[i].Y()));
	}
	this->m_pointsOK = temp;
}
void FindLineOperator::setPointNG(vector<Point2D> ptsNG)
{
	vector<cv::Point2d> temp;
	for (size_t i = 0; i < ptsNG.size(); i++)
	{
		temp.push_back(cv::Point2d(ptsNG[i].X(), ptsNG[i].Y()));
	}
	this->m_pointsNG = temp;
}
string FindLineOperator::getResultString()
{
	string str;
	if (output_midPoint->GetOutputEnabled() && output_midPoint->GetOutputChecked())
	{
		str += transPoint2D(output_midPoint->GetValue()).ToString();
	}
	if (output_startPoint->GetOutputEnabled() && output_startPoint->GetOutputChecked())
	{
		if ("" != str)
			str += ",";
		str += transPoint2D(output_startPoint->GetValue()).ToString();
	}
	if (output_endPoint->GetOutputEnabled() && output_endPoint->GetOutputChecked())
	{
		if ("" != str)
			str += ",";
		str += transPoint2D(output_endPoint->GetValue()).ToString();
	}
	if (output_line->GetOutputEnabled() && output_line->GetOutputChecked())
	{
		if ("" != str)
			str += ",";
		str += transLine2D(output_line->GetValue()).ToString();
	}
	if (output_length->GetOutputEnabled() && output_length->GetOutputChecked())
	{
		if ("" != str)
			str += ",";
		str += to_string(transValue(output_length->GetValue()));
	}
	return str;
}
BaseNode * FindLineOperator::clone()
{
	FindLineOperator *newnode = new FindLineOperator();
	newnode->setPolarProperty(this->getPolarProperty());
	newnode->setResultType(this->getResultType());
	newnode->setGradThresh(this->getGradThresh());
	newnode->setMaxIter(this->getMaxIter());
	newnode->setDistThresh(this->getDistThresh());
	newnode->setDivideCounts(this->getDivideCounts());
	newnode->setCaliperWidth(this->getCaliperWidth());
	newnode->setDivideRects(this->getDivideRects());
	newnode->setDeleteCount(this->getDeleteCount());
	newnode->setFilterType(this->m_filterType);
	newnode->setAimRect(this->getAimRect());
	newnode->input_mat = createClone<cv::Mat>(this->input_mat);
	newnode->input_refMat = createClone<cv::Mat>(this->input_refMat);
	newnode->input_affineTrans = createClone<AffineTrans>(this->input_affineTrans);
	newnode->initOutput();
	//
	newnode->setCoreParams();
	return newnode;
}
void FindLineOperator::resetOutputValue()
{
	if (nullptr != output_midPoint)
		output_midPoint->SetValue(Point2D());
	if (nullptr != output_startPoint)
		output_startPoint->SetValue(Point2D());
	if (nullptr != output_endPoint)
		output_endPoint->SetValue(Point2D());
	if (nullptr != output_line)
		output_line->SetValue(Line2D());
	if (nullptr != output_length)
		output_length->SetValue(0);
}
void FindLineOperator::initOutput()
{
	output_midPoint = createOutput<Point2D>(toolModel::ePoint);
	output_midPoint = output_midPoint->SetDisplayEnabled(true);
	output_startPoint = createOutput<Point2D>(toolModel::ePoint)->SetDisplayEnabled(true);
	output_endPoint = createOutput<Point2D>(toolModel::ePoint)->SetDisplayEnabled(true);
	output_line = createOutput<Line2D>(toolModel::eLine)->SetDisplayEnabled(true);
	output_length = createOutput<double>(toolModel::ToolDataType::eLength);
	output_midPoint->SetShowText("FindLine-MidPoint");
	output_startPoint->SetShowText("FindLine-StartPoint");
	output_endPoint->SetShowText("FindLine-EndPoint");
	output_line->SetShowText("FindLine-Line");
	output_length->SetShowText("FindLine-Length");
}
void FindLineOperator::registerOutput()
{
	//why not nullptr in output_midpoint...
	output()->Add(output_midPoint, ToolDataType::ePoint);
	output()->Add(output_startPoint, ToolDataType::ePoint);
	output()->Add(output_endPoint, ToolDataType::ePoint);
	output()->Add(output_line, ToolDataType::eLine);
	output()->Add(output_length, ToolDataType::eLength);
}
toolModel::NodeState FindLineOperator::execute()
{
	if (input_mat == nullptr)
	{
		m_state = NodeState::Idel;
		return NodeState::Idel;
	}
	if (m_divideRects.size() == 0)
	{
		m_state = NodeState::Idel;
		return NodeState::Idel;
	}
	m_state = NodeState::Running;
	m_errorCode = ErrorCode::Null;
	vector<RotRect2D> rects = m_divideRects;
	RotRect2D aimRect = m_aimRect;
	if (input_affineTrans != nullptr)
	{
		AffineTrans trans = input_affineTrans->GetValue();
		rects.clear();
		for each (auto var in m_divideRects)
		{
			rects.push_back(var.TranslateBy(trans.Translation.ToVector2D())
				.RotateBy(trans.Center, trans.Rotation));
		}
		aimRect = m_aimRect.TranslateBy(trans.Translation.ToVector2D())
			.RotateBy(trans.Center, trans.Rotation);
	}
	m_state = test(input_mat->GetValue(), rects, aimRect);
	return m_state;
}
toolModel::NodeState FindLineOperator::test(cv::Mat src, vector<RotRect2D> caliperRects, RotRect2D &aimRect)
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
	m_state = _test(srcImage, caliperRects, aimRect);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	setDuration(duration_cast<chrono::microseconds>(t2 - t1).count() / 1000.0);
	return m_state;
}
void FindLineOperator::setCoreParams()
{
	m_caliper->SetPolarProperty(m_polarProperty);
	m_caliper->SetResultType(m_resultType);
	m_caliper->SetThresholdValue(m_gradThresh);
	m_findline->SetDistThresh(m_distThresh);
	m_findline->SetFilterType(m_filterType);
	m_findline->SetMaxIter(m_maxIter);
}
toolModel::NodeState FindLineOperator::_test(cv::Mat src, vector<RotRect2D> &caliperRects, RotRect2D &aimRect)
{
	if (!src.data)
	{
		m_errorCode = ErrorCode::NG_Mat_Null;
		return NodeState::Failure;
	}
	m_pointsOK.clear();
	m_pointsNG.clear();
	vector<cv::Point2d> inPoints;
	vector<cv::Point2d> outPoints(3);
	vector<cv::Point2d> fitLinePoints;
	vector<vector<cv::Point2d>>allPoints;
	cv::Vec4f cvRslLine;
	setCoreParams();
	m_caliper->SetImage(src);
	Point2D limitCorner = Point2D(src.cols, src.rows);
	int rtn;
	int realDeleteCount = m_deleteCount;
	allPoints.clear();
	for (int i = 0; i < m_divideCounts; i++)
	{
		if (caliperRects[i].Point0().X() >= 0
			&& caliperRects[i].Point0().X() < limitCorner.X()
			&& caliperRects[i].Point1().X() >= 0
			&& caliperRects[i].Point1().X() < limitCorner.X()
			&& caliperRects[i].Point2().X() >= 0
			&& caliperRects[i].Point2().X() < limitCorner.X()
			&& caliperRects[i].Point3().X() >= 0
			&& caliperRects[i].Point3().X() < limitCorner.X()
			&& caliperRects[i].Point0().Y() >= 0
			&& caliperRects[i].Point0().Y() < limitCorner.Y()
			&& caliperRects[i].Point1().Y() >= 0
			&& caliperRects[i].Point1().Y() < limitCorner.Y()
			&& caliperRects[i].Point2().Y() >= 0
			&& caliperRects[i].Point2().Y() < limitCorner.Y()
			&& caliperRects[i].Point3().Y() >= 0
			&& caliperRects[i].Point3().Y() < limitCorner.Y()
			)
		{
			inPoints.clear();
			inPoints.push_back(cv::Point2d(caliperRects[i].Point0().X(), caliperRects[i].Point0().Y()));
			inPoints.push_back(cv::Point2d(caliperRects[i].Point1().X(), caliperRects[i].Point1().Y()));
			inPoints.push_back(cv::Point2d(caliperRects[i].Point2().X(), caliperRects[i].Point2().Y()));
			inPoints.push_back(cv::Point2d(caliperRects[i].Point3().X(), caliperRects[i].Point3().Y()));
			rtn = m_caliper->Caliper(inPoints, outPoints);
			allPoints.push_back(outPoints);
			if (rtn == 0)
			{
				fitLinePoints.push_back(outPoints[2]);
			}
			else { realDeleteCount--; }
		}
		else { realDeleteCount--; }
	}
	if (fitLinePoints.size() < allPoints.size() / 2)
	{
		m_errorCode = ErrorCode::NG_Caliper_Count;
		return NodeState::Failure;
	}
	//rtn = m_findline->FilterPoints(fitLinePoints, m_pointsOK, m_pointsNG, m_deleteCount);
	if (realDeleteCount < 0) { realDeleteCount = 0; }
	rtn = m_findline->FilterPoints(fitLinePoints, m_pointsOK, m_pointsNG, realDeleteCount);
	if (rtn == 1)
	{
		m_errorCode = ErrorCode::NG_Filter;
		return NodeState::Failure;
	}
	if (m_pointsOK.size() < 2)
	{
		m_errorCode = ErrorCode::NG_Filter_Count;
		return NodeState::Failure;
	}
	m_findline->FitPoints(m_pointsOK, cvRslLine);
	Line2D aline(Point2D(cvRslLine[2], cvRslLine[3]),
		Point2D(cvRslLine[2] + cvRslLine[0], cvRslLine[3] + cvRslLine[1])
		);
	Line2D lineA(aimRect.Point0(), aimRect.Point1());
	Line2D lineB(aimRect.Point1(), aimRect.Point2());
	Line2D lineC(aimRect.Point2(), aimRect.Point3());
	Line2D lineD(aimRect.Point3(), aimRect.Point0());
	Point2D aPoint(lineA.IntersectWith(aline));//????
	Point2D bPoint(lineB.IntersectWith(aline));
	Point2D cPoint(lineC.IntersectWith(aline));
	Point2D dPoint(lineD.IntersectWith(aline));
	Line2D lineH(aPoint, cPoint);
	Line2D lineV(bPoint, dPoint);
	if (lineH.Length() <= lineV.Length())
	{
		output_line->SetValue(lineH);
		output_length->SetValue(lineH.Length());
		output_startPoint->SetValue(aPoint);
		output_endPoint->SetValue(cPoint);
		output_midPoint->SetValue((aPoint + cPoint)*0.5f);
	}
	else
	{
		output_line->SetValue(lineV);
		output_length->SetValue(lineV.Length());
		output_startPoint->SetValue(bPoint);
		output_endPoint->SetValue(dPoint);
		output_midPoint->SetValue((bPoint + dPoint)*0.5f);
	}
	m_errorCode = ErrorCode::Null;
	return NodeState::Success;
}