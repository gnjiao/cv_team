#pragma once

#include <boost\archive\text_iarchive.hpp>

#include <boost\archive\text_oarchive.hpp>

#include <string>

#include <vector>

#include <math.h>

#include <Eigen/Dense>

#include "Angle.h"



using namespace Eigen;



namespace geo

{

	class Point2D

	{

	private:

		friend class boost::serialization::access;

		template<class Archive>

		void serialize(Archive & ar, const unsigned int version)

		{

			ar & m_x;

			ar & m_y;

		}



	private:

		double m_x;

		double m_y;



	public:

		Point2D() : m_x(0), m_y(0)

		{



		}



		Point2D(double x, double y)

		{

			m_x = x;

			m_y = y;

		}



		Point2D(double r, Angle a)

		{

			m_x = r * cos(a.Radians());

			m_y = r * sin(a.Radians());

		}



		~Point2D()

		{

		}



		inline double X()const { return m_x; }



		inline double Y()const { return m_y; }



		inline bool operator == (const Point2D & p) const

		{

			if (m_x != p.m_x)

				return false;

			if (m_x != p.m_y)

				return false;

			return true;

		}



		inline bool operator != (const Point2D & p) const

		{

			if (m_x != p.m_x)

				return true;

			if (m_y != p.m_y)

				return true;

			return false;

		}



		inline Point2D operator + (const Point2D &p) const

		{

			return Point2D(m_x + p.m_x, m_y + p.m_y);

		}



		inline Point2D operator - (const Point2D &p) const

		{

			return Point2D(m_x - p.m_x, m_y - p.m_y);

		}



		inline Point2D operator + (const Vector2d & v)const

		{

			return Point2D(m_x + v(0), m_y + v(1));

		}



		inline Point2D operator - (const Vector2d & v)const

		{

			return Point2D(m_x - v(0), m_y - v(1));

		}



		inline Point2D operator * (const double d)const

		{

			return Point2D(m_x * d, m_y * d);

		}



		inline friend Point2D operator * (const double d, const Point2D &p)

		{

			return Point2D(d * p.m_x, d * p.m_y);

		}



		inline double Norm()const { return sqrt(m_x * m_x + m_y * m_y); }



		inline Vector2d VectorTo(const Point2D & p)const

		{

			return Vector2d(p.m_x - m_x, p.m_y - m_y);

		}



		inline double DistanceTo(const Point2D & p)const

		{

			Vector2d v = this->VectorTo(p);

			return v.norm();

		}



		inline Vector2d ToVector2D()const

		{

			return Vector2d(m_x, m_y);

		}



		inline Point2D TranslateBy(const Vector2d &v)const

		{

			return Point2D(m_x + v(0), m_y + v(1));

		}



		inline Point2D TranslateTo(const Point2D &p)const

		{

			return p;

		}



		inline Point2D RotateBy(const Point2D &center, const Angle &a)const

		{

			Matrix2d rot;

			rot << cos(a.Radians()), -sin(a.Radians()),

				sin(a.Radians()), cos(a.Radians());

			Vector2d v0 = (*this - center).ToVector2D();

			Vector2d v1 = rot * v0;

			return center + v1;

		}



		/*inline Point2D TransformBy(const AffineTrans &at)const

		{

		Point2D p = this->TranslateBy(at.Translation.ToVector2D());

		p = this->RotateBy(at.Center, at.Rotation);

		return p;

		}*/



		inline bool Equals(const Point2D &p, double tolerance)const

		{

			return this->DistanceTo(p) <= abs(tolerance);

		}



		inline std::string ToString()const

		{

			return std::to_string(m_x) + "," + std::to_string(m_y);

		}

	};

}


