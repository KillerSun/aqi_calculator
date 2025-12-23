#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ANSI Colors
#define RESET "\033[0m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define ORANGE "\033[38;5;208m"
#define RED "\033[1;31m"
#define PURPLE "\033[1;35m"
#define MAROON "\033[38;5;88m"
#define CYAN "\033[1;36m"

// AQI Categories
const char *categories[] = {
    "Good", "Satisfactory", "Moderate",
    "Poor", "Very Poor", "Severe"
};

// AQI limits
int AQI_range_low[]  = {0, 51, 101, 201, 301, 401};
int AQI_range_high[] = {50,100,200,300,400,500};

// Breakpoints (CPCB Standard)
typedef struct {
    float Clow, Chigh;
    int Ilow, Ihigh;
} Breakpoint;

// PM2.5
Breakpoint PM25[] = {
    {0,30,0,50}, {31,60,51,100}, {61,90,101,200},
    {91,120,201,300},{121,250,301,400},{251,500,401,500}
};

// PM10
Breakpoint PM10[] = {
    {0,50,0,50},{51,100,51,100},{101,250,101,200},
    {251,350,201,300},{351,430,301,400},{431,500,401,500}
};

// NO2
Breakpoint NO2[] = {
    {0,40,0,50},{41,80,51,100},{81,180,101,200},
    {181,280,201,300},{281,400,301,400},{401,1000,401,500}
};

// SO2
Breakpoint SO2[] = {
    {0,40,0,50},{41,80,51,100},{81,380,101,200},
    {381,800,201,300},{801,1600,301,400},{1601,2000,401,500}
};

// CO (mg/m3)
Breakpoint CO[] = {
    {0,1,0,50},{1.1,2,51,100},{2.1,10,101,200},
    {10.1,17,201,300},{17.1,34,301,400},{34.1,50,401,500}
};

// O3
Breakpoint O3[] = {
    {0,50,0,50},{51,100,51,100},{101,168,101,200},
    {169,208,201,300},{209,748,301,400},{749,1000,401,500}
};

// Function to compute linear AQI interpolation
int computeIndex(float conc, Breakpoint arr[])
{
    for(int i=0;i<6;i++){
        if(conc >= arr[i].Clow && conc <= arr[i].Chigh){
            return ( (arr[i].Ihigh-arr[i].Ilow)/(arr[i].Chigh-arr[i].Clow) )*(conc-arr[i].Clow)+arr[i].Ilow;
        }
    }
    return -1;
}

// Get AQI Category Level
int getLevel(int aqi){
    if(aqi<=50) return 0;
    else if(aqi<=100) return 1;
    else if(aqi<=200) return 2;
    else if(aqi<=300) return 3;
    else if(aqi<=400) return 4;
    return 5;
}

// Colored Output
void printColored(int level,int aqi){
    switch(level){
        case 0: printf(GREEN); break;
        case 1: printf(YELLOW); break;
        case 2: printf(ORANGE); break;
        case 3: printf(RED); break;
        case 4: printf(PURPLE); break;
        default: printf(MAROON);
    }
    printf("\nAQI: %d | Category: %s\n\n",aqi,categories[level]);
    printf(RESET);
}

void calculateAQI()
{
    float pm25, pm10, no2, so2, co, o3;
    char city[50];
    FILE *fp;
    time_t t;

    printf("\nEnter City Name: ");
    scanf("%s", city);

    printf("\nEnter PM2.5 (µg/m3): "); scanf("%f",&pm25);
    printf("Enter PM10 (µg/m3): "); scanf("%f",&pm10);
    printf("Enter NO2 (µg/m3): "); scanf("%f",&no2);
    printf("Enter SO2 (µg/m3): "); scanf("%f",&so2);
    printf("Enter CO (mg/m3): "); scanf("%f",&co);
    printf("Enter O3 (µg/m3): "); scanf("%f",&o3);

    if(pm25<0||pm10<0||no2<0||so2<0||co<0||o3<0){
        printf(RED "\nInvalid input! Concentrations cannot be negative.\n" RESET);
        return;
    }

    int s1=computeIndex(pm25,PM25);
    int s2=computeIndex(pm10,PM10);
    int s3=computeIndex(no2,NO2);
    int s4=computeIndex(so2,SO2);
    int s5=computeIndex(co,CO);
    int s6=computeIndex(o3,O3);

    int aqi=s1;
    int arr[]={s1,s2,s3,s4,s5,s6};
    for(int i=0;i<6;i++)
        if(arr[i]>aqi) aqi=arr[i];

    int level=getLevel(aqi);
    printColored(level,aqi);

    fp=fopen("AQI_Data.csv","a");
    if(fp==NULL){
        printf(RED "Error opening file!\n" RESET);
        return;
    }

    time(&t);
    fprintf(fp,"%s,%d,%s,%s",city,aqi,categories[level],ctime(&t));
    fclose(fp);
    printf(GREEN "Saved to AQI_Data.csv\n" RESET);
}

void viewRecords()
{
    FILE *fp=fopen("AQI_Data.csv","r");
    if(fp==NULL){
        printf(RED "\nNo Records Found!\n" RESET);
        return;
    }

    char ch;
    printf(CYAN "\n===== AQI RECORDS =====\n\n" RESET);
    while((ch=fgetc(fp))!=EOF)
        putchar(ch);

    fclose(fp);
}

int main()
{
    int ch;

    while(1){
        system("clear || cls");
        printf(CYAN "============== AIR QUALITY INDEX SYSTEM (India CPCB) ==============\n" RESET);
        printf("1. Calculate AQI\n");
        printf("2. View Saved Records\n");
        printf("3. Exit\n");
        printf("Choose: ");
        scanf("%d",&ch);

        switch(ch){
            case 1: calculateAQI(); break;
            case 2: viewRecords(); break;
            case 3: printf(GREEN "\nThank you for using AQI System!\n" RESET); exit(0);
            default: printf(RED "\nInvalid Choice!\n" RESET);
        }

        printf("\nPress Enter to continue...");
        getchar();
        getchar();
    }
}
