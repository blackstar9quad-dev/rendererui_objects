#include <stdio.h>
#include <SDL2/SDL.h>
#include <unistd.h>
#include <math.h>




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

int distance_finder(int x , int y , struct circle_structure circle){
	int x_axis =  x - circle.origin.x ;
	int y_axis =  y - circle.origin.y ;

	int distance;

	x_axis *= x_axis;
	y_axis *= y_axis;

	distance =  sqrt(x_axis+y_axis);
	double radius ;
	radius = circle.radius;
	radius *= radius;
	if(distance > radius){
		return -1;
	}else if(distance <= radius){
		return 1;
	};
};

int circle_creator(struct circle_structure *circle , uint32_t framebuffer[], int h , int w){
	int total_pixels =  h * w ;

	for(int x = 0 ; x<h ; x++){
		printf("checking the row : %d \n",x);
		for(int y = 0 ;  y<w ; y++){
			if(distance_finder(x,y,*circle) <0){
				framebuffer[(x*w)+y]= BACKGROUND_CLR;
			}else if(distance_finder(x,y,*circle)>0){
				framebuffer[(x*w)+y]= CIRCLE_CLR ;
			};
		};
	};

	printf("OPERATION COMPLETED : FRAMEBUFFER MADE \n");
	return 1;
};

int circle_info_gatherer(uint32_t framebuffer[] , struct circle_structure *circle, int h , int w){
	printf("Creating the circle \n");
	printf("Enter the center coordinates of the circle \n");
	char x[10] , y[10] ;
	char radius[100];

	printf("X : ");
	if(!fgets(x,sizeof(x),stdin)){
		perror("INPUT ERROR \n");
		return -1;
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

	circle->origin.x = (double) strtol(x,NULL,10);
	circle->origin.y = (double) strtol(y,NULL,10);
	circle->radius   = (double) strtol(radius,NULL,10);

	printf("circle info saved \n");

	if(circle_creator(circle,framebuffer,h,w) <0){
		printf("OPERATION FAILED \n");
		return -1;
	};

	return 1;
};

int window_creator(SDL_Window **window, int h , int w , char name[40]){
	printf("CREATING THE SDL WINDOW \n");

	*window = SDL_CreateWindow(name,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,w,h,0);

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

	choice[strcspn(choice,"\n")] = '\0';

	if(strcmp(choice,"CPU") == 0){
		*renderer =  SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
		if(!*renderer){
			perror("RENDERER  CREATION ERROR : CPU \n");
			return -1;
		};
		printf("Simulation now running throught he CPU \n");
	}else if(strcmp(choice,"GPU") == 0){
		*renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
		if(!*renderer){
			perror("RENDERER CREATION ERROR : GPU \n");
			return -1;
		};
		printf("Simulation now run on the GPU \n");
	};

	printf("OPERATION SUCCESSFUL : RENDERER  CREATION \n");
	return 1;
};

int create_texture(SDL_Texture **texture ,SDL_Renderer *renderer, int h , int w){
	*texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,w,h);
	if(!*texture){
		perror("ERROR IN CREATING THE TEXTURE \n");
		return -1;
	};

	printf("OPERATION COMPLETED : TEXTURE MADE \n");
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
			strcpy(name,args[i+1]);
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

	if(parser(argc,args,&height,&width,name) <0){
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

	uint32_t *framebuffer = (uint32_t *) malloc(height * width * sizeof(uint32_t));
	if(circle_info_gatherer(framebuffer,&circle,height,width) <0){
		printf("OPERATION FAILED \n");
		return -1;
	};
	printf("Raw framebuffer defined \n");

	SDL_Texture *texture ;

	if(create_texture(&texture,renderer,height,width) < 0){
		printf("OPERATION FAILED \n");
		return -1;
	};

	SDL_UpdateTexture(texture,NULL,framebuffer,width*sizeof(uint32_t));
	SDL_RenderCopy(renderer,texture,NULL,NULL);
	SDL_RenderPresent(renderer);

	sleep(100);
	SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0 ;
};
