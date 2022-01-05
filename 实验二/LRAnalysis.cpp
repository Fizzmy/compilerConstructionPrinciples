#include<cstdio>
#include<iostream>
#include<vector>
#include<cstring>
#include<algorithm> 
#include<map>
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
}ans[100010];
struct symbol{
	int l,r,val;
}sans[10010];
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
				sans[stotal].val=x;
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
void add(char *s,int l,int r)//处理在产生式右部找到的一个文法符号 
{
	if (l==r && s[l]>='A'&&s[r]<='Z')//找到的是非终结符 
		pro[numpro].push_back(s[l]);
	else//找到的是终结符 
	{
		int nw=0;
		for (int i=l;i<=r;i++)
			t[nw++]=s[i];
		t[nw]=0;
		int pos=-insert(t,-1);//在字典树中寻找终结符对应的序号 
		if (pos!=ALLKEY-1) pro[numpro].push_back(pos);//如果不为空，加入存储产生式的数据结构中 
		
	}
}
void getProduct(char *s,int len)//处理产生式 
{
	numpro++;
	proClass[s[0]].push_back(numpro);//记录同一个终结符的产生式集合 
	proLeft[numpro]=s[0];//记录每个产生式左部的字符 
	int lst=2;
	for (int i=2;i<len;i++)//处理产生式的右部 
		if (s[i]==' ')
			add(s,lst,i-1),lst=i+1;
	add(s,lst,len-1);
}
void read()//读入产生式 
{
	freopen("product.txt","r",stdin);
	addkeyword();//添加文法中的非终结符（空用^代替）  
	while (1)
	{
		c=getchar();
		s[len++]=c;//读取一个产生式 
		if (c=='\n' || c==EOF)
		{
			len--;
			s[len]=0;
			getProduct(s,len);//处理这个产生式 
			len=0;
		}
		if (c==EOF) break;
	}
}
int numC=0;
bool emp[300];bool vis[300];
bool isEmpty(char s)//判断符号s能不能推导出空句子，返回值1为是，0为不是，存储在emp数组中 
{
	if (s>='A' && s<='Z')//当前符号是非终结符 
	{
		vis[s]=1;//标记当前非终结符已经访问 
		for (auto x:proClass[s])//对该非终结符的所有产生式进行处理 
		{
			if (pro[x].size()==0) {vis[s]=0;return emp[s]=1;}//如果是空产生式，直接返回1 
			bool can=1;
			for (auto y:pro[x])//枚举产生式右部每个符号 
				if (!vis[y] && !isEmpty(y))//如果当前符号没有被访问过，则对他进行访问，判断他能不能是空句子 
				{//如果不能则这个产生式不能产生空句子，枚举下个产生式 
					can=0;
					break;
				}
				//如果右部每个符号都能产生空句子，返回1，回溯vis数组 
			if (can) {vis[s]=0;return emp[s]=1;}
		}
		//所有产生式都不行则返回0，回溯vis数组 
		vis[s]=0;
		return emp[s]=0;
	}
	//如果是终结符直接返回0 
	return 0;
}
void getEmpty()//判断每个非终结符能不能推导出空句子
{
	for (int i='A';i<='Z';i++) isEmpty(i);
}
int first[300],follow[300];
int findFirst(char s)//求非终结符s的FIRST集 
{
	vis[s]=1;//标记当前非终结符已经访问 
	for (auto x:proClass[s])//对该非终结符的所有产生式进行处理
	{
		bool can=1; 
		for (auto y:pro[x])//枚举产生式右部每个字符y 
		{
			if (y<ALLKEY) first[s]|=((1<<y)&((1<<21)-1));//如果y是终结符，FIRST集添加y 
			else if (!vis[y]) first[s]|=(findFirst(y)&((1<<21)-1));//如果y是没有访问过的非终结符，则先求y的FIRST集，将y的FIRST集加入s 
			if (!emp[y]) {can=0;break;}//如果当前符号不能是空集，则直接退出循环，后面的字符不会成为FIRST集 
		}
		if (can) first[s]|=(1<<21);//如果产生式可以推出空产生式，则将空集加入FIRST集 
	}
	vis[s]=0;//回溯vis数组 
	return first[s]; 
}
void getFirst()//求每个符号的FIRST集，因为FIRST集大小为22所以可以将状态压缩，用22个bit存储，这里第22位代表的是空 
{

	for (int i=0;i<ALLKEY;i++)//求每个终结符的FIRST集 
		first[i]=1<<i;
	for (int i='A';i<='Z';i++)//求非终结符的FIRST集 
	{ 
		findFirst(i);
//FIRST集的输出代码 
//		if (first[i]!=0)
//		{
//			printf("FIRST %c: ",i);
//			for (int j=0;j<ALLKEY;j++)
//				if ((first[i]>>j)&1) printf("%s ",key[j]);
//			printf("\n");
//		}
	}
}
int findFollow(char s)//求非终结符s的FOLLOW集 
{
	vis[s]=1;//标记当前非终结符已经访问 
	for (int i=1;i<=numpro;i++)//枚举所有产生式 
	{
		for (int j=0;j<pro[i].size();j++)//枚举右部 
			if (pro[i][j]==s)//右部出现s 
			{
				bool can=1;
				for (int k=j+1;k<pro[i].size();k++)//对s后面的字符进行处理 
				{
					follow[s]|=(first[pro[i][k]]&((1<<21)-1));//将后面字符的FIRST集去除空，加入s的FOLLOW集 
					if (!emp[pro[i][k]]) {can=0;break;}//如果该字符不能为空直接返回，后面的字符的FIRST不能加入FOLLOW集 
				}
				if (can && !vis[proLeft[i]])//如果该产生式能产生空句子，将产生式左部的FOLLOW集加入s的FOLLOW集 
					follow[s]|=findFollow(proLeft[i]);//如果产生式左部的FOLLOW集还未被访问过，则先递归求 
			}
	}
	//回溯vis数组 
	vis[s]=0;
	return follow[s]; 
}
void getFollow()//求非终结符的FOLLOW集，因为FOLLOW集大小为22所以可以将状态压缩，用22个bit存储，这里第22位代表的是$ 
{
	follow['Z']=(1<<21);//开始符号的FOLLOW集有$ 
	for (int i='A';i<='Z';i++)
	{
		findFollow(i);//求每个非终结符的FOLLOW集 
//FOLLOW集的输出代码 
//		if (follow[i]!=0)
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
	if (pro[x.first].size()==x.second)//如果点在产生式的尾部 
	{
		int nfollow=follow[proLeft[x.first]];//SLR分析法，在左部的FOLLOW集对应的集合中加入归约 
		for (int j=0;j<ALLKEY;j++)
			if ((nfollow>>j)&1) //如果j在FOLLOW集里，则加入规约 
			{
				if (action[id][j].nxt==-1||j!=1)
					action[id][j].nxt=x.first,action[id][j].rs=1;
				//这里会出现移入规约冲突，经过分析只有else那里不归约，其他情况都选择归约，所以加入特判j!=1 
			}
	}
//项输出语句 
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

void dfs(int nxt,int k,vector< pair<int,int> > &nw)//求非终结符nxt的产生式能推导出的句子首部能不能有k 
{
	if (vis[nxt]) return;
	vis[nxt]=1;//标记非终结符已经访问 
	for (auto y:proClass[nxt])
	{
		if (pro[y].size()&&pro[y][0]==k)//产生式第一个字符是k，把项加入nw数组 
			nw.push_back(mp(y,1));
		if (pro[y].size())
		{
			char x=pro[y][0];
			if (x>='A'&&x<='Z')//如果产生式第一个字符还是非终结符，继续递归 
				dfs(x,k,nw);
		}
	}
}
void dfs(int id,int nxt)//求状态i中非终结符nxt能推导出的非内核项 
{
	if (vis[nxt]) return;
	vis[nxt]=1;
	for (auto y:proClass[nxt])
	{
		print(id,mp(y,0));//输出项、添加ACTION表的归约 
		if (pro[y].size())
		{	
			char x=pro[y][0];
			if (x>='A'&&x<='Z')
				dfs(id,x);
		}
	}
}

void getClousure()//求规范项集族 I状态只存内核项 
{
	I[numC++].push_back(mp(1,0));//开始状态，pair中.first存储是哪个产生式，.second存储右部点的位置 
	int lst=0;
	while (numC!=lst)
	{
		int nc=numC;
		for (int i=lst;i<nc;i++)//每次枚举所有新状态 
		{
			//cout<<i<<endl;
			for (int k=0;k<ALLKEY;k++)//枚举所有的终结符 
			{
				vector< pair<int,int> > nw; //保存新状态走对应终结符能到的状态 
				for (auto x:I[i]) //枚举状态中所有的产生式 
				{
					if (pro[x.first].size()==x.second) continue;//如果点在产生式尾则跳过 
					char nxt=pro[x.first][x.second];//找到点之后的符号 
					if (nxt>='A'&&nxt<='Z')//如果是非终结符则搜索非内核项中有没有合法的项 
						dfs(nxt,k,nw);	
					if (nxt==k) nw.push_back(mp(x.first,x.second+1));//如果是终结符则直接判断	
				}
				for (int y='A';y<='Z';y++) vis[y]=0;//清空vis数组 
				if (nw.size()==0) continue;//如果状态集为空则直接跳过 
				sort(nw.begin(),nw.end());//对状态集进行排序，方便后面进行状态集相同的判断 
				auto iter = unique(nw.begin(),nw.end());//去重 
				nw.erase(iter,nw.end());
				bool can=1;int target=0;
				for (int q=0;q<numC;q++)
					if (I[q]==nw) {can=0;target=q;break;}// 判断是否已经出现过相同的状态集 
				if (can) I[numC++]=nw,target=numC-1;//如果没有则把新状态集加入集族 
				action[i][k].nxt=target;//填充ACTION表，rs为0表示移入，为1表示归约 
				action[i][k].rs=0;
			}
			for (int k='A';k<='Z';k++)//枚举所有的非终结符，流程和上述一样，但是修改的是GOTO表 
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
//状态集输出代码 
//	freopen("status.txt","w",stdout);
	for (int i=0;i<numC;i++)
	{
		//printf("------------STATUS %d --------------\n",i);
		for (auto x:I[i])//输出项、添加ACTION表的归约 
			print(i,x);
		for (auto x:I[i])//求状态的非内核项 
		{
			if (pro[x.first].size()==x.second) continue;
			char nxt=pro[x.first][x.second];
			if (nxt>='A'&&nxt<='Z') 
				dfs(i,nxt);//如果是非终结符则递归求非内核项 
		}
		for (int y='A';y<='Z';y++) vis[y]=0;
	}
//  ACTION表和GOTO表输出代码 
//	freopen("actiongoto.txt","w",stdout);
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
void LexicalAnalysis()//词法分析，和实验一相同 
{
	readCode();
	modifyCode();
	lexicalAnalysis(); 
}
bool ERROR=0;
int st[1100],tp=0,optp=0;
char opst[1100];
//acc status 5 21
void printStack()
{
	printf("\n------------------NOW STACK------------\n");
	for (int i=1;i<=tp;i++) printf("%d ",st[i]);printf("\n");
	printf("\n-------------OPERATION STACK-----------\n");
	for (int i=1;i<=optp;i++)
	{
		//printf("%d\n",opst[i]);
		if (opst[i]>='A') printf("%c",opst[i]);else printf("%s",key[opst[i]]);
	}printf("\n");
}
void walkg(char s)
{
	int nw=st[tp];
	if (gt[nw][s-'A']!=-1)//如果GOTO对应位置有效就走 
	{
		st[++tp]=gt[nw][s-'A'];
		opst[++optp]=s;
		printStack();
	}
	else {printf("ERROR on goto!\n");ERROR=1;return;}//否则进行报错 ，忽略该输入 
}
void printPro(int x)
{
	printf("%c->",proLeft[x]);
	for (auto y:pro[x])
		if (y>='A') printf("%c",y);
		else printf("%s",key[y]);
	printf("\n"); 
}

void walk(int x)
{
	while (action[st[tp]][x].rs)//如果是规约操作 
	{
		if (st[tp]==5 && x==21) {printf("ACCEPT!\n");return;}//特判接受操作 
		int nw=action[st[tp]][x].nxt;
		for (auto x:pro[nw]) tp--,optp--;//根据产生式弹栈 
		walkg(proLeft[nw]);//走goto表 
		printPro(nw);//输出归约的产生式 
		printStack();//输出当前栈状态和符号栈 
		
	}
	int nw=st[tp];
	if (action[nw][x].nxt!=-1)//如果ACTION对应位置有效就走 
	{
		opst[++optp]=x; 
		st[++tp]=action[nw][x].nxt;
		printStack();
	}
	else {printf("ERROR on action!\n");ERROR=1;return;}//否则进行报错 ，忽略该输入 
	
}
void LRAnalysis()//按照词法分析出的token串进行LR分析
{
	freopen("LRAnalysisProcess.txt","w",stdout);
	st[++tp]=0;
	for (int i=1;i<=total;i++)
	{
		printf("ADD ");
		if (ans[i].type==1) printf("id\n");
		else if (ans[i].type==2) printf("digits\n");
		else
		{
			for (int j=ans[i].l;j<=ans[i].r;j++)
				if (!wrong[j]) printf("%c",S[j]);
		}
		printf("\n");
		//根据token串的类别进行不同的操作 
		//如果是运算符、分隔符、关键字，则token串中的val值就是LR自动机中应该走的边，注意这里边21代表的是结束符
		ERROR=0; 
		if (ans[i].type==1)//id 6 标识符 
			walk(6);
		else if (ans[i].type==2)//digits 5 数字常数 
			walk(5);
		else if (ans[i].type==3)//运算符 
			walk(ans[i].val);
		else if (ans[i].type==4)//分隔符 
			walk(ans[i].val);
		else if (ans[i].type==5)//关键字 
			walk(ans[i].val);
		if (ERROR) //错误则进行报错，输出错误位置 
		{
			for (int j=ans[i].l;j<=ans[i].r;j++)
				if (!wrong[j]) {printf("Compile Error on line %d at column %d\n",pos[j].first,pos[j].second);break;}
		} 
	}
	printf("ADD $\n");
	walk(21);//结束符 
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
}

