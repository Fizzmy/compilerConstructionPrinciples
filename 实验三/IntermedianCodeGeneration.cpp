#include<cstdio>
#include<iostream>
#include<vector>
#include<cstring>
#include<algorithm> 
#include<map>
#include<set>
#define mp(a,b) make_pair(a,b)
using namespace std;
char s[100],c;
int len=0;
vector<int>proClass[200];
vector<char>pro[110];
char proLeft[110];
int numpro=0;
int rt=1,tr[1100][73],sz=1,tag[110];
const int ALLKEY=22;
//0-4 keyword,5 constant,6 val,7-17 operation,18-20 breakword,21 epsilon
char *key[]={"if","else","while","int","float","digits","id","==",">=","<=","!=",">","<","=","+","-","*","/",";","(",")","^"};
vector< pair<int,int> >I[110];
int tf(char s)
{
	if (s>='A'&&s<='Z') return s-'A';
	else if (s>='a'&&s<='z') return s-'a'+26;
	else if (s>='0'&&s<='9') return s-'0'+52;
	else if (s=='=') return 62;
	else if (s=='>') return 63;
	else if (s=='<') return 64;
	else if (s=='+') return 65;
	else if (s=='-') return 66;
	else if (s=='*') return 67;
	else if (s=='/') return 68;
	else if (s==';') return 69;
	else if (s=='(') return 70;
	else if (s==')') return 71;
	else if (s=='^') return 72;
}
char t[110];

char s1[1010][1010];
int n,m,sum,cnt,N;
char S[100010];
pair<int,int>pos[100010];
bool wrong[100010];
struct token{
	int l,r;
	int type;
	int val;
}ans[1010];
struct symbol{
	int l,r,val;
	int type,addr;
	symbol(){
		val=type=addr=-1;
	}
}sans[1010];
int total=0,stotal=0;

int insert(char *s,int iskey,int l=0,int r=0,bool isNum=0)
{
	int n=strlen(s);
	if (n==0) return 0;
	int x=rt;
	for (int i=0;i<n;i++)
	{
		if (!tr[x][tf(s[i])])
			tr[x][tf(s[i])]=++sz,tag[sz]=-100;
		x=tr[x][tf(s[i])];
	}
	if (iskey>=0) tag[x]=-iskey;
	else
	{
		if (tag[x]==-100)
		{ 
			tag[x]=++stotal,sans[stotal].l=l,sans[stotal].r=r;
			if (isNum)
			{
				int x=0;
				for (int i=0;i<n;i++) x=x*10+s[i]-'0';
				sans[stotal].val=x;sans[stotal].type=0;
			}
			else sans[stotal].val=-1;
		}
	}
	return tag[x];
}
void addkeyword()
{
	tag[rt]=-100;
	for (int i=0;i<ALLKEY;i++)
		insert(key[i],i);
}
void add(char *s,int l,int r)
{
	if (l==r && s[l]>='A'&&s[r]<='Z')
		pro[numpro].push_back(s[l]);
	else
	{
		int nw=0;
		for (int i=l;i<=r;i++)
			t[nw++]=s[i];
		t[nw]=0;
		int pos=-insert(t,-1);
		if (pos!=ALLKEY-1) pro[numpro].push_back(pos);
		
	}
}
void getProduct(char *s,int len)
{
	numpro++;
	proClass[s[0]].push_back(numpro);
	proLeft[numpro]=s[0];
	int lst=2;
	for (int i=2;i<len;i++)
		if (s[i]==' ')
			add(s,lst,i-1),lst=i+1;
	add(s,lst,len-1);
}
void read()
{
	freopen("newproduct.txt","r",stdin);
	addkeyword();
	while (1)
	{
		c=getchar();
		s[len++]=c;
		if (c=='\n' || c==EOF)
		{
			len--;
			s[len]=0;
			getProduct(s,len);
			len=0;
		}
		if (c==EOF) break;
	}
}
int numC=0;
bool emp[300];bool vis[300];
bool isEmpty(char s)
{
	if (s>='A' && s<='Z')
	{
		vis[s]=1;
		for (auto x:proClass[s])
		{
			if (pro[x].size()==0) {vis[s]=0;return emp[s]=1;}
			bool can=1;
			for (auto y:pro[x])
				if (!vis[y] && !isEmpty(y))
				{
					can=0;
					break;
				}
			if (can) {vis[s]=0;return emp[s]=1;}
		}
		vis[s]=0;
		return emp[s]=0;
	}
	return 0;
}
void getEmpty()
{
	for (int i='A';i<='Z';i++) isEmpty(i);
}
int first[300],follow[300];
int findFirst(char s)
{
	vis[s]=1;
	for (auto x:proClass[s])
	{
		bool can=1;
		for (auto y:pro[x])
		{
			if (y<ALLKEY) first[s]|=((1<<y)&((1<<21)-1));
			else if (!vis[y]) first[s]|=(findFirst(y)&((1<<21)-1));
			if (!emp[y]) {can=0;break;}
		}
		if (can) first[s]|=(1<<21);
	}
	vis[s]=0;
	return first[s];
}
void getFirst()
{
	for (int i=0;i<ALLKEY;i++)
		first[i]=1<<i;
	for (int i='A';i<='Z';i++)
	{
		findFirst(i);
//		if (first[i]!=0)
//		{
//			printf("FIRST %c: ",i);
//			for (int j=0;j<ALLKEY;j++)
//				if ((first[i]>>j)&1) printf("%s ",key[j]);
//			printf("\n");
//		}
	}
}
int findFollow(char s)
{
	vis[s]=1;
	for (int i=1;i<=numpro;i++)
	{
		for (int j=0;j<pro[i].size();j++)
			if (pro[i][j]==s)
			{
				bool can=1;
				for (int k=j+1;k<pro[i].size();k++)
				{
					follow[s]|=(first[pro[i][k]]&((1<<21)-1));
					if (!emp[pro[i][k]]) {can=0;break;}
				}
				if (can && !vis[proLeft[i]])
					follow[s]|=findFollow(proLeft[i]);
			}
	}
	vis[s]=0;
	return follow[s]; 
}
void getFollow()
{
	follow['Z']=(1<<21);
	for (int i='A';i<='Z';i++)
	{
		findFollow(i);
//		if (first[i]!=0)
//		{
//			printf("FOLLOW %c: ",i);
//			for (int j=0;j<ALLKEY;j++)
//				if ((follow[i]>>j)&1) printf("%s ",key[j]);
//			printf("\n");
//		}
	}
}
struct ACTION{
	bool rs;
	int nxt;
	ACTION(bool _=0,int __=-1){rs=_,nxt=__;}
}action[60][30];
int gt[60][30];
void print(int id,pair<int,int> x)
{
	if (pro[x.first].size()==x.second)
	{
		int nfollow=follow[proLeft[x.first]];
		for (int j=0;j<ALLKEY;j++)
			if ((nfollow>>j)&1) 
			{
				if (action[id][j].nxt==-1 || j!=1)//j=1 else
					action[id][j].nxt=x.first,action[id][j].rs=1;
					//移入规约冲突，这里只有if else那里不归约，其他情况都选择归约 
			}
	}
//	printf("%c->",proLeft[x.first]);
//	for (int j=0;j<x.second;j++)
//	{
//		int nw=pro[x.first][j];
//		
//		if (nw<ALLKEY) printf("%s",key[nw]);
//		else printf("%c",nw);
//	}
//	printf(".");
//	for (int j=x.second;j<pro[x.first].size();j++)
//	{
//		int nw=pro[x.first][j];
//		if (nw<ALLKEY) printf("%s",key[nw]);
//		else printf("%c",nw);
//	}
//	//printf("%d %d\n",x.first,x.second);
//	printf("\n"); 
}

void dfs(int nxt,int k,vector< pair<int,int> > &nw)
{
	if (vis[nxt]) return;
	vis[nxt]=1;
	for (auto y:proClass[nxt])
	{
		if (pro[y].size()&&pro[y][0]==k)
			nw.push_back(mp(y,1));
		if (pro[y].size())
		{
			char x=pro[y][0];
			if (x>='A'&&x<='Z')
				dfs(x,k,nw);
		}
	}
}
void dfs(int id,int nxt)
{
	if (vis[nxt]) return;
	vis[nxt]=1;
	for (auto y:proClass[nxt])
	{
		print(id,mp(y,0));
		if (pro[y].size())
		{	
			char x=pro[y][0];
			if (x>='A'&&x<='Z')
				dfs(id,x);
		}
	}
}

void getClousure()
{
	I[numC++].push_back(mp(1,0));
	int lst=0;
	while (numC!=lst)
	{
		int nc=numC;
		for (int i=lst;i<nc;i++)
		{
			for (int k=0;k<ALLKEY;k++)
			{
				vector< pair<int,int> > nw; 
				for (auto x:I[i])
				{
					if (pro[x.first].size()==x.second) continue;
					char nxt=pro[x.first][x.second];
					if (nxt>='A'&&nxt<='Z')
						dfs(nxt,k,nw);	
					if (nxt==k) nw.push_back(mp(x.first,x.second+1));	
				}
				for (int y='A';y<='Z';y++) vis[y]=0;
				if (nw.size()==0) continue;
				sort(nw.begin(),nw.end());
				auto iter = unique(nw.begin(),nw.end());
				nw.erase(iter,nw.end());
				bool can=1;int target=0;
				for (int q=0;q<numC;q++)
					if (I[q]==nw) {can=0;target=q;break;}
				if (can) I[numC++]=nw,target=numC-1;
				action[i][k].nxt=target;
				action[i][k].rs=0;
			}
			for (int k='A';k<='Z';k++)
			{
				vector< pair<int,int> > nw; 
				for (auto x:I[i])
				{
					if (pro[x.first].size()==x.second) continue;
					char nxt=pro[x.first][x.second];
					if (nxt>='A'&&nxt<='Z')
						dfs(nxt,k,nw);
					
					if (nxt==k) nw.push_back(mp(x.first,x.second+1));
				}
				for (int y='A';y<='Z';y++) vis[y]=0;
				gt[i][k-'A']=-1;
				if (nw.size()==0) continue;
				sort(nw.begin(),nw.end());
				auto iter = unique(nw.begin(),nw.end());
				nw.erase(iter,nw.end());
				bool can=1;int target=0;
				for (int q=0;q<numC;q++)
					if (I[q]==nw) {can=0;target=q;break;}
				if (can) I[numC++]=nw,target=numC-1;
				gt[i][k-'A']=target;
			}
		}
		lst=nc;
		
	}
//	freopen("newstatus.txt","w",stdout);
	for (int i=0;i<numC;i++)
	{
//		printf("------------STATUS %d --------------\n",i);
		for (auto x:I[i])
			print(i,x);
		for (auto x:I[i])
		{
			//cout<<pro[x.first].size()<<" "<<x.second<<endl;
			if (pro[x.first].size()==x.second) continue;
			char nxt=pro[x.first][x.second];
			//cout<<nxt<<endl;
			if (nxt>='A'&&nxt<='Z') 
				dfs(i,nxt);
		}
		for (int y='A';y<='Z';y++) vis[y]=0;
	}
//	freopen("newactiongoto.txt","w",stdout);
//	printf("%7s","");
//	for (int i=0;i<ALLKEY-1;i++)
//		printf("%7s",key[i]);printf("%7s","$");
//	for (int i='A';i<='Z';i++) if (proClass[i].size()) printf("%7c",i);printf("\n");
//	for (int i=0;i<numC;i++)
//	{
//		printf("%7d",i);
//		for (int j=0;j<ALLKEY;j++)
//			if (action[i][j].nxt!=-1)
//				printf("%6d%c",action[i][j].nxt,(action[i][j].rs?'r':'s'));
//			else printf("%7s","");
//		for (int j=0;j<26;j++)
//			if (proClass[j+'A'].size()) if (gt[i][j]!=-1) printf("%7d",gt[i][j]); else printf("%7s","");
//		printf("\n");
//	}
}
#define BEGIN 0
#define VAL 1
#define NUM 2
#define OP 3
#define BK 4
#define KEYWORD 5

void readCode()
{
	freopen("code.txt","r",stdin);
	sum=0;
	while ((c=getchar())!=EOF)//按照行读取文件里的代码 
	{
		s1[sum][cnt++]=c;
		if (c=='\n') s1[sum][cnt]='\0',cnt=0,sum++;
	}
}
void modifyCode()//删除注释 
{
	int can=0;
	for (int i=0;i<=sum;i++)
	{
		int len=strlen(s1[i]);
		for (int j=0;j<len;j++)
		{
			if (can&&j+1<len&&s1[i][j]=='*'&&s1[i][j+1]=='/') {j++;can=0;continue;}	
			if (can) continue;
			if (j+1<len&&s1[i][j]=='/'&&s1[i][j+1]=='/') break;
			if (j+1<len&&s1[i][j]=='/'&&s1[i][j+1]=='*') {j++;can=1;continue;}
			pos[N]=mp(i+1,j+1),S[N++]=s1[i][j];
		}
	}
}
void lexicalAnalysis()
{
	int status=BEGIN;
	int pre=0;
	for (int i=0;i<=N;i++)
	{
		if (status==BEGIN)
		{
			pre=i;
			if (S[i]>='0'&&S[i]<='9')
				status=NUM;
			else if ((S[i]>='a'&&S[i]<='z') || (S[i]>='A'&&S[i]<='Z'))
				status=VAL;
			else if (S[i]=='+'||S[i]=='-'||S[i]=='*'||S[i]=='/'||S[i]=='>'||S[i]=='<'||S[i]=='='||S[i]=='!')
				status=OP;
			else if (S[i]=='('||S[i]==')'||S[i]==';'||S[i]=='\'')
				status=BK;
		}
		else if (status==VAL)
		{
			if ((S[i]>='0'&&S[i]<='9')||(S[i]>='a'&&S[i]<='z') || (S[i]>='A'&&S[i]<='Z'))
				status=VAL;
			else
			{
				i--;
				total++;
				ans[total].l=pre;ans[total].r=i;
				int nw=0;
				for (int j=pre;j<=i;j++)
					t[nw++]=S[j];
				t[nw]=0;
				int id=insert(t,-1,pre,i);
				if (id<=0)
					ans[total].type=KEYWORD,ans[total].val=-id;
				else ans[total].type=VAL,ans[total].val=id;
				status=BEGIN;
			}
		}
		else if (status==NUM)
		{
			if (S[i]>='0'&&S[i]<='9')
				status=NUM;
			else if ((S[i]>='a'&&S[i]<='z') || (S[i]>='A'&&S[i]<='Z'))
			{
				wrong[i]=1;
				printf("Compile Error on line %d at column %d : invalid constant\n",pos[i].first,pos[i].second);
				status=NUM;
			}
			else
			{
				i--;
				int nw=0;
				for (int j=pre;j<=i;j++)
					t[nw++]=S[j];
				t[nw]=0;
				int id=insert(t,-1,pre,i,1);
				total++;
				ans[total].l=pre;ans[total].r=i;ans[total].type=NUM;ans[total].val=id;
				status=BEGIN;
			}
		}
		else if (status==OP)
		{
			if ((S[i-1]=='='&&S[i]=='=') ||  (S[i-1]=='>'&&S[i]=='=') || (S[i-1]=='<'&&S[i]=='=') || (S[i-1]=='!'&&S[i]=='='))
			{
				total++;
				int nw=0;
				for (int j=pre;j<=i;j++)
					t[nw++]=S[j];
				t[nw]=0;
				int id=insert(t,-1,pre,i);
				ans[total].l=pre;ans[total].r=i;ans[total].type=OP;ans[total].val=-id;
				status=BEGIN;
			}
			else if (S[i]=='+'||S[i]=='-'||S[i]=='*'||S[i]=='/'||S[i]=='>'||S[i]=='<'||S[i]=='='||S[i]=='!')
			{
				wrong[i]=1;
				printf("Compile Error on line %d at column %d : invalid operator\n",pos[i].first,pos[i].second);
				status=OP;
			}
			else
			{
				i--;
				total++;
				int nw=0;
				for (int j=pre;j<=i;j++)
					t[nw++]=S[j];
				t[nw]=0;
				int id=insert(t,-1,pre,i);
				ans[total].l=pre;ans[total].r=i;ans[total].type=OP;ans[total].val=-id;
				status=BEGIN;
			}
		}
		else if (status==BK)
		{
			i--;
			total++;
			int nw=0;
				for (int j=pre;j<=i;j++)
					t[nw++]=S[j];
				t[nw]=0;
				int id=insert(t,-1,pre,i);
			ans[total].l=pre;ans[total].r=i;ans[total].type=BK;ans[total].val=-id;
			status=BEGIN;
		}
	}
}
void LexicalAnalysis()
{
	readCode();
	modifyCode();
	lexicalAnalysis(); 
}
bool ERROR=0;
int st[1100],tp=0,optp=0;
map<string,int> opst[1100];
int offset=0; 
int typeT[1100],tnum=0,codeNum=0;
vector<pair<char,bool> >code[1100];
set<int>setList[1100];
int slot[1100],setNum=0;
//acc status 5 
void printStack()
{
//	printf("\n------------------NOW STACK------------\n");
//	for (int i=1;i<=tp;i++) printf("%d ",st[i]);printf("\n");
//	printf("\n-------------OPERATION STACK-----------\n");
//	for (int i=1;i<=optp;i++)
//	{
//		//printf("%d\n",opst[i]["name"]);
//		if (opst[i]["name"]>='A') printf("%c",opst[i]["name"]);else printf("%s",key[opst[i]["name"]]);
//	}printf("\n");
}
void walkg(char s,map<string,int> mp)
{
	int nw=st[tp];
	if (gt[nw][s-'A']!=-1)
	{
		opst[++optp].clear();
		opst[optp].insert(mp.begin(),mp.end());
		opst[optp]["name"]=s; 
		st[++tp]=gt[nw][s-'A'];
		printStack();
	}
	else {printf("ERROR on goto!\n");ERROR=1;return;}
}
int getType(int x)//获取变量类型 
{
	if (x>=0) return sans[x].type;
	else return typeT[-x];
}
int widen(int a,int x,int y)//类型转换 
{
	if (x==-1||y==-1) printf("Undefined value!\n");
	if (x==y) return a;
	if (x==0&&y==1)
	{
		--tnum;
		typeT[-tnum]=1; 
		codeNum++; 
		code[codeNum].push_back(mp(tnum,1));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp('=',0));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp('(',0));
		code[codeNum].push_back(mp('f',0));
		code[codeNum].push_back(mp('l',0));
		code[codeNum].push_back(mp('o',0));
		code[codeNum].push_back(mp('a',0));
		code[codeNum].push_back(mp('t',0));
		code[codeNum].push_back(mp(')',0));
		code[codeNum].push_back(mp(a,1));
		return tnum;
	}
	else {printf("WARING: expect float , find int\n");}
}
void backpatch(int id,int x)//在id这个set中回填地址x 
{
	for (auto q:setList[id])
		slot[q]=x;
}
int merge(int id1,int id2)//合并两个set并返回set对应序号 
{
	setNum++;
	for (auto q:setList[id1])
		setList[setNum].insert(q);
	for (auto q:setList[id2])
		setList[setNum].insert(q);
	return setNum;
} 
void printPro(int x)//具体翻译方案 
{
	map<string,int> nw;
	//声明语句翻译 
	if (x==5)//符号表中加入地址
	{
		int id=opst[optp-1]["val"];
		sans[id].type=opst[optp-2]["type"];
		sans[id].addr=offset;
		offset+=4; 
	}
	else if (x==6||x==7)//符号表中加入表达式类型（0为int，1为float） 
	{
		nw["type"]=opst[optp]["name"]-3;
	}
	else if (x==8)//表达式翻译 等于 
	{
		int a1=widen(opst[optp-1]["val"],getType(opst[optp-1]["val"]),getType(opst[optp-3]["val"]));//类型转换 
		codeNum++;
		code[codeNum].push_back(mp(opst[optp-3]["val"],1));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp('=',0));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp(opst[optp-1]["val"],1));
	}
	else if (x==18)//+
	{
		nw["val"]=--tnum;
		typeT[-tnum]=max(getType(opst[optp-2]["val"]),getType(opst[optp]["val"]));
		int nowT=-tnum; 
		int a1=widen(opst[optp-2]["val"],getType(opst[optp-2]["val"]),typeT[nowT]);
		int a2=widen(opst[optp]["val"],getType(opst[optp]["val"]),typeT[nowT]);
		codeNum++;
		code[codeNum].push_back(mp(nw["val"],1));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp('=',0));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp(a1,1));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp('+',0));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp(a2,1));
	}
	else if (x==19)//-
	{
		nw["val"]=--tnum;
		typeT[-tnum]=max(getType(opst[optp-2]["val"]),getType(opst[optp]["val"]));
		int nowT=-tnum; 
		int a1=widen(opst[optp-2]["val"],getType(opst[optp-2]["val"]),typeT[nowT]);
		int a2=widen(opst[optp]["val"],getType(opst[optp]["val"]),typeT[nowT]);
		codeNum++;
		code[codeNum].push_back(mp(nw["val"],1));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp('=',0));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp(a1,1));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp('-',0));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp(a2,1));
	}
	else if (x==20||x==21||x==25||x==26)
	{
		nw.insert(opst[optp].begin(),opst[optp].end());
	}
	else if (x==22)//* 
	{
		nw["val"]=--tnum;
		typeT[-tnum]=max(getType(opst[optp-2]["val"]),getType(opst[optp]["val"]));
		int nowT=-tnum; 
		int a1=widen(opst[optp-2]["val"],getType(opst[optp-2]["val"]),typeT[nowT]);
		int a2=widen(opst[optp]["val"],getType(opst[optp]["val"]),typeT[nowT]);
		codeNum++;
		code[codeNum].push_back(mp(nw["val"],1));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp('=',0));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp(a1,1));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp('*',0));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp(a2,1));
	}
	else if (x==23)// /
	{
		nw["val"]=--tnum;
		typeT[-tnum]=max(getType(opst[optp-2]["val"]),getType(opst[optp]["val"]));
		int nowT=-tnum; 
		int a1=widen(opst[optp-2]["val"],getType(opst[optp-2]["val"]),typeT[nowT]);
		int a2=widen(opst[optp]["val"],getType(opst[optp]["val"]),typeT[nowT]);
		codeNum++;
		code[codeNum].push_back(mp(nw["val"],1));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp('=',0));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp(a1,1));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp('/',0));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp(a2,1));
	}
	else if (x==24)
	{
		nw.insert(opst[optp-1].begin(),opst[optp-1].end());
	}
	else if (x==15)//布尔表达式翻译 >  
	{
		setNum++;
		setList[setNum].insert(codeNum+1);
		nw["truelist"]=setNum;
		setNum++;
		setList[setNum].insert(codeNum+2);
		nw["falselist"]=setNum;
		codeNum++;
		code[codeNum].push_back(mp('i',0));
		code[codeNum].push_back(mp('f',0));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp(opst[optp-2]["val"],1));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp('>',0));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp(opst[optp]["val"],1));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp('g',0));
		code[codeNum].push_back(mp('o',0));
		code[codeNum].push_back(mp('t',0));
		code[codeNum].push_back(mp('o',0));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp('?',0));
		codeNum++;
		code[codeNum].push_back(mp('g',0));
		code[codeNum].push_back(mp('o',0));
		code[codeNum].push_back(mp('t',0));
		code[codeNum].push_back(mp('o',0));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp('?',0));
	}
	else if (x==16)//<
	{
		setNum++;
		setList[setNum].insert(codeNum+1);
		nw["truelist"]=setNum;
		setNum++;
		setList[setNum].insert(codeNum+2);
		nw["falselist"]=setNum;
		codeNum++;
		code[codeNum].push_back(mp('i',0));
		code[codeNum].push_back(mp('f',0));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp(opst[optp-2]["val"],1));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp('<',0));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp(opst[optp]["val"],1));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp('g',0));
		code[codeNum].push_back(mp('o',0));
		code[codeNum].push_back(mp('t',0));
		code[codeNum].push_back(mp('o',0));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp('?',0));
		codeNum++;
		code[codeNum].push_back(mp('g',0));
		code[codeNum].push_back(mp('o',0));
		code[codeNum].push_back(mp('t',0));
		code[codeNum].push_back(mp('o',0));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp('?',0));
	}
	else if (x==17)//= 
	{
		setNum++;
		setList[setNum].insert(codeNum+1);
		nw["truelist"]=setNum;
		setNum++;
		setList[setNum].insert(codeNum+2);
		nw["falselist"]=setNum;
		codeNum++;
		code[codeNum].push_back(mp('i',0));
		code[codeNum].push_back(mp('f',0));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp(opst[optp-2]["val"],1));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp('=',0));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp(opst[optp]["val"],1));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp('g',0));
		code[codeNum].push_back(mp('o',0));
		code[codeNum].push_back(mp('t',0));
		code[codeNum].push_back(mp('o',0));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp('?',0));
		codeNum++;
		code[codeNum].push_back(mp('g',0));
		code[codeNum].push_back(mp('o',0));
		code[codeNum].push_back(mp('t',0));
		code[codeNum].push_back(mp('o',0));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp('?',0));
	}
	else if (x==13)
	{
		nw["instr"]=codeNum+1;
	}
	else if (x==14)
	{
		setNum++;
		setList[setNum].insert(codeNum+1);
		nw["nextlist"]=setNum;
		codeNum++;
		code[codeNum].push_back(mp('g',0));
		code[codeNum].push_back(mp('o',0));
		code[codeNum].push_back(mp('t',0));
		code[codeNum].push_back(mp('o',0));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp('?',0));
	}
	else if (x==9)//控制流翻译 if 
	{
		backpatch(opst[optp-3]["truelist"],opst[optp-1]["instr"]);
		nw["nextlist"]=merge(opst[optp-3]["falselist"],opst[optp]["nextlist"]);
	}
	else if (x==10)//if else 
	{
		backpatch(opst[optp-7]["truelist"],opst[optp-5]["instr"]);
		backpatch(opst[optp-7]["falselist"],opst[optp-1]["instr"]);
		nw["nextlist"]=merge(merge(opst[optp-4]["nextlist"],opst[optp-3]["nextlist"]),opst[optp]["nextlist"]);
	}
	else if (x==11)//while 
	{
		backpatch(opst[optp]["nextlist"],opst[optp-5]["instr"]);
		backpatch(opst[optp-3]["truelist"],opst[optp-1]["instr"]);
		nw["nextlist"]=opst[optp-3]["falselist"];
		codeNum++;
		code[codeNum].push_back(mp('g',0));
		code[codeNum].push_back(mp('o',0));
		code[codeNum].push_back(mp('t',0));
		code[codeNum].push_back(mp('o',0));
		code[codeNum].push_back(mp(' ',0));
		code[codeNum].push_back(mp('?',0));
		slot[codeNum]=opst[optp-5]["instr"];
	}
	else if (x==12)
	{
		backpatch(opst[optp-2]["nextlist"],opst[optp-1]["instr"]);
		nw["nextlist"]=opst[optp]["nextlist"];
	}
	for (auto y:pro[x]) tp--,optp--;
	walkg(proLeft[x],nw);
//	printf("%c->",proLeft[x]);
//	for (auto y:pro[x])
//		if (y>='A') printf("%c",y);
//		else printf("%s",key[y]);
//	//printf("%d %d\n",x.first,x.second);
//	printf("\n"); 
}

void walk(int x,int val=-1)//和实验二大体相同，区别是符号栈opst变成了map 
{
	while (action[st[tp]][x].rs)
	{
		if (st[tp]==25) //处理归约-归约冲突，出现这种冲突时应当根据符号栈的内容进行选择 
			if (optp>=3&&opst[optp]["name"]=='S'&&opst[optp-1]["name"]=='M'&&opst[optp-2]["name"]=='S')
				action[st[tp]][x].nxt=12;
			else action[st[tp]][x].nxt=13;
		else if (st[tp]==49)
			if (optp>=6&&opst[optp]["name"]=='S'&&opst[optp-1]["name"]=='M'&&opst[optp-2]["name"]==20&&opst[optp-3]["name"]=='C')
				if (x==1) action[st[tp]][x].nxt=14;
				else action[st[tp]][x].nxt=9;
			else action[st[tp]][x].nxt=13;
		else if (st[tp]==52)
			if (optp>=7&&opst[optp]["name"]=='S'&&opst[optp-1]["name"]=='M'&&opst[optp-2]["name"]==20&&opst[optp-3]["name"]=='C')
				action[st[tp]][x].nxt=11;
			else action[st[tp]][x].nxt=13;
		else if (st[tp]==55)
			if (optp>=10&&opst[optp]["name"]=='S'&&opst[optp-1]["name"]=='M'&&opst[optp-2]["name"]==1&&opst[optp-3]["name"]=='N')
				action[st[tp]][x].nxt=10;
			else action[st[tp]][x].nxt=13;
		else if (st[tp]==9)
			if (optp>=2&&opst[optp]["name"]=='S'&&opst[optp-1]["name"]=='D'&&x==21)//当无法根据符号栈选择时，根据输入符号选择 
				action[st[tp]][x].nxt=2;
			else action[st[tp]][x].nxt=13;
		
		if (st[tp]==5 && x==21) {printf("ACCEPT!\n");return;}
		int nw=action[st[tp]][x].nxt;
		printPro(nw);
		printStack();
		
	}
	int nw=st[tp];
	if (action[nw][x].nxt!=-1)
	{
		opst[++optp].clear();
		opst[optp]["name"]=x; 
		if (val!=-1) opst[optp]["val"]=val;
		st[++tp]=action[nw][x].nxt;
		printStack();
	}
	else {printf("ERROR on action!\n");ERROR=1;return;}
	
}
void LRAnalysis()
{
//	freopen("newanalysis.txt","w",stdout);
	st[++tp]=0;ERROR=0;
	for (int i=1;i<=total;i++)
	{
//		printf("ADD ");
//		if (ans[i].type==1) printf("id\n");
//		else if (ans[i].type==2) printf("digits\n");
//		else
//		{
//			for (int j=ans[i].l;j<=ans[i].r;j++)
//				if (!wrong[j]) printf("%c",S[j]);
//		}
//		printf("\n");
		if (ans[i].type==1)//id 6 
			walk(6,ans[i].val);
		else if (ans[i].type==2)//digits 5 
			walk(5,ans[i].val);
		else if (ans[i].type==3)
			walk(ans[i].val);
		else if (ans[i].type==4)
			walk(ans[i].val);
		else if (ans[i].type==5)
			walk(ans[i].val);
		if (ERROR) 
		{
			for (int j=ans[i].l;j<=ans[i].r;j++)
				if (!wrong[j]) {printf("Compile Error on line %d at column %d\n",pos[j].first,pos[j].second);break;}
		}
	}
	walk(21);
}
void outputCode()
{
	for (int i=1;i<=codeNum;i++)
	{
		printf("%4d: ",i);
		for (auto x:code[i])
		{
			if (x.second)
				if (x.first>=0)
				{
					for (int j=sans[x.first].l;j<=sans[x.first].r;j++)
						if (!wrong[j]) printf("%c",S[j]);
				}
				else printf("t%d",-x.first);
			else
			{
				if (x.first=='?' && slot[i]) 
					printf("%d",slot[i]);
				else printf("%c",x.first);
			}
		}
		printf("\n");
	}
}
void showTable()
{
	printf("SYMBOL TABLE\n");
	printf("%5s","ID");
	printf("%5s","VAL");
	printf("%5s","TYPE");
	printf("%5s","ADDR");
	printf("%5s\n","NAME");
	for (int i=1;i<=stotal;i++)
	{
		printf("%5d",i);
		if (sans[i].val>=0) printf("%5d",sans[i].val);
		else printf("     ");
		if (sans[i].type>=0) printf("%5d",sans[i].type);
		else printf("     ");
		if (sans[i].addr>=0) printf("%5d",sans[i].addr);
		else printf("     ");
		printf("   "); 
		for (int j=sans[i].l;j<=sans[i].r;j++)
			if (!wrong[j]) printf("%c",S[j]);
		printf("\n");
	}
}
int main()
{
	read();
	getEmpty();
	getFirst();
	getFollow();
	getClousure();
	LexicalAnalysis();
	LRAnalysis();
	freopen("IntermedianCode.txt","w",stdout);
	outputCode();
	showTable();
}

