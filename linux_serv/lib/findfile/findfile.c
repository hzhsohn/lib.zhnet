
#include "findfile.h"

//实现------------------------------------------------------------------
#ifdef _WIN32
    EzhFileAttrib returnFileAttrib(int attrib)
    {
        switch(attrib)
        {
            case _A_ARCH:            
            case _A_ARCH|_A_RDONLY:            
            case _A_ARCH|_A_HIDDEN:
            case _A_ARCH|_A_SYSTEM:            
            case _A_ARCH|_A_SYSTEM|_A_RDONLY:            
            case _A_ARCH|_A_SYSTEM|_A_HIDDEN:            
            case _A_ARCH|_A_HIDDEN|_A_RDONLY:            
            case _A_ARCH|_A_HIDDEN|_A_RDONLY|_A_SYSTEM:            
                return ezhFileAttribFile;
                
            case _A_SUBDIR:
            case _A_SUBDIR|_A_RDONLY:   
            case _A_SUBDIR|_A_HIDDEN:
            case _A_SUBDIR|_A_SYSTEM:
            case _A_SUBDIR|_A_SYSTEM|_A_RDONLY:
            case _A_SUBDIR|_A_SYSTEM|_A_HIDDEN:
            case _A_SUBDIR|_A_HIDDEN|_A_RDONLY:
            case _A_SUBDIR|_A_HIDDEN|_A_RDONLY|_A_SYSTEM:
                return ezhFileAttribDir;           
        }
        return ezhFileAttribUnknow;
    }
    
	int zhFindInit(TzhFileHandle *hle,char*path)
	{
		size_t n=strlen(path);
		n--;
		if('/'!=path[n] && '\\'!=path[n])
		{ sprintf_s(hle->path,sizeof(hle->path),"%s\\*",path); }
		else
		{ sprintf_s(hle->path,sizeof(hle->path),"%s*",path); }

		if ( (hle->h_handle=(long)_findfirst( hle->path,&hle->file))==-1) 
		{return 0;}
		else
		{return 1;}
	}

    int zhFindNext(TzhFileHandle *hle,TzhFileData* out_data)
	{
        if ( -1!=_findnext(hle-> h_handle, &hle->file )) 
        {
			/*
			char path[256];
			char filename[128];
			EzhFileAttrib attrib;
			*/
			strcpy_s(out_data->path,sizeof(out_data->path),hle->path);
			strcpy_s(out_data->filename,sizeof(out_data->filename),hle->file.name);
			out_data->attrib=returnFileAttrib(hle->file.attrib);
			return 1;
        }
		else        
		{return 0;}
    }   
    void zhFindDestory(TzhFileHandle *hle)
	{
		hle->path[0]=0;
		_findclose(hle->h_handle); 
	}

#else
    EzhFileAttrib returnFileAttrib(int attrib)
    {
        switch(attrib)
        {
            case  DT_REG:            
                return ezhFileAttribFile;
            case  DT_DIR:
                return ezhFileAttribDir;               
        }
        return ezhFileAttribUnknow;
    }
	/*
    int zhFindIsDir(const char* path)
    {
        struct stat st;
        lstat(path, &st);
        return S_ISDIR(st.st_mode);
    }
    */
	int zhFindInit(TzhFileHandle *hle,char*path)
	{
		int len;
        int i=0;
        
        len = strlen(path);
        strcpy(hle->path, path);
        for(i=0;i<len;i++)
        {
            if(hle->path[i]=='\\')
            {hle->path[i]='/';}
        }
        if(hle->path[len - 1] == '/'){hle->path[len -1] = '\0';}
		//open
		hle->pdir = opendir(hle->path);
		if(hle->pdir)
		{return 1;}
		else
		{return 0;}
	}

    int zhFindNext(TzhFileHandle *hle,TzhFileData*out_data)
    {
		if((hle->pdirent = readdir(hle->pdir)))
		{
			/*
			char path[256];
			char filename[128];
			EzhFileAttrib attrib;
			*/
			strcpy(out_data->path,hle->path);
			strcpy(out_data->filename,hle->pdirent->d_name);
			out_data->attrib=returnFileAttrib(hle->pdirent->d_type);
			return 1;
		}
		else
		{
			return 0;
		}
    }
	void zhFindDestory(TzhFileHandle *hle)
	{
		hle->path[0]=0;
		closedir(hle->pdir);
	}
#endif
