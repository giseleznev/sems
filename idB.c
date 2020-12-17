#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <stdio.h>
#include <grp.h>
 
int main(int ac, char *av[]){
	gid_t GroupList[1000];
	int MaxSize = 1000;
	
	uid_t uid;
	char* pwuid;
	gid_t gid;
	char* grgid;
	int GroupCount;
	
	if (ac - 1){ 
		char* Name = av[1];
	
		uid = getpwnam(Name)->pw_uid;
		pwuid = getpwnam(Name)->pw_name;
	
		gid = getpwnam(Name)->pw_gid;
		grgid =  getgrgid(gid)->gr_name;
		
		GroupCount = getgrouplist(Name, gid,
                        GroupList, &MaxSize);
	} else {
		uid = getuid();
		pwuid = getpwuid(getuid())->pw_name;
		
		gid = getgid();
		grgid =  getgrgid(getgid())->gr_name;
		
		GroupCount = getgroups(MaxSize, GroupList);
	}
	
	printf("uid=%d(%s) gid=%d(%s) ", uid, 
							pwuid, gid, grgid);
	
	printf("группы=");
	for (int i = 0; i < GroupCount; i ++){
		printf("%d(%s),", GroupList[i], 
					getgrgid(GroupList[i])->gr_name);
	}
	printf("\n");
 }
