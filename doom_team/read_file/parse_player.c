
#include "player_stable.h"
#include "math.h"

void		parse_player(t_system *system, t_list *list) // ???
{
	t_list *elem;

	elem = find_elem(list, "player");
	*system->player = parse_player_parameters((char *)elem->content, system);
}

t_player	parse_player_parameters(char *data, t_system *system)
{
	char		**tmp;
	t_player	player;

	ft_bzero(&player, sizeof(t_player));
	if (data)
	{
		tmp = ft_strsplit(ft_strchr(data, '\t'), '\t');
		if (two_dim_len(tmp) == 2)
		{
			player.where = define_position(tmp[M1(1)]); // .position
			player.anglesin = sinf(player.angle);
			player.anglecos = cosf(player.angle);
			if (!int_error(&tmp[M1(2)]))
				player.num_sect = ft_atoi(tmp[M1(2)]); // sec
		}
		else
			print_error("Error, bad vector position");
		two_dim_del(&tmp);
	}

//	player = (t_player) {{player.where.x,player.where.y, 0}, {0, 0, 0}, player.angle, 0, 0, 0, player.num_sect};
//	player.where.z = system->sectors[player.num_sect].floor + EyeHeight;
//	player.anglesin = sinf(player.angle);
//	player.anglecos = cosf(player.angle);
//	printf("%s","сектор игрока :");
//	printf("%d\n", player.num_sect); //sec
	return (player);
}

t_xyz 		define_position(char *data) //t_vertex
{
	t_xyz		position; //t_vertex
	char		**tmp;

	ft_bzero(&position, sizeof(t_xyz)); //t_vertex
	if (data)
	{
		tmp = ft_strsplit(data, ' ');
		if (two_dim_len(tmp) == 3 && !int_error(tmp))
		{
			position.x = ft_atoi(tmp[M1(1)]);
			position.y = ft_atoi(tmp[M1(2)]);
			position.z = ft_atoi(tmp[M1(3)]);
			position.z += EyeHeight;

		}
		else
			print_error("Error, bad coords for player");
		two_dim_del(&tmp);
	}
	printf("%s\n", "x ,y, z of player:");
	printf("%f\n", position.x);
	printf("%f\n", position.y);
	printf("%f\n", position.z);
	return (position);
}

//float take_x(t_xy elem)
//{
//	return (elem.x);
//}
//float take_y(t_xy elem)
//{
//	return (elem.y);
//}

int32_t			*parse_neighbours(char *data, t_sector *sector) {

	int32_t *neighbours;
	char **tmp;
	int i;
	size_t idx;

	tmp = ft_strsplit(data, ' ');
	i = -1;
	neighbours = ft_memalloc(sizeof(int32_t) * sector->ver_arr_len);
	sector->transit_array = ft_memalloc(sizeof(t_wall) * (sector->ver_arr_len + 1));
	while (tmp[++i])
		neighbours[i] = ft_atoi(tmp[i]);
	while (i--){
		idx = (i + 1) % sector->ver_arr_len;
		sector->transit_array[idx].is_wall = neighbours[idx];
		sector->transit_array[idx].start_wall[0] = sector->vertex[i].x;
		sector->transit_array[idx].start_wall[1] = sector->vertex[i].y;
		sector->transit_array[idx].end_wall[0] = sector->vertex[idx].x;
		sector->transit_array[idx].end_wall[1] = sector->vertex[idx].y;
	}
	return neighbours;
}



//t_sector			*allocate_transit_arr(size_t size)
//{
//	t_sector		*extra;
//
//	if (!(extra = (t_sector *)ft_memalloc(sizeof(t_sector) * size)))
//		print_error("Can't allocate memory");
//	return (extra);
//}
