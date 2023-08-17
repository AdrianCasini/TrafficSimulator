#include "Tarif.h"


//------------------------------------------------------------------------------
Tarif::Tarif()
{
	mTarif = false;
	mText_eu = "";
	mText_km = "";
}

//------------------------------------------------------------------------------
Tarif::~Tarif()
{
	
}

//------------------------------------------------------------------------------
void Tarif::init(Toller* tll, Trace* tc)
{
	mTLL = tll;
	mTC  = tc;
}

//------------------------------------------------------------------------------
void Tarif::calculate()
{
	double buff_poly_x[SIM_MAX_TRIP_POINTS];
	double buff_poly_y[SIM_MAX_TRIP_POINTS];
	double x=0, y=0;
	double x_old = 0, y_old = 0;
	int ret = 0;
	bool in = false, init = true;
	int index_trip = 0;
	double trip =0.0;
	double topay = 0.0;

	for (int i = 0; i < mTLL->mPoly_points; i++)
	{
		buff_poly_x[i] = mTLL->mbuff_poly_points[2 * i + 0];
		buff_poly_y[i] = mTLL->mbuff_poly_points[2 * i + 1];
	}

	for (int j = 0; j < mTC->mPoints; j++)
	{
		x = mTC->m_buffer_points[3 * j + 0];
		y = mTC->m_buffer_points[3 * j + 1];

		ret = Simulator::pnpoly(mTLL->mPoly_points, buff_poly_x, buff_poly_y, x, y);
		if (ret == 1) //IN
		{

			mTC->mMap_buffer_trip[2 * index_trip + 0] = x;
			mTC->mMap_buffer_trip[2 * index_trip + 1] = y;
			index_trip++;
			in = true;


			mTC->mMap_buffer_color[4 * j + 0] = mTC->mColor_in.r;
			mTC->mMap_buffer_color[4 * j + 1] = mTC->mColor_in.g;
			mTC->mMap_buffer_color[4 * j + 2] = mTC->mColor_in.b;
			mTC->mMap_buffer_color[4 * j + 3] = mTC->mColor_in.f;
		}
		else //OUT
		{
			if (in == true)
			{
				in = false;
				//add fake point
				mTC->mMap_buffer_trip[2 * index_trip + 0] = 99999;
				mTC->mMap_buffer_trip[2 * index_trip + 1] = 99999;
				index_trip++;
			}

			mTC->mMap_buffer_color[4 * j + 0] = mTC->mColor_out.r;
			mTC->mMap_buffer_color[4 * j + 1] = mTC->mColor_out.g;
			mTC->mMap_buffer_color[4 * j + 2] = mTC->mColor_out.b;
			mTC->mMap_buffer_color[4 * j + 3] = mTC->mColor_out.f;
		}
	}

	for (int i = 0; i < index_trip; i++)
	{
		x = mTC->mMap_buffer_trip[2 * i + 0];
		y = mTC->mMap_buffer_trip[2 * i + 1];


		if ((x == 99999) || (y == 99999))
		{
			cout << "punto falso" << endl;
			init = true;
		}
		else if (init == true)
		{
			x_old = x;
			y_old = y;
			init = false;
			cout << "inicio:" << x << endl;
		}
		else
		{
			trip = std::sqrt( (x - x_old)*(x - x_old) + (y - y_old)*(y - y_old) ) + trip;
			x_old = x;
			y_old = y;
		}
	}
	cout << "Trip:[" << trip << "]" << endl;
	topay = trip * 15.0;

	mTarif = true;
	mText_km.formCadena("[%3.2f] Km", trip);
	mText_eu.formCadena("[%4.2f] euros",topay);

}
//------------------------------------------------------------------------------
void Tarif::clear()
{
	mTarif = false;
	mText_eu = "";
	mText_km = "";
}
