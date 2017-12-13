#include <string>
#define inspect(x) std::cerr << #x << " is " << x << " in file " << __FILE__ << " at line " << __LINE__ << std::endl
using namespace std;

class SMLRunner{
public:
	SMLRunner();
	bool loadFile(string fname);
	bool exec();
	void reset();
private:
	bool isWord(int w) const;
	int getCommand(int w) const;
	int getAddr(int w) const;
	void handleInput(int address);
	void overflow(int address);
	void memDump();
	int ACC;
	int CMD;
	int MEM[100];
};

