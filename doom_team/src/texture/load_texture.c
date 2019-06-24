#include "player_stable.h"

# include <sys/mman.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <errno.h>

//void 	loadTexture(char *filename, Texture *s) {
//// Initialize by loading textures
//
//	Texture *name = NULL;
//	do {
//                FILE *fp = fopen(filename, "rb");
//                if (!fp) perror(filename);
//		else {
//                    name = malloc(sizeof(*name));
//                    fseek(fp, 0x11, SEEK_SET);
//                    for (unsigned y = 0; y < TEXTURE_SIZE_Y; ++y)
//
//					for (unsigned x = 0; x < TEXTURE_SIZE_X; ++x)
// {
//                            int r = fgetc(fp), g = fgetc(fp), b = fgetc(fp);
//                            (*name)[x][y] = r * 65536 + g * 256 + b;
//
//					}
//                    fclose(fp);
//		}
//
//	} while (0);
//}

int LoadTexture(t_system *system)
{
	int initialized;
	int fd;

	fd = open ("textures.bin", O_RDWR | O_CREAT, 0644);
	initialized = 0;
	if(lseek(fd, 0, SEEK_END) == 0)
	{
		// Initialize by loading textures
#define LoadTexture(filename, name) \
            Texture* name = NULL; do { \
                FILE* fp = fopen(filename, "rb"); \
                if(!fp) perror(filename); else { \
                    name = malloc(sizeof(*name)); \
                    fseek(fp, 0x11, SEEK_SET); \
                    for(unsigned y = 0; y < TEXTURE_SIZE_Y; ++y) \
                        for(unsigned x = 0; x < TEXTURE_SIZE_X; ++x) \
                        { \
                            int r = fgetc(fp), g = fgetc(fp), b = fgetc(fp); \
                            (*name)[x][y] = r*65536 + g*256 + b; \
                        } \
                    fclose(fp); } \
            } while(0)

#define UnloadTexture(name) free(name)

		Texture dummylightmap;
		memset(&dummylightmap, 0, sizeof(dummylightmap));


//		t_texture *main_tex;
//		char *str;
//
//		main_tex = (t_texture*)malloc(sizeof(main_tex));
//		main_tex->name = (main_tex->name**)malloc(sizeof(main_tex->name*)*8);
//
//		main_tex->name[0] = "WallTexture";


//		Texture *WallTexture;
//		Texture *WallNormal;
//		Texture *WallTexture2;
//		Texture *WallNormal2;
//		Texture *FloorTexture;
//		Texture *FloorNormal;
//		Texture *CeilTexture;
//		Texture *CeilNormal;

//		WallTexture = 0;
//		WallNormal = 0;
//		WallTexture2 = 0;
//		WallNormal2 = 0;
//		FloorTexture = 0;
//		FloorNormal = 0;
//		CeilTexture = 0;
//		CeilNormal = 0;
//		load_textures(system, "assets/bricksx64.png", WallTexture);   LoadTexture("textures/wall.ppm", WallNormal);//load_textures(system, "assets/bricksx64.png", WallNormal);
//		load_textures(system, "assets/bedrock.png", WallTexture2);  load_textures(system, "assets/bedrock.png", WallNormal2);
//		load_textures(system, "assets/bricksx64.png", FloorTexture); load_textures(system, "assets/bricksx64.png", FloorNormal);
//		load_textures(system, "assets/planks_jungle.png", CeilTexture);  load_textures(system, "assets/planks_jungle.png", CeilNormal);

		LoadTexture("textures/wall.ppm", WallTexture);   LoadTexture("textures/wall.ppm", WallNormal);
		LoadTexture("textures/wall3.ppm", WallTexture2);  LoadTexture("textures/wall3.ppm", WallNormal2);
		LoadTexture("textures/floor.ppm", FloorTexture); LoadTexture("textures/floor.ppm", FloorNormal);
		LoadTexture("textures/ceil2.ppm", CeilTexture);   LoadTexture("textures/ceil2_norm.ppm", CeilNormal);

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
		int n;

		n = 0;
		while (n<system->NumSectors)
		{
			for(int s=printf("%d/%d", n+1,system->NumSectors); s--; ) putchar('\b');
			fflush(stdout);

			PutTextureSet(FloorTexture, FloorNormal);
			PutTextureSet(CeilTexture,  CeilNormal);
			for(unsigned w=0; w<system->sectors[n].npoints; ++w)
				PutTextureSet(WallTexture, WallNormal);
			for(unsigned w=0; w<system->sectors[n].npoints; ++w)
				PutTextureSet(WallTexture2, WallNormal2);
			n++;
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
	off_t filesize;

	filesize = lseek (fd, 0, SEEK_END);
	char* texturedata;

	texturedata = mmap (NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(!texturedata)
		perror("mmap");
	printf("Loading textures\n");
	off_t pos;
	pos = 0;
	int k;

	k = 0;
	while (k<system->NumSectors)
	{
		system->sectors[k].floortexture = (void*) (texturedata + pos);
		pos += sizeof(struct TextureSet);
		system->sectors[k].ceiltexture  = (void*) (texturedata + pos);
		pos += sizeof(struct TextureSet);
		unsigned w;

		w = system->sectors[k].npoints;
		system->sectors[k].uppertextures = (void*) (texturedata + pos);
		pos += sizeof(struct TextureSet) * w;
		system->sectors[k].lowertextures = (void*) (texturedata + pos);
		pos += sizeof(struct TextureSet) * w;
		k++;
	}
	printf("done, %llu bytes mmapped out of %llu\n", (unsigned long long)pos, (unsigned long long) filesize);
	if(pos != filesize)
	{
		printf(" -- Wrong filesize! Let's try that again.\n");
		munmap(texturedata, filesize);
	}
	return initialized;
}
