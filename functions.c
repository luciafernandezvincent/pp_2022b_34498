
#include "functions.h"


#define CANT_TIPOS 5

#define EXIT_SUCCES 0
#define EXIT_FAILURE 1


typedef enum {
    ARG_HELP_SHORT,
    ARG_HELP_LONG,
    ARG_WIDTH_SHORT,
    ARG_WIDTH_LONG,
    ARG_HEIGHT_SHORT,
    ARG_HEIGHT_LONG,
    ARG_SEED_SHORT,
    ARG_SEED_LONG,
    ARG_N_RANGE
} arg_t;


static const char *valid_args[] = {
    [ARG_HELP_SHORT] = "-h",
    [ARG_HELP_LONG] = "--help",
    [ARG_WIDTH_SHORT] = "-w",
    [ARG_WIDTH_LONG] = "--width",
    [ARG_HEIGHT_SHORT] = "-H",
    [ARG_HEIGHT_LONG] = "--height",
    [ARG_SEED_SHORT] = "-s",
    [ARG_SEED_LONG] = "-seed",
    [ARG_N_RANGE] = "-n"


};

status_t parse_arguments(int argc, char *argv[], char *help, int *width, int *height, int *seed, int *n)
{
    char *pend = NULL;
    double _width, _height, _seed,  _num;
    bool width_processed, height_processed, seed_processed, num_processed = false;

    if (argc > 10) {
        return ST_ERR_NULL;
    }

    if ((NULL == argv) || (NULL == width ) || (NULL == height)|| (NULL == seed)|| (NULL == n)) {
        return ST_ERR_NULL;
    }

    for (int i = 1; i < argc; i++) {
            
                if (strcmp(argv[i],(valid_args[0])) ==0|| strcmp( argv[i], (valid_args[1]))==0){
                    puts("--help, -h: provides you help and ends the program.\n");
                    puts("--width, -w: amount of columns for the matrix.\n");
                    puts("--height, -H: amount of rows for the matrix.\n");
                    puts("--seed, -s: seed for the random system generator.\n");
                    puts("-n: it indicates the amount of rounds you want to execute between the first and the last print.\n");
                        
                    
                }
                
                else if (strcmp(argv[i],(valid_args[2])) ==0|| strcmp( argv[i], (valid_args[3]))==0){
                    i++;
                    _width = strtod(argv[i], &pend);
                    if ('\0' != *pend) {
                        return ST_ERR_NULL;
                    }
                    width_processed = true;
                    
                }
                else if (strcmp(argv[i],(valid_args[4]))==0 || strcmp( argv[i], (valid_args[5]))==0){
                    i++;
                    _height = strtod(argv[i], &pend);
                    if ('\0' != *pend) {
                        return ST_ERR_NULL;
                    }
                    height_processed = true;
                    
                }
                else if(strcmp(argv[i],(valid_args[6]))==0 || strcmp( argv[i], (valid_args[7]))==0){
                    i++;
                    _seed = strtod(argv[i], &pend);
                    if ('\0' != *pend) {
                        return ST_ERR_NULL;
                    }
                    seed_processed = true;
                    

                }
                else if (strcmp(argv[i],(valid_args[8]))==0){
                    i++;
                    _num = strtod(argv[i], &pend);
                    if ('\0' != *pend) {
                        return ST_ERR_NULL;
                    }
                    bool num_processed = true;
                    
                }


            }

        *width = _width;
        *height = _height;
        *seed = _seed;
        *n = _num;
        if (_width  == 0||_height == 0){
            return ST_ERR_NULL;
        }
    return ST_OK;
            
}


const double init_hp = 10.0;



double damage[][6] = {{1,1,1,1,1,0},
{1,0.5,2,1,0.5,0},
{1,0.5,0.5,2,2,0}, 
{1,1,1,0.5,1,0}, 
{1,2,0.5,0.5,0.5,0},
};


pokemon **matCreate( size_t rows, size_t cols ) {
    pokemon **m = malloc(sizeof(int *) * rows);

    if (NULL == m) {
        return NULL;
    }
    for (size_t i = 0; i < rows; i++) {
        m[i] = malloc(cols * sizeof(pokemon));
        if (NULL == m[i]) {
            while (i--) {
                free(m[i]);
            }
            free(m);
            return NULL;
        }
    }

    return m;
}




void matOG( pokemon ***mat, size_t rows, size_t cols, int seed) {
    *mat = matCreate( rows+2, cols+2 );
    srand (seed);
    
    for ( size_t r = 0; r < rows+2; r++ ){ 
        for ( size_t c = 0; c < cols+2; c++ ){ 
            pokemon pok;
            
            pok.hp = init_hp;
            pok.type = rand()%CANT_TIPOS;
            
            (*mat)[r][c]= pok;
            
            if ((r == 0) || (c == 0) || (r == rows+1)|| (c == cols+1) )
            {
                (*mat)[r][c].type = -1; 
            }
        } 
    }
    
}



void matFuture( pokemon ***newmat,  size_t rows, size_t cols, pokemon ***mat)
{
    *newmat = matCreate( rows+2, cols+2 );
    
    for ( size_t r = 0; r < rows+2; r++ ){ 
        for ( size_t c = 0; c < cols+2; c++ ){ 
            
            (*newmat)[r][c] = (*mat)[r][c];
            
            }
    }  
   
}




void vecinos(int x, int y, pokemon ***mat, pokemon ***mat2, size_t rows, size_t cols){

    for (int r = -1; r<= 1; r++){
        for ( int c = -1; c <= 1; c++ ){
            if (r!=0 || c!=0 ){

                (*mat2)[x][y].hp -= damage[(*mat)[x][y].type][(*mat)[x+r][y+c].type];
                
                if ((*mat2)[x][y].hp <= 0){

                    (*mat2)[x][y].type = (*mat)[x+r][y+c].type;
                    (*mat2)[x][y].hp = init_hp;

                }
                
            }
            
        }
    }
}
 



void damage2(pokemon ***mat,pokemon ***mat2, size_t rows, size_t cols){
    for ( size_t r = 1; r < rows+1; r++ ){ 
        for ( size_t c = 1; c < cols+1; c++ ){     
            vecinos(r,c, &(*mat),&(*mat2), rows+2,cols+2);
        } 
    }
}

char *colors[] = {"255 255 255","255 0 0", "0 0 255","255 255 0", "0 128 0"};

void print_ppm(pokemon ***mat, size_t rows, size_t cols, char *filename){
    
    FILE *filepointer;
    filepointer = fopen(filename, "wb");
    fprintf(filepointer, "P3\n  %ld %ld\n 255\n", rows, cols);

    for (size_t i=1; i<rows; i++){
        for (size_t j = 1; j<cols; j++){
            printf("%s", (colors[(*mat)[i][j].type]));
            
            fprintf(filepointer, "%s ", (colors[(*mat)[i][j].type]));
        }
    }
}

void n_ppm(pokemon ***mat,pokemon ***mat2, size_t rows, size_t cols, int n, int r){
    int prints = 1;
    while (prints<=(n)){
        damage2(&(*mat),&(*mat2),rows,cols);
        if ((prints == 1) || (prints == n))
            {
                char filename[20];
                sprintf(filename, "%i.ppm", prints);
                print_ppm(&(*mat2),rows,cols,filename);
            }
        prints++;

    }
}

void freeMat(pokemon ***mat, size_t rows, size_t cols) {
    
        for (size_t i = 0; i < rows; ++i) {
                free((*mat)[i]);
                (*mat)[i] = NULL;
            }
        
        free(*mat);
        *mat = NULL;
}



int game(int argc, char *argv[]){

    pokemon **mat1;
    pokemon **mat2;

    int cols = 1;
    int rows = 1;
    int seed;
    int n;
    
    
    char *help1 ;
    int *height1 = &rows;
    int *width1 = &cols;
    int *seed1 = &seed;
    int *n1 = &n;
    
    
    parse_arguments(argc, argv, help1, width1, height1, seed1, n1);

    matOG(&mat1, rows, cols, seed);

    matFuture(&mat2, rows, cols, &mat1);

    n_ppm(&mat1,&mat2, rows, cols, n,2);
    
    freeMat(&mat1,rows,cols);
    freeMat(&mat2, rows, cols);

    


}