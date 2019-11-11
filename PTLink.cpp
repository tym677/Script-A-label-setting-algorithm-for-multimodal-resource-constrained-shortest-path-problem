#include "PTLink.h"

PTLink::PTLink(int id):Link(id)
{
	  
}

PTLink::~PTLink()
{
}


float PTLink::getTT(const int& currentMode, const float& t_arr)//resType =1,2,...
{
	return tt;
}

float PTLink::getTT(const int& currentMode)//resType =1,2,...
{
	return tt;
}

void PTLink::disp()
{
	int i=0;
	ofstream OutToFile("OutToFile.txt", ios_base::app);
	OutToFile<<"linkid = "<<LinkId<<" toNode= "<<toNode<<" boardingArc= "<<boardingArc
		<<" tt= "<<tt<<" LinkType = "<<LinkType<<" (PTlink) ";
	for (i=1;i<=TOTALRESOURCE;i++)
		OutToFile<<"r"<<i<<"= "<<Resource[i]<<" ";

	OutToFile<<endl;
}