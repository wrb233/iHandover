#define IHandover_VERSION_STR "0.0.2"
#define IHandover_REMARK_STR  "show tableviews by zw 2020-03-25,V0.0.1.\
add print function by wl 2020-06-05,V0.0.2."

#include "version.h"

//打印版本信息函数
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