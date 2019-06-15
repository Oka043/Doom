#include "ft_math.h"

t_vector		v2_rotate(t_vector v, float acos, float asin)
{
	return ((t_vector) {v.x * acos - v.y * asin, v.x * asin + v.y * acos});
}