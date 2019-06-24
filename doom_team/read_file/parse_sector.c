
#include "player_stable.h"

void			parse_sec(t_system *system, t_list *list)
{
	t_list	*elem;
	size_t	len;
	size_t	i;

	i = 0;
	elem = find_elem(list, "sector");
	len = content_len(elem, "sector");
	system->sectors = allocate_sec(len);
	while (elem && i < len)
	{
		system->sectors[i] = processing(system, i, (char *)elem->content);
		elem = elem->next;
		i++;
	}
	system->NumSectors = len; // 25 cчитает нормально
	printf("%s", "сумма секторов (25) :");
	printf("%u\n", system->NumSectors);
}

t_sector			*allocate_sec(size_t size)
{
	t_sector *sector;

	if (!(sector = (t_sector *)ft_memalloc(sizeof(t_sector) * size)))
		print_error("Error can't allocate memory region");
	return (sector);
}

t_sector	processing(t_system *system, size_t index, char *sector_data)
{
	t_sector		sector;
	char		**dat_arr;

	ft_bzero(&sector, sizeof(t_sector));
	if (sector_data)
	{
		dat_arr = ft_strsplit(ft_strchr(sector_data, '\t'), '\t');
		if (dat_arr && two_dim_len(dat_arr) == 3)
		{
			sector = parse_height_params(dat_arr[M1(1)]);
			sector.vertex = pars_ver_par(dat_arr[M1(2)], system, &sector.ver_arr_len);
			sector.neighbours = parse_neighbours(dat_arr[M1(3)], &sector);
//			transform_ver_arr(system);
//			sector.transit_array = pars_extra_par(dat_arr[M1(3)], sector.ver_arr_len, sector.vertex, dat_arr[M1(2)]);
//			pars_extra_par(dat_arr[M1(3)], dat_arr[M1(2)], &sector);
			sector.npoints = sector.ver_arr_len;
			two_dim_del(&dat_arr);
			
		}
		else
			print_error("Error, wrong sector param number1");
	}

	return (sector);
}

t_bool			int_error(char **data)
{
	while (*data)
	{
		if (!ft_isint(*data, false) && !ft_strchr(*data, '.'))
			print_error("Error, int overload");
		data++;
	}
	return (false);
}

double			**allocate_vertex_array(size_t size)
{
	double **array;

	if (!(array = (double **)ft_memalloc(sizeof(double *) * size + 1)))
		print_error("Error, can't allocate memory region");
	return (array);
}
