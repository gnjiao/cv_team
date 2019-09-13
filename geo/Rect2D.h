//Rect2D.h

#pragma once

#include <boost\archive\text_iarchive.hpp>

#include <boost\archive\text_oarchive.hpp>

#include "Point2D.h"



namespace geo

{

	class Rect2D

	{

	private:

		friend class boost::serialization::access;

		template<class Archive>

		void serialize(Archive & ar, const unsigned int version)

		{

			ar & m_center;

			ar & m_leftTop;

			ar & m_leftBot;

			ar & m_rightTop;

			ar & m_rightBot;

			ar & m_length;

			ar & m_width;

		}



	private:

		Point2D m_center;

		Point2D m_leftTop;

		Point2D m_leftBot;

		Point2D m_rightTop;

		Point2D m_rightBot;

		double m_length = 0;

		double m_width = 0;



	public:

		Rect2D()

		{

		}



		Rect2D(Point2D center, double length, double width)

		{

			m_center = center;

			m_length = abs(length);

			m_width = abs(width);



			m_leftTop = Point2D(m_center.X() - m_length / 2, m_center.Y() - m_width / 2);

			m_leftBot = Point2D(m_center.X() - m_length / 2, m_center.Y() + m_width / 2);

			m_rightTop = Point2D(m_center.X() + m_length / 2, m_center.Y() - m_width / 2);

			m_rightBot = Point2D(m_center.X() + m_length / 2, m_center.Y() + m_width / 2);

		}



		Rect2D(Point2D leftTop, Point2D rightBot)

		{

			m_center = (leftTop + rightBot) * 0.5;

			m_length = abs(rightBot.X() - leftTop.X());

			m_width = abs(rightBot.Y() - leftTop.Y());



			m_leftTop = leftTop;

			m_leftBot = Point2D(m_center.X() - m_length / 2, m_center.Y() + m_width / 2);

			m_rightTop = Point2D(m_center.X() + m_length / 2, m_center.Y() - m_width / 2);

			m_rightBot = rightBot;

		}



		~Rect2D()

		{

		}



		inline bool operator == (const Rect2D &other)const

		{

			if (this->m_center != other.m_center)

				return false;

			if (this->m_length != other.m_length)

				return false;

			if (this->m_width != other.m_width)

				return false;

			return true;

		}



		inline bool operator != (const Rect2D &other)const

		{

			if (this->m_center != other.m_center)

				return true;

			if (this->m_length != other.m_length)

				return true;

			if (this->m_width != other.m_width)

				return true;

			return false;

		}



		inline Point2D Center()const { return m_center; }



		inline Point2D LeftTop()const { return m_leftTop; }



		inline Point2D LeftBot()const { return m_leftBot; }



		inline Point2D RightTop()const { return m_rightTop; }



		inline Point2D RightBot()const { return m_rightBot; }



		inline double Length()const { return m_length; }



		inline double Width()const { return m_width; }



		inline double Diagonal()const { return sqrt(m_length * m_length + m_width * m_width); }



		inline double Circumference()const { return 2 * (m_length + m_width); }



		inline double Area()const { return m_length * m_width; }



		inline Rect2D ResizeByLeftTop(const Point2D &leftTop)const

		{

			return Rect2D(leftTop, m_rightBot);

		}



		inline Rect2D ResizeByLeftBot(const Point2D &leftBot)const

		{

			Point2D center = (leftBot + m_rightTop)*0.5;

			double length = abs(leftBot.X() - m_rightTop.X());

			double width = abs(leftBot.Y() - m_rightTop.Y());

			return Rect2D(center, length, width);

		}



		inline Rect2D ResizeByRightTop(const Point2D &rightTop)const

		{

			Point2D center = (rightTop + m_leftBot)*0.5;

			double length = abs(rightTop.X() - m_leftBot.X());

			double width = abs(rightTop.Y() - m_leftBot.Y());

			return Rect2D(center, length, width);

		}



		inline Rect2D ResizeByRightBot(const Point2D &rightBot)const

		{

			return Rect2D(m_leftTop, rightBot);

		}



		inline Rect2D Scaling(const double d)const

		{

			return Rect2D(m_center, m_length * d, m_width * d);

		}



		inline Rect2D TranslateBy(const Vector2d &v)const

		{

			return Rect2D(m_center + v, m_length, m_width);

		}



		inline Rect2D TranslateTo(const Point2D &p)const

		{

			return Rect2D(p, m_length, m_width);

		}



		inline Rect2D RotateBy(const Point2D &center, const Angle &a)const

		{

			Point2D newCenter = m_center.RotateBy(center, a);

			return Rect2D(newCenter, m_length, m_width);

		}



		inline bool Equals(const Rect2D &r, double tolerance)const

		{

			return abs(this->m_length - r.m_length) <= abs(tolerance)

				&& abs(this->m_width - r.m_width) <= abs(tolerance)

				&& this->m_center == r.m_center;

		}



		inline std::string ToString()const

		{

			return std::to_string(m_leftTop.X()) + "," + std::to_string(m_leftTop.Y()) + ","

				+ std::to_string(m_rightTop.X()) + "," + std::to_string(m_rightTop.Y()) + ","

				+ std::to_string(m_rightBot.X()) + "," + std::to_string(m_rightBot.Y()) + ","

				+ std::to_string(m_leftBot.X()) + "," + std::to_string(m_leftBot.Y());

		}

	};

}

