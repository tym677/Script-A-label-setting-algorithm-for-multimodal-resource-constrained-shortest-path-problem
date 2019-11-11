#ifndef CONFIG_H
#define CONFIG_H
#include "limits.h"
 
#define RN float(rand())/float((RAND_MAX+1.0))

const float   dist_coor_converter =  100.0;//fred
              //dist_coor_converter =  100.0;//pren
              //dist_coor_converter =  100.0;//phil
              //dist_coor_converter =  100.0;//chicaco skech
               //dist_coor_converter =  3392.077;//chicaco regional
//  int in_modeA, in_modeB: arrays to store ref_nodeindex of stations of PT mode A and mode B  

const int     //TOTALREFNODE = 152,//total nodes of reference network
			  //TOTALREFLINK=166,
			  //TOTALNODE=8,//small CSP net
			  //TOTALLINK=30,//small CSP net
			 // TOTALNODE=224, //fred sketch
			  //TOTALLINK=523, //fred sketch
			  //TOTALNODE=352, //pren
			  //TOTALLINK=749, //pren		  
			  //TOTALNODE=933, //chicaco sketch
			  //TOTALLINK=2950, //chicaco sketch
			  //TOTALNODE=12982, //chicago regional, there is no arc for 12976 and 12977
			  //TOTALLINK=39018, //chicago regional
			  TOTALNODE=8267, //lorraine network, there is no arc for 12976 and 12977
			  TOTALLINK=22308, //lorraine network (bi-direction link)
			  N_STARTNODE=100,
			 // TOTALNODE=13389, //phil
			  //TOTALLINK=40003, //phil
			  TOTALREFNODE = TOTALNODE,		
			  TOTALREFLINK = TOTALLINK,
			  MODEINDEXMAX =9,//max num of modes
			  TOTALRESOURCE=2,//resource constraints
			  //PT
			  INTERVAL_A = 300,//Metro frequency
			  INTERVAL_B = 600,//tram frequency
			  //total network
			  PQ_SIZE = 40000, //priority queue size
			  MAXINT  = INT_MAX,
			  TOTALSTATE=4,//FOOT, CAR, PT, ROAD
			  TOTALLINKTYPE=5,//FOOT, CAR, PT, ROAD, TRANSFER
			  N_NODE_MAX=20000,
			  N_PTNODE_MAX=20000;

const float   WALKSPEED       = 1.4,
			  CARSPEED        = 12.5,//m/sec ~ 45km/hr
			  METROSPEED      = 20.0,//m/sec, 45 km/hr
              BUSSPEED        = 12.5,//m/sec, 3.96 km/hr
              DIST_BOARDING   = 0.1,//0.1 km for boarding anf alighting
			  T_BOARDING      = DIST_BOARDING*1000/WALKSPEED,
              DIST_TRANSFERT  = 0.1,//100m from walking network to metro/bus
			  T_STARTSERVICE  = 18000.0,//for mode A and B 5:00AM
			  T_ENDSERVICE    = 86400.0,//24:00
			  T1= 25200.0,//for time dependent raod link travel time function AM7:00 
			  T2= 28800.0,//AM8:00
			  T3= 34200.0,//AM9:30
			  BETA1           =  0.0002,//1.4,
			  BETA2           = -0.0001;//-0.85;
const double  INF= 1.79769e+308; //chicako skech
const float   R_BOUND1=10.0,
			  R_BOUND2=10.0;
/*pre-declaration*/
//link state enum
enum {FOOT, CAR, PT, ROAD, TRANSFER};//0,1,2

struct ShortestPath
{
	bool* flag_newpath;
	int*  pathindex;
};


struct Label
{
	int     Id;
	int     state;
	Label*  PredLabel;
	Label*  NextLabel;    //CSP algorithm
	int     NodeId;
	float   t_arr;
	float   Cost;//key (travel time or estimated travel time of s,t)
	float   TT;//travel time for A* algorithm in label setting algorithm
	float   Resource[TOTALRESOURCE+1];//1 is the first resource 
};

struct PairData
{
	int Key;
	Label* label;
};

struct NFATable//Non Determinate Automata for production
{
	int** Table;
};


class CostCompare 
{
public:
	bool operator () (Label* left, Label* right) 
	{ 
		return (left->Cost) > (right->Cost); 
	}
};


 struct myset_union{
        int p[TOTALNODE+1]; 		/* parent element */
        int size[TOTALNODE+1];           /* number of elements in subtree i */
	int n;				/* number of elements in set */
};


 struct edge_pair{
        int x, y;                       /*for Kruskal*/
		float weight;
};



#endif
