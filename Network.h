#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include "Config.h"
#include "PQ.h"
#include "Node.h"
#include "Link.h"
#include "RoadLink.h"
#include "PTLink.h"
#include "TransferLink.h"
#include <ctime>
#include <cstdlib>
#include "kdtree2.h"

using  namespace std;

class Node;
class Link;
class RoadLink;
class PTLink;
class TransferLink;
class CostCompare ;
//nodeid and link id is set from 1

class Network
{

protected:
  	int      Nnode_;
	int      Nlink_;
	int      N_PTNode_;//nomber of PT nodes
	Node*    NodeTable[N_NODE_MAX];
	Link*    firstOutlink[N_NODE_MAX];	/* adjacency info */
	int      sizeOutlink[N_NODE_MAX];		/* outdegree of each vertex */
	float    ttToTarget[TOTALREFNODE+1];//tt by sec
	float    distToTarget[TOTALREFNODE+1];//tt by sec
	int      NFATable[TOTALSTATE][TOTALLINKTYPE];
	//check resource
	//set res
	float**  ResTable;//resource table
	//utility varaibles
	clock_t  end_;
    clock_t  begin_;
public:   

	int     NStation_;
	bool*   dominate_flag;//flag for label dominate indicators
	float   rBound[TOTALRESOURCE+1];
	bool*   NodeHide;
	float   shortest_dist;
	float   ttTargetToNearestS;
	int     NearestS;
    int     in_modeA[TOTALREFNODE+1];
	int     in_modeB[TOTALREFNODE+1];
	int     vec_randbound[N_STARTNODE][2];
	int*    stationNodeList;

	Network();
	~Network();
	
	void   creatNetwork();
	void   read_rand_bound();
	void   readNodes(int& Nnode, int& Nlink);
	void   readNodesTest(int& Nnode, int& Nlink);
	void   readLinksTest(int& Nnode, int& Nlink);
	void   readLinks(int& Nnode, int& Nlink, const int& inverse);
	void   readLinksByCoor(int& Nnode, int& Nlink);
	void   readNetTest(const int& Nnode, const int& Nlink);
	Node*   getNode(const int& anodeid);
	Link*   getFirstOutlink(const int& anodeid);
	float  A_star(const int& start,  const int& target, const int& Nnode,  const int& modeid);
	float  CSP_A_star(const int& start,  const int& target, const int& Nnode,  const int& modeid);	
	float  CSP_A_starDynamic(const int& start,  const int& target, int state, const float& t_dep);	
	float  CSP_A_starStatic(const int& start,  const int& target, const int& Nnode,  const int& modeid);
    float  CSP_Static(const int& start,  const int& target, const int& Nnode,  const int& modeid);
	//Nnode = TOTALNODE
	float  Dijkstra(const int& start,  const int& target,const int Nnode,  const int& resType);
	bool   Dijkstra_allnodePT(const int& start, const int& typeRes);//shortest path for all pair OD in PT
	bool   Dijkstra_AllNode(const int& start, const int& Nnode,  const int& modeid, const int& resType);//start<1 || start>TOTALREFNODE; endnode is one of the multimodal network	
	void   calDistToTarget(int target, const int& Nnode, const int& modeid);
	//set res
	void   setRes(const int& target);//set resource table for road and PT 
	void   setResStatic(const int& target);//set resource table for road only
	void   initRes(const int& target,const int& Nnode);
	void   initCSP(const int& target, const int& Nnode);
    void   setResPT();                                 //set resource table
	void   readRes(const int& target);
	void   readResStatic(const int& target);
	float  getRes(const int& s, const int& typeRes);//type de resource	
	void   testSetRes(const int& Nnode);
	void   initHideFlag(const int& Nnode);
	void   setResMultiNetwork(const int& start,  const int& target,   int state);
	void   computeNearestS(const int& target, kdtree2::KDTree* tree);
	
	//label algorithm
	bool   check_res_shortest(const int& start,  const int& target,const int& Nnode, int state);//resource bound
	bool   check_res_shortest(const int& start,  const int& target,const int& Nnode, int state,  const float& t_dep);
	bool   check_res_shortest_Dijkstra(const int& start,  const int& target,const int& Nnode, int state);
    bool   AnejaAlgo1(const int& start,  const int& end, const int& Nnode, const int& modeid);
	bool   AnejaAlgo1(const int& start, const int& end, const int& Nnode, const int& modeid,  const float& t_dep);//the first phase of Aneja's algorithm
    bool   AnejaAlgo1_Dijkstra(const int& start,  const int& end, const int& Nnode, const int& modeid);
	bool   check_res(const int& start);
	void   initLabelTable(const int& Nnode);
	bool   dominate(const int& nodeid, Label* p_label,  Label* next);
	bool   beDominated(const int& nodeid, Label* p_label, Label* pred);//check if label is dominated by the laset label in the vector
	bool   createLabel(const int& nodeid, Label* p_label,  const int& Nnode);
	bool   resCheck(Label* p_label, const Link* pLink);
	void   freeMemory(const int& Nnode);
	/*bool   UBcheck(Label* p_label, const int& fromnode, const int& tonode, const int& index, const int& end,
	                    const float& UB);*/

	/*Kruskal*/
	void  kruskal(int Nnode,int Nlink);
	void  to_edge_array(edge_pair e[],int Nnode);	
	void  quickSort(double* , int*,  int , int );
	/*creat PT line*/
	void  createPTLines();
	void  createLine(int n_line_metro, int n_stops_leftbound, int modeid);
	float dist_xy(const int& a1,const int& a2);
    void  createMultilevelNet(const int& n_line_mode1,const int&n_line_mode2);
	/*set automata table*/
	void  setFAT();
	int   getFAT(const int& state, const int& linktype );
	/*wait time*/
	float getTraveltime(const float& t_arr, const int& atnode, const bool& isBoardingArc,
	                    Link* pLink, const int& usedMode);
	float getttToTarget(const int& nodeid, const int& nodeState);
	/*get nearest node*/
	friend int getNearestNeighbor(kdtree2::KDTree* tree, Node* pTarget);


	Label** LabelTable;	

	friend float diffclock(clock_t clock1,clock_t clock2);

	friend int   randBetween(int low, int high);
};


#endif