#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#define pie 3.14
#define ray_color 0xffffffff


struct vec2{
	double x ;
	double y ;
};

struct ray_info{
	struct vec2 origin ;
	struct vec2 direction ;
	int length ;
};

struct vec2 vec_add(struct ray_info rays , struct vec2 ray_normalized){
	struct vec2 result;
	result.x = rays.origin.x + ray_normalized.x ;
	result.y = rays.origin.y + ray_normalized.y ;

	return result;
};

int texture_implementation(SDL_Texture **texture , uint32_t *framebuffer , SDL_Renderer *renderer , int w , int h){
	*texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,w,h);
	if(!*texture){
		perror("TEXTURE CREATION \n");
		return -1;
	};

	SDL_UpdateTexture(*texture,NULL,framebuffer,w*sizeof(uint32_t));
	SDL_RenderCopy(renderer,*texture,NULL,NULL);
	SDL_RenderPresent(renderer);

	return 1;
};

int ray_implement(struct ray_info *rays , int number_of_rays ,int h , int w ,  uint32_t *framebuffer){
	struct vec2  ray_normalized , result ;
	for(int i = 0 ; i<number_of_rays ; i++){
		printf("implementing the ray %d structure \n",i);
		for(int t =0 ; t<=rays[i].length ; t++){
			ray_normalized.x = rays[i].direction.x * t;
			ray_normalized.y = rays[i].direction.y * t;

			result = vec_add(rays[i],ray_normalized);

			framebuffer[((int)result.x * w) + ((int)result.y)] = ray_color ;
		};
	};

	printf("FRAMEBUFFER CREATED  READY FOR IMPLEMETATION \n");
	return 1;
};

int ray_creator(struct vec2 origin_info , struct ray_info *rays,int number_of_rays, int h , int w , uint32_t *framebuffer){
	int number_of_divide = 360 / number_of_rays ;
	char choice[1000];
	int  length;
	printf("enter the length of the rays \n");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};

	length =  (int)strtol(choice,NULL,10);

	double rad , radx , rady ;
	for(int i = 0 ; i<number_of_rays ; i++){

		int angle =  i * number_of_divide ;
		rad = angle * (pie/180);
		rays[i].origin.x = origin_info.x;
		rays[i].origin.y = origin_info.y;

		radx = cos(angle);
		rady = sin(angle);
		rays[i].direction.x = radx;
		rays[i].direction.y = rady;

		rays[i].length = length;
	};

	printf("RAYS DATA GENERATED ..... IMPLEMENTING THE RAY DATA ..... \n");

	if(ray_implement(rays,number_of_rays, h , w , framebuffer) <0){
		printf("OPERATION FAILED : RAYS IMPLEMENTATION \n");
		return -1;
	};

	printf("OPERATION COMPLETED \n");
	return 1;
};

int ray_info_taker(struct ray_info **rays, struct vec2 *origin_info , int *number_of_rays , int h , int w , uint32_t*framebuffer){
	char choice[1000];
	printf("ENTER THE RAY INFO \n");
	printf("ENTER THE NUMBER OF RAYS : ");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};

	*number_of_rays = (int) strtol(choice,NULL,10);
	*rays = (struct ray_info *)malloc(sizeof(struct ray_info) * (*number_of_rays));
	printf("\n INPUTTED \n");

	printf("ENTER THE ORIGIN OF THE RAYS \n");
	printf("X axis : ");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};
	printf("\n");
	origin_info->x = (int)strtol(choice,NULL,10);

	printf("Y axis : ");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};
	printf("\n");
	origin_info->y = (int)strtol(choice,NULL,10);

	if(ray_creator(*origin_info,*rays,*number_of_rays , h , w , framebuffer) <0){
		printf("OPERATION FAILED : RAY CREATION \n");
		return -1;
	};

	printf("operation completed : rays made \n");

	return 1;
};

int window_creator(SDL_Window **window , int h , int w , char name[]){
	printf("CREATING THE WINDOW \n");

	*window =  SDL_CreateWindow(name,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,h,w,0);

	if(!*window){
		perror("WINDOW CREATION ERROR \n");
		return -1;
	};

	printf("OPERATION COMPLETED : WINDOW CREATED \n");
	return 1;
};

int renderer_creator(SDL_Renderer **renderer ,  SDL_Window *window){
	printf("CREATING THE RENDERER \n");
	char choice[100];

	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};

	choice[strcspn(choice,"\n")] =  '\0';

	if(strcmp(choice,"GPU") == 0){
        	*renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
		if(!*renderer){
			perror("RENDERER CREATION ERROR \n");
			return -1;
		};
	}else if(strcmp(choice,"CPU") == 0){
        	*renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
		if(!renderer){
			perror("RENDERER CREATION ERROR \n");
			return -1;
		};
	};

	printf("OPERATION COMPLETED : RENDERER CREATION ");
	return 1;

};

int parser(int argc , char *args[], int *h , int *w , char name[]){
	printf("EXTRACTING THE PASSED COMMANDS \n");
	for(int i=0 ; i<argc ; i++){
		if(strcmp(args[i],"--h") == 0){
			*h =(int) strtol(args[i+1],NULL,10) ; 
			printf("HEIGHT EXTRACTED \n");
		}else if(strcmp(args[i],"--w") == 0){
			*w = (int) strtol(args[i+1],NULL,10) ;
			printf("WIDTH EXTRACTED \n");
		}else if(strcmp(args[i],"--name") == 0){
			strcpy(name,args[i+1]);
			printf("NAME EXTRACTED \n");
		};
	};

	printf("argument extracted \n");
	return 1;
};

int main(int argc , char *args[]){
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture ;
	struct vec2  origin_info;
	uint32_t *framebuffer ;
	int height , width ;
	struct ray_info *ray_origin;
	int number_of_rays;
	char name[50];
	if(parser(argc,args,&height,&width,name) <0){
		printf("OPERATION FAILED : COMMAND PARSER \n");
		return -1;
	};

	framebuffer = (uint32_t *) malloc(sizeof(uint32_t) * height*width);

	if(window_creator(&window , height , width , name)<0){
		printf("OPERATION FAILED : WINDOW CREATION \n");
		return -1;
	};

	if(renderer_creator(&renderer , window )<0){
		printf("OPERATION FAILED : RENDERER CREATION \n");
		return -1;
	};

	if(ray_info_taker(&ray_origin,&origin_info, &number_of_rays , height , width , framebuffer) <0){    /*has to also pass height and width in it and the framebuffer  */

		printf("OPERATION FAILED :  RAY CREATION \n");
		return -1;
	};

	if(texture_implementation(&texture , framebuffer , renderer , width ,  height) <0){
		printf("OPERATION FAILED : TEXTURE CREATION \n");
		return -1;
	};

	sleep(100);
	SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

	return 0;
};
