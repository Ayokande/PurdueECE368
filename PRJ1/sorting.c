#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "sorting.h"
#include <math.h>


/******************************
*[Ayomide David Akande]
*[sorting.c]: [06/27/2018].
*akande@purdue.edu]
*Compiling: [gcc -Werror -lm -Wall -O3 sorting.c sorting_main.c -o proj1]
*******************************/

int _getSize(int length);
int* _helper(int length, int size);
/*
	loads the file into Arr and returns Arr, size of array is stored in *Size
	when there is a file error, or memory allocation error, return NULL, and	
	set *Size to 0
*/
long *Load_From_File(char *Filename, int *Size)
{
    long *Arr = NULL;
    *Size = 0;
    
    FILE* fp;
    fp = fopen(Filename,"r");
    
    if (fp == NULL)
    {
        return Arr;
    }
    
    fseek(fp, 0L, SEEK_SET);
    fscanf(fp,"%d", Size);
    
    if (*Size == 0)
    {
        return Arr;
    }
    
    else
    {
        int index = 0;
        Arr = malloc(sizeof(long) * (*Size));
        for(index = 0; index < (*Size); index++ )
        {
            fscanf(fp, "%ld", &Arr[index]);
        }
        
    }
    fseek(fp, 0L, SEEK_SET);
    fclose(fp);

    return Arr;
}


int Save_To_File(char *Filename, long *Array, int Size)
{
    int n_written = 0;

    FILE* fp;
    fp = fopen(Filename,"w");
    
    if (fp == NULL)
    {
        return EXIT_FAILURE;
    }
    
    if (Size == 0)
    {
        return n_written;
    }
    
    fseek(fp, 0L, SEEK_SET);
    n_written = fwrite(Array,sizeof(*Array),Size,fp);
    fclose(fp);
    
    return n_written;
}


/*
	Print the sequence in the order in which it appears in the triangle
 	2^(p)3^(q) is the largest number, 
	If Size is 0 or 1, an empty file should be created
*/
int Print_Seq(char *Filename, int length)
{
	int seq_size = 0;
    FILE* fp;
    fp = fopen(Filename, "w");
    
    if (fp == NULL)
    {
        return EXIT_FAILURE;
    }
    
    if(length == 0 || length == 1)
    {
        fclose(fp);
        return seq_size;
    }
    
    int index = 0;
    int p = 0;
    int q = 0;
    int seq_var;
    for (index = 0; index < length; index ++)
    {
        p = index;
        q = 0;

        while (q < (index + 1) && (p + 1) > 0)
        {
            seq_var = pow(2, p) * pow(3, q);
            seq_size++;
            p--;
            q++;
            if (seq_var > length)
            {
                break;
            }
            fprintf(fp, "%d", seq_var);
        }
       
        if (seq_var > length)
        {
            break;
        }
        
    }
    fclose(fp);
    return seq_size;
}


void Shell_Insertion_Sort(long *a, int length, double *ncomp, double *nswap)
{
    int seq_size = _getSize(length);
    int * seq_arr = _helper(length,seq_size);
    int ct = seq_size - 1;
    int gap = 0;
    int i = 0;
    double comp = 0;
    double swap = 0;
    
    for (gap = seq_arr[ct]; gap > 0; gap = seq_arr[ct--])
    {
        for (i = gap; i < length; i += 1)
        {
            int temp = a[i];
            int j;
            for (j = i; j >= gap && a[j - gap] > temp; j -= gap)
            {
                a[j] = a[j - gap];
                
                *ncomp = comp++;
            }
            a[j] = temp;
            *ncomp = comp ++;
            *nswap = swap ++;
        }
    }
    
}


void Shell_Selection_Sort(long *a, int length, double *ncomp, double *nswap)
{
    int i, j, k, m, gap, temp;
    int max = 1, maxLevel = 0, numSeq = 0;
    int seq_size = _getSize(length);
    int * seq_arr = _helper(length,seq_size);
    double comp = 0;
    double swap = 0;
    
    while(max < length)
    {
        max *= 3;
        maxLevel ++;
    }
    
    for(i = maxLevel; i > 0; i--)
    {
        numSeq += i;
    }
    
    for(i = numSeq - 1; i >= 0; i--)
    {
        gap = seq_arr[i];
        gap = (gap - 1)/2;
        for(j = gap; j < length ; j++)
        {
            for(k = j, m = j; k < length; k++)
            {
                if(a[k] < a[m])
                {
                    *ncomp = comp++;
                    m = k;
                }
            }
            temp = a[j];
            a[j] = a[m];
            a[m] = temp;
            swap += 3;
            *nswap = swap;
        }
        
    }
    free(seq_arr);
}


int* _helper(int length, int size)
{
    int* Arr;
    Arr = malloc(sizeof(int) * size);
    
    int ct = 0;
    int index = 0;
    int p = 0;
    int q = 0;
    int seq_var = 0;
    
    for (index = 0; index < length; index ++)
    {
        p = index;
        q = 0;
        
        while (q < (index + 1) && (p + 1) > 0)
        {
            seq_var = pow(2, p) * pow(3, q);
            Arr[ct] = seq_var;
            ct++;
            p--;
            q++;
            if (seq_var > length)
            {
                break;
            }
        }
        if (seq_var > length)
        {
            break;
        }
        
    }

    return Arr;
}

int _getSize(int length)
{
    int index = 0;
    int p = 0;
    int q = 0;
    int seq_var = 0;
    int seq_size = 0;
    
    for (index = 0; index < length; index ++)
    {
        p = index;
        q = 0;
        
        while (q < (index + 1) && (p + 1) > 0)
        {
            seq_var = pow(2, p) * pow(3, q);
            seq_size++;
            p--;
            q++;
            if (seq_var > length)
            {
                break;
            }
        }
        if (seq_var > length)
        {
            break;
        }
        
    }
    
    return seq_size;
}
