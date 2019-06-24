#include "player_stable.h"

// Verify map for consistencies
void verify_map(t_system *system, t_sector *sectors)
{
    //int phase=0;
    Rescan:
    //DrawMap(); SDL_Delay(100);

    // Verify that the chain of vertexes forms a loop.
    for(unsigned a=0; a < system->NumSectors; ++a)
    {
        t_sector* const sect = &sectors[a];
        const t_xy* const vert = sect->vertex;

        if(vert[0].x != vert[ sect->npoints ].x
           || vert[0].y != vert[ sect->npoints ].y)
        {
            fprintf(stderr, "Internal error: Sector %u: Vertexes don't form a loop!\n", a);
        }
    }
    // Verify that for each edge that has a neighbour, the neighbour
    // has this same neighbour.
    for(unsigned a=0; a<system->NumSectors; ++a)
    {
        t_sector* sect = &sectors[a];
        const t_xy* const vert = sect->vertex;
        for(unsigned b = 0; b < sect->npoints; ++b)
        {
            if(sect->neighbours[b] >= (int)system->NumSectors)
            {
                fprintf(stderr, "Sector %u: Contains neighbour %d (too large, number of sectors is %u)\n",
                        a, sect->neighbours[b], system->NumSectors);
            }
            t_xy point1 = vert[b], point2 = vert[b+1];

            int found = 0;
            for(unsigned d = 0; d < system->NumSectors; ++d)
            {
                t_sector* const neigh = &sectors[d];
                for(unsigned c = 0; c < neigh->npoints; ++c)
                {
                    if(neigh->vertex[c+1].x == point1.x
                       && neigh->vertex[c+1].y == point1.y
                       && neigh->vertex[c+0].x == point2.x
                       && neigh->vertex[c+0].y == point2.y)
                    {
                        if(neigh->neighbours[c] != (int)a)
                        {
                            fprintf(stderr, "Sector %d: neighbour behind line (%g,%g)-(%g,%g) should be %u, %d found instead. Fixing.\n",
                                    d, point2.x,point2.y, point1.x,point1.y, a, neigh->neighbours[c]);
                            neigh->neighbours[c] = a;
                            goto Rescan;
                        }
                        if(sect->neighbours[b] != (int)d)
                        {
                            fprintf(stderr, "Sector %u: neighbour behind line (%g,%g)-(%g,%g) should be %u, %d found instead. Fixing.\n",
                                    a, point1.x,point1.y, point2.x,point2.y, d, sect->neighbours[b]);
                            sect->neighbours[b] = d;
                            goto Rescan;
                        }
                        else
                            ++found;
                    }
                }
            }
            if(sect->neighbours[b] >= 0 && sect->neighbours[b] < (int)system->NumSectors && found != 1)
                fprintf(stderr, "Sectors %u and its neighbour %d don't share line (%g,%g)-(%g,%g)\n",
                        a, sect->neighbours[b],
                        point1.x,point1.y, point2.x,point2.y);
        }
    }
    // Verify that the vertexes form a convex hull.
    for(unsigned a=0; a< system->NumSectors; ++a)
    {
        t_sector* sect = &sectors[a];
        const t_xy* const vert = sect->vertex;
        for(unsigned b = 0; b < sect->npoints; ++b)
        {
            unsigned c = (b+1) % sect->npoints, d = (b+2) % sect->npoints;
            float x0 = vert[b].x, y0 = vert[b].y;
            float x1 = vert[c].x, y1 = vert[c].y;
            switch(PointSide(vert[d].x,vert[d].y, x0,y0, x1,y1))
            {
                case 0:
                    continue;
                    // Note: This used to be a problem for my engine, but is not anymore, so it is disabled.
                    //       If you enable this change, you will not need the IntersectBox calls in some locations anymore.
                    if(sect->neighbours[b] == sect->neighbours[c]) continue;
                    fprintf(stderr, "Sector %u: Edges %u-%u and %u-%u are parallel, but have different neighbours. This would pose problems for collision detection.\n",
                            a,  b,c, c,d);
                    break;
                case -1:
                    fprintf(stderr, "Sector %u: Edges %u-%u and %u-%u create a concave turn. This would be rendered wrong.\n",
                            a,  b,c, c,d);
                    break;
                default:
                    // This edge is fine.
                    continue;
            }
            fprintf(stderr, "- Splitting sector, using (%g,%g) as anchor", vert[c].x,vert[c].y);

            // Insert an edge between (c) and (e),
            // where e is the nearest point to (c), under the following rules:
            // e cannot be c, c-1 or c+1
            // line (c)-(e) cannot intersect with any edge in this sector
            float nearest_dist     = 1e29f;
            unsigned nearest_point = ~0u;
            for(unsigned n = (d+1) % sect->npoints; n != b; n = (n+1)% sect->npoints) // Don't go through b,c,d
            {
                float x2 = vert[n].x, y2 = vert[n].y;
                float distx = x2-x1, disty = y2-y1;
                float dist = distx*distx + disty*disty;
                if(dist >= nearest_dist) continue;

                if(PointSide(x2,y2, x0,y0, x1,y1) != 1) continue;

                int ok = 1;

                x1 += distx*1e-4f; x2 -= distx*1e-4f;
                y1 += disty*1e-4f; y2 -= disty*1e-4f;
                for(unsigned f = 0; f < sect->npoints; ++f)
                    if(IntersectLineSegments(x1,y1, x2,y2,
                                             vert[f].x,vert[f].y, vert[f+1].x, vert[f+1].y))
                    { ok = 0; break; }
                if(!ok) continue;

                // Check whether this split would resolve the original problem
                if(PointSide(x2,y2, vert[d].x,vert[d].y, x1,y1) == 1) dist += 1e6f;
                if(dist >= nearest_dist) continue;

                nearest_dist   = dist;
                nearest_point = n;
            }
            if(nearest_point == ~0u)
            {
                fprintf(stderr, " - ERROR: Could not find a vertex to pair with!\n");
                SDL_Delay(200);
                continue;
            }
            unsigned e = nearest_point;
            fprintf(stderr, " and point %u - (%g,%g) as the far point.\n", e, vert[e].x,vert[e].y);
            // Now that we have a chain: a b c d e f g h
            // And we're supposed to split it at "c" and "e", the outcome should be two chains:
            // c d e         (c)
            // e f g h a b c (e)

            t_xy* vert1   = malloc(sect->npoints * sizeof(*vert1));
            t_xy* vert2   = malloc(sect->npoints * sizeof(*vert2));
            signed char* neigh1 = malloc(sect->npoints * sizeof(*neigh1));
            signed char* neigh2 = malloc(sect->npoints * sizeof(*neigh2));

            // Create chain 1: from c to e.
            unsigned chain1_length = 0;
            for(unsigned n = 0; n < sect->npoints; ++n)
            {
                unsigned m = (c + n) % sect->npoints;
                neigh1[chain1_length]  = sect->neighbours[m];
                vert1[chain1_length++] = sect->vertex[m];
                if(m == e) { vert1[chain1_length] = vert1[0]; break; }
            }
            neigh1[chain1_length-1] = system->NumSectors;
            // Create chain 2: from e to c.
            unsigned chain2_length = 0;
            for(unsigned n = 0; n < sect->npoints; ++n)
            {
                unsigned m = (e + n) % sect->npoints;
                neigh2[chain2_length]  = sect->neighbours[m];
                vert2[chain2_length++] = sect->vertex[m];
                if(m == c) { vert2[chain2_length] = vert2[0]; break; }
            }
            neigh2[chain2_length-1] = a;
            // Change sect into using chain1.
            free(sect->vertex);    sect->vertex    = vert1;
            free(sect->neighbours); sect->neighbours = neigh1;
            sect->npoints = chain1_length;
            // Create another sector that uses chain2.
            sectors = realloc(sectors, ++system->NumSectors * sizeof(*sectors));
            sect = &sectors[a];
            sectors[system->NumSectors-1] = (t_sector) { sect->floor, sect->ceil, vert2, chain2_length, neigh2 };
            // The other sector may now have neighbours that think
            // their neighbour is still the old sector. Rescan to fix it.
            goto Rescan;
        }
    }

    //printf("PHASE 2\n"); SDL_Delay(500);
    //if(phase == 0) { phase = 1; goto Rescan; }
    printf("%d sectors.\n", system->NumSectors);

#if 0
    /* This code creates the simplified map file for the program featured in the YouTube video. */
    FILE *fp = fopen("map-clear.txt", "wt");
    unsigned NumVertexes = 0;
    struct xy vert[256];
    int       done[256];
    for(unsigned n=0; n<system->NumSectors; ++n)
        for(unsigned s=0; s<sectors[n].npoints; ++s)
        {
            struct xy point = sectors[n].vertex[(s+1)%sectors[n].npoints];
            unsigned v=0;
            for(; v<NumVertexes; ++v)
                if(point.x == vert[v].x && point.y == vert[v].y)
                    break;
            if(v == NumVertexes)
                { done[NumVertexes] = -1; vert[NumVertexes++] = point; }
        }

    // Sort the vertexes by Y coordinate, X coordinate
    qsort(vert, NumVertexes, sizeof(*vert), vert_compare);

    for(unsigned m=0,v=0; v<NumVertexes; ++v)
    {
        if(done[v] >= 0) continue;
        fprintf(fp, "vertex\t%g\t%g", vert[v].y, vert[v].x); done[v] = m++;
        for(unsigned v2=v+1; v2<NumVertexes; ++v2)
            if(done[v2] < 0 && vert[v2].y == vert[v].y)
                { fprintf(fp, " %g", vert[v2].x); done[v2] = m++; }
        fprintf(fp, "\n");
    }
    fprintf(fp, "\n");

    for(unsigned n=0; n<system->NumSectors; ++n)
    {
        fprintf(fp, "sector\t%g %g\t", sectors[n].floor, sectors[n].ceil);
        int wid = 0;
        for(unsigned s=0; s<sectors[n].npoints; ++s)
        {
            struct xy point = sectors[n].vertex[(s+1)%sectors[n].npoints];
            unsigned v=0;
            for(; v<NumVertexes; ++v)
                if(point.x == vert[v].x && point.y == vert[v].y)
                    break;
            wid += fprintf(fp, " %u", done[v]);
        }
        fprintf(fp, "%*s", 24-wid, "");
        for(unsigned s=0; s<sectors[n].npoints; ++s)
            fprintf(fp, "%d ", sectors[n].neighbours[s]);
        fprintf(fp, "\n");
    }

    fprintf(fp, "\nplayer\t%g %g\t%g\t%d\n", player->where.x, player->where.z, player->angle, player->num_sect);

    fclose(fp);
#endif
}