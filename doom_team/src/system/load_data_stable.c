#include "player_stable.h"

void    LoadData_stable(t_player *player, t_system *system)
{
    char    word[256];
    char    *ptr;
    char    *line;
    t_xy    vertex[MAX_VERTICLES];
    t_xy    *vertexptr;
    float   x,y,angle,number, numbers[MAX_EDGES];
    int     n, m;
    int     fd;

    fd = open("map.txt", O_RDONLY);
    vertexptr = vertex;
    if(!fd)
    {
        perror("map.txt");
        exit(1);
    }

    while(get_next_line(fd, &line))
    {
        switch (sscanf(ptr = line, "%32s%n", word, &n) == 1 ? word[0] : '\0') {
            case 'v': // vertex
                for (sscanf(ptr += n, "%f%n", &y, &n); sscanf(ptr += n, "%f%n", &x, &n) == 1;) {
                    if (vertexptr >= vertex + MAX_VERTICLES) {
                        fprintf(stderr, "ERROR: Too many vertices, limit is %u\n", MAX_VERTICLES);
                        exit(2);
                    }
                    *vertexptr++ = (t_xy) {x, y};
                }
                break;
            case 's': // sector
                system->sectors = realloc(system->sectors, ++system->NumSectors * sizeof(*system->sectors));
                t_sector *sect = &system->sectors[system->NumSectors - 1];
                sscanf(ptr += n, "%f%f%n", &sect->floor, &sect->ceil, &n);
                for (m = 0; sscanf(ptr += n, "%32s%n", word, &n) == 1 && word[0] != '#';) {
                    if (m >= MAX_EDGES) {
                        fprintf(stderr, "ERROR: Too many edges in sector %u. Limit is %u\n", system->NumSectors - 1, MAX_EDGES);
                        exit(2);
                    }
                    numbers[m++] = word[0] == 'x' ? -1 : strtof(word, 0);
                }
                sect->npoints = m /= 2;
                sect->neighbours = malloc((m) * sizeof(*sect->neighbours));
                sect->vertex = malloc((m + 1) * sizeof(*sect->vertex));
#ifdef VisibilityTracking
                sect->visible = 0;
#endif
                for (n = 0; n < m; ++n)
                    sect->neighbours[n] = numbers[m + n];
                for (n = 0; n < m; ++n) {
                    int v = numbers[n];
                    if (v >= vertexptr - vertex) {
                        fprintf(stderr, "ERROR: Invalid vertex number %d in sector %u; only have %u\n",
                                v, system->NumSectors - 1, (int) (vertexptr - vertex));
                        exit(2);
                    }
                    sect->vertex[n + 1] = vertex[v]; // TODO: bounds checking
                }
                sect->vertex[0] = sect->vertex[m];
                break;
#ifdef LightMapping
            case 'l': // light
                lights = realloc(lights, ++NumLights * sizeof(*lights));
                struct light* light = &lights[NumLights-1];
                sscanf(ptr += n, "%f %f %f %f %f %f %f", &light->where.x, &light->where.z, &light->where.y,
                                            &number,     &light->light.x, &light->light.y, &light->light.z);
                light->sector = (int)number;
                break;
#endif
            case 'p': // player
                sscanf(ptr += n, "%f %f %f %f", &x, &y, &angle, &number);

                *player = (t_player) {{x, y, 0}, {0, 0, 0}, angle, 0, 0, 0, number};
                player->where.z = system->sectors[player->num_sect].floor + EyeHeight;
                player->anglesin = sinf(player->angle);
                player->anglecos = cosf(player->angle);
        }
        free(line);
    }
    free(line);
    close(fd);
}