#include <stdlib.h>
#include <stdio.h>

#define EN_RUTA 0 // autobus en ruta
#define EN_PARADA 1 // autobus en la parada

int n_usuarios, cap_bus, n_paradas;

int estado = EN_RUTA; // estado inicial
int parada_actual = 0;// parada en la que se encuentra el autobus
int n_ocupantes = 0; // ocupantes que tiene el autobus

int * esperando_parada  = NULL; 	// personas que desean subir en cada parada
int * esperando_bajar = NULL;	// personas que desean bajar en cada parada

void * thread_autobus(void * args) {
    while (  1) { /*condicion*/
        // esperar a que los viajeros suban y bajen
        Autobus_En_Parada();
        // conducir hasta siguiente parada
        Conducir_Hasta_Siguiente_Parada();
    }
}
void * thread_usuario(void * arg) {
    int id_usuario = (int) arg;
	int a,b;

    while (1) { /*condicion*/
        a = rand() % n_paradas;
        do {
            b = rand() % n_paradas;
        } while (a == b);

        Usuario(id_usuario, a, b);
    }
}
void Usuario(int id_usuario, int origen, int destino) {
    // Esperar a que el autobus este en parada origen para subir
    Subir_Autobus(id_usuario, origen);
    // Bajarme en estacion destino
    Bajar_Autobus(id_usuario, destino);
}

void Autobus_En_Parada() {
    /* Ajustar el estado y bloquear al autobus hasta que no haya pasajeros que
    quieran bajar y/o subir la parada actual. Despues se pone en marcha */
}
void Conducir_Hasta_Siguiente_Parada() {
    /* Establecer un Retardo que simule el trayecto y actualizar numero de parada */
	printf("Iniciando la conduccion");
	sleep(random() % 5);
	printf("Terminando la conduccion");
	parada_actual = ( parada_actual +1) % n_paradas ;

}
void Subir_Autobus(int id_usuario, int origen) {
    /* El usuario indicara que quiere subir en la parada ’origen’, esperara a que
    el autobus se pare en dicha parada y subira. El id_usuario puede utilizarse para
    proporcionar informacion de depuracion */
}
void Bajar_Autobus(int id_usuario, int destino) {
    /* El usuario indicara que quiere bajar en la parada ’destino’, esperara a que
    el autobus se pare en dicha parada y bajara. El id_usuario puede utilizarse para
    proporcionar informacion de depuracion */
}
int main(int argc, char * argv[]) {
	pthread_t autobus;
	pthread_t * usuarios = NULL;

	if (argc!=4){
		fprintf(stderr, "Usage: %s <capacidad_autobus> <numero_usuarios> <numero_paradas>\n", argv[0]);
		exit(1);
	}

	cap_bus = atoi(argv[1]);
	n_usuarios = atoi(argv[2]);
	n_paradas = atoi(argv[3]);

	esperando_parada  = malloc(n_paradas * sizeof(int)); //= {0,0,...0};
	esperando_bajar = malloc(n_paradas * sizeof(int)); //= {0,0,...0};


	usuarios = malloc(sizeof(pthread_t) * n_usuarios);

	pthread_create(&autobus, NULL, thread_autobus, NULL);


    for (int i = 0; i< n_usuarios; i++) 
        pthread_create(&usuarios[i], NULL, thread_usuario, (void*)i);  
    
	for (int i = 0; i< n_usuarios; i++) 
        pthread_join(&usuarios[i]);  


	pthread_join(&autobus);


	return 0;

}