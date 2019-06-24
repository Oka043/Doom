#include "player.h"



t_sector* LoadData(t_player *player, t_system *system, t_sector *sectors)
{
    FILE* fp = fopen("map.txt", "rt");
    if(!fp) { perror("map.txt"); exit(1); }
    char Buf[256], word[256], *ptr;
    t_xy vertex[MaxVertices], *vertexptr = vertex;
    float x,y,angle,number, numbers[MaxEdges];
    int n, m;
    while(fgets(Buf, sizeof Buf, fp))
        switch(sscanf(ptr = Buf, "%32s%n", word, &n) == 1 ? word[0] : '\0')
        {
            case 'v': // vertex
                for(sscanf(ptr += n, "%f%n", &y, &n); sscanf(ptr += n, "%f%n", &x, &n) == 1; )
                {
                    if(vertexptr >= vertex+MaxVertices) { fprintf(stderr, "ERROR: Too many vertices, limit is %u\n", MaxVertices); exit(2); }
                    *vertexptr++ = (t_xy) { x, y };
                }
                break;
            case 's': // sector
                sectors = realloc(sectors, ++system->NumSectors * sizeof(*sectors));
                system->sectors = &sectors[system->NumSectors-1];
                sscanf(ptr += n, "%f%f%n", &system->sectors->floor,&system->sectors->ceil, &n);
                for(m=0; sscanf(ptr += n, "%32s%n", word, &n) == 1 && word[0] != '#'; )
                {
                    if(m >= MaxEdges)
                    { fprintf(stderr, "ERROR: Too many edges in sector %u. Limit is %u\n", system->NumSectors-1, MaxEdges); exit(2); }
                    numbers[m++] = word[0]=='x' ? -1 : strtof(word,0);
                }
                system->sectors->npoints   = m /= 2;
                system->sectors->neighbours = malloc( (m  ) * sizeof(*system->sectors->neighbours) );
                system->sectors->vertex    = malloc( (m+1) * sizeof(*system->sectors->vertex)    );
#ifdef VisibilityTracking
                sect->visible = 0;
#endif
                for(n=0; n<m; ++n)
                    system->sectors->neighbours[n] = numbers[m + n];
                for(n=0; n<m; ++n)
                {
                    int v = numbers[n];
                    if(v >= vertexptr-vertex)
                    { fprintf(stderr, "ERROR: Invalid vertex number %d in sector %u; only have %u\n",
                              v, system->NumSectors-1, (int)(vertexptr-vertex)); exit(2); }
                    system->sectors->vertex[n+1]  = vertex[v]; // TODO: bounds checking
                }
                system->sectors->vertex[0] = system->sectors->vertex[m];
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
                sscanf(ptr += n, "%f %f %f %f", &x,&y, &angle,&number);

                *player = (t_player){{x,y,0},{0,0,0}, angle,0,0,0, number };
                player->where.z = sectors[player->num_sect].floor + EyeHeight;
                player->anglesin = sinf(player->angle);
                player->anglecos = cosf(player->angle);
        }
    fclose(fp);
    return sectors;
}