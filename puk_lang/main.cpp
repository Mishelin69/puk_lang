#include "parser/header/Pukser.hpp"

#if defined __GNUC__
	#define start_testfile(n) \
		n.parse("./test.puk");
#else
	#define start_testfile(n) \
		n.parse("../test.puk");
#endif

int main() {

	Pukser::Pukser puk;

	start_testfile(puk);

	return 0;
}
