#pragma once

#include <boost\archive\text_iarchive.hpp>

#include <boost\archive\text_oarchive.hpp>

#include "Line2D.h"

#include "Point2D.h"



namespace geo

{

	class RotRect2D

	{

	private:

		friend class boost::serialization::access;

		template<class Archive>

		void serialize(Archive & ar, const unsigned int version)

		{

			ar & m_center;

			ar & m_pointDir;

			ar & m_point0;

			ar & m_point1;

			ar & m_point2;

			ar & m_point3;

			ar & m_length;

			ar & m_width;

			ar & m_direction;

			ar & m_dirWidth;

		}



	private:

		Point2D m_center;

		Point2D m_pointDir;

		Point2D m_point0;

		Point2D m_point1;

		Point2D m_point2;

		Point2D m_point3;

		double m_length = 0;

		double m_width = 0;

		Point2D m_direction;

		Point2D m_dirWidth;

		Matrix2d m_rot90;



	public:

		RotRect2D()

		{

			m_rot90 << 0, -1,

				1, 0;

		}



		RotRect2D(Point2D center, double length, double width)

		{

			m_rot90 << 0, -1,
					   1, 0;

			m_center = center;

			m_length = length;

			m_width = width;

			m_direction = Point2D(1, 0);

			Vector2d v_direction = m_direction.ToVector2D();

			Vector2d v_dirWidth = m_rot90 * v_direction;

			m_dirWidth = Point2D(v_dirWidth(0), v_dirWidth(1));

			m_pointDir = m_center + 0.5 * m_length * m_direction;



			Vector2d vLenght, vWidth;

			vLenght = length * v_direction;

			vWidth = width * v_dirWidth;

			m_point0 = m_center - vLenght / 2 - vWidth / 2;

			m_point1 = m_center + vLenght / 2 - vWidth / 2;

			m_point2 = m_center + vLenght / 2 + vWidth / 2;

			m_point3 = m_center - vLenght / 2 + vWidth / 2;

		}

		RotRect2D(Point2D center, double length, double width, Vector2d &direction)

		{

			m_rot90 << 0, -1,

				1, 0;



			m_center = center;

			m_length = length;

			m_width = width;

			Vector2d v_direction = direction.normalized();

			m_direction = Point2D(v_direction(0), v_direction(1));

			Vector2d v_dirWidth = m_rot90 * v_direction;

			m_dirWidth = Point2D(v_dirWidth(0), v_dirWidth(1));

			m_pointDir = m_center + 0.5 * m_length * m_direction;



			Vector2d vLenght, vWidth;

			vLenght = length * v_direction;

			vWidth = width * v_dirWidth;

			m_point0 = m_center - vLenght / 2 - vWidth / 2;

			m_point1 = m_center + vLenght / 2 - vWidth / 2;

			m_point2 = m_center + vLenght / 2 + vWidth / 2;

			m_point3 = m_center - vLenght / 2 + vWidth / 2;



		}

		~RotRect2D()

		{

		}



		inline bool operator == (const RotRect2D &other)const

		{

			if (this->m_center != other.m_center)

				return false;

			if (this->m_length != other.m_length)

				return false;

			if (this->m_width != other.m_width)

				return false;

			if (this->m_direction != other.m_direction)

				return false;

			return true;

		}



		inline bool operator != (const RotRect2D &other)const

		{

			if (this->m_center != other.m_center)

				return true;

			if (this->m_length != other.m_length)

				return true;

			if (this->m_width != other.m_width)

				return true;

			if (this->m_direction != other.m_direction)

				return true;

			return false;

		}



		inline Point2D Center()const { return m_center; }



		inline Point2D Point0()const { return m_point0; }



		inline Point2D Point1()const { return m_point1; }



		inline Point2D Point2()const { return m_point2; }



		inline Point2D Point3()const { return m_point3; }



		inline double Lenght()const { return m_length; }



		inline double Width()const { return m_width; }



		inline Vector2d Direction()const { return m_direction.ToVector2D(); }



		Angle SignedAngleToX(bool clockWise = false, bool returnNegative = true)const

		{

			Line2D xAxis = Line2D(Point2D(0, 0), Point2D(1, 0));

			Line2D l = Line2D(m_center, m_pointDir);

			return l.SignedAngleTo(xAxis, clockWise, returnNegative);

		}



		Angle SignedAngleToY(bool clockWise = false, bool returnNegative = true)const

		{

			Line2D yAxis = Line2D(Point2D(0, 0), Point2D(0, 1));

			Line2D l = Line2D(m_center, m_pointDir);

			return l.SignedAngleTo(yAxis, clockWise, returnNegative);

		}



		inline double Diagonal()const { return sqrt(m_length * m_length + m_width * m_width); }



		inline double Circumference()const { return 2 * (m_length + m_width); }



		inline double Area()const { return m_length * m_width; }



		inline RotRect2D ResizeByPoint0(const Point2D &p0)const

		{

			Vector2d v = (m_point2 - p0).ToVector2D();

			double diagonal = v.norm();

			double cosTheta = m_direction.ToVector2D().dot(v.normalized());

			double sinTheta = sqrt(1 - cosTheta * cosTheta);

			return RotRect2D(

				(p0 + m_point2)*0.5,

				abs(diagonal*cosTheta),

				abs(diagonal *sinTheta),

				m_direction.ToVector2D());

		}



		inline RotRect2D ResizeByPoint1(const Point2D &p1)const

		{

			Vector2d v = (p1 - m_point3).ToVector2D();

			double diagonal = v.norm();

			double cosTheta = m_direction.ToVector2D().dot(v.normalized());

			double sinTheta = sqrt(1 - cosTheta * cosTheta);

			return RotRect2D(

				(p1 + m_point3)*0.5,

				abs(diagonal*cosTheta),

				abs(diagonal *sinTheta),

				m_direction.ToVector2D());

		}



		inline RotRect2D ResizeByPoint2(const Point2D &p2)const

		{

			Vector2d v = (p2 - m_point0).ToVector2D();

			double diagonal = v.norm();

			double cosTheta = m_direction.ToVector2D().dot(v.normalized());

			double sinTheta = sqrt(1 - cosTheta * cosTheta);

			return RotRect2D(

				(m_point0 + p2)*0.5,

				abs(diagonal*cosTheta),

				abs(diagonal *sinTheta),

				m_direction.ToVector2D());

		}



		inline RotRect2D ResizeByPoint3(const Point2D &p3)const

		{

			Vector2d v = (m_point1 - p3).ToVector2D();

			double diagonal = v.norm();

			double cosTheta = m_direction.ToVector2D().dot(v.normalized());

			double sinTheta = sqrt(1 - cosTheta * cosTheta);

			return RotRect2D(

				(m_point1 + p3)*0.5,

				abs(diagonal*cosTheta),

				abs(diagonal *sinTheta),

				m_direction.ToVector2D());

		}



		inline RotRect2D Scaling(const double d)const

		{

			return RotRect2D(m_center, m_length * d, m_width * d, m_direction.ToVector2D());

		}



		inline RotRect2D TranslateBy(const Vector2d &v)const

		{

			return RotRect2D(m_center + v, m_length, m_width, m_direction.ToVector2D());

		}



		inline RotRect2D TranslateTo(const Point2D &p)const

		{

			return RotRect2D(p, m_length, m_width, m_direction.ToVector2D());

		}



		inline RotRect2D RotateBy(const Angle &a)const

		{

			Point2D newPointDir = m_pointDir.RotateBy(m_center, a);

			Vector2d newDirection = (newPointDir - m_center).ToVector2D().normalized();

			return RotRect2D(m_center, m_length, m_width, newDirection);

		}



		inline RotRect2D RotateTo(const Point2D &pointDir)const

		{

			Vector2d newDirection = (pointDir - m_center).ToVector2D().normalized();

			return RotRect2D(m_center, m_length, m_width, newDirection);

		}



		inline RotRect2D RotateBy(const Point2D &center, const Angle &a)const

		{

			Point2D newCenter = m_center.RotateBy(center, a);

			Point2D newPointDir = m_pointDir.RotateBy(center, a);

			Vector2d newDirection = (newPointDir - newCenter).ToVector2D().normalized();

			return RotRect2D(newCenter, m_length, m_width, newDirection);

		}



		inline bool Equals(const RotRect2D &r, double tolerance)const

		{

			return abs(this->m_length - r.m_length) <= abs(tolerance)

				&& abs(this->m_width - r.m_width) <= abs(tolerance)

				&& this->m_center == r.m_center

				&& this->m_direction == r.m_direction;

		}



		inline std::string ToString()const

		{

			return std::to_string(m_point0.X()) + "," + std::to_string(m_point0.Y()) + ","

				+ std::to_string(m_point1.X()) + "," + std::to_string(m_point1.Y()) + ","

				+ std::to_string(m_point2.X()) + "," + std::to_string(m_point2.Y()) + ","

				+ std::to_string(m_point3.X()) + "," + std::to_string(m_point3.Y());

		}

		inline Angle GetAngle()const

		{

			Vector2d vec = m_direction.ToVector2D();

			float an = std::atan2f(vec[1], vec[0]);

			return Angle(an, geo::Rad);

		}

	};

}