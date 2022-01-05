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
int tf(char s)//�����ܳ��ֵ��ַ�ת����0-72�����֣�ѹ���ֵ�������  
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
//s��Ҫ������ѯ�ĵ��ʣ�l,r�����������S���λ�� 
//iskey����-1:���ֵ��������һ������ / iskey��-1 isNum=0:��ѯһ����ʶ���ڷ��ű��е�λ�ã�������������Ӹñ�ʶ��
//iskey��-1 isNum=1:��ѯһ�������ڷ��ű��е�λ�ã�������������Ӹ����� 
{
	int n=strlen(s);
	if (n==0) return 0;
	int x=rt;
	for (int i=0;i<n;i++)//�Ӹ���ʼ���ֵ������ƶ� 
	{
		if (!tr[x][tf(s[i])])//�¸�λ�õĵ㲻���ڣ�����һ���µ� 
			tr[x][tf(s[i])]=++sz,tag[sz]=-100;
		x=tr[x][tf(s[i])];
	}
	if (iskey>=0) tag[x]=-iskey;//�������ߵ��Ľ���ϴ��ϸ����  ���Ϊ����ʾ�ǹؼ��ʻ���� Ϊ����ʾ�Ǳ�ʶ�������ֳ��� 
	else
	{
		if (tag[x]==-100)//��ʶ�������ֲ����� 
		{
			tag[x]=++stotal,sans[stotal].l=l,sans[stotal].r=r;
			//˵�������±�ʶ���������� ��������� ����ʶ�������ּ�����ű� 
			if (isNum)
			{
				int x=0;
				for (int i=0;i<n;i++) x=x*10+s[i]-'0';
				sans[stotal].val=x;
				//��������ֳ�������Ҫ����洢���ֵ�ֵ 
			}
			else sans[stotal].val=-1;
		}
	}
	return tag[x];
}
void addKeyword()//��ӹؼ��� 
{
	tag[rt]=-100;
	for (int i=0;i<ALLKEY;i++)
		insert(key[i],i);
}
void readCode()
{
	freopen("code.txt","r",stdin);
	sum=0;
	while ((c=getchar())!=EOF)//�����ж�ȡ�ļ���Ĵ��� 
	{
		s[sum][cnt++]=c;
		if (c=='\n') s[sum][cnt]='\0',cnt=0,sum++;
	}
}
void modifyCode()//ɾ��ע�� 
{
	int can=0;
	for (int i=0;i<=sum;i++)
	{
		int len=strlen(s[i]);
		for (int j=0;j<len;j++)
		{
			if (can&&j+1<len&&s[i][j]=='*'&&s[i][j+1]=='/') {j++;can=0;continue;}//����*/�����Դ���ĺ��� 
			if (can) continue;
			if (j+1<len&&s[i][j]=='/'&&s[i][j+1]=='/') break;//����//ֱ���������к���Ĵ��� 
			if (j+1<len&&s[i][j]=='/'&&s[i][j+1]=='*') {j++;can=1;continue;}//����/*����Ĵ���ȫ������ 
			pos[N]=mp(i+1,j+1),S[N++]=s[i][j];//��S��ע��������д���������������pos�洢ÿ��λ�õ��ַ���Դ�����е�λ��S 
		}
	}
}
void lexicalAnalysis()
{
	//ʹ�������Զ����Ľṹ��status��ʾ��ǰ���Զ����ϵ��ĸ���
	// BEGIN ��ʼ�� 
	// VAL Ҫƥ��һ����ʶ�� 
	// NUM Ҫƥ��һ�����ֳ���
	// OP Ҫƥ��һ�������
	// BK Ҫƥ��һ���ָ��� 
	int status=BEGIN;
	int pre=0;
	for (int i=0;i<=N;i++)
	{
		if (status==BEGIN)//��ǰ�ڳ�ʼ�� 
		{
			pre=i;
			if (S[i]>='0'&&S[i]<='9')//��һ���ַ������֣�����NUM 
				status=NUM;
			else if ((S[i]>='a'&&S[i]<='z') || (S[i]>='A'&&S[i]<='Z'))//��һ���ַ�����ĸ������VAL 
				status=VAL;
			else if (S[i]=='+'||S[i]=='-'||S[i]=='*'||S[i]=='/'||S[i]=='>'||S[i]=='<'||S[i]=='='||S[i]=='!')//��һ���ַ��������������OP 
				status=OP;
			else if (S[i]=='('||S[i]==')'||S[i]==';'||S[i]=='\'')//��һ���ַ��Ƿָ���������BK 
				status=BK;
		}
		else if (status==VAL)//��ǰҪƥ��һ����ʶ�� 
		{
			if ((S[i]>='0'&&S[i]<='9')||(S[i]>='a'&&S[i]<='z') || (S[i]>='A'&&S[i]<='Z'))
				status=VAL;//��һ���ַ�����ĸ������״̬���� 
			else//������ǣ�˵���ҵ����� 
			{
				i--;//ָ������ 
				total++;//����token�� 
				ans[total].l=pre;ans[total].r=i;
				int nw=0;
				for (int j=pre;j<=i;j++)
					t[nw++]=S[j];
				t[nw]=0;
				int id=insert(t,-1,pre,i);//��ѯ�Ƿ��ǹؼ��ֻ������Ѿ����ֹ��ı�ʶ�� 
				if (id<=0)//�ǹؼ��� 
					ans[total].type=KEYWORD,ans[total].val=-id;//val�е�ֵ����������Ǹ��ؼ��֣��������﷨������ 
				else ans[total].type=VAL,ans[total].val=id;//�Ǳ�ʶ����val�е�ֵ�Ǹñ�ʶ���ڷ��ű��λ�� 
				status=BEGIN;//״̬��س�ʼ̬ 
			}
		}
		else if (status==NUM)//��ǰҪƥ��һ������ 
		{
			if (S[i]>='0'&&S[i]<='9')//��һ���ַ������֣�״̬���� 
				status=NUM;
			else if ((S[i]>='a'&&S[i]<='z') || (S[i]>='A'&&S[i]<='Z'))//��һ���ַ�����ĸ����Ч���ʣ�������������ַ�
			{
				wrong[i]=1;
				printf("Compile Error on line %d at column %d : invalid constant\n",pos[i].first,pos[i].second);
				status=NUM;
			}
			else//�����ǣ�˵���ҵ����� 
			{
				i--;//ָ������ 
				int nw=0;
				for (int j=pre;j<=i;j++)
					t[nw++]=S[j];
				t[nw]=0;
				int id=insert(t,-1,pre,i,1);//��ѯ�Ƿ����Ѿ����ֹ������ֳ��� 
				total++;//����token��val�е�ֵ�Ǹ����ֳ����ڷ��ű��λ�� 
				ans[total].l=pre;ans[total].r=i;ans[total].type=NUM;ans[total].val=id;
				status=BEGIN;//״̬��س�ʼ̬ 
			}
		}
		else if (status==OP)//Ҫƥ��һ�������
		{
			if ((S[i-1]=='='&&S[i]=='=') ||  (S[i-1]=='>'&&S[i]=='=') || (S[i-1]=='<'&&S[i]=='=') || (S[i-1]=='!'&&S[i]=='=')) 
			{//�ж��ǲ��������ַ�������� 
				total++;
				int nw=0;
				for (int j=pre;j<=i;j++)
					t[nw++]=S[j];
				t[nw]=0;
				int id=insert(t,-1,pre,i);//��ѯ�������Ӧ��val ������token�� 
				ans[total].l=pre;ans[total].r=i;ans[total].type=OP;ans[total].val=-id;//val�е�ֵ����������Ǹ���������������﷨������ 
				status=BEGIN;
			}
			else if (S[i]=='+'||S[i]=='-'||S[i]=='*'||S[i]=='/'||S[i]=='>'||S[i]=='<'||S[i]=='='||S[i]=='!')
			{//�ж��ǲ��ǲ��Ϸ��������ַ�������� 
				wrong[i]=1;//��Ч���ʣ�������������ַ� 
				printf("Compile Error on line %d at column %d : invalid operator\n",pos[i].first,pos[i].second);
				status=OP;
			}
			else
			{//������һ���ַ����������������ͬ�� 
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
		{//�Ƿָ�����Ҳ�ǰ������ķ������� 
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
	addKeyword();//���ֵ�������ӹؼ��ʡ����ŵ� 
	readCode();//������� 
	modifyCode();//�Դ�������޸ģ�ɾ��ע�� 
	lexicalAnalysis(); //�ʷ����� 
	showTable();//���token��ͷ��ű� 
}

