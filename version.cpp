#define IHandover_VERSION_STR "0.0.1"
#define IHandover_REMARK_STR  "show tableviews by zw 2020-03-25,V0.0.1."

#include "version.h"

//��ӡ�汾��Ϣ����
bool echoVersion(int argc, char *argv[]){
	for(int i = 0;i < argc;i++){
		if(qstricmp(argv[i],"-v") == 0 || qstricmp(argv[i],"--version") == 0 ){
			std::cout << "Version: " << IHandover_VERSION_STR << std::endl;
			std::cout << "Remark : " << IHandover_REMARK_STR  << std::endl;
			return true;
		} 
	}
	return  false;
}