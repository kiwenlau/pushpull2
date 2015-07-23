//separate push, pull and total time, then the result can be easily copied to excel or world table

# include<stdio.h>
# include<stdlib.h>
# include<string.h>

int main()
{
	//print push, pull and total time into corresponding files
    system("cat output2/average.txt | grep push > push.txt");
    system("cat output2/average.txt | grep pull > pull.txt");
    system("cat output2/average.txt | grep total > total.txt");
    
    system("mkdir output3");
    
    //print push, pull and total time separately
    printtime();
    
    remove("push.txt");
    remove("pull.txt");
    remove("total.txt");
}

int printtime()
{
    FILE *fp1, *fp2;
    char files[3][20]={"push.txt", "pull.txt", "total.txt"};
    char steps[3][40]={"average push time: ", "average pull time:", "average total time:"};
    char str[50];
    int i;
    
    fp2=fopen(".//output3//result.txt", "wt");
    
    for (i=0; i<3; i++)
    {
        fp1=fopen(files[i], "rt");
        fprintf(fp2, "%s\n", steps[i]);
        while(fgets(str, 50, fp1))
        {
            fprintf(fp2, "%s", str+21);
        }
        fprintf(fp2, "\n");
        fclose(fp1);
    }
    
    fclose(fp2);
}