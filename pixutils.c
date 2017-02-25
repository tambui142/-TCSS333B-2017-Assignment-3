#include <stdlib.h>
#include "pixutils.h"


//private methods -> make static
static pixMap* pixMap_init(unsigned char arrayType);
static pixMap* pixMap_copy(pixMap *p);


static pixMap* pixMap_init(unsigned char arrayType){
	//initialize everything to zero except arrayType
	pixMap *p=(pixMap*)malloc(sizeof(pixMap));
	p->image = 0;
	p->imageWidth = 0;
	p->imageHeight = 0;
	p->arrayType = arrayType;
	p->pixArray_arrays = 0;
	p->pixArray_blocks = 0;
	p->pixArray_overlay = 0;
	return p;
}	

void pixMap_destroy (pixMap **p){
 //free all mallocs and put a zero pointer in *p
	if(p && (**p).image) {
		(**p).image=0;
		free((**p).image);	
	}
	if(p && (**p).pixArray_arrays) {
		(**p).pixArray_arrays=0;
		free((**p).pixArray_arrays);
	}
	if(p && (**p).pixArray_blocks) {
		(**p).pixArray_blocks=0;
		free((**p).pixArray_blocks);
	}
	if(p && (**p).pixArray_overlay) {
		(**p).pixArray_overlay=0;
		free((**p).pixArray_overlay);
	}
	
	if(p) free(p);
}
	
pixMap *pixMap_read(char *filename,unsigned char arrayType){
 //library call reads in the image into p->image and sets the width and height
 pixMap *p=pixMap_init(arrayType);
 int error;
 if((error=lodepng_decode32_file(&(p->image), &(p->imageWidth), &(p->imageHeight),filename))){
  fprintf(stderr,"error %u: %s\n", error, lodepng_error_text(error));
  return 0;
	}
 //allocate the 2-D rgba arrays

 
	if (arrayType ==0){
  //an only allocate for the number of rows - each row will be an array of MAXWIDTH
/*  int (*a) [MAXWIDTH]=malloc(MAXWIDTH*sizeof(void*));
  for(int i=0; i<MAXWIDTH; i++) {
		memcpy(a[i],p[i],MAXWIDTH*sizeof(int));
	}
*/	
	
	 /*p->pixArray_arrays[MAXWIDTH]=(rgba*)->image;
     for(int i =1; i < p->imageHeight;i++) {
	 p->pixArray_arrays[i]=p->pixArray_arrays[i-1]+1->imageWidth;
	 }*/
	 
	 //p->pixArray_arrays=malloc(p->imageHeight*sizeof(rgba*));
 }
  //copy each row of the image into each row
  
	else if (arrayType ==1){
		//allocate a block of memory (dynamic array of p->imageHeight) to store the pointers
		//use a loop allocate a block of memory for each row
/*	int n = 0;
	int (*a) [MAXWIDTH]=malloc(imageWidth*sizeof(int[MAXWIDTH]));
	for(int i =0; i < imageWidth; i++) {
		for(int j=0, j< imageHeight; j++) {
			memcpy(&(a[i][j]),&(p[i][j]), imageWidth*imageHeight*sizeof(int));
		}
	}
	int **a=malloc(p->imageHeight*sizeof(void*);
	for(int i=0; i<p->imageHeight;i++) {
		a[i]= malloc(p->imageWidth*sizeof(int));
	}
*/		
  //copy each row of the image into the newly allocated block
//p->pixArray_blocks=malloc(p->imageHeight*sizeof(rgba*));
/*		int n=0;
		rgba** r2 = malloc(p->imageWidth*sizeof(rgba*));
		for(int i=0; i<p->imageHeight*4;i++) {
			r2[i] =  malloc(p->imageWidth);
			for(int j=0;j<p->imageWidth;j++) 
				//p->pixArray_blocks[i][j]=n++;
				r2[i][j]=&n++;
			}
*/
		rgba** r2 = malloc(p->imageHeight*sizeof(rgba*));
		r2[0] = (rgba*) p->image;
		for(int i =1; i < p->imageHeight; i++) {
			r2[i] = r2[i-1] + p->imageWidth;
		}
		p->pixArray_overlay = r2;
}		
	else if (arrayType =='2'){
  //allocate a block of memory (dynamic array of p->imageHeight) to store the pointers
  //set the first pointer to the start of p->image
  //each subsequent pointer is the previous pointer + p->imageWidth
		//int **a=malloc(imageHeight*imageWidth*sizeof(void*));
		//unsigned char *myptr;
		//myptr = &a;
		rgba** r2 = malloc(p->imageHeight*sizeof(rgba*));
		r2[0] = (rgba*) p->image;
		for(int i =1; i < p->imageHeight; i++) {
			r2[i] = r2[i-1] + p->imageWidth;
		}
		p->pixArray_overlay = r2;
	}
	else{
		return 0;
	}				
	return p;
}
int pixMap_write(pixMap *p,char *filename){
	int error=0;
	//for arrayType 0 and arrayType 1 have to write out a controws  to the image using memcpy
	 if (p->arrayType ==0){
   //have to copy each row of the array into the corresponding row of the image	
	 }	
		else if (p->arrayType ==1){
   //have to copy each row of the array into the corresponding row of the image		
	 }
	//library call to write the image out 
 if(lodepng_encode32_file(filename, p->image, p->imageWidth, p->imageHeight)){
  fprintf(stderr,"error %u: %s\n", error, lodepng_error_text(error));
  return 1;
	}
	return 0;
}	 

int pixMap_rotate(pixMap *p,float theta){
	pixMap *oldPixMap=pixMap_copy(p);
	if(!oldPixMap)return 1;
 const float ox=p->imageWidth/2.0f;
 const float oy=p->imageHeight/2.0f;
 const float s=sin(degreesToRadians(-theta));
	const float c=cos(degreesToRadians(-theta));
	for(int y=0;y<p->imageHeight;y++){
		for(int x=0;x<p->imageWidth;x++){ 
   float rotx = c*(x-ox) - s * (oy-y) + ox;
   float roty = -(s*(x-ox) + c * (oy-y) - oy);
		 int rotj=rotx+.5;
		 int roti=roty+.5; 
		 if(roti >=0 && roti < oldPixMap->imageHeight && rotj >=0 && rotj < oldPixMap->imageWidth){
			 if(p->arrayType==0) memcpy(p->pixArray_arrays[y]+x,oldPixMap->pixArray_arrays[roti]+rotj,sizeof(rgba));
			 else if(p->arrayType==1) memcpy(p->pixArray_blocks[y]+x,oldPixMap->pixArray_blocks[roti]+rotj,sizeof(rgba));
			 else if(p->arrayType=='2') memcpy(p->pixArray_overlay[y]+x,oldPixMap->pixArray_overlay[roti]+rotj,sizeof(rgba));			 
			}
			else{
				if(p->arrayType==0) memset(p->pixArray_arrays[y]+x,0,sizeof(rgba));
			 else if(p->arrayType==1) memset(p->pixArray_blocks[y]+x,0,sizeof(rgba));
			 else if(p->arrayType=='2') memset(p->pixArray_overlay[y]+x,0,sizeof(rgba));		
			}		
		}	
	}
 pixMap_destroy(&oldPixMap);
 return 0;
}

pixMap *pixMap_copy(pixMap *p){
	pixMap *new=pixMap_init(p->arrayType);
	//allocate memory for new image of the same size a p->image and copy the image
	//allocate memory and copy the arrays. 
	new->imageHeight = p->imageHeight;
	new->imageWidth = p->imageWidth;
	
	new->image = malloc(p->imageWidth * p->imageHeight * sizeof(rgba));
	
	
	//int r2 = sizeof(rgba) * p->imageWidth;
	
	new->image = malloc (p->imageHeight * p->imageWidth * sizeof(rgba));
	memcpy(new->image, p->image, p->imageHeight * p->imageWidth * sizeof(rgba));
	
	if (new->arrayType ==0){
	  for(int i= 0; i< p->imageWidth;i++) {
		  for(int j=0; j< p->imageHeight;j++) {
			  new->pixArray_arrays[i][j]=p->pixArray_arrays[i][j];
	  }
  }
}
//	memcpy(new->image,p->image, p->imageHeight*p->imageWidth*sizeof(rgba));
		
 else if (new->arrayType ==1){
	  for(int i= 0; i< p->imageWidth;i++) {
		  for(int j=0; j< p->imageHeight;j++) {
			  new->pixArray_blocks[i][j]=p->pixArray_blocks[i][j];
	  }
	}
}
	else if (new->arrayType =='2'){
	  printf("jjjjjjjjjjjjj\n");
	  new->pixArray_overlay = malloc(p->imageHeight * p->imageWidth * sizeof(rgba));
	  memcpy(new->pixArray_overlay, p->pixArray_overlay, p->imageHeight * p->imageWidth * sizeof(rgba));
	  
	  
	  /*
	  for(int i= 0; i< p->imageWidth;i++) {
		  for(int j=0; j< p->imageHeight;j++) {
			  new->pixArray_overlay[i][j]=p->pixArray_overlay[i][j];
	  }*/
	}
	return new;
}	
