#ifndef _TRANS_URLENCOFDING_H_H___


#ifdef  __cplusplus
extern "C" {
#endif

int	ecUrldecode(char *str, int len);


/***  urlencode使用
 * 
	char *p;int k;
	char szb[]="人生如梦";
	//加码
	p=urlencode(szb,strlen(szb),&k);
	//解码
	urldecode(p,k);
	free(p);
	p=NULL;
*/
char * ecUrlencode(char const *s, int len, int *new_length);

#ifdef  __cplusplus
}
#endif


#define _TRANS_URLENCOFDING_H_H___
#endif