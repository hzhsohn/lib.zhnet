#ifndef _TRANS_STRPROC_H___


#ifdef  __cplusplus
extern "C" {
#endif


//大小写转换
void strlow(char*src,char*dst);
void strupp(char*src,char*dst);

//去除字符串空格
char *ltrim( char *str );
/**去除右边空格**/
char *rtrim( char *str);
/**去除两边空格**/
char *trim(char *str);



//查找并替换字符串,pos为第几次出现次数开始替换
//例子:replace_str_n(strSrc,"我被替换了","靓仔",strDst);
void replace_str(const char *pInput,const char *pSrc,const char *pDst,char *pOutput);
//例子:replace_str_n(strSrc,"我被替换了","靓仔",0,strDst);
void replace_str_n(const char *pInput,const char *pSrc,const char *pDst,int nPos,char *pOutput);

//不区分大小写判断字符串
char *strstri(const char *phaystack, const char *pneedle);

//判断是否为纯字母
int strisword(const char* str);

#ifdef  __cplusplus
}
#endif


#define _TRANS_STRPROC_H___
#endif