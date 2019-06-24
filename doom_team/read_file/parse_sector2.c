
#include "player_stable.h"

t_sector		parse_height_params(char *data)
{

	t_sector	height;
	char		**tmp;

	ft_bzero(&height, sizeof(t_sector));
	if (data)
	{
		tmp = ft_strsplit(data, ' ');
		if (two_dim_len(tmp) == 2 && !int_error(tmp))
		{
			height.floor = ft_atoi(tmp[M1(1)]); // все нормально записывает в формате х.000000
			height.ceil = ft_atoi(tmp[M1(2)]); // все нормально записывает в формате х.000000
		}
		else
			print_error("Error, wrong sector height param");
		two_dim_del(&tmp);
	}
	return (height);
}

t_xy		*parse_vertex_pair(t_system *system, char **data, size_t size)
{
	t_xy	*vertex_array;
	int32_t	val;
	size_t	i;
	size_t j;

	j = 0;
	i = 1;
	vertex_array = (t_xy *)ft_memalloc((size + 1) * sizeof(t_xy));
//size кол-во элем в строке(вер пар)
	while (i <= size)
	{
		val = ft_atoi(data[j]);
		if (ABS(val) < system->sum_vert_pair)
		{
			vertex_array[i]     = (*search_vertex_pair(system, val));
			if (i == size)
				vertex_array[0] = vertex_array[i];
		}
		else
			print_error("Error, no vertex pair");
		i++;
		j++;
	}
//	transform_ver_arr(system, size);
	return (vertex_array);
}

t_xy		*search_vertex_pair(t_system *system, int32_t pair)
{
	t_xy	*vertex_pair;
	size_t	i;
	size_t	j;

	i = 0;
	vertex_pair = allocate_vertex_pair();
//	vertex_pair = ft_memalloc(sizeof(t_xy));

	while (i <= system->sum_vert)
	{
		j = 0;
		while (j < system->vertex[i].x_arr_size)
		{
			if (pair == 0)
			{
				vertex_pair->y = system->vertex[i].y;
				vertex_pair->x = system->vertex[i].x_arr[j];
				return (vertex_pair);
			}
			pair--;
			j++;
		}
		i++;
	}
	return (NULL);
}

double			*allocate_vertex_pair(void)
{
	double	*vertex;

	if (!(vertex = (double *)ft_memalloc(sizeof(double) * 2)))  // + 1
		print_error("Error, can't allocate memory region");
	return (vertex);
}

//t_sector 		*pars_extra_par(char *data, char *data1, t_sector *sector)
//{
//	t_sector	*trans;
//	char		**tmp;
//
//	trans = NULL;
//	if (data)
//	{
//		tmp = ft_strsplit(data, ' ');
//		if (two_dim_len(tmp) == sector->ver_arr_len && !int_error(tmp))
//			;
////			trans = parse_extra_pair(tmp, &data1, sector);  //tmp1
//		else
//			print_error("Error, sector walidation, bad params");
//		two_dim_del(&tmp);
//	}
//	return (trans);
//}
