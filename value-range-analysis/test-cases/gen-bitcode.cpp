#include <cstdlib>
#include <cstdio>
#include <string>
#include <sstream>
#include <cstring>
using namespace std;
int main(int argc, char const *argv[])
{
	for(int i=1;i<=34;++i)
	{
		stringstream ss;
		ss<<i;
		//char *num=itoa(i);
		string cmd_str="clang -c -emit-llvm -g3 test"+ss.str()+".c";
		printf("%s\n",cmd_str.c_str());
		system(cmd_str.c_str());

		cmd_str="opt -S -mem2reg test"+ss.str()+".bc -o test"+ss.str()+".opt";
		printf("%s\n",cmd_str.c_str());
		system(cmd_str.c_str());

		cmd_str="opt -dot-cfg test"+ss.str()+".opt";
		printf("%s\n",cmd_str.c_str());
		system(cmd_str.c_str());

		cmd_str="mv cfg.foo.dot test"+ss.str()+".dot";
		printf("%s\n",cmd_str.c_str());
		system(cmd_str.c_str());
		//delete num;
		ss.clear();
	}
	printf("done!\n");
	return 0;
}