#include <string.h>
#include "hexfile.h"

extern int verbose;

intelln_t * readhexline(FILE * infile);

intelln_t * loadhexfile(char * filename) {
	FILE * infile;
	intelln_t * _first = 0;
    struct intelln * line; 
    struct intelln * lastline;
    
	infile = fopen(filename,"r");
    if (infile == NULL) 
    {
		fprintf(stderr,"Could not open file %s\n",filename);
    	return NULL;
    }
    
    while (!feof(infile))
    {
       line = readhexline(infile);
       if (!line) {
	  printf("ERROR EMPTY LINE\n");
          return 0;
       }
       if (_first==0) {
	  _first = line;
       }
       else {
          lastline->next = line;
       }
       lastline = line;
    }
  return _first;
}

void deletehexdata(intelln_t * hexdata)
{
	intelln_t * next;
	while (hexdata)
	{
		free(hexdata->data);
		next = hexdata->next;
		free(hexdata);
		hexdata = next;
	}
}

intelln_t * readhexline(FILE * infile) {
   unsigned char inbuf[524];
   unsigned char data[256];
   unsigned char crcbuf[4];
   unsigned char hexval[8];
   intelln_t * line; 
   int len;
   int addr;
   int type;
   int k;
   unsigned char crc, compcrc=0;

   unsigned char c=fgetc(infile);
   while (c != ':') {
      c=fgetc(infile);
   }
   /* read length */
   for (k=0;k<2;k++) 
   {
      hexval[k] = fgetc(infile);
   }
   hexval[2]=0;
   len = strtol((char *)hexval, NULL, 16); 
   /* read length */
   for (k=0;k<4;k++) 
   {
      hexval[k] = fgetc(infile);
   }
   hexval[4]=0;
   addr = strtol((char *)hexval, NULL, 16); 
   /* read type */
   for (k=0;k<2;k++) 
   {
      hexval[k] = fgetc(infile);
   }
   hexval[2]=0;
   type = strtol((char *)hexval, NULL, 16); 
   inbuf[0] = 0;
   fgets((char *)inbuf,2*len+1,infile);
   fgets((char *)crcbuf,3,infile);
   crc = strtol((char *)crcbuf, NULL, 16);

   if (verbose>4)   printf("length:%i, addr:%x, type:%i, data:%s, crc:%s %x \n",len,addr,type,inbuf,crcbuf,crc); 

   compcrc = len+type+(addr>>8)+(addr & 0xff);
   /* dehex data */
   for (k=len-1;k>=0;k--) {
      inbuf[2*k+2] = 0;
      data[k] = strtol((char *)(inbuf+2*k), NULL, 16); 
      compcrc += data[k];
   }

   compcrc =  ~compcrc + 1;
   for (k=0;k<len;k++) {
      inbuf[2*k+2] = 0;
   }
   /* cleanup end of line */
   c = 0;
   while ((!feof(infile)) && (c!=':')) {
      c = fgetc(infile);
   }
   if (c==':') {
      ungetc(c,infile);
   }

   if (crc==compcrc) {
      line = malloc(sizeof(intelln_t));
      line->data = malloc(len);
      memcpy(line->data, data, len);
      line->addr = addr;
      line->len = len;
      line->type = type;
   }
   else
     {
       printf("ERROR IN CRC\n");
     }
   return line;
}
