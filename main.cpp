
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include "Config.h"
#include "PQ.h"
#include "PTLink.h"
#include "Network.h"
#include "kdtree2.h"
/*
#include <boost/multi_array.hpp>

using namespace boost;
using namespace std;

typedef multi_array<float,2> array2dfloat;
*/
void Clear()
{
	
	 ofstream OutToFile("OutToFile.txt", ios_base::out);
	 ofstream OutResTableRoad("OutResTableRoad.txt", ios_base::out);
	 ofstream OutResTablePT("OutResTablePT.txt", ios_base::out);
	 ofstream OutMinSpanTree("OutMinSpanTree.txt", ios_base::out);	 
	 ofstream OutLines("OutLines.txt", ios_base::out);
	 ofstream OutToFile_randstart("OutToFile_randstart.txt", ios_base::out);
	 OutToFile_randstart.clear();
	 OutToFile.clear();
	 OutResTableRoad.clear();
	 OutResTablePT.clear();
	 OutMinSpanTree.clear();
	 OutLines.clear();
}

float diffclock(clock_t clock1,clock_t clock2)
{
     float diffticks=clock1-clock2;
     float diffms=(diffticks)/(CLOCKS_PER_SEC);
     return diffms;
}

#define RN float(rand())/float((RAND_MAX+1.0))//random number between 0 and 1

int getNearestNeighbor(kdtree2::KDTree* tree, Node* pTarget);
int randBetween(int low, int high);//generate a number a such that low<=a<high
void generate_randstartnode(int N);
void read_randstartnode(int* vec_startnode, int N);

void main()
{
 
  ofstream OutToFile("OutToFile.txt", ios_base::app);
  ofstream OutMinSpanTree("OutMinSpanTree.txt", ios_base::app);//not used
  ofstream OutResTableRoad("OutResTableRoad.txt", ios_base::app);  
  ofstream OutResTablePT("OutResTablePT.txt", ios_base::app);
  ofstream OutLines("OutLines.txt", ios_base::app);
  Clear();
  clock_t end=0;
  clock_t begin=0;
  begin=clock();
  srand((unsigned)time(NULL));
  //int target =  200;//fred
  //int target =  350;//pren
  //int target =  932;//chicago sketch
  //chigardo regional and phil
  int i=0, startnode = 24,target =  8000, 
	       initState = CAR, 
		   stationNodeid =0, 
		   count=1, nStation=0, random_s=0;

  float t_dep = 7*3600;

  Network* pNetwork = new Network();
  
   
  pNetwork->initCSP(target,TOTALREFNODE);
  
  begin=clock();
  

  int N=N_STARTNODE;//N_STARTNODE = 100
  //generate_randstartnode(N);
  // exit(-1);
  int* vec_startnode = new int[N+1];  
  read_randstartnode(vec_startnode, N);
  
  for (int s=4;s<=11;s++)
  {	 
	  cout<<"\niter = "<<s;
	  OutToFile<<"\niter = "<<s<<" ";
	  //OutToFile<<"\nstart node = "<<vec_startnode[s]<<" ";
	  pNetwork->rBound[1] = pNetwork->vec_randbound[s-1][0];
	  pNetwork->rBound[2] = pNetwork->vec_randbound[s-1][1];
	  //cout<< pNetwork->rBound[1]<<" "<< pNetwork->rBound[2]<<" ";
	  cout<< pNetwork->CSP_A_starStatic(vec_startnode[s],target,TOTALREFNODE,  CAR);
      //pNetwork->CSP_Static(vec_startnode[s],target,TOTALREFNODE, CAR);
  }
  end=clock();
  OutToFile<<"\nexecuted time () = "<<diffclock(end , begin)<<endl;
 
  cout<<"\npress any key to continue!!"<<endl;
  std::cin.get();
  exit(-1);
  //delete tree;
  delete pNetwork;
  cout<<"\npress any key to continue!!"<<endl;
  std::cin.get();
}
void generate_randstartnode(int N)
{
  ofstream OutToFile_randstart("OutToFile_randstart.txt", ios_base::app);
  for (int s=1;s<=N;s++)
  {
	  int rand_start = rand()%TOTALREFNODE+1;
	  OutToFile_randstart<<rand_start<<" ";
  }
}

/*for static CSP*/
void read_randstartnode(int* vec_startnode, int N)
{
	ofstream OutToFile("OutToFile.txt", ios_base::app);
 
    ifstream iFilestartnode("ran_startnode5.txt");

    
	//read start nodes
	assert(iFilestartnode);

	int value=0;
    
	for (int i=1;i<=N;i++)	
	{
		iFilestartnode >> value;
		vec_startnode[i] = value;
		//cout<<vec_startnode[i]<<endl;
	}
	 //std::cin.get();
	//exit(-1);
}

int getNearestNeighbor(kdtree2::KDTree* tree, Node* pTarget)
{
  
  kdtree2::KDTreeResultVector result;
  vector<float> query(2);

  query[0] = pTarget->x_;
  query[1] = pTarget->y_;

  tree->n_nearest(query,1,result);
  
  return result[0].idx;
}

int randBetween(int low, int high) //generate a number a such that low<=a<high
{
	int rval = rand();
	if (low==high)
		return low;
	else
	   return low + rval % (high-low);
}
