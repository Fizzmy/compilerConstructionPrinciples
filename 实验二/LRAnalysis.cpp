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
void add(char *s,int l,int r)//�����ڲ���ʽ�Ҳ��ҵ���һ���ķ����� 
{
	if (l==r && s[l]>='A'&&s[r]<='Z')//�ҵ����Ƿ��ս�� 
		pro[numpro].push_back(s[l]);
	else//�ҵ������ս�� 
	{
		int nw=0;
		for (int i=l;i<=r;i++)
			t[nw++]=s[i];
		t[nw]=0;
		int pos=-insert(t,-1);//���ֵ�����Ѱ���ս����Ӧ����� 
		if (pos!=ALLKEY-1) pro[numpro].push_back(pos);//�����Ϊ�գ�����洢����ʽ�����ݽṹ�� 
		
	}
}
void getProduct(char *s,int len)//�������ʽ 
{
	numpro++;
	proClass[s[0]].push_back(numpro);//��¼ͬһ���ս���Ĳ���ʽ���� 
	proLeft[numpro]=s[0];//��¼ÿ������ʽ�󲿵��ַ� 
	int lst=2;
	for (int i=2;i<len;i++)//�������ʽ���Ҳ� 
		if (s[i]==' ')
			add(s,lst,i-1),lst=i+1;
	add(s,lst,len-1);
}
void read()//�������ʽ 
{
	freopen("product.txt","r",stdin);
	addkeyword();//����ķ��еķ��ս��������^���棩  
	while (1)
	{
		c=getchar();
		s[len++]=c;//��ȡһ������ʽ 
		if (c=='\n' || c==EOF)
		{
			len--;
			s[len]=0;
			getProduct(s,len);//�����������ʽ 
			len=0;
		}
		if (c==EOF) break;
	}
}
int numC=0;
bool emp[300];bool vis[300];
bool isEmpty(char s)//�жϷ���s�ܲ����Ƶ����վ��ӣ�����ֵ1Ϊ�ǣ�0Ϊ���ǣ��洢��emp������ 
{
	if (s>='A' && s<='Z')//��ǰ�����Ƿ��ս�� 
	{
		vis[s]=1;//��ǵ�ǰ���ս���Ѿ����� 
		for (auto x:proClass[s])//�Ը÷��ս�������в���ʽ���д��� 
		{
			if (pro[x].size()==0) {vis[s]=0;return emp[s]=1;}//����ǿղ���ʽ��ֱ�ӷ���1 
			bool can=1;
			for (auto y:pro[x])//ö�ٲ���ʽ�Ҳ�ÿ������ 
				if (!vis[y] && !isEmpty(y))//�����ǰ����û�б����ʹ�����������з��ʣ��ж����ܲ����ǿվ��� 
				{//����������������ʽ���ܲ����վ��ӣ�ö���¸�����ʽ 
					can=0;
					break;
				}
				//����Ҳ�ÿ�����Ŷ��ܲ����վ��ӣ�����1������vis���� 
			if (can) {vis[s]=0;return emp[s]=1;}
		}
		//���в���ʽ�������򷵻�0������vis���� 
		vis[s]=0;
		return emp[s]=0;
	}
	//������ս��ֱ�ӷ���0 
	return 0;
}
void getEmpty()//�ж�ÿ�����ս���ܲ����Ƶ����վ���
{
	for (int i='A';i<='Z';i++) isEmpty(i);
}
int first[300],follow[300];
int findFirst(char s)//����ս��s��FIRST�� 
{
	vis[s]=1;//��ǵ�ǰ���ս���Ѿ����� 
	for (auto x:proClass[s])//�Ը÷��ս�������в���ʽ���д���
	{
		bool can=1; 
		for (auto y:pro[x])//ö�ٲ���ʽ�Ҳ�ÿ���ַ�y 
		{
			if (y<ALLKEY) first[s]|=((1<<y)&((1<<21)-1));//���y���ս����FIRST�����y 
			else if (!vis[y]) first[s]|=(findFirst(y)&((1<<21)-1));//���y��û�з��ʹ��ķ��ս����������y��FIRST������y��FIRST������s 
			if (!emp[y]) {can=0;break;}//�����ǰ���Ų����ǿռ�����ֱ���˳�ѭ����������ַ������ΪFIRST�� 
		}
		if (can) first[s]|=(1<<21);//�������ʽ�����Ƴ��ղ���ʽ���򽫿ռ�����FIRST�� 
	}
	vis[s]=0;//����vis���� 
	return first[s]; 
}
void getFirst()//��ÿ�����ŵ�FIRST������ΪFIRST����СΪ22���Կ��Խ�״̬ѹ������22��bit�洢�������22λ������ǿ� 
{

	for (int i=0;i<ALLKEY;i++)//��ÿ���ս����FIRST�� 
		first[i]=1<<i;
	for (int i='A';i<='Z';i++)//����ս����FIRST�� 
	{ 
		findFirst(i);
//FIRST����������� 
//		if (first[i]!=0)
//		{
//			printf("FIRST %c: ",i);
//			for (int j=0;j<ALLKEY;j++)
//				if ((first[i]>>j)&1) printf("%s ",key[j]);
//			printf("\n");
//		}
	}
}
int findFollow(char s)//����ս��s��FOLLOW�� 
{
	vis[s]=1;//��ǵ�ǰ���ս���Ѿ����� 
	for (int i=1;i<=numpro;i++)//ö�����в���ʽ 
	{
		for (int j=0;j<pro[i].size();j++)//ö���Ҳ� 
			if (pro[i][j]==s)//�Ҳ�����s 
			{
				bool can=1;
				for (int k=j+1;k<pro[i].size();k++)//��s������ַ����д��� 
				{
					follow[s]|=(first[pro[i][k]]&((1<<21)-1));//�������ַ���FIRST��ȥ���գ�����s��FOLLOW�� 
					if (!emp[pro[i][k]]) {can=0;break;}//������ַ�����Ϊ��ֱ�ӷ��أ�������ַ���FIRST���ܼ���FOLLOW�� 
				}
				if (can && !vis[proLeft[i]])//����ò���ʽ�ܲ����վ��ӣ�������ʽ�󲿵�FOLLOW������s��FOLLOW�� 
					follow[s]|=findFollow(proLeft[i]);//�������ʽ�󲿵�FOLLOW����δ�����ʹ������ȵݹ��� 
			}
	}
	//����vis���� 
	vis[s]=0;
	return follow[s]; 
}
void getFollow()//����ս����FOLLOW������ΪFOLLOW����СΪ22���Կ��Խ�״̬ѹ������22��bit�洢�������22λ�������$ 
{
	follow['Z']=(1<<21);//��ʼ���ŵ�FOLLOW����$ 
	for (int i='A';i<='Z';i++)
	{
		findFollow(i);//��ÿ�����ս����FOLLOW�� 
//FOLLOW����������� 
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
	if (pro[x.first].size()==x.second)//������ڲ���ʽ��β�� 
	{
		int nfollow=follow[proLeft[x.first]];//SLR�����������󲿵�FOLLOW����Ӧ�ļ����м����Լ 
		for (int j=0;j<ALLKEY;j++)
			if ((nfollow>>j)&1) //���j��FOLLOW���������Լ 
			{
				if (action[id][j].nxt==-1||j!=1)
					action[id][j].nxt=x.first,action[id][j].rs=1;
				//�������������Լ��ͻ����������ֻ��else���ﲻ��Լ�����������ѡ���Լ�����Լ�������j!=1 
			}
	}
//�������� 
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

void dfs(int nxt,int k,vector< pair<int,int> > &nw)//����ս��nxt�Ĳ���ʽ���Ƶ����ľ����ײ��ܲ�����k 
{
	if (vis[nxt]) return;
	vis[nxt]=1;//��Ƿ��ս���Ѿ����� 
	for (auto y:proClass[nxt])
	{
		if (pro[y].size()&&pro[y][0]==k)//����ʽ��һ���ַ���k���������nw���� 
			nw.push_back(mp(y,1));
		if (pro[y].size())
		{
			char x=pro[y][0];
			if (x>='A'&&x<='Z')//�������ʽ��һ���ַ����Ƿ��ս���������ݹ� 
				dfs(x,k,nw);
		}
	}
}
void dfs(int id,int nxt)//��״̬i�з��ս��nxt���Ƶ����ķ��ں��� 
{
	if (vis[nxt]) return;
	vis[nxt]=1;
	for (auto y:proClass[nxt])
	{
		print(id,mp(y,0));//�������ACTION��Ĺ�Լ 
		if (pro[y].size())
		{	
			char x=pro[y][0];
			if (x>='A'&&x<='Z')
				dfs(id,x);
		}
	}
}

void getClousure()//��淶��� I״ֻ̬���ں��� 
{
	I[numC++].push_back(mp(1,0));//��ʼ״̬��pair��.first�洢���ĸ�����ʽ��.second�洢�Ҳ����λ�� 
	int lst=0;
	while (numC!=lst)
	{
		int nc=numC;
		for (int i=lst;i<nc;i++)//ÿ��ö��������״̬ 
		{
			//cout<<i<<endl;
			for (int k=0;k<ALLKEY;k++)//ö�����е��ս�� 
			{
				vector< pair<int,int> > nw; //������״̬�߶�Ӧ�ս���ܵ���״̬ 
				for (auto x:I[i]) //ö��״̬�����еĲ���ʽ 
				{
					if (pro[x.first].size()==x.second) continue;//������ڲ���ʽβ������ 
					char nxt=pro[x.first][x.second];//�ҵ���֮��ķ��� 
					if (nxt>='A'&&nxt<='Z')//����Ƿ��ս�����������ں�������û�кϷ����� 
						dfs(nxt,k,nw);	
					if (nxt==k) nw.push_back(mp(x.first,x.second+1));//������ս����ֱ���ж�	
				}
				for (int y='A';y<='Z';y++) vis[y]=0;//���vis���� 
				if (nw.size()==0) continue;//���״̬��Ϊ����ֱ������ 
				sort(nw.begin(),nw.end());//��״̬���������򣬷���������״̬����ͬ���ж� 
				auto iter = unique(nw.begin(),nw.end());//ȥ�� 
				nw.erase(iter,nw.end());
				bool can=1;int target=0;
				for (int q=0;q<numC;q++)
					if (I[q]==nw) {can=0;target=q;break;}// �ж��Ƿ��Ѿ����ֹ���ͬ��״̬�� 
				if (can) I[numC++]=nw,target=numC-1;//���û�������״̬�����뼯�� 
				action[i][k].nxt=target;//���ACTION��rsΪ0��ʾ���룬Ϊ1��ʾ��Լ 
				action[i][k].rs=0;
			}
			for (int k='A';k<='Z';k++)//ö�����еķ��ս�������̺�����һ���������޸ĵ���GOTO�� 
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
//״̬��������� 
//	freopen("status.txt","w",stdout);
	for (int i=0;i<numC;i++)
	{
		//printf("------------STATUS %d --------------\n",i);
		for (auto x:I[i])//�������ACTION��Ĺ�Լ 
			print(i,x);
		for (auto x:I[i])//��״̬�ķ��ں��� 
		{
			if (pro[x.first].size()==x.second) continue;
			char nxt=pro[x.first][x.second];
			if (nxt>='A'&&nxt<='Z') 
				dfs(i,nxt);//����Ƿ��ս����ݹ�����ں��� 
		}
		for (int y='A';y<='Z';y++) vis[y]=0;
	}
//  ACTION���GOTO��������� 
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
	while ((c=getchar())!=EOF)//�����ж�ȡ�ļ���Ĵ��� 
	{
		s1[sum][cnt++]=c;
		if (c=='\n') s1[sum][cnt]='\0',cnt=0,sum++;
	}
}
void modifyCode()//ɾ��ע�� 
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
void LexicalAnalysis()//�ʷ���������ʵ��һ��ͬ 
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
	if (gt[nw][s-'A']!=-1)//���GOTO��Ӧλ����Ч���� 
	{
		st[++tp]=gt[nw][s-'A'];
		opst[++optp]=s;
		printStack();
	}
	else {printf("ERROR on goto!\n");ERROR=1;return;}//������б��� �����Ը����� 
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
	while (action[st[tp]][x].rs)//����ǹ�Լ���� 
	{
		if (st[tp]==5 && x==21) {printf("ACCEPT!\n");return;}//���н��ܲ��� 
		int nw=action[st[tp]][x].nxt;
		for (auto x:pro[nw]) tp--,optp--;//���ݲ���ʽ��ջ 
		walkg(proLeft[nw]);//��goto�� 
		printPro(nw);//�����Լ�Ĳ���ʽ 
		printStack();//�����ǰջ״̬�ͷ���ջ 
		
	}
	int nw=st[tp];
	if (action[nw][x].nxt!=-1)//���ACTION��Ӧλ����Ч���� 
	{
		opst[++optp]=x; 
		st[++tp]=action[nw][x].nxt;
		printStack();
	}
	else {printf("ERROR on action!\n");ERROR=1;return;}//������б��� �����Ը����� 
	
}
void LRAnalysis()//���մʷ���������token������LR����
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
		//����token���������в�ͬ�Ĳ��� 
		//�������������ָ������ؼ��֣���token���е�valֵ����LR�Զ�����Ӧ���ߵıߣ�ע�������21������ǽ�����
		ERROR=0; 
		if (ans[i].type==1)//id 6 ��ʶ�� 
			walk(6);
		else if (ans[i].type==2)//digits 5 ���ֳ��� 
			walk(5);
		else if (ans[i].type==3)//����� 
			walk(ans[i].val);
		else if (ans[i].type==4)//�ָ��� 
			walk(ans[i].val);
		else if (ans[i].type==5)//�ؼ��� 
			walk(ans[i].val);
		if (ERROR) //��������б����������λ�� 
		{
			for (int j=ans[i].l;j<=ans[i].r;j++)
				if (!wrong[j]) {printf("Compile Error on line %d at column %d\n",pos[j].first,pos[j].second);break;}
		} 
	}
	printf("ADD $\n");
	walk(21);//������ 
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

