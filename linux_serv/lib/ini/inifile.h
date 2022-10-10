#ifndef INI_FILE_H_
#define INI_FILE_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _TzhIniSection
{
	char* name;
	struct _TzhIniSection* next;
}TzhIniSection;

typedef struct _TzhIniKey
{
	char* name;
	struct _TzhIniKey* next;
}TzhIniKey;

int zhIniReadString( const char *section, const char *key,char *value, int size,const char *default_value, const char *file);
int zhIniReadInt( const char *section, const char *key,int default_value, const char *file);
int zhIniWriteString( const char *section, const char *key,const char *value, const char *file);
int zhIniStrcmp(const char*s,const char*d);

//read select group ,must be free memory
TzhIniSection* zhIniReadSection(int* ary_count, const char *file);
void zhIniFreeSection(TzhIniSection*section);

TzhIniKey* zhIniReadKey(const char *section,int* ary_count, const char *file);
void zhIniFreeKey(TzhIniKey*key);


//redefine function
#define read_profile_string         zhIniReadString         
#define read_profile_int            zhIniReadInt            
#define write_profile_string        zhIniWriteString    

#define read_profile_section		zhIniReadSection
#define free_profile_section		zhIniFreeSection
#define read_profile_key			zhIniReadKey
#define free_profile_key			zhIniFreeKey


#ifdef __cplusplus
}; //end of extern "C" {

#endif

#endif
