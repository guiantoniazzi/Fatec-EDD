#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <locale.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <conio.h>
#include <fstream>
#include <fileapi.h>

/*
[IDEIA]
- ABRE O ARQUIVO BASE COM OS DADOS

- SEPARA ELES EM UM VETOR BASE

- DUPLICA O VETOR BASE PARA OUTRO VETOR A - Selection Sort
	- INICIA O TEMPO 1
	- APLICA A ORGANIZAÇÃO A
	- TERMINA O TEMPO 1

- DUPLICA O VETOR BASE PARA OUTRO VETOR B - Quick Sort
	- INICIA O TEMPO 2
	- APLICA A ORGANIZAÇÃO B
	- TERMINA O TEMPO 2

- DUPLICA O VETOR BASE PARA OUTRO VETOR C - Mergesort
	- INICIA O TEMPO 3
	- APLICA A ORGANIZAÇÃO C
	- TERMINA O TEMPO 3

- COMPARA AS 3 VERSÕES E QUAL TEVE O MELHOR DESEMPENHO EM TEMPO
*/

const std::string computador = "Leticia";

struct linha{
        std::string nomeArquivo;
        float tempoSelection;
        float tempoQuick;
        float tempoMerge;
        std::string tamanho;
        std::string duplicidade;
        std::string org_previa;
        std::string caminho;
};

//Funções
float delta_tempo(clock_t Tempo_Final, clock_t Tempo_Inicial)
{
	float resultado;
	resultado = ((Tempo_Final - Tempo_Inicial) / (CLOCKS_PER_SEC / 1000));
	resultado = resultado/1000;
	return resultado;
}

//Sort A
void selection_sort (int vetor[], int tamanho)
{
    int i, j, min, x;
    for (i=1; i<=tamanho-1; i++)
	{
        min = i;
		for (j=i+1; j<=tamanho; j++)
		{
	            if (vetor[j] < vetor[min])
	            {
	            	min = j;
				}
		}
		x = vetor[min];
		vetor[min] = vetor[i];
		vetor[i] = x;
    }
}

//Sort B
void quick_sort(int vetor[], int esq, int dir){
    int pivo = esq, i,ch,j;
    for(i=esq+1;i<=dir;i++){
        j = i;
        if(vetor[j] < vetor[pivo]){
            ch = vetor[j];
            while(j > pivo){
                vetor[j] = vetor[j-1];
                j--;
            }
            vetor[j] = ch;
            pivo++;
        }
    }
    if(pivo-1 >= esq){
        quick_sort(vetor,esq,pivo-1);
    }
    if(pivo+1 <= dir){
        quick_sort(vetor,pivo+1,dir);
    }
 }

//Sort C
void mergeSort(int *vetor, int posicaoInicio, int posicaoFim) {
    int i, j, k, metadeTamanho, *vetorTemp;
    if(posicaoInicio == posicaoFim) return;
    metadeTamanho = (posicaoInicio + posicaoFim ) / 2;

    mergeSort(vetor, posicaoInicio, metadeTamanho);
    mergeSort(vetor, metadeTamanho + 1, posicaoFim);

    i = posicaoInicio;
    j = metadeTamanho + 1;
    k = 0;
    vetorTemp = (int *) malloc(sizeof(int) * (posicaoFim - posicaoInicio + 1));

    while(i < metadeTamanho + 1 || j  < posicaoFim + 1) {
        if (i == metadeTamanho + 1 ) {
            vetorTemp[k] = vetor[j];
            j++;
            k++;
        }
        else {
            if (j == posicaoFim + 1) {
                vetorTemp[k] = vetor[i];
                i++;
                k++;
            }
            else {
                if (vetor[i] < vetor[j]) {
                    vetorTemp[k] = vetor[i];
                    i++;
                    k++;
                }
                else {
                    vetorTemp[k] = vetor[j];
                    j++;
                    k++;
                }
            }
        }

    }
    for(i = posicaoInicio; i <= posicaoFim; i++) {
        vetor[i] = vetorTemp[i - posicaoInicio];
    }
    free(vetorTemp);
}

void escreveArquivo(linha registro){

    DIR *ds;

    ds = opendir("./saida");
    if (ds == NULL) {
        mkdir("./saida");
        ds = opendir("./saida");
    }

    std::string caminhoResultado = "./saida/resultado.csv";
//    std::ofstream outfile (caminhoResultado.c_str());
    std::ofstream outfile (caminhoResultado.c_str(), std::ios::app);

    outfile << registro.nomeArquivo << ";";
    outfile << registro.tempoSelection << ";";
    outfile << registro.tempoQuick << ";";
    outfile << registro.tempoMerge << ";";
    outfile << computador << ";";
    outfile << registro.caminho << std::endl;

    outfile.close();
}

void executaMetodos(const char* caminhoArquivo, std::string nomeArquivo){

    FILE *arq;
    char Linha[100000];
    char *result;
    int i = 0;
    int minhaArray[99999];
    int tamanho = 0;

    arq = fopen(caminhoArquivo, "rb");
    if (arq == NULL)
    {
        printf("Problemas na abertura do arquivo\n");
    }
    while (!feof(arq))
    {
        result = fgets(Linha, 100000, arq);
        if (result) {
            //std::cout << Linha << "\n";
            minhaArray[i] = atoi(Linha);
            i++;
            tamanho = i;
        }
    }
    fclose(arq);

    //Variáveis de Tempo
    clock_t tempoI, tempoF;
    float tempoA, tempoB, tempoC;

    //Inicio do TempoA
    tempoI = clock();

    //método A - Selection Sort
    selection_sort(minhaArray, tamanho);

    //Fim do TempoA
    tempoF = clock();

    //Cálculo de TempoA
    tempoA = delta_tempo(tempoF, tempoI);

    //Inicio do TempoB
    tempoI = clock();

    //método B - Quick Sort
    quick_sort(minhaArray, 10, 20);

    //Fim do TempoB
    tempoF = clock();

    //Cálculo de TempoB
    tempoB = delta_tempo(tempoF, tempoI);

    //Inicio do TempoC
    tempoI = clock();

    //método C - Mergesort
    mergeSort(minhaArray, 0, tamanho - 1);

    //Fim do TempoC
    tempoF = clock();

    //Cálculo de TempoC
    tempoC = delta_tempo(tempoF, tempoI);


    linha registro;

    registro.nomeArquivo = nomeArquivo;
    registro.tempoSelection = tempoA;
    registro.tempoQuick = tempoB;
    registro.tempoMerge = tempoC;
    registro.caminho = caminhoArquivo;

    escreveArquivo(registro);
}


void leDir(const char* nomeDir){

    DIR* dirp;
	struct dirent* entryp;
	dirp = opendir(nomeDir);



    while((entryp=readdir(dirp)) != nullptr){
       std::string fileName = entryp->d_name;
       std::string caminho = std::string(nomeDir) + "/" + fileName;

       DWORD atributos = GetFileAttributes(caminho.c_str());
       if (atributos & FILE_ATTRIBUTE_DIRECTORY){
            if(fileName != "." && fileName != ".."){
                leDir(caminho.c_str());
            }
       }
       else if(atributos & FILE_ATTRIBUTE_ARCHIVE){
           executaMetodos(caminho.c_str(), fileName);
           std::cout << "Processando..." << fileName << "\n";
       }
    }

    closedir(dirp);
}


int main()
{
    const char* diretorioOrigem = "./arquivos";
    leDir(diretorioOrigem);
    std::cout << "FIM";
    return 0;
}
