/*
  zhHttp.h - http moudle
  2013/1/9

  Copyright (C) Han.zhihong
*/
#include "zhHttp.h"

bool zhHttpUrlSplit(const char*szUrl,char* dst_host,int* dst_port,char*dst_file,char* dst_parm)
{
	bool ret;
	int bufsize=0;
	int n=0;
	if(0==memcmp(&szUrl[0],"http:",5) && 
	  (0==memcmp(&szUrl[5],"//",2)||0==memcmp(&szUrl[5],"\\\\",2)))
	{
		int i=0;
		ret=1;

		//http://开始搜索
		i=7;
		while(szUrl[i]!=':' && szUrl[i]!='/' && szUrl[i]!='\\' && szUrl[i]!='?' && szUrl[i]!=0)
		{
			dst_host[bufsize]=szUrl[i];
			bufsize++;
			i++;
		}
		dst_host[bufsize]=0;
		bufsize=0;

		//获取端口
		if(szUrl[i]==':')
		{
			char buf[32]={0};
			
			bufsize=0;
			i++;
			while(szUrl[i]!='?' && szUrl[i]!='/' && szUrl[i]!='\\' && szUrl[i]!=0)
			{
				buf[bufsize]=szUrl[i];
				bufsize++;
				i++;
			}
			buf[bufsize]=0;
			*dst_port=atoi(buf);
		}
		else
		{
			*dst_port=80;
		}

		//获取文件路径
		bufsize=0;
		while(szUrl[i]!='?' && szUrl[i]!=0)
		{
			dst_file[bufsize]=szUrl[i];
			bufsize++;
			i++;
		}
		dst_file[bufsize]=0;

		//获取问号后面的参数
		bufsize=0;
		n=0;
		while(szUrl[i]!=0)
		{
			if(0==n &&'?'==szUrl[i])
			{
				n++;
				i++;
				continue;
			}
			dst_parm[bufsize]=szUrl[i];
			bufsize++;
			i++;
		}
		dst_parm[bufsize]=0;
	}
	else
	{
		ret=0;
		strcpy(dst_host,"");
		*dst_port=0;
		strcpy(dst_parm,"");
	}
	return ret;
}

bool zhHttpGet(const char*szUrl,int begin_byte,int timeout_second,ZH_ONHTTPDATA* pfCallback)
{
	TzhHttpThread* threadParm;
	bool ret;
	SOCKET s;
	
	threadParm=(TzhHttpThread*)malloc(sizeof(TzhHttpThread));
	memset(threadParm,0,sizeof(TzhHttpThread));

	zhSockInit(&s,ZH_SOCK_TCP);
	threadParm->s=s;

	//解析参数
	ret=zhHttpUrlSplit(szUrl,threadParm->host,&threadParm->port,threadParm->file,threadParm->parameter);
	threadParm->method=1;
	threadParm->body=NULL;
	threadParm->body_len=0;
	threadParm->beginByte=begin_byte;
	threadParm->timeout_second=timeout_second;
	threadParm->pfCallback=pfCallback;

	//
	_zhHttpThread_Data(threadParm);
	return ret;
}

bool zhHttpPost(const char*szUrl,const char*body,int body_len,int begin_byte,int timeout_second,ZH_ONHTTPDATA* pfCallback)
{
	TzhHttpThread* threadParm;
	bool ret;
	SOCKET s;
	
	threadParm=(TzhHttpThread*)malloc(sizeof(TzhHttpThread));
	memset(threadParm,0,sizeof(TzhHttpThread));

	zhSockInit(&s,ZH_SOCK_TCP);
	threadParm->s=s;

	//解析参数
	ret=zhHttpUrlSplit(szUrl,threadParm->host,&threadParm->port,threadParm->file,threadParm->parameter);
	threadParm->method=2;
	threadParm->body=(char*)malloc(body_len);
	memcpy(threadParm->body,body,body_len);
	threadParm->body_len=body_len;
	threadParm->beginByte=begin_byte;
	threadParm->timeout_second=timeout_second;
	threadParm->pfCallback=pfCallback;

	//
	_zhHttpThread_Data(threadParm);
	return ret;
}

bool zhHttpPostFile(const char*szUrl,FILE* postFile,int timeout_second,ZH_ONHTTPDATA* pfCallback)
{
	TzhHttpThread* threadParm;
	bool ret;
	SOCKET s;
	
	threadParm=(TzhHttpThread*)malloc(sizeof(TzhHttpThread));
	memset(threadParm,0,sizeof(TzhHttpThread));

	zhSockInit(&s,ZH_SOCK_TCP);
	threadParm->s=s;

	//解析参数
	ret=zhHttpUrlSplit(szUrl,threadParm->host,&threadParm->port,threadParm->file,threadParm->parameter);
	threadParm->method=3;
	threadParm->body=NULL;
	threadParm->body_len=0;
	threadParm->beginByte=0;
	threadParm->posfp=postFile;
	threadParm->timeout_second=timeout_second;
	threadParm->pfCallback=pfCallback;

	//开启线程
	_zhHttpThread_Data(threadParm);
	//
	free(threadParm);
	threadParm=NULL;

	return ret;
}

bool zhHttpSize(const char*szUrl,int timeout_second,ZH_ONHTTPDATA* pfCallback)
{
	TzhHttpThread* threadParm;
	bool ret;
	SOCKET s;
	
	threadParm=(TzhHttpThread*)malloc(sizeof(TzhHttpThread));
	memset(threadParm,0,sizeof(TzhHttpThread));

	zhSockInit(&s,ZH_SOCK_TCP);
	threadParm->s=s;

	//解析参数
	ret=zhHttpUrlSplit(szUrl,threadParm->host,&threadParm->port,threadParm->file,threadParm->parameter);
	threadParm->method=4;
	threadParm->body=NULL;
	threadParm->body_len=0;
	threadParm->beginByte=0;
	threadParm->timeout_second=timeout_second;
	threadParm->pfCallback=pfCallback;

	//专门用来解释参数
	_zhHttpThread_Head(threadParm);
	//
	free(threadParm);
	threadParm=NULL;

	return ret;
}

int zhHttpUrlDecode(char *str, int len)
{
	char *dest = str; 
	char *data = str; 
	int value; 
	int c; 
	while (len--) 
	{ 
	if (*data == '+') 
	{ 
	*dest = ' '; 
	} 
	else if (*data == '%' && len >= 2 && isxdigit((int) *(data + 1))  && isxdigit((int) *(data + 2))) 
	{ 
	c = ((unsigned char *)(data+1))[0]; 
	if (isupper(c)) 
	c = tolower(c); 
	value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16; 
	c = ((unsigned char *)(data+1))[1]; 
	if (isupper(c)) 
	c = tolower(c); 
	value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10; 
	*dest = (char)value ; 
	data += 2; 
	len -= 2; 
	}
	else
	{ 
	*dest = *data; 
	} 
	data++; 
	dest++; 
	} 
	*dest = '\0'; 
	return dest - str; 
}


char * zhHttpUrlEncode(char const *s, int len, int *new_length) 
{ 
	unsigned char const *from, *end; 
	unsigned char c;
	unsigned char hexchars[] = "0123456789ABCDEF";
	unsigned char *to = ( unsigned char *) malloc(3 * len + 1); 
	unsigned char *start = NULL;
	from = (unsigned char *)s; 
	end =  (unsigned char *)s + len; 
	start = to;
	while (from < end) { 
	c = *from++; 
	if (c == ' ') { 
	*to++ = '+'; 
	} else if ((c < '0' && c != '-' && c != '.') || 
	(c < 'A' && c > '9') || 
	(c > 'Z' && c < 'a' && c != '_') || 
	(c > 'z')) { 
	to[0] = '%'; 
	to[1] = hexchars[c >> 4]; 
	to[2] = hexchars[c & 15]; 
	to += 3; 
	} else { 
	*to++ = c; 
	} 
	} 
	*to = 0; 
	if (new_length) { 
	*new_length = to - start; 
	} 
	return (char *) start; 
}

void _zhHttpThread_Data(TzhHttpThread* p)
{
	int doc_total_len;
	bool ret;
	char recv_buf[1024];
	int recv_len;
	int recv_total_len;
	char cache_buf[2048];
	int cache_len;
	char *pSearch;
	int search_len;

	ret=zhSockBlockingConnect(p->s,p->host,p->port,0);

	//连接失败
	if(false==ret)
	{
		p->pfCallback(ezhHttpOperatConnectFail,p->host,p->port,p->file,p->parameter,p->body,p->body_len,NULL,0);
		goto _end;
	}

	p->pfCallback(ezhHttpOperatConnected,p->host,p->port,p->file,p->parameter,p->body,p->body_len,NULL,0);
	switch(p->method)
	{
	case 1://get模式
		{
			//提交HTTP头
			char buf[2048]={0};
			char tmp[256]={0};
			snprintf(tmp,sizeof(tmp),"GET /%s?%s HTTP/1.1\r\n",p->file,p->parameter);
			strcat(buf,tmp);
			snprintf(tmp,sizeof(tmp),"Accept: */*\r\n");
			strcat(buf,tmp);
			snprintf(tmp,sizeof(tmp),"User-Agent: zhHttp/1.0\r\n");
			strcat(buf,tmp);
			snprintf(tmp,sizeof(tmp),"Range: bytes=%u-\r\n",p->beginByte);
			strcat(buf,tmp);
			snprintf(tmp,sizeof(tmp),"Accept-Encoding: gzip, deflate\r\n");
			strcat(buf,tmp);
			//snprintf(tmp,sizeof(tmp),"Referer: http://%s%s \r\n",p->host,p->file);
			//strcat(buf,tmp);
			snprintf(tmp,sizeof(tmp),"Host: %s:%d\r\n",p->host,p->port);
			strcat(buf,tmp);
			snprintf(tmp,sizeof(tmp),"Connection: Keep-Alive\r\n");
			strcat(buf,tmp);
			snprintf(tmp,sizeof(tmp),"\r\n");
			strcat(buf,tmp);
			zhSockSend(p->s,buf,strlen(buf));
		}
		break;
	case 2://post模式
		{
			//提交HTTP头
			char buf[2048]={0};
			char tmp[256]={0};
			int send_len;
			int send_pos;

			if(p->parameter[0])
				{
					snprintf(tmp,sizeof(tmp),"POST /%s?%s HTTP/1.1\r\n",p->file,p->parameter);
				}
				else
				{
					snprintf(tmp,sizeof(tmp),"POST /%s HTTP/1.1\r\n",p->file);
				}
			strcat(buf,tmp);
			snprintf(tmp,sizeof(tmp),"Accept: */*\r\n");
			strcat(buf,tmp);
			snprintf(tmp,sizeof(tmp),"Content-Type: application/x-www-form-urlencoded\r\n");
			strcat(buf,tmp);
			snprintf(tmp,sizeof(tmp),"User-Agent: zhHttp/1.0\r\n");
			strcat(buf,tmp);
			snprintf(tmp,sizeof(tmp),"Range: bytes=%u-\r\n",p->beginByte);
			strcat(buf,tmp);
			snprintf(tmp,sizeof(tmp),"Accept-Encoding: gzip, deflate\r\n");
			strcat(buf,tmp);
			snprintf(tmp,sizeof(tmp),"Referer: http://%s%s \r\n",p->host,p->file);
			strcat(buf,tmp);
			snprintf(tmp,sizeof(tmp),"Host: %s:%d\r\n",p->host,p->port);
			strcat(buf,tmp);
			snprintf(tmp,sizeof(tmp),"Content-Length: %d\r\n",p->body_len);
			strcat(buf,tmp);
			snprintf(tmp,sizeof(tmp),"Connection: Keep-Alive\r\n");
			strcat(buf,tmp);
			snprintf(tmp,sizeof(tmp),"Cache-Control: no-cache\r\n");
			strcat(buf,tmp);
			snprintf(tmp,sizeof(tmp),"\r\n");
			strcat(buf,tmp);
			zhSockSend(p->s,buf,strlen(buf));

			//----------------分割提交
			send_len=p->body_len;
			send_pos=0;
			while(true)
			{
					int ret;
					ret=zhSockSend(p->s,&p->body[send_pos],send_len);
					if(ret>0)
					{
						send_pos+=ret;
						send_len-=ret;
					}
					else if(0==ret)
					{
						zhPlatSleep(1);
					}
					else if(-1==ret)
					{
						p->pfCallback(ezhHttpOperatPostFail,p->host,p->port,p->file,p->parameter,p->body,p->body_len,NULL,0);
						goto _end;
					}
					if(send_len<=0)
					{break;}
			}
			//-----------------
		}
		break;
	case 3://post提交文件模式
		{
				int rlret=0;
				int rlen=0;
				char* read_buf;
				//提交HTTP头
				char tmp[256];
				char subPostDescrpt[32]={0};
				sprintf(subPostDescrpt,"-------%ld",time(NULL));

				if(p->parameter[0])
				{
					snprintf(tmp,sizeof(tmp),"POST /%s?%s HTTP/1.1\r\n",p->file,p->parameter);
				}
				else
				{
					snprintf(tmp,sizeof(tmp),"POST /%s HTTP/1.1\r\n",p->file);
				}
				zhSockSend(p->s,tmp,strlen(tmp));
				snprintf(tmp,sizeof(tmp),"Accept: */*\r\n");
				zhSockSend(p->s,tmp,strlen(tmp));
				snprintf(tmp,sizeof(tmp),"Content-Type: multipart/form-data; boundary=%s\r\n",subPostDescrpt);
				zhSockSend(p->s,tmp,strlen(tmp));
				snprintf(tmp,sizeof(tmp),"User-Agent: zhHttp/1.0\r\n");
				zhSockSend(p->s,tmp,strlen(tmp));
				snprintf(tmp,sizeof(tmp),"Accept-Encoding: gzip, deflate\r\n");
				zhSockSend(p->s,tmp,strlen(tmp));
				snprintf(tmp,sizeof(tmp),"Referer: http://%s%s \r\n",p->host,p->file);
				zhSockSend(p->s,tmp,strlen(tmp));
				snprintf(tmp,sizeof(tmp),"Host: %s:%d\r\n",p->host,p->port);
				zhSockSend(p->s,tmp,strlen(tmp));
				snprintf(tmp,sizeof(tmp),"Content-Length: %d\r\n",p->body_len);
				zhSockSend(p->s,tmp,strlen(tmp));
				snprintf(tmp,sizeof(tmp),"Connection: Keep-Alive\r\n");
				zhSockSend(p->s,tmp,strlen(tmp));
				snprintf(tmp,sizeof(tmp),"Cache-Control: no-cache\r\n");
				zhSockSend(p->s,tmp,strlen(tmp));
				zhSockSend(p->s,"\r\n",2);

				//----------------分割提交
				zhSockSend(p->s,subPostDescrpt,strlen(subPostDescrpt)); 
				zhSockSend(p->s,"\r\n",2);
				//
				if(p->posfp)
				{
						read_buf =(char* )malloc(1024);
						do{
								rlen=fread(read_buf,1,1024,p->posfp);
								if(rlen>0)
								{
									int send_len=rlen;
									while(true)
									{
										rlret=zhSockSend(p->s,read_buf,rlen);
										if(rlret>0)
										{
											send_len-=rlret;
										}
										else if(0==rlret)
										{
											zhPlatSleep(1);
										}
										else if(-1==rlret)
										{
											p->pfCallback(ezhHttpOperatPostFail,p->host,p->port,p->file,p->parameter,p->body,p->body_len,NULL,0);
											goto _end;
										}
										if(send_len<=0)
										{break;}
									}
								}
						}while(rlen>0);
				
						free(read_buf);
						read_buf=NULL;
				}
				//----------------分割提交
				zhSockSend(p->s,subPostDescrpt,strlen(subPostDescrpt)); 
				zhSockSend(p->s,"\r\n",2);
				zhSockSend(p->s,"\r\n",2);
			//-----------------
			//这是协议此功能没做
			/*
				POST /a.php? HTTP/1.1
				Accept: <星号>/<星号>
				Referer: http://192.168.1.160/a.php?
				Accept-Language: zh-cn
				User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022; .NET4.0C)
				Content-Type: multipart/form-data; boundary=---------------------------7dd148211f031c
				Accept-Encoding: gzip, deflate
				Host: 192.168.1.160
				Content-Length: 493
				Connection: Keep-Alive
				Cache-Control: no-cache
				<回车>
				-----------------------------7dd148211f031c
				Content-Disposition: form-data; name="username"

				123
				-----------------------------7dd148211f031c
				Content-Disposition: form-data; name="password"

				cba
				-----------------------------7dd148211f031c
				Content-Disposition: form-data; name="fileField"; filename="新建 文本文档.txt"
				Content-Type: text/plain

				bbb
				-----------------------------7dd148211f031c
				Content-Disposition: form-data; name="button"

				提交
				-----------------------------7dd148211f031c--
				<回车>
			*/
		}
		break;
	}

	//接收返回来的HTTP协议头
	doc_total_len=0;
	cache_len=0;
	memset(cache_buf,0,sizeof(cache_buf));
	while(1)
	{
		bool jmp=false;
		//这里接收的是cache的缓冲区大小
		recv_len=zhSockRecv(p->s,recv_buf,sizeof(recv_buf));
		if(recv_len>0)
		{
			if(cache_len+recv_len>=sizeof(cache_buf))
			{
				p->pfCallback(ezhHttpOperatRecviceFail,p->host,p->port,p->file,p->parameter,p->body,p->body_len,NULL,0);
				goto _end;
			}
			//缓存叠加
			memcpy(&cache_buf[cache_len],recv_buf,recv_len);
			cache_len+=recv_len;
			//检测HTTP头协议完整
			pSearch=cache_buf;
			search_len=0;
			while(pSearch[0]!=0)
			{
				pSearch++;
				search_len++;
				//数据正常
				if(pSearch[0]=='\r' && pSearch[1]=='\n' && pSearch[2]=='\r' && pSearch[3]=='\n')
				{
					search_len+=4;
					jmp=true;
				}
				if(pSearch[0]=='\n' && pSearch[1]=='\n')
				{
					search_len+=2;
					jmp=true;
				}
				if(jmp)
				{
					char a[50]={0};
					char b[50]={0};
					char c[50]={0};
					sscanf(cache_buf,"%s %s %s",a,b,c);
					if(0==strcmp("200",b) || 0==strcmp("206",b))
					{
						//获取数据
						char value[32]={0};
						zhHttpGetProtocolValue(cache_buf,"Content-Length",value);
						doc_total_len=atoi(value);
						goto _data_ok;
					}
					else if(0==strcmp("302",b))
					{
						char value[128]={0};
						zhHttpGetProtocolValue(cache_buf,"Location",value);
						p->pfCallback(ezhHttpOperatPageJump,p->host,p->port,p->file,p->parameter,p->body,p->body_len,value,strlen(value));
						goto _end;
					}
					else
					{
						p->pfCallback(ezhHttpOperatRecviceFail,p->host,p->port,p->file,p->parameter,p->body,p->body_len,NULL,0);
						goto _end;
					}
				}
			}
		}
		else if(0==recv_len)
		{
			zhPlatSleep(1);
		}
		else if(-1==recv_len)
		{
			if(false==jmp)
			{
				p->pfCallback(ezhHttpOperatGetData,p->host,p->port,p->file,p->parameter,p->body,p->body_len,cache_buf,cache_len);
			}
			p->pfCallback(ezhHttpOperatFinish,p->host,p->port,p->file,p->parameter,p->body,p->body_len,NULL,0);
			goto _end;
		}
	}

_data_ok:
	//将cache除去头部其它的缓冲数据回调到函数里
	recv_total_len=0;
	if(cache_len-search_len>0)
	{
		cache_buf[cache_len]=0;
		p->pfCallback(ezhHttpOperatGetData,p->host,p->port,p->file,p->parameter,p->body,p->body_len,&cache_buf[search_len],cache_len-search_len);
		recv_total_len+=(cache_len-search_len);
	}
	//接收返回来的内容
	while(1)
	{
		recv_len=zhSockRecv(p->s,recv_buf,sizeof(recv_buf));
		if(recv_len>0)
		{
			p->pfCallback(ezhHttpOperatGetData,p->host,p->port,p->file,p->parameter,p->body,p->body_len,recv_buf,recv_len);
			recv_total_len+=recv_len;
		}
		else if(0==recv_len)
		{
			zhPlatSleep(1);
		}
		else if(-1==recv_len)
		{
			p->pfCallback(ezhHttpOperatFinish,p->host,p->port,p->file,p->parameter,p->body,p->body_len,NULL,0);
			
			goto _end;
		}
		//接收已经完成
		if(doc_total_len>0 && recv_total_len>=doc_total_len)
		{
			p->pfCallback(ezhHttpOperatFinish,p->host,p->port,p->file,p->parameter,p->body,p->body_len,NULL,0);
			break;
		}
	}
_end:
	zhSockClose(p->s);
	if(p->body)
	{
		free(p->body);
		p->body=NULL;
	}
}

void _zhHttpThread_Head(TzhHttpThread* p)
{
	bool ret;
	char recv_buf[1024];
	int recv_len;
	int doc_total_len;//这里是根据返回来的头确定要接收的长度
	char cache_buf[2048];
	int cache_len;
	char *pSearch;
	int search_len;

	ret=zhSockBlockingConnect(p->s,p->host,p->port,0);

	//连接失败
	if(false==ret)
	{
		p->pfCallback(ezhHttpOperatConnectFail,p->host,p->port,p->file,p->parameter,p->body,p->body_len,NULL,0);
		goto _end;
	}

	p->pfCallback(ezhHttpOperatConnected,p->host,p->port,p->file,p->parameter,p->body,p->body_len,NULL,0);
	switch(p->method)
	{
		case 4://header模式
		{
			//提交HTTP头
			char buf[2048]={0};
			char tmp[256]={0};

			snprintf(tmp,sizeof(tmp),"HEAD /%s HTTP/1.1\r\n",p->file);
			strcat(buf,tmp);
			snprintf(tmp,sizeof(tmp),"Accept: */*\r\n");
			strcat(buf,tmp);
			snprintf(tmp,sizeof(tmp),"User-Agent: zhHttp/1.0\r\n");
			strcat(buf,tmp);
			snprintf(tmp,sizeof(tmp),"Accept-Encoding: gzip, deflate\r\n");
			strcat(buf,tmp);
			//snprintf(tmp,sizeof(tmp),"Referer: http://%s%s \r\n",p->host,p->file);
			//strcat(buf,tmp);
			snprintf(tmp,sizeof(tmp),"Host: %s:%d\r\n",p->host,p->port);
			strcat(buf,tmp);
			snprintf(tmp,sizeof(tmp),"Connection: Keep-Alive\r\n");
			strcat(buf,tmp);
			snprintf(tmp,sizeof(tmp),"\r\n");
			strcat(buf,tmp);
			zhSockSend(p->s,buf,strlen(buf));
		}
		break;
	}

	//接收返回来的HTTP协议头
	doc_total_len=0;
	cache_len=0;
	memset(cache_buf,0,sizeof(cache_buf));
	while(1)
	{
		//这里接收的是cache的缓冲区大小
		recv_len=zhSockRecv(p->s,recv_buf,sizeof(recv_buf));
		if(recv_len>0)
		{
			if(cache_len+recv_len>=sizeof(cache_buf))
			{
				p->pfCallback(ezhHttpOperatRecviceFail,p->host,p->port,p->file,p->parameter,p->body,p->body_len,NULL,0);
				goto _end;
			}
			//缓存叠加
			memcpy(&cache_buf[cache_len],recv_buf,recv_len);
			cache_len+=recv_len;
			//检测HTTP头协议完整
			pSearch=cache_buf;
			search_len=0;
			while(pSearch[0]!=0)
			{
				bool jmp=false;
				pSearch++;
				search_len++;
				//数据正常
				if(pSearch[0]=='\r' && pSearch[1]=='\n' && pSearch[2]=='\r' && pSearch[3]=='\n')
				{
					search_len+=4;
					jmp=true;
				}
				if(pSearch[0]=='\n' && pSearch[1]=='\n')
				{
					search_len+=2;
					jmp=true;
				}
				if(jmp)
				{
					char a[50]={0};
					char b[50]={0};
					char c[50]={0};
					sscanf(cache_buf,"%s %s %s",a,b,c);
					if(0==strcmp("200",b) || 0==strcmp("206",b))
					{
						//获取数据
						char value[32]={0};
						zhHttpGetProtocolValue(cache_buf,"Content-Length",value);
						doc_total_len=atoi(value);
						goto _data_ok;
					}
					else if(0==strcmp("302",b))
					{
						char value[128]={0};
						zhHttpGetProtocolValue(cache_buf,"Location",value);
						p->pfCallback(ezhHttpOperatPageJump,p->host,p->port,p->file,p->parameter,p->body,p->body_len,value,strlen(value));
						goto _end;
					}
					else
					{
						p->pfCallback(ezhHttpOperatRecviceFail,p->host,p->port,p->file,p->parameter,p->body,p->body_len,NULL,0);
						goto _end;
					}
				}
			}
		}
		else if(0==recv_len)
		{
			zhPlatSleep(1);
		}
		else if(-1==recv_len)
		{
			p->pfCallback(ezhHttpOperatFinish,p->host,p->port,p->file,p->parameter,p->body,p->body_len,NULL,0);
			goto _end;
		}
	}

_data_ok:
	p->pfCallback(ezhHttpOperatGetSize,p->host,p->port,p->file,p->parameter,p->body,p->body_len,NULL,doc_total_len);
p->pfCallback(ezhHttpOperatFinish,p->host,p->port,p->file,p->parameter,p->body,p->body_len,NULL,0);

_end:
	zhSockClose(p->s);
	if(p->body)
	{
		free(p->body);
		p->body=NULL;
	}
}

void zhHttpGetProtocolValue(const char*buf,const char*name,char*value)
{
	char* pSearch;
	int nTmp;
	//获取相关信息
	pSearch=(char*)strstr(buf,name);
	if(pSearch)
	{
		//查找冒号
		while(pSearch[0]!=':' && pSearch[0] !='\n' && pSearch[0]!=0 && !(pSearch[0]=='\r'&& pSearch[1]=='\n'))
		{
			pSearch++;
		}
		//移除冒号
		if(*pSearch==':'){pSearch++;}
		//移除空格
		while(pSearch[0]==' '&& pSearch[0]!=0)
		{pSearch++;}
		nTmp=0;
		while(pSearch[0] !='\n' && pSearch[0]!=0 && !(pSearch[0]=='\r'&& pSearch[1]=='\n'))
		{
			value[nTmp]=pSearch[0];
			nTmp++;
			pSearch++;
		}
		value[nTmp]=0;
	}
}

//字符串获取参数的函数---------------------------------------
bool zhHttpGetParameter(const char*str,const char*parameter,char*value)
{
	 //format is "a=123&b=456"
	 #define SPLIT_1         "&"
	 #define SPLIT_2         '='
	 int bRet;
	 char *pSplit;
	 char *pPara;
	 char *p1,*p2;
	 char *pszStr;
	 int nStrLen;
 
	 bRet=0;
	 nStrLen=strlen(str);

	 if (0==nStrLen) {
	  return bRet;
	 }
 
	 pszStr=(char*)malloc(nStrLen+1);
	 memset(pszStr, 0, nStrLen+1);
	 strcpy(pszStr,str);
 
	 pPara=strtok_r(pszStr,SPLIT_1, &pSplit);
 
	 do{
		  p1=pPara;
		  p2=strchr(pPara, SPLIT_2);
		  if(p2)
		  {
			  *p2=0x00;
			  p2++;
			  if (0==strcmp(parameter, p1)) {
				strcpy(value, p2);
				bRet=1;
			   break;
			  }
		  }
	 }while ((pPara=strtok_r(NULL,SPLIT_1, &pSplit)));
	 free(pszStr);
	 pszStr=NULL;
	 return bRet;
}


///////////////////////////// 不区分大小写查找字符 ///////
char *zhHttpStrstri(const char *phaystack, const char *pneedle)
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
