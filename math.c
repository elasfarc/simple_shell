#include "shell.h"

int _pow(int n, int power)
{
	return power == 0
		? 1
		: n * _pow(n, power - 1);
}


