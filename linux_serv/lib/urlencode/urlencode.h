#ifndef _TRANS_URLENCOFDING_H_H___


#ifdef  __cplusplus
extern "C" {
#endif

int	ecUrldecode(char *str, int len);


/***  urlencodeʹ��
 * 
	char *p;int k;
	char szb[]="��������";
	//����
	p=urlencode(szb,strlen(szb),&k);
	//����
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