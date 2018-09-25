#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h> // for close, read, write, sleep
#include <fcntl.h>  // for open
#include <stdlib.h> // malloc
#include <string.h>
#include <sys/stat.h> //stat
#include <pthread.h>

int fdl = 0;
int fdd = 0;
unsigned char* pixels;
unsigned char* disparity;
unsigned char* alpha;
float* vertices;
float* colors;
int* indices;

float focal;
float bline;

// float pz = focal * bline / d;
// float px = (current_col - center_col) * pz / focal;
// float py = (current_row - center_row) * pz / focal;

float* setVertices( int width, int height ) {
    int i = 0, j=0;

    float center_row = (float) height/2.0;
    float center_col = (float) width/2.0;

    for ( int row=0; row<height; row++ ) {
        for ( int col=0; col<width; col++ ) {
            float pz;
            if (disparity[j]==0){
                pz = (focal * bline);
            }
            else{
                pz = (focal * bline) / (float)disparity[j];
            }
            j++;
            vertices[i++] = (((float)col)-center_col)*(pz/focal);
            vertices[i++] = (((float)row)-center_row)*(pz/focal);
            vertices[i++] = (float) pz;
        }
    }
}

float* setColors( int width, int height) {
    int i = 0;
    int j = 0;
    for ( int row=0; row<height; row++ )
        for ( int col=0; col<width; col++ ) {
            //gray scale
            colors[i] = (float)pixels[j]/255.0;
            colors[i+1] = (float)pixels[j]/255.0;
            colors[i+2] = (float)pixels[j]/255.0;
            if (disparity[j]==0 || alpha[j]==1.0){
                colors[i+3] = 0.0;
            }
            else{
                colors[i+3] = 1.0;
            }
            i+=4; j++;
        }
}

int* setIndices( int width, int height ) {
    int i = 0;
    for ( int row=0; row<height-1; row++ ) {
        if ( (row&1)==0 ) { // even rows
            for ( int col=0; col<width; col++ ) {
                indices[i++] = col + row * width;
                indices[i++] = col + (row+1) * width;
            }
        } else { // odd rows
            for ( int col=width-1; col>0; col-- ) {
                indices[i++] = col + (row+1) * width;
                indices[i++] = col - 1 + + row * width;
            }
        }
    }
}

int* setTransparencies(int width, int height ) {
    int i = width;
    int threshold = 1;
    for ( int row=1; row<height-1; row++ ) {
        for (int col=0; col<width; col++ ) {
            if(
                disparity[i]-disparity[i+1]      >threshold || 
                disparity[i]-disparity[i-1]      >threshold || 
                disparity[i]-disparity[i+width]  >threshold || 
                disparity[i]-disparity[i-width]  >threshold || 
                disparity[i]-disparity[i+width-1]>threshold || 
                disparity[i]-disparity[i-width+1]>threshold ||
                disparity[i]-disparity[i+width+1]>threshold || 
                disparity[i]-disparity[i-width-1]>threshold
            ){
                alpha[i]         = 1;
            }
            i++;
        }
    }
}

void file_checker(){
    struct stat stat_record;
    const char* left_file = "/tmp/mem_mapped_left";//"im3.pgm";
    const char* rdisp_file = "/tmp/mem_mapped_rdisp";//"im3_dmap.pgm";

    while (stat(left_file, &stat_record)<0 || stat_record.st_size==0)
        sleep(1);

    fdl = open(left_file, O_RDONLY, 0700);
    if (mmap(0, 1280*720+12, PROT_READ|PROT_EXEC, MAP_SHARED, fdl, 0) == MAP_FAILED)
    {
        close(fdl);
        perror("Error mmapping left file");
        return;
    }

    while (stat(rdisp_file, &stat_record)<0 || stat_record.st_size==0)
        sleep(1);

    fdd = open(rdisp_file, O_RDONLY, 0700);
    if (mmap(0, 1280*720+20, PROT_READ|PROT_EXEC, MAP_SHARED, fdd, 0) == MAP_FAILED)
    {
        close(fdd);
        perror("Error mmapping rdisp file");
        return;
    }
    return;
}

void setup_mapping(int max_width, int max_height){

    disparity = (unsigned char*) malloc(max_width*max_height);
    alpha = (unsigned char*) malloc(max_width*max_height);
    pixels = (unsigned char*) malloc(max_width*max_height);

    memset(disparity, 0, max_width*max_height);
    memset(alpha, 0, max_width*max_height);
    memset(pixels, 0, max_width*max_height);

    // float calib[9];
    // FILE* f =fopen(calib_path, "r");
    // fread((void*)(calib), sizeof(calib[0]), 9, f);

    //focal = 1747.72/2.0;//calib[0];
    //bline = 0.035;

    pthread_t x_thread;

    /* create a second thread which executes inc_x(&x) */
    if(pthread_create(&x_thread, NULL, (void * (*)(void *))file_checker, NULL)) {
        fprintf(stderr, "Can't start thread for file checking\n");
        return;
    }

}


int get_vertex_array(float* vertices_a, float* colors_a, int* indices_a) {

    int width, height;
    float fps;
    if(fdl && fdd){
        read(fdl, &width, 4);
        read(fdl, &height, 4);
        read(fdl, &fps, 4);
        read(fdl, pixels, width*height);
        lseek(fdl, 0, SEEK_SET);
        read(fdd, &focal, 4);
        read(fdd, &bline, 4);
        if(width==720)
            focal = 1200.0;
        else
            focal = 600.0;
        bline = 0.035;
        read(fdd, &width, 4);
        read(fdd, &height, 4);
        read(fdd, &fps, 4);
        read(fdd, disparity, width*height);
        lseek(fdd, 0, SEEK_SET);
    }else{
        return 0;
    }

    vertices = vertices_a;
    colors = colors_a;
    indices = indices_a;

    setVertices(width,height);
    memset(alpha, 0, width*height);
    setTransparencies(width,height);
    setColors(width, height);
    setIndices(width,height);
    return (width*height) + (width-1)*(height-2) -1;

}
