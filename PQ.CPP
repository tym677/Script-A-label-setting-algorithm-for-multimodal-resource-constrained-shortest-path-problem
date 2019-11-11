
#include "PQ.h"


PQ::PQ()
{
	n=0;
}

PQ::~PQ()
{
}


int PQ::pq_parent(int pos)
{
	if (pos == 1) 
		return -1;
	else
		return (int)(pos/2);		/* implicitly take floor(n/2) */
}

int PQ::pq_young_child(int pos)
{
	return (2 * pos);
}

void PQ::bubble_up(int pos)
{
	if (pq_parent(pos) == -1)	
		return;	/* at root of heap, no parent */
	
	if ( q[pq_parent(pos)]->Cost > q[pos]->Cost)
	{
		swap(pos, pq_parent(pos));
		bubble_up(pq_parent(pos));
	}
}

void PQ::bubble_down(int pos)
{
	int c;				/* child index */
	int i;				/* counter */
	int min_index;		        /* index of lightest child */

	c = pq_young_child(pos);
	min_index = pos;

	for (i=0; i<=1; i++) 
		if ((c+i) <= n)
		{
			if (q[min_index]->Cost > q[c+i]->Cost ) 
				min_index = c+i;
		}

        if (min_index != pos) 
		{
			swap(pos,min_index);
			bubble_down(min_index);
        }
}
int  PQ::size()
{
	return n;
}

bool PQ::decrease_key(int nodeid, float newkey)
{
	 ofstream OutToFile("OutToFile.txt", ios_base::app);
	int i, pos=0, pos_new=0,temp=0;
	//get pos of x
	for (i=1;i<=n;i++)
	{
		if ( q[i]->NodeId == nodeid )
		{
			pos=i;
			break;
		}
	}
	if (pos==0)
	{

	   return 0;
	}
	else
	{
	  q[pos]->Cost = newkey;
	
	  if (pos == 1)
		  return 1;
	
	  if ( q[pq_parent(pos)]->Cost > q[pos]->Cost)
	  {
		//cout<<" pq_parent(pos) = "<<pq_parent(pos)<<" q[pq_parent(pos)]->Cost= "<<q[pq_parent(pos)]->Cost;
		swap(pos, pq_parent(pos));
		bubble_up(pq_parent(pos));
	  }
	  return 1;
	}
}

bool PQ::decrease_key(const int& nodeid, const float& newkey, const float& t_arr)
{
	 ofstream OutToFile("OutToFile.txt", ios_base::app);
	int i, pos=0, pos_new=0,temp=0;
	//get pos of x
	for (i=1;i<=n;i++)
	{
		if ( q[i]->NodeId == nodeid )
		{
			pos=i;
			break;
		}
	}
	if (pos==0)
	{
	   return 0;
	}
	else
	{
	  q[pos]->Cost = newkey;
	  q[pos]->t_arr = t_arr;
	
	  if (pos == 1)
		  return 1;
	
	  if ( q[pq_parent(pos)]->Cost > q[pos]->Cost)
	  {
		//cout<<" pq_parent(pos) = "<<pq_parent(pos)<<" q[pq_parent(pos)]->Cost= "<<q[pq_parent(pos)]->Cost;
		swap(pos, pq_parent(pos));
		bubble_up(pq_parent(pos));
	  }
	  return 1;
	}
}

void PQ::insert(Label* x)
{
        if (n >= PQ_SIZE)
			cout<<"Warning: priority queue overflow (n >= PQ_SIZE) ";
        else 
		{
			n += 1;
			q[n] = x;
			bubble_up(n);
		}
}

void PQ::swap(int i, int j)
{
	Label* temp;			/* placeholder */

	temp = q[i];
	q[i] = q[j];
	q[j] = temp;
}


Label*  PQ::extract_min()
{
        Label* min;			/* minimum value */

        if (n <= 0) 
			cout<<"Warning: empty priority queue!!\n";
        else 
		{
			min = q[1];
			q[1] = q[n];
			n = n - 1;
			bubble_down(1);
        }
        return min;
}

bool PQ::is_empty()
{
        if (n <= 0) 
			return 1;
        else
			return 0;
}

void  PQ::print_pq()
{
        int i;                          /* counter */
		cout<<"\nprint_pq() : ";
        for (i=1; i<=n; i++)
			cout<<"\n("<<q[i]->Cost<<" "<<q[i]->Cost<<")";
}


