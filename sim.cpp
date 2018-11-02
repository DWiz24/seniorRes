#include "core.h"
//Maxcars noInfo AllInfo
/*8 15063
 *32 16059 17316
 *128 19545 23437
 *256 25287 32379
 *512 43575 51294
 *1024 83645 76916
 *2048 160495 133025
 */
using namespace std;

struct Car {
	int id;
	int start;
	int dest;
	int pos; //as of hitting the next intersection
	long long startTime;
	int pnode;
	int adjnum;
};

int MAXCARS=8;
PQ eventq;
int nextCarID=0;

long long travelTime(Edge& e) { //to be called after placing car on edge
		return (long long)(e.time*(1L+e.cars)/2);
}

void countCars() {
	
	int carcount=0;
	for (int i=0; i<nodes; i++) {
		for (int j=0; j<gr[i].adj.size(); j++) {
			carcount+=gr[i].adj[j].cars;
		}
	}
	cout<<carcount<<endl;
}

Car* initCar(long long time) {
	Car* c=(Car*)malloc(sizeof(Car));
	//cout<<c<<endl;
	c->id=nextCarID;
	nextCarID++;
	c->start=rand()%nodes;
	c->pos=c->start;
	c->dest=rand()%nodes;
	c->startTime=time;
	c->pnode=-1;
	c->adjnum=-1;
	push(eventq,time,c);
	return c;
}

void runSim(long long maxT) {
	initPQ(eventq,MAXCARS);
	for (int i=0; i<MAXCARS; i++) {
		initCar(0);
	}
	int trips=0;
	long long tt=0;
	while (eventq.sz>0) {
		//cout<<eventq.sz<<endl;
		QE q=pop(eventq);
		long long t=q.t;
		Car* c=(Car*)q.val;
		//cout <<"car id="<<c->id<<" pos="<<c->pos<<" t="<<t<<" dest="<<c->dest<<endl;
		if (c->pos==c->dest) {
			//cout<<c->id <<" reached Destination dt="<<t-c->startTime<<endl;
			//cout<<c->id<<" reached destination ["<<c->startTime<<","<<t<<"]"<<endl;
			//cout<<c->start<<" to "<<c->dest<<endl;
			//cout<<"dt="<<t-c->startTime<<endl;
			
			trips++;
			tt+=t-c->startTime;
			
			if (c->pnode!=-1) {
				gr[c->pnode].adj[c->adjnum].cars--;
			}
			free(c);
			initCar(t);
			//cout<<"finished init"<<endl;
		} else {
			dijkstra(c->dest,c->pos);
			long long best=1L<<61;
			int choice=-1;
			for (int i=0; i<gr[c->pos].adj.size(); i++) {
				long long v=gr[c->pos].adj[i].time + gr[gr[c->pos].adj[i].d].dijkD;
				if (v<best) {
					best=v;
					choice=i;
				}
			}
			if (c->pnode!=-1) {
				gr[c->pnode].adj[c->adjnum].cars--;
			}
			//cout<<"in"<<endl;
			c->pnode=c->pos;
			c->adjnum=choice;
			//cout<<choice<<endl;
			gr[c->pos].adj[choice].cars++;
			//cout<<"m1"<<endl;
			c->pos=gr[c->pos].adj[choice].d;
			//cout<<"out"<<endl;
			push(eventq,t+travelTime(gr[c->pos].adj[choice]),c);
			if(t>maxT) {
				break;
			}
		}
	}
	while(eventq.sz) {
		free(pop(eventq).val);
	}
	deletePQ(eventq);
	
	cout<<"Trips="<<trips<<endl;
	cout<<"Avg dt="<<tt/trips<<endl;
	
}
