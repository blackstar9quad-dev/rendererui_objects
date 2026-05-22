#include <stdio.h>
#include <SDL2/SDL.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#define CIRCLE_CLR 0xffffffff
#define BACKGROUND_CLR 0x00000000

struct vec2{
	double x;
	double y;
};

struct circle_info{
	double radius;
	struct vec2 origin;
};

int distance_checker(struct circle_info circle , int x ,  int y){
	double x_axis , y_axis , radius;
	x_axis = x - circle.origin.x ;
	y_axis = y - circle.origin.y;
	radius = circle.radius;

	x_axis *= x_axis;
	y_axis *= y_axis;
	radius *= radius;

	double dis;

	dis =  sqrt(x_axis + y_axis);

	if(radius > dis ){
		return 1;
	}else if(radius < dis){
		return -1;
	};
};

int initilizor(struct circle_info circle ,  int h , int w , uint32_t *framebuffer){
	for(int x = 0 ; x<h ; x++){
		/*printf("WORKING ON ROW : %d ",x);*/
		for( int y = 0 ; y<w ; y++){
			if(distance_checker(circle,x,y) <0){
				framebuffer[(x*w)+y] = CIRCLE_CLR;
			}else if(distance_checker(circle,x,y) >0){
				framebuffer[(x*w)+y] = BACKGROUND_CLR;
			};
		};
	};
};

int texture_creator(SDL_Texture **texture, SDL_Renderer *renderer , int h , int w , uint32_t *framebuffer){
	if(*texture == NULL){
		*texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,w,h);
		if(!*texture){
         		perror("TEXTURE CREATION ERROR \n");
	        	return -1;
        	};
	};

	SDL_UpdateTexture(*texture,NULL,framebuffer,w*sizeof(uint32_t));
	SDL_RenderCopy(renderer,*texture,NULL,NULL);
	SDL_RenderPresent(renderer);
};

int event_handler(SDL_Event *event, SDL_Renderer *renderer , struct circle_info *circle , int h , int w , uint32_t *framebuffer,  struct SDL_Texture **texture){
	int flag;
	flag = 1;


	if(initilizor(*circle,h,w,framebuffer) ,0){
		printf("ERROR IN INITLIZING THE CIRCLE IN THE FRAMEBUFFER \n");
		return -1;
	};

	if(texture_creator(texture,renderer,h,w,framebuffer) <0){
		printf("OPERATION FAILED : TEXTURE CREATION \n");
		return -1 ;
	};

	while(flag){
		while(SDL_PollEvent(event) !=0){
			if(event->type == SDL_QUIT){
				flag = 0 ;
			}else if(event->type == SDL_MOUSEMOTION && event->motion.state != 0){
				circle->origin.x =event->motion.x ;
				circle->origin.y =event->motion.y ;
			};

			if(initilizor(*circle,h,w,framebuffer)<0){
				printf("ERROR IN INIILIZING THE FRAMEBUFFER \n");
				return -1;
			};

			if(texture_creator(texture,renderer,h,w,framebuffer)<0){
				printf("OPERATION FAILED : TEXTURE CREATION \n");
				return -1;
			};
		};
	};

	return 0;
};

int circle_info_taker(struct circle_info *circle){
	char choice[1024];

	printf("ENTER THE ORIGIN POINTS OF THE CIRCLE \n");
	printf("ENTER THE X AXIS : ");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};

	circle->origin.x = (double)strtol(choice,NULL,10);

	printf("\n");

	printf("ENTER THE Y AXIS : ");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};

	circle->origin.y = (double)strtol(choice,NULL,10);
	printf("\n");

	printf("ENTER THE RADIUS : ");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};

	circle->radius = (double)strtol(choice,NULL,10);
	printf("\n");

	printf("INFO TAKEN \n");

	return 1;
};

int renderer_creator(SDL_Renderer **renderer , SDL_Window *window){
	char choice[50];
	printf("DEFINING THE RENDERER \n");

	printf("1>CPU \n 2>GPU \n");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};

	choice[strcspn(choice,"\n")] = '\0';

	if(strcmp(choice,"GPU") == 0){
        	*renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
	}else if(strcmp(choice,"CPU") == 0){
        	*renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
	};

	if(!*renderer){
		perror("RENDERER CREATION \n");
		return -1;
	};

	printf("OPERATION COMPLETED : RENDERER CREATED \n");

	return 1;
};

int window_creator(SDL_Window **window , int h ,  int w , char name[]){
	printf("CREATING THE WINDOW \n");

	*window = SDL_CreateWindow(name,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,h,w,0);
	if(!*window){
		perror("WINDOW CREATION ERROR \n");
		return -1;
	};

	printf("OPERATION SUCCESSFUL : WINDOW CREATED \n");
	return 1;
};

int parser(int argc , char *args[] , int *h , int *w , char name[]){
	printf("PARSING THE PASSED COMMANDS ...... \n");
	for(int i = 0 ; i<argc ; i++){
		if(strcmp(args[i],"--h") == 0){
			*h = (int)strtol(args[i+1],NULL,10);
			printf("HEIGHT TAKEN \n");
		}else if(strcmp(args[i],"--w") == 0){
			*w = (int)strtol(args[i+1],NULL,10);
			printf("WIDTH  TAKEN \n");
		}else if(strcmp(args[i],"--name") == 0){
			strcpy(name,args[i+1]);
			printf("NAME TAKEN \n");
		};
	};

	printf("OPERATION SUCCESSFUL : COMMAND PARSED \n");
	return  1;
};

int  main(int argc , char *args[]){
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture ;
	SDL_Event event;
	int height , width ;

	window , renderer , texture = NULL;
	uint32_t *framebuffer;
	char name[20];
	struct circle_info circle ;

	if(parser(argc,args,&height,&width,name) <0){
		printf("OPERATION FAILED : COMMAND PARSING \n");
		return -1;
	};

	framebuffer = (uint32_t *) malloc(height * width * sizeof(uint32_t));

	if(window_creator(&window,height,width,name) <0){
		printf("OPERATION FAILED : WINDOW CREATION \n");
		return -1;
	};

	if(renderer_creator(&renderer,window)<0){
		printf("OPERATION FAILED : RENDERER CREATION \n");
		return -1;
	};

	if(circle_info_taker(&circle) <0){
		printf("OPERATION FAILED : CIRCLE INFO \n");
		return -1;
	};

	if(event_handler( &event,renderer ,&circle,height,width,framebuffer,&texture) <0){
		printf("OPERATION FAILED : EVENT HANDLER \n");
		return -1;
	};


};

