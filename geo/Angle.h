#pragma once

#include <boost\archive\text_iarchive.hpp>

#include <boost\archive\text_oarchive.hpp>

#include <string>



#define PI 3.14159265359

#define RadToDeg (180.0 / PI)

#define DegToRad (PI / 180.0)



namespace geo

{

	enum RadOrDeg

	{

		Rad,

		Deg

	};



	class Angle

	{

	private:

		friend class boost::serialization::access;

		template<class Archive>

		void serialize(Archive & ar, const unsigned int version)

		{

			ar & m_radians;

		}



	private:

		double m_radians = 0;



	public:

		Angle()

		{

		}



		Angle(double value, RadOrDeg type)

		{

			switch (type)

			{

			case geo::Rad:

				m_radians = value;

				break;

			case geo::Deg:

				m_radians = value * DegToRad;

				break;

			}

		}



		~Angle()

		{

		}



		inline bool operator == (const Angle &a)const

		{

			return m_radians == a.m_radians;

		}



		inline bool operator != (const Angle &a)const

		{

			return m_radians != a.m_radians;

		}



		inline bool operator < (const Angle &a)const

		{

			return m_radians < a.m_radians;

		}



		inline bool operator > (const Angle &a)const

		{

			return m_radians > a.m_radians;

		}



		inline bool operator <= (const Angle &a)const

		{

			return m_radians <= a.m_radians;

		}



		inline bool operator >= (const Angle &a)const

		{

			return m_radians >= a.m_radians;

		}



		inline Angle operator + (const Angle &a)const

		{

			return Angle(m_radians + a.m_radians, RadOrDeg::Rad);

		}



		inline Angle operator - (const Angle &a)const

		{

			return Angle(m_radians - a.m_radians, RadOrDeg::Rad);

		}



		inline Angle operator * (const double d)const

		{

			return Angle(m_radians * d, RadOrDeg::Rad);

		}



		inline friend Angle operator * (const double d, const Angle &a)

		{

			return Angle(d * a.m_radians, geo::Rad);

		}



		inline double Radians()const { return m_radians; }



		inline double Degrees()const { return m_radians * RadToDeg; }



		inline Angle Inverse()const

		{

			return Angle(-m_radians, RadOrDeg::Rad);

		}



		inline std::string ToRadString()const

		{

			return std::to_string(m_radians);

		}



		inline std::string ToDegString()const

		{

			return std::to_string(this->Degrees());

		}

	};

}

