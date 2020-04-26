#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <lcd.h>

#define LCD_RS  9
#define LCD_E   10
#define LCD_D4  12
#define LCD_D5  14
#define LCD_D6  13
#define LCD_D7  15

unsigned char degree[8] =
{
0b00110,
0b01001,
0b01001,
0b00110,
0b00000,
0b00000,
0b00000,
0b00000
};

unsigned char processor[8] =
{
0b00000,
0b01010,
0b01110,
0b11011,
0b01110,
0b11011,
0b01110,
0b01010
};

char* getUptime(void);
char* getTemp(void);
char* getWeather(void);

int main() {
    int code = wiringPiSetup();
    if (code != 0)
        return -1;
    int fd = lcdInit(2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);
    
    lcdCharDef(fd, 10, degree);
    lcdCharDef(fd, 11, processor);
    lcdPosition(fd, 0, 0);
    lcdPutchar(fd, 11);
    lcdPosition(fd, 5, 0);
    lcdPutchar(fd, 10);
    lcdPuts(fd, "C");

    int counter = 0;

    while (1)
    {
        if (counter == 3600)
            counter == 0;

        if (counter % 5 == 0) {
            lcdPosition(fd, 1, 0);
            lcdPuts(fd, getTemp());
        }

        if (counter % 10 == 0) {
            lcdPosition(fd, 0, 1);
            lcdPuts(fd, getUptime());
        }

        counter++;
        delay(1000);
    }

    return 0;
}

char* getUptime() {
    FILE* fileUptime;
    fileUptime = fopen("/proc/uptime", "r");
    if (fileUptime == NULL) {
        return "error read file";
    }

    char uptime[10], next;
    for (int i = 0; i < 10; i++) {
        next = fgetc(fileUptime);
        if (next == '.') {
            uptime[i] = '\0';
            break;
        }
        uptime[i] = next;
    }

    fclose(fileUptime);

    int days = atoi(uptime) / (60 * 60 * 24);
    int hours = (atoi(uptime) - days * 60 * 60 * 24) / (60 * 60);
    int minutes = (atoi(uptime) - days * 60 * 60 * 24 - hours * 60 * 60) / 60;

    char* up = malloc(20);
    sprintf(up, "%dd %dh %dm", days, hours, minutes);
    return up;
}

char* getTemp() {
    FILE* ft0, * ft1;

    if ((ft0 = fopen("/sys/devices/virtual/thermal/thermal_zone0/temp", "r")) == NULL) {
        printf("Read t0 error");
    }
    if ((ft1 = fopen("/sys/devices/virtual/thermal/thermal_zone1/temp", "r")) == NULL) {
        printf("Read t1 error");
    }

    char t0[6], t1[6];
    fgets(t0, 6, ft0);
    fgets(t1, 6, ft1);

    fclose(ft0);
    fclose(ft1);

    char* t = malloc(5);
    gcvt((atof(t0) + atof(t1)) / 2000, 3, t);
    return t;
}

char* getWeather() {
    return "weather";
}


