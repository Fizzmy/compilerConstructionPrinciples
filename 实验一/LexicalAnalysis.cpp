#include<cstdio>
#include<iostream>
#include<cstring>
#define mp(a,b) make_pair(a,b)
#define BEGIN 0
#define VAL 1
#define NUM 2
#define OP 3
#define BK 4
#define KEYWORD 5
using namespace std;
char s[1010][1010],c;
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
int rt=1,tr[100010][73],sz=1;
int tag[100010];
int tf(char s)//将可能出现的字符转换到0-72的数字，压缩字典树数组  
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
const int ALLKEY=22;
//0-4 keyword,5 constant,6 val,7-17 operation,18-20 breakword,21 epsilon
char *key[]={"if","else","while","int","float","digits","id","==",">=","<=","!=",">","<","=","+","-","*","/",";","(",")","^"};
char t[100010];
int insert(char *s,int iskey,int l=0,int r=0,bool isNum=0)
//s是要插入或查询的单词，l,r是这个单词在S里的位置 
//iskey不是-1:向字典树中添加一个单词 / iskey是-1 isNum=0:查询一个标识符在符号表中的位置，若不存在则添加该标识符
//iskey是-1 isNum=1:查询一个数字在符号表中的位置，若不存在则添加该数字 
{
	int n=strlen(s);
	if (n==0) return 0;
	int x=rt;
	for (int i=0;i<n;i++)//从根开始从字典树上移动 
	{
		if (!tr[x][tf(s[i])])//下个位置的点不存在，创建一个新点 
			tr[x][tf(s[i])]=++sz,tag[sz]=-100;
		x=tr[x][tf(s[i])];
	}
	if (iskey>=0) tag[x]=-iskey;//在最终走到的结点上打上负标记  标记为正表示是关键词或符号 为负表示是标识符或数字常量 
	else
	{
		if (tag[x]==-100)//标识符或数字不存在 
		{
			tag[x]=++stotal,sans[stotal].l=l,sans[stotal].r=r;
			//说明遇到新标识符或新数字 打上正标记 将标识符或数字加入符号表 
			if (isNum)
			{
				int x=0;
				for (int i=0;i<n;i++) x=x*10+s[i]-'0';
				sans[stotal].val=x;
				//如果是数字常量还需要计算存储数字的值 
			}
			else sans[stotal].val=-1;
		}
	}
	return tag[x];
}
void addKeyword()//添加关键词 
{
	tag[rt]=-100;
	for (int i=0;i<ALLKEY;i++)
		insert(key[i],i);
}
void readCode()
{
	freopen("code.txt","r",stdin);
	sum=0;
	while ((c=getchar())!=EOF)//按照行读取文件里的代码 
	{
		s[sum][cnt++]=c;
		if (c=='\n') s[sum][cnt]='\0',cnt=0,sum++;
	}
}
void modifyCode()//删除注释 
{
	int can=0;
	for (int i=0;i<=sum;i++)
	{
		int len=strlen(s[i]);
		for (int j=0;j<len;j++)
		{
			if (can&&j+1<len&&s[i][j]=='*'&&s[i][j+1]=='/') {j++;can=0;continue;}//遇到*/结束对代码的忽略 
			if (can) continue;
			if (j+1<len&&s[i][j]=='/'&&s[i][j+1]=='/') break;//遇到//直接跳过该行后面的代码 
			if (j+1<len&&s[i][j]=='/'&&s[i][j+1]=='*') {j++;can=1;continue;}//遇到/*后面的代码全部忽略 
			pos[N]=mp(i+1,j+1),S[N++]=s[i][j];//用S将注释外的所有代码连接起来，用pos存储每个位置的字符在源代码中的位置S 
		}
	}
}
void lexicalAnalysis()
{
	//使用类似自动机的结构，status表示当前在自动机上的哪个点
	// BEGIN 初始点 
	// VAL 要匹配一个标识符 
	// NUM 要匹配一个数字常量
	// OP 要匹配一个运算符
	// BK 要匹配一个分隔符 
	int status=BEGIN;
	int pre=0;
	for (int i=0;i<=N;i++)
	{
		if (status==BEGIN)//当前在初始点 
		{
			pre=i;
			if (S[i]>='0'&&S[i]<='9')//下一个字符是数字，移入NUM 
				status=NUM;
			else if ((S[i]>='a'&&S[i]<='z') || (S[i]>='A'&&S[i]<='Z'))//下一个字符是字母，移入VAL 
				status=VAL;
			else if (S[i]=='+'||S[i]=='-'||S[i]=='*'||S[i]=='/'||S[i]=='>'||S[i]=='<'||S[i]=='='||S[i]=='!')//下一个字符是运算符，移入OP 
				status=OP;
			else if (S[i]=='('||S[i]==')'||S[i]==';'||S[i]=='\'')//下一个字符是分隔符，移入BK 
				status=BK;
		}
		else if (status==VAL)//当前要匹配一个标识符 
		{
			if ((S[i]>='0'&&S[i]<='9')||(S[i]>='a'&&S[i]<='z') || (S[i]>='A'&&S[i]<='Z'))
				status=VAL;//下一个字符是字母和数字状态不变 
			else//如果不是，说明找到单词 
			{
				i--;//指针左移 
				total++;//加入token表 
				ans[total].l=pre;ans[total].r=i;
				int nw=0;
				for (int j=pre;j<=i;j++)
					t[nw++]=S[j];
				t[nw]=0;
				int id=insert(t,-1,pre,i);//查询是否是关键字或者是已经出现过的标识符 
				if (id<=0)//是关键字 
					ans[total].type=KEYWORD,ans[total].val=-id;//val中的值代表具体是那个关键字，供后文语法分析用 
				else ans[total].type=VAL,ans[total].val=id;//是标识符，val中的值是该标识符在符号表的位置 
				status=BEGIN;//状态变回初始态 
			}
		}
		else if (status==NUM)//当前要匹配一个数字 
		{
			if (S[i]>='0'&&S[i]<='9')//下一个字符是数字，状态不变 
				status=NUM;
			else if ((S[i]>='a'&&S[i]<='z') || (S[i]>='A'&&S[i]<='Z'))//下一个字符是字母，无效单词，报错并跳过这个字符
			{
				wrong[i]=1;
				printf("Compile Error on line %d at column %d : invalid constant\n",pos[i].first,pos[i].second);
				status=NUM;
			}
			else//都不是，说明找到单词 
			{
				i--;//指针左移 
				int nw=0;
				for (int j=pre;j<=i;j++)
					t[nw++]=S[j];
				t[nw]=0;
				int id=insert(t,-1,pre,i,1);//查询是否是已经出现过的数字常量 
				total++;//加入token表，val中的值是该数字常量在符号表的位置 
				ans[total].l=pre;ans[total].r=i;ans[total].type=NUM;ans[total].val=id;
				status=BEGIN;//状态变回初始态 
			}
		}
		else if (status==OP)//要匹配一个运算符
		{
			if ((S[i-1]=='='&&S[i]=='=') ||  (S[i-1]=='>'&&S[i]=='=') || (S[i-1]=='<'&&S[i]=='=') || (S[i-1]=='!'&&S[i]=='=')) 
			{//判断是不是两个字符的运算符 
				total++;
				int nw=0;
				for (int j=pre;j<=i;j++)
					t[nw++]=S[j];
				t[nw]=0;
				int id=insert(t,-1,pre,i);//查询运算符对应的val ，加入token表 
				ans[total].l=pre;ans[total].r=i;ans[total].type=OP;ans[total].val=-id;//val中的值代表具体是那个运算符，供后文语法分析用 
				status=BEGIN;
			}
			else if (S[i]=='+'||S[i]=='-'||S[i]=='*'||S[i]=='/'||S[i]=='>'||S[i]=='<'||S[i]=='='||S[i]=='!')
			{//判断是不是不合法的两个字符的运算符 
				wrong[i]=1;//无效单词，报错并跳过这个字符 
				printf("Compile Error on line %d at column %d : invalid operator\n",pos[i].first,pos[i].second);
				status=OP;
			}
			else
			{//否则是一个字符的运算符，处理方法同上 
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
		{//是分隔符，也是按照上文方法处理 
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
void showTable()
{
	freopen("tokenTable.txt","w",stdout); 
	//printf("TOKEN TABLE\n");
	printf("%5s","NAME");
	printf("%10s","TYPE");
	printf("%5s\n","VAL");
	for (int i=1;i<=total;i++)
	{
		for (int j=1;j<=5-(ans[i].r-ans[i].l+1);j++) printf(" ");
		for (int j=ans[i].l;j<=ans[i].r;j++)
			if (!wrong[j]) printf("%c",S[j]);
		//printf(" ");
		if (ans[i].type==1)
			printf("%10s","VALUE");
		else if (ans[i].type==2)
			printf("%10s","NUMBER");
		else if (ans[i].type==3)
			printf("%10s","OPERATION");
		else if (ans[i].type==4)
			printf("%10s","SEPARATOR");
		else if (ans[i].type==5)
			printf("%10s","KEYWORD");
		if (ans[i].val!=-1) printf("%5d",ans[i].val);
		else printf("     ");
		printf("\n");
	}
	freopen("symbolTable.txt","w",stdout); 
	//printf("SYMBOL TABLE\n");
	printf("%5s","ID");
	printf("%5s","NAME");
	printf("%5s\n","VAL");
	for (int i=1;i<=stotal;i++)
	{
		printf("%5d",i);
		for (int j=1;j<=5-(sans[i].r-sans[i].l+1);j++) printf(" ");
		for (int j=sans[i].l;j<=sans[i].r;j++)
			if (!wrong[j]) printf("%c",S[j]);
		if (sans[i].val>=0) printf("%5d",sans[i].val);
		else printf("     "); 
		printf("\n");
	}
}
int main()
{
	addKeyword();//向字典树中添加关键词、符号等 
	readCode();//读入代码 
	modifyCode();//对代码进行修改，删除注释 
	lexicalAnalysis(); //词法分析 
	showTable();//输出token表和符号表 
}

