#include "Node.h"

Node::Node(int anodeid)
{
	NodeId = anodeid;
	state_= 0;
	NodeIdLong = NodeId;
	t_depVec=0;
	maxDepindex_=0;
	ResTable_PT=0;
}

Node::~Node()
{

	delete t_depVec;
	/*for (int i=1;i<=N_PTNode_;i++)
		delete ResTable_PT[i];
	
	delete ResTable_PT;*/
	
}

void Node::disp()
{
	ofstream OutToFile("OutToFile.txt", ios_base::app);
	OutToFile<<"nodeid = "<<NodeId<<  fixed <<" NodeIdLong= "<<NodeIdLong<<" state_= "<<state_<<endl;
	
	//	OutToFile<<" x_= "<<x_<<" y_ = "<<y_;
}
void  Node::set_t_depVec(const float& first_dep, int PTmode)
{
	int i;
	float interval=0;
	if (PTmode == 1)//A =1, B=2,...
		interval = (float)INTERVAL_A;
	else
		interval = (float)INTERVAL_B;

	maxDepindex_ = (int)((T_ENDSERVICE - first_dep)/interval);

	t_depVec = new float [maxDepindex_+1];

	for (i=0;i<=maxDepindex_;i++)
		t_depVec[i] = first_dep + i*interval;

}



float Node::binary_search(const float& key, int low, int high)
{

	if (low > high) return (-1);	/* key not found */

	int middle = int((low+high)/2);
	
	if (key > t_depVec[middle] && key <= t_depVec[middle+1]) 
		return t_depVec[middle+1];
	if (key == t_depVec[middle])
		return t_depVec[middle];
	
	if (t_depVec[middle] > key) 
		return binary_search(key,low,middle-1) ;
	else
		return binary_search(key,middle+1,high) ;
}


float Node::getWaitTime(const float& t_arr)
{
	 ofstream OutToFile("OutToFile.txt", ios_base::app);
	//cout<<"\ngetWaitTime call"<<endl;
	if (t_arr <= t_depVec[0])
	{
		OutToFile<<"\nt_arr = "<<t_arr<<"  t_depVec[0]= "<< t_depVec[0]<<" t_depVec[0] - t_arr = "<<t_depVec[0] - t_arr;
		return t_depVec[0] - t_arr;
	}
	
	if (t_arr > t_depVec[maxDepindex_])
	{
		cout<<"\nt_arr > t_depVec[maxDepindex_] !!"<<" 86400.0 + t_depVec[0] = "<<86400.0 + t_depVec[0]
		<<" t_arr = "<<t_arr<<" 86400.0 + t_depVec[0] - t_arr = "<<86400.0 + t_depVec[0] - t_arr;
		return  86400.0 + t_depVec[0] - t_arr;
	}
	float  t_nextdep = binary_search(t_arr, 0, maxDepindex_);
	
	 if (t_nextdep != -1)
		 return t_nextdep - t_arr;
	 else
	 {
		 cout<<"\ngetWaitTime error at NodeId "<<NodeId<<" "
			 <<" NodeIdLong= "<<NodeIdLong<<endl;
		 for (int i = 0;i<=maxDepindex_;i++)
			 cout<<t_depVec[i]<<" ";
		  std::cin.get(); exit(-1);
		  return -1;
	 }
}


void Node::setCoord(float x, float y)
{
	x_ = x;
	y_ = y;
}
 
