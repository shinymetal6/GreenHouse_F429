#include <stdio.h>
#include <stdlib.h>
#define BUF_SIZE    131072*4
unsigned char  bmpfile[BUF_SIZE];
FILE*   fpin;
FILE*   fpout;

int readbmp(char *filename)
{
int bmp_len;
    fpin = fopen (filename, "r");
    if ( fpin == NULL )
    {
        printf( "Unable to open %s for read\n",filename ) ;
        return -1;
    }
    bmp_len = fread(bmpfile, 1, BUF_SIZE, fpin);
    fclose(fpin);
    return bmp_len;
}

int convert_to_c(char *filename,char *arrayname,int bmp_len)
{
int i,k=0;
    fpout = fopen(filename, "w");
    if ( fpout == NULL )
    {
        printf( "Unable to open %s for write\n",filename ) ;
        return -1;
    }
    fprintf(fpout,"#include \"main.h\"\n");
    fprintf(fpout,"__attribute__((section(\".table\"))) const uint8_t %s[%d] = \n",arrayname,bmp_len);
    fprintf(fpout,"{\n");
    printf("%s has %d length\n",arrayname,bmp_len);
    for(i=0;i<bmp_len;i++, k++)
    {
        if ( k == 8)
        {
            k=0;
            fprintf(fpout,"\n");
        }
        fprintf(fpout,"0x%02x,",bmpfile[i]);
        //printf("\n");
    }
    fprintf(fpout,"\n};\n");
    fclose(fpout);
    return 0;
}

int main()
{
int bmp_len;

    bmp_len = readbmp ("../../Images/fan48_idle.bmp");
    convert_to_c("../../Core/Src/Application/Images/fan48_idle.c","fan48_idle",bmp_len);
    bmp_len = readbmp ("../../Images/fan48_active1.bmp");
    convert_to_c("../../Core/Src/Application/Images/fan48_active1.c","fan48_active1",bmp_len);
    bmp_len = readbmp ("../../Images/fan48_active2.bmp");
    convert_to_c("../../Core/Src/Application/Images/fan48_active2.c","fan48_active2",bmp_len);

    bmp_len = readbmp ("../../Images/heater48_idle.bmp");
    convert_to_c("../../Core/Src/Application/Images/heater48_idle.c","heater48_idle",bmp_len);
    bmp_len = readbmp ("../../Images/heater48_active1.bmp");
    convert_to_c("../../Core/Src/Application/Images/heater48_active1.c","heater48_active1",bmp_len);
    bmp_len = readbmp ("../../Images/heater48_active2.bmp");
    convert_to_c("../../Core/Src/Application/Images/heater48_active2.c","heater48_active2",bmp_len);

    bmp_len = readbmp ("../../Images/waterpump48_idle.bmp");
    convert_to_c("../../Core/Src/Application/Images/waterpump48_idle.c","waterpump48_idle",bmp_len);
    bmp_len = readbmp ("../../Images/waterpump48_active1.bmp");
    convert_to_c("../../Core/Src/Application/Images/waterpump48_active1.c","waterpump48_active1",bmp_len);
    bmp_len = readbmp ("../../Images/waterpump48_active2.bmp");
    convert_to_c("../../Core/Src/Application/Images/waterpump48_active2.c","waterpump48_active2",bmp_len);

    bmp_len = readbmp ("../../Images/light48_idle.bmp");
    convert_to_c("../../Core/Src/Application/Images/light48_idle.c","light48_idle",bmp_len);
    bmp_len = readbmp ("../../Images/light48_active1.bmp");
    convert_to_c("../../Core/Src/Application/Images/light48_active1.c","light48_active1",bmp_len);
    bmp_len = readbmp ("../../Images/light48_active2.bmp");
    convert_to_c("../../Core/Src/Application/Images/light48_active2.c","light48_active2",bmp_len);
    return 0;
}
