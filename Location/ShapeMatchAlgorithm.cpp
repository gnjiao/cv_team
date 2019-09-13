#include "ShapeMatchAlgorithm.h"
#include <Windows.h>

CShapeMatchAlgorithm::CShapeMatchAlgorithm()
{
	m_matchModel = NULL;
	m_bCreateModel = false;
	m_bSetPara = false;
	m_featurePointsMaxLen = 1;
}
CShapeMatchAlgorithm::~CShapeMatchAlgorithm()
{
	DeleteModel();
}
/************************************************************************
** Function: DeleteModel()
** Description:
** InPut:
** OutPut:
** Return: void
** Modify:
************************************************************************/
void CShapeMatchAlgorithm::DeleteModel()
{
	if (m_matchModel != NULL)
	{
		for (int i = 0; i < m_pyramidLevels; i++)
		{
			for (int j = 0; j < m_matchModel[i].angleSize; j++)
			{
				if (m_matchModel[i].cordinates[j] != NULL)
				{
					delete[] m_matchModel[i].cordinates[j];
					m_matchModel[i].cordinates[j] = NULL;
				}
				if (m_matchModel[i].edgeDerivativeX[j] != NULL)
				{
					delete[] m_matchModel[i].edgeDerivativeX[j];
					m_matchModel[i].edgeDerivativeX[j] = NULL;
				}
				if (m_matchModel[i].edgeDerivativeY[j] != NULL)
				{
					delete[] m_matchModel[i].edgeDerivativeY[j];
					m_matchModel[i].edgeDerivativeY[j] = NULL;
				}
				if (m_matchModel[i].edgeMagnitude[j] != NULL)
				{
					delete[] m_matchModel[i].edgeMagnitude[j];
					m_matchModel[i].edgeMagnitude[j] = NULL;
				}
			}
			if (m_matchModel[i].cordinates != NULL)
			{
				delete[] m_matchModel[i].cordinates;
				m_matchModel[i].cordinates = NULL;
			}
			if (m_matchModel[i].edgeDerivativeX != NULL)
			{
				delete[] m_matchModel[i].edgeDerivativeX;
				m_matchModel[i].edgeDerivativeX = NULL;
			}
			if (m_matchModel[i].edgeDerivativeY != NULL)
			{
				delete[] m_matchModel[i].edgeDerivativeY;
				m_matchModel[i].edgeDerivativeY = NULL;
			}
			if (m_matchModel[i].edgeMagnitude != NULL)
			{
				delete[] m_matchModel[i].edgeMagnitude;
				m_matchModel[i].edgeMagnitude = NULL;
			}
			if (m_matchModel[i].angles != NULL)
			{
				delete[] m_matchModel[i].angles;
				m_matchModel[i].angles = NULL;
			}
			if (m_matchModel[i].scales != NULL)
			{
				delete[] m_matchModel[i].scales;
				m_matchModel[i].scales = NULL;
			}
		}
		delete[] m_matchModel;
		m_matchModel = NULL;
	}
	return;
}
/************************************************************************
** Function: GetFeaturePoints()
** Description:
** InPut:
src:
roi:
minThresh:
maxThresh:
** OutPut:
featurePoints:
** Return:
0: ?
1:
2:
3:
4:
** Modify:
************************************************************************/
int CShapeMatchAlgorithm::GetFeaturePoints(const cv::Mat src, const cv::Rect roi, const double minThresh, const double maxThresh,
	vector<cv::Point>& featurePoints)
{
	if (src.empty())
	{
		return 1;
	}
	if (roi.width < 1 || roi.height < 1 || roi.x < 0 || roi.y < 0 || roi.x + roi.width > src.cols ||
		roi.y + roi.height > src.rows)
	{
		return 2;
	}
	if (minThresh >= maxThresh)
	{
		return 3;
	}
	featurePoints.clear();
	cv::Mat srcImage;
	if (src.channels() > 1)
	{
		cv::cvtColor(src, srcImage, cv::COLOR_RGB2GRAY);
	}
	else
	{
		src.copyTo(srcImage);
	}
	cv::Mat modelImage;
	srcImage(roi).copyTo(modelImage);
	cv::Mat edges;
	cv::GaussianBlur(modelImage, modelImage, cv::Size(5, 5), 1.5);
	cv::Canny(modelImage, edges, minThresh * 3.6, maxThresh * 3.6);
	for (int i = 0; i < edges.rows; i++)
	{
		uchar* edgesPtr = edges.ptr<uchar>(i);
		for (int j = 0; j < edges.cols; j++)
		{
			if (edgesPtr[j] > 100)
			{
				int fpLen = featurePoints.size();
				bool bAdd = true;
				for (int k = 0; k < fpLen; k++)
				{
					if (abs(featurePoints[k].x - j) < 3 && abs(featurePoints[k].y - i) < 3)
					{
						bAdd = false;
						break;
					}
				}
				if (bAdd)
				{
					featurePoints.push_back(cv::Point(j, i));
				}
			}
		}
	}
	if (featurePoints.empty())
	{
		return 4;
	}
	int fpLen = featurePoints.size();
	for (int i = 0; i < fpLen; i++)
	{
		featurePoints[i].x += roi.x;
		featurePoints[i].y += roi.y;
	}
	m_tempRect = roi;
	return 0;
}
/************************************************************************
** Function: CreateModel()
** Description:
** InPut:
src:
** OutPut:
featurePoints:
** Return:
0:
1:
2:
3:
4:
** Modify:
************************************************************************/
int CShapeMatchAlgorithm::CreateModel(const cv::Mat src, const vector<cv::Point> featurePoints, cv::Point& centerPoint)
{
	m_bCreateModel = false;
	if (src.empty())
	{
		return 1;
	}
	if (featurePoints.empty())
	{
		return 3;
	}
	if (m_matchModel != NULL)
	{
		DeleteModel();
	}
	cv::Mat srcImage;
	if (src.channels() > 1)
	{
		cv::cvtColor(src, srcImage, cv::COLOR_RGB2GRAY);
	}
	else
	{
		src.copyTo(srcImage);
	}
	vector <vector <cv::Point>> cordinates;
	if (GetPyramidLevelsAndCordinates(featurePoints, m_pyramidLevels, cordinates) > 0)
	{
		return 4;
	}
	if (cordinates.size() != m_pyramidLevels || m_pyramidLevels < 1)
	{
		return 4;
	}
	vector<cv::Mat> modelImagePyramid;
	if (GetPyramid(srcImage, m_pyramidLevels, modelImagePyramid) > 0)
	{
		return 5;
	}
	int featurePointsMaxLen = 0;
	if (GetFeaturePointsMaxLen(featurePoints, featurePointsMaxLen) > 0)
	{
		return 6;
	}
	m_featurePointsMaxLen = featurePointsMaxLen;
	vector<vector<short>> deviratesX;
	vector<vector<short>> deviratesY;
	vector<vector<double>> angles;
	vector<vector<double>> scales;
	for (int i = 0; i < m_pyramidLevels; i++)
	{
		vector<short> tmpDeviratesX;
		vector<short> tmpDeviratesY;
		if (GetDerivatives(modelImagePyramid[i], cordinates[i], tmpDeviratesX, tmpDeviratesY) > 0)
		{
			return 7;
		}
		deviratesX.push_back(tmpDeviratesX);
		deviratesY.push_back(tmpDeviratesY);
	}
	if (deviratesX.size() != m_pyramidLevels || deviratesY.size() != m_pyramidLevels)
	{
		return 7;
	}
	if (GetAngles(m_pyramidLevels, featurePointsMaxLen, angles) > 0 || angles.size() != m_pyramidLevels)
	{
		return 8;
	}
	if (GetScales(m_pyramidLevels, featurePointsMaxLen, scales) > 0 || scales.size() != m_pyramidLevels)
	{
		return 9;
	}
	for (int i = 0; i < cordinates.size(); i++)
	{
		int RSum = 0, CSum = 0;
		for (int j = 0; j < cordinates[i].size(); j++)
		{
			CSum += cordinates[i][j].x;
			RSum += cordinates[i][j].y;
		}
		cv::Point centerOfGravity; //Center of gravity of template
		centerOfGravity.x = CSum / cordinates[i].size(); // center of gravity
		centerOfGravity.y = RSum / cordinates[i].size(); // center of gravity
		if (i == 0)
		{
			centerPoint = centerOfGravity;
			m_tempCenter = centerOfGravity;
		}
		for (int j = 0; j < cordinates[i].size(); j++)
		{
			cordinates[i][j].x -= centerOfGravity.x;
			cordinates[i][j].y -= centerOfGravity.y;
		}
	}
	m_matchModel = new MatchModel[m_pyramidLevels];
	for (int i = 0; i < m_pyramidLevels; i++)
	{
		m_matchModel[i].cordinateSize = cordinates[i].size();
		m_matchModel[i].angleSize = angles[i].size();
		m_matchModel[i].scaleSize = scales[i].size();
		m_matchModel[i].cordinates = new cv::Point*[angles[i].size()];
		m_matchModel[i].edgeDerivativeX = new short*[angles[i].size()];
		m_matchModel[i].edgeDerivativeY = new short*[angles[i].size()];
		m_matchModel[i].edgeMagnitude = new double*[angles[i].size()];
		for (int j = 0; j < angles[i].size(); j++)
		{
			m_matchModel[i].cordinates[j] = new cv::Point[m_matchModel[i].cordinateSize];
			m_matchModel[i].edgeDerivativeX[j] = new short[m_matchModel[i].cordinateSize];
			m_matchModel[i].edgeDerivativeY[j] = new short[m_matchModel[i].cordinateSize];
			m_matchModel[i].edgeMagnitude[j] = new double[m_matchModel[i].cordinateSize];
			double sinVal = sin(angles[i][j] * CV_PI / 180);
			double cosVal = cos(angles[i][j] * CV_PI / 180);
			for (int k = 0; k < m_matchModel[i].cordinateSize; k++)
			{
				double x, y;
				x = cordinates[i][k].x * cosVal - cordinates[i][k].y * sinVal;
				y = cordinates[i][k].x * sinVal + cordinates[i][k].y * cosVal;
				if (x > 0)
				{
					m_matchModel[i].cordinates[j][k].x = x + 0.5;
				}
				else
				{
					m_matchModel[i].cordinates[j][k].x = x - 0.5;
				}
				if (y > 0)
				{
					m_matchModel[i].cordinates[j][k].y = y + 0.5;
				}
				else
				{
					m_matchModel[i].cordinates[j][k].y = y - 0.5;
				}
				m_matchModel[i].edgeDerivativeX[j][k] = deviratesX[i][k] * cosVal - deviratesY[i][k] * sinVal;
				m_matchModel[i].edgeDerivativeY[j][k] = deviratesY[i][k] * cosVal + deviratesX[i][k] * sinVal;
				double MagG = sqrt(m_matchModel[i].edgeDerivativeX[j][k] * m_matchModel[i].edgeDerivativeX[j][k] +
					m_matchModel[i].edgeDerivativeY[j][k] * m_matchModel[i].edgeDerivativeY[j][k]);
				if (MagG != 0)
				{
					m_matchModel[i].edgeMagnitude[j][k] = 1 / MagG;
				}
				else
				{
					m_matchModel[i].edgeMagnitude[j][k] = 0;
				}
			}
		}
		m_matchModel[i].angles = new double[angles[i].size()];
		m_matchModel[i].scales = new double[scales[i].size()];
		for (int j = 0; j < angles[i].size(); j++)
		{
			m_matchModel[i].angles[j] = angles[i][j];
		}
		for (int j = 0; j < scales[i].size(); j++)
		{
			m_matchModel[i].scales[j] = scales[i][j];
		}
	}
	m_bCreateModel = true;
	return 0;
}
int CShapeMatchAlgorithm::GetPyramidLevelsAndCordinates(const vector<cv::Point> featurePoints, int& pyramidLevels,
	vector<vector<cv::Point>>& cordinates)
{
	if (featurePoints.empty())
	{
		return 1;
	}
	cordinates.clear();
	pyramidLevels = 0;
	vector<cv::Point> tmpFeaturePoints;
	while (true)
	{
		tmpFeaturePoints.clear();
		int fpLen = featurePoints.size();
		for (int i = 0; i < fpLen; i++)
		{
			cv::Point tmpPoint;
			tmpPoint.x = featurePoints[i].x / pow(2.0, pyramidLevels) + 0.5;
			tmpPoint.y = featurePoints[i].y / pow(2.0, pyramidLevels) + 0.5;
			if (tmpFeaturePoints.empty())
			{
				tmpFeaturePoints.push_back(tmpPoint);
			}
			else
			{
				bool bAdd = true;
				int tfpLen = tmpFeaturePoints.size();
				for (int j = 0; j < tfpLen; j++)
				{
					if (abs(tmpPoint.x - tmpFeaturePoints[j].x) < 1 && abs(tmpPoint.y - tmpFeaturePoints[j].y) < 1)
					{
						bAdd = false;
						break;
					}
				}
				if (bAdd)
				{
					tmpFeaturePoints.push_back(tmpPoint);
				}
			}
		}
		if (tmpFeaturePoints.size() < 30)
		{
			break;
		}
		cordinates.push_back(tmpFeaturePoints);
		pyramidLevels++;
	}
	return 0;
}
int CShapeMatchAlgorithm::GetPyramid(const cv::Mat src, const int pyramidLevels, vector<cv::Mat>& pyramid)
{
	if (src.empty())
	{
		return 1;
	}
	if (pyramidLevels < 1)
	{
		return 2;
	}
	cv::Mat pyMat;
	for (int pyCount = 0; pyCount < pyramidLevels; pyCount++)
	{
		cv::Mat tmpMat;
		if (pyCount == 0)
		{
			src.copyTo(tmpMat);
		}
		else
		{
			cv::pyrDown(pyMat, tmpMat, cv::Size(pyMat.cols / 2, pyMat.rows / 2));
		}
		pyramid.push_back(tmpMat);
		if (!pyMat.empty())
		{
			pyMat.release();
		}
		//cv::imwrite("D:\\1.jpg", tmpMat);
		tmpMat.copyTo(pyMat);
	}
	return 0;
}
int CShapeMatchAlgorithm::GetDerivatives(const cv::Mat src, const vector<cv::Point> featurePoints, vector<short>& derivativesX,
	vector<short>& derivativesY)
{
	if (src.empty())
	{
		return 1;
	}
	if (featurePoints.empty())
	{
		return 2;
	}
	derivativesX.clear();
	derivativesY.clear();
	cv::Mat gx = cv::Mat(src.rows, src.cols, CV_16SC1); //create Matrix to store X derivative
	cv::Mat gy = cv::Mat(src.rows, src.cols, CV_16SC1); //create Matrix to store Y derivative
	cv::Sobel(src, gx, gx.depth(), 1, 0); //gradient in X direction
	cv::Sobel(src, gy, gy.depth(), 0, 1); //gradient in Y direction
	int fpLen = featurePoints.size();
	for (int i = 0; i < fpLen; i++)
	{
		if (featurePoints[i].y > 0 && featurePoints[i].y < src.rows && featurePoints[i].x > 0 && featurePoints[i].x < src.cols)
		{
			derivativesX.push_back(gx.ptr<short>(featurePoints[i].y)[featurePoints[i].x]);
			derivativesY.push_back(gy.ptr<short>(featurePoints[i].y)[featurePoints[i].x]);
		}
		else
		{
			return 3;
		}
	}
	return 0;
}
int CShapeMatchAlgorithm::GetAngles(const int pyramidLevels, const int featurePointsMaxLen, vector<vector<double>>& angles)
{
	if (pyramidLevels < 1)
	{
		return 1;
	}
	if (featurePointsMaxLen < 1)
	{
		return 2;
	}
	angles.clear();
	double angleStep = 200.0 / featurePointsMaxLen;
	for (int i = 0; i < m_pyramidLevels; i++)
	{
		if (i > 0)
			angleStep *= 2;
		vector<double> tmpAngles;
		for (double angle = 0; angle < 180; angle += angleStep)
		{
			tmpAngles.push_back(angle);
			if (angle > 0) tmpAngles.push_back(-angle);
		}
		sort(tmpAngles.begin(), tmpAngles.end());
		angles.push_back(tmpAngles);
	}
	return 0;
}
int CShapeMatchAlgorithm::GetScales(const int pyramidLevels, const int featurePointsMaxLen, vector<vector<double>>& scales)
{
	if (pyramidLevels < 1)
	{
		return 1;
	}
	if (featurePointsMaxLen < 1)
	{
		return 2;
	}
	scales.clear();
	double scaleStep = 1.0 / featurePointsMaxLen;
	for (int i = 0; i < m_pyramidLevels; i++)
	{
		if (i > 0)
			scaleStep *= 2;
		vector<double> tmpScales;
		for (double scale = 0; scale < 0.2; scale += scaleStep)
		{
			tmpScales.push_back(scale + 1);
			if (scale > 0) tmpScales.push_back(1 - scale);
		}
		sort(tmpScales.begin(), tmpScales.end());
		scales.push_back(tmpScales);
	}
	return 0;
}
int CShapeMatchAlgorithm::GetFeaturePointsMaxLen(const vector <cv::Point> featurePoints, int& featurePointsMaxLen)
{
	if (featurePoints.empty())
	{
		return 1;
	}
	featurePointsMaxLen = 0;
	int xMin = 1000000;
	int yMin = 1000000;
	int xMax = 0;
	int yMax = 0;
	int fpLen = featurePoints.size();
	for (int i = 0; i < fpLen; i++)
	{
		if (featurePoints[i].x > xMax)
		{
			xMax = featurePoints[i].x;
		}
		if (featurePoints[i].y > yMax)
		{
			yMax = featurePoints[i].y;
		}
		if (featurePoints[i].x < xMin)
		{
			xMin = featurePoints[i].x;
		}
		if (featurePoints[i].y < yMin)
		{
			yMin = featurePoints[i].y;
		}
	}
	featurePointsMaxLen = max(xMax - xMin, yMax - yMin);
	return 0;
}
/************************************************************************
** Function: ShapeMatch()
** Description:
** InPut:
minMatchScore:
greediness:
startAngle:
endAngle:
minScale:
maxScale:
maxMatchNum:
** OutPut:
** Return:
0:
1:
2:
3:
** Modify:
************************************************************************/
int CShapeMatchAlgorithm::SetMatchPara(const double minMatchScore, const double greediness, const double startAngle,
	const double endAngle, const double minScale, const double maxScale, const int maxMatchNum, const cv::Rect srcROI)
{
	m_bSetPara = false;
	if (minMatchScore < 0.1)
	{
		m_minMatchScore = 0.1;
	}
	else if (minMatchScore > 0.99)
	{
		m_minMatchScore = 0.99;
	}
	else
	{
		m_minMatchScore = minMatchScore;
	}
	if (greediness < 0.1)
	{
		m_greediness = 0.1;
	}
	else if (greediness > 0.99)
	{
		m_greediness = 0.99;
	}
	else
	{
		m_greediness = greediness;
	}
	if (startAngle > endAngle)
	{
		return 3;
	}
	else
	{
		m_startAngle = startAngle - 0.1;
		m_endAngle = endAngle + 0.1;
	}
	if (minScale > maxScale)
	{
		return 4;
	}
	else
	{
		m_minScale = minScale - 0.1;
		m_maxScale = maxScale + 0.1;
	}
	if (maxMatchNum < 1)
	{
		return 5;
	}
	else
	{
		m_maxMatchNum = maxMatchNum;
	}
	if (srcROI.x < 0 || srcROI.y < 0 || srcROI.width < 1 || srcROI.height < 1)
	{
		return 6;
	}
	else
	{
		m_srcROI = srcROI;
	}
	m_bSetPara = true;
	return 0;
}
/************************************************************************
** Function: ShapeMatch()
** Description: ?
** InPut:
src:
** OutPut:
matchScore:
matchAngle:
matchScale:
matchPoint:
timeSpan:
resultFeaturePoints
** Return:
0:
1:
2:
3:
4:
** Modify:
************************************************************************/
int CShapeMatchAlgorithm::ShapeMatch(const cv::Mat src, vector<double>& matchScore, vector<double>& matchAngle,
	vector<double>& matchScale, vector<cv::Point>& matchPoint, vector <vector<cv::Point>>& resultFeaturePoints, double& timeSpan)
{
	if (src.empty())
	{
		return 1;
	}
	if (!m_bCreateModel || !m_bSetPara)
	{
		return 2;
	}
	if (m_srcROI.x < 0 || m_srcROI.y < 0 || m_srcROI.width < 1 || m_srcROI.height < 1 || m_srcROI.x + m_srcROI.width > src.cols ||
		m_srcROI.y + m_srcROI.height > src.rows)
	{
		return 3;
	}
	cv::Mat srcImage;
	if (src.channels() > 1)
	{
		cv::cvtColor(src, srcImage, cv::COLOR_RGB2GRAY);
	}
	else
	{
		src.copyTo(srcImage);
	}
	cv::Mat roiOfsrc;
	srcImage(m_srcROI).copyTo(roiOfsrc);
	vector<cv::Mat> SearchPyramidal;
	if (GetPyramid(roiOfsrc, m_pyramidLevels, SearchPyramidal) > 0)
	{
		return 4;
	}
	matchPoint.clear();
	matchAngle.clear();
	matchScale.clear();
	matchScore.clear();
	LARGE_INTEGER lFrequency, lBeginCount;
	QueryPerformanceFrequency(&lFrequency);
	QueryPerformanceCounter(&lBeginCount);
	cv::Mat* pySdx = new cv::Mat[m_pyramidLevels];
	cv::Mat* pySdy = new cv::Mat[m_pyramidLevels];
	double** matGradMag;
	matGradMag = new double*[m_pyramidLevels];
	for (int i = 0; i < m_pyramidLevels; i++)
	{
		matGradMag[i] = new double[SearchPyramidal[i].rows * SearchPyramidal[i].cols];
		if (GetSearchImageInfo(SearchPyramidal[i], pySdx[i], pySdy[i], matGradMag[i]) != 0)
		{
			return 5;
		}
	}
	// ofstream fout;
	// fout.open("D:\\test.txt");
	vector<MatchPara> matchPara;
	for (int pyCount = m_pyramidLevels - 1; pyCount > -1; pyCount--)
	{
		if (pyCount == m_pyramidLevels - 1)
		{
			int startAngleIndex = 0;
			int endAngleIndex = m_matchModel[pyCount].angleSize;
			if (GetSearchAngleRange(pyCount, startAngleIndex, endAngleIndex) != 0)
			{
				return 6;
			}
			int startScaleIndex = 0;
			int endScaleIndex = m_matchModel[pyCount].scaleSize;
			if (GetSearchScaleRange(pyCount, startScaleIndex, endScaleIndex) != 0)
			{
				return 7;
			}
			SearchHighestPyramidLevel(SearchPyramidal[pyCount], pySdx[pyCount], pySdy[pyCount], startAngleIndex, endAngleIndex,
				startScaleIndex, endScaleIndex, matGradMag[pyCount], matchPara);
		}
		else
		{
			if (!matchPara.empty())
			{
				SearchLowPyramidLevel(pySdx[pyCount], pySdy[pyCount], pyCount, matGradMag[pyCount], matchPara);
			}
		}
	}
	SelectResult(matchScore, matchAngle, matchScale, matchPoint, resultFeaturePoints, matchPara);
	for (int i = 0; i < m_pyramidLevels; i++)
	{
		if (matGradMag[i] != NULL)
			delete[] matGradMag[i];
	}
	delete[] matGradMag;
	delete[] pySdx;
	delete[] pySdy;
	LARGE_INTEGER lEndCount;
	QueryPerformanceCounter(&lEndCount);
	timeSpan = (double)(lEndCount.QuadPart - lBeginCount.QuadPart) / (double)lFrequency.QuadPart * 1000;
	// for (int i = 0; i < matchPoint.size(); i++)
	// {
	// cout << matchScore[i] << " " << matchPoint[i] << " " << matchAngle[i] << " " << matchScale[i] << " " << timeSpan << endl;
	// }
	// fout.close();
	return 0;
}
int CShapeMatchAlgorithm::SelectResult(vector<double>& matchScore, vector<double>& matchAngle, vector<double>& matchScale,
	vector<cv::Point>& matchPoint, vector <vector<cv::Point>>& resultFeaturePoints, vector<MatchPara>& matchPara)
{
	int mpLen = matchPara.size();
	vector<int> matchAngleIndex;
	for (int j = 0; j < mpLen; j++)
	{
		if (matchPoint.empty() && matchPara[j].matchScore > m_minMatchScore&& m_matchModel[0].angles[matchPara[j].matchAngleIndex] >= m_startAngle &&
			m_matchModel[0].angles[matchPara[j].matchAngleIndex] <= m_endAngle && m_matchModel[0].scales[matchPara[j].matchScaleIndex] >= m_minScale &&
			m_matchModel[0].scales[matchPara[j].matchScaleIndex] <= m_maxScale)
		{
			matchScore.push_back(matchPara[j].matchScore);
			matchAngle.push_back(m_matchModel[0].angles[matchPara[j].matchAngleIndex]);
			matchScale.push_back(m_matchModel[0].scales[matchPara[j].matchScaleIndex]);
			matchPoint.push_back(matchPara[j].matchPoint);
			matchAngleIndex.push_back(matchPara[j].matchAngleIndex);
		}
		else
		{
			bool bAdd = true;
			for (int i = 0; i < matchPoint.size(); i++)
			{
				double dist = sqrt(pow(matchPara[j].matchPoint.x - matchPoint[i].x, 2) + pow(matchPara[j].matchPoint.y - matchPoint[i].y, 2));
				if (dist < m_featurePointsMaxLen / 2)
				{
					if (matchPara[j].matchScore > matchScore[i])
					{
						matchScore[i] = matchPara[j].matchScore;
						matchPoint[i] = matchPara[j].matchPoint;
						matchAngle[i] = m_matchModel[0].angles[matchPara[j].matchAngleIndex];
						matchScale[i] = m_matchModel[0].scales[matchPara[j].matchScaleIndex];
						matchAngleIndex[i] = matchPara[j].matchAngleIndex;
					}
					bAdd = false;
					break;
				}
			}
			if (bAdd && matchPara[j].matchScore > m_minMatchScore && m_matchModel[0].angles[matchPara[j].matchAngleIndex] >= m_startAngle &&
				m_matchModel[0].angles[matchPara[j].matchAngleIndex] <= m_minScale && m_matchModel[0].scales[matchPara[j].matchScaleIndex] >= m_minScale &&
				m_matchModel[0].scales[matchPara[j].matchScaleIndex] <= m_maxScale)
			{
				matchScore.push_back(matchPara[j].matchScore);
				matchAngle.push_back(m_matchModel[0].angles[matchPara[j].matchAngleIndex]);
				matchScale.push_back(m_matchModel[0].scales[matchPara[j].matchScaleIndex]);
				matchPoint.push_back(matchPara[j].matchPoint);
				matchAngleIndex.push_back(matchPara[j].matchAngleIndex);
			}
		}
		if (matchPoint.size() == m_maxMatchNum)
		{
			break;
		}
	}
	resultFeaturePoints.clear();
	for (int i = 0; i < matchPoint.size(); i++)
	{
		vector<cv::Point> resultPoint;
		cv::Point tmpPoint;
		for (int j = 0; j < m_matchModel[0].cordinateSize; j++)
		{
			tmpPoint.x = m_matchModel[0].cordinates[matchAngleIndex[i]][j].x * matchScale[i] + matchPoint[i].y + m_srcROI.x + 0.5;
			tmpPoint.y = m_matchModel[0].cordinates[matchAngleIndex[i]][j].y * matchScale[i] + matchPoint[i].x + m_srcROI.y + 0.5;
			resultPoint.push_back(tmpPoint);
		}
		resultFeaturePoints.push_back(resultPoint);
	}
	return 0;
}
int CShapeMatchAlgorithm::SearchHighestPyramidLevel(const cv::Mat src, const cv::Mat sdx, const cv::Mat sdy,
	const int startAngleIndex, const int endAngleIndex, const int startScaleIndex, const int endScaleIndex,
	const double* const mag, vector<MatchPara>& matchPara)
{
	int pyCount = m_pyramidLevels - 1;
	// stoping criterias to search for model
	double normMinScore = (m_minMatchScore - 0.1 * m_pyramidLevels) / m_matchModel[pyCount].cordinateSize; // precompute minumum score
	double normGreediness = ((1 - m_greediness * (m_minMatchScore - 0.1 * m_pyramidLevels)) / (1 - m_greediness)) /
		m_matchModel[pyCount].cordinateSize; // precompute greedniness
	MatchPara fmp, smp, tmp;
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			double fir = 0;
			double sec = 0;
			double thir = 0;
			for (int m = startAngleIndex; m < endAngleIndex; m++)
			{
				for (int n = startScaleIndex; n < endScaleIndex; n++)
				{
					double tmpScore = 0.0;
					cv::Point tmpPoint = { 0, 0 };
					Match(sdx, sdy, cv::Point(i, j), m, n, pyCount, normMinScore, normGreediness, mag, tmpScore, tmpPoint);
					if (tmpScore > fir)
					{
						fmp.matchPoint = tmpPoint;
						fmp.matchScore = tmpScore;
						fmp.matchAngleIndex = m;
						fmp.matchScaleIndex = n;
						fir = tmpScore;
					}
					else if (tmpScore > sec)
					{
						smp.matchPoint = tmpPoint;
						smp.matchScore = tmpScore;
						smp.matchAngleIndex = m;
						smp.matchScaleIndex = n;
						sec = tmpScore;
					}
					else if (tmpScore > thir)
					{
						tmp.matchPoint = tmpPoint;
						tmp.matchScore = tmpScore;
						tmp.matchAngleIndex = m;
						tmp.matchScaleIndex = n;
						thir = tmpScore;
					}
				}
			}
			if (fir > 0.3)
			{
				matchPara.push_back(fmp);
			}
			if (sec > 0.3 && fir - sec < 0.05)
			{
				matchPara.push_back(smp);
			}
			if (thir >0.3 && sec - thir < 0.05)
			{
				matchPara.push_back(tmp);
			}
		}
	}
	if (SelectMatchPoints(pyCount, matchPara) != 0)
	{
		return 8;
	}
	return 0;
}
int CShapeMatchAlgorithm::SearchLowPyramidLevel(const cv::Mat sdx, const cv::Mat sdy, const int pyCount,
	const double* const mag, vector<MatchPara>& matchPara)
{
	double normMinScore = (m_minMatchScore - 0.1 * pyCount) / m_matchModel[pyCount].cordinateSize; // precompute minumum score
	double normGreediness = ((1 - m_greediness * (m_minMatchScore - 0.1 * pyCount)) / (1 - m_greediness)) / m_matchModel[pyCount].cordinateSize; // precompute greedniness
	vector<MatchPara> tmpMatchPara;
	double avrScore = 0.0;
	int mpLen = matchPara.size();
	for (int m = 0; m < mpLen; m++)
	{
		MatchPara mp;
		double tmpScore = 0.0;
		double tmpMaxScore = 0.0;
		for (int p = -1; p < 2; p++)
		{
			int tmpScaleIndex = 2 * matchPara[m].matchScaleIndex + p;
			for (int q = -1; q < 2; q++)
			{
				int tmpAngleIndex = 2 * matchPara[m].matchAngleIndex + q;
				if (tmpScaleIndex > -1 && tmpScaleIndex < m_matchModel[pyCount].scaleSize && tmpAngleIndex > -1 &&
					tmpAngleIndex < m_matchModel[pyCount].angleSize)
				{
					cv::Point searchPoint = { matchPara[m].matchPoint.x * 2, matchPara[m].matchPoint.y * 2 };
					for (int i = searchPoint.x - 3; i < searchPoint.x + 4; i++)
					{
						for (int j = searchPoint.y - 3; j < searchPoint.y + 4; j++)
						{
							cv::Point tmpMatchPoint;
							tmpScore = 0.0;
							Match(sdx, sdy, cv::Point(i, j), tmpAngleIndex, tmpScaleIndex, pyCount, normMinScore,
								normGreediness, mag, tmpScore, tmpMatchPoint);
							if (tmpScore > tmpMaxScore)
							{
								mp.matchPoint = tmpMatchPoint;
								mp.matchScore = tmpScore;
								mp.matchAngleIndex = tmpAngleIndex;
								mp.matchScaleIndex = tmpScaleIndex;
								tmpMaxScore = tmpScore;
							}
						}
					}
				}
			}
		}
		if (mp.matchPoint.x != 0 && mp.matchPoint.y != 0)
		{
			if (mp.matchScore > 0.4)
			{
				bool bAdd = true;
				for (int i = 0; i < tmpMatchPara.size(); i++)
				{
					if (mp.matchPoint == tmpMatchPara[i].matchPoint && mp.matchScore == tmpMatchPara[i].matchScore &&
						mp.matchAngleIndex == tmpMatchPara[i].matchAngleIndex && mp.matchScaleIndex == tmpMatchPara[i].matchScaleIndex)
					{
						bAdd = false;
					}
				}
				if (bAdd)
				{
					tmpMatchPara.push_back(mp);
				}
			}
		}
	}
	SelectMatchPoints(pyCount, tmpMatchPara);
	matchPara.clear();
	matchPara.assign(tmpMatchPara.begin(), tmpMatchPara.end());
	return 0;
}
int CShapeMatchAlgorithm::GetSearchImageInfo(const cv::Mat src, cv::Mat& sdx, cv::Mat& sdy, double* mag)
{
	if (src.empty())
	{
		return 1;
	}
	if (mag == NULL)
	{
		return 2;
	}
	sdx = cv::Mat(src.rows, src.cols, CV_16SC1); // X derivatives
	sdy = cv::Mat(src.rows, src.cols, CV_16SC1); // y derivatives
	cv::Sobel(src, sdx, sdx.depth(), 1, 0); // find X derivatives
	cv::Sobel(src, sdy, sdy.depth(), 0, 1); // find Y derivatives
	for (int j = 0; j < src.rows; j++)
	{
		short* gxPtr = sdx.ptr<short>(j);
		short* gyPtr = sdy.ptr<short>(j);
		for (int k = 0; k < src.cols; k++)
		{
			double MagG = sqrt((double)gxPtr[k] * gxPtr[k] + (double)gyPtr[k] * gyPtr[k]); // 1/Sqrt(dx^2 +dy^2)
			if (MagG != 0) // handle divide by zero
				mag[j * src.cols + k] = 1 / MagG;
			else
				mag[j * src.cols + k] = 0;
		}
	}
	return 0;
}
int CShapeMatchAlgorithm::GetSearchAngleRange(const int pyramidLevel, int& startAngleIndex, int& endAngleIndex)
{
	if (pyramidLevel < 0 || pyramidLevel >= m_pyramidLevels)
	{
		return 1;
	}
	startAngleIndex = 0;
	endAngleIndex = m_matchModel[pyramidLevel].angleSize;
	for (int i = 0; i < m_matchModel[pyramidLevel].angleSize; i++)
	{
		if (m_matchModel[pyramidLevel].angles[i] < m_startAngle)
		{
			startAngleIndex = i;
		}
		if (m_matchModel[pyramidLevel].angles[i] > m_endAngle)
		{
			endAngleIndex = i;
			break;
		}
	}
	return 0;
}
int CShapeMatchAlgorithm::GetSearchScaleRange(const int pyramidLevel, int& startScaleIndex, int& endScaleIndex)
{
	if (pyramidLevel < 0 || pyramidLevel >= m_pyramidLevels)
	{
		return 1;
	}
	startScaleIndex = 0;
	endScaleIndex = m_matchModel[pyramidLevel].scaleSize;
	for (int i = 0; i < m_matchModel[pyramidLevel].scaleSize; i++)
	{
		if (m_matchModel[pyramidLevel].scales[i] < m_minScale)
		{
			startScaleIndex = i;
		}
		if (m_matchModel[pyramidLevel].scales[i] > m_maxScale)
		{
			endScaleIndex = i;
			break;
		}
	}
	return 0;
}
int CShapeMatchAlgorithm::SelectMatchPoints(const int pyramidLevel, vector<MatchPara>& matchPara)
{
	if (matchPara.empty())
	{
		return 1;
	}
	sort(matchPara.begin(), matchPara.end(), CompareByMatchScore());
	int selectLen = MIN((m_maxMatchNum * (pyramidLevel + 1) * 5), matchPara.size());
	matchPara.erase(matchPara.begin() + selectLen, matchPara.end());
	return 0;
}
int CShapeMatchAlgorithm::Match(const cv::Mat Sdx, const cv::Mat Sdy, const int angleStep, const int scaleStep, const int pyramidStep,
	const double normMinScore, const double normGreediness, const double* const matGradMag, double& matchScore, cv::Point& matchPoint)
{
	if (Sdx.empty() || Sdy.empty())
	{
		return 1;
	}
	if (Sdx.size() != Sdy.size())
	{
		return 2;
	}
	if (matGradMag == NULL)
	{
		return 3;
	}
	cv::Rect searchArea;
	if (matchPoint.x == 0 && matchPoint.y == 0)
	{
		//searchArea = { 0, 0, Sdx.cols , Sdx.rows};
		searchArea.x = 0;
		searchArea.y = 0;
		searchArea.width = Sdx.cols;
		searchArea.height = Sdx.rows;
	}
	else
	{
		searchArea.x = max(0, matchPoint.y * 2 - 2);
		searchArea.y = max(0, matchPoint.x * 2 - 2);
		searchArea.width = min(Sdx.cols - searchArea.x, 4);
		searchArea.height = min(Sdx.rows - searchArea.y, 4);
	}
	matchScore = 0;
	double partialSum = 0;
	double sumOfCoords = 0;
	double partialScore = 0;
	for (int i = searchArea.y; i < searchArea.y + searchArea.height; i++)
	{
		for (int j = searchArea.x; j < searchArea.x + searchArea.width; j++)
		{
			partialSum = 0; // initilize partialSum measure
			bool bfinish = false;
			for (int m = 0; m < m_matchModel[pyramidStep].cordinateSize; m++)
			{
				double x, y;
				x = m_matchModel[pyramidStep].cordinates[angleStep][m].x * m_matchModel[pyramidStep].scales[scaleStep];
				y = m_matchModel[pyramidStep].cordinates[angleStep][m].y * m_matchModel[pyramidStep].scales[scaleStep];
				int curX = i + y + 0.5; // template X coordinate
				int curY = j + x + 0.5; // template Y coordinate
				if (curX < 0 || curY < 0 || curX > Sdx.rows - 1 || curY > Sdx.cols - 1)
					continue;
				double iSx = Sdx.ptr<short>(curX)[curY];// get curresponding X derivative from source image
				double iSy = Sdy.ptr<short>(curX)[curY];// get curresponding Y derivative from source image
				double iTx = m_matchModel[pyramidStep].edgeDerivativeX[angleStep][m];// template X derivative
				double iTy = m_matchModel[pyramidStep].edgeDerivativeY[angleStep][m];// template Y derivative
				if ((iSx != 0 || iSy != 0) && (iTx != 0 || iTy != 0))
				{
					//partial Sum = Sum of(((Source X derivative* Template X drivative) + Source Y derivative * Template Y derivative)) / Edge magnitude of(Template)* edge magnitude of(Source))
					partialSum += ((iSx*iTx) + (iSy*iTy))*(m_matchModel[pyramidStep].edgeMagnitude[angleStep][m] *
						matGradMag[curX * Sdx.cols + curY]);
				}
				sumOfCoords = m + 1;
				partialScore = partialSum / sumOfCoords;
				// check termination criteria
				// if partial score score is less than the score than needed to make the required score at that position
				// break serching at that coordinate.
				if (partialScore < (MIN(((m_minMatchScore - 0.1) - 1) + normGreediness * sumOfCoords, normMinScore * sumOfCoords)))
					break;
				bfinish = true;
			}
			if (bfinish && partialScore > matchScore && sumOfCoords > m_matchModel[pyramidStep].cordinateSize * m_greediness)
			{
				matchScore = partialScore; // Match score
				matchPoint = cv::Point(i, j);
			}
		}
	}
	return 0;
}
int CShapeMatchAlgorithm::Match(const cv::Mat Sdx, const cv::Mat Sdy, const cv::Point searchPoint, const int angleStep,
	const int scaleStep, const int pyramidStep, const double normMinScore, const double normGreediness,
	const double* const matGradMag, double& matchScore, cv::Point& matchPoint)
{
	if (Sdx.empty() || Sdy.empty())
	{
		return 1;
	}
	if (Sdx.size() != Sdy.size())
	{
		return 2;
	}
	if (matGradMag == NULL)
	{
		return 3;
	}
	matchScore = 0;
	double partialSum = 0;
	double sumOfCoords = 0;
	double partialScore = 0;
	bool bfinish = false;
	for (int m = 0; m < m_matchModel[pyramidStep].cordinateSize; m++)
	{
		double x, y;
		x = m_matchModel[pyramidStep].cordinates[angleStep][m].x * m_matchModel[pyramidStep].scales[scaleStep];
		y = m_matchModel[pyramidStep].cordinates[angleStep][m].y * m_matchModel[pyramidStep].scales[scaleStep];
		int curX = searchPoint.x + y + 0.5; // template X coordinate
		int curY = searchPoint.y + x + 0.5; // template Y coordinate
		if (curX < 0 || curY < 0 || curX > Sdx.rows - 1 || curY > Sdx.cols - 1)
			continue;
		double iSx = Sdx.ptr<short>(curX)[curY];// get curresponding X derivative from source image
		double iSy = Sdy.ptr<short>(curX)[curY];// get curresponding Y derivative from source image
		double iTx = m_matchModel[pyramidStep].edgeDerivativeX[angleStep][m];// template X derivative
		double iTy = m_matchModel[pyramidStep].edgeDerivativeY[angleStep][m];// template Y derivative
		if ((iSx != 0 || iSy != 0) && (iTx != 0 || iTy != 0))
		{
			//partial Sum = Sum of(((Source X derivative* Template X drivative) + Source Y derivative * Template Y derivative)) / Edge magnitude of(Template)* edge magnitude of(Source))
			partialSum += ((iSx*iTx) + (iSy*iTy))*(m_matchModel[pyramidStep].edgeMagnitude[angleStep][m] *
				matGradMag[curX * Sdx.cols + curY]);
		}
		sumOfCoords = m + 1;
		partialScore = partialSum / sumOfCoords;
		// check termination criteria
		// if partial score score is less than the score than needed to make the required score at that position
		// break serching at that coordinate.
		if (pyramidStep == m_pyramidLevels - 1)
		{
			if (partialScore < (MIN(((m_minMatchScore - 0.1) - 1) + normGreediness * sumOfCoords, normMinScore * sumOfCoords)))
				break;
		}
		else
		{
			if (partialScore < (MIN((m_minMatchScore - 1) + normGreediness * sumOfCoords, normMinScore * sumOfCoords)))
				break;
		}
		bfinish = true;
	}
	//cout << sumOfCoords << " " << m_matchModel[pyramidStep].cordinateSize << endl;
	if (bfinish && sumOfCoords > m_matchModel[pyramidStep].cordinateSize * m_greediness)
	{
		matchScore = partialScore; // Match score
		matchPoint = cv::Point(searchPoint.x, searchPoint.y);
	}
	return 0;
}