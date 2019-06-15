cmake_minimum_required(VERSION 3.5)
project(doom-nukem)

set(CMAKE_C_STANDARD 99)

include_directories(/Users/olaktion/.brew/Cellar/sdl2_image/2.0.4/include/SDL2)
include_directories(/Users/olaktion/.brew/Cellar/sdl2_ttf/2.0.15/include/SDL2)
#include_directories(include/sdl-gpu/include/SDL_gpu.h)
include_directories(include)
include_directories(./SDL/Cellar/sdl2/2.0.9_1/include)

set(SOURCE_FILES
        include/window.h
        include/get_next_line.h
        src/libft/libft.h
        include/parts.h


#        src/ft_math/v2_rotate.c         #not work
#        src/camera/clip_camera.c        #not work
#        src/camera/to_screen.c          #not work
#
#        src/unload.c
#        prend-master/portrend.c
#        src/load_data.c

        src/unload_stable.c
        main.c
        src/load_data_stable.c

#        prend-master/main.c

        src/draw_scr_stable.c
        prend-master/save_frame.c
        src/texture_mapping.c
        src/draw_map.c
        src/poly.c
        src/verfy_map.c
        src/draw_image.c
        src/move_player.c
        src/ft_math/get_next_line.c


#        /*LIBRARY FUNC*/
        src/libft/ft_strchr.c
        src/libft/ft_strcpy.c
        src/libft/ft_strjoin.c
        src/libft/ft_strnew.c
        src/libft/ft_memalloc.c
        src/libft/ft_strlen.c
        src/libft/ft_bzero.c

        src/event.c src/done.c
        src/ft_math/ft_swap.c src/key_event.c src/player_init.c

#       /*READ_FILE*/
        read_file/work_with_file.c
        read_file/validate_vertex.c
        read_file/parse_vertex.c
        read_file/parse_vertex2.c
        read_file/parse_sector.c
        read_file/parse_sector2.c
        read_file/parse_player.c

        #
        src/libft/ft_atoi.c
        src/libft/ft_memalloc.c
        src/libft/ft_isdigit.c
        src/libft/ft_isint.c
        src/libft/ft_isspace.c
        src/libft/ft_memdel.c
        src/libft/ft_lstnew.c
        src/libft/ft_putendl.c
        src/libft/ft_strdel.c
        src/libft/ft_strequ.c
        src/libft/ft_memcpy.c
        src/libft/ft_strndup.c
        src/libft/ft_memcmp.c
        src/libft/ft_strsplit.c
        src/libft/ft_strstr.c
        src/libft/ft_memmove.c
        src/libft/ft_strdup.c
        src/libft/ft_putchar.c
        src/libft/ft_lstadd.c
        src/libft/ft_strclr.c
        src/libft/ft_strsub.c
        )
add_executable(doom-nukem ${SOURCE_FILES})


#link_directories (${PROJECT_SOURCE_DIR}/SDL/Cellar/sdl2/2.0.9_1/)
target_link_libraries(doom-nukem  ${CMAKE_SOURCE_DIR} -L/Users/olaktion/.brew/Cellar/sdl2/2.0.9_1/lib -lSDL2
        -L/Users/olaktion/.brew/Cellar/sdl2_image/2.0.4/lib -lSDL2_image -L/Users/olaktion/.brew/Cellar/sdl2_ttf/2.0.15/lib -lSDL2_ttf)
#target_link_libraries(doom-nuckem ${MINILIBX})
#target_link_libraries(doom-nuckem ${CMAKE_SOURCE_DIR}/libft/libft.a)
