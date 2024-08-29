#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

void filtrar_primos(int pipe_izquierdo[2]) {
    close(pipe_izquierdo[1]);

    int p;
    int r = read(pipe_izquierdo[0], &p, sizeof(p));
    if (r > 0) {
        printf("primo %d\n", p);
        
        int pipe_derecho[2]; 
        int pd = pipe(pipe_derecho); //creo un nuevo pipe (pipe derecho)
        if (pd < 0){
            perror("Error al crear pipe");
            exit(EXIT_FAILURE);
        }
        int f = fork(); //creo un nuevo proceso hijo (proceso hijo derecho)
        if (f < 0) {
            perror("Error en fork");
            exit(EXIT_FAILURE);
        }
        if (f == 0){
            //proceso hijo
            close(pipe_izquierdo[0]);
            filtrar_primos(pipe_derecho); //Recursivamente filtrar los números que no son multiplos de p
            exit(EXIT_SUCCESS);
        }
        else{
            //proceso padre
            close(pipe_derecho[0]); 
            int n;
            while (read(pipe_izquierdo[0], &n, sizeof(int)) > 0) { //filtro los no multiplos de p colocandolos en el pipe
                if (n % p != 0) {
                    write(pipe_derecho[1], &n, sizeof(n));
                }
            }
            close(pipe_derecho[1]); 
            close(pipe_izquierdo[0]); 
            wait(NULL);
        }

    }
}

int
main(int argc, char *argv[])
{
	// Deshabilitar el buffering de stdout
    setbuf(stdout, NULL);
	int parametro = atoi(argv[1]);
	int cant_num = parametro - 1;

    int pipe_derecho[2];
    int pd = pipe(pipe_derecho); 
    if (pd < 0) {
        perror("Error al crear pipe");
        exit(EXIT_FAILURE);
    }

    int j = 2;
    for (int i = 0; i < cant_num; i++) //escribimos en el pipe derecho la secuencia de numeros de 2 a n
	{	
        write(pipe_derecho[1],&j,sizeof(j));	
		j++;
	}
    close(pipe_derecho[1]);

    int fd = fork();
    if (fd < 0) {
        perror("Error en fork");
        exit(EXIT_FAILURE);
    }
    if (fd == 0){
        //proceso hijo (proceso derecho)
        filtrar_primos(pipe_derecho); 
        exit(EXIT_SUCCESS);
    }else{
        //proceso padre
        close(pipe_derecho[0]);
        wait(NULL); 
    }
    return 0;
}