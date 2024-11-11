#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#define N 200//the number of array's row.
#define D 4//the number of array's dimension.
#define K 3//the number of categories.
#define DATAFILE "data1(4-3).txt"//the name program tend to open.

void rand_set_ave(double a[][D + 1], double b[][D], int n);//select vectors as categories' centers. 
int loaddata(double a[][D]);//load data into the array & return the identity of data.
int sortdata(double a[][D + 1],double b[][D],int n,int sorting_se[]);//sort the data into different categories.
void average(double a[][D], double b[][D + 1], int c[], int n);//calculate the centers of different categories.
int check_ave(double a[][D + 1], double b[][D + 1]);//check the consequence of sorting and the center.
void output(double a[][D + 1], int b[], int t, int n);//out put and print.
void writedata(int a, double b[][D + 1], double c[][D + 1], int d[], int t, int n, char* filename);//write the output into a file.

//TEST FUNCTIONS BELOW THIS LINE
/*void printhub(double hub[][D], int n);
void printsorting(int sorting_se[], int n);
void printave(double ave[][D + 1]);*/
//TEST FUNCTIONS ABOVE THIS LINE

int main()
{
    double hub[N][D], ave[K][D + 1], re_ave[K][D + 1];
    int sorting_se[N];//store the consequence of sorting.
    int n,return_sorting,checking,t,i,k,output_lock;
    char filename[30];//set the output filename array.

    for (i = 1; i <= 5; i++)//run the entire program 5 times.
    {

        //LOADIND & SETTING
        t = 0;
        n = loaddata(hub);//number of data.
        rand_set_ave(ave, hub, n);

        //SORTING
        do
        {
            t++;//time-counter.
            return_sorting = sortdata(ave, hub, n, sorting_se);//to reflect the success of sorting.
            average(hub, re_ave, sorting_se, n);
            checking = check_ave(ave, re_ave);
            if (checking == 1)
                break;//if pratical centers totally equal the theoratical centers respectively, end sorting.
        } while (1);

        //OUTPUTING
        output_lock = 0;//a output lock to prevent a specific phenomenon which all vectors are devided into the same category.
        for (k = 0; k < K; k++)
        {
           if (re_ave[k][D] == 0)
                output_lock = 1;
        }
        if (output_lock == 1)
        {
            i--;
            continue;//if all vectors are devided into the same category, give up and run the program again.
        }
        printf("第%d次聚类结果：\n初始聚类中心：第", i);
        for (k = 0; k < K; k++)
            printf("%4d ", (int)ave[k][D]);//print the orders of the original sorting centers.
        printf(" 条数据\n");
        output(re_ave, sorting_se, t, n);//print the main output.
        writedata(i, ave, re_ave, sorting_se, t, n, filename);
        system("pause");
        system("cls");//press any key to run the program again.
    }
}

void rand_set_ave(double ave[][D + 1], double hub[][D], int n)
{
    int k,d,random,random_lock,i;
    srand((unsigned int)time(NULL));
    for (k = 0; k < K; k++)//get random rows in hub as the centers of each categories.
    {
        if (k == 0)
            random = rand() % n + 1;
        while(k != 0)//set a lock system to make sure the centers are different from each other.
        {
            random = rand() % n + 1;
            random_lock = 0;
            for (i = 0; i <= k; i++)
            {
                if (random == ave[i][D])
                    random_lock = 1;
            }
            if (random_lock == 0)
                break;
            else
                continue;
        }
        for (d = 0; d < D; d++)//put the specific data into ave array.
        {
            ave[k][d] = hub[random + 1][d];
        }
        ave[k][D] = random + 1;//storage the number of the centers into the additional column of ave array. 
    }
}

int loaddata(double a[][D])
{
    FILE *file_position;
    int count = 0,i;
    file_position = fopen(DATAFILE, "rb");
    if(file_position==NULL)
    {
	    printf("Can't open the file.\n");//if the program cannot open the file, the program will stop itself.
	    exit(0);
    }
    while (fscanf(file_position, "%lf", a[count + 1]) != EOF)
    {
        for (i = 1; i < D; i++)
            fscanf(file_position, "%lf", &a[count + 1][i]);//read the count+1 row's D dimentions and fill into the hub array.
        count++;//change the vaviable to count the identity of data rows.
    }
    fclose(file_position);
    return count;
}

int sortdata(double ave[][D + 1],double hub[][D],int n,int sorting_se[])
{
    int i, j, k;
    double distance_sqr[N][K];
    for (i = 1; i <= n; i++)//face to every row vector. Start with row 1.
    {
        sorting_se[i] = 0;
        for (k = 0; k < K; k++)//face to every category center. Start with column 0.
        {
            distance_sqr[i][k] = 0;//reset the distance to 0.
            for (j = 0; j < D; j++)//face to every dimention.
                distance_sqr[i][k] += pow(ave[k][j] - hub[i][j],2.0);//calculate the sigma of dimentional distance's square as total distance square.
        }
        for (k = 1; k < K; k++)
        {
            if (distance_sqr[i][k] < distance_sqr[i][sorting_se[i]])//compare distances to different centers and sorting.
                sorting_se[i] = k;//write the consequence into the consequence array sorting_se[].
        }
    }
    return 1;
}

void average(double hub[][D],double re_ave[][D + 1],int sorting_se[], int n)
{
    int i, k, d;
    for (k = 0; k < K; k++)
        for (d = 0; d <= D; d++)
            re_ave[k][d] = 0;//reset the averages of each category to 0.
    for (i = 1; i <= n; i++)
    {
        re_ave[sorting_se[i]][D]++;//storage the amount of each categories in the additional column of re_ave array.
        for (d = 0; d < D; d++)
            re_ave[sorting_se[i]][d] += hub[i][d];//calculate the sum of different categories.
    }
    for (k = 0; k < K; k++)
    {
        for (d = 0; d < D; d++)
            re_ave[k][d] /= re_ave[k][D];//use sums and amounts to calculate the average vectors of different categories.
    }
}

int check_ave(double ave[][D + 1], double re_ave[][D + 1])
{
    int k, d, checkpoint = 0;
    for (k = 0; k < K; k++)
    {
        for (d = 0; d < D; d++)
        {
            if (fabs((ave[k][d] - re_ave[k][d]) / ave[k][d]) > 0.0000001)//give the sorting program a little space to bear the mistakes.
                checkpoint++;//check if the real centers of each category equal the theoretical centers respectively.
        }
    }
    if (checkpoint == 0)
        return 1;
    else
    {
        for (k = 0; k < K; k++)
            for (d = 0; d < D; d++)
                ave[k][d] = re_ave[k][d];//copy the centers from re_ave to ave array.
        return 0;
    }
}

void output(double re_ave[][D + 1], int sorting_se[], int t,int n)
{
    int k, m, count;
    printf("\n经过%d次迭代后：\n", t);
    for (k = 0; k < K; k++)//face to K categories.
    {
        printf("第%2d 类有%3d 个数据\n", k + 1, (int)re_ave[k][D]);//print the amount of this category.
        count = 0;
        for (m = 1; m <= n; m++)
        {
            if (sorting_se[m] == k)
            {
                printf("%-6d", m);//print the order number of specific data.
                count++;
                if (count % 15 == 0)
                    printf("\n");//publishing.
            }
        }
        printf("\n\n\n\n");
    }
}

void writedata(int i, double ave[][D + 1], double re_ave[][D + 1], int sorting_se[], int t, int n, char* filename)
{
    int k,j, count, m;
    time_t time_1=time(NULL);
    struct tm* time = localtime(&time_1);
    char time_minsec[30];
    FILE* writing;
    if (i == 1)
    {
        sprintf(time_minsec, "%02d%02d", time->tm_hour, time->tm_min);//get the local time.
        strcpy(filename, " 王梓涵");
        strcat(time_minsec, filename);
        strcat(time_minsec, ".txt\0");
        strcpy(filename, time_minsec);//build the file name of the output file.
    }
    if (i == 1)
        writing = fopen(filename, "a+");//open or make a write-only file.
    else
        writing = fopen(filename, "a");

    fprintf(writing,"第%d次聚类结果：\n初始聚类中心：第", i);
    for (k = 0; k < K; k++)
        j = fprintf(writing, "%4d ", (int)ave[k][D]);//print the orders of the original sorting centers.
    j=fprintf(writing, " 条数据\n");
    j=fprintf(writing, "\n经过%d次迭代后：\n", t);

    for (k = 0; k < K; k++)//face to K categories.
    {
        j=fprintf(writing,"第%2d 类有%3d 个数据\n", k + 1, (int)re_ave[k][D]);//print the amount of this category.
        count = 0;
        for (m = 1; m <= n; m++)
        {
            if (sorting_se[m] == k)
            {
                j=fprintf(writing,"%-6d", m);//print the order number of specific data.
                count++;
                if (count % 15 == 0)
                {
                    j = fprintf(writing, "\n");//publishing.
                }

            }
        }
        j=fprintf(writing,"\n\n\n\n");
    }
    fclose(writing);
}

//Those functions below this line are designed for test purpose.
/*void printhub(double hub[][D], int n)//use for debuging.
{
    int i, j;
    for (i = 1; i <= n; i++)
    {
        for (j = 0; j < D; j++)
        {
            printf("%.1lf\t", hub[i][j]);//print the hub array.
            if (j == D - 1)
                printf("\n");
        }
    }
}

void printsorting(int sorting_se[], int n)//use for debuging.
{
    int i;
    for (i = 1; i <= n; i++)
        printf("%d\n", sorting_se[i]);//print the sorting consequence.
}

void printave(double ave[][D + 1])
{
    int i, j;
    for (i = 0; i < K; i++)
    {
        for (j = 0; j < D; j++)
        {
            printf("%.1lf\t", ave[i][j]);
            if (j == D - 1)
                printf("\n");
        }
    }
}*/