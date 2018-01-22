/*
 * libnss_pool.c
 * Author: Marcin Stolarek (stolarek.marcin@gmail.com)
*/

#include <nss.h>
#include <pwd.h>
#include <shadow.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//This defines the maximal size of the pool stored in /etc/pool-passwd
#define MAX_POOL_SIZE 20

//poolPasswd file
#define POOLPASSWD "/etc/pool-passwd"


enum nss_status
_nss_pool_getpwnam_r( const char *name, 
	   	     struct passwd *p, 
	             char *buffer, 
	             size_t buflen, 
	             int *errnop)
{

	FILE * fd, * log;
	struct passwd *it;
	short created=0;
	fd=fopen(POOLPASSWD,"r");
	if(fd==NULL) {
		return NSS_STATUS_NOTFOUND;
	}
	log=fopen("/tmp/debug","w+");
	setbuf(log,NULL);
	*buffer=NULL;
	while ( (it=fgetpwent(fd)) != NULL) {

		fprintf(log,"Read line: %s\n",it->pw_name);
		if(!strcmp(name,it->pw_name) && !(strcmp("x",it->pw_passwd)) && !created ) {
			fprintf(log,"Match %s \n",it->pw_name);
			*p=*it;
			//entry was there no need to process further
			fclose(log);
			return NSS_STATUS_SUCCESS;
			
		}
		//if passwd set to ! then it means that entry is empty and can be used for new user
		else if(!strcmp("!",it->pw_passwd) && !created ) {
			fprintf(log,"new %s \n",it->pw_name);
			strcpy(it->pw_name,name); //this may be an issue if previous login name was shorter then new one
			fprintf(log,"new %s \n",it->pw_name);
			strcpy(it->pw_passwd,"x");
			*p=*it;
			created=1;
		}
		
		fprintf(log,"snprintf, when strlen(buffer)=%d and buflen=%d\n",(int)strlen(buffer),(int)buflen);
		fprintf(log,"Adding to pool-passwd: %s:%s:%d:%d:%s:%s:%s\n",it->pw_name,it->pw_passwd,it->pw_uid,it->pw_gid,it->pw_gecos,it->pw_dir,it->pw_shell);

		int howLong=snprintf(buffer+strlen(buffer),buflen,"%s:%s:%d:%d:%s:%s:%s\n",it->pw_name,it->pw_passwd,it->pw_uid,it->pw_gid,it->pw_gecos,it->pw_dir,it->pw_shell);

		fprintf(log,"checking buffer\n");
		if(howLong+1>buflen) {
			fprintf(log,"try again\n");
			fclose(log);
			return NSS_STATUS_TRYAGAIN;
		}
		
	}
	fclose(fd);

	if(!created) {
		return NSS_STATUS_NOTFOUND;
	}

	//If we are here it means that new user was created (name changed) in pool
	fd=fopen(POOLPASSWD,"w");
	fprintf(fd,"%s",buffer);
	fclose(fd);

	fclose(log);
	return NSS_STATUS_TRYAGAIN; //Means that there are no more entries, we don't implement users enumeration
	
}

enum nss_status
_nss_pool_getpwuid_r(uid_t uid, struct passwd *p,
            char *buf, size_t buflen, struct passwd **result)
{

	FILE * fd, * log;
	struct passwd *it;
	fd=fopen(POOLPASSWD,"r");
	if(fd==NULL) {
		return NSS_STATUS_NOTFOUND;
	}
	log=fopen("/tmp/debugUid","w+");
	setbuf(log,NULL);
	while( (it=fgetpwent(fd)) != NULL) {
			fprintf(log,"%s\n",it->pw_name);
			if(it->pw_uid==uid) {
				*p=*it;
				fclose(log);
				fclose(fd);
				return NSS_STATUS_SUCCESS;
			}
	}
		
	fclose(log);
	fclose(fd);
	return NSS_STATUS_NOTFOUND;
}
