#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float Clow, Chigh;
    int Ilow, Ihigh;
} Breakpoint;

/* AQI Formula */
int computeAQI(float C, Breakpoint bp) {
    return (int)((bp.Ihigh - bp.Ilow) / (bp.Chigh - bp.Clow) * (C - bp.Clow) + bp.Ilow + 0.5);
}

/* ---------------- INDIAN NAQI BREAKPOINTS ---------------- */

/* PM2.5 (µg/m3) */
int AQI_PM25(float c) {
    Breakpoint bp;
    if (c <= 30) bp=(Breakpoint){0,30,0,50};
    else if (c<=60) bp=(Breakpoint){31,60,51,100};
    else if (c<=90) bp=(Breakpoint){61,90,101,200};
    else if (c<=120) bp=(Breakpoint){91,120,201,300};
    else if (c<=250) bp=(Breakpoint){121,250,301,400};
    else bp=(Breakpoint){251,350,401,500};
    return computeAQI(c,bp);
}

/* PM10 (µg/m3) */
int AQI_PM10(float c) {
    Breakpoint bp;
    if (c<=50) bp=(Breakpoint){0,50,0,50};
    else if (c<=100) bp=(Breakpoint){51,100,51,100};
    else if (c<=250) bp=(Breakpoint){101,250,101,200};
    else if (c<=350) bp=(Breakpoint){251,350,201,300};
    else if (c<=430) bp=(Breakpoint){351,430,301,400};
    else bp=(Breakpoint){431,600,401,500};
    return computeAQI(c,bp);
}

/* NO2 (µg/m3) */
int AQI_NO2(float c) {
    Breakpoint bp;
    if (c<=40) bp=(Breakpoint){0,40,0,50};
    else if (c<=80) bp=(Breakpoint){41,80,51,100};
    else if (c<=180) bp=(Breakpoint){81,180,101,200};
    else if (c<=280) bp=(Breakpoint){181,280,201,300};
    else if (c<=400) bp=(Breakpoint){281,400,301,400};
    else bp=(Breakpoint){401,1000,401,500};
    return computeAQI(c,bp);
}

/* SO2 (µg/m3) */
int AQI_SO2(float c) {
    Breakpoint bp;
    if (c<=40) bp=(Breakpoint){0,40,0,50};
    else if (c<=80) bp=(Breakpoint){41,80,51,100};
    else if (c<=380) bp=(Breakpoint){81,380,101,200};
    else if (c<=800) bp=(Breakpoint){381,800,201,300};
    else if (c<=1600) bp=(Breakpoint){801,1600,301,400};
    else bp=(Breakpoint){1601,2000,401,500};
    return computeAQI(c,bp);
}

/* CO (mg/m3) */
int AQI_CO(float c) {
    Breakpoint bp;
    if (c<=1.0) bp=(Breakpoint){0,1,0,50};
    else if (c<=2.0) bp=(Breakpoint){1.1,2.0,51,100};
    else if (c<=10) bp=(Breakpoint){2.1,10,101,200};
    else if (c<=17) bp=(Breakpoint){10.1,17,201,300};
    else if (c<=34) bp=(Breakpoint){17.1,34,301,400};
    else bp=(Breakpoint){34.1,50,401,500};
    return computeAQI(c,bp);
}

/* O3 (µg/m3) */
int AQI_O3(float c) {
    Breakpoint bp;
    if (c<=50) bp=(Breakpoint){0,50,0,50};
    else if (c<=100) bp=(Breakpoint){51,100,51,100};
    else if (c<=168) bp=(Breakpoint){101,168,101,200};
    else if (c<=208) bp=(Breakpoint){169,208,201,300};
    else if (c<=748) bp=(Breakpoint){209,748,301,400};
    else bp=(Breakpoint){749,1000,401,500};
    return computeAQI(c,bp);
}

/* AQI Category Name */
const char* getCategory(int aqi) {
    if (aqi <= 50) return "Good";
    if (aqi <= 100) return "Satisfactory";
    if (aqi <= 200) return "Moderate";
    if (aqi <= 300) return "Poor";
    if (aqi <= 400) return "Very Poor";
    return "Severe";
}

int main() {
    float pm25, pm10, no2, so2, co, o3;

    printf("\nEnter pollutant concentrations:\n");
    printf("PM2.5 (µg/m³): "); scanf("%f", &pm25);
    printf("PM10 (µg/m³): ");  scanf("%f", &pm10);
    printf("NO2 (µg/m³): ");   scanf("%f", &no2);
    printf("SO2 (µg/m³): ");   scanf("%f", &so2);
    printf("CO (mg/m³): ");    scanf("%f", &co);
    printf("O3 (µg/m³): ");    scanf("%f", &o3);

    int aqi_pm25 = AQI_PM25(pm25);
    int aqi_pm10 = AQI_PM10(pm10);
    int aqi_no2  = AQI_NO2(no2);
    int aqi_so2  = AQI_SO2(so2);
    int aqi_co   = AQI_CO(co);
    int aqi_o3   = AQI_O3(o3);

    int finalAQI =
        (aqi_pm25 > aqi_pm10 ? aqi_pm25 : aqi_pm10);
    finalAQI =
        (finalAQI > aqi_no2 ? finalAQI : aqi_no2);
    finalAQI =
        (finalAQI > aqi_so2 ? finalAQI : aqi_so2);
    finalAQI =
        (finalAQI > aqi_co ? finalAQI : aqi_co);
    finalAQI =
        (finalAQI > aqi_o3 ? finalAQI : aqi_o3);

    printf("\n-------------------------");
    printf("\n   AQI REPORT (INDIA)");
    printf("\n-------------------------");
    printf("\nPM2.5 AQI: %d", aqi_pm25);
    printf("\nPM10  AQI: %d", aqi_pm10);
    printf("\nNO2   AQI: %d", aqi_no2);
    printf("\nSO2   AQI: %d", aqi_so2);
    printf("\nCO    AQI: %d", aqi_co);
    printf("\nO3    AQI: %d", aqi_o3);

    printf("\n\nFinal AQI = %d", finalAQI);
    printf("\nCategory : %s\n", getCategory(finalAQI));

    /* Save to file */
    FILE *fp = fopen("AQI_Report.txt", "w");
    if (fp) {
        fprintf(fp,
            "AQI REPORT (Indian National AQI)\n"
            "----------------------------------\n"
            "PM2.5 AQI : %d\n"
            "PM10  AQI : %d\n"
            "NO2   AQI : %d\n"
            "SO2   AQI : %d\n"
            "CO    AQI : %d\n"
            "O3    AQI : %d\n"
            "\nFinal AQI : %d\n"
            "Category  : %s\n",
            aqi_pm25, aqi_pm10, aqi_no2, aqi_so2, aqi_co, aqi_o3,
            finalAQI, getCategory(finalAQI)
        );
        fclose(fp);
        printf("\nReport saved to AQI_Report.txt\n");
    } else {
        printf("\nError: Could not save report!\n");
    }

    return 0;
}