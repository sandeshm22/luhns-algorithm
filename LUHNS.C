#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<process.h>

#define LEN 50
#define MAX 50

struct word
{
	char s[LEN];
	int f;
}w[MAX];

int wCnt;
int ct,n,i,j,k,counter=0,cnt,cnt1,flag=0;
char str[20],str1[20],str2[20],ch,fname[20];

// high frequency words

char stop_list[40][30];

// suffixes
char suffix[10][5];
FILE *fp,*fp1;
char c;
char s[LEN];

int exist(char *s)
{
	int i;
	for(i=0;i<wCnt;i++)
	{
		if(!strcmp(s,w[i].s))
		{
		   w[i].f++;
		   return 1;
		}
	}
	return 0;
}


void show()
{
	int i;
	printf("\n\n\n Total number of distinct words = %d",wCnt);
	printf("\n Word \t Frequency");
	for(i=0;i<wCnt;i++)
	{
		printf("\n %s \t %d ",w[i].s,w[i].f);
	}
}

void freq(char *fname)
{
	fp=fopen(fname,"r");
	i=0;
	wCnt=0;
	printf("\n\n\n Documents contents... \n\n");
	while((c=fgetc(fp))!=EOF)
	{
		if(c==' ' || c=='\n' || c=='\r' || c=='.')
		{
			s[i]='\0';
			if(!exist(s))
			{
				strcpy(w[wCnt++].s,s);
				w[wCnt-1].f=1;
			}
			printf(" %s",s);
			i=0;
		}
		else
		   s[i++]=c;
	}
	show();
	fclose(fp);
	getch();
}


void freq1(char *fname)
{
	fp=fopen(fname,"r");
	i=0;
	wCnt=0;
	printf("\n\n\n Documents contents... \n\n");
	while((c=fgetc(fp))!=EOF)
	{
		if(c==' ' || c=='\n' || c=='\r' || c=='.'||c=='\0')
		{
			s[i]='\0';
			if(!exist(s))
			{
				strcpy(w[wCnt++].s,s);
				w[wCnt-1].f=1;
			}
			printf(" %s",s);
			i=0;
		}
		else
			s[i++]=c;
	}
	show();
	fclose(fp);
	getch();
}


void readword(FILE *fp)
{
	i=0;
	while(1)
	{
		ch=fgetc(fp);
		if(feof(fp))
			break;
		if(ch==' ')
			break;
		if(ch=='\n')
			break;
		str[i]=ch;
		i++;
	}
}

void initialise_array()
{
	char a[30];
	int count=0;
	FILE *fp2;
	char c;
	fp2=fopen("stopwords.txt","r");
	while((c=fgetc(fp2))!=EOF)
	{
	printf("%c",c);
	}
	getch();
	fclose(fp2);
   /*	while(fscanf(fp2,"%s",a))
	{
		strcpy(stop_list[count++],a);
	}*/
	cnt=count;
	count=0;
	fp2=fopen("suffix.txt","r");
	while(fscanf(fp2,"%s",&a))
	{
		strcpy(suffix[count++],a);
	}
	cnt1=count;
}

void main()
{

	FILE *fp2=fopen("stopwords.txt","r");
	clrscr();
	initialise_array();
	freq("ip.txt");
	fp=fopen("ip.txt","r");
	fp1=fopen("freq.dat","w+");

	if(fp==NULL)
	{
		printf("\nCannot read the doc...");
		getch();
		exit(0);
	}

	clrscr();

	/*
	 STEP 1: Removal of high frequency words
	*/
	printf("\n\nContent of the document after removal of high frequency words is : \n");
	while(1)
	{
		flag=0;
		if(feof(fp))
			break;
		readword(fp);
		str[i]='\0';

		//check with each word in stop list
		for(i=0;i<cnt;i++)
		if(strcmpi(stop_list[i],str)!=0)
			 flag++;
		if(flag==cnt)
		{
			fwrite(str,strlen(str)+1,1,fp1);
			fwrite("\n",1,1,fp1);
		}
	}
	rewind(fp1);
	freq("freq.dat");

	/*
	STEP 2: Suffix stripping
	*/
	fp=fopen("suffix.dat","w+");
	rewind(fp1);
	clrscr();
	printf("\nContent of the document after removal of suffixes is : \n");
	while(1)
	{
		flag=0;
		readword(fp1);
		if(feof(fp1))
			break;
		str[i]='\0';
		for(i=0;i<cnt1;i++)
		{
			ct=0,n=0;
			k=strlen(str)-strlen(suffix[i]);
			for(j=k;str[j]!='\0';j++)  //separating suffixes
			{
				str1[n]=str[j];
				n++;
			}
			str1[n]='\0';
			n=0;
			if(strcmp(str1,suffix[i])==0)
			{
				for(j=0;j<k;j++)    //index term
				{
					str2[n]=str[j];
					n++;
				}
				str2[n]='\0';
				if(str2[n-1]!='q' && str2[n-1]!='Q')
				{
					if(strlen(str2)>2)
					{
						fwrite(str2,strlen(str2)+1,1,fp);
						counter++;
						fwrite("\n",1,1,fp);
						ct=1;
						break;
					}
				}
			}
		}
		if(ct==0)
		{
			fwrite(str,strlen(str)+1,1,fp);
			counter++;
			fwrite("\n",1,1,fp);
		}
	}
	rewind(fp);
	freq("suffix.dat");

	/*
	STEP 3: Deleting equivalent stems/finding duplicate or s\imilar words
	*/

	fp=fopen("suffix.dat","r");
	fp1=fopen("dupl.dat","w+");
	rewind(fp);
	clrscr();
	printf("\nContent of the doc after removal of duplicates is:  \n");
	readword(fp);
	str[i]='\0';
	ct=0;
	fwrite(str,strlen(str)+1,1,fp1);
	ct++;
	while(1)
	{
		rewind(fp1);
		flag=0;
		readword(fp);
		str[i]='\0';
		if(feof(fp))
		break;
		flag=0;
		while(1)
		{
			if(feof(fp))
				break;
			i=0;
			while(1)
			{
				ch=fgetc(fp1);
				if(feof(fp1))
					goto I;
				if(ch==' ')
					break;
				if(ch=='\n')
					break;
				if(ch=='\x0')
					break;
				str1[i]=ch;
				i++;
			}
			str1[i]='\0';
			if(strcmpi(str,str1)!=0)
				flag++;
		}
		I:if(flag==ct)
		{
			 fwrite(str,strlen(str)+1,1,fp1);
			 ct++;
		}
	}
	rewind(fp1);
	fclose(fp);
	fclose(fp1);
	freq1("dupl.dat");
	getch();
}


