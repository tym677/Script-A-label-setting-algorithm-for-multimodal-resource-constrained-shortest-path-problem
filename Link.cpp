#include "Link.h"


Link::Link(int id)
{
	LinkId= id;
	LinkType=ROAD;//mode of link: 1:W or road, 2 :reserve, 3:metro
	Hide=0;
	boardingArc=0;
	nextLink=0;
	dist_=0;
	tt=0;
}

Link::~Link()
{
} 

float Link::getTT(const int& currentMode, const float& t_arr)//resType =1,2,...
{
		return 0;
}
float Link::getTT(const int& currentMode)
{

	return 0;

}//resType =1,2,...

float Link::getDist()
{
	return dist_;
}

void Link::disp()
{
}
