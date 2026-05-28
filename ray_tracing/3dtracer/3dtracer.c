#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>

struct vec2{
	double x ;
	double y ;
};

struct vec3{
	double x ;
	double y ;
	double z ;
};

struct sphere_info{
	struct vec3 origin ;
	int radius ;
};

struct ray_info{
	struct vec3 origin ;  //same the camera origin ;
	struct vec3 direction ;
};


struct camera_info{
	struct vec3 origin;
	int viewportheight ;
	int viewportwidth  ;
	int viewportdistance ; //used as z for the viewport points converted from the vec2 framebuffer
};


int camera_detail(struct camera_info *camera){
	char choice[1000];

	printf("ENTER THE CAMERA INFO \n");
	printf("ENTER X : ");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUIT ERROR \n");
		return -1;
	};

	camera->origin.x = (double)strtol(choice,sizeof(choice),stdin);
	printf("\n");

	printf("ENTER Y : ");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};
	camera->origin.y = (double)strtol(choice,NULL,10);
	printf("\n");

	printf("ENTER Z : ");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;  
	};
	camera->origin.z = (double)strtol(choice,NULL,10);
	printf("\n");

	printf("ENTER THE VIEWPORT HEIGHT : " );
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};
	camera->viewportheight = (int)strtol(choice,NULL,10);
	printf("\n");

	printf("ENTER THE VIEWPORT WIDTH : " );
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};
	camera->viewportwidth = (int)strtol(choice,NULL,10);
	printf("\n");

	printf("ENTER THE VIEWPORT DISTANCE : " );
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};
	camera->viewportdistance= (int)strtol(choice,NULL,10);
	printf("\n");


	printf("INFO SAVED \n");
	return -1 ;
};

int sphere_detail(struct sphere_info *sphere){
	char choice[100];

	printf("ENTER THE SPHERE ORIGIN \n");
	printf("ENTER X : ");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};
	sphere->origin.x = (double)strtol(choice,NULL,10);
	printf("\n");

	printf("ENTER Y : ");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};
	sphere->origin.y = (double)strtol(choice,NULL,10);
	printf("\n");

	printf("ENTER Z : ");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};
	sphere->origin.z = (double)strtol(choice,NULL,10);
	printf("\n");

	printf("ENTER Z : ");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};
	sphere->radius =  (double)strtol(choice,NULL,10);
	printf("\n");


	printf("DATA SAVED \n");

	return 1;
};

int renderer_creation(SDL_Renderer **renderer , SDL_Window *window){
	printf("CREATING THE RENDERER ... \n");
	char choice[100];

	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};

	choice[strcspn(choice,"\n")] = '\0' ;
	if(strcmp(choice,"CPU") == 0){
		*renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
	}else if(strcmp(choice,"GPU") == 0 ){
		*renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATION);
	};

	if(!*renderer){
		perror("ERROR IN CREATING THE RENDERER \n");
		return -1;
	};

	printf("OPERATION SUCCESSFUL : RENDERER CREATED \n");
	return 1;
};

int window_creator(SDL_Window **window , int h , int w , char *name){
	printf("WINDOW CREATION STARTED ...\n");

	*window = SDL_CreateWindow(name,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,h,w,0);

	if(!*window){
		perror("ERROR IN CREATION OF WINDOW \n");
		return -1;
	};

	printf("OPERATION SUCCESSFUL : WINDOW CREATED \n");
	return -1;
};

int parser(int argc  , char *args[] , int *h , int *w . char *name){
	printf("PARSING THE PASSED COMMAND \n");

	for( int i = 0 ; i<argc ; i++){
		if(strcmp(args[i],"--h") == 0){
			*h = (int)strtol(args[i+1],NULL,10);
			printf("HEIGHT SAVED \n");
		}else if(strcmp(args[i],"--w") == 0){
			*w = (int)strtol(args[i+1],NULL,10);
			printf("WIDTH SAVED \n");
		}else if(strcmp(args[i],"--name") == 0){
			strcpy(name,args[i+1]);
			printf("NAMED SAVED \n");
		};
	};


	printf("OPEARTION SUCCESSFUL : COMMAND PARSED \n");
	return 1;
};

int main(int argc , char *args[]){
	SDL_Window *window ;
	SDL_Renderer *rendere ;
	SDL_Texture *texture ;
	int height , width ;
	char name[100];
	struct camera_info camera ;
	struct ray_info *rays ;
	struct sphere_info sphere ;
	int number_of_rays ;
	uint32_t *framebuffer ;

	if(parser(argc,args,&height,&width,name) <0){
		printf("OPERATION FAILED : PARSER FAILED \n");
		return -1;
	};

//-------------------------------------------------------------------------------------------------//
	framebuffer = (uint32_t *)malloc(height * width * sizeof(uint32_t));
	if(!framebuffer){
		perror("MEMORY ALLOCATION ERROR \n");
		return -1;
	};
//-------------------------------------------------------------------------------------------------//


	if(window_creator(&window , height , width , name) <0){
		printf("OPERATION FAILED : WINDOW CREATION FAILED \n");
		return -1;
	};

	if(renderer_creation(&renderer,window) <0){
		printf("OPERATION FAILED : RENDERER CREATION FAILED \n");
		return -1 ;
	};

//-------------------------------------------------------------------------------------------------//
	char choice[10000];
	printf("ENTER THE NUMBER OF RAYS TO BE GENERATED \n");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};
	number_of_rays =  (int)strtol(choice,NULL,10);
	printf("number of rays saved \n");
	rays = (struct ray_info *)malloc(number_of_rays * sizeof(struct rays_info));
	if(!rays){
		perror("MEMORY ALLOCATION ERROR \n");
		return -1;
	};
//-------------------------------------------------------------------------------------------------//

	if(sphere_detail(&sphere) <0){
		printf("OPERATION FAILED : SPHERE CREATION \n");
		return -1;
	};

	if(camera_detail(&camera) <0){
		printf("OPERATION FAILED : CAMERA CREATION \n");
		return -1;
	};
};
