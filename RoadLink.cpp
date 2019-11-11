#include "RoadLink.h"

RoadLink::RoadLink(int id):Link(id)
{
	 TT[0]=0;TT[1]=0;
}

RoadLink::~RoadLink()
{

}

float RoadLink::getTT(const int& currentMode, const float& t_arr)//resType =1,2,...
{
	switch (currentMode)
	{
	case FOOT: 
		return TT[0];
		break;
	case CAR:
		if (t_arr<T3)
		{
			if (t_arr >= T2)
		       return TT[1] + BETA2*(t_arr-T2);
			else
			{
				if (t_arr >= T1)
					return TT[1] +  BETA1*(t_arr-T1);
				else
					return TT[1] ;
			}
		}
		else
			return TT[1] ;
		break;
	default:
       cout << "value of currentMode unknown";
	   std::cin.get();
	   exit(-1);
	   return -1;
  }
}


float RoadLink::getTT(const int& currentMode)//resType =1,2,...
{
	switch (currentMode)
	{
	case FOOT: 
		return TT[0];
		break;
	case CAR:
		return TT[1] ;
		break;
	default:
       cout << "value of currentMode unknown";
	   std::cin.get();
	   exit(-1);
	   return -1;
	}
}



void RoadLink::disp()
{
	int i=0;
	ofstream OutToFile("OutToFile.txt", ios_base::app);
	OutToFile<<"linkid = "<<LinkId<<" toNode= "<<toNode<<" boardingArc= "<<boardingArc
		<<" TT[0]= "<<TT[0]<<" TT[1] = "<<TT[1]<<" LinkType = "<<LinkType<<" (road link) ";
	for (i=1;i<=TOTALRESOURCE;i++)
		OutToFile<<"r"<<i<<"= "<<Resource[i]<<" ";

	OutToFile<<endl;
}