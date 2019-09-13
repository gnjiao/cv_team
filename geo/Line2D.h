//Line2D.h

#pragma once

#include <boost\archive\text_iarchive.hpp>

#include <boost\archive\text_oarchive.hpp>

#include "Point2D.h"

#include "Angle.h"

#include <Eigen/Dense>



using namespace Eigen;



namespace geo

{

	class Line2D

	{

	private:

		friend class boost::serialization::access;

		template<class Archive>

		void serialize(Archive & ar, const unsigned int version)

		{

			ar & m_startPoint;

			ar & m_endPoint;

		}



	private:

		Point2D m_startPoint;

		Point2D m_endPoint;

		const double Precision = 0.0000000001;

	public:

		Line2D()

		{

		}



		Line2D(Point2D startPoint, Point2D endPoint)

		{

			m_startPoint = startPoint;

			m_endPoint = endPoint;

		}



		Line2D(Point2D &anyPoint, Vector2d &direction)

		{

			m_startPoint = anyPoint;

			m_endPoint = anyPoint + direction;

		}



		~Line2D()

		{

		}



		Line2D &operator = (const Line2D &rhs)

		{

			m_startPoint = rhs.StartPoint();

			m_endPoint = rhs.EndPoint();

			return *this;

		}



		inline bool operator == (const Line2D &other)const

		{

			if (this->m_startPoint != other.m_startPoint)

				return false;

			if (this->m_endPoint != other.m_endPoint)

				return false;

			return true;

		}



		inline bool operator != (const Line2D &other)const

		{

			if (this->m_startPoint != other.m_startPoint)

				return true;

			if (this->m_endPoint != other.m_endPoint)

				return true;

			return false;

		}



		inline Point2D StartPoint()const { return m_startPoint; }



		inline Point2D EndPoint()const { return m_endPoint; }



		inline Point2D MidPoint()const { return (m_startPoint + m_endPoint)*0.5; }



		inline double Length()const { return m_startPoint.DistanceTo(m_endPoint); }



		inline Vector2d Direction()const { return m_startPoint.VectorTo(m_endPoint).normalized(); }



		Angle SignedAngleToX(bool clockWise = false, bool returnNegative = true)const

		{

			Line2D xAxis = Line2D(Point2D(0, 0), Point2D(1, 0));

			return SignedAngleTo(xAxis, clockWise, returnNegative);

		}



		Angle SignedAngleToY(bool clockWise = false, bool returnNegative = true)const

		{

			Line2D yAxis = Line2D(Point2D(0, 0), Point2D(0, 1));

			return SignedAngleTo(yAxis, clockWise, returnNegative);

		}



		inline Point2D ClosestPointTo(const Point2D &p, bool mustBeOnSegment)const

		{

			Vector2d v = m_startPoint.VectorTo(p);

			double dotProduct = v.dot(Direction());

			if (mustBeOnSegment)

			{

				if (dotProduct < 0)

				{

					dotProduct = 0;

				}

				double l = Length();

				if (dotProduct > l)

				{

					dotProduct = l;

				}

			}

			Vector2d alongVector = dotProduct * Direction();

			return m_startPoint + alongVector;

		}



		inline Line2D LineTo(const Point2D &p, bool mustBeOnSegment)const

		{

			return Line2D(this->ClosestPointTo(p, mustBeOnSegment), p);

		}



		inline Angle SignedAngleTo(const Line2D &other, bool clockWise = false, bool returnNegative = true)const

		{

			int sign = clockWise ? -1 : 1;

			double a1 = atan2(this->Direction()(1), this->Direction()(0));

			if (a1 < 0)

			{

				a1 += 2 * PI;

			}



			double a2 = atan2(other.Direction()(1), other.Direction()(0));

			if (a2 < 0)

			{

				a2 += 2 * PI;

			}



			double a = sign * (a2 - a1);

			if (a < 0 && !returnNegative)

			{

				a += 2 * PI;

			}



			if (a > PI && returnNegative)

			{

				a -= 2 * PI;

			}



			return Angle(a, Rad);

		}



		inline Angle AngleTo(const Line2D &other)const

		{

			double x = this->Direction()(0) * other.Direction()(1) - other.Direction()(0)* this->Direction()(1);

			double y = this->Direction()(0) * other.Direction()(0) + other.Direction()(1)* this->Direction()(1);

			return Angle(abs(atan2(x, y)), Rad);

		}



		inline bool IsParallelTo(const Line2D &other)const

		{

			double dp = this->Direction().dot(other.Direction());

			return abs(1 - dp) <= Precision;

		}



		inline bool IsParallelTo(const Line2D &other, const Angle &tolerance)const

		{

			Angle a = this->AngleTo(other);

			if (a < tolerance)

			{

				return true;

			}

			return Angle(PI, Rad) - a < tolerance;

		}



		inline bool IsPerpendicularTo(const Line2D &other)const

		{

			double dp = this->Direction().dot(other.Direction());

			return abs(dp) < Precision;

		}



		inline bool IsPerpendicularTo(const Line2D &other, const Angle &tolerance)const

		{

			Angle a = this->AngleTo(other);

			return abs(a.Radians() - PI / 2) < tolerance.Radians();

		}



		inline Point2D IntersectWith(const Line2D &other)const

		{

			if (this->IsParallelTo(other))

			{

				return Point2D(INT_MAX, INT_MAX);

			}

			Point2D p = this->m_startPoint;

			Point2D q = other.m_startPoint;

			Vector2d r = this->m_startPoint.VectorTo(this->m_endPoint);

			Vector2d s = other.m_startPoint.VectorTo(other.m_endPoint);

			Vector2d v = (q - p).ToVector2D();

			double d1 = v(0) * s(1) - v(1) * s(0);

			double d2 = r(0) * s(1) - r(1) * s(0);

			double t = d1 / d2;

			return p + (t*r);

		}



		inline Point2D IntersectWith(const Line2D &other, const Angle &tolerance)const

		{

			if (this->IsParallelTo(other, tolerance))

			{

				return Point2D(INT_MAX, INT_MAX);

			}

			return this->IntersectWith(other);

		}



		inline Line2D TranslateBy(const Vector2d &v)const

		{

			return Line2D(m_startPoint + v, m_endPoint + v);

		}



		inline Line2D RotateBy(const Point2D &center, const Angle &a)const

		{

			Point2D p0 = m_startPoint.RotateBy(center, a);

			Point2D p1 = m_endPoint.RotateBy(center, a);

			return Line2D(p0, p1);

		}



		inline bool Equals(const Line2D &other, double tolerance)const

		{

			return this->m_startPoint.Equals(other.m_startPoint, tolerance)

				&& this->m_endPoint.Equals(other.m_endPoint, tolerance);

		}



		inline std::string ToString()const

		{

			return m_startPoint.ToString() + "," + m_endPoint.ToString();

		}



		inline Line2D ResizeByStartPoint(Point2D &temp)

		{

			return Line2D(temp, m_endPoint);

		}



		inline Line2D ResizeByEndPoint(Point2D &temp)

		{

			return Line2D(m_startPoint, temp);

		}



		inline Vector2d DirectionBy(Angle a = Angle(90, Deg))

		{

			Matrix2d rot;

			rot << cos(a.Radians()), -sin(a.Radians()),

				sin(a.Radians()), cos(a.Radians());

			return rot * Direction();

		}

	};

}