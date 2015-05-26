#include "Structures\BinStr.h"
#include <stdio.h>

int main() {
	BinStr a = create_BinStr("11", 2);
	BinStr b = create_BinStr("11", 2);
	BinStr c = add(a, b);
	print(c);
	destroy_BinStr(a);
	destroy_BinStr(b);
	destroy_BinStr(c);
	return 0;
}
