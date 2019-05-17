NAME :=				map_editor

SRC_DIR :=			./src/
OBJ_DIR :=			./obj/
INC_DIR :=			./inc/

SRC :=              main.c\

OBJ =				$(addprefix $(OBJ_DIR), $(SRC:.c=.o))

INC :=				doom.h
HEADER :=			$(addprefix $(INC_DIR), $(INC))

LIBFT =				$(LIBFT_DIR)libft.a
LIBFT_DIR :=		$(LIB_DIR)libft/
LIBFT_INC :=		$(LIBFT_DIR)includes/
LIBFT_FLAGS :=		-lft -L $(LIBFT_DIR)

SDL_INC :=			-I./frameworks/SDL2.framework/Headers\
                    -I./frameworks/SDL2_ttf.framework/Headers\
                    -I./frameworks/SDL2_mixer.framework/Versions/A/Headers

FRM :=              -F./frameworks\
                    -rpath ./frameworks\
                    -framework SDL2 -framework SDL2_ttf -framework SDL2_mixer -framework OpenCL

SDL :=				-F /Library/Frameworks/ -framework SDL2 -F /Library/Frameworks/ -framework SDL2_image
OPENGL :=			-framework OpenGL
APPKIT :=			-framework AppKit
FRAMEWORKS :=		$(SDL) $(OPENGL) $(APPKIT)

CC_FLAGS :=			-Wall -Wextra -Werror
LINK_FLAGS :=		$(MLX_FLAGS) $(LIBFT_FLAGS)
HEADER_FLAGS :=		-I $(INC_DIR)  -I $(LIBFT_INC) $(SDL_INC)

CC :=				gcc

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
		gcc $(OBJ) $(LINK_FLAGS) $(FRM) -o $(NAME)

$(OBJ): | $(OBJ_DIR)

$(OBJ_DIR):
		mkdir $(OBJ_DIR)

$(OBJ_DIR)%.o: %.c $(HEADER)
		$(CC) -c $< -o $@ $(CC_FLAGS) $(HEADER_FLAGS)

$(LIBFT):
		make -C $(LIBFT_DIR)

clean:
		rm -f $(OBJ)
		make clean -C $(LIBFT_DIR)

fclean: clean
		rm -f $(NAME)
		rm -rf $(OBJ_DIR)
		make fclean -C $(LIBFT_DIR)

re: fclean all

vpath %.c $(SRC_DIR)
