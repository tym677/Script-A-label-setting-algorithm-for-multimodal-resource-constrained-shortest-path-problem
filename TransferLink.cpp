#include "TransferLink.h"

TransferLink::TransferLink(int id):Link(id)
{
	
}

TransferLink::~TransferLink()
{
}


float TransferLink::getTT(const int& currentMode)//resType =1,2,...
{
		return tt;
}

float TransferLink::getTT(const int& currentMode, const float& t_arr)//resType =1,2,...
{
		return tt;
}


void TransferLink::disp()
{
	int i=0;
	ofstream OutToFile("OutToFile.txt", ios_base::app);
	OutToFile<<"linkid = "<<LinkId<<" toNode= "<<toNode<<" boardingArc= "<<boardingArc
		<<" tt= "<<tt<<" LinkType = "<<LinkType<<" (transfer link) ";
	for (i=1;i<=TOTALRESOURCE;i++)
		OutToFile<<"r"<<i<<"= "<<Resource[i]<<" ";

	OutToFile<<endl;
}