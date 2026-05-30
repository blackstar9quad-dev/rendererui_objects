#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <unistd.h>

#define LIGHT_CLR 0xFFDF00

struct vec3{
	double x ; 
	double y ;
	double z ;
};

struct sphere_info{
	struct vec3 origin ;
	double radius ;
};

struct camera_info{
	struct vec3 origin;
	struct vec3 light_source;
	int viewpointwidth ;
	int viewpointheight ;
	int viewpointlength ;
};

struct vec3 vec3add(struct vec3 vector1 , struct vec3 vector2){
	struct vec3 result;

	result.x = vector1.x + vector2.x ;
	result.y = vector1.y + vector2.y ;
	result.z = vector1.z + vector2.z ;

	return result;
};

struct vec3 vec3sub(struct vec3 vector1 , struct vec3 vector2){
	struct vec3 result ;

	result.x = vector2.x - vector1.x ;
	result.y = vector2.y - vector1.y ;
	result.z = vector2.z - vector1.z ;
	
	return result;
};

double vec3dot(struct vec3 vector1 , struct vec3 vector2){
	double result;

	result =  ((vector1.x *  vector2.x )+(vector1.y * vector2.y)+(vector1.z * vector2.z));

	return result ;
};

struct vec3 vec3div(struct vec3 vector1 , double divisor){
	struct vec3 result ;

	result.x = vector1.x / divisor;
	result.y = vector1.y / divisor;
	result.z = vector1.z / divisor;

	return result;
};

double intensity(int x , int y , double t , struct camera_info camera , struct sphere_info sphere,struct vec3 directions){
	struct vec3  Vn,N,Q,P,L;
	double I ;

	P.x = (camera.origin.x) + (directions.x * t);
	P.y = (camera.origin.y) + (directions.y * t);
	P.z = (camera.origin.z) + (directions.z * t) ;

	Vn = vec3sub(sphere.origin,P);

	N = vec3div(Vn,sphere.radius);

	Q = camera.light_source ;

	L = vec3sub(P,Q);

	double magnitude = sqrt((L.x * L.x) + (L.y * L.y) + (L.z * L.z));
	L = vec3div(L,magnitude);

	double dot_product = vec3dot(N,L);

	I = 8000 * ((fmax(0,dot_product)) / (magnitude * magnitude));
	
	return I;
};

double quad_eq(struct vec3 eq_points){
	double a,b,c,t1 , t2 , t , diss ;

	a= eq_points.x ;
	b= eq_points.y ;
	c= eq_points.z ;

	diss = (b * b) - ( 4 * a * c) ;

	if(diss <0){
		return -1;
	};

	t1 = ((-b) - (sqrt(diss))) / (2 * a) ;
	t2 = ((-b) + (sqrt(diss))) / (2 * a) ;

        if(t1 > 0 && t2 > 0){
		t = fmin(t1,t2);
	}else if(t1 > 0){
		t = t1;
	}else if(t2 > 0){
		t = t2;
	}else{
		    return -1;
	}


	return t ;
};

struct vec3 point_generator(struct vec3 directions , struct camera_info camera , struct sphere_info sphere){
	struct vec3 result ;
	double Dx,Dy,Dz,Cx,Cy,Cz,Ox,Oy,Oz,R;

	Ox = camera.origin.x ;
        Oy = camera.origin.y ;
        Oz = camera.origin.z ;
        Cx = sphere.origin.x ;
        Cy = sphere.origin.y ;
        Cz = sphere.origin.z ;
        Dx = directions.x ;
        Dy = directions.y ;
        Dz = directions.z ;
        R  = sphere.radius;

	result.x = ((Dx * Dx) + (Dy * Dy) + (Dz * Dz));
	result.y = 2 *((Dx * (Ox - Cx)) + (Dy * (Oy - Cy)) + (Dz * (Oz - Cz)));
	result.z = (((Ox - Cx) * (Ox - Cx)) + ((Oy - Cy) * (Oy - Cy)) + ((Oz - Cz) * (Oz - Cz))) - (R*R);

	return result;
};

struct vec3 direction_generator(struct vec3 viewpoints , struct camera_info camera){
	struct vec3  result ;

	result = vec3sub(camera.origin,viewpoints);

	return result ;
};


struct vec3 viewpoint_generator(int x , int y , struct camera_info camera ,  int h , int w){
	struct vec3 result;

	result.x = (((double)x / (double)w) - 0.5) * camera.viewpointwidth;
	result.y = (0.5 - ((double)y / (double)h)) * camera.viewpointheight;
	result.z = camera.viewpointlength ;

	return result ;
};

int texture_call(SDL_Texture **texture , SDL_Renderer *renderer, int h , int w , uint32_t *framebuffer){
	if(!*texture){
		*texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,w,h);
	};

	if(!*texture){
		perror("TEXTURE CREATION ERROR \n");
		return -1;
	};

	SDL_UpdateTexture(*texture,NULL,framebuffer,w*sizeof(uint32_t));
	SDL_RenderCopy(renderer,*texture,NULL,NULL);
	SDL_RenderPresent(renderer);

	return 1;
};

uint32_t color_generator(double I ){

	int  red , blue , green ;

	red = (LIGHT_CLR >> 16) & 0xFF ;
	green = (LIGHT_CLR >> 8) & 0xFF ;
	blue = LIGHT_CLR & 0XFF ;

	red = (int)(red * I);
	green = (int)(green * I);
	blue = (int)(blue * I);

	if(red > 255) {
		red = 255 ;
	};

	if(green > 255) {
		green = 255 ;
	};

	if(blue > 255) {
		blue = 255 ;
	};

	uint32_t color = ((255 << 24) | (red << 16) | (green << 8) | blue) ;
	
	return color ;
};


int light_generator(struct camera_info camera  , int h , int w , uint32_t *framebuffer){

	struct vec3 viewpoints,directions , eq_points ;
	double t ;
	struct sphere_info sphere_light;

	sphere_light.origin.x = camera.light_source.x ; 
	sphere_light.origin.y = camera.light_source.y ;
	sphere_light.origin.z = camera.light_source.z ;


	sphere_light.radius = 10;

	for(int y = 0 ; y<h ; y++){
		for(int x = 0 ; x<w ; x++){
			viewpoints = viewpoint_generator(x,y,camera,h,w);
			directions = direction_generator(viewpoints,camera);
			eq_points  = point_generator(directions , camera , sphere_light);

			t = quad_eq(eq_points);

			if(t>=0){
				framebuffer[(y*w)+x] = 0xFFFFFFFF;
			};

		};
	};

	printf("light sphere generated \n");
	return 1;
};

int initilizor(uint32_t *framebuffer ,struct sphere_info sphere , struct camera_info camera , int h , int w , struct SDL_Texture **texture ,  SDL_Renderer *renderer){
	struct vec3 viewpoints , directions , eq_points ;
	double t , I ;

	for(int y= 0 ; y<h ; y++){
		for(int x=0 ; x<w ; x++){
			viewpoints = viewpoint_generator(x,y,camera,h,w);
			directions = direction_generator(viewpoints,camera);
			eq_points  = point_generator(directions , camera , sphere);

			t = quad_eq(eq_points);

			if(t >= 0){
				I = intensity(x,y,t,camera,sphere,directions);
				framebuffer[(y*w)+x] = (uint32_t)color_generator(I);
			}else if(t<0){
				framebuffer[(y*w)+x] = 0x00000000 ;
			};
		};
	};



	if(light_generator(camera,h,w,framebuffer) <0){
		printf("OPERATION FAILED : light sphere generation \n");
		return -1;
	};



	if(texture_call(texture,renderer,h,w,framebuffer)<0){
		printf("OPERATION  FAILED :  INITLIZING TEXTURE \n");
		return -1;
	};

	return 1;
};

int event_handler(SDL_Event *event , struct camera_info *camera , struct sphere_info sphere , int h , int w ,SDL_Texture **texture , uint32_t *framebuffer ,SDL_Renderer *renderer , SDL_Window *window){
	printf("intilizing the fist time \n");
	if(initilizor(framebuffer,sphere,*camera,h,w,texture,renderer) < 0){
		printf("OPERATION FAILED : INITLIZATION GONE WRONG \n");
		return -1;
	};

	bool flag = true ;

	while(flag){
		while(SDL_PollEvent(event) != 0 ){
			if(event->type == SDL_QUIT){
				flag = false;
				break;
			}else if(event->type == SDL_MOUSEMOTION && event->motion.state !=0){
				bool flag = false ;
				if(event->motion.x == camera->origin.x && event->motion.y == camera->origin.y){
					camera->origin.x = event->motion.x ;
					camera->origin.y = event->motion.y ;

				}else if(event->motion.x == camera->light_source.x && event->motion.y == camera->light_source.y){
					camera->light_source.x  = event->motion.x ;
					camera->light_source.y  = event->motion.y ;

				};
			};
			if(initilizor(framebuffer,sphere,*camera,h,w,texture,renderer) < 0){ 
				printf("OPERATION FAILED : INITLIZATION GONE WRONG \n");   
				return -1;      
			};
		};
	};


	printf("OUT OF THE EVENT LOOP \n");

	return 1;

};

int camera_creator(struct camera_info *camera){
	char choice[1000];

	printf("ENTER THE INFO FOR THE CAMERA \n");
	printf("ENTER X : ");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};
	camera->origin.x = (double)strtol(choice,NULL,10);
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

	printf("ENTER THE LIGHT SOURCE POINT \n");
	printf("ENTER X : ");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};
	camera->light_source.x = (double)strtol(choice,NULL,10);
	printf("\n");

	printf("ENTER Y : ");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};
	camera->light_source.y = (double)strtol(choice,NULL,10);
	printf("\n");

	printf("ENTER Z : ");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};
	camera->light_source.z = (double)strtol(choice,NULL,10);
	printf("\n");

	printf("ENTER VIEWPOINTWIDTH : ");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};
	camera->viewpointwidth= (int)strtol(choice,NULL,10);
	printf("\n");

	printf("ENTER VIEWPOINTHEIGHT : ");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};
	camera->viewpointheight= (int)strtol(choice,NULL,10);
	printf("\n");

	printf("ENTER VIEWPOINTLENGTH : ");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};
	camera->viewpointlength= (int)strtol(choice,NULL,10);
	printf("\n");

	printf("CAMERA INFO SAVED \n");
	return 1;
};

int sphere_creator(struct sphere_info *sphere){
	char choice[1000];

	printf("ENTER THE INFO FOR THE SPHERE \n");
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

	printf("ENTER RADIUS : ");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};
	sphere->radius = (double)strtol(choice,NULL,10);
	printf("\n");

	printf("INFO SAVED : OPERATION SUCCESSFULL \n");

	return 1;

};

int renderer_creation(SDL_Renderer **renderer , SDL_Window *window){
	printf("creating the renderer \n");
	char choice[100];
	printf("1>CPU \n 2> GPU \n");
	if(!fgets(choice,sizeof(choice),stdin)){
		perror("INPUT ERROR \n");
		return -1;
	};

	choice[strcspn(choice,"\n")] = '\0';

	if(strcmp(choice,"CPU") == 0){
		*renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
	}else if(strcmp(choice,"GPU") == 0){
		*renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
	};

	if(!*renderer){
		perror("RENDERER CREATION ERROR \n");
		return -1;
	};

	printf("OPERATION SUCCESSFUL : RENDERER  CREATION \n");
	return 1;
};

int window_creation(SDL_Window **window , int h  ,int w , char name[]){
	printf("creating the window \n");

	*window = SDL_CreateWindow(name,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,h,w,0);
	if(!*window){
		perror("WINDOW CREATION ERROR \n");
		return -1;
	};

	printf("OPERATION SUCCESSFUL : WINDOW CREATION \n");
	return 1;
};

int parser(int argc , char *args[] , int *h , int *w , char name[]){
	printf("PARSING THE COMMANDS PASSSED \n");
	for(int i = 0 ;  i<argc ; i++){
		if(strcmp(args[i],"--h") == 0){
			*h = (int)strtol(args[i+1],NULL,10);
			printf("height saved \n");
		}else if(strcmp(args[i],"--w") == 0){
			*w = (int)strtol(args[i+1],NULL,10);
			printf("width saved \n");
		}else if(strcmp(args[i],"--name") == 0){
			strcpy(name,args[i+1]);
			printf("name saved \n");
		};
	};

	printf("OPERATION SUCCESSFULL : COMMAND PARSED \n");
	return 1;
};

int main(int argc , char *args[]){
	SDL_Window *window ;
	SDL_Renderer *renderer ;
	SDL_Texture *texture ;
	int height , width ;
	char name[100];
	struct camera_info camera;
	struct sphere_info sphere ;
	uint32_t *framebuffer ;
	SDL_Event event ;
	if(parser(argc,args,&height,&width,name) <0){
		printf("OPERATION FAILED : PARSER FAILED \n");
		return -1;
	};

	framebuffer = (uint32_t *)malloc(height * width * sizeof(uint32_t));

	if(window_creation(&window,height,width,name) <0){
		printf("OPERATION FAILED : WINDOW CREATION FAILED \n");
		return -1;
	};

	if(renderer_creation(&renderer,window) <0){
		printf("OPERATION FAILED : RENDERER CREATION FAILED \n");
		return -1;
	};

	if(camera_creator(&camera) <0){
		printf("OPERATION FAILED : CAMERA CREATION FAILED \n");
		return -1;
	};

	if(sphere_creator(&sphere) <0){
		printf("OPERATION FAILED : SPHERE CREATION FAILED \n");
		return -1;
	};

	texture = NULL ;

	if(event_handler(&event , &camera , sphere , height , width , &texture , framebuffer , renderer,window) <0){
		printf("OPERATION FAILED : EVENT CREATION FAILED \n");
		return -1;
	};

	printf("CLOSING THE WINDOW \n");

	SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

	printf("SYSTEM CLOSED \n");
	sleep(5);

	return 0;
};
