#include <iostream>
#include <stdio.h>
#include<stdlib.h>
#include <locale.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <conio.h>
#include <fstream> 

/*
[IDEIA]
- ABRE O ARQUIVO BASE COM OS DADOS

- SEPARA ELES EM UM VETOR BASE

- DUPLICA O VETOR BASE PARA OUTRO VETOR A - Selection Sort
	- INICIA O TEMPO 1
	- APLICA A ORGANIZA��O A
	- TERMINA O TEMPO 1
	
- DUPLICA O VETOR BASE PARA OUTRO VETOR B - Quick Sort
	- INICIA O TEMPO 2
	- APLICA A ORGANIZA��O B
	- TERMINA O TEMPO 2
	
- DUPLICA O VETOR BASE PARA OUTRO VETOR C - Mergesort
	- INICIA O TEMPO 3
	- APLICA A ORGANIZA��O C
	- TERMINA O TEMPO 3

- COMPARA AS 3 VERS�ES E QUAL TEVE O MELHOR DESEMPENHO EM TEMPO
*/

//Fun��es
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

int main()
{
	DIR *dp, *ds;
	struct dirent *ep;
	dp = opendir("./txtTeste");
	
	int minhaArray[99999];
	int tamanho = 0;
	
	if(dp != NULL) {
		while(ep = readdir(dp)) {
			std::string nomeArquivo = ep->d_name;
			if(nomeArquivo.find(".txt") != std::string::npos) {
			
				FILE *arq;
			  	char Linha[100];
			  	char *result;
			  	int i = 0;
			  	
			  	std::string caminho = "./txtTeste/" + nomeArquivo;
			
			  	const char *cstr = caminho.c_str();
			  	
			  	arq = fopen(cstr, "rb");
			  	if (arq == NULL)
			  	{
			    	printf("Problemas na abertura do arquivo\n");
			  	}
			  	while (!feof(arq))
			  	{
			      	result = fgets(Linha, 100, arq);
			      	if (result) {
			      		//std::cout << Linha << "\n";
						minhaArray[i] = atoi(Linha);
						i++;
						tamanho = i;
					}
			  	}
			  	fclose(arq);
			  	
			  	//Vari�veis de Tempo
			    clock_t tempoI, tempoF;
				float tempoA, tempoB, tempoC;
				
				//Inicio do TempoA
				tempoI = clock();
				
				//m�todo A - Selection Sort
				selection_sort(minhaArray, tamanho);
				
				//Fim do TempoA
				tempoF = clock();
				
				//C�lculo de TempoA
				tempoA = delta_tempo(tempoF, tempoI);
				
				//Inicio do TempoB
				tempoI = clock();
				
				//m�todo B - Quick Sort
				quick_sort(minhaArray, 10, 20);
				
				//Fim do TempoB
				tempoF = clock();
				
				//C�lculo de TempoB
				tempoB = delta_tempo(tempoF, tempoI);
				
				//Inicio do TempoC
				tempoI = clock();
				
				//m�todo C - Mergesort
				mergeSort(minhaArray, 0, tamanho - 1);
			
				//Fim do TempoC
				tempoF = clock();
				
				//C�lculo de TempoC
				tempoC = delta_tempo(tempoF, tempoI);
			
				//FINALIZA��O
				std::cout << nomeArquivo << "\n";
				printf("%0.10f \n", tempoA);
				printf("%0.10f \n", tempoB);
				printf("%0.10f \n\n", tempoC);
				
				ds = opendir("./saida");
				if (ds == NULL) {
					mkdir("./saida");
					ds = opendir("./saida");
				}
				
				std::string caminha = "./saida/" + nomeArquivo;
				std::ofstream outfile (caminha.c_str());

				outfile << nomeArquivo << std::endl;
				outfile << tempoA << std::endl;
				outfile << tempoB << std::endl;
				outfile << tempoC << std::endl;

				outfile.close();
			}
		}
		(void) closedir(dp);
	}
	else {
		printf("N�o foi poss�vel abrir a pasta");
	}
}
