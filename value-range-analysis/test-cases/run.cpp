#include <cstdlib>
#include <cstdio>
#include <string>
#include <sstream>
#include <cstring>
#include <unistd.h>
using namespace std;
int main(int argc, char const *argv[])
{
	freopen("run-result.txt","w",stdout);
	for(int i=1;i<=34;++i)
	{
		stringstream ss;
		ss<<i;
		//char *num=itoa(i);
		string cmd_str="./llvm-assignment ./assign-3/test"+ss.str()+".opt";
		//printf("test%d.c:\n",i);
		//printf("%s\n",cmd_str.c_str());
		system(cmd_str.c_str());
		//sleep(1);

		//delete num;
		ss.clear();
	}
	printf("done!\n");
	fclose(stdout);
	return 0;
}