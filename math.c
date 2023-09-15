#include "shell.h"

/**
 * _pow - get the result of an integer @n powerd to @power.
 * @n: the base.
 * @power: the power.
 *
 * Return: (int) @n power @power.
 */
int _pow(int n, int power)
{
	return (power == 0
		? 1
		: n * _pow(n, power - 1));
}


