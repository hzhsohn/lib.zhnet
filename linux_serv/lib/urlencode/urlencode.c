#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "urlencode.h"


int	ecUrldecode(char *str, int len)
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
	return (int)(dest - str); 
}


char * ecUrlencode(char const *s, int len, int *new_length) 
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
