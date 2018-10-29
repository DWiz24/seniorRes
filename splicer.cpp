#include <bits/stdc++.h>

using namespace std;

struct Edge {
	int s;
	int d;
	int time;
	int dist;
	int cars;
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

int xmin=-74110987;
int ymin=40807900;
int xh=10000;
int yh=10000;

int inside(Node* n) {
	return n->x>=xmin && n->x<=xmin+xh && n->y>=ymin && n->y <=ymin+yh;
}

void dfs(int s) {
	if (gr[s].dijkD!=-1 && inside(gr+s)) {
		gr[s].dijkD=1;
		for (int i=0; i<gr[s].adj.size(); i++) {
			dfs(gr[s].adj[i].d);
		}
	}
}

void writeSubgraph() {
	for (int i=0; i<nodes; i++) {
		dijkD[i]=-1;
	}
	int target=-1;
	while (target==-1 || !inside(gr+target)) {
		target=rand()%nodes;
	}
	dfs(target);
	int p=1;
	FILE* cords=fopen("Sectc.co","w");
	int count=0;
	for (int i=0; i<nodes; i++) {
		if (dijkD!=-1 && inside(gr+i)) {
			count++;
		}
	}
	fprintf(cords,"p nodenumaa %d\n",count);
	for (int i=0; i<nodes; i++) {
		if (dijkD!=-1 && inside(gr+i)) {
			gr[i].dijkD=p;
			fprintf(cords,"v %d %d %d\n",p,gr[i].x,gr[i].y);
			p++;
			
		}
	}
	fclose(cords);
	FILE* df=fopen("Sectd.gr","w");
	FILE* tf=fopen("Sectt.gr","w");
	for (int i=0; i<nodes; i++) {
		if (dijkD!=-1 && inside(gr+i)) {
			for (int j=0; j<gr[i].adj.size(); j++) {
				int dn=(gr[i].adj[j].d);
				if (dijkD!=-1 && inside(gr+dn)) {
					fprintf(df,"a %lld %lld %d\n",gr[i].dijkD,gr[dn].dijkD,gr[i].adj[j].dist);
					fprintf(tf,"a %lld %lld %d\n",gr[i].dijkD,gr[dn].dijkD,gr[i].adj[j].time);
				}
			}
		}
	}
	fclose(df);
	fclose(tf);
	cout<<"Nodes written: "<<p-1<<endl;
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
	srand(time(0));
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
	
	writeSubgraph();
	
	fclose(cf);
	fclose(df);
	fclose(tf);
	
	return 0;
}
