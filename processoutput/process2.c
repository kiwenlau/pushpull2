// print average push, pull and total time of all image into a single file (in the order of imagelist, that is, in acending order by image size)

# include<stdio.h>
# include<stdlib.h>
# include<string.h>

int main()
{
    FILE *fp;
    char inputfile[50];
    int len;
    fp=fopen("imagelist.txt", "rt");
    mkdir("output2");
    system("cp output1/* .");
    while (fgets(inputfile, 50, fp))
    {
        len=strlen(inputfile);
        inputfile[len-1]='\0';
        strcat(inputfile, ".txt");
        processfile(inputfile);
        remove(inputfile);
    }
    fclose(fp);
}

int processfile(char *inputfile)
{
    
    
    char cmd[50];
    char imagename[20];
    int len;
    
    len=strlen(inputfile);
    strcpy(imagename, inputfile);
    imagename[len-4]='\0';
    
    //print imafe name into average.txt
    strcpy (cmd, "echo ");
    strcat (cmd, imagename);
    strcat (cmd, " >> output2/average.txt");
    system (cmd);
    
    // change the name of input file
    rename(inputfile, "tempfile.txt");
    
    //get the average save, transfer, load and total time and print into average.txt
    system("cat tempfile.txt | grep average >> output2/average.txt");
    system("echo "" >> output2/average.txt");
    rename("tempfile.txt", inputfile);
}

