#include <stdio.h>
#include <SDL2/SDL>


#define CIRCLE_CLR  0xffffffff
#define BACKGROUND_CLR 0x00000000

struct vec2{
	double x;
	double y;
};

struct circle_structure{
	double radius ;
	struct vec2 origin;
};

int distance_finder(){
};

int circle_creator(struct circle_info *circle , uint32_t framebuffer[]){
	
};

int circle_info_gatherer(uint32_t framebuffer[] , struct circle_structure *circle){
	printf("Creating the circle \n");
	printf("Enter the center coordinates of the circle \n");
	char x[10] , y[10] ;
	char radius[100];

	printf("X : ");
	if(!fgets(x,sizeof(x),stdin)){
		perror("INPUT ERROR \n");
		return -1
	};
	printf("\n");

	printf("Y : ");
	if(!fgets(y,sizeof(y),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};
	printf("\n");

	printf("ENTER THE RADIUS : ");
	if(!fgets(radius,sizeof(radius),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};
	printf("\n");

	circle.origin.x = (double) strtol(x,NULL,10);
	circle.origin.y = (double) strtol(y,NULL,10);
	circle.radius   = (double) strtol(radius,NULL,10);

	printf("circle info saved \n");

	if(circle_creator(circle) <0){
		printf("OPERATION FAILED \n");
		return -1;
	};

	return 1;
};

int window_creator(SDL_Window **window, int h , int w , char name[40]){
	printf("CREATING THE SDL WINDOW \n");

	*window = SDL_CreateWindow(name,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTER,h,w,0);

	if(!*window){
		perror("WINDOW CREATION ERROR \n");
		return -1;
	};

	printf("OPERATION SUCCESSFUL : WINDOW CREATED \n");
	return 1;
};

int renderer_creator(SDL_Renderer **renderer , SDL_Window *window){
	printf("DESCRIBING THE RENDERER \n");
	char choice[100];

	printf("DO U WANNA RUN THE SIMULATION ON THE 1>CPU , 2>GPU\n");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};

	if(strcmp(choice,"CPU") == 0){
        	*renderer =  SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
		if(!renderer){
			perror("RENDERER  CREATION ERROR : CPU \n");
			return -1;
		};
		printf("Simulation now running throught he CPU \n");
	}else if(strcmp(choice,"GPU") == 0){
		*renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
		if(!renderer){
			perror("RENDERER CREATION ERROR : GPU \n");
			return -1;
		};
		printf("Simulation now run on the GPU \n");
	};

	printf("OPERATION SUCCESSFUL : RENDERER  CREATION \n");
	return 1;
};



int parser(int argc , char *args[], int *h , int *w , char name[40]){
	printf("PARING THE COMMAND \n");

	for(int i = 0 ; i < argc ; i++){
		if(strcmp(args[i],"--h") == 0){
			*h = (int) strtol(args[i+1],NULL,10);
			printf("HEIGHT SAVED \n");
		}else if(strcmp(args[i],"--w") == 0){
			*w = (int) strtol(args[i+1],NULL,10);
			printf("WIDTH SAVED \n");
		}else if(strcmp(args[i],"--name") == 0){
			strcpy(args[i+1],name);
			printf("NAME SAVED \n");
		};
	}

	printf("OPERATION SUCCESSFUL : COMMAND PARSED 	\n");
	return 1;
};

int main(int argc , char *args[]){
	int height , width , result ;
	char name[40];

	struct circle_structure circle ;

/*--------------------------------------------------------------------------------------------------*/

	SDL_Window *window;
	SDL_Renderer *renderer;


	SDL_Init(SDL_INIT_VIDEO);
	printf("SDL INITILISED \n");

	if(parser(argc,args,&height,&width) <0){
		printf("OPERATION FAILED \n");
		return -1;
	};

	if(window_creator(&window,height,width,name) < 0){
		printf("OPERATION FAILED : window creation \n");
		return -1;
	};

	if(renderer_creator(&renderer,window) <0){
		printf("OPERATION FAILED : renderer creation \n");
		return -1;
	};

/*--------------------------------------------------------------------------------------------------*/

	uint32_t framebuffer[height*width];
	if(circle_info_gatherer(framebuffer,&circle) <0){
		printf("OPERATION FAILED \n");
		return -1;
	};
	printf("Raw framebuffer defined \n");


};
