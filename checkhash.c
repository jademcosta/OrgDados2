#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAIORPRIMO 2003
#define MAIOR_QUANTIDADE_DE_PALAVRAS 2000


char** lePalavrasDoDicionario();
void colocaPalavrasDoDicionarioNumaHashTable(char**);
int transformaStringEmNumero(char*);
unsigned int hash1(unsigned int);
unsigned int hash2(unsigned int);
void insereNaHashTable(unsigned int, unsigned int, unsigned int);
void inicializaHashTable();
void comparaPalavrasDeUmTexto();
int verificaSeEstaNaHash(char*);

int *hashTable;
int quantidadeDePalavrasDoDicionario;

int main() {
    
    char** palavrasDoDicionario;
    
    palavrasDoDicionario = lePalavrasDoDicionario();
    inicializaHashTable();
    colocaPalavrasDoDicionarioNumaHashTable(palavrasDoDicionario);
    
    comparaPalavrasDeUmTexto();
    
    
    return 0;
}


char** lePalavrasDoDicionario(int *quantidadeDePalavras) {
    
    char** textoLido;
    int i, caractereDaVez, linha;
    FILE *dicionarioFile;
    char c;
    
    //Damos o malloc no vetor de strings
    textoLido = (char**) malloc(MAIOR_QUANTIDADE_DE_PALAVRAS * sizeof(char*));
    if(textoLido == NULL) {
        printf("Erro no malloc da leitura de texto do dicionario.\n");
        exit(1);
    }
    for(i = 0; i < MAIOR_QUANTIDADE_DE_PALAVRAS; i++) {
        textoLido[i] = (char*) malloc(15 * sizeof(char));
        if(textoLido[i] == NULL) {
            printf("Erro no malloc da linha da leitura de texto do dicionario.\n");
            exit(1);
        }
    }
    
    //Abrimos o arquivo do dicionario e lemos ele
    dicionarioFile = fopen ("arq1.dic", "r");
    printf("Abriu o arquivo.\n");
    caractereDaVez = 0;
    linha = 0;
    while ((c = getc(dicionarioFile)) != EOF) {     
        if(c == '\n') {
            textoLido[linha][caractereDaVez] = '\0'; //VER SE TENHO QUE DEIXAR OU NAO ISSO AQUI
            linha++;
            caractereDaVez = 0;
            continue;
        }
        
        textoLido[linha][caractereDaVez] = c;
        caractereDaVez++;
    }
    
    printf("Fim da leitura do arquivo de dicionario. Quantidade de palavras: %d\n", (linha + 1));
    quantidadeDePalavrasDoDicionario = linha;
    
    fclose (dicionarioFile);
    
    //fgets(textoLido[1],15,stdin);
    //printf("Retornando o vetor de strings do dicionario.\n");
    return textoLido;
} //Fim da função lePalavrasDoDicionario

void colocaPalavrasDoDicionarioNumaHashTable(char** palavrasDoDicionario) {

    int j, h1, h2;
    unsigned int stringParaNumero;
    
    for(j = 0; j < quantidadeDePalavrasDoDicionario; j++) {
        if((palavrasDoDicionario[j][0] == '\n') || (palavrasDoDicionario[j][0] == '\0')) {
            continue;
        }
        
        stringParaNumero = transformaStringEmNumero(palavrasDoDicionario[j]);
    
        h1 = hash1(stringParaNumero);
        h2 = hash2(stringParaNumero);
        
        //printf("String %s, deu %d, %d, %d\n", palavrasDoDicionario[j], stringParaNumero, h1, h2);
        insereNaHashTable(stringParaNumero, h1, h2);
    }
    printf("Quantidade total de palavras do dicionário: %d\n\n\n", j);
    //return hashTable;
} //Fim da função colocaPalavrasDoDicionarioNumaHashTable

int transformaStringEmNumero(char *string) {
    int tamanhoDaString, j, numeroDaString, pesoDaVez;
    tamanhoDaString = strlen(string);
    
    //printf("Dentro da funcao de transformação em numero. O tamanho da string é: %d\n", tamanhoDaString);
    
    numeroDaString = 0;
    //Aqui usamos -1 para ignorar o \n no final da string
    for(j = 0; j < (tamanhoDaString - 1); j++) {
        if(string[j] == '\0') {
            printf("String atual, tamanho: %d\n", strlen(string));
        }
        pesoDaVez = string[j] * (j + 1);
        numeroDaString = numeroDaString + abs(pesoDaVez);
        
        //printf("Letra %c que é %d * %d deu %d e com abs: %d\n", string[j], string[j], (j + 1), pesoDaVez, abs(string[j]));
    }
	if(numeroDaString == 4850) printf("Para palavra %s deu: %d\n", string, numeroDaString);
    if(numeroDaString < 0) {
        printf("Erro, a função h3 deu um número menor que zero!!! Deu: %d\n", numeroDaString);
        exit(-1);
    }
    
    //printf("a palavra %s se chama %d\n", string, numeroDaString);
    return numeroDaString;
} //Fim da função transformaStringEmNumero

//Função que calcula o numero base
unsigned int hash1(unsigned int numero) {
    return (numero % MAIORPRIMO);
}

//Função que calcula o número de deslocamento
unsigned int hash2(unsigned int numero) {
    int resultado;
    resultado = floor(numero / MAIORPRIMO);
    
    return ( resultado % (MAIORPRIMO)) + 1;   
}

void insereNaHashTable(unsigned int h3, unsigned int h1, unsigned int h2) {
    
    int contadorDeParada, posicao;
    int continuarProcurando;
    
    if(hashTable[h1] == -1) {
      //printf("Inseriu (de primeira) %d na posicao %d\n", h3, h1);
       hashTable[h1] = h3; 
       return;
    }
    
    contadorDeParada = 0;
    posicao = h1;
    continuarProcurando = 1;
    while(continuarProcurando) {
        posicao = (posicao + h2) % MAIORPRIMO;
        
        if(hashTable[posicao] == -1) {
            hashTable[posicao] = h3; 
            //printf("Inseriu %d na posicao %d\n", h3, posicao);
            break;
        }
        
        contadorDeParada++;
        if(contadorDeParada == MAIORPRIMO) {
            printf("Hash cheia!");
            break;
        }
    } //Fim do while
    
    return;
} //Fim da função insereNaHashTable

//Dá o malloc e seta todas as posições = zero
void inicializaHashTable() {
    printf("Começou a inicializar a hashTable.\n");
    int j;
    
    hashTable = (int *) malloc(MAIORPRIMO * sizeof(int));
    if(hashTable == NULL) {
        printf("Erro no malloc da hashTable.\n");        
        exit(-1);
    }
    
    for(j = 0; j < MAIORPRIMO; j++) {
        hashTable[j] = -1;
    }
    
    printf("Terminou de inicializar a hashTable.\n");
}


void comparaPalavrasDeUmTexto() {
    printf("Começando a ler o arquivo e comparar palavras.\n");
    char* palavra;
    int i, caractereDaVez, linha, retorno;
    FILE *arquivoDeTexto, *arquivoDeErros;
    char c;
    char buf[20];
    
    //Damos o malloc na string
    

    palavra = (char*) malloc(15 * sizeof(char));
    if(palavra == NULL) {
        printf("Erro no malloc da palavra do texto.\n");
        exit(1);
    }
    
    //Abrimos o arquivo do texto e lemos ele
    arquivoDeTexto = fopen ("nome.txt", "r");
    //Abrimos o arquivo de erros
    arquivoDeErros = fopen ("nome.err", "w");
    printf("Abriu o arquivo de texto (para comparar).\n");
    caractereDaVez = 0;
    linha = 0;
    while ((c = getc(arquivoDeTexto)) != EOF) {     
        if(c == '\n') {
            palavra[caractereDaVez] = '\0';
            linha++;
            caractereDaVez = 0;
            
            retorno = verificaSeEstaNaHash(palavra);
            if(retorno != 2) {
                //if(retorno) printf("A palavra %s está na hash.\n", palavra); 
                //if(!retorno) fputs("A palavra %s da linha %d não está na hash.\n", palavra, linha, arquivoDeErros); 
                if(!retorno) {
                    sprintf(buf, "%s linha %d\n", palavra, linha);
                    fputs(buf, arquivoDeErros);
                }
            }
            continue;
        }
        
        if(c == ' ') {
            palavra[caractereDaVez] = '\0';
            caractereDaVez = 0;
            retorno = verificaSeEstaNaHash(palavra);
            if(retorno != 2) {
                //if(retorno) printf("A palavra %s está na hash.\n", palavra); 
                //if(!retorno) fputs("A palavra %s da linha %d não está na hash.\n", palavra, linha, arquivoDeErros);
                if(!retorno) {
                    sprintf(buf, "%s linha %d\n", palavra, linha);
                    fputs(buf, arquivoDeErros);
                }
            }
            continue;
        }
        
        //Ignora esses caracteres
        if((c == '.') || (c == ',') || (c == ';') || (c == ':') || (c == '!') || (c == '?')) {
            continue;
        }
        
        palavra[caractereDaVez] = c;
        caractereDaVez++;
    } //Fim do while
    
    printf("Fim da leitura do arquivo de texto.");
    quantidadeDePalavrasDoDicionario = linha;
    
    fclose (arquivoDeTexto);
    fclose (arquivoDeErros);
    printf("Terminou de ler o arquivo e comparar palavras.\n");
} //Fim da função comparaPalavrasDeUmTexto

int verificaSeEstaNaHash(char *palavra) {
    
    
    int h1, h2, stringParaNumero, continuarProcurando, contadorDeParada, posicao;
    //printf("Verificando %s \n", palavra);
    
    if(palavra[0] == '\0') return 2;
    
    stringParaNumero = transformaStringEmNumero(palavra);
    
    h1 = hash1(stringParaNumero);
    h2 = hash2(stringParaNumero);
    

    if(hashTable[h1] == -1) return 0; //Se for -1 na posição base, a palavra não consta na hashtable
    
    
    if(hashTable[h1] == stringParaNumero) {
		//printf("Procurou por %d em %d e nao achou. Valor nessa posicao: %d\n", stringParaNumero, h1, hashTable[h1]);
		return 1;
    } else {
		//printf("Procurou por %d em %d e nao achou. Valor nessa posicao: %d\n", stringParaNumero, h1, hashTable[h1]);
	}
    
    contadorDeParada = 0;
    posicao = h1;
    continuarProcurando = 1;
    while(continuarProcurando) {
        posicao = (posicao + h2) % MAIORPRIMO;
        
        if(hashTable[posicao] == -1) {
            return 0;
            break;
        }
        
        if(hashTable[posicao] == stringParaNumero) {
            return 1;
            break;
        }
        
        contadorDeParada++;
        if(contadorDeParada == MAIORPRIMO) {
            break;
        }
    } //Fim do while
    
    return 0;
} //Fim da função verificaSeEstaNaHash
