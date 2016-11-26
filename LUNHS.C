
#include<stdio.h>
#include<conio.h>
#include<string.h>

typedef struct word
{
   char sym[20];
   int cnt;
   struct word *next;
}word;

void count(word *);
int search(char *,word *);
void display(word *);
int search_stop_words(char *,char [][10]);

void main()
{
	word *head=NULL;
	clrscr();
	count(head);

   getch();
}


void count(word *head)
{
	int i=0,flag,flag1;
	FILE *fp;
	char *temp="",c,c1,SW[51][10]={"not","with","in","about","above","had","will","shall","should","with","and","or","as","a","an","the","which","why","what","is","was","were","if","it","else","by","to","too","then","they","their","there","i","me","we","he","she","of","off","am","do","can","done","are","be","all","them","that","into","used","you"};
	word *w,*p;

	fp=fopen("sample.txt","r");

	while(!feof(fp))
	 {
		c=fgetc(fp);
		if(c!=' ' && c!=',' && c!=';' && c!='.' && c!='!' && c!='?' && c!='\n')
		 {
		   if(c=='-')
			{
			   c1=c;
			   c=fgetc(fp);
			   if(c=='\n')
				 continue;

			   else
			   {
				temp[i++]=c1;
				temp[i]='\0';
			   }
			}
		   temp[i++]=c;
		   temp[i]='\0';
		 }
		else
		 {
		   temp[i]='\0';
		   flag1=search_stop_words(temp,SW);
		   if(flag1==1)
			{
			  i=0;
			  continue;
			}

		   flag=search(temp,head);

		   if(flag==0)
			{
			  w=(word *)malloc(sizeof(word));
			  if(head==NULL)
			   {
				 head=w;
				 strcpy(head->sym,temp);
				 head->cnt=1;
				 head->next=NULL;
			   }
			  else
			   {
				 strcpy(w->sym,temp);
				 w->cnt=1;
				 w->next=NULL;
				 p=head;
				 while(p->next!=NULL)
				  p=p->next;

				 p->next=w;
			   }
			}
		   i=0;
		 }
	 }
	display(head);
}

int search(char *temp,word *head)
{
   word *p;

   p=head;

   while(p!=NULL)
	{
	   if(strcmpi(temp,p->sym)==0)
		{
		  p->cnt++;
		  return 1;
		}
	   else
		 p=p->next;
	}

   return 0;
}

void display(word *head)
{
	word *p;
	flushall();
	fflush(stdout);
	p=head;
	while(p!=NULL)
	 {
		printf("%s\t%d\n\t",p->sym,p->cnt);
		p=p->next;
		getch();
	 }

}

int search_stop_words(char *temp,char SW[][10])
{
	 int i;

	 for(i=0;i<51;i++)
	  {
		 if(strcmpi(temp,SW[i])==0)
		  return(1);
	  }
   return 0;
}