#pragma once

#include <boost\archive\text_iarchive.hpp>

#include <boost\archive\text_oarchive.hpp>

#include "Point2D.h"

#include "Angle.h"

#include "Line2D.h"



namespace geo

{

	class Circle2D

	{

	private:

		friend class boost::serialization::access;

		template<class Archive>

		void serialize(Archive & ar, const unsigned int version)

		{

			ar & m_center;

			ar & m_radius;

		}



	private:

		Point2D m_center;

		double m_radius = 0;



	public:

		Circle2D()

		{

		}



		Circle2D(Point2D center, double radius)

		{

			m_center = center;

			m_radius = radius;

		}



		Circle2D(Point2D pointA, Point2D pointB, Point2D pointC)

		{

			Line2D line1(pointA, pointB);

			Line2D line2(pointA, pointC);

			if (line1.IsParallelTo(line2))

			{

				throw "Points cannot generate a circle";

			}



			Point2D midpointAB = (pointA + pointB) * 0.5;

			Point2D midpointBC = (pointB + pointC) * 0.5;

			double gradientAB = (pointB.Y() - pointA.Y()) / (pointB.X() - pointA.X());

			double gradientBC = (pointC.Y() - pointB.Y()) / (pointC.X() - pointB.X());

			double gradientl1 = -1 / gradientAB;

			double gradientl2 = -1 / gradientBC;



			double denominator = gradientl2 - gradientl1;

			double nominator = midpointAB.Y() - (gradientl1 * midpointAB.X()) + (gradientl2 * midpointBC.X()) - midpointBC.Y();

			double centerX = nominator / denominator;

			double centerY = (gradientl1 * (centerX - midpointAB.X())) + midpointAB.Y();

			Point2D center(centerX, centerY);



			m_center = center;

			m_radius = center.DistanceTo(pointA);

		}



		~Circle2D()

		{

		}



		inline bool operator == (const Circle2D &c)const

		{

			if (m_center != c.m_center)

				return false;

			if (m_radius != c.m_radius)

				return false;

			return true;

		}



		inline bool operator != (const Circle2D &c)const

		{

			if (m_center != c.m_center)

				return true;

			if (m_radius != c.m_radius)

				return true;

			return false;

		}



		inline Circle2D operator * (const double d)const

		{

			return Circle2D(m_center, m_radius * abs(d));

		}



		inline friend Circle2D operator * (const double d, const Circle2D &c)

		{

			return Circle2D(c.m_center, d*c.m_radius);

		}



		inline Point2D Center()const { return m_center; }



		inline double Radius()const { return m_radius; }



		inline double Circumference()const { return 2 * m_radius * PI; }



		inline double Diameter()const { return 2 * m_radius; }



		inline double Area()const { return PI * m_radius * m_radius; }



		inline Circle2D Scaling(const double d)const

		{

			return Circle2D(m_center, m_radius * d);

		}



		inline Circle2D TranslateBy(const Vector2d &v)const

		{

			return Circle2D(m_center + v, m_radius);

		}



		inline Circle2D TranslateTo(const Point2D &p)const

		{

			return Circle2D(p, m_radius);

		}



		inline Circle2D RotateBy(const Point2D &center, const Angle &a)const

		{

			Point2D newCenter = m_center.RotateBy(center, a);

			return Circle2D(newCenter, m_radius);

		}



		/* inline Circle2D TransformBy(const AffineTrans &at)const

		{

		Point2D newCenter = m_center.TransformBy(at);

		return Circle2D(newCenter, m_radius);

		}*/



		inline bool Equals(const Circle2D &c, double tolerance)const

		{

			return abs(m_radius - c.m_radius) <= abs(tolerance)

				&& m_center == c.m_center;

		}



		inline std::string ToString()const

		{

			return m_center.ToString() + "," + std::to_string(m_radius);

		}

	};

}

