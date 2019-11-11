#ifndef ROADLINK_H
#define ROADLINK_H

#include "Link.h"
#include "Config.h"

using  namespace std;

//nodeid and link id is set from 1

class Link;

class RoadLink: public Link
{
public:
	
	float  TT[2];            // foot or car travel time on road links size of TT = 2

	RoadLink(int id);
	virtual ~RoadLink();
	
    virtual void disp();
	virtual float  getTT(const int& currentMode, const float& t_arr);
	virtual float  getTT(const int& currentMode);
};

#endif