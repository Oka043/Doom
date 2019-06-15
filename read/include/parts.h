#ifndef PARTS_H
# define PARTS_H

#include <SDL2/SDL.h>
//#include <SDL_ttf.h>
#include <unistd.h>
#include <stdio.h>
# include "../src/libft/libft.h"
#include <fcntl.h>

# define ABS(X)		(((X) < 0) ? (-(X)) : (X))
# define M1(X)		((X) - 1)

typedef struct 	s_vertex
{
	double 		*x_arr;
	size_t		x_arr_size;
	double		x;
	double		y;
	double		z;

}				t_vertex;

typedef	struct	s_height
{
	double		floor;
	double		cell; // пока ничего
}				t_height;

typedef	struct	s_sec
{
	size_t		color;
	size_t		sec_index;	// номер сектора	
	double		**vertex;
	size_t		ver_arr_len;
	t_height	height;
	int32_t		*sec_dots;	// массив точек сектора
}				t_sec;

typedef	struct	s_sdl
{
//	TTF_Font	*font;
	SDL_Window	*window;
	SDL_Surface *winsurface;
//	SDL_Surface	*skybox;
	SDL_Event	event;

}				t_sdl;

typedef struct 	s_tplayer
{
	int32_t		sec; // сектор в котором стоит
	t_vertex	pos;	// позиция игрока

}				t_tplayer;

typedef	struct	s_main
{
	t_vertex	*vertex; // массив структур всех точек карты
	t_sec		*sec; // массив структур всех секторов
	size_t		sum_sect;	
	size_t		sum_vert;	
	size_t		sum_vert_pair;
	t_sdl		sdl;   // все для работы с sdl

	t_tplayer	player;	// потом из общего файла допилить все данные о игроке
	SDL_Surface	*texture;
}				t_main;


t_main 			*init_main(void);
void    		sdl_init(t_main *main);

/*
work_with_file
*/

void			read_file(t_main *main);
void 			print_error(const char *msg); 
void			tmp_list(t_list **lst, const void *content,
				const size_t content_size);
void        	free_tmp_list(t_list **head); // если будет отдельный файл free можно перенсти туда
void			init_all(t_main *main);

/*
parse_vertex
*/
void			parse_vertex(t_main *main, t_list *list);
t_list			*find_elem(t_list *list, char *elem);
size_t 			content_len(t_list *list, char *content);
t_vertex 		*allocate_vertex(size_t size);
double			**pars_ver_par(char *data, t_main *main, size_t *len);

/*
validate_vertex
*/
t_bool			validate_vertex(char *vertex_data);
t_bool			validate_dilims(char *vertex_data);
t_bool			validate_coords(char *vertex_data);
t_bool			validate_num(char *vertex_data);
double			*allocate_vertex_x_array(size_t size);

/*
parse_vertex2
*/
double			parse_ver_y(char *vertex_line);
double			*parse_ver_x(char *vertex_line, size_t *size_arr);
char			*find_x_values(char *vertex_line);
size_t			two_dim_len(char **array);
void			two_dim_del(char ***array);

/*
parse_sector
*/
void			parse_sec(t_main *main, t_list *list);
t_sec			*allocate_sec(size_t size);
t_sec			processing(t_main *main, size_t index, char *sec_data);
t_bool			integer_overload_error(char **data);
double			**allocate_vertex_array(size_t size);

/*
parse_sector2
*/
t_height		parse_height_params(char *data);
double			**parse_vertex_pair(t_main *main, char **data, size_t size);
double			*search_vertex_pair(t_main *main, int32_t pair);
double			*allocate_vertex_pair();
int32_t			*pars_extra_par(char *data, size_t vertex_array_len);

/*
parse_player
*/
void			parse_player(t_main *main, t_list *list);
t_tplayer		parse_player_parameters(char *data);
t_vertex		define_pos(char *data);
int32_t			*parse_extra_pair(char **data, size_t size);
int32_t			*allocate_extra_arr(size_t size);

#endif