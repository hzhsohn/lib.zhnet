#ifndef _TRANS_STRPROC_H___


#ifdef  __cplusplus
extern "C" {
#endif


//��Сдת��
void strlow(char*src,char*dst);
void strupp(char*src,char*dst);

//ȥ���ַ����ո�
char *ltrim( char *str );
/**ȥ���ұ߿ո�**/
char *rtrim( char *str);
/**ȥ�����߿ո�**/
char *trim(char *str);



//���Ҳ��滻�ַ���,posΪ�ڼ��γ��ִ�����ʼ�滻
//����:replace_str_n(strSrc,"�ұ��滻��","����",strDst);
void replace_str(const char *pInput,const char *pSrc,const char *pDst,char *pOutput);
//����:replace_str_n(strSrc,"�ұ��滻��","����",0,strDst);
void replace_str_n(const char *pInput,const char *pSrc,const char *pDst,int nPos,char *pOutput);

//�����ִ�Сд�ж��ַ���
char *strstri(const char *phaystack, const char *pneedle);

//�ж��Ƿ�Ϊ����ĸ
int strisword(const char* str);

#ifdef  __cplusplus
}
#endif


#define _TRANS_STRPROC_H___
#endif