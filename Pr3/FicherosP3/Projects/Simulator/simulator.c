#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define EN_RUTA 0 // autobus en ruta
#define EN_PARADA 1 // autobus en la parada

int n_usuarios, cap_bus, n_paradas;

int estado = EN_RUTA; // estado inicial
int parada_actual = 0;// parada en la que se encuentra el autobus
int n_ocupantes = 0; // ocupantes que tiene el autobus

int * esperando_subir  = NULL; 	// personas que desean subir en cada parada
int * esperando_bajar = NULL;	// personas que desean bajar en cada parada

pthread_mutex_t m;
pthread_cond_t bus_esperando,usuarios_esperando;

void Autobus_En_Parada() {  

     /* Ajustar el estado y bloquear al autobus hasta que no haya pasajeros que
    quieran bajar y/o subir la parada actual. Despues se pone en marcha */

    pthread_mutex_lock(&m);

    estado =  EN_PARADA;    

    while((esperando_bajar[parada_actual] > 0) || (esperando_subir[parada_actual] > 0 && n_ocupantes < cap_bus)){// NO puede esperar a que suban si el bus esta lleno
        pthread_cond_broadcast(&usuarios_esperando);
        pthread_cond_wait(&bus_esperando, &m);
    }      

    pthread_mutex_unlock(&m);
   
}
void Conducir_Hasta_Siguiente_Parada() {
    /* Establecer un Retardo que simule el trayecto y actualizar numero de parada */
    
    pthread_mutex_lock(&m);
    estado = EN_RUTA;
    pthread_mutex_unlock(&m);

	printf("Bus iniciando la conduccion desde %d \n",parada_actual);
	sleep(random() % 5);

    pthread_mutex_lock(&m);
	parada_actual = ( parada_actual + 1 ) % n_paradas ;
	pthread_mutex_unlock(&m);

    printf("Bus ha llegado al destino %d \n",parada_actual);


}
void * thread_autobus(void * args) {
    while (1) { 
        Autobus_En_Parada();  // esperar a que los viajeros suban y bajen
        Conducir_Hasta_Siguiente_Parada();  // conducir hasta siguiente parada
    }
}
void Subir_Autobus(int id_usuario, int origen) {
    /* El usuario indicara que quiere subir en la parada ’origen’, esperara a que
    el autobus se pare en dicha parada y subira. El id_usuario puede utilizarse para
    proporcionar informacion de depuracion */
    
    pthread_mutex_lock(&m);

    esperando_subir[origen] +=1;

    while(estado == EN_RUTA || n_ocupantes == cap_bus || parada_actual != origen){
        pthread_cond_wait(&usuarios_esperando, &m);
    }

    esperando_subir[origen] -=1;
    n_ocupantes++;
    printf("%d se esta subiendo al autobus. \n",id_usuario);

    pthread_cond_signal(&bus_esperando);

    pthread_mutex_unlock(&m);

    printf("%d se ha subido al autobus \n",id_usuario);


}
void Bajar_Autobus(int id_usuario, int destino) {
    /* El usuario indicara que quiere bajar en la parada ’destino’, esperara a que
    el autobus se pare en dicha parada y bajara. El id_usuario puede utilizarse para
    proporcionar informacion de depuracion */

    printf("%d quiere bajarse del autobus en %d \n",id_usuario,destino);
    
    pthread_mutex_lock(&m);

    esperando_bajar[destino] +=1;

    while(estado == EN_RUTA || parada_actual != destino){
        pthread_cond_wait(&usuarios_esperando, &m);
    }

    esperando_bajar[destino] -=1;
    n_ocupantes--;

    pthread_cond_signal(&bus_esperando);
    pthread_cond_broadcast(&usuarios_esperando); // Puede haber alguno que no haya subido por estar lleno

    pthread_mutex_unlock(&m);

    printf("%d se ha bajado del autobus \n",id_usuario);

}
void Usuario(int id_usuario, int origen, int destino) {
    
    printf("%d solicia ir desde %d hasta %d \n",id_usuario,origen,destino);
    Subir_Autobus(id_usuario, origen);  // Esperar a que el autobus este en parada origen para subir
    Bajar_Autobus(id_usuario, destino);  // Bajarme en estacion destino
}
void * thread_usuario(void * arg) {
    int id_usuario = (int) arg;
	int a,b;

    while (1) { 
        a = rand() % n_paradas;
        do {
            b = rand() % n_paradas;
        } while (a == b);

        Usuario(id_usuario, a, b);
    }
}
int main(int argc, char * argv[]) {
	pthread_t autobus;
	pthread_t * usuarios = NULL;

	if (argc!=4){
		fprintf(stderr, "Usage: %s <capacidad_autobus> <numero_usuarios> <numero_paradas>\n", argv[0]);
		exit(-1);
	}
    
	cap_bus = atoi(argv[1]);
    n_usuarios = atoi(argv[2]);
	n_paradas = atoi(argv[3]);

    if( cap_bus < 0 || n_usuarios < 0 || n_paradas < 0){
        fprintf(stderr, "No es posible tener numeros negativos ¿¿¿???", argv[0]);
		exit(-1);
    }

    pthread_mutex_init(&m,NULL);
    pthread_cond_init(&bus_esperando,NULL);
    pthread_cond_init(&usuarios_esperando,NULL);

	esperando_subir  = malloc(n_paradas * sizeof(int)); //= {0,0,...0};
	esperando_bajar = malloc(n_paradas * sizeof(int)); //= {0,0,...0};

	usuarios = malloc(sizeof(pthread_t) * n_usuarios);

    printf("\n##### Iniciando el recorrido de %d paradas con una capacidad de %d asientos y %d usuarios \n\n",n_paradas,cap_bus,n_usuarios);
	pthread_create(&autobus, NULL, thread_autobus, NULL);

    for(int i = 0; i< n_usuarios; i++){
        pthread_create(&usuarios[i], NULL, thread_usuario, (void*)i); 
    } 
    
	for(int i = 0; i< n_usuarios; i++){
        pthread_join(usuarios[i],NULL);  
    }

	pthread_join(autobus,NULL);

    pthread_mutex_destroy(&m);
    pthread_cond_destroy(&usuarios_esperando);
    pthread_cond_destroy(&bus_esperando);


	return 0;

}