//process the output of benchmark.sh

// get the push and pull time from the output of benchmark.sh
// compute the average push, pull and total time (omit the data of 2 largest and 2 smallest total time)
// print push, pull time and their average for each image

// program input 1: imagelist.txt, which list all tested docker images in acending order by image size
// program input 2: output0, which is a directory contains all output of the benchmark.sh

# include<stdio.h>
# include<stdlib.h>
# include<string.h>

# define N 10

int main()
{
    FILE *fp;
    char inputfile[50];
    int len;
    fp=fopen("imagelist.txt", "rt");
    mkdir("output1");
    system("cp output0/* .");
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
    FILE *fp1, *fp2;
    
    char outputfile[30];
    
    // change the name of input file
    rename(inputfile, "tempfile1.txt");
    
    // get push and pull time from output of run.sh
    system("cat tempfile1.txt | grep real >> tempfile2.txt");
    
    fp1=fopen("tempfile2.txt", "rt");
    fp2=fopen(".\\output1\\tempfile3.txt", "wt");
    
    proceessoutput(fp1, fp2);
    
    fclose(fp1);
    fclose(fp2);
    
    rename("tempfile1.txt", inputfile);
    remove("tempfile2.txt");
    
    strcpy (outputfile, ".\\output1\\");
    strcat (outputfile, inputfile);
    
    rename(".\\output1\\tempfile3.txt", outputfile);
}

int proceessoutput(FILE *fp1, FILE *fp2)
{
    int i, j, k, a, b;
    char buf[50], t[2*N][12];
    float real[2*N], push[N], pull[N], total[N], sum, d, e, f;
    int trim[4];
    
    // get the time from tempfile2.txt (string)
    k=0;
    while (fgets(buf, 50, fp1)&&k<2*N)
        strcpy(t[k++], buf+6);
    
    // transfer time from string to float
    for (i=0; i<2*N; i++)
        real[i]=atof(t[i]);
    
    a=0;
    b=0;
    for (i=0; i<2*N; i++)
    {
        if (i%2==0)
            push[a++]=real[i];
        else
            pull[b++]=real[i];
    }
    
    for (i=0; i<N; i++)
        total[i]=push[i]+pull[i];
    
    maximum2(total, trim);
    minimum2(total, trim);
    
    fprintf(fp2, "Omitted test data:\n");
    for (i=0; i<4; i++)
    {
        j=trim[i];
        fprintf(fp2, "Test %2d      push: %.2f, pull: %.2f, total: %.2f\n", j+1, push[j], pull[j], total[j]);
        push[j]=pull[j]=total[j]=0;
    }
    
    
    
    // compute the average of push time
    fprintf(fp2, "\ndocker push time:\n");
    sum=0;
    for (i=0; i<N; i++)
    {
        sum+=push[i];
        if(push[i]==0)
            fprintf(fp2, "\n");
        else
            fprintf(fp2, "%.2f\n", push[i]);
    }
    d=sum/(N-4);

    
    // compute the average of pull time
    fprintf(fp2, "\ndocker pull time:\n");
    sum=0;
    for (i=0; i<N; i++)
    {
        sum+=pull[i];
        if(pull[i]==0)
            fprintf(fp2, "\n");
        else
            fprintf(fp2, "%.2f\n", pull[i]);
    }
    e=sum/(N-4);
    
    // compute the average of total time
    fprintf(fp2, "\ndocker total time:\n");
    sum=0;
    for (i=0; i<N; i++)
    {
        sum+=total[i];
        if(total[i]==0)
            fprintf(fp2, "\n");
        else
            fprintf(fp2, "%.2f\n", total[i]);
    }
    f=sum/(N-4);
    
    fprintf(fp2, "\n");
    fprintf(fp2, "average push time:   %.2f\n", d);
    fprintf(fp2, "average pull time:   %.2f\n", e);
    fprintf(fp2, "average total time:  %.2f\n", f);
}

int maximum2(float *total, int *trim)
{
    int i, max1, max2;
    float temp;
    
    max1=0;
    for (i=1; i<N; i++)
    {
        if(total[i]>total[max1])
            max1=i;
    }
    
    temp=total[max1];
    total[max1]=total[0];
    total[0]=temp;
    
    max2=1;
    for (i=2; i<N; i++)
    {
        if(total[i]>total[max2])
            max2=i;
    }
    
    if (max2==max1)
        max2=0;
    
    trim[0]=max1;
    trim[1]=max2;
    
    temp=total[0];
    total[0]=total[max1];
    total[max1]=temp;
}

int minimum2(float *total, int *trim)
{
    int i, min1, min2;
    float temp;
    
    min1=0;
    for (i=1; i<N; i++)
    {
        if(total[i]<total[min1])
            min1=i;
    }
    
    temp=total[min1];
    total[min1]=total[0];
    total[0]=temp;
    
    min2=1;
    for (i=2; i<N; i++)
    {
        if(total[i]<total[min2])
            min2=i;
    }
    
    if (min2==min1)
        min2=0;
    
    trim[2]=min1;
    trim[3]=min2;
    
    temp=total[0];
    total[0]=total[min1];
    total[min1]=temp;
}

