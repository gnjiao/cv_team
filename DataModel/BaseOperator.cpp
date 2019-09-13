#include "DataModel\BaseOperator.h"



void toolModel::BaseOperator::drawFeatures(cv::Mat)
{
}



Point2D toolModel::BaseOperator::transPoint2D(Point2D it)
{

	return it;

}



Line2D toolModel::BaseOperator::transLine2D(Line2D it)
{

	return Line2D(transPoint2D(it.StartPoint()), transPoint2D(it.EndPoint()));

}



Circle2D toolModel::BaseOperator::transCircle2D(Circle2D it)
{

	return Circle2D(transPoint2D(it.Center()), transValue(it.Radius()));

}



double toolModel::BaseOperator::transValue(double it)
{

	return it;

}



double toolModel::BaseOperator::transArea(double it)
{
	return it;

}





//

