/* primeiro projeto de IAED
 * autor: vmm
 */

/* 

O seguinte projeto foi utilizado por mim, Tomás Taborda nº103641;
sendo este ficheiro totalmente feito pelo autor vmm, sendo
este autor o dono deste projeto; neste segundo projeto, irei utilizar o seu projeto e
foram alteradas pequenas partes para corresponderem ao enunciado 
do segundo projeto.

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAX_NUM_AEROPORTOS 40	/* número máximo de areoportos */
#define MAX_NUM_VOOS 30000	/* número máximo de voos */

#define MAX_CODIGO_AEROPORTO 4	/* dimensão do código do aeroporto */
#define MAX_NOME_PAIS 31	/* dimensão do nome do pais */
#define MAX_NOME_CIDADE 51	/* dimensão do nome da cidade */

#define MAX_CODIGO_VOO 7	/* dimensão do código do voo */
#define MAX_DATA 11		/* dimensão da data */
#define MAX_HORA 6		/* dimensão da hora */

#define NAO_EXISTE -1		/* código de erro */

#define ANO_INICIO 2022		/* ano inicial do sistema */
#define DIAS_ANO 365		/* número de dias no ano */
#define HORAS_DIA 24		/* número de horas por dia */
#define MINUTOS_HORA 60		/* número de minutos numa hora */
#define MINUTOS_DIA 1440	/* número de minutos do dia */

#define TRUE 1			/* verdadeiro */
#define FALSE 0			/* falso */

/* Tipos de Dados */

typedef struct {
	char id[MAX_CODIGO_AEROPORTO];
	char pais[MAX_NOME_PAIS];
	char cidade[MAX_NOME_CIDADE];
	int numVoos;
} Aeroporto;

typedef struct {
	int dia;
	int mes;
	int ano;
} Data;

typedef struct {
	int hora;
	int minuto;
} Hora;

typedef struct link1{
	char *ReservaCODE;
	int sizeRcode;
    int numPassageiros;
    char VooCODE[MAX_CODIGO_VOO];
    Data data;
	struct link1 *next;     
} linkedlistReservasGERAL;     

linkedlistReservasGERAL *first=NULL;

typedef struct {
	char id[MAX_CODIGO_VOO];
	char partida[MAX_CODIGO_AEROPORTO];
	char chegada[MAX_CODIGO_AEROPORTO];
	Data data;
	Hora hora;
	Hora duracao;
	int capacidade;
	int horaPartida;
	int horaChegada;
} Voo;

/* Variaveis Globais */

int _numAeroportos = 0;		/* número de aeroportos introduzidos */
Aeroporto _aeroportos[MAX_NUM_AEROPORTOS];	/* vetor de aeroportos */

int _numVoos = 0;		/* número de voos introduzidos */
Voo _voos[MAX_NUM_VOOS];	/* vetor de voos */

Data _hoje = { 1, 1, 2022 };	/* data atual do sistema */

const int _diasMesAc[] =	/* dias acumulados por mês (jan=1) */
{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

/* Funcoes Leitura */

Hora leHora() {
	Hora h;
	scanf("%d:%d", &h.hora, &h.minuto);
	return h;
}


Data leData() {
	Data d;
	scanf("%d-%d-%d", &d.dia, &d.mes, &d.ano);
	return d;
}


int leProximaPalavra(char str[]) {
	char c = getchar();
	int i = 0;
	while (c == ' ' || c == '\t')
		c = getchar();
	while (c != ' ' && c != '\t' && c != '\n') {
		str[i++] = c;
		c = getchar();
	}
	str[i] = '\0';
	return (c == '\n');
}


void lePalavraAteFimDeLinha(char str[]) {
	char c = getchar();
	int i = 0;
	while (c == ' ' || c == '\t')
		c = getchar();
	while (c != '\n') {
		str[i++] = c;
		c = getchar();
	}
	str[i] = '\0';
}


void leAteFimDeLinha() {
	char c = getchar();
	while (c != '\n')
		c = getchar();
}


/* Funcoes Datas e Horas */

void mostraData(Data d) {
	if (d.dia < 10)
		printf("0");
	printf("%d-", d.dia);
	if (d.mes < 10)
		printf("0");
	printf("%d-%d", d.mes, d.ano);
}


void mostraHora(Hora h) {
	if (h.hora < 10)
		printf("0");
	printf("%d:", h.hora);
	if (h.minuto < 10)
		printf("0");
	printf("%d", h.minuto);
}


int converteDataNum(Data d) {
	return (d.ano - ANO_INICIO) * DIAS_ANO + _diasMesAc[d.mes - 1] +
		d.dia - 1;
}


int converteHoraNum(Hora h) {
	return ((h.hora * MINUTOS_HORA) + h.minuto);
}


int converteDataHoraNum(Data d, Hora h) {
	return converteDataNum(d) * MINUTOS_DIA + converteHoraNum(h);
}


Hora converteNumHora(int num) {
	Hora h;
	h.minuto = num % MINUTOS_HORA;
	h.hora = ((num - h.minuto) / MINUTOS_HORA) % HORAS_DIA;
	return h;
}


Data converteNumData(int num) {
	Data d;
	int i = 0;
	num = (num - (num % MINUTOS_DIA)) / MINUTOS_DIA;
	d.ano = (num / DIAS_ANO) + ANO_INICIO;
	num = num - ((d.ano - ANO_INICIO) * DIAS_ANO);
	while (i <= 11 && num >= _diasMesAc[i])
		i++;
	d.mes = i;
	d.dia = num - _diasMesAc[i - 1] + 1;
	return d;
}


int validaData(Data d) {
	int numData = converteDataNum(d);
	Data proximoAno = _hoje;
	proximoAno.ano++;
	return !(numData < converteDataNum(_hoje)
		 || numData > converteDataNum(proximoAno));
}


int validaHora(Hora h) {
	return !(h.hora > 12 || (h.hora == 12 && h.minuto > 0));
}


/* Algoritmo de ordenação BubbleSort */

void bubbleSort(int indexes[], int size, int (*cmpFunc)(int a, int b)) {
	int i, j, done;

	for (i = 0; i < size - 1; i++) {
		done = 1;
		for (j = size - 1; j > i; j--)
			if ((*cmpFunc) (indexes[j - 1], indexes[j])) {
				int aux = indexes[j];
				indexes[j] = indexes[j - 1];
				indexes[j - 1] = aux;
				done = 0;
			}
		if (done)
			break;
	}
}


/* Funcoes Aeroportos */


int aeroportoInvalido(char id[]) {
	int i;
	for (i = 0; id[i] != '\0'; i++)
		if (!(id[i] >= 'A' && id[i] <= 'Z'))
			return TRUE;
	return FALSE;
}


int encontraAeroporto(char id[]) {
	int i;
	for (i = 0; i < _numAeroportos; i++)
		if (!strcmp(id, _aeroportos[i].id))
			return i;
	return NAO_EXISTE;
}


void adicionaAeroporto() {
	Aeroporto a;

	leProximaPalavra(a.id);
	leProximaPalavra(a.pais);
	lePalavraAteFimDeLinha(a.cidade);

	if (aeroportoInvalido(a.id))
		printf("invalid airport ID\n");
	else if (_numAeroportos == MAX_NUM_AEROPORTOS)
		printf("too many airports\n");
	else if (encontraAeroporto(a.id) != NAO_EXISTE)
		printf("duplicate airport\n");
	else {
		strcpy(_aeroportos[_numAeroportos].id, a.id);
		strcpy(_aeroportos[_numAeroportos].pais, a.pais);
		strcpy(_aeroportos[_numAeroportos].cidade, a.cidade);
		_aeroportos[_numAeroportos].numVoos = 0;
		_numAeroportos++;
		printf("airport %s\n", a.id);
	}
}


void mostraAeroporto(int index) {
	printf("%s %s %s %d\n", _aeroportos[index].id,
		_aeroportos[index].cidade, _aeroportos[index].pais,
		_aeroportos[index].numVoos);
}


int cmpAeroportos(int a, int b) {
	return (strcmp(_aeroportos[a].id, _aeroportos[b].id) > 0);
}


void listaTodosAeroportos() {
	int i;
	int indexAeroportos[MAX_NUM_AEROPORTOS];

	for (i = 0; i < _numAeroportos; i++)
		indexAeroportos[i] = i;

	bubbleSort(indexAeroportos, _numAeroportos, cmpAeroportos);

	for (i = 0; i < _numAeroportos; i++)
		mostraAeroporto(indexAeroportos[i]);
}


void listaAeroportos() {
	char id[MAX_CODIGO_AEROPORTO];
	int indexAeroporto, ultima = 0;

	ultima = leProximaPalavra(id);
	if (strlen(id) == 0)
		listaTodosAeroportos();
	else {
		while (strlen(id) != 0) {
			indexAeroporto = encontraAeroporto(id);
			if (indexAeroporto == NAO_EXISTE)
				printf("%s: no such airport ID\n", id);
			else
				mostraAeroporto(indexAeroporto);
			if (!ultima)
				ultima = leProximaPalavra(id);
			else
				break;
		}
	}
}



/* Funcoes Voos */

void mostraVoo(int index) {
	printf("%s %s %s ", _voos[index].id, _voos[index].partida,
		_voos[index].chegada);
	mostraData(_voos[index].data);
	printf(" ");
	mostraHora(_voos[index].hora);
	printf("\n");
}

void mostraVooPartida(int index) {
	printf("%s %s ", _voos[index].id, _voos[index].chegada);
	mostraData(_voos[index].data);
	printf(" ");
	mostraHora(_voos[index].hora);
	printf("\n");
}

void mostraVooChegada(int index) {
	Hora h = converteNumHora(_voos[index].horaChegada);
	printf("%s %s ", _voos[index].id, _voos[index].partida);
	mostraData(converteNumData(_voos[index].horaChegada));
	printf(" ");
	mostraHora(h);
	printf("\n");
}



int encontraVoo(char id[], Data d) {
	int numData = converteDataNum(d);
	int i;

	for (i = 0; i < _numVoos; i++)
		if (!strcmp(id, _voos[i].id)
			&& numData == converteDataNum(_voos[i].data))
			return i;
	return NAO_EXISTE;
}


int validaIDVoo(char id[]) {
	int i = 0, l = strlen(id);
	if (l < 3 || l > 6)
		return FALSE;
	for (i = 0; i < 2; i++)
		if (!(id[i] >= 'A' && id[i] <= 'Z'))
			return FALSE;

	while (id[i] != '\0') {
		if (!(id[i] >= '0' && id[i] <= '9'))
			return FALSE;
		i++;
	}
	return TRUE;
}

int validaVoo(Voo v) {
	if (validaIDVoo(v.id) == FALSE)
		printf("invalid flight code\n");
	else if (encontraVoo(v.id, v.data) != NAO_EXISTE)
		printf("flight already exists\n");
	else if (encontraAeroporto(v.partida) == NAO_EXISTE)
		printf("%s: no such airport ID\n", v.partida);
	else if (encontraAeroporto(v.chegada) == NAO_EXISTE)
		printf("%s: no such airport ID\n", v.chegada);
	else if (_numVoos == MAX_NUM_VOOS)
		printf("too many flights\n");
	else if (validaData(v.data) == FALSE)
		printf("invalid date\n");
	else if (validaHora(v.duracao) == FALSE)
		printf("invalid duration\n");
	else if (v.capacidade < 10)               /* Aqui altera-se o limite máximo para a capacidade do voo de 100, como diz no enunciado, que já não existe */ 
		printf("invalid capacity\n");
	else
		return TRUE;
	return FALSE;
}

void criaVoo(Voo v) {
	strcpy(_voos[_numVoos].id, v.id);
	strcpy(_voos[_numVoos].partida, v.partida);
	strcpy(_voos[_numVoos].chegada, v.chegada);
	_voos[_numVoos].data = v.data;
	_voos[_numVoos].hora = v.hora;
	_voos[_numVoos].duracao = v.duracao;
	_voos[_numVoos].capacidade = v.capacidade;
	_voos[_numVoos].horaPartida =
		converteDataHoraNum(_voos[_numVoos].data,
					_voos[_numVoos].hora);
	_voos[_numVoos].horaChegada =
		_voos[_numVoos].horaPartida +
		converteHoraNum(_voos[_numVoos].duracao);
	_numVoos++;
}

void adicionaListaVoos() {
	Voo v;
	int i;

	if (leProximaPalavra(v.id)) {
		for (i = 0; i < _numVoos; i++)
			mostraVoo(i);
		return;
	} else {
		leProximaPalavra(v.partida);
		leProximaPalavra(v.chegada);
		v.data = leData();
		v.hora = leHora();
		v.duracao = leHora();
		scanf("%d", &v.capacidade);
		leAteFimDeLinha();
	}

	if (validaVoo(v))
		criaVoo(v);
}


int cmpVoosPartida(int a, int b) {
	return (_voos[a].horaPartida > _voos[b].horaPartida);
}


int cmpVoosChegada(int a, int b) {
	return (_voos[a].horaChegada > _voos[b].horaChegada);
}


void listaVoosPartida() {
	int indexVoos[MAX_NUM_VOOS], i, n = 0;
	char partida[MAX_CODIGO_AEROPORTO];

	lePalavraAteFimDeLinha(partida);

	if (encontraAeroporto(partida) == NAO_EXISTE)
		printf("%s: no such airport ID\n", partida);

	for (i = 0; i < _numVoos; i++) {
		if (!strcmp(_voos[i].partida, partida))
			indexVoos[n++] = i;
	}

	bubbleSort(indexVoos, n, cmpVoosPartida);

	for (i = 0; i < n; i++)
		mostraVooPartida(indexVoos[i]);
}


void listaVoosChegada() {
	int indexVoos[MAX_NUM_VOOS], i, n = 0;
	char chegada[MAX_CODIGO_AEROPORTO];

	lePalavraAteFimDeLinha(chegada);

	if (encontraAeroporto(chegada) == NAO_EXISTE)
		printf("%s: no such airport ID\n", chegada);

	for (i = 0; i < _numVoos; i++) {
		if (!strcmp(_voos[i].chegada, chegada))
			indexVoos[n++] = i;
	}

	bubbleSort(indexVoos, n, cmpVoosChegada);

	for (i = 0; i < n; i++)
		mostraVooChegada(indexVoos[i]);
}


void alteraData() {
	Data d;

	d = leData();
	leAteFimDeLinha();
	if (validaData(d) == FALSE)
		printf("invalid date\n");
	else {
		_hoje = d;
		mostraData(_hoje);
		printf("\n");
	}
}

/* ----------------------------------------------------------------------2ºProjeto---------------------------------------------------------------------------- */
/*
Tomás dos Santos Taborda
nº 103641
LETI

Neste projeto foram usadas funções que pertencem ao projeto providenciado
pelo professor, sendo apenas alteradas pequenas partes para se adequar melhor
ao enunciado do segundo projeto. Tudo o que se encontra neste ficheiro contudo
são funções, da minha autoria, que às vezes usam funções do projeto do professor.

Neste projeto, em relação ao anterior, percebeu-se que a organização do nosso código
é importantíssima. Por isso, está devidamente separado, organizado e comentado.

*/

/* Função que liberta a memória da Linked List ---------------------------------------------------------------------------------------------- */


void LibertaTudo(){
	linkedlistReservasGERAL *tmp;

	while (first != NULL){	
		tmp = first;
		first = first->next;
		
		free(tmp->ReservaCODE);
		free(tmp);
	}
	exit (0);
}

/*
Esta função é responsável por acabar o programa e libertar a memória
correspondente à Linked List quando não existe memória disponível ou
quando no input existe um 'q
*/

/* Funções auxiliares que verificam o input com getchar() ----------------------------------------------------------------------------------- */

int verificainput(){
	char c=getchar();

	while (c >= 'A' && c <= 'Z')
		c = getchar();
	while (c){
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
			return FALSE;
		c = getchar();	
	}
	return TRUE;
}

/* 
Esta função apenas verifica se existe letras a seguir ao getchar seguinte 
e irá ajudar mais tarde
*/


char *guardaCodeReserva(){
    char c = getchar();
    char *Rcode;
	int i=0, sz=1;

	Rcode = malloc(1*(sizeof(char)));

	if (Rcode == NULL){
		printf("No memory.\n");
		free(Rcode);
		LibertaTudo();
	}
	
	while (c == ' ' || c == '\t')
		c = getchar();
    while ((c != ' ' ) && (c != '\t') && (c != '\n')){
        *(Rcode + i)  = c;
		Rcode = realloc(Rcode, (sz++) * sizeof(Rcode));
		i++;
		c = getchar();
		if (Rcode == NULL){
			printf("No memory.\n");
			free(Rcode);
			LibertaTudo();
		}
	}
	if (Rcode == NULL){
		printf("No memory.\n");
		free(Rcode);
		LibertaTudo();
	}
	*(Rcode + i)  = '\0';
	return Rcode;
}

/*
Esta função é uma das mais importantes, pois guarda o código 
de reserva a partir do input com alocação e realocação de memória 
de modo a apenas guardar a mem]oria necesaária pois não existe limite máximo para o seu tamanho
*/


/* Verificadores e Validadores auxiliares --------------------------------------------------------------------------------------------------- */

char validaCodeReserva(char *r){
	int i, x = strlen(r);

	if (x < 10){
        return FALSE;
	}
	for (i=0; i < x; i++){
		if (!((r[i] >= 'A' && r[i] <= 'Z') || (r[i] >= '0' && r[i] <= '9')))
            return FALSE;
	}
	return TRUE;
}

/*
Nesta função o argumento é verificado e validado, se o código
de reserva for válido, a função retorna TRUE, se não retorna FALSE
*/


int verificaVOOS(char idVOO[7]){
	int i;

	for (i=0; i < _numVoos; i++){
		if (strcmp(_voos[i].id, idVOO) == 0){
			return TRUE;
		}
	}
	return FALSE;
}

/* 
Esta função apenas verifica se o argumento da função existe na lista de voos
e se existir retorna TRUE, se não retirna FALSE 
*/


int verificaReserva(char *ReservaCode){
	linkedlistReservasGERAL *tmp = first;

	while (tmp != NULL){
		if (strcmp(tmp->ReservaCODE, ReservaCode) == 0)
			return TRUE;
		tmp = tmp->next;
	}
	return FALSE;
}

/*
Aqui esta função verifica se o código de reserva existe na
Linked List responsável por guardar as reservas
*/


/* Funções auxilares ------------------------------------------------------------------------------------------------------------------------ */ 
/* Estas funções ajudam a validar certas partes do input */

int numReservas(){
	int counter=0;
	linkedlistReservasGERAL *tmp;

	for (tmp = first; tmp != NULL; (tmp = tmp->next)){	
		counter++;
	}
	return counter;
}


int ReservaExistente(linkedlistReservasGERAL Reserva){
	linkedlistReservasGERAL  *tmp = first;
	int c=0;

	while (tmp != NULL){
		if (strcmp(tmp->ReservaCODE, Reserva.ReservaCODE) == 0)
			return FALSE;      /* --> se encontrar um que tem o mesmo código de reserva, retorna FALSE */
		c++;
		tmp = tmp->next;
	}
	if (c == numReservas()){
		return TRUE;
	}
	return 0;
}


int calculadorDEcapacidade(linkedlistReservasGERAL Reserva){
	int soma=0, i, capacidadeVoo=0;
	linkedlistReservasGERAL *tmp = first;

	while (tmp!=NULL){
		if (tmp->data.ano == Reserva.data.ano && tmp->data.dia == Reserva.data.dia &&
		tmp->data.mes == Reserva.data.mes && (strcmp(tmp->VooCODE, Reserva.VooCODE) == 0))
			soma = soma + tmp->numPassageiros;		
		tmp = tmp->next;
	}
	for (i = 0; i < _numVoos; i++){
		if (strcmp(_voos[i].id, Reserva.VooCODE) == 0){
			capacidadeVoo = _voos[i].capacidade;
			break;
		}
	}
	soma = soma + Reserva.numPassageiros;
	if (soma > capacidadeVoo){
		return FALSE;
	}
	else
		return TRUE;
	
}

/* 
A função acima é responsável por detetar o erro de quando existem mais passageiros do que a capacidade de voo,
Calculando assim a sua capacidade de acordo com as reservas 
*/


char procuraReservaVoo(char idVOO[], linkedlistReservasGERAL Reserva){
	int c;

	for (c = 0; c < _numVoos; c++){
		if ((strcmp(idVOO, _voos[c].id) == 0) && (_voos[c].data.ano == Reserva.data.ano)
		&& (_voos[c].data.mes == Reserva.data.mes) && (_voos[c].data.dia == Reserva.data.dia))
			return TRUE;
	}
	return FALSE;
}

/* Aqui é simplesmente procurado se existe ou não um dado ID de voo com uma data no sistema */


void listvalues(){
	linkedlistReservasGERAL *tmp= first;

	while (tmp!=NULL){
		printf("%s ", tmp->VooCODE);
		mostraData(tmp->data);
		printf(" ");
		printf("%s %d", tmp->ReservaCODE, tmp->numPassageiros);
		printf("\n");
		tmp = tmp->next;
	}
}

/* A função acima é puramente facultativa, apenas ajudou me a construir a função de criação */
/* de uma reserva, listando as que já existem na Linked List */

/* Valida Reserva e identificador de erros -------------------------------------------------------------------------------------------------- */

int validaReserva(linkedlistReservasGERAL Reserva) {
	if (validaData(Reserva.data) == FALSE){
		free(Reserva.ReservaCODE);
		printf("invalid date\n");
	}
	else if ((procuraReservaVoo(Reserva.VooCODE, Reserva) == FALSE) || (verificaVOOS(Reserva.VooCODE) == FALSE)){
		printf("%s: flight does not exist\n", Reserva.VooCODE);
		free(Reserva.ReservaCODE);
	}
	else if (validaCodeReserva(Reserva.ReservaCODE) == FALSE){
		free(Reserva.ReservaCODE);
		printf("invalid reservation code\n");
	}
	else if (Reserva.numPassageiros < 0){
		free(Reserva.ReservaCODE);
		printf("invalid passenger number\n");
	}
	else if (ReservaExistente(Reserva) == FALSE){
		printf("%s: flight reservation already used\n", Reserva.ReservaCODE);
		free(Reserva.ReservaCODE);
	}
	else if (calculadorDEcapacidade(Reserva) == FALSE){
		free(Reserva.ReservaCODE);
		printf("too many reservations\n");
	}
	else
		return TRUE;
	return FALSE;
}

/* 
Quando um erro é encontrado, a memória alocada para o
código de reserva é libertada pois nenhuma reserva irá ser criada 
Quando nenhum erro é encontrado, a função dá return TRUE para a reserva respetiva ser criada
*/

/* Funções auxiliares que eliminam voos/reservas com Ids de Voos/Reservas ------------------------------------------------------------------- */

void eliminaVOO(char idVOO[]){
	int i=0, j;

	while (i < _numVoos){
		if (strcmp(_voos[i].id, idVOO) == 0){
			if (i < (_numVoos - 1)){
				for (j = i; j < (_numVoos - 1); j++){
					_voos[j] = _voos[j + 1];
				}
			}
			_numVoos--;
		}
		else i++; 
	}
}

/* 
É possível apagar um elemento de um array se, quando encontrado o Voo correspondente,
os elementos daí para a frente se "deslocarem" um bloco para trás, "retirando" assim o voo
correspondente, decrementando o número de elementos presente na lista.
*/


void apagaReservacomIDVoo(char *idVOO){
	linkedlistReservasGERAL *tmp = first;
	linkedlistReservasGERAL *help = first;

	while (tmp != NULL){
		if (strcmp(tmp->VooCODE, idVOO) == 0){
			if (tmp == first){
				first = first->next;
				free(tmp->ReservaCODE);
				free(tmp);
				tmp = first;
			}
			else {
				help->next = tmp->next;
				free(tmp->ReservaCODE);
                free(tmp);
                tmp =  help->next;
			}
		}
		else {
			help = tmp;
			tmp = tmp->next;
		}
	}
}

/* 
Esta função agarra no código de Voo procurando
o pela Linked list e se esse código fôr encontrado
são eliminadas as reservas correspondentes, "ligando"
o node seguinte com o anterior, quando um elemento é
apagado, de modo a que nenhum deles se "perca"
*/


void apagaReservacomReservaCode(char *ReservaCODE){
	linkedlistReservasGERAL *tmp = first;
	linkedlistReservasGERAL *help = first;
	
	while (tmp != NULL){
		if (strcmp(tmp->ReservaCODE, ReservaCODE) == 0){
			if (tmp == first){
				first = first->next;
				free(tmp->ReservaCODE);
				free(tmp);
				tmp = first;
			}
			else {
				help->next = tmp->next;
				free(tmp->ReservaCODE);
                free(tmp);
                tmp =  help->next;
			}
		}
		else {
			help = tmp;
			tmp = tmp->next;
		}
	}
}

/*
O comportamento desta função é similar à anterior mas esta usa
o código de reserva para procurar e apagar o respetivo elemento
*/


/* Funções principais ----------------------------------------------------------------------------------------------------------------------- */

/* Estas funções são as que reunem praticamente todas as funções anteriores e */
/* que representam fundamentalmente os comandos dados pelo enunciado e o que eles fazem */


void listaReservasVoo(linkedlistReservasGERAL Reserva){
	linkedlistReservasGERAL *tmp= first;

	if ((procuraReservaVoo(Reserva.VooCODE, Reserva) == FALSE)){
		printf("%s: flight does not exist\n", Reserva.VooCODE);
		return;
	}
	if (tmp == NULL){
		return;
	}
	while (tmp!=NULL){
		if (tmp->data.ano == Reserva.data.ano && tmp->data.dia == Reserva.data.dia &&
		tmp->data.mes == Reserva.data.mes && (strcmp(tmp->VooCODE, Reserva.VooCODE) == 0)){
			printf("%s %d\n", tmp->ReservaCODE, tmp->numPassageiros);		
		}
		tmp = tmp->next;
	}
	return;
}

/* 
Aqui em cima, é verificado se o Voo existe na respetiva data 
Se o elemento da lista tiver o mesmo código e data que o input ele dá print
*/


void criaReserva(linkedlistReservasGERAL Reserva){
	linkedlistReservasGERAL *new = (linkedlistReservasGERAL*)malloc(sizeof(linkedlistReservasGERAL));
	linkedlistReservasGERAL *last1 = first;

	if (new == NULL){
		printf("No memory.\n");
		free(new);
		LibertaTudo();
	}

	strcpy(new->VooCODE, Reserva.VooCODE);
	new->data.ano = Reserva.data.ano;
	new->data.dia = Reserva.data.dia;
	new->data.mes = Reserva.data.mes; 
	new->ReservaCODE = Reserva.ReservaCODE;
	new->sizeRcode = strlen(Reserva.ReservaCODE);
	new->numPassageiros = Reserva.numPassageiros;
	new->next=NULL;

	if(first == NULL){
		first = new;
	}
	else if (strcmp(first->ReservaCODE, new->ReservaCODE) > 0){
		new->next=first;
		first = new;
	}
	else{
		while (last1->next != NULL && (strcmp(new->ReservaCODE, last1->next->ReservaCODE) > 0)){
			last1 = last1->next;
		}
		new->next = last1->next;
		last1 -> next = new;
	}
}

/* 
A função acima é a responsável por criar uma reserva, copiando os dados recolhidos do argumento, 
para um novo node da linked list. Esta função não só cria uma nova reserva como também a ordena logo assim
que é criada de acordo com a ordem lexicográfica das nodes já existentes e do novo código de reserva.
Esta função aloca memória para um novo node quando os dados da reserva são validados
*/


void criaListaReserva(){
    linkedlistReservasGERAL Reserva;

	leProximaPalavra(Reserva.VooCODE);
	Reserva.data = leData();
	Reserva.ReservaCODE = guardaCodeReserva();

	if ((validaCodeReserva(Reserva.ReservaCODE) == FALSE) && strlen(Reserva.ReservaCODE) < 2){
		if (validaData(Reserva.data) == TRUE){
			listaReservasVoo(Reserva);
			free(Reserva.ReservaCODE);
		}
		else {
			free(Reserva.ReservaCODE);
			printf("invalid date\n");
		}
	}
	else {
		Reserva.sizeRcode = strlen(Reserva.ReservaCODE);
		scanf("%d", &Reserva.numPassageiros);
		leAteFimDeLinha();
	
		if (validaReserva(Reserva) == TRUE)
			criaReserva(Reserva);
	}
}

/* 
Esta função é a função correspondente à função 'r'
reunindo assim todas as funções interiores para avaliar o input e dependendo
do seu conteúdo, é selecionado o que deve ser feito depois do input ser verificado.
Se o input fôr aoenas o código de Voo e uma certa data, apenas são listadas as respetivas 
reservas, se o input fôr validado e a informação alocada guardada na estrutura Reserva é libertada 

Se o input não fôr para listar as reservas então é guardada o resto da informação 
para, se validada, ser criada uma nova reserva
*/


void apagaVOOeRESERVA(){
	linkedlistReservasGERAL Reserva;
	Reserva.ReservaCODE = guardaCodeReserva();

	if (validaIDVoo(Reserva.ReservaCODE) == TRUE){
		apagaReservacomIDVoo(Reserva.ReservaCODE);

		if (verificaVOOS(Reserva.ReservaCODE) == FALSE){
			free(Reserva.ReservaCODE);
			printf("not found\n");
			return;
		}
		else {
			eliminaVOO(Reserva.ReservaCODE);
			free(Reserva.ReservaCODE);
			return;
		}
	}
	else {
		if (verificaReserva(Reserva.ReservaCODE) == FALSE){
			printf("not found\n");
			free(Reserva.ReservaCODE);
			return;
		}
		else {
			apagaReservacomReservaCode(Reserva.ReservaCODE);
			free(Reserva.ReservaCODE);
			return;
		}
	}
}

/*
Esta função representa fundamentalmente a função 'e' e é a responsável por apagar dependendo se o input
é um código de Voo ou um código de Reserva, as reservas correspondentes.

Se o input é um código de Voo então a função não só elimina
as reservas correspondentes como também elimina o voo da lista que contém os voos

Se o input é um código de reserva, apenas as reservas correspondentes com o mesmo código são apagadas
*/


int main() {
	int c;

	while ((c = getchar()) != EOF) {
		switch (c) {
		case 'q': LibertaTudo();
			break;
		case 'a': adicionaAeroporto();
			break;
		case 'l': listaAeroportos();
			break;
		case 'v': adicionaListaVoos();
			break;
		case 'p': listaVoosPartida();
			break;
		case 'c': listaVoosChegada();        
			break;                           
		case 't': alteraData();
			break;
		case 'r': criaListaReserva();
			break;                 
		case 'e': apagaVOOeRESERVA();
			break;
		default: printf("Invalid comand: %c\n", c);
		}
	}

	return 0;
} 

/* 
Foram acrescentadas as funções 'r' e 'e', à função main do professor,
Agora a função q é responsável por não só terminar
o programa como também libertar toda a memória alocada durante o seu funcionamento 
*/