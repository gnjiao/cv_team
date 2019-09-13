#include "Calipers.h"



int Calipers::Caliper(const vector<Point2d> RectConers, vector<Point2d>& pointsOut)

{
	Point2d pointA = RectConers[0];

	Point2d pointB = RectConers[1];

	Point2d pointC = RectConers[2];

	Point2d pointD = RectConers[3];



	pointsOut.clear();

	for (int i = 0; i < 3; i++)

	{

		pointsOut.push_back(cv::Point2d(0, 0));

	}

	int locationXcc = 0, locationYcc = 0;

	Eigen::MatrixXd uniteAD;

	Eigen::MatrixXd uniteAB;

	int firstX = 0, firstY = 0;

	int nextX = 0, nextY = 0;

	int SortX = 0, SortY = 0;

	float MaxDifference;

	float thisMag = 0, nextMag = 0, firstMag = 0, maxMag = 0;

	Eigen::MatrixXd AB(2, 1), AD(2, 1);


	AB(0, 0) = pointB.x - pointA.x;

	AB(1, 0) = pointB.y - pointA.y;

	AD(0, 0) = pointD.x - pointA.x;

	AD(1, 0) = pointD.y - pointA.y;


	double thetaABAD = atan2(AB(1, 0), AB(0, 0)) - atan2(AD(1, 0), AD(0, 0));


	thetaABAD = thetaABAD * 180 / CV_PI;



	if (thetaABAD == 0)

	{

		return -1;

	}



	int CountAD = 0;

	int CountAB = 0;



	CountAD = AD.norm();

	CountAB = AB.norm();


	Eigen::MatrixXd differences = MatrixXd::Zero(CountAB - 1, CountAD);

	Eigen::MatrixXd differencesP = MatrixXd::Zero(CountAB - 1, CountAD);

	Eigen::MatrixXd differencesN = MatrixXd::Zero(CountAB - 1, CountAD);

	int tempDiff;


	if (m_polarProperty == 0)

	{

		for (int i = 0; i < CountAD; i++)

		{


			uniteAD = i *AD / CountAD;


			firstX = cvRound((pointA.x) + uniteAD(0, 0));

			firstY = cvRound((pointA.y) + uniteAD(1, 0));



			uchar* data = nullptr;

			MaxDifference = 0;


			locationXcc = firstX;

			locationYcc = firstY;



			for (int j = 0; j < CountAB - 1; j++)

			{

				uniteAB = (j + 1)*AB / CountAB;


				nextX = cvRound(firstX + uniteAB(0, 0));

				nextY = cvRound(firstY + uniteAB(1, 0));


				data = m_srcImage.ptr<uchar>(locationYcc) + locationXcc;

				thisMag = *data;

				data = m_srcImage.ptr<uchar>(nextY) + nextX;

				nextMag = *data;

				tempDiff = thisMag - nextMag;

				if (tempDiff>m_threshold_value)

				{

					differences(j, i) += tempDiff;

				}

				else differences(j, i) += 0;

				locationXcc = nextX;

				locationYcc = nextY;

			}

		}

		return FindLinePosition(differences, pointA, pointB, pointD, pointsOut);

	}

	if (m_polarProperty == 1)

	{

		for (int i = 0; i < CountAD; i++)

		{

			uniteAD = i *AD / CountAD;

			firstX = cvRound((pointA.x) + uniteAD(0, 0));

			firstY = cvRound((pointA.y) + uniteAD(1, 0));



			uchar* data = nullptr;

			MaxDifference = 0;

			locationXcc = firstX;

			locationYcc = firstY;



			for (int j = 0; j < CountAB - 1; j++)

			{

				uniteAB = (j + 1)*AB / CountAB;

				nextX = cvRound(firstX + uniteAB(0, 0));

				nextY = cvRound(firstY + uniteAB(1, 0));



				data = m_srcImage.ptr<uchar>(locationYcc) + locationXcc;

				thisMag = *data;

				data = m_srcImage.ptr<uchar>(nextY) + nextX;

				nextMag = *data;

				tempDiff = thisMag - nextMag;

				if (tempDiff < -m_threshold_value)

				{

					differences(j, i) += abs(tempDiff);

				}


				else differences(j, i) += 0;

				locationXcc = nextX;

				locationYcc = nextY;

			}

		}

		return FindLinePosition(differences, pointA, pointB, pointD, pointsOut);

	}



	if (m_polarProperty == 2)

	{

		for (int i = 0; i < CountAD; i++)

		{
			uniteAD = i *AD / CountAD;

			firstX = cvRound((pointA.x) + uniteAD(0, 0));

			firstY = cvRound((pointA.y) + uniteAD(1, 0));



			uchar* data = nullptr;

			MaxDifference = 0;



			locationXcc = firstX;

			locationYcc = firstY;



			for (int j = 0; j < CountAB - 1; j++)

			{

				uniteAB = (j + 1)*AB / CountAB;



				nextX = cvRound(firstX + uniteAB(0, 0));

				nextY = cvRound(firstY + uniteAB(1, 0));



				data = m_srcImage.ptr<uchar>(locationYcc) + locationXcc;

				thisMag = *data;

				data = m_srcImage.ptr<uchar>(nextY) + nextX;

				nextMag = *data;



				tempDiff = thisMag - nextMag;



				if (tempDiff>m_threshold_value)

				{

					differencesP(j, i) += tempDiff;

				}

				else differencesP(j, i) += 0;



				if (tempDiff<-m_threshold_value)

				{

					differencesN(j, i) += abs(tempDiff);

				}

				else differencesN(j, i) += 0;;

				locationXcc = nextX;

				locationYcc = nextY;

			}

		}

		return FindLinePosition2(differencesP, differencesN, pointA, pointB, pointD, pointsOut);

	}
	return 0;
}



int Calipers::FindLinePosition(Eigen::MatrixXd differences, const Point2d pointA,

	const Point2d pointB, const Point2d pointD, vector<Point2d>& pointsOut)

{

	Eigen::MatrixXd AB(2, 1), AD(2, 1);


	AB(0, 0) = pointB.x - pointA.x;

	AB(1, 0) = pointB.y - pointA.y;

	AD(0, 0) = pointD.x - pointA.x;

	AD(1, 0) = pointD.y - pointA.y;



	int CountAD = 0;

	int CountAB = 0;



	CountAD = AD.norm();

	CountAB = AB.norm();



	int countNum = 0;

	int differencesTemp = 0;

	int sumBest = 0;

	int sumColmun = 0;

	int sortX = 0, sortY = 0;



	if (m_resultType == 0)

	{

		for (int i = 0; i < CountAB - 1; i++)

		{

			sumColmun = 0;

			countNum = 0;

			for (int j = 0; j < CountAD; j++)

			{

				sumColmun += differences(i, j);

				differencesTemp = differences(i, j);



				if (differencesTemp != 0)

				{

					countNum++;

				}

			}

			if (countNum > CountAD / 3 || sumColmun > CountAD / 5 * m_threshold_value)

			{

				sortX = i;

				break;

			}

		}

	}

	if (m_resultType == 1)

	{

		for (int i = 0; i < CountAB - 1; i++)

		{

			sumColmun = 0;

			for (int j = 0; j < CountAD; j++)

			{

				sumColmun += differences(i, j);

			}



			if (sumColmun>sumBest)

			{

				sumBest = sumColmun;

				sortX = i;

				countNum++;

			}

		}

	}


	if (m_resultType == 2)

	{

		for (int i = CountAB - 2; i >= 0; i--)

		{

			sumColmun = 0;

			countNum = 0;

			for (int j = 0; j < CountAD; j++)

			{

				sumColmun += differences(i, j);

				differencesTemp = differences(i, j);



				if (differencesTemp != 0)

				{

					countNum++;

				}

			}

			if (countNum > CountAD / 3 || sumColmun > CountAD / 5 * m_threshold_value)

			{

				sortX = i;

				break;

			}

		}

	}



	if (countNum == 0)

	{

		pointsOut.clear();

		return -1;

	}



	pointsOut[0].x = cvRound(pointA.x + (sortX * AB / AB.norm())(0, 0));

	pointsOut[0].y = cvRound(pointA.y + (sortX * AB / AB.norm())(1, 0));



	pointsOut[1].x = cvRound(pointD.x + (sortX * AB / AB.norm())(0, 0));

	pointsOut[1].y = cvRound(pointD.y + (sortX * AB / AB.norm())(1, 0));



	pointsOut[2].x = cvRound((pointsOut[0].x + pointsOut[1].x) / 2);

	pointsOut[2].y = cvRound((pointsOut[0].y + pointsOut[1].y) / 2);



	return 0;

}



int Calipers::FindLinePosition2(Eigen::MatrixXd differencesP, Eigen::MatrixXd differencesN, const Point2d pointA,

	const Point2d pointB, const Point2d pointD, vector<Point2d>& pointsOut)

{

	Eigen::MatrixXd AB(2, 1), AD(2, 1);

	AB(0, 0) = pointB.x - pointA.x;

	AB(1, 0) = pointB.y - pointA.y;

	AD(0, 0) = pointD.x - pointA.x;

	AD(1, 0) = pointD.y - pointA.y;



	int CountAD = 0;

	int CountAB = 0;



	CountAD = AD.norm();

	CountAB = AB.norm();



	int countNumP = 0;

	int countNumN = 0;

	int differencesTempP = 0;

	int differencesTempN = 0;

	int sumBest = 0;

	int sumColmunP = 0;

	int sumColmunN = 0;

	int sortX = 0, sortY = 0;


	if (m_resultType == 0)

	{

		for (int i = 0; i < CountAB - 1; i++)

		{

			sumColmunP = 0;

			sumColmunN = 0;

			countNumP = 0;

			countNumN = 0;

			for (int j = 0; j < CountAD; j++)

			{

				sumColmunP += differencesP(i, j);

				sumColmunN += differencesN(i, j);

				differencesTempP = differencesP(i, j);

				differencesTempN = differencesN(i, j);

				if (differencesTempP != 0)

				{

					countNumP++;

				}

				if (differencesTempN != 0)

				{

					countNumN++;

				}

			}

			if (countNumP > CountAD / 3 || sumColmunP > CountAD / 5 * m_threshold_value

				|| countNumN > CountAD / 3 || sumColmunN > CountAD / 5 * m_threshold_value)

			{

				sortX = i;

				break;

			}

		}

	}

	if (m_resultType == 1)

	{

		for (int i = 0; i < CountAB - 1; i++)

		{

			sumColmunP = 0;

			sumColmunN = 0;

			for (int j = 0; j < CountAD; j++)

			{

				sumColmunP += differencesP(i, j);

				sumColmunN += differencesN(i, j);

				differencesTempP = differencesP(i, j);

				differencesTempN = differencesN(i, j);

			}



			if (sumColmunP>sumBest)

			{

				sumBest = sumColmunP;

				sortX = i;

				countNumP++;

			}

			if (sumColmunN>sumBest)

			{

				sumBest = sumColmunN;

				sortX = i;

				countNumN++;

			}

		}

	}

	if (m_resultType == 2)

	{

		for (int i = CountAB - 2; i >= 0; i--)

		{

			sumColmunP = 0;

			sumColmunN = 0;

			countNumP = 0;

			countNumN = 0;

			for (int j = 0; j < CountAD; j++)

			{

				sumColmunP += differencesP(i, j);

				sumColmunN += differencesN(i, j);

				differencesTempP = differencesP(i, j);

				differencesTempN = differencesN(i, j);

				if (differencesTempP != 0)

				{

					countNumP++;

				}

				if (differencesTempN != 0)

				{

					countNumN++;

				}

			}

			if (countNumP > CountAD / 3 || sumColmunP > CountAD / 5 * m_threshold_value

				|| countNumN > CountAD / 3 || sumColmunN > CountAD / 5 * m_threshold_value)

			{

				sortX = i;

				break;

			}

		}

	}



	if (countNumP == 0 && countNumN == 0)

	{

		pointsOut.clear();

		return -1;

	}



	pointsOut[0].x = cvRound(pointA.x + (sortX * AB / AB.norm())(0, 0));

	pointsOut[0].y = cvRound(pointA.y + (sortX * AB / AB.norm())(1, 0));



	pointsOut[1].x = cvRound(pointD.x + (sortX * AB / AB.norm())(0, 0));

	pointsOut[1].y = cvRound(pointD.y + (sortX * AB / AB.norm())(1, 0));



	pointsOut[2].x = cvRound((pointsOut[0].x + pointsOut[1].x) / 2);

	pointsOut[2].y = cvRound((pointsOut[0].y + pointsOut[1].y) / 2);



	return 0;

}