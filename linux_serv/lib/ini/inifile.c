#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "inifile.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MAX_FILE_SIZE 1024*16
#define LEFT_BRACE '['
#define RIGHT_BRACE ']'

static int load_ini_file(const char *file, char *buf,int *file_size)
{
    FILE *in = NULL;
    int i=0;
    *file_size =0;

    assert(file !=NULL);
    assert(buf !=NULL);

    in = fopen(file,"r");
    if( NULL == in) {
        return 0;
    }

    buf[i]=fgetc(in);
    
    //load initialization file

    while( buf[i]!= (char)EOF) {
        i++;
        assert( i < MAX_FILE_SIZE); //file too big

        buf[i]=fgetc(in);
    }
    
    buf[i]='\0';
    *file_size = i;

    fclose(in);
    return 1;
}

static int isnewline(char c)
{
    return ('\n' == c || '\r' == c )? 1 : 0;
}
static int isend(char c)
{
    return '\0'==c? 1 : 0;
}
static int isleftbarce(char c)
{
    return LEFT_BRACE == c? 1 : 0;
}
static int isrightbrace(char c )
{
    return RIGHT_BRACE == c? 1 : 0;
}
static int parse_file(const char *section, const char *key, const char *buf,int *sec_s,int *sec_e,
                     int *key_s,int *key_e, int *value_s, int *value_e)
{
    const char *p = buf;
    int i=0;

    assert(buf!=NULL);
    assert(section != NULL && strlen(section));
    assert(key != NULL && strlen(key));

    *sec_e = *sec_s = *key_e = *key_s = *value_s = *value_e = -1;

    while( !isend(p[i]) ) {
        //find the section

        if( ( 0==i || isnewline(p[i-1]) ) && isleftbarce(p[i]) )
        {
            int section_start=i+1;
			int tmp=0;

            //find the ']'

            do {
                i++;
            } while( !isrightbrace(p[i]) && !isend(p[i]));

			tmp=i-section_start;
            if(strlen(section)==tmp && 0 == memcmp(p+section_start,section, tmp)) 
			{
                int newline_start=0;

                i++;

                //Skip over space char after ']'

                while(isspace(p[i])) {
                    i++;
                }

                //find the section

                *sec_s = section_start;
                *sec_e = i;

                while( ! (isnewline(p[i-1]) && isleftbarce(p[i])) 
                && !isend(p[i]) ) {
                    int j=0;
                    //get a new line

                    newline_start = i;

                    while( !isnewline(p[i]) && !isend(p[i]) ) {
                        i++;
                    }
                    
                    //now i is equal to end of the line

                    j = newline_start;

                    if(';' != p[j]) //skip over comment

                    {
                        while(j < i && p[j]!='=') {
                            j++;
                            if('=' == p[j]) {
                                if(zhIniStrcmp(key,p+newline_start)==0)
                                {
                                    //find the key ok

                                    *key_s = newline_start;
                                    *key_e = j-1;

                                    *value_s = j+1;
                                    *value_e = i;

                                    return 1;
                                }
                            }
                        }
                    }

                    i++;
                }
            }
        }
        else
        {
            i++;
        }
    }
    return 0;
}

/**
*@brief read string in initialization file\n
* retrieves a string from the specified section in an initialization file
*@param section [in] name of the section containing the key name
*@param key [in] name of the key pairs to value 
*@param value [in] pointer to the buffer that receives the retrieved string
*@param size [in] size of value buffer
*@param default_value [in] defualt value of result
*@param file [in] path of the initialization file
*@return 1 : read success; \n 0 : read fail
*/
int zhIniReadString( const char *section, const char *key,char *value,
         int size, const char *default_value, const char *file)
{
    char buf[MAX_FILE_SIZE]={0};
    int file_size;
    int sec_s,sec_e,key_s,key_e, value_s, value_e;

    //check parameters

    assert(section != NULL && strlen(section));
    assert(key != NULL && strlen(key));
    assert(value != NULL);
    assert(size > 0);
    assert(file !=NULL &&strlen(key));

    if(!load_ini_file(file,buf,&file_size))
    {
        if(default_value!=NULL)
        {
            strncpy(value,default_value, size);
        }
        return 0;
    }

    if(!parse_file(section,key,buf,&sec_s,&sec_e,&key_s,&key_e,&value_s,&value_e))
    {
        if(default_value!=NULL)
        {
            strncpy(value,default_value, size);
        }
        return 0; //not find the key

    }
    else
    {
        int cpcount = value_e -value_s;

        if( size-1 < cpcount)
        {
            cpcount = size-1;
        }
    
        memset(value, 0, size);
        memcpy(value,buf+value_s, cpcount );
        value[cpcount] = '\0';

        return 1;
    }
}

/**
*@brief int value in initialization file\n
* retrieves int value from the specified section in an initialization file
*@param section [in] name of the section containing the key name
*@param key [in] name of the key pairs to value 
*@param default_value [in] defualt value of result
*@param file [in] path of the initialization file
*@return profile int value,if read fail, return default value
*/
int zhIniReadInt( const char *section, const char *key,int default_value, 
                const char *file)
{
    char value[32] = {0};
    if(!zhIniReadString(section,key,value, sizeof(value),NULL,file))
    {
        return default_value;
    }
    else
    {
        return atoi(value);
    }
}

/**
 * @brief write a profile string to a ini file
 * @param section [in] name of the section,can't be NULL and empty string
 * @param key [in] name of the key pairs to value, can't be NULL and empty string
 * @param value [in] profile string value
 * @param file [in] path of ini file
 * @return 1 : success\n 0 : failure
 */
int zhIniWriteString(const char *section, const char *key,
                    const char *value, const char *file)
{
    char buf[MAX_FILE_SIZE]={0};
    char w_buf[MAX_FILE_SIZE]={0};
    int sec_s,sec_e,key_s,key_e, value_s, value_e;
    int value_len = (int)strlen(value);
    int file_size;
    FILE *out;

    //check parameters

    assert(section != NULL && strlen(section));
    assert(key != NULL && strlen(key));
    assert(value != NULL);
    assert(file !=NULL &&strlen(key));

    if(!load_ini_file(file,buf,&file_size))
    {
        sec_s = -1;
    }
    else
    {
        parse_file(section,key,buf,&sec_s,&sec_e,&key_s,&key_e,&value_s,&value_e);
    }

    if( -1 == sec_s)
    {
        
        if(0==file_size)
        {
            sprintf(w_buf+file_size,"[%s]\n%s=%s\n",section,key,value);
        }
        else
        {
            //not find the section, then add the new section at end of the file

            memcpy(w_buf,buf,file_size);
            sprintf(w_buf+file_size,"\n[%s]\n%s=%s\n",section,key,value);
        }
    }
    else if(-1 == key_s)
    {
        //not find the key, then add the new key & value at end of the section

        memcpy(w_buf,buf,sec_e);
        sprintf(w_buf+sec_e,"%s=%s\n",key,value);
        sprintf(w_buf+sec_e+strlen(key)+strlen(value)+2,buf+sec_e, file_size - sec_e);
    }
    else
    {
        //update value with new value

        memcpy(w_buf,buf,value_s);
        memcpy(w_buf+value_s,value, value_len);
        memcpy(w_buf+value_s+value_len, buf+value_e, file_size - value_e);
    }
    
    out = fopen(file,"w");
    if(NULL == out)
    {
        return 0;
    }
    
    if(-1 == fputs(w_buf,out) )
    {
        fclose(out);
        return 0;
    }

    fclose(out);
    return 1;
}

int zhIniStrcmp(const char*s,const char*d)
{
	while(*s!='\0')
	{  
		  if(*s!=*d)return -1;
		  s++;
		  d++;
	}
	if(*d=='=')return 0;
	else return -1;
}


TzhIniSection* zhIniReadSection(int* ary_count, const char *file)
{
	char buf[MAX_FILE_SIZE]={0};
    int file_size;
	TzhIniSection* ary_section;
	TzhIniSection* tmp_section;

    //check parameters

    if(!load_ini_file(file,buf,&file_size))
    {
        return 0;
    }

	ary_section=NULL;
	tmp_section=NULL;
	*ary_count=0;

	if(file_size>0)
	{
		int nk=0;
		int i=0;
		int len=0;
		char* pstr;
		
_re_proc:
		while(buf[nk]!=0)
		{
			pstr=&buf[nk];
			//检测第一个字符是否为注释
			if(*pstr==';')
			{
				//移除整行
				while(*pstr!=0 && *pstr!=13 && *pstr!=10)
				{
					pstr++;
					nk++;
				}
			}

			//检测开始
			len=0;
			while(*pstr!=0 && *pstr!=13 && *pstr!=10)
			{
				if(*pstr=='[')
				{
					pstr++;
					nk++;
					
					break;
				}
				pstr++;
				nk++;
			}

			//检测结束
			while(*pstr!=0 && *pstr!=13 && *pstr!=10)
			{
				if(*pstr==']')
				{
					pstr++;
					break;
				}
				pstr++;
				len++;
			}
			if(0==len){
				nk++;
				goto _re_proc;
			}
			if(0==i)
			{
				ary_section=malloc(sizeof(TzhIniSection));
				ary_section->next=NULL;
				ary_section->name=malloc(len+1);
				strncpy(ary_section->name,&buf[nk],len);
				ary_section->name[len]=0;
				//printf("%s\n",ary_section->name);
				tmp_section=ary_section;
				(*ary_count)++;
			}
			else
			{
				tmp_section->next=malloc(sizeof(TzhIniSection));
				tmp_section=tmp_section->next;
				tmp_section->next=NULL;
				tmp_section->name=malloc(len+1);
				strncpy(tmp_section->name,&buf[nk],len);
				tmp_section->name[len]=0;
				//printf("%s\n",tmp_section->name);
				tmp_section=tmp_section;
				(*ary_count)++;
			}
			i++;
			nk+=len;
			nk++;
		}
	}
	return ary_section;
}

void zhIniFreeSection(TzhIniSection*section)
{
	if(section==NULL)
	{return;}

	free_profile_section(section->next);
	if(section->name)
	{
		free(section->name);
		section->name=NULL;
	}
	if(section->next)
	{
		free(section->next);
		section->next=NULL;
	}
}

TzhIniKey* zhIniReadKey(const char *section,int* ary_count, const char *file)
{
	char buf[MAX_FILE_SIZE]={0};
    int file_size;
	TzhIniKey* ary_key;
	TzhIniKey* tmp_key;

    //check parameters

    if(!load_ini_file(file,buf,&file_size))
    {
        return 0;
    }

	ary_key=NULL;
	tmp_key=NULL;
	*ary_count=0;

	if(file_size>0)
	{
		int nk=0;
		int i=0;
		int len=0;
		char* pstr;
		char* psection;
		
_re_proc:
		while(buf[nk]!=0)
		{
			pstr=&buf[nk];
			//检测第一个字符是否为注释
			if(*pstr==';')
			{
				//移除整行
				while(*pstr!=0 && *pstr!=13 && *pstr!=10)
				{
					pstr++;
					nk++;
				}
			}

			//检测开始
			len=0;
			while(*pstr!=0 && *pstr!=13 && *pstr!=10)
			{
				if(*pstr=='[')
				{
					pstr++;
					nk++;
					
					break;
				}
				pstr++;
				nk++;
			}

			//检测结束
			while(*pstr!=0 && *pstr!=13 && *pstr!=10)
			{
				if(*pstr==']')
				{
					pstr++;
					break;
				}
				pstr++;
				len++;
			}
			if(0==len){
				nk++;
				goto _re_proc;
			}

			psection=&buf[nk];
			nk+=len;
			nk++;

			//检测Section是否一样
			if(0==memcmp(psection,section,strlen(section)))
			{
				pstr=&buf[nk];
_nnc:
				//检测整段
				while(*pstr!=0)
				{

					//检测第一个字符是否为注释
					if(*pstr==';')
					{
						//移除整行
						while(*pstr!=0 && *pstr!=13 && *pstr!=10)
						{
							pstr++;
							nk++;
						}
					}
					//检测段结束
					if(*pstr=='[')
					{
						goto _end;
					}

					//检测整行
					len=0;
					while(*pstr!=0 && *pstr!=13 && *pstr!=10)
					{
						len++;
						pstr++;
					}
						
					if(len==0)
					{
						pstr++;
						nk++;
						goto _nnc;
					}

					if(0==i)
					{
						int d=0;
						for(d=0;d<len;d++)
						{
							if(buf[nk+d]=='=')
							{break;}
						}
						ary_key=malloc(sizeof(TzhIniKey));
						ary_key->next=NULL;
						ary_key->name=malloc(d+1);
						strncpy(ary_key->name,&buf[nk],d);
						ary_key->name[d]=0;
						//printf("%s\n",ary_key->name);
						tmp_key=ary_key;
						(*ary_count)++;
					}
					else
					{
						int d=0;
						for(d=0;d<len;d++)
						{
							if(buf[nk+d]=='=')
							{break;}
						}
						tmp_key->next=malloc(sizeof(TzhIniKey));
						tmp_key=tmp_key->next;
						tmp_key->next=NULL;
						tmp_key->name=malloc(d+1);
						strncpy(tmp_key->name,&buf[nk],d);
						tmp_key->name[d]=0;
						//printf("%s\n",tmp_key->name);
						tmp_key=tmp_key;
						(*ary_count)++;
					}
					i++;
					nk+=len;
				}
			}
		}
	}

_end:
	return ary_key;
}

void zhIniFreeKey(TzhIniKey*key)
{
	if(key==NULL)
	{return;}

	free_profile_key(key->next);
	if(key->name)
	{
		free(key->name);
		key->name=NULL;
	}
	if(key->next)
	{
		free(key->next);
		key->next=NULL;
	}
}

#ifdef __cplusplus
}; //end of extern "C" {

#endif
