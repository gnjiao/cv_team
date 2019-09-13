#pragma once

#include "geo\Point2D.h"

namespace geo

{
	struct AffineTrans

	{

	private:

		friend class boost::serialization::access;

		template<class Archive>

		void serialize(Archive & ar, const unsigned int version)

		{

			ar & Center;

			ar & Translation;

			ar & Rotation;

		}



	public:

		geo::Point2D Center;

		geo::Point2D Translation;

		geo::Angle Rotation;



		AffineTrans Inverse()

		{

			AffineTrans at;

			at.Translation = this->Translation * (-1);

			at.Rotation = this->Rotation.Inverse();

			at.Center = this->Center - this->Translation;

			return at;

		}

	};

}


