#include "QuadMap.h"


//------------------------------------------------------------------------------
QuadMap::QuadMap()
{


}

//------------------------------------------------------------------------------
QuadMap::~QuadMap()
{

}

//------------------------------------------------------------------------------
void QuadMap::init()
{
	mQuadMng = new QuadRow[SIM_MAP_QUADS]; 
	set_quad(44.0, -10.0, 35.0, 5.0);
}

//------------------------------------------------------------------------------
void QuadMap::set_quad(double corner_NW_lat, double corner_NW_lon, double corner_SE_lat, double corner_SE_lon)
{
	double lat, lon;

	lat = corner_NW_lat;
	lon = corner_NW_lon;
	mDelta_lat = (corner_NW_lat - corner_SE_lat) / double(SIM_MAP_QUADS);
	mDelta_lon = (corner_SE_lon - corner_NW_lon) / double(SIM_MAP_QUADS);

	for (int ilat = 0; ilat < SIM_MAP_QUADS; ilat++)
	{
		lon = corner_NW_lon;
		for (int ilon = 0; ilon < SIM_MAP_QUADS; ilon++)
		{
			mQuadMng[ilat].Qxy[ilon].lat = lat;
			mQuadMng[ilat].Qxy[ilon].lon = lon;
			mQuadMng[ilat].Qxy[ilon].Y = Simulator::getYfromLat(lat);
			mQuadMng[ilat].Qxy[ilon].X = Simulator::getXfromLon(lon);

			lon = lon + mDelta_lon;
		}
		lat = lat - mDelta_lat;
	}
}

//------------------------------------------------------------------------------
void QuadMap::get_ref(double x, double y, double& xref, double& lonref, double& yref, double& latref, double& lat, double& lon)
{
	int ilon = 0;
	int ilat = 0;

	for (ilat = SIM_MAP_QUADS - 1; ilat >= 0; ilat--)
	{
		if (y < mQuadMng[ilat].Qxy[ilon].Y)
		{
			break;
		}
	}

	for (ilon = SIM_MAP_QUADS - 1; ilon >= 0; ilon--)
	{
		if (x > mQuadMng[ilat].Qxy[ilon].X)
		{
			break;
		}
	}

	xref = mQuadMng[ilat].Qxy[ilon].X;
	yref = mQuadMng[ilat].Qxy[ilon].Y;
	lonref = mQuadMng[ilat].Qxy[ilon].lon;
	latref = mQuadMng[ilat].Qxy[ilon].lat;
	lat = Simulator::get_delta_lat(latref, (y - yref));
	lon = Simulator::get_delta_lon(lonref, latref, lat, (x - xref));

}




