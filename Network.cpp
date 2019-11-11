#pragma warning (disable : 4786)

#include "Network.h"
#include "myset_union.h"

Network::Network()
{
	int i=0;
	Nnode_ = 0;
	Nlink_ = 0;
	N_PTNode_=0;
	NStation_=0;

	shortest_dist=0.0;
    
	ttTargetToNearestS=0;
	NearestS=0;

	
	
  	for (i=1; i<=N_NODE_MAX; i++) //N_NODE_MAX is the max number of nodes
	{
		sizeOutlink[i]  = 0;
		firstOutlink[i] = 0;
	}

	for (i=0;i<=TOTALREFNODE;i++)
    {
      in_modeA[i]=0;
	  in_modeB[i]=0;
    }

   creatNetwork();//create ref nodes and links
   	setFAT();
  
}

Network::~Network()
{
	
	int i;

	for (i=0;i<=TOTALREFNODE;i++)
		delete ResTable[i];
	delete ResTable;

	for (i=1;i<=Nnode_;i++)
	{
		delete NodeTable[i];
		delete firstOutlink[i];
	}
	
	//delete LabelTable;	
	delete NodeHide;
	delete dominate_flag;

}

	
/*for static CSP*/
void  Network::read_rand_bound()
{
	//ifstream iFilestartnode("randbound.txt");
	ifstream iFilestartnode("randbound1.txt");
	assert(iFilestartnode);

	int value=0;
    
		
		for (int i=0;i<N_STARTNODE;i++)	
	    {
			for (int j=0;j<2;j++)
	        {
				iFilestartnode >> value;
				vec_randbound[i][j] = value;
				//cout<<vec_randbound[i][j]<<" ";
			}
		    //cout<<endl;
	    }
}

void Network::creatNetwork()
{
	//int i,j,modeid=0;
	readNodes(Nnode_, Nlink_);
	readLinksByCoor(Nnode_, Nlink_);//1 mean inverse network
	read_rand_bound();
	//readNetTest(Nnode_, Nlink_);
	//exit(-1);

}

void  Network::readNodes(int& Nnode, int& Nlink)
{
	ofstream OutToFile("OutToFile.txt", ios_base::app);
	int i=0,j=0,k=0,value=0,count=0;
	float fvalue1=0,fvalue2=0;

	//FILE* tf = fopen("friedrichshain_center_node.txt", "r");//change
	//FILE* tf = fopen("prenzlauerberg_center_node.txt", "r");//change
	//FILE* tf = fopen("ChicagoSketch_node.txt", "r");//change
   //FILE* tf = fopen("berlin_center_node.txt", "r");//change
	//FILE* tf = fopen("Philadelphia_node.txt", "r");//change
	//FILE* tf = fopen("ChicagoRegional_node.txt", "r");//change
	FILE* tf = fopen("lorraine_nodeset5.txt", "r");//change

	if (!tf)
	{
		cerr<<"\ncannot open input file in readNodes!!";
		exit(-1);
	}

	//create node table
	for (i=1;i<=TOTALREFNODE;i++)
	{
		fscanf (tf,"%d",&value);//form node
	    NodeTable[i] = new Node(value+1);//node id transcript   
		fscanf (tf,"%f",&fvalue1);//form node
		fscanf (tf,"%f",&fvalue2);//form node
		NodeTable[i]->setCoord(fvalue1,fvalue2);
		NodeTable[i]->state_ = FOOT;
		++count;
		++Nnode;
	}
}

void  Network::readNodesTest(int& Nnode, int& Nlink)
{
	ofstream OutToFile("OutToFile.txt", ios_base::app);
	int i=0,j=0,k=0,value=0,count=0;
	
	
}

//read road links
void Network::readLinks(int& Nnode, int& Nlink,  const int& inverse)
{
	ofstream OutToFile("OutToFile.txt", ios_base::app);
	int i=0,j=0,k=0,fromnode=0,tonode=0, count=0;
	float cost=0,res=0,dist;

	//FILE* tf = fopen("friedrichshain_center_net.txt", "r");//change
	//FILE* tf = fopen("prenzlauerberg_center_net.txt", "r");//change
	//FILE* tf = fopen("ChicagoSketch_net.txt", "r");//change
	//FILE* tf = fopen("berlin_center_net.txt", "r");//change
	//FILE* tf = fopen("Philadelphia_net.txt", "r");//change
	FILE* tf = fopen("ChicagoRegional_net.txt", "r");//change
	if (!tf)
	{
		cerr<<"\ncannot open input file in read link!!";
		exit(-1);
	}
	RoadLink* pLink=0;

	for (i=1;i<=TOTALREFLINK;i++)
	{
		 pLink = new RoadLink(i);
		 if (inverse)
		 {
            fscanf (tf,"%d",&tonode);
		    fscanf (tf,"%d",&fromnode);
		 }
		 else
		 {
			fscanf (tf,"%d",&fromnode);
		    fscanf (tf,"%d",&tonode);		 
		 }
		 fscanf (tf,"%f",&dist);
		 fscanf (tf,"%f",&cost);
		 fscanf (tf,"%f",&res);
		 pLink->toNode = tonode;
		 pLink->dist_ = dist;
		 pLink->nextLink = firstOutlink[fromnode];
		 pLink->TT[0] = dist*1000/WALKSPEED;//by sec
		 pLink->TT[1] = dist*1000/CARSPEED;
		 pLink->Resource[0] = pLink->TT[1];//by euro LinkType is initialized as ROAD
		 pLink->Resource[1] = cost;//by euro
		 pLink->Resource[2] = res;
		 firstOutlink[fromnode] = pLink;
		 ++sizeOutlink[fromnode];
		 ++Nlink;
	}

}

//read road links
void Network::readLinksByCoor(int& Nnode, int& Nlink)
{
	ofstream OutToFile("OutToFile.txt", ios_base::app);
	int i=0,j=0,k=0,fromnode=0,tonode=0, count=0, linkid=0;
	float cost=0,res=0;
	float dist=0;

	//FILE* tf = fopen("friedrichshain_center_net.txt", "r");//change
	//FILE* tf = fopen("prenzlauerberg_center_net.txt", "r");//change
	//FILE* tf = fopen("ChicagoSketch_net.txt", "r");//change
	//FILE* tf = fopen("berlin_center_net.txt", "r");//change
	//FILE* tf = fopen("Philadelphia_net.txt", "r");//change
	//FILE* tf = fopen("ChicagoRegional_net.txt", "r");//change
	FILE* tf = fopen("lorraine_linkset5_new.txt", "r");//change
	if (!tf)
	{
		cerr<<"\ncannot open input file in read link!!";
		exit(-1);
	}
	RoadLink* pLink=0;
	OutToFile<<"\n****dist****\n";
	for (i=1; i<= TOTALLINK; i++)
	{
		 pLink = new RoadLink(i);
		 // fscanf (tf,"%d",&linkid);//link id in the map
         fscanf (tf,"%d",&fromnode);
         fscanf (tf,"%d",&tonode);	 
		 fscanf (tf,"%f",&dist);
		 fscanf (tf,"%f",&cost);
		 fscanf (tf,"%f",&res);
		 pLink->toNode = tonode;
		 //OutToFile<<"\n"<<fromnode<<" "<<tonode<<" "<<dist;
		 pLink->dist_ = dist;//by km
		 pLink->nextLink = firstOutlink[fromnode];
		 pLink->TT[0] = dist*1000/WALKSPEED;//by sec
		 pLink->TT[1] = dist*1000/CARSPEED;
		 pLink->Resource[0] = pLink->TT[1];//by euro LinkType is initialized as ROAD
		 pLink->Resource[1] = cost;//by euro
		 pLink->Resource[2] = res;
		 firstOutlink[fromnode] = pLink;
		 ++sizeOutlink[fromnode];
		 ++Nlink;
	}
	OutToFile<<"\n****dist****\n";
}


void Network::readLinksTest(int& Nnode, int& Nlink)
{
	ofstream OutToFile("OutToFile.txt", ios_base::app);
	int i=0,j=0,k=0,fromnode=0,tonode=0, count=0;
	float cost=0,res=0,dist;

	//FILE* tf = fopen("ChicagoSketch_net.txt", "r");//change
	FILE* tf = fopen("data_constraitSP.txt", "r");//change
	if (!tf)
	{
		cerr<<"\ncannot open input file in readLinksTest!!";
		exit(-1);
	}
	RoadLink* pLink=0;
	for (i=1;i<=TOTALLINK;i++)
	{
		 pLink = new RoadLink(i);
         fscanf (tf,"%d",&fromnode);
		 fscanf (tf,"%d",&tonode);
		 fscanf (tf,"%f",&dist);
		 fscanf (tf,"%f",&cost);
		 fscanf (tf,"%f",&res);
		 
		 pLink->toNode = tonode;
		 pLink->dist_ = dist;
		 pLink->nextLink = firstOutlink[fromnode];
		 pLink->TT[0] = (int)dist;
		 pLink->TT[1] = (int)dist;
		 pLink->Resource[1] = cost;
		 pLink->Resource[2] = res;
		 firstOutlink[fromnode] = pLink;
		 ++sizeOutlink[fromnode];
		 ++Nlink;
	}

}

void Network::readNetTest(const int& Nnode, const int& Nlink)
{
     ofstream OutToFile("OutToFile.txt", ios_base::app);
	int i=0,j=0;
	//for ref level, node id is set from 1

	OutToFile<<"\nread link test!!\n";
	Link* pLink=0;
	for (i=1; i<=Nnode; i++)
	{
		//OutToFile<<"\nnode = "<<i<<" sizeOutlink = "<<sizeOutlink[i]<<endl;
		pLink = firstOutlink[i];
		int node_a=i,node_b=0;
		//OutToFile<<"\n node id = "<<NodeTable[i]->NodeId<<" x_ = "<<NodeTable[i]->x_<<" y_ = "<<NodeTable[i]->y_;
		while (pLink != 0)
		{
			node_b = pLink->toNode;
			double dist_temp = dist_xy(node_a,node_b);
			OutToFile<<"\nnode_a = "<<node_a<<" node_b = "<<node_b<<" dist by coord= "<<dist_xy(node_a,node_b)
				<<" getDist = "<<pLink->getDist()<<" ratio = "<<dist_xy(node_a,node_b)/pLink->getDist()
				<<" tt by car = "<<pLink->getTT(CAR)<<" res : "<<pLink->Resource[0]<<" "<<pLink->Resource[1]<<" "
				<<pLink->Resource[2]<<endl;
			pLink = pLink->nextLink;
		}
	}
	
}

Node* Network::getNode(const int& anodeid)
{
	return NodeTable[anodeid];

}

//compute dist to target node
void Network::calDistToTarget(int target, const int& Nnode, const int& modeid)
{
	ofstream OutToFile("OutToFile.txt", ios_base::app);
	int i;
	
	float x2=NodeTable[target]->x_,
	       y2=NodeTable[target]->y_;
	float diff_x, diff_y;

	for(i=1;i<=Nnode;i++)
	{
	    diff_x = NodeTable[i]->x_ - x2;
		diff_y = NodeTable[i]->y_ - y2;
		distToTarget[i] = (float)( sqrt( diff_x * diff_x + diff_y * diff_y ) * dist_coor_converter );
		//ttToTarget[i] = distToTarget[i]*1000.0/;// by km for compare with CPLEX	
	}

	

}
//Nnode is Nnode_ 
void Network::setResMultiNetwork(const int& start,  const int& target,   int state)
{
  ofstream OutToFile("OutToFile.txt", ios_base::app);
  int end = target;

  int currstate=state;//current state

  if (start<1 || start>Nnode_)
  {
	  cout<<"\nstart node out of range!! start node (1)= "<<start;
	  std::cin.get(); exit(-1);
  }
  if (end<1 || end>Nnode_)
  {
	  cout<<"\nend node out of range!! end node = "<<end;
	  std::cin.get(); exit(-1);
  }
  if (start == end)
	  cout<<"start == end tt=0";
	 
  //compute resr for input;
  //setRes(target);
  //exit(-1);
  //creat PT line for shortest path
  createPTLines();//create multilevel network
 
  readRes(target);//compute nearest station to the target and read res bound on shortest res paths. 
  
  //Note it needs to change readlink to normal link readLinks(Nnode_, Nlink_,0)
  //readNetTest(Nnode_, Nlink_);
  //calDistToTarget(target, TOTALREFNODE, 0);//compute dist of all ref nodes to target node
}

//startnode = 25, target = 932 , initState = FOOT,
float Network::CSP_A_starDynamic(const int& start, const int& target, int state, const float& t_dep)
{
  ofstream OutToFile("OutToFile.txt", ios_base::app);
  int currstate = state, end = target, newstate=0;
  bool flag=0;//1 means 
  flag = AnejaAlgo1(start, target, Nnode_, currstate, t_dep); //check if res on the shortest time path satisfoes all res constraints and check if shortest res path satisfies its constrain
  cout<<"\n****flag= "<<flag<<endl;
  //for labeling algorithm
  if (flag)//if flag ==1 means no solution
     return flag ;
  else
  {
  //cout<<"\nlabel setting algorithm"<<endl;
  int i,j,m, count_label=0, outlinkid=0, linktype=0, count_node=0;
  float dist_j=0, cost_j=0,t_arr=0, t_temp=0;
  bool isboarding=0;
  
  Link*     pLink=0;
  Label*    pLabel=0;
  Label*    end_label=0;
  Label*    elem=0;
  
  PQ queue;

  //init
  initLabelTable(Nnode_);

  //create new label
  pLabel = new Label;
  dominate_flag[count_label] = 0;
  pLabel->Id = count_label++;//label id
  pLabel->state = currstate;
  pLabel->TT = 0;
  pLabel->Cost =  getttToTarget(start,currstate);
  pLabel->NodeId = start;
  pLabel->PredLabel = NULL;
  pLabel->NextLabel = NULL;
  pLabel->t_arr = t_dep;
  for (i=1;i<=TOTALRESOURCE;i++)
     pLabel->Resource[i] = 0.0;//cumulated utilized resource until i
  createLabel(start, pLabel, Nnode_);
  queue.insert(pLabel);

  while( !queue.is_empty() ) 
  {
	  elem = queue.extract_min();
	  i = elem->NodeId;
	  currstate = elem->state;
	  t_arr =  elem->t_arr;
	  ++count_node;

	if ( !dominate_flag[elem->Id] )//id is label id
	{
		if (i == end)
		{	
			//end_=clock();		    
			float dist_temp = elem->TT;
			OutToFile<<"\ndist = "<<dist_temp;
			for ( m=1;m<=TOTALRESOURCE;m++)
				OutToFile<<" r"<<m<<" = "<<elem->Resource[m]<<" ";	
			OutToFile<<endl;
			do 
			{			
				elem = elem->PredLabel;
				if (elem != NULL)
				{
					OutToFile<<" ( "<<elem->NodeId;
					for ( m=1;m<=TOTALRESOURCE;m++)
						cout<<", r"<<m<<" = "<<elem->Resource[m]<<" ";
					OutToFile<<" ) "<<endl;
				}
			}while (elem != NULL);
			//OutToFile<<"\ntotal created labels = "<<count_label<<" total visited nodes = "<<count_node<<endl;
			freeMemory(Nnode_);
			return dist_temp;
		}//end if  (i == end)

		pLink = firstOutlink[i];
	    while (pLink != 0) 
	    {
			j = pLink->toNode;//outgoing node 
			if ( getFAT(currstate, pLink->LinkType) != -1 )
			{
				if ( resCheck(elem, pLink) )///if 1 no violation
				{	
				  isboarding =  pLink->boardingArc;
				  newstate = getFAT(currstate, pLink->LinkType);
			      t_temp = getTraveltime(t_arr,j,isboarding,pLink,currstate);
				  //create label
				  pLabel = new Label;
				  dominate_flag[count_label] = 0;
				  pLabel->Id = count_label++;
				  pLabel->state = newstate;		
				  pLabel->TT = elem->TT + t_temp;
				  pLabel->Cost = pLabel->TT + getttToTarget(j,newstate);
				  pLabel->t_arr = t_arr + t_temp;
				  pLabel->NodeId = j;
				  pLabel->PredLabel = elem;
				  pLabel->NextLabel = NULL;//init for the label* container
				  for (m=1;m<=TOTALRESOURCE;m++)
					  pLabel->Resource[m] = elem->Resource[m] + pLink->Resource[m];
				  if ( createLabel(j, pLabel, Nnode_) )
					  queue.insert(pLabel);
				  else
					  delete pLabel;  
				}		
				pLink = pLink->nextLink;
			}
			else
				pLink = pLink->nextLink;
		}//end while
	}//end dominate flag check	
  }//while	
 
  //cout<<"\nexecuted time = "<<diffclock(end_ , begin_)<<endl;
  //cout<<"\ntotal created labels = "<<count_label<<" total visited nodes = "<<count_node<<endl;
  freeMemory(Nnode_);
  return 1;
  }
}


float Network::CSP_A_star(const int& start,  const int& target, const int& Nnode,  const int& modeid)
{

  ofstream OutToFile("OutToFile.txt", ios_base::app);
  int end = target;
  if (start<1 || start>TOTALREFNODE)
  {
	  cout<<"\nstart node out of range!! start node (2)= "<<start;
	  std::cin.get(); exit(-1);
  }
  if (end<1 || end>TOTALREFNODE)
  {
	  cout<<"\nend node out of range!! end node = "<<end;
	  std::cin.get(); exit(-1);
  }
  if (start == end)
	  return 0;

  if (Nnode !=TOTALREFNODE)
  {
	  cout<<"\nNnode !=TOTALREFNODE error!!"<<endl;
	  std::cin.get();exit(-1);
  }
  //implement Aneja's network reduction algorithm
   
  initRes(target,Nnode);
  //cout<<"\nsetRes";
  //setRes(target, Nnode);
  //exit(-1);
  cout<<"\nreadRes";
  readRes(target);
  //cout<<"\ntestSetRes";
  testSetRes(Nnode);
  //exit(-1);
  //cout<<"\ncalDistToTarget call";
  begin_=clock();
 // calDistToTarget(target,  Nnode, 0);
  AnejaAlgo1(start, target, Nnode, 0); //reduce graph
  cout<<"\nlabel setting algorithm"<<endl;
  //for labeling algorithm
  
  int i,j,m, count_label=0, outlinkid=0, linktype=0, count_node=0;
  float dist_j=0, cost_j=0;
  
  Link*     pLink=0;
  Label*    pLabel=0;
  Label*    end_label=0;
  Label*    elem=0;
  
  PQ queue;

  //init
  initLabelTable(Nnode);

  //create new label
  pLabel = new Label;
  dominate_flag[count_label] = 0;
  pLabel->Id = count_label++;//label id
  pLabel->Cost = 0;
  pLabel->NodeId = start;
  pLabel->PredLabel = NULL;
  pLabel->NextLabel = NULL;
  for (i=1;i<=TOTALRESOURCE;i++)
     pLabel->Resource[i] = 0.0;//cumulated utilized resource until i
  createLabel(start, pLabel,Nnode);
  queue.insert(pLabel);

  while( !queue.is_empty() ) 
  {
	  elem = queue.extract_min();
	  i = elem->NodeId;
	  ++count_node;

	if ( !dominate_flag[elem->Id] )//id is label id
	{
		if (i == end)
		{	
			end_=clock();
		    cout<<"\nexecuted time = "<<diffclock(end_ , begin_)<<endl;
			float dist_temp = elem->Cost;
			cout<<"\nelem->Cost = "<<dist_temp;
			for ( m=1;m<=TOTALRESOURCE;m++)
				cout<<" r"<<m<<" = "<<elem->Resource[m]<<" ";	
			cout<<endl;
			do 
			{			
				elem = elem->PredLabel;
				if (elem != NULL)
				{
					cout<<" ( "<<elem->NodeId;
					for ( m=1;m<=TOTALRESOURCE;m++)
						cout<<", r"<<m<<" = "<<elem->Resource[m]<<" ";
					cout<<" ) "<<endl;
				}
			}while (elem != NULL);
			//cout<<"\ntotal created labels = "<<count_label<<" total visited nodes = "<<count_node<<endl;
			freeMemory(Nnode);
			return dist_temp;
		}//end if  (i == end)

		pLink = firstOutlink[i];
	    while (pLink != 0) 
	    {
			j = pLink->toNode;//outgoing node 
			if ( resCheck(elem, pLink) )///if 1 no violation
		    {				  
				  //create label
				  pLabel = new Label;
				  dominate_flag[count_label] = 0;
				  pLabel->Id = count_label++;
				  pLabel->Cost = elem->Cost + pLink->getTT(modeid,7*3600);
				  
				  pLabel->NodeId = j;
				  pLabel->PredLabel = elem;
				  pLabel->NextLabel = NULL;//init for the label* container
				  for (m=1;m<=TOTALRESOURCE;m++)
					  pLabel->Resource[m] = elem->Resource[m] + pLink->Resource[m];
				  if ( createLabel(j, pLabel, Nnode) )
					  queue.insert(pLabel);
				  else
					  delete pLabel;  
			}		
		  pLink = pLink->nextLink;
		}//end while
	}//end dominate flag check	
  }//while	
  end_ = clock();
  //cout<<"\nexecuted time = "<<diffclock(end_ , begin_)<<endl;
  //cout<<"\ntotal created labels = "<<count_label<<" total visited nodes = "<<count_node<<endl;
  freeMemory(Nnode);
  return 1;	
}

void Network::initCSP(const int& target, const int& Nnode)
{
	
  if (target<1 || target>TOTALREFNODE)
  {
	  cout<<"\nend node out of range!! end node = "<<target;
	  std::cin.get(); exit(-1);
  }
  if (Nnode !=TOTALREFNODE)
  {
	  cout<<"\nNnode !=TOTALREFNODE error!!"<<endl;
	  std::cin.get();exit(-1);
  }
  //cout<<"\nreadRes";
  readResStatic(target);
  //cout<<"\ntestSetRes";
  //testSetRes(Nnode);
  //exit(-1);
  
  //init

  //cout<<"\ncalDistToTarget call";
  //calDistToTarget(target,  Nnode, 0);


}

float  Network::CSP_Static(const int& start,  const int& target, const int& Nnode,  const int& modeid)
{
	 ofstream OutToFile("OutToFile.txt", ios_base::app);
  int end = target;
  if (start == end)
  {
	  OutToFile<<"\n"<<0;
	  return 0;
  }
  bool flag=0;
  initLabelTable(Nnode);
  flag = AnejaAlgo1_Dijkstra(start, target, Nnode, CAR); //reduce graph
  
  if (flag)//if flag ==1 means no solution
     return flag ;
  else
  {
  int i,j,m, count_label=0, outlinkid=0, linktype=0, count_node=0;
  float dist_j=0, cost_j=0;
  
  Link*     pLink=0;
  Label*    pLabel=0;
  Label*    end_label=0;
  Label*    elem=0;
  
  PQ queue;

  //create new label
  pLabel = new Label;
  dominate_flag[count_label] = 0;
  pLabel->Id = count_label++;//label id
  pLabel->TT = 0;
  pLabel->Cost = distToTarget[start]*1000/CARSPEED;
  pLabel->NodeId = start;
  pLabel->PredLabel = NULL;
  pLabel->NextLabel = NULL;
  for (i=1;i<=TOTALRESOURCE;i++)
     pLabel->Resource[i] = 0.0;//cumulated utilized resource until i
  createLabel(start, pLabel,Nnode);
  queue.insert(pLabel);

  while( !queue.is_empty() ) 
  {
	  elem = queue.extract_min();
	  i = elem->NodeId;
	  ++count_node;

	if ( !dominate_flag[elem->Id] )//id is label id
	{
		if (i == end)
		{	
			//cout<<"\nexecuted time = "<<diffclock(end_ , begin_)<<endl;
		   // OutToFile<<"\nexecuted time = "<<diffclock(end_ , begin_)<<endl;
			float dist_temp = elem->TT;
			OutToFile<<"\nstartnode = "<<start<<" TT = "<<dist_temp;
			for ( m=1;m<=TOTALRESOURCE;m++)
				OutToFile<<" r"<<m<<" = "<<elem->Resource[m]<<" ";	
			
			freeMemory(Nnode);
			return dist_temp;
		}//end if  (i == end)

		pLink = firstOutlink[i];
	    while (pLink != 0) 
	    {
			j = pLink->toNode;//outgoing node 
			dist_j = elem->TT+ pLink->getTT(CAR);
			if ( resCheck(elem, pLink) )///if 1 no violation
		    {				  
				  //create label
				  pLabel = new Label;
				  dominate_flag[count_label] = 0;
				  pLabel->Id = count_label++;
				  pLabel->TT = dist_j ;
				  pLabel->Cost = dist_j ;
				  pLabel->NodeId = j;
				  pLabel->PredLabel = elem;
				  pLabel->NextLabel = NULL;//init for the label* container
				  for (m=1;m<=TOTALRESOURCE;m++)
					  pLabel->Resource[m] = elem->Resource[m] + pLink->Resource[m];
				  if ( createLabel(j, pLabel, Nnode) )
					  queue.insert(pLabel);
				  else
					  delete pLabel;  
			}		
		  pLink = pLink->nextLink;
		}//end while
	}//end dominate flag check	
  }//while	
  freeMemory(Nnode);
  return 1;
  }

}

float Network::CSP_A_starStatic(const int& start,  const int& target, const int& Nnode,  const int& modeid)
{
  ofstream OutToFile("OutToFile.txt", ios_base::app);
  int end = target;
 
  if (start == end)
  {
	  OutToFile<<0<<endl;
	  return 0;
  }

  bool flag=0;
  initLabelTable(Nnode);

  flag = AnejaAlgo1(start, target, Nnode, CAR); //reduce graph
  //cout<<"\nflag= "<<flag;

  if (flag)//if flag ==1 means no solution
     return flag ;
  else
  {
  int i,j,m, count_label=0, outlinkid=0, linktype=0, count_node=0;
  float dist_j=0, cost_j=0;
  
  Link*     pLink=0;
  Label*    pLabel=0;
  Label*    end_label=0;
  Label*    elem=0;
  
  PQ queue;

  //create new label
  pLabel = new Label;
  dominate_flag[count_label] = 0;
  pLabel->Id = count_label++;//label id
  pLabel->TT = 0;
  pLabel->Cost = distToTarget[start]*1000/CARSPEED;
  //cout<<"\ndistToTarget["<<start<<"] = "<<distToTarget[start]<<" pLabel->Cost= "<<pLabel->Cost;
  pLabel->NodeId = start;
  pLabel->PredLabel = NULL;
  pLabel->NextLabel = NULL;
  for (i=1;i<=TOTALRESOURCE;i++)
     pLabel->Resource[i] = 0.0;//cumulated utilized resource until i
  createLabel(start, pLabel,Nnode);
  queue.insert(pLabel);

  while( !queue.is_empty() ) 
  {
	  elem = queue.extract_min();
	  i = elem->NodeId;
	  ++count_node;

	if ( !dominate_flag[elem->Id] )//id is label id
	{
		if (i == end)
		{	
			//cout<<"\nexecuted time = "<<diffclock(end_ , begin_)<<endl;
		    // OutToFile<<"\nexecuted time = "<<diffclock(end_ , begin_)<<endl;
			float dist_temp = elem->TT;
			OutToFile<<"startnode = "<<start<<" TT = "<<dist_temp;
			
			//cout<<"\nstartnode = "<<start<<" TT = "<<dist_temp;
			for ( m=1;m<=TOTALRESOURCE;m++)
				OutToFile<<" r"<<m<<" = "<<elem->Resource[m]<<" ";	
			OutToFile<<endl;
			/*do 
			{			
				elem = elem->PredLabel;
				if (elem != NULL)
				{
					cout<<" ( "<<elem->NodeId;
					for ( m=1;m<=TOTALRESOURCE;m++)
						cout<<", r"<<m<<" = "<<elem->Resource[m]<<" ";
					cout<<" ) "<<endl;
				}
			}while (elem != NULL);*/
			//cout<<"\ntotal created labels = "<<count_label<<" total visited nodes = "<<count_node<<endl;
			freeMemory(Nnode);
			return dist_temp;
		}//end if  (i == end)

		pLink = firstOutlink[i];
	    while (pLink != 0) 
	    {
			j = pLink->toNode;//outgoing node 
			dist_j = elem->TT+ pLink->getTT(CAR);
			if ( resCheck(elem, pLink) )///if 1 no violation
		    {				  
				  //create label
				  pLabel = new Label;
				  dominate_flag[count_label] = 0;
				  pLabel->Id = count_label++;
				  pLabel->TT = dist_j ;
				  pLabel->Cost = dist_j + distToTarget[j]*1000/CARSPEED;
				  pLabel->NodeId = j;
				  pLabel->PredLabel = elem;
				  pLabel->NextLabel = NULL;//init for the label* container
				  for (m=1;m<=TOTALRESOURCE;m++)
					  pLabel->Resource[m] = elem->Resource[m] + pLink->Resource[m];
				  if ( createLabel(j, pLabel, Nnode) )
					  queue.insert(pLabel);
				  else
					  delete pLabel;  
			}		
		  pLink = pLink->nextLink;
		}//end while
	}//end dominate flag check	
  }//while	
  freeMemory(Nnode);
  return 1;
  }
}


//ResTable is used only for road
void Network::initRes(const int& target,const int& Nnode)
{

	int i,j;
	ResTable = new float *[Nnode+1]; // allocation to thefirst parameter
    for (i=0;i<=Nnode;i++)
    {	
		ResTable[i]= new float [TOTALRESOURCE+1];
		for(j=0;j<=TOTALRESOURCE;j++)
			ResTable[i][j]=0; // allocation to thethird parameter
     }

}


/*
set up shortest res table for all nodes to target node in road network and for all PT nodes to each station
Note that it require read inverse network 
*/
void Network::setResStatic(const int& target)
{
	
	int k;
	//for road use inverse network
	for (k=0;k<=TOTALRESOURCE;k++)
	{
		
		if ( !Dijkstra_AllNode(target, TOTALREFNODE, 0, k ))//OK output to OutResTableRoad and rename it as OutResTableRoad1
		{
			cout<<"\nDijkstra_AllNode(target, TOTALREFNODE, 0, k ) error "
				<<" (target, TOTALREFNODE, 0, k) = "<<target<<" "<<TOTALREFNODE<<" "<<0<<" "<<k<<endl;
		}
	}
}


/*
set up shortest res table for all nodes to target node in road network and for all PT nodes to each station
Note that it require read inverse network 
*/
void Network::setRes(const int& target)
{
	 ofstream OutResTableRoad("OutResTableRoad.txt", ios_base::app);
	int k;
	//for road use inverse network
	for (k=0;k<=TOTALRESOURCE;k++)
	{
		
		if ( !Dijkstra_AllNode(target, TOTALREFNODE, 0, k ))//OK output to OutResTableRoad and rename it as OutResTableRoad1
		{
			cout<<"\nDijkstra_AllNode(target, TOTALREFNODE, 0, k ) error "
				<<" (target, TOTALREFNODE, 0, k) = "<<target<<" "<<TOTALREFNODE<<" "<<0<<" "<<k<<endl;
		}
	}
	
   createPTLines();//create multilevel network for set up res table for PT network,
                   //the output of this task is used for input 
  
   setResPT();
  
}

/*
set up shortest restable for all nodes in road network and for all PT nodes to each stations 
Note that it require read inverse network. The result is outputed to OutResTablePT.txt
*/
void Network::setResPT()
{
	 ofstream OutToFile("OutToFile.txt", ios_base::app);
	int i,k=0,target=0;
	//for all PT nodes to stations
	//OutToFile<<"\nin_modeA[i]>0 list : ";
	for (i=1;i<=TOTALREFNODE;i++)
	{
		if (in_modeA[i]>0)
		{
			target = in_modeA[i];
			//OutToFile<<in_modeA[i]<<" ";
			for (k=0;k<=TOTALRESOURCE;k++)
		    {
				if ( !Dijkstra_allnodePT(target, k) )
			    {
				   cout<<"\nDijkstra_allnodePT(target, k) error "
				       <<" (target, k) = "<<target<<" "<<k<<endl;	
				}
				
			}
		}
	}
	//OutToFile<<"\nin_modeB[i]>0 list : ";
	for (i=1;i<=TOTALREFNODE;i++)
	{
		if (in_modeB[i]>0)
		{
			target = in_modeB[i];
		    //OutToFile<<in_modeB[i]<<" ";
			for (k=0;k<=TOTALRESOURCE;k++)
		    {
				if ( !Dijkstra_allnodePT(target, k) )
			    {
				   cout<<"\nDijkstra_allnodePT(target, k) error "
				       <<" (target, k) = "<<target<<" "<<k<<endl;	
				}				
			}
		}
	}
	/*
	OutToFile<<"\nin_modeA[i]>0 list : ";
	for (i=1;i<=TOTALREFNODE;i++)
	{
		if (in_modeA[i]>0)
			OutToFile<<in_modeA[i]<<" ";
	}
	OutToFile<<"\nin_modeB[i]>0 list : ";
	for (i=1;i<=TOTALREFNODE;i++)
	{
		if (in_modeB[i]>0)
			OutToFile<<in_modeB[i]<<" ";
	}*/


}
//return minimum travel time or resource to target node 
float  Network::getRes(const int& s, const int& typeRes)//typeRes =1,2,....
{
	if ( s < TOTALREFNODE+1)
	   return ResTable[s][typeRes];
	else
	   return NodeTable[NearestS]->ResTable_PT[s-TOTALREFNODE][typeRes];
}
//target is known a priori, resource used from start to target node
bool Network::check_res(const int& start)
{
	int i;
	for (i=1;i<=TOTALRESOURCE;i++)
	{
		if (ResTable[start][i] > rBound[i])
			return 1;	
	}
	return 0;
}
//compute nearest station 
void Network::computeNearestS(const int& target, kdtree2::KDTree* tree)
{
	  ofstream OutToFile("OutToFile.txt", ios_base::app);
	  int i, temp=0;
	  Node* pTarget = NodeTable[target];
	  temp = getNearestNeighbor(tree, pTarget);
	  NearestS = stationNodeList[temp];
	  OutToFile<<"\n***computeNearestS***\n";
	  OutToFile<<"\nNearestS = "<<NearestS;
	  OutToFile<<"\nplus1 = "<<stationNodeList[temp+1];
	  OutToFile<<"\nminus1 = "<<stationNodeList[temp-1];
	  for (i=1;i<=TOTALREFNODE;i++)
	  {
		if ( in_modeA[i] == NearestS || in_modeB[i] == NearestS)
		{
		     ttTargetToNearestS = distToTarget[i] * 1000.0 / WALKSPEED;
			 cout<<"\ntarget = "<<target<<" NearestS = "<<NearestS
				 <<" ttTargetToNearestS= "<<ttTargetToNearestS<<" sec ( "<<ttTargetToNearestS/60.0<<" min )"<<endl;
			 break;
		}
		/*if ( in_modeA[i] == stationNodeList[temp+1] || in_modeB[i] == stationNodeList[temp+1])
		{
		     ttTargetToNearestS = distToTarget[i]*1000.0/WALKSPEED;
			 cout<<"\nttTargetToNearestS plus 1 = "<<ttTargetToNearestS<<endl;
		}
		if ( in_modeA[i] == stationNodeList[temp-1] || in_modeB[i] == stationNodeList[temp-1])
		{
		     ttTargetToNearestS = distToTarget[i]*1000.0/WALKSPEED;
	         cout<<"\nttTargetToNearestS minus 1 = "<<ttTargetToNearestS<<endl;
		}*/
	  }	
}

//set tt or res used from any PTnode to a station
void  Network::readResStatic(const int& target)
{
    ofstream OutToFile("OutToFile.txt", ios_base::app);
	int i,k;
	float fvalue;

	initRes(target,TOTALREFNODE);//init res from 0 to k (0 is travel time by sec)
	//setResStatic(target);//reclaculated for each target node (remember use inverse network)
	//exit(-1);
	//ifstream iFileRoad("OutResTableRoad_pren.txt");
	//ifstream iFileRoad("OutResTableRoad_fred.txt");
	//ifstream iFileRoad("OutResTableRoad_berlin.txt");
	
	ifstream iFileRoad("OutResTableRoad_lorraine.txt");
	//ifstream iFileRoad("OutResTableRoad1_chicaco.txt");
	//ifstream iFileRoad("OutResTableRoad_chicagoReg.txt");

	assert(iFileRoad);
	
	for (k=0;k<=TOTALRESOURCE;k++)//from 0 
	   for (i=1;i<=TOTALREFNODE;i++)	
		{
			iFileRoad >> fvalue;
			ResTable[i][k] = fvalue;
			//cout<<"\nResTable[i][k] = "<<ResTable[i][k];
		}

	for (i=1;i<=TOTALREFNODE;i++)	
	{
		distToTarget[i] = ResTable[i][0] * CARSPEED /1000; //Km

	}
}


//set tt or res used from any PTnode to a station
void  Network::readRes(const int& target)
{
    ofstream OutToFile("OutToFile.txt", ios_base::app);
	int i,k,value;
	float fvalue;
	
	initRes(target,TOTALREFNODE);

	ifstream iFileRoad("OutResTableRoad_lorraine.txt");
	assert(iFileRoad);
	
	for (k=0;k<=TOTALRESOURCE;k++)
	   for (i=1;i<=TOTALREFNODE;i++)	
		{
			iFileRoad >> fvalue;
			ResTable[i][k] = fvalue;
		}
	
	for (i=1;i<=TOTALREFNODE;i++)	
		distToTarget[i] = ResTable[i][0] * CARSPEED /1000 ; 

	ifstream iFilePT("OutResTablePT_lorraine.txt");
	assert(iFilePT);
	
	while (!iFilePT.eof())
	{
		//create res table for PT		
	    iFilePT>>value;
		NodeTable[value]->ResTable_PT = new float* [N_PTNode_+1];
		for (i=0;i<=N_PTNode_;i++)
		{
		    NodeTable[value]->ResTable_PT[i] = new float [TOTALRESOURCE+1];
			{
				for (k=0;k<=TOTALRESOURCE;k++)
				{
					NodeTable[value]->ResTable_PT[i][k] = 0;	
					//cout<<NodeTable[value]->ResTable_PT[i][k] <<" ";
				}
			}
		}

		for (k=0;k<=TOTALRESOURCE;k++)
		{
			  if (k>0)
			     iFilePT>>value;
			  for (i=1;i<=N_PTNode_;i++)
			  {
				  iFilePT>>fvalue;
				 // cout<<fvalue<<endl;
				  NodeTable[value]->ResTable_PT[i][k] = fvalue;
			  }
		}//end for k
	}//end while
	//compute nearest station to target node
	
    /*test
	OutToFile<<"\n****ResTable_Road test ****\n";
	
	   for (i=1;i<=TOTALREFNODE;i++)	
		{
			OutToFile<<i<<" ";
			for (k=1;k<=TOTALRESOURCE;k++)
			   OutToFile<<ResTable[i][k]<<" ";
			OutToFile<<endl;
		}
	   
	OutToFile<<"\n****ResTable_PT test ****\n";
	int j,v;
	for (i=1;i<=TOTALREFNODE;i++)
	{
	  if (in_modeA[i]>0)
	  {
		  v = in_modeA[i];
		  OutToFile<<"A"<<v<<" ";
		  for (j=1;j<=N_PTNode_;j++)	
		  {
			  for (k=0;k<=TOTALRESOURCE;k++)
				  OutToFile<<NodeTable[v]->ResTable_PT[j][k]<<" ";
		  }
		  OutToFile<<endl;
	  }
	  
	  if (in_modeB[i]>0)
	  {
		  v = in_modeB[i];
		  OutToFile<<"B"<<v<<" ";
		  for (j=1;j<=N_PTNode_;j++)	
		  {
			  for (k=0;k<=TOTALRESOURCE;k++)
				  OutToFile<<NodeTable[v]->ResTable_PT[j][k]<<" ";
		  }
			  OutToFile<<endl;
	  }
	}
	*/
}



float Network::A_star(const int& start,  const int& target, const int& Nnode,  const int& modeid)
{
  ofstream OutToFile("OutToFile.txt", ios_base::app);
  int end = target;
  if (start == end)
	  return 0;

  if (Nnode !=TOTALREFNODE)
  {
	  cout<<"\nNnode !=TOTALREFNODE error!!"<<endl;
	  std::cin.get();exit(-1);
  }

  cout<<"\ncalDistToTarget call";
  //calDistToTarget(target,  Nnode, 0);
 
  float*    distance = new float [Nnode+1];   //Nnode =    TOTALREFNODE   by sec
  float*    cost     = new float [Nnode+1];   //Nnode =    TOTALREFNODE   
  int*      parent   = new int    [Nnode+1];    
  bool*     inQueue  = new bool   [Nnode+1];  

  if (start<1 || start>TOTALREFNODE)
  {
	  cout<<"\nstart node out of range!! start node (3) = "<<start;
	  std::cin.get(); exit(-1);
  }
  if (end<1 || end>TOTALREFNODE)
  {
	  cout<<"\nend node out of range!! end node = "<<end;
	  std::cin.get(); exit(-1);
  }
  
  Link*     pLink;
  Label*    elem ;  
  Label*    pLabel;
  Label*    LabelTable[TOTALREFNODE+1] ; 
  PQ        queue;

  int i, j, k, count=0, outlinkid=0, linktype=0,countlink=0;
  float dist_j=0,cost_j=0;
  
  for ( i=1; i<=Nnode; i++) 
  {
    distance[i] = (float)MAXINT;
	cost[i] = (float)MAXINT;
    parent[i] = -1;
    inQueue[i] = 0;
  }
  pLabel = new Label;
  pLabel->NodeId = start;
  pLabel->Cost = distToTarget[start];//key
  distance[start] = 0;//dist from start node
  cost[start] = distToTarget[start];//key
  LabelTable[count++]=pLabel;
  queue.insert(pLabel);
  inQueue[start] = 1;
  while( !queue.is_empty()) 
  {
    elem = queue.extract_min();
	i = elem->NodeId;  
	if (i == end)
	{
		OutToFile<<"\nshortest path from "<<start
			<<" to "<<end<<" tt = "<<distance[i]<<endl;
		OutToFile<<"path = "<<end<<" ";
		do {
			end = parent[end];			
			OutToFile<<end<<" ";
		}while (end != start);
		//OutToFile<<" count = "<<count<<" source = "<<start<<" target = "<<target<<" "<<distance[i]<<endl;
		float distvalue= distance[i];	
		for (k=0;k<count;k++)
			delete LabelTable[k];
		delete inQueue;  delete parent;  delete distance;delete cost;
		return distvalue;
	}

	pLink = firstOutlink[i];
	while (pLink != 0) 
	{
		j = pLink->toNode;//outgoing node
		dist_j = distance[i] + pLink->getDist();	
		cost_j = dist_j+ distToTarget[j];
		if ( !inQueue[j] ) 
		{
			  inQueue[j] = 1;
			  distance[j] = dist_j;
			  cost[j] = cost_j;
			  parent[j] = i;	
			  pLabel = new Label;
              pLabel->NodeId = j;
			  pLabel->Cost = cost_j;
			  LabelTable[count++]=pLabel;
			  queue.insert(pLabel);
		}//end if
		else
		{
			if (cost_j < cost[j] )//decrease key
			{
				distance[j] = dist_j;
				cost[j] = cost_j;
				parent[j] = i;
				if ( ! queue.decrease_key(j,cost_j))
				{
				   //create new label
				   //cout<<"\n ! queue.decrease_key(j,cost_j) !!"<<endl;
			       pLabel = new Label;
                   pLabel->NodeId = j;
			       pLabel->Cost = cost_j;
				   LabelTable[count++]=pLabel;
			       queue.insert(pLabel);	
				   inQueue[j]=1;
				}
			}
		}
		pLink = pLink->nextLink;
	}//end while
  }//while
    
  //cout<<"\nshortest path from "<<start<<" to "<<end<<" = "<<"INF"<<endl;
  delete inQueue;
  delete parent;
  delete cost;
  delete distance; 	
  for (k=0;k<count;k++)
	delete LabelTable[k];
  return -1;
}

float Network::Dijkstra(const int& start,  const int& target, const int Nnode,  const int& modeid)
{
	ofstream OutToFile("OutToFile.txt", ios_base::app);
  int end = target;
  if (start == end)
  {
	  OutToFile<<"\nshortest path from "<<start
			<<" to "<<end<<" tt = "<<0<<endl;
	  return 0;
  }
  if (Nnode !=TOTALREFNODE)
  {
	  cout<<"\nNnode !=TOTALREFNODE error!!"<<endl;
	  std::cin.get();exit(-1);
  }

  float*  distance = new float [Nnode+1];   //Nnode =    TOTALREFNODE   by sec
  int*    parent   = new int    [Nnode+1];    
  bool*   inQueue  = new bool   [Nnode+1];  

  if (start<1 || start>TOTALREFNODE)
  {
	  cout<<"\nstart node out of range!! start node (4)= "<<start;
	  std::cin.get(); exit(-1);
  }
  if (end<1 || end>TOTALREFNODE)
  {
	  cout<<"\nend node out of range!! end node = "<<end;
	  std::cin.get(); exit(-1);
  }
  const int size  = (const int)(1.5*TOTALNODE);
  Link*     pLink;
  Label*    elem ;  
  Label*    pLabel;
  Label*    LabelTable[size] ; 
  PQ        queue;

  int i, j, k, count=0, outlinkid=0, linktype=0;
  float dist_j=0,cost_j=0;
  
  for ( i=1; i<=Nnode; i++) 
  {
    distance[i] = (float)MAXINT;
    parent[i] = -1;
    inQueue[i] = 0;
  }
  pLabel = new Label;
  pLabel->NodeId = start;
  pLabel->Cost = 0;//key
  distance[start] = 0;//dist from start node
  LabelTable[count++]=pLabel;
  queue.insert(pLabel);
  inQueue[start]=1;

  while( !queue.is_empty()) 
  {
    elem = queue.extract_min();
	i = elem->NodeId; 
	if (i == end)
	{
		OutToFile<<"\nshortest path from "<<start
			<<" to "<<end<<" tt = "<<distance[i]<<endl;
		OutToFile<<"path = "<<end<<" ";
		do {
			end = parent[end];
			OutToFile<<end<<" ";
		}while (end != start);
		OutToFile<<" count = "<<count<<" "<<start<<" "<<distance[i]<<endl;
		float distvalue= distance[i];	
		for (k=0;k<count;k++)
			delete LabelTable[k];
		delete inQueue;  delete parent;  delete distance;
		return distvalue;
	}
	
	pLink = firstOutlink[i];
	while (pLink != 0) 
	{
		j = pLink->toNode;//outgoing node
		dist_j = distance[i] + pLink->getDist();
		if ( !inQueue[j] ) 
		  {
			  inQueue[j] = 1;
			  distance[j] = dist_j;
			  parent[j] = i;
			  pLabel = new Label;
              pLabel->NodeId = j;
			  pLabel->Cost = dist_j;
			  LabelTable[count++]=pLabel;
			  queue.insert(pLabel);
		  }//end if
		  else
		  {
			 if (dist_j < distance[j] )//decrease key
			 {
				distance[j] = dist_j;				
				parent[j] = i;
				if ( ! queue.decrease_key(j,cost_j))
				{
				   //create new label
			       pLabel = new Label;
                   pLabel->NodeId = j;
			       pLabel->Cost = dist_j;
				   LabelTable[count++]=pLabel;
				   inQueue[j]=1;
			       queue.insert(pLabel);				
				}//end if
			 }//end if
		   }//end else
		pLink = pLink->nextLink;
	}//end while
  }//while
    
  //cout<<"\nshortest path from "<<start<<" to "<<end<<" = "<<"INF"<<endl;
  delete inQueue;
  delete parent;
  delete distance; 	
  for (k=0;k<count;k++)
	delete LabelTable[k];
  return -1;
 
}
/*compute the shortest time or res from all PT nodeq to all stations */
bool Network::Dijkstra_allnodePT(const int& start,  const int& typeRes)
{
	
  ofstream OutToFile("OutToFile.txt", ios_base::app);
   ofstream OutResTablePT("OutResTablePT.txt", ios_base::app);
  if (N_PTNODE_MAX < Nnode_)
  {
	  cout<<"\n N_PTNODE_MAX < Nnode_ error !!";
	  cout<<"\nN_PTNODE_MAX = "<<N_PTNODE_MAX<<" Nnode_ = "<<Nnode_; 
	  std::cin.get();exit(-1);  
  }

  float*  distance = new float  [Nnode_ + 1];   //Nnode =    TOTALREFNODE   by sec
  int*    parent   = new int  [Nnode_ + 1];    
  bool*   inQueue  = new bool [Nnode_ + 1];  

  if (start<TOTALREFNODE+1 || start>Nnode_)
  {
	  cout<<"\nstart node out of range!! start node (5)= "<<start;
	  std::cin.get(); exit(-1);
  }
  
  //const int size  = (const int)(1.5 * N_PTNode_);
  if (1.5 * N_PTNode_ > N_PTNODE_MAX)
	  cout<<"warning !! 1.5 * N_PTNode_ > N_PTNODE_MAX !!";

  Link*     pLink;
  Label*    elem ;  
  Label*    pLabel;
  Label*    LabelTable[N_PTNODE_MAX] ; 
  PQ        queue;

  int i, j, k, count=0, outlinkid=0, linktype=0,count_visited=0;
  float dist_j=0;
  
  for ( i=1; i<=Nnode_; i++) 
  {
    distance[i] = (float)MAXINT;
    parent[i] = -1;
    inQueue[i] = 0;
  }
  pLabel = new Label;
  pLabel->NodeId = start;
  pLabel->Cost = 0;//key
  distance[start] = 0;//dist from start node
  LabelTable[count++]=pLabel;
  queue.insert(pLabel);
  inQueue[start]=1;
  ++count_visited;
  while( !queue.is_empty()) 
  {
    elem = queue.extract_min();
	i = elem->NodeId;  

	if (count_visited == N_PTNode_)
	{		
		OutResTablePT<<start<<" ";
		 for (j = 1; j<=N_PTNode_; j++)
		 {
			 //OutResTablePT<<" to "<<j+TOTALREFNODE<<" ("<<NodeTable[j+TOTALREFNODE]->NodeIdLong
			//	 <<")"<<" res "<<typeRes<<" = "<<distance[j+TOTALREFNODE]<<endl;
			  OutResTablePT<<distance[j+TOTALREFNODE]<<" ";
			//ResTable[start][j][typeRes] = distance[j];
		 }
		 OutResTablePT<<endl;
		 for (k=0;k<count;k++)
			delete LabelTable[k];
		delete inQueue;  delete parent;  delete distance;
		return 1;
	}
	pLink = firstOutlink[i];
	
	while (pLink != 0 ) 
	{
	  if (pLink->LinkType==PT)
	  {
		j = pLink->toNode;//outgoing node
		dist_j = distance[i] + pLink->Resource[typeRes];
	    //cout<<"dist_j = "<<dist_j;
		if ( !inQueue[j] ) 
		{
			  ++count_visited;
			  inQueue[j] = 1;
			  distance[j] = dist_j;
			  parent[j] = i;
			  pLabel = new Label;
              pLabel->NodeId = j;
			  pLabel->Cost = dist_j;
			  LabelTable[count++]=pLabel;
			  queue.insert(pLabel);
			  //cout<<"newj = "<<j<<" "<<distance[j]<<" count_visited = "<<count_visited<<endl;
		  }//end if
		  else
		  {
			 if (dist_j < distance[j] )//decrease key
			 {
				distance[j] = dist_j;				
				parent[j] = i;
				if ( ! queue.decrease_key(j,dist_j))
				{
				   //create new label
				   //cout<<"\n ! queue.decrease_key(j,cost_j) !!"<<endl;
			       pLabel = new Label;
                   pLabel->NodeId = j;
			       pLabel->Cost = dist_j;
				   LabelTable[count++]=pLabel;
			       queue.insert(pLabel);	
				   inQueue[j]=1;
				}//end if
			 }//end if
		   }//end else
		 pLink = pLink->nextLink;
	   }
	   else
		   pLink = pLink->nextLink;
	}//end while
  }//while
    
  //cout<<"\nshortest path from "<<start<<" to "<<end<<" = "<<"INF"<<endl;
  delete inQueue;
  delete parent;
  delete distance; 	
  for (k=0;k<count;k++)
	delete LabelTable[k];
  cout<<"\n Dijkstra_allnodePT error !! there are some nodes not-visited !!";
  return 0;
 
}
//based on Dijkstra
bool  Network::AnejaAlgo1_Dijkstra(const int& start,  const int& end, const int& Nnode, 
	                      const int& modeid)
{
	ofstream OutToFile("OutToFile.txt", ios_base::app);
	int  dist = 0;//compute the shortest path
	Link* pLink=0;
	//cout<<"\nAnejaAlgo1";
    if (check_res_shortest_Dijkstra(start,end, Nnode, modeid))//check if the resource constriants on the p* are satistied
	{		
		if (check_res(start))//check if res of shortest res path is greater than its limits 
		{
			//OutToFile<<"\nthere is no solution for the constrained shortest path!! constriant violation on the minimum resource path!!";
			//std::cin.get();
			OutToFile<<"\nINF";
			return 1;
		}
		else 
			return 0;
	}
	else
	{
		OutToFile<<"\n"<<shortest_dist;
		//OutToFile<<"\n resource constraints on  shortest paht are satisfied !!";
		//OutToFile<<"\n shortest dist = "<<shortest_dist<<endl;	
		//std::cin.get();
		return 1;
	}
}

//based on A*
bool  Network::AnejaAlgo1(const int& start,  const int& end, const int& Nnode, 
	                      const int& modeid)
{
	ofstream OutToFile("OutToFile.txt", ios_base::app);
	int  dist = 0;//compute the shortest path
	Link* pLink=0;
	//cout<<"\nAnejaAlgo1";
    if (check_res_shortest(start,end, Nnode, modeid))//check if the resource constriants on the p* are satistied
	{		
		if (check_res(start))//check if res of shortest res path is greater than its limits 
		{
			//OutToFile<<"\nthere is no solution for the constrained shortest path!! constriant violation on the minimum resource path!!";
			//std::cin.get();
			OutToFile<<"\nINF";
			return 1;
		}
		else 
			return 0;
	}
	else
	{
		OutToFile<<"\n"<<shortest_dist;
		//OutToFile<<"\n resource constraints on  shortest paht are satisfied !!";
		//OutToFile<<"\n shortest dist = "<<shortest_dist<<endl;	
		//std::cin.get();
		return 1;
	}
}
//startnode = 25, target = 932 , initState = FOOT,
bool  Network::AnejaAlgo1(const int& start,  const int& end, const int& Nnode, 
	                      const int& modeid, const float& t_dep)
{
	ofstream OutToFile("OutToFile.txt", ios_base::app);
	int  dist = 0;//compute the shortest path
	Link* pLink=0;
	
	//cout<<"\ncheck_res_shortest !!";
    if (check_res_shortest(start, end, Nnode, modeid, t_dep))//check if the resource constriants on the p* are satistied
	{		
		//cout<<"\ncheck_res !!";
		if (check_res(start))//check if res of shortest res path is greater than its limits 
		{
			//cout<<"\nthere is no solution for the constrained shortest path!! constriant violation on the minimum resource path!!";
			//std::cin.get();
			OutToFile<<"\nINF";
			return 1;
		}	
		else 
			return 0;
	}
	else
	{
		OutToFile<<"\n"<<shortest_dist;
		OutToFile<<"\n resource constraints on shortest paht are satisfied !!";
		OutToFile<<"\n shortest dist = "<<shortest_dist<<endl;
		return 1;
	}
}

float Network::getttToTarget(const int& nodeid,  const int& nodeState)
{
	float temp=0;
	switch (nodeState)
	{
	case CAR:
		return distToTarget[nodeid]*1000.0/CARSPEED;
		break;
	case FOOT:
		return distToTarget[nodeid]*1000.0/WALKSPEED;
		break;
	case PT:
		temp = getRes(nodeid, 0) + ttTargetToNearestS ;//dist from current node to nearest station plus walking time to target node
		return temp;
		break;
	default:
		cout<<"\nnodeState error in  getttToTarget!!";
	    std::cin.get();
		exit(-1);	
	}
}

//if return 1 means res bound is depassed
//startnode = 25, target = 932 , initState = FOOT
bool  Network::check_res_shortest(const int& start,  const int& target,const int& Nnode, 
	                              int state,  const float& t_dep)//resource bound
{
  ofstream OutToFile("OutToFile.txt", ios_base::app);
  int i, j, k, ss, currstate = state, newstate=0;
  int end = target;
  bool isboarding=0;
  float t_arr = 0;
  
  if (start == end)
	  return 0;

  const int size = TOTALREFNODE+1;
  float*    distance = new float [Nnode+1];   
  float*    cost     = new float [Nnode+1];  
  int*      parent   = new int   [Nnode+1];    
  bool*     inQueue  = new bool  [Nnode+1];  
  float     sum_res[TOTALRESOURCE];  
  float**   res_predlink = new float* [Nnode+1];
  for (i=0;i<Nnode+1;i++)
  {
	  res_predlink[i] = new float [TOTALRESOURCE]; 
	  for (j=0;j<TOTALRESOURCE;j++)
		  res_predlink[i][j]=0;
  }

  for (i=0;i<TOTALRESOURCE;i++)
	  sum_res[i]=0.0;

  if (start<1 || start>TOTALREFNODE)
  {
	  cout<<"\nstart node out of range!! start node (6) = "<<start;
	  std::cin.get(); exit(-1);
  }
  if (end<1 || end>TOTALREFNODE)
  {
	  cout<<"\nend node out of range!! end node = "<<end;
	  std::cin.get(); exit(-1);
  }
  
  Link*     pLink;
  Label*    elem ;  
  Label*    pLabel;
  Label*    LabelTable[3*TOTALREFNODE+1]; 
  PQ        queue;

  int   count=0, outlinkid=0, linktype=0,countlink=0;
  float dist_j=0,cost_j=0;
  
  for ( i=1; i<=Nnode; i++) 
  {
    distance[i] = (float)MAXINT;
	cost[i] = (float)MAXINT;
    parent[i] = -1;
    inQueue[i] = 0;
  }

  pLabel = new Label;
  pLabel->NodeId = start;
  pLabel->state = currstate;
  pLabel->t_arr = t_dep;
  pLabel->Cost = getttToTarget(start,currstate);//key, travel time (by sec) to target node by mode currstate (foot or car)
  distance[start] = 0;//dist from start node
  cost[start] = pLabel->Cost;//key
  LabelTable[count++]=pLabel;
  queue.insert(pLabel);
  inQueue[start] = 1;
  for (ss=0;ss<TOTALRESOURCE;ss++)
     res_predlink[start][ss]=0;

  while( !queue.is_empty()) 
  {
    elem = queue.extract_min();
	i = elem->NodeId;  
	currstate = elem->state;
	t_arr = elem->t_arr;//t_arr is the time arriving at head of link

	if (i == end)
	{
		shortest_dist = distance[i];
		OutToFile<<"\ncheck_res_shortest : path = "<<end<<" ";
		for (ss=0;ss<TOTALRESOURCE;ss++)
			sum_res[ss] += res_predlink[end][ss];
		do {
			end = parent[end];
		    for (ss=0;ss<TOTALRESOURCE;ss++)
			{
			   sum_res[ss] += res_predlink[end][ss];
			  // cout<<"\nres_predlink["<<end<<"]["<<ss<<"] = "<<res_predlink[end][ss];
			}
			OutToFile<<end<<" ";
		}while (end != start);	
		for (ss=0;ss<TOTALRESOURCE;ss++)
		OutToFile<<"\nsum_res["<<ss<<"] = "<<sum_res[ss]; 
		
		for (ss=0;ss<TOTALRESOURCE;ss++)
		OutToFile<<"\nrBound["<<ss+1<<"] = "<<rBound[ss+1];
		
		 bool flag=0;
		 for (ss=0;ss<TOTALRESOURCE;ss++)
		 {
			 if ( sum_res[ss] > rBound[ss+1])
			 {
				 flag=1;
				 OutToFile<<"\n(res bounds are active)"<<endl;
				 OutToFile<<"start = "<<start<<" target = "<<target<<" shortest_dist = "<<shortest_dist<<endl;
				 for (ss=0;ss<TOTALRESOURCE;ss++)
					 OutToFile<<"used res["<<ss<<"] = "<<sum_res[ss]<<" rBound[ss+1]= "<<rBound[ss+1]<<endl;
				 break;
			 }		 
		 }		
		for (k=0;k<count;k++)
			delete LabelTable[k];
		delete inQueue;  delete parent;  
		delete distance;delete cost;
		for (k=0;k<Nnode+1;k++)
			delete res_predlink[k];
		delete res_predlink;
		return flag;
	}

	pLink = firstOutlink[i];
	while (pLink != 0) 
	{
		j = pLink->toNode;//outgoing node	
		if ( getFAT(currstate, pLink->LinkType) != -1 )
		{		
			isboarding =  pLink->boardingArc;
			//t_arr is the time arriving at head of link, currstate is the mode at head of link  
			float t_temp = getTraveltime(t_arr,j,isboarding,pLink,currstate);
			dist_j = distance[i] + t_temp;
			newstate = getFAT(currstate, pLink->LinkType);
			cost_j = dist_j + getttToTarget(j,newstate);//be careful here
			if ( !inQueue[j] ) 
			{
			  inQueue[j] = 1;
			  distance[j] = dist_j;
			  cost[j] = cost_j;
			  parent[j] = i;	
			  for (ss=0;ss<TOTALRESOURCE;ss++)
				  res_predlink[j][ss] = pLink->Resource[ss+1];
			  pLabel = new Label;
              pLabel->NodeId = j;
			  pLabel->t_arr = t_arr + t_temp;	  
			  pLabel->Cost = cost_j;
			  pLabel->state = newstate;
			  LabelTable[count++]=pLabel;
			  queue.insert(pLabel);
			}//end if
			else
			{
				if (cost_j < cost[j] )//decrease key
				{
					distance[j] = dist_j;
					cost[j] = cost_j;
					parent[j] = i;
					for (ss=0;ss<TOTALRESOURCE;ss++)
						res_predlink[j][ss]=pLink->Resource[ss+1];
					if ( ! queue.decrease_key(j, cost_j, t_arr + t_temp))
					{
						//create new label
						pLabel = new Label;
						pLabel->NodeId = j;
					    pLabel->t_arr =  t_arr + t_temp;			 
						pLabel->Cost = cost_j;
						pLabel->state = newstate;
						LabelTable[count++]=pLabel;
						queue.insert(pLabel);	
						inQueue[j]=1;
					}//end if
				}//end if
			}//end else
			pLink = pLink->nextLink;
		}//end if  getFAT(currstate, pLink->LinkType) != -1
		else
			pLink = pLink->nextLink;
	}//end while
  }//end while !queue.is_empty()
  
  cout<<"\nshortest path from "<<start<<" to "<<end<<" = "<<"INF"<<endl;
  delete inQueue;
  delete parent;
  delete cost;
  delete distance; 
  for (k=0;k<count;k++)
	delete LabelTable[k];
  
  for (k=0;k<Nnode+1;k++)
	  delete res_predlink[k];
  
  delete res_predlink;
  return 0;
}

//static version based on Dijkstra
bool  Network::check_res_shortest_Dijkstra(const int& start,  const int& target,const int& Nnode, int state)//resource bound
{
	
  ofstream OutToFile("OutToFile.txt", ios_base::app);
  int i, j, k,ss, currstate = state, newstate=0 ;
  
  int end = target;
  if (start == end)
	  return 0;

  const int size = TOTALREFNODE+1;
  float*    distance = new float [Nnode+1];   //Nnode =    TOTALREFNODE   by sec
  float*    cost     = new float [Nnode+1];   //Nnode =    TOTALREFNODE   
  int*      parent   = new int   [Nnode+1];    
  bool*     inQueue  = new bool  [Nnode+1];  
  float     sum_res[TOTALRESOURCE];  
  float     res_predlink[size][TOTALRESOURCE];

  for (i=0;i<TOTALRESOURCE;i++)
	  sum_res[i]=0.0;

  Link*     pLink;
  Label*    elem ;  
  Label*    pLabel;
  Label*    LabelTable[3*TOTALREFNODE+1] ; 
  PQ        queue;

  int   count=0, outlinkid=0, linktype=0,countlink=0;
  float dist_j=0,cost_j=0;
  
  for ( i=1; i<=Nnode; i++) 
  {
    distance[i] = (float)MAXINT;
	cost[i] = (float)MAXINT;
    parent[i] = -1;
    inQueue[i] = 0;
  }
  pLabel = new Label;
  pLabel->NodeId = start;
  pLabel->state = currstate;
  pLabel->Cost = 0;//key
 
  distance[start] = 0;//dist from start node
  cost[start] = pLabel->Cost;//key
  LabelTable[count++]=pLabel;
  queue.insert(pLabel);
  inQueue[start] = 1;
  for (ss=0;ss<TOTALRESOURCE;ss++)
     res_predlink[start][ss]=0;

  while( !queue.is_empty()) 
  {
    elem = queue.extract_min();
	i = elem->NodeId;  
	currstate = elem->state;
	if (i == end)
	{
		shortest_dist = distance[i];
		
		for (ss=0;ss<TOTALRESOURCE;ss++)
			sum_res[ss] += res_predlink[end][ss];

		do {
			end = parent[end];
		    for (ss=0;ss<TOTALRESOURCE;ss++)
			   sum_res[ss] += res_predlink[end][ss];
			//OutToFile<<end<<" ";
		}while (end != start);	

		bool flag=0;
		 for (ss=0;ss<TOTALRESOURCE;ss++)
		 {
			 if ( sum_res[ss] > rBound[ss+1])
			 {
				 flag=1;
				 /*cout<<"\n(res bounds are active)"<<endl;
				 cout<<"start = "<<start<<" target = "<<target<<" shortest_dist = "<<shortest_dist<<endl;
				 for (ss=0;ss<TOTALRESOURCE;ss++)
				 {
					 cout<<"used res["<<ss<<"] = "<<sum_res[ss]<<" rBound[ss+1]= "<<rBound[ss+1]<<endl;
				 }*/
				 break;
			 }		 
		 }
		
		for (k=0;k<count;k++)
			delete LabelTable[k];
		delete inQueue;  delete parent;  
		delete distance;delete cost;
		return flag;
	}

	pLink = firstOutlink[i];
	while (pLink != 0) 
	{
		j = pLink->toNode;//outgoing node			
		if ( getFAT(currstate, pLink->LinkType) != -1 )
		{
			dist_j = distance[i] + (float)pLink->getTT(currstate);////OK
			newstate = getFAT(currstate, pLink->LinkType);//OK
			cost_j = dist_j ;
			
			if ( !inQueue[j] ) 
			{
			  inQueue[j] = 1;
			  distance[j] = dist_j;
			  cost[j] = cost_j;
			  parent[j] = i;	
			  for (ss=0;ss<TOTALRESOURCE;ss++)
				  res_predlink[j][ss]=pLink->Resource[ss+1];
			  pLabel = new Label;
              pLabel->NodeId = j;
			  pLabel->Cost = cost_j;
			  pLabel->state = newstate;
			  LabelTable[count++]=pLabel;
			  queue.insert(pLabel);
			}//end if
			else
			{
				if (cost_j < cost[j] )//decrease key
				{
					distance[j] = dist_j;
					cost[j] = cost_j;
					parent[j] = i;
					for (ss=0;ss<TOTALRESOURCE;ss++)
						res_predlink[j][ss]=pLink->Resource[ss+1];
					if ( ! queue.decrease_key(j,cost_j))
					{
						//create new label
						//cout<<"\n ! queue.decrease_key(j,cost_j) !!"<<endl;
						pLabel = new Label;
						pLabel->NodeId = j;
						pLabel->Cost = cost_j;
						pLabel->state = newstate;
						LabelTable[count++]=pLabel;
						queue.insert(pLabel);	
						inQueue[j]=1;
					}//end if
				}//end if
			}//end else
			pLink = pLink->nextLink;
		}//end if  getFAT(currstate, pLink->LinkType) != -1
		else
			pLink = pLink->nextLink;
	}//end while
  }//while
    
  //cout<<"\nshortest path from "<<start<<" to "<<end<<" = "<<"INF"<<endl;
  delete inQueue;
  delete parent;
  delete cost;
  delete distance; 
  for (k=0;k<count;k++)
	delete LabelTable[k];
  return 0;
}

//static version based on A*
bool  Network::check_res_shortest(const int& start,  const int& target,const int& Nnode, int state)//resource bound
{
	
  ofstream OutToFile("OutToFile.txt", ios_base::app);
  int i, j, k,ss, currstate = state, newstate=0 ;
  
  int end = target;
  if (start == end)
	  return 0;

  const int size = TOTALREFNODE+1;
  float*    distance = new float [Nnode+1];   //Nnode =    TOTALREFNODE   by sec
  float*    cost     = new float [Nnode+1];   //Nnode =    TOTALREFNODE   
  int*      parent   = new int   [Nnode+1];    
  bool*     inQueue  = new bool  [Nnode+1];  
  float     sum_res[TOTALRESOURCE];  
  float     res_predlink[size][TOTALRESOURCE];

  for (i=0;i<TOTALRESOURCE;i++)
	  sum_res[i]=0.0;

  Link*     pLink;
  Label*    elem ;  
  Label*    pLabel;
  Label*    LabelTable[3*TOTALREFNODE+1] ; 
  PQ        queue;

  int   count=0, outlinkid=0, linktype=0,countlink=0;
  float dist_j=0,cost_j=0;
  
  for ( i=1; i<=Nnode; i++) 
  {
    distance[i] = (float)MAXINT;
	cost[i] = (float)MAXINT;
    parent[i] = -1;
    inQueue[i] = 0;
  }
  pLabel = new Label;
  pLabel->NodeId = start;
  pLabel->state = currstate;
  pLabel->Cost = getttToTarget(start,currstate);//key
 
  distance[start] = 0;//dist from start node
  cost[start] = pLabel->Cost;//key
  LabelTable[count++]=pLabel;
  queue.insert(pLabel);
  inQueue[start] = 1;
  for (ss=0;ss<TOTALRESOURCE;ss++)
     res_predlink[start][ss]=0;

  while( !queue.is_empty()) 
  {
    elem = queue.extract_min();
	i = elem->NodeId;  
	currstate = elem->state;
	if (i == end)
	{
		shortest_dist = distance[i];
		
		for (ss=0;ss<TOTALRESOURCE;ss++)
			sum_res[ss] += res_predlink[end][ss];

		do {
			end = parent[end];
		    for (ss=0;ss<TOTALRESOURCE;ss++)
			   sum_res[ss] += res_predlink[end][ss];
			//OutToFile<<end<<" ";
		}while (end != start);	

		bool flag=0;
		 for (ss=0;ss<TOTALRESOURCE;ss++)
		 {
			 if ( sum_res[ss] > rBound[ss+1])
			 {
				 flag=1;
				 /*cout<<"\n(res bounds are active)"<<endl;
				 cout<<"start = "<<start<<" target = "<<target<<" shortest_dist = "<<shortest_dist<<endl;
				 for (ss=0;ss<TOTALRESOURCE;ss++)
				 {
					 cout<<"used res["<<ss<<"] = "<<sum_res[ss]<<" rBound[ss+1]= "<<rBound[ss+1]<<endl;
				 }*/
				 break;
			 }		 
		 }
		
		for (k=0;k<count;k++)
			delete LabelTable[k];
		delete inQueue;  delete parent;  
		delete distance;delete cost;
		return flag;
	}

	pLink = firstOutlink[i];
	while (pLink != 0) 
	{
		j = pLink->toNode;//outgoing node			
		if ( getFAT(currstate, pLink->LinkType) != -1 )
		{
			dist_j = distance[i] + (float)pLink->getTT(currstate);////OK
			newstate = getFAT(currstate, pLink->LinkType);//OK
			cost_j = dist_j + getttToTarget(j,newstate);
			
			if ( !inQueue[j] ) 
			{
			  inQueue[j] = 1;
			  distance[j] = dist_j;
			  cost[j] = cost_j;
			  parent[j] = i;	
			  for (ss=0;ss<TOTALRESOURCE;ss++)
				  res_predlink[j][ss]=pLink->Resource[ss+1];
			  pLabel = new Label;
              pLabel->NodeId = j;
			  pLabel->Cost = cost_j;
			  pLabel->state = newstate;
			  LabelTable[count++]=pLabel;
			  queue.insert(pLabel);
			}//end if
			else
			{
				if (cost_j < cost[j] )//decrease key
				{
					distance[j] = dist_j;
					cost[j] = cost_j;
					parent[j] = i;
					for (ss=0;ss<TOTALRESOURCE;ss++)
						res_predlink[j][ss]=pLink->Resource[ss+1];
					if ( ! queue.decrease_key(j,cost_j))
					{
						//create new label
						//cout<<"\n ! queue.decrease_key(j,cost_j) !!"<<endl;
						pLabel = new Label;
						pLabel->NodeId = j;
						pLabel->Cost = cost_j;
						pLabel->state = newstate;
						LabelTable[count++]=pLabel;
						queue.insert(pLabel);	
						inQueue[j]=1;
					}//end if
				}//end if
			}//end else
			pLink = pLink->nextLink;
		}//end if  getFAT(currstate, pLink->LinkType) != -1
		else
			pLink = pLink->nextLink;
	}//end while
  }//while
    
  //cout<<"\nshortest path from "<<start<<" to "<<end<<" = "<<"INF"<<endl;
  delete inQueue;
  delete parent;
  delete cost;
  delete distance; 
  for (k=0;k<count;k++)
	delete LabelTable[k];
  return 0;
}
//Nnode is total nodes +1
void Network::initLabelTable(const int& Nnode)
{
	int N= int(10*Nnode +1);
	dominate_flag  = new bool [10*Nnode];
	LabelTable     = new Label* [N];//label attach to each node
	for (int i=1;i<=N;i++)
		LabelTable[i]=NULL;
}

bool Network::dominate(const int& nodeid, Label* p_label, Label* next)
{
	int i;
	int count =0;
	for (i=1;i<=TOTALRESOURCE;i++)
	{
		if (p_label->Resource[i] <=  next->Resource[i])
			++count;
	}
	if (count == TOTALRESOURCE)
		return 1;//dominate
	else
		return 0;
}

bool Network::beDominated(const int& nodeid, Label* p_label, Label* pred)
{
	int i;
	int count =0;
	for (i=1;i<=TOTALRESOURCE;i++)
	{
		if (p_label->Resource[i] >= pred->Resource[i])
			++count;
	}
	if (count == TOTALRESOURCE)
		return 1;//dominate
	else
		return 0;
}
void  Network::freeMemory(const int& Nnode)
{
	int nodeid=0, count=0;
	Label* next;
	Label* temp;

	for (nodeid=1;nodeid<=Nnode;nodeid++)
	{
		if (LabelTable[nodeid] !=0)
		{
			for (next = LabelTable[nodeid]; next != 0; )
			{
				temp = next;
				next = temp->NextLabel;
				delete temp;				
			}
		}
	}
}


bool Network::createLabel(const int& nodeid, Label* p_label, const int& Nnode)
{
	//for test
    /*if (nodeid<1 || nodeid >= Nnode)
	{
		cout<<"\nnodeid out of range !! nodeid= "<<nodeid<<endl;
		std::cin.get();exit(-1);
	}*/
	
	Label* next=0;
	Label* temp=0;
	Label* pred=0;

	if (LabelTable[nodeid] == 0)
	{
		LabelTable[nodeid] = p_label;
		return  1;
	}
	else
	{
		for (next = LabelTable[nodeid]; next != 0; next = next->NextLabel)
	    {
			//cout<<"\np_label->Cost = "<<p_label->Cost<<" next->Cost = "<<next->Cost;
		   if ( p_label->Cost < next->Cost )
		   {
		       //check dominace
			   if ( dominate(nodeid, p_label, next) )
			   {
				   dominate_flag[next->Id]=1;
				   temp = next->NextLabel;
				   p_label->NextLabel = temp;
				   if (pred !=0)
					   pred->NextLabel = p_label;				   
			   }
			   else
			   {
				   p_label->NextLabel = next;
				   if (pred !=0)
					   pred->NextLabel = p_label;
			   }
			   return 1;
		   }
		   pred = next;
		}

			if ( beDominated(nodeid, p_label, pred) )
				return 0;
			else
			{
				pred->NextLabel = p_label;
			    return 1;
			}		
	}//end else
}
//no violation return 1


/*
bool   Network::UBcheck(Label* p_label, const int& fromnode, const int& tonode, const int& index, const int& end,
	                    const float& UB)
{

	float temp= p_label->Cost + NodeTable[fromnode]->OutLinkTable[index]->Resource[0]
		      + ResTable[tonode][end][0];

	//UB is known current solution
	if ( temp <= UB)
		return 1;//OK
	else
		return 0;
	
}*/


bool Network::resCheck(Label* p_label, const Link* pLink)
{
	 ofstream OutToFile("OutToFile.txt", ios_base::app);
	int m, tonode=pLink->toNode ;
	float temp=0;
	/*for test
	if (p_label->NodeId == 515 && pLink->toNode == 932)
	{
	  OutToFile<<"\nresCheck call : ";
	  for (m=1;m<=TOTALRESOURCE;m++)
	  {
		OutToFile<<"\nm = "<<m<<" ( "<<p_label->Resource[m]<<", "<<pLink->Resource[m]<<", "<<ResTable[tonode][m]<<" )= ";
		temp = p_label->Resource[m] + pLink->Resource[m] 
		      + ResTable[tonode][m];
        OutToFile<<temp<<" rBound[m] = "<<rBound[m];
	  }
	}*/
	for (m=1;m<=TOTALRESOURCE;m++)
	{		
		temp = p_label->Resource[m] + pLink->Resource[m] 
		      + getRes(tonode, m);
	    //cout<<"\n"
		if (temp > rBound[m])
			return 0;//not ok
	}
	return 1;//ok 
}

void  Network::initHideFlag(const int& Nnode)
{
	NodeHide = new bool [Nnode+1];
	for (int i=1;i<Nnode;i++)
		NodeHide[i]=0;
}

bool Network::Dijkstra_AllNode(const int& start, const int& Nnode, const int& modeid, const int& resType)
{  
  //ofstream OutToFile("OutToFile.txt", ios_base::app);
  ofstream OutResTableRoad("OutResTableRoad.txt", ios_base::app);
  if (Nnode !=TOTALREFNODE)
  {
	  cout<<"\nNnode !=TOTALREFNODE error!!"<<endl;
	  std::cin.get();exit(-1);
  }

  float*  distance = new float  [Nnode+1];   //Nnode =    TOTALREFNODE   by sec
  int*    parent   = new int  [Nnode+1];    
  bool*   inQueue  = new bool [Nnode+1];  
  
  const int size  = (const int)(1.5*TOTALREFNODE);
  Link*     pLink;
  Label*    elem ;  
  Label*    pLabel;
  Label*    LabelTable[size] ; 
  PQ        queue;

  int i, j, k, count=0, outlinkid=0, linktype=0,count_visited=0;
  float dist_j=0;
  
  for ( i=1; i<=Nnode; i++) 
  {
    distance[i] = (float)MAXINT;
    parent[i] = -1;
    inQueue[i] = 0;
  }
  pLabel = new Label;
  pLabel->NodeId = start;
  pLabel->Cost = 0;//key
  distance[start] = 0;//dist from start node
  LabelTable[count++]=pLabel;
  queue.insert(pLabel);
  inQueue[start]=1;
  ++count_visited;
  while( !queue.is_empty()) 
  {
    elem = queue.extract_min();
	i = elem->NodeId;  
	pLink = firstOutlink[i];
	while (pLink != 0) 
	{
		j = pLink->toNode;//outgoing node
		dist_j = distance[i] + pLink->Resource[resType];
		if ( !inQueue[j] ) 
		  {
			  ++count_visited;
			  inQueue[j] = 1;
			  distance[j] = dist_j;
			  parent[j] = i;
			  pLabel = new Label;
              pLabel->NodeId = j;
			  pLabel->Cost = dist_j;
			  LabelTable[count++]=pLabel;
			  queue.insert(pLabel);
			  //cout<<"newj = "<<j<<" "<<distance[j]<<" count_visited = "<<count_visited<<endl;
		  }//end if
		  else
		  {
			 if (dist_j < distance[j] )//decrease key
			 {
				distance[j] = dist_j;				
				parent[j] = i;
				if ( ! queue.decrease_key(j,dist_j))
				{
				   //create new label
				   //cout<<"\n ! queue.decrease_key(j,cost_j) !!"<<endl;
			       pLabel = new Label;
                   pLabel->NodeId = j;
			       pLabel->Cost = dist_j;
				   LabelTable[count++]=pLabel;
			       queue.insert(pLabel);	
				   inQueue[j]=1;
				}//end if
			 }//end if
		   }//end else
		pLink = pLink->nextLink;
	}//end while
  }//while
    
  //cout<<"\nshortest path from "<<start<<" to "<<end<<" = "<<"INF"<<endl;

		//OutResTableRoad<<" start = "<<start<<" ";
		 for (j = 1; j<=Nnode; j++)
		 {
			//OutToFile<<j<<" "<<distance[j]<<endl;
			//ResTable[start][j][typeRes] = distance[j];
			 if (distance[j] <0 )
			 {
				 std::cout<<"\ndistance[j] <0 error !! ";
				 std::cin.get();exit(-1);
			 }
			 else
			 {
				 //cout<<"\ndistance[j] = "<<distance[j];
			 	OutResTableRoad<<distance[j]<<" ";
			 }
		 }
		 //OutResTableRoad<<endl;
		 for (k=0;k<count;k++)
			delete LabelTable[k];
		delete inQueue;  delete parent;  delete distance;
		
	
  return 1;
}

void   Network::testSetRes(const int& Nnode)
{
	
   ofstream OutToFile("OutToFile.txt", ios_base::app);
	int i,j;
	OutToFile<<"\nTestSetRes test!!"<<endl;
	for (i=1;i<=Nnode;i++)
	{
		OutToFile<<"\nstart node = "<<i;
		OutToFile<<" ( ";
		for (j=1;j<=TOTALRESOURCE;j++)
			OutToFile<<ResTable[i][j]<<" ";
		OutToFile<<" )";
	}
}


void Network::to_edge_array(edge_pair e[], int Nnode)
{
	int i,m;			/* counters */
	Link*  pLink;			/* temporary pointer */

	m = 0;
        for (i=1; i<=Nnode; i++)
		{
			pLink = firstOutlink[i];
			while (pLink != 0)
			{
				if (pLink->toNode > i)
				{
					e[m].x = i;
					e[m].y = pLink->toNode;
					e[m].weight = pLink->tt;
					m = m+1;
				}
				pLink = pLink->nextLink;
			}
		}
}


void Network::kruskal(int Nnode,int Nlink)
{
    ofstream OutMinSpanTree("OutMinSpanTree.txt", ios_base::app);


	int i,j, totalLink=Nlink*2;				
	Link* pLink;

	edge_pair** edgeTable = new edge_pair* [totalLink];		
	for (i=0;i<totalLink;i++)
		edgeTable[i] = new edge_pair;

	myset_union* s = new myset_union;		
	myset_union_init(s, Nnode);

	//sort link length
	int* indexVec  =  new int    [totalLink];
	double* costVec=  new double [totalLink];
	int count=0;
	
	for (i=1;i<=Nnode;i++)
	{	
		pLink= firstOutlink[i];
		while (pLink != 0)
		{
			j = pLink->toNode;
			edgeTable[count]->x = i;
			edgeTable[count]->y = j;
			edgeTable[count]->weight = pLink->tt ;
			//OutToFile<<"count (1)= "<<count<<" "<<i<<" "<<j<<" "<<edgeTable[count]->weight<<endl;
		    indexVec[count]=count;
		    costVec[count] = pLink->tt;
			++count;
			edgeTable[count]->x = j;
			edgeTable[count]->y = i;
			edgeTable[count]->weight = pLink->tt ;
			//OutToFile<<"count (2)= "<<count<<" "<<j<<" "<<i<<" "<<edgeTable[count]->weight<<endl;
		    indexVec[count]=count;
		    costVec[count] = pLink->tt;
			++count;
			
			pLink = pLink->nextLink;
		}		
	}
	//cout<<"\ncount = "<<count<<" totalLink= "<<totalLink<<endl;
	quickSort(costVec, indexVec, 0, totalLink-1);
	//quicksort test
	/*OutToFile<<"\n*****quickSort test***\n";
	for (i=0;i<Nlink;i++)
		OutToFile<<"edgeid = "<<indexVec[i]<<" cost = "<<costVec[indexVec[i]]<<endl;
	*/
	int fromnode=0, tonode=0;
    float sum=0, w=0;


	for (i=0; i<Nlink; i++) 
	{
		fromnode= edgeTable[indexVec[i]]->x;
		tonode= edgeTable[indexVec[i]]->y;
		w = edgeTable[indexVec[i]]->weight;
		if ( !same_component(s,fromnode,tonode) )
		{		
			OutMinSpanTree<<fromnode<<" "<<tonode<<" "<<w<<endl;
			OutMinSpanTree<<tonode<<" "<<fromnode<<" "<<w<<endl;
			sum += w;
			myunion_sets(s,fromnode,tonode);
		}
	}

	delete indexVec; delete costVec;delete s;
	for (i=0;i<totalLink;i++)
		delete edgeTable[i];
	delete edgeTable;
}

void  Network::createPTLines()
{
	 ofstream OutToFile("OutToFile.txt", ios_base::app);
	
	 //  int in_modeA, in_modeB: arrays to store ref_nodeindex of stations of PT mode A and mode B  
	//createLine(10, 10, 1);//generate line data by creating 10 PT mode-A lines, each line containing 10-20 stops 
	//createLine(20, 20, 2);//generate line data by creating 20 PT mode-B lines, each line containing 20-30 stops
	//exit(-1);
	int i ;
	createMultilevelNet(10,10);//create 10 lines of mode A and 10 lines of mode B
	
	OutToFile<<"\nin_modeA[i]>0 list : ";
	for (i=1;i<=TOTALREFNODE;i++)
	{
		if (in_modeA[i]>0)
			OutToFile<<in_modeA[i]<<" ";
	}
	OutToFile<<"\nin_modeB[i]>0 list : ";
	for (i=1;i<=TOTALREFNODE;i++)
	{
		if (in_modeB[i]>0)
			OutToFile<<in_modeB[i]<<" ";
	}
}

/*
Remark:the number of lines is counted differently for each direction
*/
void  Network::createMultilevelNet(const int& n_line_mode1,const int&n_line_mode2)
{
    //  int in_modeA, in_modeB: arrays to store ref_node index of stations of PT mode A and mode B  
	ofstream OutToFile("OutToFile.txt", ios_base::app);
	int i=0,j=0,fromnode=0,tonode=0, modeid=0, lineid=0, n_stop=0, stop=0;
	float dist=0;

	//FILE* tf = fopen("data_OutLines_fred.txt", "r");//change
	//FILE* tf = fopen("data_OutLines_pren.txt", "r");//change
	//FILE* tf = fopen("data_OutLines_chicagoskt.txt", "r");//change
	//FILE* tf = fopen("data_OutLines_chicagoReg.txt", "r");//change
	//FILE* tf = fopen("data_OutLines_phil.txt", "r");//change
	FILE* tf = fopen("data_OutLines_lorraine.txt", "r");//change
	
	
	if (!tf)
	{
		cerr<<"\ncannot open input file for data_OutLines!!";
		exit(-1);
	}
	PTLink* pLink=0;
	Node* p_fnode=0; Node* p_tonode=0;Node* p_station=0;
	int fnode=0, tnode=0, snode=0, ref_fnode=0,ref_tnode=0, direction=0,id_long=0;
	int n_node_temp= Nnode_;
	/***********************
	create network of mode A
	***********************/
	//OutToFile<<"\n *****set_t_depVec test !! *****"<<endl;
	for (i=0;i<n_line_mode1;i++)
	{
		 //start a new line
		 fscanf (tf,"%d",&modeid);
		 fscanf (tf,"%d",&lineid);
		 fscanf (tf,"%d",&n_stop);
		 fscanf (tf,"%d",&direction);
		 n_node_temp += n_stop;
		 if (n_node_temp > N_NODE_MAX)
		 {
			 cout<<"\nn_node_temp > N_NODE_MAX error!! "<<" n_node_temp = "<<n_node_temp<<endl;
		     std::cin.get(); exit(-1);
		 }
		 //first node
		 p_fnode = new Node(++Nnode_);//p_fnode means pointer of fromnode
		 fnode = Nnode_;//fnode means fromnode
		 fscanf (tf,"%d",&ref_fnode);
		 p_fnode->state_ = PT;
		 NodeTable[fnode] = p_fnode;
		 p_fnode->set_t_depVec(T_STARTSERVICE, 1);
		 //OutToFile<<fnode<<" ";
	     p_fnode->NodeIdLong =  modeid*100000000+ lineid*1000000+ direction*100000 + ref_fnode;
		// p_fnode->disp();
		 //create station node
		 if (in_modeA[ref_fnode] == 0)
		 {		 
			//create station node
			 p_station = new Node(++Nnode_);
			 snode = Nnode_;			 
			 in_modeA[ref_fnode] = snode;	
			 //cout<<"\nin_modeA["<<ref_fnode<<"]= "<<in_modeA[ref_fnode]<<endl;
			 p_station->NodeIdLong = modeid*100000000 + ref_fnode;
		     p_station->x_ = NodeTable[ref_fnode]->x_;//set coords only for station nodes
		     p_station->y_ = NodeTable[ref_fnode]->y_;
			 p_station->state_= PT;
			 NodeTable[snode] = p_station;
			 ++NStation_;
			// p_station->disp();	
		 }
		 else
			 snode = in_modeA[ref_fnode];
		 //create boarding/alighting arcs, from linenode-> station and inverse
		   		 
			 //create boarding link
			 pLink = new PTLink(++Nlink_);
			 pLink->toNode = fnode;
			 pLink->tt = DIST_BOARDING*1000/WALKSPEED;
			 pLink->nextLink = firstOutlink[snode]; 
			 pLink->LinkType = PT;
			 pLink->boardingArc=1;
			 pLink->Resource[0] = pLink->tt ;
			 pLink->Resource[1] = DIST_BOARDING * 0.1;
		     pLink->Resource[2] = RN;		     
			 firstOutlink[snode] = pLink;
			 ++sizeOutlink[snode];
			// pLink->disp();

		 for (j=0;j<n_stop-1;j++)
		 {
			 //read dist of link
			 fscanf (tf,"%f",&dist);
			 //nextnode
			 p_tonode = new Node(++Nnode_);
			 tnode = Nnode_;
			 fscanf (tf,"%d",&ref_tnode);
			 p_tonode->state_ = PT;
			 p_tonode->set_t_depVec(T_STARTSERVICE, 1);
			// OutToFile<<tnode<<" ";
			 p_tonode->NodeIdLong =  modeid*100000000+ lineid*1000000+ direction*100000 + ref_tnode;
			 NodeTable[Nnode_] = p_tonode;
			 //p_tonode->disp();

			 //create station node
			 if (in_modeA[ref_tnode] == 0)
		     {	
				 //create station node
				 p_station = new Node(++Nnode_);
				 snode = Nnode_;
				 in_modeA[ref_tnode] = snode;	
				 //cout<<"\nin_modeA["<<ref_tnode<<"]= "<<in_modeA[ref_tnode]<<endl;
				 p_station->NodeIdLong = modeid*100000000 + ref_tnode;
				 p_station->x_ = NodeTable[ref_tnode]->x_;//set coords only for station nodes
				 p_station->y_ = NodeTable[ref_tnode]->y_;
				 p_station->state_= PT;
				 NodeTable[snode] = p_station;
				  ++NStation_;
				// p_station->disp();
			 }		
			 else
				 snode = in_modeA[ref_tnode];

			 //create line link
			 pLink = new PTLink(++Nlink_);
			 pLink->toNode = tnode;
			 pLink->tt = dist*1000.0/METROSPEED;
			 pLink->nextLink = firstOutlink[fnode];  
			 pLink->LinkType = PT;
			 pLink->Resource[0] = pLink->tt ;
			 pLink->Resource[1] = dist * 0.1;
		     pLink->Resource[2] = RN;		     
			 firstOutlink[fnode] = pLink;
			 ++sizeOutlink[fnode];
			
			 //create  alighting links
			 pLink = new PTLink(++Nlink_);
			 pLink->toNode = snode;
			 pLink->tt = DIST_BOARDING*1000.0/WALKSPEED;
			 pLink->nextLink = firstOutlink[tnode]; 
			 pLink->LinkType = PT;			 
			 pLink->Resource[0] = pLink->tt ;
			 pLink->Resource[1] = DIST_BOARDING * 0.1;
		     pLink->Resource[2] = RN;		     
			 firstOutlink[tnode] = pLink;
			 ++sizeOutlink[tnode];
			 //test
			// pLink->disp();
			 //create boarding link
			 if (j != n_stop-2)
			 {
				 pLink = new PTLink(++Nlink_);
				 pLink->toNode = tnode;
				 pLink->tt = DIST_BOARDING*1000.0/WALKSPEED;
				 pLink->nextLink = firstOutlink[snode]; 
				 pLink->LinkType = PT;
				 pLink->boardingArc=1;
			     pLink->Resource[0] = pLink->tt ;
				 pLink->Resource[1] = DIST_BOARDING * 0.1;
				 pLink->Resource[2] = RN;	
				 firstOutlink[snode] = pLink;
				 ++sizeOutlink[snode];
				 //pLink->disp();
			 }
			 fnode= tnode;
		 }//end j
	}//end i
    /***********************
	create network of mode B
	***********************/
	for (i=0;i<n_line_mode2;i++)
	{
		 //start a new line
		 fscanf (tf,"%d",&modeid);
		 fscanf (tf,"%d",&lineid);
		 fscanf (tf,"%d",&n_stop);
		 fscanf (tf,"%d",&direction);
		 //first node
		 p_fnode = new Node(++Nnode_);
		 fnode = Nnode_;
		 fscanf (tf,"%d",&ref_fnode);
		 p_fnode->state_ = PT;
		 NodeTable[fnode] = p_fnode;
		 p_fnode->set_t_depVec(T_STARTSERVICE, 2);
		// OutToFile<<fnode<<" ";
	     p_fnode->NodeIdLong =  modeid*100000000+ lineid*1000000+ direction*100000 + ref_fnode;
		 //p_fnode->disp();
		 //create station node
		 if (in_modeB[ref_fnode] == 0)
		 {		 
			//create station node
			 p_station = new Node(++Nnode_);
			 snode = Nnode_;
			 in_modeB[ref_fnode] = snode;	
			  //cout<<"\nin_modeB["<<ref_fnode<<"]= "<<in_modeB[ref_fnode]<<endl;
			 p_station->NodeIdLong = modeid*100000000 + ref_fnode;
		     p_station->x_ = NodeTable[ref_fnode]->x_;//set coords only for station nodes
		     p_station->y_ = NodeTable[ref_fnode]->y_;
			 p_station->state_= PT;
			 NodeTable[snode] = p_station;
			  ++NStation_;
			 //p_station->disp();	
		 }
		 else
			 snode = in_modeB[ref_fnode];   		 
			 //create boarding link
			 pLink = new PTLink(++Nlink_);
			 pLink->toNode = fnode;
			 pLink->tt = DIST_BOARDING*1000.0/WALKSPEED;
			 pLink->nextLink = firstOutlink[snode]; 
			 pLink->LinkType = PT;
			 pLink->boardingArc=1;
			 pLink->Resource[0] = pLink->tt ;
			 pLink->Resource[1] = DIST_BOARDING * 0.1;
		     pLink->Resource[2] = RN;		     
			 firstOutlink[snode] = pLink;
			 ++sizeOutlink[snode];
			// pLink->disp();

		 for (j=0;j<n_stop-1;j++)
		 {
			 //read dist of link
			 fscanf (tf,"%f",&dist);
			 //nextnode
			 p_tonode = new Node(++Nnode_);
			 tnode = Nnode_;
			 fscanf (tf,"%d",&ref_tnode);
			 p_tonode->state_ = PT;
			 p_tonode->set_t_depVec(T_STARTSERVICE, 2);
			// OutToFile<<tnode<<" ";
			 p_tonode->NodeIdLong =  modeid*100000000+ lineid*1000000+ direction*100000 + ref_tnode;
			 NodeTable[tnode] = p_tonode;
			 //p_tonode->disp();

			 //create station node
			 if (in_modeB[ref_tnode] == 0)
		     {	
				 //create station node
				 p_station = new Node(++Nnode_);
				 snode = Nnode_;
				 in_modeB[ref_tnode] = snode;	
				 //cout<<"\nin_modeB["<<ref_tnode<<"]= "<<in_modeB[ref_tnode]<<endl;
				 p_station->NodeIdLong = modeid*100000000 + ref_tnode;
				 p_station->x_ = NodeTable[ref_tnode]->x_;//set coords only for station nodes
				 p_station->y_ = NodeTable[ref_tnode]->y_;
				 p_station->state_= PT;
				 NodeTable[snode] = p_station;
				  ++NStation_;
				// p_station->disp();
			 }		
			 else
				 snode = in_modeB[ref_tnode];

			 //create line link
			 pLink = new PTLink(++Nlink_);
			 pLink->toNode = tnode;
			 pLink->tt = dist*1000.0/BUSSPEED;
			 pLink->nextLink = firstOutlink[fnode];  
			 pLink->LinkType = PT;
			 pLink->Resource[0] = pLink->tt ;
			 pLink->Resource[1] = dist * 0.1;
		     pLink->Resource[2] = RN;		     
			 firstOutlink[fnode] = pLink;
			 ++sizeOutlink[fnode];
			
			 //create  alighting links
			 pLink = new PTLink(++Nlink_);
			 pLink->toNode = snode;
			 pLink->tt = DIST_BOARDING*1000.0/WALKSPEED;
			 pLink->nextLink = firstOutlink[tnode]; 
			 pLink->LinkType = PT;
			 pLink->Resource[0] = pLink->tt ;
			 pLink->Resource[1] = DIST_BOARDING * 0.1;
		     pLink->Resource[2] = RN;		     
			 firstOutlink[tnode] = pLink;
			 ++sizeOutlink[tnode];
			 //test
			// pLink->disp();
			 //create boarding link
			 if (j != n_stop-2)
			 {
				 pLink = new PTLink(++Nlink_);
				 pLink->toNode = tnode;
				 pLink->tt = DIST_BOARDING*1000.0/WALKSPEED;
				 pLink->nextLink = firstOutlink[snode]; 
				 pLink->LinkType = PT;
				 pLink->boardingArc=1;
			     pLink->Resource[0] = pLink->tt ;
				 pLink->Resource[1] = DIST_BOARDING * 0.1;
				 pLink->Resource[2] = RN;	
				 firstOutlink[snode] = pLink;
				 ++sizeOutlink[snode];
				 //pLink->disp();
			 }
			 fnode= tnode;
		 }//end j
	}//end i
	/*connect stations and road nodes*/
	TransferLink* pTrLink=0;
	int snode1=0,snode2=0;
	for (i=1;i<=TOTALREFNODE;i++)
	{
		if (in_modeA[i] > 0 )
		{
			//transfer link to road
			snode = in_modeA[i] ;
			pTrLink = new TransferLink(++Nlink_);
			pTrLink->toNode = i;
			pTrLink->tt = DIST_TRANSFERT*1000.0/WALKSPEED;
			pTrLink->nextLink = firstOutlink[snode]; 
			pTrLink->LinkType = TRANSFER;
			pTrLink->Resource[0] = pTrLink->tt ;
			pTrLink->Resource[1] = DIST_TRANSFERT * 0.1;
			pTrLink->Resource[2] = RN;	
			firstOutlink[snode] = pTrLink;
			++sizeOutlink[snode];
			//pTrLink->disp();
			//transfer link to station
			pTrLink = new TransferLink(++Nlink_);
			pTrLink->toNode = snode;
			pTrLink->tt = DIST_TRANSFERT*1000.0/WALKSPEED;//by sec
			pTrLink->nextLink = firstOutlink[i]; 
			pTrLink->LinkType = TRANSFER;
		    pTrLink->Resource[0] = pTrLink->tt ;
			pTrLink->Resource[1] = DIST_TRANSFERT * 0.1;
			pTrLink->Resource[2] = RN;	
			firstOutlink[i] = pTrLink;
			++sizeOutlink[i];
			//pTrLink->disp();		
		}
		if (in_modeB[i] > 0 )
		{
			//transfer link to road
			snode = in_modeB[i] ;
			pTrLink = new TransferLink(++Nlink_);
			pTrLink->toNode = i;
			pTrLink->tt = DIST_TRANSFERT*1000.0/WALKSPEED;
			pTrLink->nextLink = firstOutlink[snode]; 
			pTrLink->LinkType = TRANSFER;
		    pTrLink->Resource[0] = pTrLink->tt ;
			pTrLink->Resource[1] = DIST_TRANSFERT * 0.1;
			pTrLink->Resource[2] = RN;	
			firstOutlink[snode] = pTrLink;
			++sizeOutlink[snode];
			//pTrLink->disp();
			//transfer link to station
			pTrLink = new TransferLink(++Nlink_);
			pTrLink->toNode = snode;
			pTrLink->tt = DIST_TRANSFERT*1000.0/WALKSPEED;
			pTrLink->nextLink = firstOutlink[i]; 
			pTrLink->LinkType = TRANSFER;
			pTrLink->Resource[0] = pTrLink->tt ;
			pTrLink->Resource[1] = DIST_TRANSFERT * 0.1;
			pTrLink->Resource[2] = RN;	
			firstOutlink[i] = pTrLink;
			++sizeOutlink[i];
			//pTrLink->disp();		
		}

		if (in_modeA[i] > 0 && in_modeB[i] > 0)
		{
			//transfer link between PT modes 
			snode1 = in_modeA[i] ;
			snode2 = in_modeB[i] ;
			//cout<<"\ni = "<<" snode1 = "<<snode1<<" snode2= "<<snode2<<endl;
			pLink = new PTLink(++Nlink_);
			pLink->toNode = snode1;
			pLink->tt = DIST_TRANSFERT*1000.0/WALKSPEED;
			pLink->nextLink = firstOutlink[snode2]; 
			pLink->LinkType = PT;
			pLink->Resource[0] = pLink->tt;
			pLink->Resource[1] = DIST_TRANSFERT * 0.1;
			pLink->Resource[2] = RN;	
			firstOutlink[snode2] = pLink;
			++sizeOutlink[snode2];
			//pLink->disp();
			//transfer link to station
			pLink = new PTLink(++Nlink_);
			pLink->toNode = snode2;
			pLink->tt = DIST_TRANSFERT*1000.0/WALKSPEED;
			pLink->nextLink = firstOutlink[snode1]; 
			pLink->LinkType = PT;
			pLink->Resource[0] = pLink->tt;
			pLink->Resource[1] = DIST_TRANSFERT * 0.1;
			pLink->Resource[2] = RN;	
			firstOutlink[snode1]= pLink;
			++sizeOutlink[snode1];
			//pLink->disp();		
		}//end if 
	}//end node i 
	N_PTNode_ = Nnode_ - TOTALREFNODE;
	cout<<"\nNnode_= "<<Nnode_<<" TOTALREFNODE= "<<TOTALREFNODE<<" N_PTNode_= "<<N_PTNode_<<endl;

}


void  Network::createLine(int n_line_metro, int n_stops_leftbound, int modeid)
{
    
	ofstream OutLines("OutLines.txt", ios_base::app);
	int i=0,j=0,k=0,fromnode=0,tonode=0, count=0;
	Link* pLink;
	
	//generate PTline;
    int n_stop=0;
	int v,w, v_temp, flag=0, a1,a2, vec_size=0;
	float fcount=0, fsum=0, fdist=0;

    vector<int> temp_line;
	v = rand()%TOTALREFNODE+1;
	for (i=0;i<n_line_metro;i++)
	{		
		//create randomly a line and then randomly seclect a node in the existing lines and create the second lines
		if (i !=0)
		{
			v_temp = rand()%(temp_line.size());
			v= temp_line[v_temp];
		}
		temp_line.clear();
	    n_stop = randBetween(n_stops_leftbound, n_stops_leftbound+10);//n_stop is between bound and bound+10
		temp_line.push_back(v);//insert the first stop 
		count=1;
	    while (count < n_stop)
		{		
			if (sizeOutlink[v]>0)
			{
				w = rand()%sizeOutlink[v];//outlink size of node v 
				pLink = firstOutlink[v];
				j=0;
				while (pLink!=0 && j<w)
				{
					pLink = pLink->nextLink;
					++j;			
				}
				v=pLink->toNode;
				flag=0;
				for (j=0;j<(int)temp_line.size();j++)
				{
					if (temp_line[j]==v)
					{
						flag=1;
						break;
					}
				}
				if (flag==0)
				{
					temp_line.push_back(v);
					++count;				
				}
			}//end if
		}//end while
		
		//output forward direction

		OutLines<<modeid<<" "<<i+1<<" "<<n_stop<<" "<<1<<" ";//actually stop is n_stop+1, 
	    vec_size = temp_line.size();
		for (j=0;j<vec_size-1;j++)
		{
			++fcount;
			a1 = temp_line[j];a2 = temp_line[j+1];
			fdist = dist_xy(a1,a2);
			OutLines<<a1<<" "<<fdist<<" ";
			//OutLines<<a1<<" ";
			fsum += fdist;
		}
		OutLines<<a2<<endl;
		if (count != n_stop)
			cout<<"\nfcount ! = n_stop+1 error!!"<<fcount<<" "<<n_stop+1<<endl;
		
		//backward direction

		OutLines<<modeid<<" "<<i+1<<" "<<n_stop<<" "<<2<<" ";//actually stop is n_stop+1, forward direction: 0
		
		for (j=1;j<vec_size;j++)
		{
			++fcount;
			a1 = temp_line[vec_size-j];a2 = temp_line[vec_size-j-1];
			fdist = dist_xy(a1,a2);
			OutLines<<a1<<" "<<fdist<<" ";
			//OutLines<<a1<<" ";
			fsum += fdist;
		}
		OutLines<<a2<<endl;

	}//end for i
	temp_line.clear();
	OutLines<<"average train stop distance = "<<fsum/fcount<<endl;
}

float Network::dist_xy(const int& a1,const int& a2)
{
	float x,y;
	x = NodeTable[a1]->x_ - NodeTable[a2]->x_ ;
	y = NodeTable[a1]->y_ - NodeTable[a2]->y_ ;
	return sqrt(x*x +y*y)*dist_coor_converter;
	
}

void Network::setFAT()
{
	for (int i =0;i<TOTALSTATE;i++)
		for (int j =0;j<TOTALLINKTYPE;j++)
			NFATable[i][j]=-1;

	NFATable[FOOT][ROAD] = FOOT;
    NFATable[FOOT][TRANSFER] = PT;
	
	NFATable[PT][PT] = PT;
	NFATable[PT][TRANSFER] = FOOT;

	NFATable[CAR][ROAD] = CAR;
	NFATable[CAR][TRANSFER] = PT;

}
int Network::getFAT(const int& state, const int& linktype)
{
	return NFATable[state][linktype];
}

float Network::getTraveltime(const float& t_arr, const int& atnode, const bool& isBoardingArc,
	                          Link* pLink, const int& usedMode)//usedMode is the mode used for the link
							                                   //atnode is the tail of link 
{
    ofstream OutToFile("OutToFile.txt", ios_base::app);
	float t1=0,t2=0, temp=0;;
	if (isBoardingArc)
	{
		//OutToFile<<"\nisBoardingArc = "<<isBoardingArc<<" atnode = "<<atnode<<" t_depVec[0] = "<<NodeTable[atnode]->t_depVec[0];
		//OutToFile<<"\nisBoardingArc * NodeTable[atnode]->getWaitTime(t_arr) = "<<isBoardingArc * NodeTable[atnode]->getWaitTime(t_arr)<<" usedMode = "<<usedMode
			//<<" t_arr= "<<t_arr<<" pLink->getTT(usedMode, t_arr)= "<<pLink->getTT(usedMode, t_arr);
		temp = isBoardingArc * NodeTable[atnode]->getWaitTime(t_arr+T_BOARDING) + pLink->getTT(usedMode, t_arr);
		if ( temp < 0 )
		{
			cout<<"\nisBoardingArc = "<<isBoardingArc<<" atnode= "<<atnode<<" t_arr= "<<t_arr
				<<" waittime = "<<NodeTable[atnode]->getWaitTime(t_arr)<<" getTT = "<<pLink->getTT(usedMode, t_arr);
		    std::cin.get();
            exit(-1);		
		}
		else
	       return  temp;
	}
	else
	{
		//OutToFile<<"\npLink->getTT(usedMode, t_arr) = "<<" usedMode = "<<usedMode<<" t_arr= "<<t_arr
		//  <<" getTT(usedMode, t_arr) = "<<pLink->getTT(usedMode, t_arr);
		return pLink->getTT(usedMode, t_arr);		
	}

}

Link*   Network::getFirstOutlink(const int& anodeid)
{
	return firstOutlink[anodeid];

}



void Network::quickSort(double* rc, int* x, int low, int high)
{
   int  temp, left, right;
   double median;
	
	if (high > low)
	{
		left   = low;
		right  = high;
		median = rc[x[low]];
		
		while (right > left)
		{
			while ( rc[x[left]] < median )
				left++;
			
			while (rc[x[right]] > median)
				right--;
			
			if (left > right) break;
			
			temp		= x[left];
			x[left]  	= x[right];
			x[right] 	= temp;
		
			left++;
			right--;
		}
		 
		quickSort(rc, x, low, right);
		quickSort(rc, x, left,  high);
	}
}

