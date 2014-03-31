/*
 * project.cpp
 *
 *  Created on: Apr 25, 2013
 *      Author: maciej
 */
#include<cstdio>
#include<iostream>
#include<algorithm>
 
#include<list>
using namespace std;
 
const int MAX_X=201;
const int MAX_Y=201;
const int MAX_K=1002;
 
 
typedef pair<int,int> Point;
typedef list<Point*> Points;
 
int pr=0;
Point directions[4] = {Point(0,-1),Point(0,1),Point(1,0),Point(-1,0)};
 
int paths[MAX_X][MAX_Y];
const int INF=1000000000;
int city[MAX_X][MAX_Y];
int used[MAX_X][MAX_Y];
int sets[MAX_X][MAX_Y];
 
list<int> setsh;
list<int> setsh2;
int agentsSt[MAX_K];
int agentsEnd[MAX_K];
list<int> res;
 
list<int> l;
list<int> l2;
 
list<int> pathl;
list<int> pathh;
list<int> pathc;
int X,Y;  //x - columns, y -rows
int K;
void printSolution();
bool checkOthers=false;
int bruteStCalls = 0;
const int BRUTE_LIMIT=300000000;
inline bool inRange(int x, int y){
	if(x<1||y<1)
		return false;
	if(x>X||y>Y)
		return false;
	return true;
}
inline bool samePoints(int p1, int q1){
	return p1==q1;
}
int metric(int x1,int y1,int q1, int q2){
	return abs(x1-q1)+abs(y1-q2);
}
bool move(int agent, int x, int y);
bool move2(int agent, int x, int y,bool recount);
int getVal(int x, int y){
	return x*10000+y;
}
int getX(int v){
	return v/10000;
}
int getY(int v){
	return v%10000;
}
void dfsclear(int agent,int p1, int p2){
	l.push_back(getVal(p1, p2));
	while(!l2.empty()){
		int i =l2.front();
		l2.pop_front();
		used[getX(i)][getY(i)]=0;
	}
	//used[p1][p2]=-1;
 
}
void dfs(int agent,int p1,int p2){
	l.push_back(getVal(p1, p2));
	int d=1;
 
	//printf("setup %d %d %d\n",agent,p->first, p->second);
	used[p1][p2]=0;
	while(!l.empty()){
		int i = l.front();
		l.pop_front();
		int x = getX(i);
		int y = getY(i);
		//printf("%d %d %d\n",x,y,d);
		if(!inRange(x,y))
			continue;
		//printf("%d %d %d\n",x,y,d);
		if(used[x][y]!=0)
			continue;
		//printf("%d %d %d\n",x,y,d);
		if(city[x][y]!=0&&x!=p1&&y!=p2)
			continue;
		//printf("%d %d %d ok\n",x,y,d);
		used[x][y]=d++;
		l2.push_back(getVal(x,y));
		l.push_back(getVal(x-1,y));
		l.push_back(getVal(x+1,y));
		l.push_back(getVal(x,y-1));
		l.push_back(getVal(x,y+1));
 
	}
	for(int i = 1; i<=Y; i++)
	{
		printf("\n");
		for(int j=1; j<=X; j++)
			printf("%d ",used[j][i]);
	}
	printf("\n");
 
	for(int i = 1; i<=Y; i++)
	{
		printf("\n");
		for(int j=1; j<=X; j++)
			printf("%d ",city[j][i]);
	}
	printf("\n");
}
int path(int xx, int yy, int agent){
	int x=xx;
	int y = yy;
 
 
	int endX=getX(agentsEnd[agent]);
	int endY=getY(agentsEnd[agent]);
	if(endX==x&&endY==y)return 0;
	if(!inRange(x,y))return INF;
	if(used[x][y]==-1||city[x][y]!=0)return INF;
 
 
	pathl.push_back(getVal(x,y));
	int d=0,i;
 
	//printf("\n%d %d %d %d \n",xx,yy,d,agent);
 
/*	for(int i = 1; i<=Y; i++)
	{
		printf("\n");
		for(int j=1; j<=X; j++)
			printf("%d ",used[j][i]);
	}
*/
	/*
	printf("\n");
 
	for(int i = 1; i<=Y; i++)
	{
		printf("\n");
		for(int j=1; j<=X; j++)
			printf("%d ",city[j][i]);
	}
*/
	bool found=false;
	while(!pathl.empty()&&!found){
		d++;
		while(!pathl.empty()){
			i=pathl.front();
			x=getX(i);
			y=getY(i);
			if(x==endX&&y==endY)
				found=true;
			if(inRange(x,y)&&used[x][y]==0&&city[x][y]==0){
				used[x][y]=d;
				pathh.push_back(i);
				pathc.push_back(getVal(x-1,y));
				pathc.push_back(getVal(x+1,y));
				pathc.push_back(getVal(x,y-1));
				pathc.push_back(getVal(x,y+1));
			}
			pathl.pop_front();
		}
		while(!pathc.empty()){
			pathl.push_back(pathc.front());
			pathc.pop_front();
		}
		if(found)
			pathl.clear();
	}
 
	/*printf("\n%d %d %d %d \n",xx,yy,d,agent);
 
	for(int i = 1; i<=Y; i++)
	{
		printf("\n");
		for(int j=1; j<=X; j++)
			printf("%d ",used[j][i]);
	}
	printf("\n");
 
	for(int i = 1; i<=Y; i++)
	{
		printf("\n");
		for(int j=1; j<=X; j++)
			printf("%d ",city[j][i]);
	}
 
	printf("\n");*/
	while(!pathh.empty()){
		int i =pathh.front();
		pathh.pop_front();
		used[getX(i)][getY(i)]=0;
	}
/*
	for(int i = 1; i<=Y; i++)
	{
		printf("\n");
		for(int j=1; j<=X; j++)
			printf("%d ",used[j][i]);
	}
	printf("\n");
*/
	if(!found)
		return INF;
	return d;
}
 
void clear(){
	while(!pathh.empty()){
		int i =pathh.front();
		pathh.pop_front();
		used[getX(i)][getY(i)]=0;
	}
}
void path2( int agent){
 
	int endX=getX(agentsEnd[agent]);
	int endY=getY(agentsEnd[agent]);
 
 
	pathl.push_back(getVal(endX,endY));
	int d=1,i;
	clear();
	//printf("\n%d %d %d %d \n",xx,yy,d,agent);
 
/*	for(int i = 1; i<=Y; i++)
	{
		printf("\n");
		for(int j=1; j<=X; j++)
			printf("%d ",used[j][i]);
	}
*/
	/*
	printf("\n");
 
	for(int i = 1; i<=Y; i++)
	{
		printf("\n");
		for(int j=1; j<=X; j++)
			printf("%d ",city[j][i]);
	}
*/
	int x,y;
	while(!pathl.empty()){
		d++;
		while(!pathl.empty()){
			i=pathl.front();
			x=getX(i);
			y=getY(i);
			if(inRange(x,y)&&used[x][y]==0&&(city[x][y]==0||abs(city[x][y])==agent)){
				used[x][y]=d;
				pathh.push_back(i);
				pathc.push_back(getVal(x-1,y));
				pathc.push_back(getVal(x+1,y));
				pathc.push_back(getVal(x,y-1));
				pathc.push_back(getVal(x,y+1));
			}
			pathl.pop_front();
		}
		while(!pathc.empty()){
			pathl.push_back(pathc.front());
			pathc.pop_front();
		}
	}
/*
	printf("\n%d %d \n",d,agent);
 
	for(int i = 1; i<=Y; i++)
	{
		printf("\n");
		for(int j=1; j<=X; j++)
			printf("%d ",used[j][i]);
	}
	printf("\n");
 
	for(int i = 1; i<=Y; i++)
	{
		printf("\n");
		for(int j=1; j<=X; j++)
			printf("%d ",city[j][i]);
	}
 
	printf("\n");
 
 
	for(int i = 1; i<=Y; i++)
	{
		printf("\n");
		for(int j=1; j<=X; j++)
			printf("%d ",used[j][i]);
	}
	printf("\n");
*/
 
}
bool checkIfOthers(int agent){
	bool isOk=true;
	int w,x,y;
	for(int i =agent+1; i<=K&&isOk; i++){
        if(sets[getX(agentsSt[i])][getY(agentsSt[i])]!=
           sets[getX(agentsEnd[i])][getY(agentsEnd[i])]
        )isOk=false;
        else
        if(sets[getX(agentsSt[i])][getY(agentsSt[i])]==0){
			int endX=getX(agentsEnd[i]);
			int endY=getY(agentsEnd[i]);
			setsh.push_back(getVal(endX,endY));
 
			while(!setsh.empty()){
				//printf("aa");
				w=setsh.front();
				setsh.pop_front();
				x=getX(w);
				y=getY(w);
		//		printf("sets : %d %d\n",x,y);
				if(inRange(x,y)&&sets[x][y]==0&&(city[x][y]==0||abs(city[x][y])>=i)){
					sets[x][y]=i;
					setsh2.push_back(w);
					setsh.push_back(getVal(x-1,y));
					setsh.push_back(getVal(x+1,y));
					setsh.push_back(getVal(x,y-1));
					setsh.push_back(getVal(x,y+1));
				}
			}
			if(sets[getX(agentsSt[i])][getY(agentsSt[i])]!=
			   sets[getX(agentsEnd[i])][getY(agentsEnd[i])]
			)
				isOk=false;
        }
	}
	/*printf("\nsets \n");
 
	for(int i = 1; i<=Y; i++)
	{
		printf("\n");
		for(int j=1; j<=X; j++){
			printf("%d ",sets[j][i]);
			//sets[j][i]=0;
		}
	}*/
	while(!setsh2.empty()){
		w=setsh2.front();
		setsh2.pop_front();
		x=getX(w);
		y=getY(w);
		sets[x][y]=0;
	}
 
	return isOk;
}
 
bool jump(int agent, int p1, int p2){
	//dfs(agent, getX(agentsEnd[agent]),getY(agentsEnd[agent]));
	//printf("u\n");
	if(checkOthers){
		if(!checkIfOthers(agent))
			return false;
	}
 
	int a=path(p1-1,p2,agent);
	int b=path(p1+1,p2,agent);
	int c=path(p1,p2-1,agent);
	int d=path(p1,p2+1,agent);
 
	int* t = new int[4];
	int* s = new int[4];
	int* e = new int[4];
	t[0]=a;
	s[0]=p1-1;
	e[0]=p2;
 
	t[1]=b;
	s[1]=p1+1;
	e[1]=p2;
 
	t[2]=c;
	s[2]=p1;
	e[2]=p2-1;
 
	t[3]=d;
	s[3]=p1;
	e[3]=p2+1;
 
	for(int i = 0; i<=3; i++)
		for(int j=i+1; j<=3; j++)
			if(t[i]>t[j]){
				swap(t[i],t[j]);
				swap(s[i],s[j]);
				swap(e[i],e[j]);
			}
	bool f = false;
 
	for(int ii =0; ii<=3&&f==false&&(ii==0||(rand()%INF)<=pr); ii++)
		if(t[ii]==INF)continue;
		else
			if(move(agent,s[ii],e[ii])==true)
				f=true;
 
 
	//printf("u\n");
	delete [] t;
	delete [] s;
	delete [] e;
	return f;
}
int getUsed(int x,int y){
	if(!inRange(x,y))
		return INF;
	if(used[x][y]<=0)return INF;
	return used[x][y];
}
bool jump2(int agent, int p1, int p2, bool countPath){
	//dfs(agent, getX(agentsEnd[agent]),getY(agentsEnd[agent]));
	//printf("u\n");
 
 
	if(countPath)
		path2(agent);
	int a=getUsed(p1-1,p2);
 
	int b=getUsed(p1+1,p2);
 
	int c=getUsed(p1,p2-1);
 
	int d=getUsed(p1,p2+1);
 
 
	int* t = new int[4];
	int* s = new int[4];
	int* e = new int[4];
	t[0]=a;
	s[0]=p1-1;
	e[0]=p2;
 
	t[1]=b;
	s[1]=p1+1;
	e[1]=p2;
 
	t[2]=c;
	s[2]=p1;
	e[2]=p2-1;
 
	t[3]=d;
	s[3]=p1;
	e[3]=p2+1;
	//dfsclear(agent, getX(agentsEnd[agent]),getY(agentsEnd[agent]));
	//printf("%d %d %d %d\n",a,b,c,d, agent);
	//printf("%d %d\n", p->first,p->second);
	for(int i = 0; i<=3; i++)
		for(int j=i+1; j<=3; j++)
			if(t[i]>t[j]){
				swap(t[i],t[j]);
				swap(s[i],s[j]);
				swap(e[i],e[j]);
			}
	bool f = false;
	/*
	if(move(agent,p->first-1,p->second)==true||
			move(agent,p->first,p->second-1)==true||
			move(agent,p->first+1, p->second)==true||
			move(agent,p->first,p->second+1)==true)
		return true;*/
 
 
	for(int ii =0; ii<=3&&f==false&&(ii==0||(rand()%INF)<=pr); ii++)
		if(t[ii]==INF)continue;
		else
		if(ii==0){
			if(move2(agent,s[ii],e[ii],false)==true)
				f=true;
		} else {
			if(move2(agent,s[ii],e[ii],true)==true)
				f=true;
		}
 
	//printf("u\n");
	delete [] t;
	delete [] s;
	delete [] e;
	return f;
}
/*
bool jump2(int agent, int p1, int p2){
	int a=metric(p1-1,p2, getX(agentsEnd[agent]),getY(agentsEnd[agent]));
	int b=metric(p1+1,p2, getX(agentsEnd[agent]),getY(agentsEnd[agent]));
	int c=metric(p1,p2-1, getX(agentsEnd[agent]),getY(agentsEnd[agent]));
	int d=metric(p1,p2+1, getX(agentsEnd[agent]),getY(agentsEnd[agent]));
 
	if(a<b&&c<d){
		if(move(agent,p1-1,p2)==true||
				move(agent,p1,p2-1)==true||
				move(agent,p1+1, p2)==true||
				move(agent,p1,p2+1)==true)
			return true;
	}   else if(a<b&&c==d){
		if(
		   move(agent,p1-1,p2)==true||
		   move(agent,p1,p2-1)==true||
		   move(agent,p1,p2+1)==true||
		   move(agent,p1+1,p2)==true
		   )
			return true;
	}  else if(a<b&&c>d){
		if(
		   move(agent,p1-1,p2)==true||
		   move(agent,p1,p2+1)==true||
		   move(agent,p1,p2-1)==true||
		   move(agent,p1+1,p2)==true
		   )
			return true;
	}  else if(a==b&&c<d){
		if(
		   move(agent,p1,p2-1)==true||
		   move(agent,p1-1,p2)==true||
		   move(agent,p1+1,p2)==true||
		   move(agent,p1,p2+1)==true
		   )
			return true;
	}  else if(a==b&&c>d){
		if(
		   move(agent,p1,p2+1)==true||
		   move(agent,p1-1,p2)==true||
		   move(agent,p1+1,p2)==true||
		   move(agent,p1,p2-1)==true
		   )
			return true;
	}  else if(a>b&&c<d){
		if(
		   move(agent,p1+1,p2)==true||
		   move(agent,p1,p2-1)==true||
		   move(agent,p1,p2+1)==true||
		   move(agent,p1-1,p2)==true
		   )
			return true;
	}  else if(a>b&&c==d){
		if(
		   move(agent,p1+1,p2)==true||
		   move(agent,p1,p2-1)==true||
		   move(agent,p1,p2+1)==true||
		   move(agent,p1-1,p2)==true
		   )
			return true;
	}  else if(a>b&&c>d){
		if(
		   move(agent,p1+1,p2)==true||
		   move(agent,p1,p2-1)==true||
		   move(agent,p1,p2+1)==true||
		   move(agent,p1-1,p2)==true
		   )
			return true;
	}
	return false;
}
*/
bool solve(int agent, int p);
bool solve2(int agent, int p,bool recount);
void printRes();
 
 
int main(){
	srand(time(NULL));
	scanf("%d%d",&X,&Y);
	scanf("%d",&K);
	int a,b,c,d;
	for(int i = 1; i<=K; i++){
		scanf("%d%d%d%d",&a,&b,&c,&d);
		agentsSt[i]= getVal(a,b);
		agentsEnd[i]= getVal(c,d);
	}
	for(int i=1; i<=K; i++)
	for(int j = i+1; j<=K; j++)
		if(abs(getX(agentsEnd[i])-getX(agentsSt[i]))+abs(getY(agentsEnd[i])-getY(agentsSt[i]))>
	abs(getX(agentsEnd[j])-getX(agentsSt[j]))+abs(getY(agentsEnd[j])-getY(agentsSt[j]))){
			swap(agentsSt[i],agentsSt[j]);
			swap(agentsEnd[i],agentsEnd[j]);
		}
 
/*
	for(int i=1; i<=K; i++)
	for(int j = i+1; j<=K; j++)
		if(getX(agentsEnd[i])>getX(agentsSt[j])||(getX(agentsEnd[i])==getX(agentsSt[j]&&getY(agentsEnd[i])>getY(agentsEnd[j])))){
			swap(agentsSt[i],agentsSt[j]);
			swap(agentsEnd[i],agentsEnd[j]);
		}
	*/
	for(int i = 1; i<=K; i++){
		city[getX(agentsSt[i])][getY(agentsSt[i])]=i;
		city[getX(agentsEnd[i])][getY(agentsEnd[i])]=-i;
	}
	pr=0;
	if(solve2(1,agentsSt[1],true)){
		printRes();
		return 0;
	}
	clear();
	pr=0;
	//printf("no straight path\n");
	if(solve(1,agentsSt[1])){
		printRes();
		return 0;
	}
	checkOthers=true;
	for(int i =1; i<=100; i++){
		pr=(INF/100)*i;
		//printf("probability %d\n",i);
		if(solve(1,agentsSt[1])){
			printRes();
			return 0;
		}
		//printf("Solution: %d\n",bruteStCalls);
		bruteStCalls=0;
	}
	pr=INF;
	if(solve(1,agentsSt[1])){
		printRes();
		return 0;
	}
	//printf("false\n");
	return 0;
}
 
bool move(int agent, int x, int y){
	if(inRange(x,y)){
		if(city[x][y]==0){
			city[x][y]=agent;
			used[x][y]=-1;
 
			if(solve(agent,getVal(x,y)))
				return true;
			city[x][y]=0;
			used[x][y]=0;
		} else if(city[x][y]==-agent){
			res.push_back(getVal(x,y));
			if(solve(agent+1,agentsSt[agent+1]))
				return true;
			res.pop_back();
		}
	}
	return false;
}
bool move2(int agent, int x, int y,bool recount){
	if(inRange(x,y)){
		if(city[x][y]==0){
			city[x][y]=agent;
			used[x][y]=-1;
 
			if(solve2(agent,getVal(x,y),recount))
				return true;
			city[x][y]=0;
			used[x][y]=0;
		} else if(city[x][y]==-agent){
			res.push_back(getVal(x,y));
			if(solve2(agent+1,agentsSt[agent+1],true))
				return true;
			res.pop_back();
		}
	}
	return false;
}
bool solve(int agent,int p){
	bruteStCalls++;
 
	//if(bruteStCalls%1000000==0)
	//	printf("%d\n",bruteStCalls);
	if(bruteStCalls>BRUTE_LIMIT)
		return false;
	if(agent>K){
		//printf("%d\n",bruteStCalls);
		//printSolution();
		return true;
	}
	res.push_back(p);
	if(jump(agent,getX(p),getY(p)))
		return true;
	res.pop_back();
	return false;
}
bool solve2(int agent,int p, bool recount){
	bruteStCalls++;
 
	//if(bruteStCalls%1000000==0)
	//	printf("%d\n",bruteStCalls);
	if(bruteStCalls>BRUTE_LIMIT)
		return false;
	if(agent>K){
		//printf("%d\n",bruteStCalls);
		//printSolution();
		return true;
	}
	res.push_back(p);
	if(jump2(agent,getX(p),getY(p),recount))
		return true;
	res.pop_back();
	return false;
}
void printSolution(){
	printf("Solution: %d\n",bruteStCalls);
	for(int i = 1; i<=Y; i++){
		for(int j=1; j<=X; j++)
			printf("%d  ",city[j][i]);
		printf("\n");
	}
}
void printRes(){
	int agent=1;
	int end;
	int p;
	int q;
	printf("%d\n",res.size()-K);
	while(agent<=K){
		end = agentsEnd[agent];
		p = *res.begin();
		res.pop_front();
		do{
			q = *res.begin();
			res.pop_front();
			printf("%d %d %d %d\n",getX(p),getY(p),getX(q)-getX(p),getY(q)-getY(p));
			p=q;
		}while(!samePoints(p,end));
		agent++;
	}
}
