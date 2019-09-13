#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

using namespace std;

struct MatchModel

{

	int cordinateSize;

	int angleSize;

	int scaleSize;



	cv::Point** cordinates;

	short** edgeDerivativeX;

	short** edgeDerivativeY;

	double** edgeMagnitude;

	double* angles;

	double* scales;

};



struct MatchPara

{

	int matchScaleIndex;

	int matchAngleIndex;

	double matchScore;

	cv::Point matchPoint;

};



struct CompareByMatchScore

{

	bool operator()(const MatchPara& p1, const MatchPara& p2)

	{

		return p1.matchScore > p2.matchScore;

	}

};



class CShapeMatchAlgorithm

{

public:

	CShapeMatchAlgorithm();

	~CShapeMatchAlgorithm();



	int GetFeaturePoints(const cv::Mat src, const cv::Rect roi, const double minThresh, const double maxThresh,

		vector<cv::Point>& featurePoints);



	int CreateModel(const cv::Mat src, const vector<cv::Point> featurePoints, cv::Point& centerPoint);



	int SetMatchPara(const double minMatchRate, const double greediness, const double startAngle, const double endAngle,

		const double minScale, const double maxScale, const int maxMatchNum, const cv::Rect srcROI);



	int ShapeMatch(const cv::Mat src, vector<double>& matchScore, vector<double>& matchAngle, vector<double>& matchScale,

		vector<cv::Point>& matchPoint, vector <vector<cv::Point>>& resultFeaturePoints, double& timeSpan);

	void DeleteModel();



private:

	int GetPyramidLevelsAndCordinates(const vector<cv::Point> featurePoints, int& pyramidLevels, vector<vector<cv::Point>>& cordinates);

	int GetPyramid(const cv::Mat src, const int pyramidLevels, vector<cv::Mat>& pyramid);

	int GetDerivatives(const cv::Mat src, const vector<cv::Point> featurePoints, vector<short>& derivativesX,
		vector<short>& derivativesY);

	int GetAngles(const int pyramidLevels, const int featurePointsSize, vector<vector<double>>& angles);

	int GetScales(const int pyramidLevels, const int featurePointsSize, vector<vector<double>>& scales);

	int GetFeaturePointsMaxLen(const vector <cv::Point> featurePoints, int& featurePointsSize);

	int GetSearchImageInfo(const cv::Mat src, cv::Mat& sdx, cv::Mat& sdy, double* mag);

	int GetSearchAngleRange(const int pyramidLevel, int& startAngleIndex, int& endAngleIndex);

	int GetSearchScaleRange(const int pyramidLevel, int& startScaleIndex, int& endScaleIndex);

	int SelectMatchPoints(const int pyramidLevel, vector<MatchPara>& matchPara);

	int SearchHighestPyramidLevel(const cv::Mat src, const cv::Mat sdx, const cv::Mat sdy, const int startAngleIndex,
		const int endAngleIndex, const int startScaleIndex, const int endScaleIndex, const double* const mag,
		vector<MatchPara>& matchPara);

	int SearchLowPyramidLevel(const cv::Mat sdx, const cv::Mat sdy, const int pyCount, const double* const mag,
		vector<MatchPara>& matchPara);

	int SelectResult(vector<double>& matchScore, vector<double>& matchAngle, vector<double>& matchScale,
		vector<cv::Point>& matchPoint, vector <vector<cv::Point>>& resultFeaturePoints, vector<MatchPara>& matchPara);



	int Match(const cv::Mat Sdx, const cv::Mat Sdy, const int angleStep, const int scaleStep, const int pyramidStep,
		const double normMinScore, const double normGreediness, const double* const matGradMag, double& matchScore,
		cv::Point& matchPoint);

	int Match(const cv::Mat Sdx, const cv::Mat Sdy, const cv::Point searchPoint, const int angleStep,
		const int scaleStep, const int pyramidStep, const double normMinScore, const double normGreediness,
		const double* const matGradMag, double& matchScore, cv::Point& matchPoint);



	bool m_bCreateModel;

	bool m_bSetPara;

	double m_minMatchScore;

	double m_minScale;

	double m_maxScale;

	double m_startAngle;

	double m_endAngle;

	double m_greediness;

	cv::Rect m_srcROI;

	int m_maxMatchNum;

	int m_pyramidLevels;

	int m_featurePointsMaxLen;

	vector <cv::Point> featurePointCenter;

	MatchModel* m_matchModel;

	cv::Rect m_tempRect;

	cv::Point m_tempCenter;

};







