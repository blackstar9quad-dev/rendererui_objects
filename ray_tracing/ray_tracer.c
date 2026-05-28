#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>


struct color {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};
software_redering_ui
struct vec2{
	double x ;
	double y;
};

struct interaction_points{
	struct ray_info *ray_detail;
	struct vec2 interaction_point;
};

struct ray_info{
	struct vec2 origin_ray;
	struct vec2 direction;
	int length;
};

struct circle_info{
	struct vec2 origin;
	double radius;
};

int intensity_maker(){
};

int quad_eq(struct circle_info circle , struct ray_info rays){
	double Dx,Dy,Cx,Cy,Ox,Oy,R;

	Dx = rays.direction.x ;
	Dy = rays.direction.y ;
	Cx = circle.origin.x ;
	Cy = circle.origin.y ;
	Ox = rays.origin.x ;
	Oy = rays.origin.y ;
	R  = circle.radius;

	Deltax = Ox - Cx ;
	Deltay = Oy - Cy ;

	double a , b ,c ,dis ,t ;

	a = (Dx * Dx) + (Dy * Dy);
	b = 2*((Deltax*Dx) +   (Deltay*Dy));
	c = ((Deltax * Deltax) + (Deltay * Deltay) - (R * R));

	dis = ((b * b) - (4 * a * c ));

	if(t<0){
		return 0;
	};

	t = (-b - (sqrt(dis))) / 2 *a ;

	return t ;
}

struct vec2 interaction_points_finder(struct ray_info rays , int t){
	struct vec2 result ;

	result.x = rays.origin.x + (t * rays.direction.x) ;
	result.y = rays.origin.y + (t * rays.direction.y) ;

	return result ;
};

void buffer_implementor(uint32_t *circlebuffer , struct vec2 interaction_points , int w ){
	double x , y ;
	x = interaction_points.x ;
	y = interaction_points.y ;

	circlebuffer[(x*w)+y] = RAYCOLOR;

};

int implementor(struct circle_info circle , struct ray_info *rays ,  int number_of_rays){
	struct vec2 interaction_points;
	int result;
	for(int i = 0 ; i<=number_of_rays ; i++){
		if(quad_eq(circle , rays[i])<0){
			printf("OPERATION FAILED");
			return -1;
		}else if(quad_eq(circle , rays[i]) == 0){
			printf("NO interection with the circle \n");
			return -1;
		}else if(result=quad_eq(circle , rays[i]) >0){
			if(interaction_points = (struct vec2)interaction_points_finder(rays[i] ,result) <0){
				printf("OPERATION FAILED : COMPUTING INTERECTION POINTS \n");
				return -1;
			};

			buffer_implementor(circlebuffer,interaction_points,w);
		};
	};
};

int ray_generator(struct ray_info **rays){
	char choice[100];
	int number_of_rays;
	struct vec2 origin ;
	printf("ENTER THE NUMBR OF RAYS TO BE GENERATED : ");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};

	printf("\n");

	number_of_rays = (int)strtol(choice,NULL,10);

	*rays = (struct ray_info *)malloc(number_of_rays * sizeof(struct ray_info));

	printf("ENTER THE ORIGIN POINT OF THE RAYS \n ");
	printf("ENTER X : ");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
	};

	printf("\n");

	origin.x = (double)strtol(choice,NULL,10);

	printf("ENTER Y : ");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};

	printf("\n");

	origin.y = (double)strtol(choice,NULL,10);

	double rad , radx, rady ;
	for(int angle=0 ; i<=360 ; i++){
		rad = (angle * pie)/180 ;

		radx = cos(rad);
		rady = sin(rad);

		(*rays)[i].origin.x =  origin.x;
		(*rays)[i].origin.y =  origin.y;
		(*rays)[i].direction.x = radx ;
		(*rays)[i].direction.y = rady ;
	};

	printf("rays origin and direction established \n");

	return 1;
};



int distance_checker(struct circle_info circle , double x , double y){
	double dis_x , dis_y , sudo_dis ;

	dis_x = x-circle.origin.x;
	dis_y = y-circle.origin.y;

	dis_x *= dis_x ;
	dis_y *= dis_y ;

	sudo_dis = sqrt(dis_x + dis_y);

	if(sudo_dis <= circle.radius){
		return 1;
	}else if(sudo_dis > circle.radius){
		return -1;
	};
};

int circle_generation(struct circle_info circle, uint32_t *circle_buffer , int h , int w ){
	int total_pixel ;
	total_pixel =  h * w ;

	for(int x= 0 ; i<h ; i++){
		printf("WORKING IN ROW : %d ",x);
		for(int y=0 ; y<w ; y++){
			if(distance_checker(circle,x,y) <0){
				circle_buffer[(x*w)+y] = BACKGROUND_CLR;
			}else if(distance_checker(circle,x,y) > 0){
				circle_buffer[(x*w)+y] = CIRCLE_CLR ;
			};
		};
	};

	printf("CIRCLE MADE \n");

	return 1;
};

int circle_creator(struct circle_info *circle ,  uint32_t *circle_buffer ,  int h , int w){
	char choice[100];



	printf("Enter the circle info \n");
	printf("Enter X : ");
	if(!fgets(choice,sizeof(fgets),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};

	circle->origin.x = (double)strtol(choice,NULL,10);

	printf("\n");

	printf("Enter Y : ");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};

	circle->origin.y = (double)strtol(choice,NULL,10);

	printf("\n");

	printf("Enter the radius : " );
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("IMPUT ERROR \n");
		return -1;
	};

	circle->radius = (double)strtol(choice,NULL,10);

	printf("\n");

	printf("CIRCLE INFO TAKEN SUCCESSFULLY \n");

	if(circle_generator(*circle,circle_buffer,h,w) <0){
		printf("OPERATION FAILED :CIRCLE GENERATION \n");
		return -1;
	}

	printf("OPERATION SUCCESSFUL : CIRCLE GENERATED \n");
	return 1;
};

int renderer_creator(SDL_Renderer **rendere , SDL_Window *window){
	printf("1>CPU , 2>GPU \n");
	char choice[100];

	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};

	choice[strcspn(choice,"\n")] = '\0';

	if(strcmp(choice,"CPU") == 0){
        	*renderer =  SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
	}else if(strcmp(choice,"GPU") == 0){
        	*renderer =  SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
	};

	if(!*renderer){
		perror("RENDERER CREATION ERROR \n");
		return -1;
	};

	printf("OPERATION SUCCESSFUL  :  RENDERER CREATED \n");
	return 1;
};

int window_creator(SDL_Window **window , int h , int w , char name[]){
	*window = SDL_CreateWindow(name,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,h,w,0);
	if(!*window){
		perror("WINDOW CREATION ERROR \n");
		return -1;
	};

	printf("WINDOW CREATED \n");
	return 1;
};

int parser(int argc ; char *args[] ; int *h , int *w , char name[]){
	printf("STARTING PARSING THE COMMAND PASSED \n");
	for(int i = 0 ;  i<=argc ; i++){
		if(strcmp(args[i],"--h") == 0){
			*h = (int)strtol(args[i+1],NULL,10);
			printf("height saved \n");
		}else if(strcmp(args[i],"--w") == 0){
			*w = (int)strtol(args[i+1],NULL,10);
			printf("width  saved \n");
		}else if(strcmp(args[i],"--name") == 0){
			strcpy(name,args[i+1]);
			printf("name saved \n");
		};
	};

	printf("DATA PARSED \n");
	return 1;
};

int main(int argc , char *args[]){
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture ;
	int height , width ;
	char name[50];
	struct circle_info circle;
	struct ray_info *rays;
	uint32_t *circle_buffer , *ray_buffer ;
	if(parser(argc,args,&height,&width,name) <0){
		printf("OPERATION FAILED : PARSING \n");
		return -1;
	};

	circle_buffer = (uint32_t *)malloc(height * width * sizeof(uint32_t));
	ray_buffer    = (uint32_t *)malloc(height * width * sizeof(uint32_t));

	if(!circle_buffer){
		perror("MEMORY ALLOCATION ERROR : CIRCLE BUFFER \n");
		return -1;
	};

	if(!ray_buffer){
		perror("MEMORY ALLOCATION ERROR : RAY BUFFER \n");
		return -1;
	};

	if(window_creator(&window,height,width,name) <0){
		printf("OPERATION FAILED : WINDOW ERROR \n");
		return -1;
	};

	if(renderer_creator(&renderer,window) <0){
		printf("OPERATION FAILED : RENDERER ERROR \n");
		return -1;
	};

	if(circle_creation()<0){
		printf("OPERATION FAILED : CIRCLE CREATION \n");
		return -1;
	};

	if(ray_generator() <0){
		printf("OPERATION FAILED \n");
		return -1;
	};

	if(implementor() <0){
		printf("OPERATION FAILED : IMPLEMENTATION \n");
		return -1;
	};


};
