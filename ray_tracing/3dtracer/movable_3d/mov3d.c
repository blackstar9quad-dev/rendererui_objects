#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

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
	struct vec3 light_origin;
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

struct vec3 vec3dot(struct vec3 vector1 , struct vec3 vector2){
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
	struct vec3  Vn,N,Q,P;

	P.x = (camera.origin.x) + (directions.x * t);
	P.y = (camera.origin.y) + (directions.y * t);
	P.z = (camera.origin.z) + (directions.z * t) ;

	Vn = vec3sub(sphere.origin,P);

	N = vec3div(Vn,sphere.radius);

	Q = camera.light_origin ;

	L = vec3sub(P,Q);

	double magnitude = sqrt((L.x * L.x) + (L.y * L.y) + (L.z + L.z));
	L = vec3div(L,magnitude);

	double dot_product = vec3dot(N,L);

	I = (1) * ((max(0,dot_product)) / (magnitude * magnitude));
	
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

	t = max(t1,t2);

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

	result.x = ((x/w) - 0.5) * camera.viewpointwidth ;
	result.y = (0.5 - (y/h)) * camera.viewpointheight;
	result.z = camera.viewpointlength ;

	return result ;
};

int initlizor(uint32_t *framebuffer ,struct sphere_info sphere , struct camera_info camera , int h , int w ){
	struct vec3 viewpoints , direction , eq_points ;
	double t , I ;
	for(int y= 0 ; y<h ; y++){
		for(int x=0 ; x<w ; x++){
			viewpoints = viewpoint_generator(x,y,camera,h,w);
			directions = direction_generator(viewpoints,camera);
			eq_points  = point_generator(directions , camera , sphere);

			t = quad_eq(eq_points);

			if(t >= 0){
				I = intenstity(x,y,t,camera,sphere);
				framebuffer[(y*w)+x] = LIGHT_CLR * I ;
			}else if(t<0){
				framebuffer[(y*w)+x] = 0x00000000 ;
			};
		};
	};

	return 1;
};

int event_handler(){
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
	struct circle_info circle;
	struct sphere_info sphere ;
	uint32_t *framebuffer ;
	SDL_Event event ;
	if(parser(argc,args,&height,&width,name) <0){
		printf("OPERATION FAILED : PARSER FAILED \n");
		return -1;
	};

	if(window_creator(&window,height,width,name) <0){
		printf("OPERATION FAILED : WINDOW CREATION FAILED \n");
		return -1;
	};

	if(renderer_creator(&renderer,window) <0){
		printf("OPERATION FAILED : RENDERER CREATION FAILED \n");
		return -1;
	};

	if(camera_creator(&camera) <0){
		printf("OPERATION FAILED : CAMERA CREATION FAILED \n");
		return -1
	};

	if(sphere_creator(&sphere) <0){
		printf("OPERATION FAILED : SPHERE CREATION FAILED \n");
		return -1;
	};
};
