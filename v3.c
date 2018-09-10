/*
	C PROGRAM FOR OFFLINE DICTIONARY
	AUTHOR: SWAPNIL SAXENA
	COMPUTER SCIENCE AND ENGINEERING
	JAYPEE INSTITUTE OF INFORMATION TECHNOLOGY
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include<time.h>
#include<windows.h>
#define NEWLINE printf("\n") ;
#define AND &&
#define OR ||


// structure for node of dictionary linked list
struct key_val
{
	char key[40] ;
	long seekto ;
	struct key_val *next ;
	struct key_val *back ;
};
// we made a structure variable d_node and declared it as a user defined data type
typedef struct key_val d_node ;
// structure related pointer variables
d_node *element = NULL ;
d_node *prev = NULL;


// structure for node of syn_ant
struct node
{
	char *syn;
	char *ant;
	struct node *next;
};

// structure for time
struct performance
{
	clock_t start;
	clock_t end;
	double time_used;
};

// file pointer for dictionary
FILE *f = NULL;

// file pointer for syn_ant
FILE *g = NULL;

// reads the dictionary file line by line
char* fline();
// fline function variables listed here
char data[80], ch;

// reads the syn_ant file line by line
int gline();
// gline function variables listed here
char *line;

// to create the dictionary linked list
void initialize();
// initialize function variable listed here
char *tmpchr , *lastdef = "\0" ;
int number_of_keys = 0 ;

// to create the syn_ant linked list
struct node *ignite();

// return 1 if given string is all capital
int upper(char *ch);

// changes the given string to all capital
void to_upper(char *a);

// creates a node for syn_ant linked list
void make_node(struct node **ref);

// searches for child as a substring in the parent string
char *sub_srch(char *parent, char *child);

// reads the definition given a file pointer
void defread (double where);

// the function returns 0 if input is zero else changes the input to all capital
int input_cap();

//searches for the meaning of the word in dictionary
void meaning(struct node *ptr);

// searches for syn
void syn_meaning(struct node *ptr);

// searches for ant
void ant_meaning(struct node *ptr);

// returns the command to be executed
int command();

// lists all syn_ant data
void show(struct node *a);

// lists all dictionary data
void show_dic();

// deallocates all memory
void all_clear(struct node *a);

// print instructions
void instructions();

// structure to save timing
struct performance stats;

// input variable
char user[40];

// function pointer
void (*fun_ptr)(struct node*);

void main()
{
	system("color E3");

	int i = 0;

	int to_do = 0;

	f = fopen("final_dict.txt","r");

	g = fopen("ant_syn.txt","r");

	// initialise function pointer
	fun_ptr = &(meaning);

	// intial code for dictionary

    d_node *head ;

    head = (d_node *)malloc(sizeof(d_node)) ;

    // we saved the head address in the global structure d_node pointer variable element
    element = head ;

    // the initialize function would import the data from the file to the linked list
    initialize() ;

    element = head ;

	// initial code for syn_ant

	struct node *start = (struct node*)calloc(sizeof(struct node),1);

	// save the address of start
	start = ignite();
	NEWLINE
	NEWLINE
	printf("PRESS ANY KEY TO CONTINUE...\n");
	//get delay
	getch();

	//clear console
	system("cls");

	//show(start);

	instructions();
	getch();

    while(1)
    {
    	//label
    	come_again:

    	// clear console
    	system("cls");

    	printf("INPUT: ");
    	scanf("%s",user);

    	// change to capital
    	input_cap();

    	// clear console
    	system("cls");

    	to_do = command();

    	switch(to_do)
	    {
	    	case 0:
	    	// clear the console
		    	system("cls") ;
		    	element = head;
		    	all_clear(start);
		    	printf("MEMORY DEALLOCATED SUCCESSFULLY !!!");
		    	printf("\nPRESS ANY KEY TO CONTINUE...\n");
		    	// get delay
		    	getch();
		    	// clear the console
		    	system("cls") ;
		    	fclose(f);
		    	fclose(g);
		    	printf("BOTH FILES CLOSED SUCCESSFULLY !!!\n");
		    	printf("\nPRESS ANY KEY TO CONTINUE...\n");
		    	// get delay
		    	getch();
		    	system("cls") ;
		    	printf("PROGRAM TERMINATED SUCCESSFULLY !!!\n\n");
		    	printf("\nPRESS ANY KEY TO CONTINUE...\n");
		    	// get delay
		    	getch();
		    	exit(0);

	    	case 1:
		    	meaning(NULL);
		    	// reset pointer
		    	element = head;
		    	goto come_again;
		    	break;

	    	case 2:
		    	syn_meaning(start);
		    	goto come_again;
		    	break;

	    	case 3:
		    	ant_meaning(start);
		    	goto come_again;
		    	break;

		    case 4:
		    	// it means mode has been changed hence take the input again
		    	goto come_again;
		    	break;

		    case 5:
		    	// list all words
		    	// reset element
		    	element = head;
		    	show_dic();
		    	printf("\n\nPRESS ANY KEY TO CONTINUE !!!\n");
		    	getch();
		    	system("cls");
		    	goto come_again;
		    	break;

		    case 6:
		    	// list all syn_ant
		    	show(start);
		    	printf("\n\nPRESS ANY KEY TO CONTINUE !!!\n");
		    	getch();
		    	system("cls");
		    	goto come_again;
		    	break;
		    default:
			    //pass
		    	break;
	    }
	}
}

char *fline()
{
    int i ;

	for (i=0;i<80;i++)
	{
		data[i]='\0' ;
	}

	i = 0 ;

	do
	{
		ch = getc(f);
		if(ch=='\n')
		{
			break ;
		}
		data[i] = ch;
		i++ ;
	}
	while( ( i < 80 ) AND ( ch != EOF ) AND ( ch != '\n') ) ;
	// i < 80 in do while loop means 0 to 79 (not 80) since condition evaluated at last hence last charachter always null
	return data ; // the return string pointer wont have charachter such as \n
}

int gline()
{
	long start_line = ftell(g); // stores the start of each line

	int i = 0; //i would serve as counter
	int l = 0; // saves the lenght
	char ch = getc(g); // read charachter by charachter

	while(1)
	{
		if (ch == '\n')
		{
			break;
		}
		if (ch == EOF)
		{
			break;
		}
		i++;
		ch = getc(g);
	}

	l = i ; //save the lenght

	fseek(g,start_line,0); // reset pointer to start of current line

	line = (char*)calloc(sizeof(char),l+1); // allocate memory, 1 extra bit for null charachter

	i = 0; //reset counter

	ch = getc(g); // start fetching charachters again

	while(( i < l ) AND ( ch != EOF ))
	{
		if((ch >= 97) AND (ch <= 122))
		{
			// convert lower to upper case
			ch = ch-32;
		}
		line[i] = ch;
		ch = getc(g);
		i++;
	}

	line[i] = '\0'; // set null

	return l;
}

void initialize ()
{
	//initialize clock
	stats.start = clock();
	// feof is true when end of file is reached
	// while loops until true
	// hence not-feof means condition would be false when end of file is reached
	tmpchr = fline() ;
	while ( !(feof(f)) )
	{
		if ( upper(tmpchr) == 1 )
		{
			int i = 0 ; // counter for element key
			int j = 0 ; // counter for tmpchr
			// copying word charachter by charachter
			while( ( *(tmpchr+j) != '\0' ) AND ( j < 80) AND ( (tmpchr+j) != NULL ) AND ( *(tmpchr+j) != ';') AND ( i < 40 ) )
			{
				if (  ( *(tmpchr+j) == ' ') OR ( *(tmpchr+j) == '-') OR ( *(tmpchr+j) == '_') OR ( *(tmpchr+j) == '.') OR
					 ( *(tmpchr+j) == '\'') )
				{
					j++ ;
					continue ;
				}
				else if ( 65 <= *(tmpchr+j) <= 90 )
				{
					(element->key)[i] = *(tmpchr+j) ;
					i++ ;
					j++ ;
				}
				else
				{
					break ;
				}
			}
			// sets the last element as null pointer
			if(i==40)
			{
				i--;
			}
			(element->key)[i] = '\0' ;
			// if it is a duplicate then we would skip to next word
			if ((strcmp(lastdef,element->key)) == 0 )
			{
				// strcmp returns 0 is strings are equal
				// we continue the loop
				// increase the file pointer to next line
				tmpchr = fline(f) ;
				// for next iteration
				continue ;
			}
			else
			{
				// this else code is run when c encounters the first distinct copy of  a word
				// copy current key address to lastdef for later checking
				lastdef = (element->key) ;
				number_of_keys += 1 ;
				//printf("%s",element->key) ;
				//NEWLINE
			}
			{
				// now the file pointer points to defintion part so we save the location
				element->seekto = ftell(f) ;
				// create next d_node
				element->next = (d_node *)malloc(sizeof(d_node)) ;
				// assign previous d_node address to structure pointer back
				element->back = prev ;
				// save current d_node address in global structure pointer element prev to be used in next element for previous pointter
				prev = element ;
				// assign address of next element to gloabl structure d_node variable element
				element = (element->next) ;
				// increase file pointer to next line and save in gloabl char pointer variable tmpchr
				tmpchr = fline(f) ;
			}
		}
		else
		{
			while(!(upper(tmpchr)))
			{
				// we want the loop to end as soon as upper(ptr) returns 1 i.e true
				if(feof(f))
                {
                	// free the memory allocated for last most pointer defintion
                	free(element->next) ;
                	// set last element next pointer to null
                	element->next = NULL ;
                    printf("DICTIONARY INFORMATION");
                    printf("\nIMPORT COMPLETED SUCCESSFULLY");
                    NEWLINE

                    stats.end = clock();
                    stats.time_used = ((double)(stats.end - stats.start)) / CLOCKS_PER_SEC ;

                    printf("\nTIME TAKEN TO IMPORT DATA = %lf SECONDS",stats.time_used);
                    printf("\nNUMBER OF WORDS IMPORTED = %d",number_of_keys);
                    printf("\nAVERAGE SPEED RECORDED = %f WORDS PER SECOND",(number_of_keys/stats.time_used));
                    printf("\nTIME NEEDED TO IMPORT ONE WORD = %f SECONDS\n\n",(stats.time_used/number_of_keys));
                    //system("cls") ;
                    return ;
                }
				tmpchr = fline(f) ;
			}
		}
	}
}

struct node *ignite()
{
	// initialise clock
	stats.start = clock();

	int i = 0;

	struct node *head;
	head = (struct node*)calloc(sizeof(struct node),1);
	struct node *ptr;
	ptr = head;
	while(!feof(g))
	{
		make_node(&ptr);
		i++;
		ptr = ptr->next;
		if(ptr->syn)
		{
			//printf("SYN : %s\n",ptr->syn);
		}
		if(ptr->ant)
		{
			//printf("ANT : %s\n",ptr->ant);
		}
		//printf("NODE CREATED !!!\n\n");

	}
	// end the linked list
	ptr->next = NULL;
	NEWLINE
	NEWLINE
	printf("SYNONYMS AND ANTONYMS INFORMATION");
    printf("\nIMPORT COMPLETED SUCCESSFULLY");
    NEWLINE
    //system("cls") ;
    stats.end = clock();
    stats.time_used = ((double)(stats.end - stats.start)) / CLOCKS_PER_SEC ;

    printf("\nTIME TAKEN TO IMPORT DATA = %lf SECONDS",stats.time_used);
    printf("\nNUMBER OF ENTITIES IMPORTED = %d",i);
    printf("\nAVERAGE SPEED RECORDED = %f ENTITIES PER SECOND",(i/stats.time_used));
    printf("\nTIME NEEDED TO IMPORT ONE WORD = %f SECONDS\n\n",(stats.time_used/i));
	return (head->next) ;
}

int upper(char *ch)
{
	int i =0 ; // pointer counter
	if (*ch == NULL OR *ch == EOF)
	{
		return 0 ;
	}
	while( *(ch+i) != '\0' AND i<80 )
	// we are pretty sure that our charachter string pointer will never point to \n charachter and end at \0
	{
		if ( ((int)(*(ch+i)) <= 122 AND (int)(*(ch+i)) >= 97) OR ((int)(*(ch+i)) <= 57 AND (int)(*(ch+i)) >= 48))
            // ASCII code range of small letters and numbers
		{
			return 0 ;
		}
		else
		{
			i++ ; // 0 means false i.e presence of any other letter means charachter isnt fully capital
		}
	}

	return 1 ; // return 1 means true that charachter string is fully capital
}

void to_upper(char *arr)
{
	while( *(arr) != '\0' )
	{
		if( ( (*arr) >= 97 ) AND ( (*arr) <= 122 ) )
		{
			*arr = (*arr)-32;
		}
		arr++;
	}
	return;
}

void make_node(struct node **ref)
{
	struct node *ptr = *ref;

	ptr->next = (struct node*)calloc(sizeof(struct node),1); // ptr is a node that already exists

	ptr = ptr->next; // ptr now points to the new node

	// initialise
	ptr->syn = NULL;
	ptr->ant = NULL;

	int l = 0; // lenght of line
	int i = 0; //counter
	int key_size = 0;
	char key[30];

	while(1)
	{
		l = gline(); // read next line

		if ( l == 0 )
		{
			// when nothing or newline is encountered
			// l = 0 therefore coninue
			continue;
		}
		else if ( l == 1 )
		{
			// if l = 1 then it means we encountered an equal to sign
			return;
		}
		else
		{
			// do nothing
		}

		// NEW IF ELSE CONDITIONS

		if ((line[0] == 'K') AND (line[1] == 'E') AND (line[2] == 'Y'))
		{
			//printf("\n->IN KEY\n");
			key_size = l ;
			if (key_size > 30)
			{
				key_size = 30;
			}
			// initialise the key array
			for(i=0;i<30;i++)
			{
				key[i] = '\0';
			}
			// as we have to read from 6th charachter
			i = 5 ;
			while(line[i] != '\0')
			{
				if(((line[i] >= 65) AND (line[i] <= 90)) OR ((line[i] >= 65) AND (line[i] <= 90))
					OR (line[i] == ' ') OR (line[i]== ':'))
				{
					key[i-5] = line[i];
					i++;
				}
				else
				{
					break;
				}
			}
			//set null
			key[i-5] = '\0';
			//printf("\n\n ->KEY CREATED !!!\n\n");
		}
		else if ((line[0] == 'S') AND (line[1] == 'Y') AND (line[2] == 'N'))
		{
			ptr->syn = (char*)calloc(sizeof(char),l+key_size+5	); // the l+key_size is more than enough for all operations
			// initialise the array
			for(i=0;i<(l+key_size);i++)
			{
				(ptr->syn)[i] = '\0';
			}
			// copy all elements starting from 5 th character i.e include the space
			i = 4;
			while(1)
			{
				if ((line[i] == '\0') OR (line[i] == '.'))
				{
					break;
				}
				(ptr->syn)[i-4] = line[i];
				i++;
			}
			//insert null again
			(ptr->syn)[i-4] = ',';
			i++;
			(ptr->syn)[i-4] = ' ';
			i++;
			// elements copied
			// new copy elements from key by
			for(int k = 0 ; k <(key_size-4);)
			{
				(ptr->syn)[i-4] = key[k];
				i++;
				k++;
			}
			//insert full stop
			(ptr->syn)[i-4] = '.';
			i++;
			//insert null again
			(ptr->syn)[i-4] = '\0';
		}
 		else if ((line[0] == 'A') AND (line[1] == 'N') AND (line[2] == 'T'))
		{
			ptr->ant = (char*)calloc(sizeof(char),l+5);
			// initialise the array
			for(i=0;i<(l+1);i++)
			{
				(ptr->ant)[i] = '\0';
			}
			// copy all elements starting from 5 th character i.e include the space
			i = 4;
			while(1)
			{
				if (line[i] == '\0')
				{
					break;
				}
				(ptr->ant)[i-4] = line[i];
				i++;
			}
			//insert null again
			(ptr->ant)[i] = '\0';
			// elements copied
		}
		else
		{
			continue;
		}
	}
}

char *sub_srch(char *str, char *sub)
{
	char *p1, *p2, *p3;
	int i=0,j=0,flag=0;

	//initialise the char pointers to the arguments
	p1 = str;
	p2 = sub;

	if((p1 == NULL) OR (p2 == NULL))
	{
		// if any of the pointer is null
		return NULL;
	}

	//printf("%s\n\n",str);
	for(i = 0; i<strlen(str); i++)
	{
		if(*p1 == *p2)
		{
			p3 = p1;
			for(j = 0;j<strlen(sub);j++)
			{
				if(*p3 == *p2)
				{
				  p3++;p2++;
				}
				else
				{
				  break;
				}
			}
			p2 = sub;
			if(j == strlen(sub))
			{
				if(((*p3==',') OR (*p3=='.')) AND (*(p3-j-1)==' '))
				{
					// to check that the substring is a word
					flag = 1;
					return str;
				}
			}
		}
		p1++;
	}
	if (flag == 0)
	{
		return NULL;
	}
	else
	{
		return str;
	}
}

void defread (double where)
{
	fseek(f,where,0) ;
	tmpchr = fline(f) ;
	while( (upper(tmpchr) == 0) AND (!(feof(f))) )
	{
		printf("%s",tmpchr );
		NEWLINE
		// increase file pointer
		tmpchr = fline(f) ;
	}
}

int input_cap()
{
    int i = 0 ;
    char *s = user ;
    while( ( i < 40 ) AND ( *( s + i ) != '\0') )
    {
        if ( ( *( s + i ) >= 97 ) AND ( *( s + i ) <= 122 ) )
        {
            *( s + i ) -= 32 ;
        }
        else if ( ( *( s + i ) >= 65 ) AND ( *( s + i ) <= 90 ) )
        {
            // continue
        }
        else

        {
            return 0 ;
        }

        i++ ;
    }
    return 1 ;
}

void meaning(struct node *ptr)
{
	stats.start = clock();
	// the argument does nothing but brings uniformity about the functions
	int i = 0;
	// the following loop checks all the words
    for( i = 0; i < number_of_keys ; i++ )
    {
        if(strcmp(user,element->key)==0)
        {
        	stats.end = clock();
            system("cls") ;
            printf("%s\n\n",user);
            NEWLINE
            // to print out the definition
            defread(element->seekto) ;
            stats.time_used = ((double)(stats.end - stats.start)) / CLOCKS_PER_SEC ;
            printf("\n\n\nTIME TAKEN TO COMPLETE QUERY = %lf",stats.time_used);
            printf("\n\nPress any key to continue...");
            // to create delay
            getch() ;
            break ;
        }
        else
        {
            // increase element  pointer
        	element = element->next ;
        }
    }
    stats.end = clock();
    // if he loop runs fully w/o break then the following condition is true
    // this happens only when word is not found
    if( i == number_of_keys )
    {
    	stats.time_used = ((double)(stats.end - stats.start)) / CLOCKS_PER_SEC ;
        printf("\n\n\nTIME TAKEN TO COMPLETE QUERY = %lf",stats.time_used);
    	printf("\n\nCould not find the word");
    	printf("\n\nPress any key to continue...");
        getch() ;
    }
}

void syn_meaning(struct node *ptr)
{
	stats.start = clock();

	int flag = 0;

	while(ptr != NULL)
	{
		char *result;

		result = sub_srch(ptr->syn,user);
		if (result != NULL)
		{
			printf("%s ",result);
			flag ++;
			//break;
		}


		result = sub_srch(ptr->ant,user);
		if (result != NULL)
		{
			flag++;
			printf("%s ",result);
			//break;
		}


		ptr = ptr->next;
	}

	stats.end = clock();
	stats.time_used = ((double)(stats.end - stats.start)) / CLOCKS_PER_SEC ;
    printf("\n\n\nTIME TAKEN TO COMPLETE QUERY = %lf",stats.time_used);
	// create delay
	if(flag==0)
	{
    	printf("\n\nCOULD NOT FIND THE WORD");
	}
	printf("\n\n\nPRESS ANY KEY TO CONTINUE...");
	getch();
	system("cls");
}

void ant_meaning(struct node *ptr)
{
	stats.start = clock();

	int flag = 0;

	while(ptr != NULL)
	{
		char *result;

		result = sub_srch(ptr->syn,user);
		if (result != NULL)
		{
			flag++;
			printf("%s ",ptr->ant);
			//break;
		}


		result = sub_srch(ptr->ant,user);
		if (result != NULL)
		{
			flag++;
			printf("%s ",ptr->syn);
			//break;
		}


		ptr = ptr->next;
	}
	stats.end = clock();
	stats.time_used = ((double)(stats.end - stats.start)) / CLOCKS_PER_SEC ;
    printf("\n\n\nTIME TAKEN TO COMPLETE QUERY = %lf",stats.time_used);
    if(flag==0)
    {
    	printf("\n\nCOULD NOT FIND THE WORD");
    }
	// create delay
	printf(".\n\n\nPRESS ANY KEY TO CONTINUE...");
	getch();
	system("cls");
}

int command()
{
	if (user[0] == '0')
	{
		return 0;
	}
	else if ((user[0] == '-') AND (user[1] == '>') AND (user[2] == 'D') AND (user[3] == 'I') AND (user[4] == 'C') AND (user[5] == '\0'))
	{
		fun_ptr = &(meaning);
		printf("DICTIONARY MODE INITIATED !!!\n");
		printf("PRESS ANY KEY TO CONTINUE...\n");
		//create delay
		getch();
		system("cls");
		return 4;
	}
	else if ((user[0] == '-') AND (user[1] == '>') AND (user[2] == 'S') AND (user[3] == 'Y') AND (user[4] == 'N') AND (user[5] == '\0'))
	{
		fun_ptr = &(syn_meaning);
		printf("SYNONYM MODE INITIATED !!!\n");
		printf("PRESS ANY KEY TO CONTINUE...\n");
		//create delay
		getch();
		system("cls");
		return 4;
	}
	else if ((user[0] == '-') AND (user[1] == '>') AND (user[2] == 'A') AND (user[3] == 'N') AND (user[4] == 'T') AND (user[5] == '\0'))
	{
		fun_ptr = &(ant_meaning);
		printf("ANTONYM MODE INITIATED !!!\n");
		printf("PRESS ANY KEY TO CONTINUE...\n");
		//create delay
		getch();
		system("cls");
		return 4;
	}
	else if ((user[0] == '-') AND (user[1] == '>') AND (user[2] == 'W') AND (user[3] == 'R') AND (user[4] == 'D') AND (user[5] == '\0'))
	{
		fun_ptr = &(ant_meaning);
		printf("ABOUT TO LIST ALL THE WORDS OF DICTIONARY !!!\n");
		printf("PRESS ANY KEY TO CONTINUE...\n");
		//create delay
		getch();
		system("cls");
		return 5;
	}
	else if ((user[0] == '-') AND (user[1] == '>') AND (user[2] =='A') AND (user[3] == 'S') AND (user[4] == 'A') AND (user[5] == '\0'))
	{
		fun_ptr = &(ant_meaning);
		printf("ABOUT TO LIST ALL THE SYNONYMS AND ANTONYMS !!!\n");
		printf("PRESS ANY KEY TO CONTINUE...\n");
		//create delay
		getch();
		system("cls");
		return 6;
	}
	else if ((user[0] == '-') AND (user[1] == '>'))
	{
		printf("UNKNOWN COMMAND !!!\n");
		printf("PRESS ANY KEY TO CONTINUE...\n");
		//create delay
		getch();
		system("cls");
		return 4;
	}
	else
	{
		if (fun_ptr == &(meaning))
		{
			return 1;
		}
		else if (fun_ptr == &(syn_meaning))
		{
			return 2;
		}
		else if (fun_ptr == &(ant_meaning))
		{
			return 3;
		}
		else
		{
			return 1;
		}
	}
}

void show(struct node *ptr)
{
	stats.start = clock();
	while(ptr != NULL)
	{
		printf("SYN : %s\n",ptr->syn);
		printf("ANT : %s\n",ptr->ant);
		printf("\n\n");
		ptr = ptr->next;
	}
	stats.end = clock();
	stats.time_used = ((double)(stats.end - stats.start)) / CLOCKS_PER_SEC ;
    printf("\n\n\nTIME TAKEN TO COMPLETE QUERY = %lf",stats.time_used);
}

void show_dic()
{
	stats.start = clock();
	while(1)
	{
		if(element->next)
		{
			printf("%s\n",element->key);
			//defread(element->seekto);
			element = element->next;
		}
		else
		{
			break;
		}
	}
	stats.end = clock();
	stats.time_used = ((double)(stats.end - stats.start)) / CLOCKS_PER_SEC ;
    printf("\n\n\nTIME TAKEN TO COMPLETE QUERY = %lf",stats.time_used);
}

void all_clear(struct node *start)
{
	// free dic
	struct key_val *temp,*ptr;
    temp = element;
    while(temp->next != NULL)
    {
        ptr=temp;
        temp=temp->next;
        free(ptr);
    }
    free(temp);
    // free syn_ant
    struct node *cur, *pehla;
    cur = start;
    while(cur->next != NULL)
    {
        pehla = cur;
        cur=cur->next;
        free(pehla);
    }
    free(cur);
}

void instructions()
{
	printf("INSTRUCTIONS\n\n");
	printf("AVAILABLE COMMANDS:\n");
	printf("1. \"->DIC\" : INITIATE DICTIONARY MODE\n");
	printf("2. \"->SYN\" : INITIATE SYNONYM MODE\n");
	printf("3. \"->ANT\" : INITIATE ANTONYM MODE\n");
	printf("4. \"->WRD\" : LIST ALL DICTIONARY WORDS\n");
	printf("5. \"->ASA\" : LIST ALL SYNONYMS AND ANTONYMS\n");
	printf("6. \"0\" : EXIT THE PROGRAM\n");
	printf("\n\nPRESS ANY KEY TO CONTINUE...\n");
}