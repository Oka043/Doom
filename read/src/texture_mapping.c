#ifdef TextureMapping
typedef int Texture[1024][1024];
struct TextureSet { Texture texture, normalmap, lightmap, lightmap_diffuseonly; };
#endif

/* Sectors: Floor and ceiling height; list of wall vertexes and neighbors */

#ifdef VisibilityTracking
#define MaxVisibleSectors 256
 struct xy VisibleFloorBegins[MaxVisibleSectors][W], VisibleFloorEnds[MaxVisibleSectors][W];
 char VisibleFloors[MaxVisibleSectors][W];
 struct xy VisibleCeilBegins[MaxVisibleSectors][W], VisibleCeilEnds[MaxVisibleSectors][W];
 char VisibleCeils[MaxVisibleSectors][W];
 unsigned NumVisibleSectors=0;
#endif



#ifdef LightMapping
static struct light
{
    struct xyz where, light;
    unsigned char sector;
}* lights = NULL;
static unsigned NumLights = 0;
#endif


#ifdef TextureMapping
# include <sys/mman.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <errno.h>

static int LoadTexture(void)
{
    int initialized = 0;
    int fd = open("portrend_textures.bin", O_RDWR | O_CREAT, 0644);
    if(lseek(fd, 0, SEEK_END) == 0)
    {
InitializeTextures:;
        // Initialize by loading textures
        #define LoadTexture(filename, name) \
            Texture* name = NULL; do { \
                FILE* fp = fopen(filename, "rb"); \
                if(!fp) perror(filename); else { \
                    name = malloc(sizeof(*name)); \
                    fseek(fp, 0x11, SEEK_SET); \
                    for(unsigned y=0; y<1024; ++y) \
                        for(unsigned x=0; x<1024; ++x) \
                        { \
                            int r = fgetc(fp), g = fgetc(fp), b = fgetc(fp); \
                            (*name)[x][y] = r*65536 + g*256 + b; \
                        } \
                    fclose(fp); } \
            } while(0)

        #define UnloadTexture(name) free(name)

        Texture dummylightmap;
        memset(&dummylightmap, 0, sizeof(dummylightmap));

        LoadTexture("wall2.ppm", WallTexture);   LoadTexture("wall2_norm.ppm", WallNormal);
        LoadTexture("wall3.ppm", WallTexture2);  LoadTexture("wall3_norm.ppm", WallNormal2);
        LoadTexture("floor2.ppm", FloorTexture); LoadTexture("floor2_norm.ppm", FloorNormal);
        LoadTexture("ceil2.ppm", CeilTexture);   LoadTexture("ceil2_norm.ppm", CeilNormal);

        #define SafeWrite(fd, buf, amount) do { \
                const char* source = (const char*)(buf); \
                long remain = (amount); \
                while(remain > 0) { \
                    long result = write(fd, source, remain); \
                    if(result >= 0) { remain -= result; source += result; } \
                    else if(errno == EAGAIN || errno == EINTR) continue; \
                    else break; \
                } \
                if(remain > 0) perror("write"); \
            } while(0)
        #define PutTextureSet(txtname, normname) do { \
            SafeWrite(fd, txtname,       sizeof(Texture)); \
            SafeWrite(fd, normname,      sizeof(Texture)); \
            SafeWrite(fd, &dummylightmap, sizeof(Texture)); \
            SafeWrite(fd, &dummylightmap, sizeof(Texture)); } while(0)

        printf("Initializing textures... ");
        lseek(fd, 0, SEEK_SET);
        for(unsigned n=0; n<NumSectors; ++n)
        {
            for(int s=printf("%d/%d", n+1,NumSectors); s--; ) putchar('\b');
            fflush(stdout);

            PutTextureSet(FloorTexture, FloorNormal);
            PutTextureSet(CeilTexture,  CeilNormal);
            for(unsigned w=0; w<sectors[n].npoints; ++w) PutTextureSet(WallTexture, WallNormal);
            for(unsigned w=0; w<sectors[n].npoints; ++w) PutTextureSet(WallTexture2, WallNormal2);
        }
        ftruncate(fd, lseek(fd, 0, SEEK_CUR));
        printf("\n"); fflush(stdout);

        UnloadTexture(WallTexture);  UnloadTexture(WallNormal);
        UnloadTexture(WallTexture2); UnloadTexture(WallNormal2);
        UnloadTexture(FloorTexture); UnloadTexture(FloorNormal);
        UnloadTexture(CeilTexture);  UnloadTexture(CeilNormal);

        #undef UnloadTexture
        #undef LoadTexture
        initialized = 1;
    }
    off_t filesize = lseek(fd, 0, SEEK_END);
    char* texturedata = mmap(NULL, filesize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if(!texturedata) perror("mmap");

    printf("Loading textures\n");
    off_t pos = 0;
    for(unsigned n=0; n<NumSectors; ++n)
    {
        sectors[n].floortexture = (void*) (texturedata + pos); pos += sizeof(struct TextureSet);
        sectors[n].ceiltexture  = (void*) (texturedata + pos); pos += sizeof(struct TextureSet);
        unsigned w = sectors[n].npoints;
        sectors[n].uppertextures = (void*) (texturedata + pos); pos += sizeof(struct TextureSet) * w;
        sectors[n].lowertextures = (void*) (texturedata + pos); pos += sizeof(struct TextureSet) * w;
    }
    printf("done, %llu bytes mmapped out of %llu\n", (unsigned long long)pos, (unsigned long long) filesize);
    if(pos != filesize)
    {
        printf(" -- Wrong filesize! Let's try that again.\n");
        munmap(texturedata, filesize);
        goto InitializeTextures;
    }
    return initialized;
}

#ifdef LightMapping
#define vlen(x,y,z) sqrtf((x)*(x) + (y)*(y) + (z)*(z))
#define vlen2(x0,y0,z0, x1,y1,z1) vlen((x1)-(x0), (y1)-(y0), (z1)-(z0))
#define vdot3(x0,y0,z0, x1,y1,z1) ((x0)*(x1) + (y0)*(y1) + (z0)*(z1))
#define vxs3(x0,y0,z0, x1,y1,z1) (struct xyz){ vxs(y0,z0, y1,z1), vxs(z0,x0, z1,x1), vxs(x0,y0, x1,y1) }

struct Intersection
{
    // Map coordinates where the hit happened. x,z = map, y = height
    struct xyz where;
    // Information about the surface that was hit
    struct TextureSet* surface;
    struct xyz         normal;   // Perturbed surface normal
    int                sample;   // RGB sample from surface texture & lightmap
    int                sectorno;
};

static int ClampWithDesaturation(int r,int g,int b)
{
    int luma = r*299 + g*587 + b*114;
    if(luma > 255000)  { r=g=b=255; }
    else if(luma <= 0) { r=g=b=0; }
    else
    {
        double sat = 1000;
        if(r > 255) sat = min(sat, (luma-255e3) / (luma-r)); else if(r < 0) sat = min(sat, luma / (double)(luma-r));
        if(g > 255) sat = min(sat, (luma-255e3) / (luma-g)); else if(g < 0) sat = min(sat, luma / (double)(luma-g));
        if(b > 255) sat = min(sat, (luma-255e3) / (luma-b)); else if(b < 0) sat = min(sat, luma / (double)(luma-b));
        if(sat != 1.)
        {
            r = (r - luma) * sat/1e3 + luma; r = clamp(r,0,255);
            g = (g - luma) * sat/1e3 + luma; g = clamp(g,0,255);
            b = (b - luma) * sat/1e3 + luma; b = clamp(b,0,255);
        }
    }
    return r*65536 + g*256 + b;
}
static int ApplyLight(int texture, int light)
{
    int tr = (texture >>16) & 0xFF;
    int tg = (texture >> 8) & 0xFF;
    int tb = (texture >> 0) & 0xFF;
    int lr = ((light  >>16) & 0xFF);
    int lg = ((light  >> 8) & 0xFF);
    int lb = ((light  >> 0) & 0xFF);
    int r = tr*lr *2 / 255;
    int g = tg*lg *2 / 255;
    int b = tb*lb *2 / 255;
#if 1
    return ClampWithDesaturation(r,g,b);
#else
    return clamp(tr*lr / 255,0,255)*65536
         + clamp(tg*lg / 255,0,255)*256
         + clamp(tb*lb / 255,0,255);
#endif
}
static void PutColor(int* target, struct xyz color)
{
    *target = ClampWithDesaturation(color.x, color.y, color.z);
}
static void AddColor(int* target, struct xyz color)
{
    int r = ((*target >> 16) & 0xFF) + color.x;
    int g = ((*target >>  8) & 0xFF) + color.y;
    int b = ((*target >>  0) & 0xFF) + color.z;
    *target = ClampWithDesaturation(r, g, b);
}

static struct xyz PerturbNormal(struct xyz normal,
                                struct xyz tangent,
                                struct xyz bitangent,
                                int normal_sample)
{
    struct xyz perturb = { ((normal_sample >> 16) & 0xFF) / 127.5f - 1.f,
                           ((normal_sample >>  8) & 0xFF) / 127.5f - 1.f,
                           ((normal_sample >>  0) & 0xFF) / 127.5f - 1.f};
    // TODO: Verify whether this calculation is correct
    return (struct xyz) { normal.x * perturb.z + bitangent.x * perturb.y + tangent.x * perturb.x,
                          normal.y * perturb.z + bitangent.y * perturb.y + tangent.y * perturb.x,
                          normal.z * perturb.z + bitangent.z * perturb.y + tangent.z * perturb.x };
}

static void GetSectorBoundingBox(int sectorno, struct xy* bounding_min, struct xy* bounding_max)
{
    const struct sector* sect = &sectors[sectorno];
    for(int s = 0; s < sect->npoints; ++s)
    {
        bounding_min->x = min(bounding_min->x, sect->vertex[s].x);
        bounding_min->y = min(bounding_min->y, sect->vertex[s].y);
        bounding_max->x = max(bounding_max->x, sect->vertex[s].x);
        bounding_max->y = max(bounding_max->y, sect->vertex[s].y);
    }
}

// Return values:
//    0 = clear path, nothing hit
//    1 = hit, *result indicates where it hit
//    2 = your princess is in another castle (a direct path doesn't lead to this sector)
static int IntersectRay(struct xyz origin, int origin_sectorno,
                        struct xyz target, int target_sectorno,
                        struct Intersection* result)
{
    unsigned n_rescan=0;
    int prev_sectorno=-1;
rescan:;
    ++n_rescan;

    struct sector* sect = &sectors[origin_sectorno];
    /*printf("INTERSECT: Now in sector %d at %.3f %.3f %.3f, going towards sector %d at %.3f %.3f %.3f\n",
        origin_sectorno,origin.x,origin.y,origin.z,
        target_sectorno,target.x,target.y,target.z);*/

    // Check if this beam hits one of the sector's edges.
    unsigned u=0, v=0, lu=0, lv=0;
    struct xyz tangent, bitangent;

    for(int s = 0; s < sect->npoints; ++s)
    {
        float vx1 = sect->vertex[s+0].x, vy1 = sect->vertex[s+0].y;
        float vx2 = sect->vertex[s+1].x, vy2 = sect->vertex[s+1].y;

        if(!IntersectLineSegments(origin.x,origin.z, target.x,target.z, vx1,vy1, vx2,vy2)/*
        || PointSide(target.x,target.z, vx1,vy1, vx2,vy2) >= 0*/)
            continue;

        // Determine the X & Z coordinates of the wall hit.
        struct xy hit = INTERSECT(origin.x,origin.z, target.x,target.z, vx1,vy1, vx2,vy2);
        float x = hit.x;
        float z = hit.y;

        // Also determine the Y coordinate.
        float y = origin.y + ((abs(target.x-origin.x) > abs(target.z-origin.z))
                              ? ((x - origin.x) * (target.y - origin.y) / (target.x - origin.x))
                              : ((z - origin.z) * (target.y - origin.y) / (target.z - origin.z)) );

        /*fprintf(stderr, "(%.2f, %.2f, %.2f) - (%.2f, %.2f, %.2f) versus (%.2f, %.2f) - (%.2f, %.2f) intersected at (%.2f, %.2f, %.2f)\n",
            origin.x,origin.y,origin.z,
            target.x,target.y,target.z,
            vx1,vy1, vx2,vy2,
            x,y,z);*/

        /* Check where the hole is. */
        float hole_low = 9e9, hole_high = -9e9;
        if(sect->neighbors[s] >= 0)
        {
            hole_low  = max( sect->floor, sectors[sect->neighbors[s]].floor );
            hole_high = min( sect->ceil,  sectors[sect->neighbors[s]].ceil  );
        }

        if(y >= hole_low && y <= hole_high)
        {
            // The point fit in between this hole.
            origin_sectorno = sect->neighbors[s];
            origin.x        = x + (target.x - origin.x)*1e-2;
            origin.y        = y + (target.y - origin.y)*1e-2;
            origin.z        = z + (target.z - origin.z)*1e-2;

            float distance = vlen(target.x-origin.x, target.y-origin.y, target.z-origin.z);
            if(origin_sectorno == prev_sectorno)
            {
                // Disregard this boundary.
                continue;
            }
            if(distance < 1e-3f || origin_sectorno == prev_sectorno)
            {
                // Close enough.
                goto close_enough;
            }
            prev_sectorno = origin_sectorno;
            goto rescan;
        }

        // It hit the wall.
        // Did it hit the sector's floor first?
        if(y < sect->floor) goto hit_floor;
        if(y > sect->ceil)  goto hit_ceil;
        // Nope. It hit the wall.
        result->where    = (struct xyz) { x,y,z };
        result->surface  = (y < hole_low) ? &sect->lowertextures[s] : &sect->uppertextures[s];
        result->sectorno = origin_sectorno;
        /*printf("  Thus hit a wall\n");*/

        float nx = vy2-vy1, nz = vx1-vx2, len = sqrtf(nx*nx + nz*nz);
        result->normal = (struct xyz){ nx/len, 0, nz/len };

        nx = vx2-vx1; nz = vy2-vy1; len = sqrtf(nx*nx + nz*nz);
        tangent   = (struct xyz){ nx/len, 0, nz/len };
        bitangent = (struct xyz) { 0,1,0};

        // Calculate the texture coordinates.
        float dx = vx2-vx1;
        float dy = vy2-vy1;
        v = (unsigned)((y - sect->floor) * 1024.f / (sect->ceil - sect->floor)) % 1024u;
        u = (abs(dx) > abs(dy) ? (unsigned)((x - vx1) * 1024 / dx)
                               : (unsigned)((z - vy1) * 1024 / dy)) % 1024u;
        // Lightmap coordinates are the same as texture coordinates.
        lu = u;
        lv = v;
    perturb_normal:;
        int texture_sample = result->surface->texture[v][u];
        int normal_sample  = result->surface->normalmap[v][u];
        int light_sample   = result->surface->lightmap[lv][lu];
        result->sample = ApplyLight(texture_sample, light_sample);
        result->normal = PerturbNormal(result->normal, tangent, bitangent, normal_sample);
        return 1;
    }

    if(target.y > sect->ceil)
    {
    hit_ceil:
        result->where.y = sect->ceil;
        result->surface = sect->ceiltexture;
        result->normal = (struct xyz){0,-1,0};
        tangent        = (struct xyz){1,0,0};
        bitangent      = vxs3(result->normal.x,result->normal.y,result->normal.z, tangent.x,tangent.y,tangent.z);
    hit_ceil_or_floor:
        // Either the floor or ceiling was hit. Determine the X & Z coordinates.
        result->where.x = (result->where.y - origin.y) * (target.x - origin.x) / (target.y - origin.y) + origin.x;
        result->where.z = (result->where.y - origin.y) * (target.z - origin.z) / (target.y - origin.y) + origin.z;
        /*printf("  Hit the ceiling or floor at %.3f, %.3f, %.3f\n", result->where.x, result->where.y, result->where.z);*/
        // Calculate the texture coordinates.
        u = ((unsigned)(result->where.x * 256)) % 1024u;
        v = ((unsigned)(result->where.z * 256)) % 1024u;
        // Calculate the lightmap coordinates.
        struct xy bounding_min = {1e9f, 1e9f}, bounding_max = {-1e9f, -1e9f};
        GetSectorBoundingBox(origin_sectorno, &bounding_min, &bounding_max);
        lu = ((unsigned)((result->where.x - bounding_min.x) * 1024 / (bounding_max.x - bounding_min.x))) % 1024;
        lv = ((unsigned)((result->where.y - bounding_min.y) * 1024 / (bounding_max.y - bounding_min.y))) % 1024;
        goto perturb_normal;
    }
    if(target.y < sect->floor)
    {
    hit_floor:
        result->where.y = sect->floor;
        result->surface = sect->floortexture;
        result->normal = (struct xyz){0, 1,0};
        tangent        = (struct xyz){-1,0,0};
        bitangent      = vxs3(result->normal.x,result->normal.y,result->normal.z, tangent.x,tangent.y,tangent.z);
        goto hit_ceil_or_floor;
    }
close_enough:;

    /*printf("  Hit nothing. Sector %s.\n", origin_sectorno == target_sectorno ? "match" : "doesn't match");*/
    // Is the target in this sector?
    return origin_sectorno == target_sectorno ? 0 : 2;
}

#define narealightcomponents    32 //512;//64
#define area_light_radius       0.4
#define nrandomvectors          128 // 8192
#define firstround              1
#define maxrounds               100
#define fade_distance_diffuse   10.0
#define fade_distance_radiosity 10.0
#define radiomul                1.0
static struct xyz tvec[nrandomvectors];
static struct xyz avec[narealightcomponents];

static void DiffuseLightCalculation(struct xyz normal, struct xyz tangent, struct xyz bitangent,
                                    struct TextureSet* texture,
                                    unsigned tx, unsigned ty,
                                    unsigned lx, unsigned ly,
                                    struct xyz point_in_wall, unsigned sectorno)
{
    struct xyz perturbed_normal = PerturbNormal(normal,tangent,bitangent,
                                                texture->normalmap[tx][ty]);

    // For each lightsource, check if there is an obstacle
    // in between this vertex and the lightsource. Calculate
    // the ambient light levels from the fact.
    // This simulates diffuse light.
    struct xyz color = {0,0,0};
    for(unsigned l=0; l<NumLights; ++l)
    {
        const struct light* light = &lights[l];
        struct xyz source = { point_in_wall.x + normal.x * 1e-5f,
                              point_in_wall.y + normal.y * 1e-5f,
                              point_in_wall.z + normal.z * 1e-5f };
        for(unsigned qa=0; qa<narealightcomponents; ++qa)
        {
            struct xyz target = { light->where.x + avec[qa].x, light->where.y + avec[qa].y, light->where.z + avec[qa].z };
            struct xyz towards = { target.x-source.x, target.y-source.y, target.z-source.z };
            float len = vlen(towards.x, towards.y, towards.z), invlen = 1.0f / len;
            towards.x *= invlen;
            towards.y *= invlen;
            towards.z *= invlen;

            float cosine = vdot3(perturbed_normal.x,perturbed_normal.y,perturbed_normal.z, towards.x,towards.y,towards.z);
            //if(cosine > 1) fprintf(stderr, "cosine = %.3f\n", cosine);
            float power = cosine / (1.f + powf(len / fade_distance_diffuse, 2.0f));
            power /= (float) narealightcomponents;
            if(power > 1e-7f)
            {
                struct Intersection i;
                if(IntersectRay(source, sectorno, target, light->sector, &i) == 0)
                {
                    color.x += light->light.x * power;
                    color.y += light->light.y * power;
                    color.z += light->light.z * power;
    }   }   }   }
    PutColor(&texture->lightmap[lx][ly], color);
}

static void RadiosityCalculation(struct xyz normal, struct xyz tangent, struct xyz bitangent,
                                 struct TextureSet* texture,
                                 unsigned tx, unsigned ty,
                                 unsigned lx, unsigned ly,
                                 struct xyz point_in_wall, unsigned sectorno)
{
    struct xyz perturbed_normal = PerturbNormal(normal,tangent,bitangent,
                                                texture->normalmap[tx][ty]);

    // Shoot rays to each random direction and see what it hits.
    // Take the last round's light value from that location.
    struct xyz source = { point_in_wall.x + normal.x * 1e-3f,
                          point_in_wall.y + normal.y * 1e-3f,
                          point_in_wall.z + normal.z * 1e-3f };
    float basepower = radiomul / nrandomvectors;

    // Apply the set of random vectors to this surface.
    // This produces a set of vectors all pointing away
    // from the wall to random directions.
    struct xyz color = {0,0,0};
    for(unsigned qq=0; qq<nrandomvectors; ++qq)
    {
        struct xyz rvec = tvec[qq];
        // If the random vector points to the wrong side from the wall, flip it
        if(vdot3(rvec.x, rvec.y, rvec.z, normal.x, normal.y, normal.z) < 0)
        {
            rvec.x = -rvec.x;
            rvec.y = -rvec.y;
            rvec.z = -rvec.z;
        }

        struct xyz target = { source.x + rvec.x * 512.f,
                              source.y + rvec.y * 512.f,
                              source.z + rvec.z * 512.f };

        struct Intersection i;
        if(IntersectRay(source, sectorno, target, -1 /* no particular sector */, &i) == 1) // hit something
        {
            float cosine = vdot3(perturbed_normal.x, i.normal.x,
                                 perturbed_normal.y, i.normal.y,
                                 perturbed_normal.z, i.normal.z) * basepower;
            float len = vlen(i.where.x-source.x, i.where.y-source.y, i.where.z-source.z);
            float power = abs(cosine) / (1.f + powf(len / fade_distance_radiosity, 2.0f));
            color.x += ((i.sample >> 16) & 0xFF) * power;
            color.y += ((i.sample >>  8) & 0xFF) * power;
            color.z += ((i.sample >>  0) & 0xFF) * power;
    }   }
    AddColor(&texture->lightmap[lx][ly], color);
}

static void Begin_Radiosity(struct TextureSet* set)
{
    memcpy(&set->lightmap, &set->lightmap_diffuseonly, sizeof(Texture));
}
static double End_Radiosity(struct TextureSet* set, const char* label)
{
    long differences = 0;
    for(unsigned x=0; x<1024; ++x)
    for(unsigned y=0; y<1024; ++y)
    {
        int old = set->lightmap_diffuseonly[x][y];
        int r = (old >> 16) & 0xFF, g = (old >> 8) & 0xFF, b = (old) & 0xFF;
        int new = set->lightmap[x][y];
        r -= (new >> 16) & 0xFF; g -= (new >> 8) & 0xFF; b -= (new) & 0xFF;
        differences += abs(r) + abs(g) + abs(b);
    }
    double result = differences / (double)(1024*1024);
    fprintf(stderr, "Differences in %s: %g\33[K\n", label, result);
    return result;
}
static void End_Diffuse(struct TextureSet* set)
{
    memcpy(&set->lightmap_diffuseonly, &set->lightmap, sizeof(Texture));
}

#ifdef _OPENMP
# include <omp.h>

#define OMP_SCALER_LOOP_BEGIN(a,b,c,d,e,f) do { \
    int this_thread = omp_get_thread_num(), num_threads = omp_get_num_threads(); \
    int my_start = (this_thread  ) * ((c)-(a)) / num_threads + (a); \
    int my_end   = (this_thread+1) * ((c)-(a)) / num_threads + (a); \
    struct Scaler e##int = Scaler_Init(a, my_start, (c)-1, (d) * 32768, (f) * 32768); \
    for(int b = my_start; b < my_end; ++b) \
    { \
        float e = Scaler_Next(&e##int) / 32768.f;

#else

#define OMP_SCALER_LOOP_BEGIN(a,b,c,d,e,f) do { \
    struct Scaler e##int = Scaler_Init(a, a, (c)-1, (d) * 32768, (f) * 32768); \
    for(int b = (a); b < (c); ++b) \
    { \
        float e = Scaler_Next(&e##int) / 32768.f;

#endif

#define OMP_SCALER_LOOP_END() \
    } }  while(0)

/* My lightmap calculation involves some raytracing.
 * There are faster ways to do it, but this is the only way I know how to do it in software.
 */
static void BuildLightmaps(void)
{
    for(unsigned round=firstround; round<=maxrounds; ++round)
    {
        fprintf(stderr, "Lighting calculation, round %u...\n", round);
#ifndef _OPENMP
        fprintf(stderr, "Note: This would probably go faster if you enabled OpenMP in your compiler options. It's -fopenmp in GCC and Clang.\n");
#endif

        // Create uniformly distributed random unit vectors
        for(unsigned n=0; n<nrandomvectors; ++n)
        {
            double u = (rand() % 1000000) / 1e6; // 0..1
            double v = (rand() % 1000000) / 1e6; // 0..1
            double theta = 2*3.141592653*u;
            double phi   = acos(2*v-1);
            tvec[n].x = cos(theta) * sin(phi);
            tvec[n].y = sin(theta) * sin(phi);
            tvec[n].z = cos(phi);
        }

        // A lightsource is represented by a spherical cloud
        // of smaller lightsources around the actual lightsource.
        // This achieves smooth edges for the shadows.
        #define drand(m) ((rand()%1000-500)*5e-2*m)
        for(unsigned qa=0; qa<narealightcomponents; ++qa)
        {
            double len;
            do {
              avec[qa] = (struct xyz){ drand(100.0), drand(100.0), drand(100.0) };
              len = sqrt(avec[qa].x*avec[qa].x + avec[qa].y*avec[qa].y + avec[qa].z*avec[qa].z);
            } while(len < 1e-3);
            avec[qa].x *= area_light_radius/len;
            avec[qa].y *= area_light_radius/len;
            avec[qa].z *= area_light_radius/len;
        }
        #undef drand


        fprintf(stderr, "Note: You can interrupt this program at any time you want. If you wish to resume\n"
                        "      the lightmap calculation at a later date, use the --rebuild commandline option.\n"
                        "      If you have already finished round 1 (diffuse light), and don't wish to do that\n"
                        "      again, change the '#define firstround' value to your liking. Value 1 means\n"
                        "      it starts from beginning, and any value from 2-100 (actual value is not important)\n"
                        "      means to progressively improve the radiosity (cumulative). The current value is %d.\n",
            firstround);

        double total_differences = 0;
        for(unsigned sectorno=0; sectorno<NumSectors; ++sectorno)
        {
            struct sector* const sect = &sectors[sectorno];
            const struct xy* const vert = sect->vertex;

            double sector_differences = 0;

            if(1) // Do ceiling and floor
            {
                struct xy bounding_min = {1e9f, 1e9f}, bounding_max = {-1e9f, -1e9f};
                GetSectorBoundingBox(sectorno, &bounding_min, &bounding_max);
                struct xyz floornormal    = (struct xyz){0, 1, 0}; // floor
                struct xyz floortangent   = (struct xyz){1, 0, 0};
                struct xyz floorbitangent = vxs3(floornormal.x,floornormal.y,floornormal.z, floortangent.x,floortangent.y,floortangent.z);
                struct xyz ceilnormal     = (struct xyz){0,-1, 0}; // ceiling
                struct xyz ceiltangent    = (struct xyz){1, 0, 0};
                struct xyz ceilbitangent  = vxs3(ceilnormal.x,ceilnormal.y,ceilnormal.z, ceiltangent.x,ceiltangent.y,ceiltangent.z);

                fprintf(stderr, "Bounding box for sector %d/%d: %g,%g - %g,%g\n",
                    sectorno+1,NumSectors, bounding_min.x,bounding_min.y, bounding_max.x,bounding_max.y);
                // Round 1: Check lightsources
                if(round == 1)
                {
                    struct Scaler txtx_int = Scaler_Init(0,0,1023, bounding_min.x*32768, bounding_max.x*32768);
                    for(unsigned x=0; x<1024; ++x)
                    {
                        fprintf(stderr, "- Sector %d ceils&floors, %u/%u diffuse light...\r", sectorno+1, x,1024);
                        float txtx = Scaler_Next(&txtx_int)/32768.f;

                        // For better cache locality, first do floors and then ceils
                        #pragma omp parallel
                        OMP_SCALER_LOOP_BEGIN(0,y,1024, bounding_min.y, txty, bounding_max.y);
                            DiffuseLightCalculation(floornormal, floortangent, floorbitangent, sect->floortexture,
                                                    ((unsigned)(txtx*256)) % 1024, ((unsigned)(txty*256)) % 1024,
                                                    x,y,
                                                    (struct xyz){txtx, sect->floor, txty}, sectorno);
                        OMP_SCALER_LOOP_END();
                        #pragma omp parallel
                        OMP_SCALER_LOOP_BEGIN(0,y,1024, bounding_min.y, txty, bounding_max.y);
                            DiffuseLightCalculation(ceilnormal, ceiltangent, ceilbitangent, sect->ceiltexture,
                                                    ((unsigned)(txtx*256)) % 1024, ((unsigned)(txty*256)) % 1024,
                                                    x,y,
                                                    (struct xyz){txtx, sect->ceil, txty}, sectorno);
                        OMP_SCALER_LOOP_END();
                    }
                    fprintf(stderr, "\n");
                    End_Diffuse(sect->floortexture);
                    End_Diffuse(sect->ceiltexture);
                }
                else
                {
                    // Round 2+: Radiosity
                    Begin_Radiosity(sect->floortexture);
                    Begin_Radiosity(sect->ceiltexture);

                    // Calculate radiosity in decreased resolution
                    struct Scaler txtx_int = Scaler_Init(0,0,1023, bounding_min.x*32768, bounding_max.x*32768);
                    for(unsigned x=0; x<1024; ++x)
                    {
                        float txtx = Scaler_Next(&txtx_int)/32768.f;

                        fprintf(stderr, "- Sector %u ceils&floors, %u/%u radiosity...\r", sectorno+1, x,1024);
                        #pragma omp parallel
                        OMP_SCALER_LOOP_BEGIN(0,y,1024, bounding_min.y, txty, bounding_max.y);
                            RadiosityCalculation(floornormal, floortangent, floorbitangent, sect->ceiltexture,
                                                 ((unsigned)(txtx*256)) % 1024, ((unsigned)(txty*256)) % 1024, x,y,
                                                 (struct xyz){txtx, sect->floor, txty}, sectorno);
                        OMP_SCALER_LOOP_END();

                        #pragma omp parallel
                        OMP_SCALER_LOOP_BEGIN(0,y,1024, bounding_min.y, txty, bounding_max.y);
                            RadiosityCalculation(ceilnormal, ceiltangent, ceilbitangent, sect->ceiltexture,
                                                 ((unsigned)(txtx*256)) % 1024, ((unsigned)(txty*256)) % 1024, x,y,
                                                 (struct xyz){txtx, sect->ceil, txty}, sectorno);
                        OMP_SCALER_LOOP_END();
                    }

                    char Buf[128];
                    sprintf(Buf, "Sector %u floors", sectorno+1); sector_differences += End_Radiosity(sect->floortexture, Buf);
                    sprintf(Buf, "Sector %u ceils", sectorno+1); sector_differences += End_Radiosity(sect->ceiltexture, Buf);
                }
            }

            if(1)for(unsigned s=0; s<sect->npoints; ++s)
            {
                float xd = vert[s+1].x - vert[s].x;
                float zd = vert[s+1].y - vert[s].y;
                float len = vlen(xd,zd,0);

                struct xyz normal    = {-zd/len, 0, xd/len};
                struct xyz tangent   = {xd/len, 0, zd/len};
                struct xyz bitangent = {0,1,0};

                float hole_low = 9e9, hole_high = -9e9;
                if(sect->neighbors[s] >= 0)
                {
                    hole_low  = max( sect->floor, sectors[sect->neighbors[s]].floor );
                    hole_high = min( sect->ceil,  sectors[sect->neighbors[s]].ceil  );
                }

                if(round == 1)
                {
                    // Round 1: Check lightsources
                    struct Scaler txtx_int = Scaler_Init(0,0,1023, vert[s].x*32768,vert[s+1].x*32768);
                    struct Scaler txtz_int = Scaler_Init(0,0,1023, vert[s].y*32768,vert[s+1].y*32768);
                    for(unsigned x=0; x<1024; ++x)
                    {
                        float txtx = Scaler_Next(&txtx_int)/32768.f;
                        float txtz = Scaler_Next(&txtz_int)/32768.f;

                        fprintf(stderr, "- Sector %u Wall %u/%u %u/%u diffuse light...\r", sectorno+1, s+1, sect->npoints, x,1024);
                        #pragma omp parallel
                        OMP_SCALER_LOOP_BEGIN(0,y,1024, sect->ceil, txty, sect->floor);
                            struct TextureSet* texture = &sect->uppertextures[s];

                            if(sect->neighbors[s] >= 0 && txty < hole_high)
                            {
                                if(txty > hole_low) continue;
                                texture = &sect->lowertextures[s];
                            }

                            struct xyz point_in_wall = { txtx, txty, txtz };
                            DiffuseLightCalculation(normal, tangent, bitangent, texture, x,y, x,y,
                                                    point_in_wall, sectorno);
                        OMP_SCALER_LOOP_END();
                    }
                    End_Diffuse(&sect->uppertextures[s]);
                    End_Diffuse(&sect->lowertextures[s]);
                }
                else
                {
                    Begin_Radiosity(&sect->uppertextures[s]);
                    Begin_Radiosity(&sect->lowertextures[s]);

                    // Round 2+: Radiosity
                    struct Scaler txtx_int = Scaler_Init(0,0,1023, vert[s].x*32768,vert[s+1].x*32768);
                    struct Scaler txtz_int = Scaler_Init(0,0,1023, vert[s].y*32768,vert[s+1].y*32768);
                    for(unsigned x=0; x<1024; ++x)
                    {
                        float txtx = Scaler_Next(&txtx_int)/32768.f;
                        float txtz = Scaler_Next(&txtz_int)/32768.f;

                        fprintf(stderr, "- Sector %u Wall %u/%u %u/%u radiosity...\r", sectorno+1, s+1, sect->npoints, x,1024);
                        #pragma omp parallel
                        OMP_SCALER_LOOP_BEGIN(0,y,1024, sect->ceil, txty, sect->floor);
                            struct TextureSet* texture = &sect->uppertextures[s];

                            if(sect->neighbors[s] >= 0 && txty < hole_high)
                            {
                                if(txty > hole_low) continue;
                                texture = &sect->lowertextures[s];
                            }
                            struct xyz point_in_wall = { txtx, txty, txtz };
                            RadiosityCalculation(normal, tangent, bitangent, texture, x,y, x,y, point_in_wall, sectorno);
                        OMP_SCALER_LOOP_END();
                    }

                    char Buf[128];
                    sprintf(Buf, "Sector %u wall %u lower texture", sectorno+1, s+1); sector_differences += End_Radiosity(&sect->uppertextures[s], Buf);
                    sprintf(Buf, "Sector %u wall %u upper texture", sectorno+1, s+1); sector_differences += End_Radiosity(&sect->lowertextures[s], Buf);
                }
                fprintf(stderr, "\n");
            }

            fprintf(stderr, "Round %u differences in sector %u: %g\n", round, sectorno+1, sector_differences);
            total_differences += sector_differences;
        }

        fprintf(stderr, "Round %u differences total: %g.\n", round, total_differences);
        if(total_differences < 1e-6)
        {
            break;
        }
    }
}
#endif
#endif

/* PLOT */

#ifdef TextureMapping
static void vline2(int x, int y1,int y2, struct Scaler ty,unsigned txtx, const struct TextureSet* t)
{
    int *pix = (int*) surface->pixels;
    y1 = clamp(y1, 0, H-1);
    y2 = clamp(y2, 0, H-1);
    pix += y1 * W2 + x;

    for(int y = y1; y <= y2; ++y)
    {
        unsigned txty = Scaler_Next(&ty);
  #ifdef LightMapping
        *pix = ApplyLight( t->texture[txtx % 1024][txty % 1024], t->lightmap[txtx % 1024][txty % 1024] );
  #else
        *pix = t->texture[txtx % 1024][txty % 1024];
  #endif
        pix += W2;
    }
}
#endif

/*main*/