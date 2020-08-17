#include <stdlib.h>  //standard library definitions
#include <stdio.h>   //standard buffered input/output
#include <ctype.h>   //character types
#include <string.h>  //string operations
#include <strings.h> //string operations
#include <math.h>    //mathematical declarations
#include <time.h>    //time types
#include <complex.h> //complex arithmetic
#include <assert.h>  //verify program assertion
#include <limits.h>
#include <stdio.h>

#define Mlin 100 //Max no of characters in single line
typedef struct Line_type
{
    char point1[10];       
    double Relaibility_value;
    char point2[10];
} LINE;

int ReadFile(FILE *txt, LINE *Node)
{
    char begin_point[Mlin], end_point[Mlin], d[Mlin];
    int ch,  index;
    char lines[Mlin];
    char c;
    int i= 0;

    while (!feof(txt))
    {   
        fgets(lines, Mlin, txt);
        sscanf(lines," %s %s %s", begin_point, d, end_point);

        strcpy(Node[i].point1, begin_point);
        double value= atof(d);
        Node[i].Relaibility_value=value;
        strcpy(Node[i].point2, end_point);
        i++;
    }
    return i;
}

void calculate_reliability(LINE *Node, int total_lines)
{
    int i=0,j=1, count=0, t,l, flag,a;
    double Rparallel;
    int  N_parallel_lines=0, N_series_lines=0;
    double Rseries;
    
    char *ptr;
    while(1)
    {
        double check_r=0;
        for(i=0; i< total_lines; i++)
        {
            for(j=i+1; j< total_lines; j++)
            {   
                if(Node[i].Relaibility_value != 0 && Node[j].Relaibility_value != 0)
                {
                    if(strcmp(Node[i].point2, Node[j].point1)==0 && strcmp(Node[i].point1,Node[j].point2)!=0)
                    {   
                        ptr=Node[i].point2;
                        flag=0;
                        for(l=0; l<total_lines; l++)
                        {   
                            if(l != i && l!= j)
                            {
                                if (strcmp(Node[l].point1,ptr)==0 || strcmp(Node[l].point2,ptr)==0)
                                {   
                                    flag =1;
                                    break;
                                }
                            }
                        }
                        if (flag==0)
                        {
                            Rseries= Node[i].Relaibility_value * Node[j].Relaibility_value;
                            //printf("The reliability is %f \n",Rseries);
                            Node[i].Relaibility_value= Rseries;
                            strcpy(Node[i].point2,Node[j].point2);  
                            Node[j].Relaibility_value=0;Node[j].point1[0]= '\0';Node[j].point2[0]=  '\0';     
                        }
                        
                    }
                    
                }
            }
                
        }
        
        
        
        for(i=0; i< total_lines-1; i++)
        {
            for(j=i+1; j< total_lines; j++)
            {
                if(Node[i].Relaibility_value != 0 && Node[j].Relaibility_value != 0)
                {
                    if((strcmp(Node[i].point1,Node[j].point1)==0 && strcmp(Node[i].point2,Node[j].point2 )==0 ) || (strcmp(Node[i].point1, Node[j].point2)==0 && strcmp(Node[i].point2,Node[j].point1)==0) )
                    {

                        Rparallel=Node[i].Relaibility_value+Node[j].Relaibility_value-Node[i].Relaibility_value * Node[j].Relaibility_value;
                        //printf("Parallel value is %f \n \n", Rparallel);
                        Node[i].Relaibility_value= Rparallel;
                        Node[j].Relaibility_value=0;Node[j].point1[0]= '\0';Node[j].point2[0]=  '\0';
                    }
                }           
            }
        }
        
        for(t=0; t<total_lines; t++)
        {
            check_r= check_r + Node[t].Relaibility_value;
        }
        
        if(check_r == Node[0].Relaibility_value)
        {
            break;
        }
    
    }
    

}



int main(int argc, char **argv)
{
    FILE *Reliability_file;
    LINE *Node;

    int total_lines=0;
    int a;
    //LINE Node[];
    /*********************** Read Text file and store in data structure **********/

    Reliability_file= fopen(argv[1],"r");
    Node = (LINE *) malloc (1000* sizeof(LINE));
    total_lines= ReadFile(Reliability_file, Node); 
    
    printf("%d \n",total_lines);
    
    printf("total Line %d \n",total_lines);
    for(a=0; a<total_lines; a++){ printf("%d  \t %s  \t %f  \t %s  \n",a,Node[a].point1, Node[a].Relaibility_value, Node[a].point2);}
    fclose(Reliability_file);
    calculate_reliability(Node, total_lines);
    printf("The total reliability of the network is %f \n", Node[0].Relaibility_value);
    return 0;
    
}