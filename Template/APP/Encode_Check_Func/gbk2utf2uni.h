#ifndef GBK2UTF2UNI_H
#define GBK2UTF2UNI_H

unsigned short getun(unsigned short gb);
unsigned short getgb(unsigned short unicode);
int gb2unicode(unsigned char *unicode, unsigned char*gb);
int unicode2gb(unsigned char*gb, unsigned char * unicode);
int unicode2utf8(unsigned char * utf, unsigned char * unicode);
int utf82unicode(unsigned char * unicode, unsigned char * utf);
int gb2utf8(unsigned  char * utf, unsigned  char * gb);
int utf82gbk(unsigned  char * gb, unsigned  char * utf);
#endif
