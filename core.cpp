#include <bits/stdc++.h>

using namespace std;

struct Edge {
	int s;
	int d;
	int time;
	int dist;
};

struct Node {
	int id;
	vector<Edge> adj;
	int x;
	int y;
	long long dijkD;
};

Node* gr=0;
int nodes;

struct QE {
	void* val;
	long long t;
};

struct PQ {
	QE* a=0;
	int sz=0;
	int maxsz=-1;
};

void deletePQ(PQ& p) {
	free(p.a);
	p.sz=0;
	p.maxsz=-1;
}

void initPQ(PQ& p, int maxnum) {
	deletePQ(p);
	p.a=(QE*)malloc(sizeof(QE)*maxnum+1);
	p.maxsz=maxnum;
}

void swap(QE* a, int i, int k) {
	QE temp=a[i];
	a[i]=a[k];
	a[k]=temp;
}

void heapUp(PQ& p, int i) {
	while (i>1) {
		if (p.a[i].t<p.a[i/2].t) {
			swap(p.a,i,i/2);
			i/=2;
		} else {
			return;
		}
	}
}

void heapDown(PQ& p, int i) {
	while (i*2<=p.sz) {
		if (i*2+1<=p.sz && p.a[i*2+1].t<p.a[i].t && p.a[i*2+1].t<=p.a[i*2].t) {
			swap(p.a,i,i*2+1);
			i=i*2+1;
		} else if (p.a[i*2].t<p.a[i].t) {
			swap(p.a,i,i*2);
			i*=2;
		} else {
			return;
		}
	}
}

void printHeap(PQ& p) {
	cout<<"Heap contents: ";
	for (int i=1; i<=p.sz; i++) {
		cout<<((Node*)p.a[i].val)->id<<" ";
	}
	cout<<endl;
}

void push(PQ& p, long long t, void* v) {
	if (p.sz==p.maxsz) {
		cout<<"Error too many PQ elements\n";
		return;
	}
	p.a[p.sz+1].t=t;
	p.a[p.sz+1].val=v;
	p.sz++;
	heapUp(p,p.sz);
}

QE pop(PQ& p) {
	if (p.sz==0) {
		cout<<"Nothing in the pq\n";
	}
	QE ret=p.a[1];
	p.a[1]=p.a[p.sz];
	p.sz--;
	heapDown(p,1);
	
	return ret;
}

long long dijkstra(int start, int end) {
	for (int i=0; i<nodes; i++) {
		gr[i].dijkD=1L<<60;
	}
	PQ p;
	initPQ(p,nodes*4);
	push(p,0,gr+start);
	while (p.sz>0) {
		QE q=pop(p);
		//cout<<"p.sz="<<p.sz<<endl;
		//cout<<"t="<<q.t<<endl;
		Node* n=(Node*) q.val;
		//cout<<q.val<<endl;
		//cout<<n->id<<endl;
		//printHeap(p);
		if (q.t<=n->dijkD) {
			if (n->id==end) {
				deletePQ(p);
				return q.t;
			}
			for (int i=0; i<n->adj.size(); i++) {
				long long dt=q.t+n->adj[i].time;
				if (dt<gr[n->adj[i].d].dijkD) {
					gr[n->adj[i].d].dijkD=dt;
					push(p,dt,gr+n->adj[i].d);
					//cout<<"pushed "<<n->adj[i].d<<endl;
					//printHeap(p);
				}
			}
		}
	}
	return -1;
}

void readCoords(FILE* f) {
	char line[256];
	while (fgets(line,256,f)) {
		if (line[0]=='p') {
			nodes=atol(line+12);
			gr=(Node*)malloc(sizeof(Node)*nodes);
		} else if (line[0]=='v') {
			int x;
			int y;
			int id;
			sscanf(line,"v %d %d %d\n",&id,&x,&y);
			id--;
			gr[id].id=id;
			gr[id].x=x;
			gr[id].y=y;
		}
	}
}

void readDists(FILE* f) {
	char line[256];
	while (fgets(line,256,f)) {
		if (line[0]=='a') {
			int x;
			int y;
			int w;
			sscanf(line,"a %d %d %d\n",&x,&y,&w);
			x--;
			y--;
			int valid=1;
			for (int i=0; i<gr[x].adj.size(); i++) {
				if (gr[x].adj[i].d==y) {
					valid=0;
					//cout<<"dup edges\n";
					break;
				}
			}
			if (valid) {
				//cout<<"gud\n";
				Edge e;
				e.s=x;
				e.d=y;
				e.dist=w;
				gr[x].adj.push_back(e);
			}
		}
	}
}

void readTimes(FILE* f) {
	char line[256];
	while (fgets(line,256,f)) {
		if (line[0]=='a') {
			int x;
			int y;
			int w;
			sscanf(line,"a %d %d %d\n",&x,&y,&w);
			x--;
			y--;
			int done=0;
			for (int i=0; i<gr[x].adj.size(); i++) {
				if (gr[x].adj[i].d==y) {
					gr[x].adj[i].time=w;
					done=1;
					break;
				}
			}
			if (!done) {
				cout<<"mathching failed\n";
			}
		}
	}
}

int main(int argc, char** argv) {
	if (argc<2) {
		cout<<"Use args\n";
		return 1;
	}
	FILE* cf=fopen(argv[1],"r");
	int l=strlen(argv[1]);
	strcpy(argv[1]+(l-4),"d.gr");
	FILE* df=fopen(argv[1],"r");
	argv[1][l-4]='t';
	FILE* tf=fopen(argv[1],"r");
	if (cf==0 || tf==0 || df==0) {
		cout<<"error opening files\n";
		return 1;
	}
	readCoords(cf);
	readDists(df);
	//cout<<"hi"<<endl;
	readTimes(tf);
	cout<<"Distances from node 7:\n";
	for (int i=0; i<10; i++) {
		cout<<"Node "<<i<<" distance "<<dijkstra(7,i)<<endl;
	}
	long long sum=0;
	for (int i=0; i<nodes; i++) {
		sum+=dijkstra(7,i);
		if (i%1000==0) cout<<i<<endl;
	}
	cout<<"Average distance from node 7: "<<sum/(double)nodes<<endl;
	fclose(cf);
	fclose(df);
	fclose(tf);
	
	return 0;
}
