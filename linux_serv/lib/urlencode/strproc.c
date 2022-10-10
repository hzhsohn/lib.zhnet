#include "strproc.h"
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include<stdio.h>
#include<ctype.h>

///////////////////////////////
// ת����Сд
void strlow(char*src,char*dst)
{
    size_t i;
    for(i=0; i< strlen(src); i++)
    {
        dst[i] = tolower(src[i]);
    }
}

void strupp(char*src,char*dst)
{
    size_t i;
    for(i=0; i< strlen(src); i++)
    {
        dst[i] = toupper(src[i]);
    }
}

///////////////////////////////
//ȥ���ַ����ո�
char *ltrim( char *str ) 
{
    /**ȥ����߿ո�**/
    int length = (int)strlen( str );
    char *i;
    char *len;
    int m = 0;
    int n = 0;
    i = str;
    len = str + length;
    
    for (; i<len; i++ ) {
        if ( *i == ' ' || *i == '\t' || *i == '\n' ) {
            n ++;
        } else {
            break;
        }
    }
    for ( m=0; m<=length-n; m++ ) {
        *(str + m) = *(str + n + m);
    }
    return str;
}        

/**ȥ���ұ߿ո�**/
char *rtrim( char *str) 
{
    char *i;
    i = str + strlen( str ) - 1;
    
    for (; i>=str; i-- ) {
        if ( *i == ' ' || *i == '\t' || *i == '\n' ) {
            *(str + strlen(str) -1) = '\0';
        } else {
            break;
        }
    }
    return str;                                                                                                                            
}

/**ȥ�����߿ո�**/
char *trim(char *str)
{
    ltrim(str);
    rtrim(str);
    return str;
}





//replace_str(strSrc,"�ұ��滻��","����",strDst);
//���Ҳ��滻�ַ���
void replace_str(const char *pInput,const char *pSrc,const char *pDst,char *pOutput)
{
    const char   *pi;
    char    *po, *p;
    int     nSrcLen, nDstLen, nLen;
	char	*tmp;

	tmp=(char*)malloc(strlen(pInput)+1);
	strcpy(tmp,pInput);

    // ָ�������ַ������ζ�ָ��.
    pi = pInput;    
    // ָ������ַ������ζ�ָ��.
    po = pOutput;
    // ���㱻�滻�����滻���ĳ���.
    nSrcLen = (int)strlen(pSrc);
    nDstLen = (int)strlen(pDst);

    // ����piָ���ַ����е�һ�γ����滻����λ��,������ָ��(�Ҳ����򷵻�null).   
    p = (char *)strstr(pi, pSrc);
    if(p)
    {
        // �ҵ�.
        while(p)
        {
            // ���㱻�滻��ǰ���ַ����ĳ���.
            nLen = (int)(p - pi);
            // ���Ƶ�����ַ���.
            memcpy(po, pi, nLen);
            memcpy(po + nLen, pDst, nDstLen);
            // �������滻��.
            pi = p + nSrcLen;
            // ����ָ���������ָ��λ��.
            po = po + nLen + nDstLen;
            // ��������.
            p = (char *)strstr(pi, pSrc);
        }
        // ����ʣ���ַ���.
        strcpy(po, pi);
    }
    else
    {
        // û���ҵ���ԭ������.
        strcpy(po, pi);
    }

	free(tmp);
	tmp=NULL;
}

//���Ҳ��滻�ַ���
void replace_str_n(const char *pInput,const char *pSrc,const char *pDst,int nPos,char *pOutput)
{
    const char   *pi;
    char    *po, *p;
    int     nSrcLen, nDstLen, nLen;
    int     i;
	char	*tmp;
    
    if(nPos<0){return;}
    
	tmp=(char*)malloc(strlen(pInput)+1);
	strcpy(tmp,pInput);

    // ָ�������ַ������ζ�ָ��.
    pi = tmp;    
    // ָ������ַ������ζ�ָ��.
    po = pOutput;
    // ���㱻�滻�����滻���ĳ���.
    nSrcLen = (int)strlen(pSrc);
    nDstLen = (int)strlen(pDst);
    
    // ����piָ���ַ����е�һ�γ����滻����λ��,������ָ��(�Ҳ����򷵻�null).
    p=(char*)pi;
    for (i=0; i<=nPos && NULL!=p; i++)
    {
		if(i==0)
		{
			p = strstr(p, pSrc); 
		}
		else
		{
			p = strstr(p+nSrcLen, pSrc); 
		}
	}
    
    if(p)
    {
        // ���㱻�滻��ǰ���ַ����ĳ���.
        nLen = (int)(p - pi);
        // ���Ƶ�����ַ���.
        memcpy(po, pi, nLen);
        memcpy(po + nLen, pDst, nDstLen);
        // �������滻��.
        pi = p + nSrcLen;
        // ����ָ���������ָ��λ��.
        po = po + nLen + nDstLen;
        
        // ����ʣ���ַ���.
        strcpy(po, pi);
    }
    else
    {
        // û���ҵ���ԭ������.
        strcpy(po, pi);
    }

	free(tmp);
	tmp=NULL;
}


/* int ret=strstri("hi,baby,i miss you..", "miss"); */
char *strstri(const char *phaystack, const char *pneedle)
{
  register const unsigned char *haystack, *needle;
    register unsigned bl, bu, cl, cu;
    
    haystack = (const unsigned char *) phaystack;
    needle = (const unsigned char *) pneedle;
    
    bl = tolower(*needle);
    if (bl != '\0')
    {
        // Scan haystack until the first character of needle is found:
        bu = toupper(bl);
        haystack--;                             /* possible ANSI violation */
        do
        {
            cl = *++haystack;
            if (cl == '\0')
                goto ret0;
        }
        while ((cl != bl) && (cl != bu));
        
        // See if the rest of needle is a one-for-one match with this part of haystack:
        cl = tolower(*++needle);
        if (cl == '\0')  // Since needle consists of only one character, it is already a match as found above.
            goto foundneedle;
        cu = toupper(cl);
        ++needle;
        goto jin;
        
        for (;;)
        {
            register unsigned a;
            register const unsigned char *rhaystack, *rneedle;
            do
            {
                a = *++haystack;
                if (a == '\0')
                    goto ret0;
                if ((a == bl) || (a == bu))
                    break;
                a = *++haystack;
                if (a == '\0')
                    goto ret0;
            shloop:
                ;
            }
            while ((a != bl) && (a != bu));
            
        jin:
            a = *++haystack;
            if (a == '\0')  // Remaining part of haystack is shorter than needle.  No match.
                goto ret0;
            
            if ((a != cl) && (a != cu)) // This promising candidate is not a complete match.
                goto shloop;            // Start looking for another match on the first char of needle.
            
            rhaystack = haystack-- + 1;
            rneedle = needle;
            a = tolower(*rneedle);
            
            if (tolower(*rhaystack) == (int) a)
                do
                {
                    if (a == '\0')
                        goto foundneedle;
                    ++rhaystack;
                    a = tolower(*++needle);
                    if (tolower(*rhaystack) != (int) a)
                        break;
                    if (a == '\0')
                        goto foundneedle;
                    ++rhaystack;
                    a = tolower(*++needle);
                }
            while (tolower(*rhaystack) == (int) a);
            
            needle = rneedle;               /* took the register-poor approach */
            
            if (a == '\0')
                break;
        } // for(;;)
   
} // if (bl != '\0')
foundneedle:
    return (char*) haystack;
ret0:
    return 0;
}


int strisword(const char* str)
{
	int i=0;
	char a=0;
	for(i=0;i < strlen(str);i++)
	{
		a=str[i];
		if( !(a>='a' && a<='z') && !(a>='A' && a<='Z') && !(a>='0' && a<='9')  )
		{ return 0; }
	}
	//�Ǵ�Ӣ����ĸ��
    return 1;    
}