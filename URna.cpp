//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "URna.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TFmRna *FmRna;
Thread *MyThread;
void lerArquivos(AnsiString ArquivoDeEventos);
void carregarArquivo(int x);
void addAmostraGraph(int NumeroDeAmostras, float* Amostras);
float normAmostras(int NumeroDeAmostras, float* Amostras);




//---------------------------------------------------------------------------

float funcao_ativacao(float net, int funcao, float a)
{
    if (!funcao)
    {
    // Função Logística
    /*
                    1
      y(n) = ---------------
             1 + exp(-a.net)
    */

        return( 1.0 / (1.0 + exp(-a * net)) );

    }
    else
    {
    // Função Tangente Hiperbólica
    /*
             exp(a.net) - exp(-a.net)
      y(n) = ------------------------
             exp(a.net) + exp(-a.net)
    */

        return( (exp(a * net) - exp(-a * net)) / (exp(a * net) + exp(-a * net)) );
    }
}
//---------------------------------------------------------------------------

float derivada(float net, int funcao, float a)
{

    if (!funcao){
	// Derivada da Função Logística
    /*
                    1                       1
      y(n) = --------------- * ( 1 - --------------- )
			 1 - exp(-a.net)         1 - exp(-a.net)
     */

        return( (1.0 / (1.0 + exp(-a * net))) * (1.0 - (1.0 / (1.0 + exp(-a * net)))) );

    }
    else
    {
    // Derivada da Função Tangente Hiperbólica
    /*
                   exp(a.net) - exp(-a.net)
      y(n) = 1 - ( ------------------------ )²
                   exp(a.net) + exp(-a.net)

    */

		return( 1.0 - pow((exp(a * net) - exp(-a * net)) / (exp(a * net) + exp(-a * net)),2) );
    }
}

//---------------------------------------------------------------------------

unsigned long contador = 0, epocas = 0;

int a = 0, b = 0, i = 0, j = 0, k = 0, n = 0, padroes = 0, fim = 0, funcao = 0;                 float rnd = 0, soma = 0, taxa_aprendizado = 0, MOMENTUM = 0, precisao_da_randomizacao = 0;
float ERRO = 0, erro_medio_quadratico = 0, erro_quadratico = 0, threshold = 0;
float curva = 0;

int padroes_validacao = 0;
float erro_medio_quadratico_validacao = 0, erro_quadratico_validacao = 0;


   // cx deve ser 512
const int cx = 20;         // Camada de entrada. // rba Reduzi a camada de entrada pois o processo estava pesando mt
const int c1 = 1;          // Camada Intermediária.
const int c2 = 2;           // Camada de Saída. /// 4 Opções 2 bits 00 01 10 11

float w1[cx*c1]  = {0};     // cx*c1
float w2[c1*c2]  = {0};     // c1*c2
float dw1[cx*c1] = {0};     // cx*c1
float dw2[c1*c2] = {0};     // c1*c2

AnsiString NomeDoArquivo;
int NumeroDeAmostras;
char Tipo;
int Duracao;
float* Amostras;
Boolean testar = false;

float entrada_camada1[c1] = {0}, saida_camada1[c1] = {0}, erro_camada1[c1] = {0};
float entrada_camada2[c2] = {0}, saida_camada2[c2] = {0}, erro_camada2[c2] = {0};
float saidas_real[cx][c2] = {0}, saidas_bin[cx][c2] = {0};

int saidas_formatadas_c1[c1] = {0};
int saidas_formatadas_c2[c2] = {0};

 // Padrões de Entrada da Rede.
float p[520] = {0};

// Valores desejaos dos padrões ao final do treinamento.
// Matriz temporaria      		  4 padroes
float d[4][c2] =
{
	0, 0,
	0, 1,
	1, 0,
	1, 1
};

float v[3][cx] =
{
/*
//1.2
0.000000, 0.075327, 0.150226,
//1.6
0.000000, 0.100362, 0.199710,
//2.2
0.000000, 0.137790, 0.272952,
//2.4
0.000000, 0.150226, 0.297042,
//2.8
0.000000, 0.175023, 0.344643,
//3.1
0.000000, 0.193549, 0.379779,
//3.5
0.000000, 0.218143, 0.425779,
*/
//3.7
0.000000, 0.230389, 0.448383,
//4.2
0.000000, 0.260842, 0.503623,
//4.4
0.000000, 0.272952, 0.525175
};
//
//
// Valores desejados dos padrões ao final do treinamento.
float dv[3][c2] =
{
/*
1.0,	0.0,    0.0,	0.0,	0.0,    0.0,	0.0,    0.0,	0.0,	0.0,
0.0,	1.0,	0.0,	0.0,	0.0,    0.0,	0.0,    0.0,	0.0,	0.0,
0.0,	0.0,	1.0,	0.0,	0.0,    0.0,	0.0,    0.0,	0.0,	0.0,
0.0,	0.0,	0.0,	1.0,	0.0,    0.0,	0.0,    0.0,	0.0,	0.0,
0.0,	0.0,	0.0,	0.0,	1.0,    0.0,	0.0,    0.0,	0.0,	0.0,
0.0,	0.0,	0.0,	0.0,	0.0,    1.0,	0.0,    0.0,	0.0,	0.0,
0.0,	0.0,	0.0,	0.0,	0.0,    0.0,	1.0,    0.0,	0.0,	0.0,
0.0,	0.0,	0.0,	0.0,	0.0,    0.0,	0.0,    1.0,	0.0,	0.0,
0.0,	0.0,	0.0,	0.0,	0.0,    0.0,	0.0,    0.0,	1.0,	0.0,
0.0,	0.0,	0.0,	0.0,	0.0,    0.0,	0.0,    0.0,	0.0,	1.0
*/

0.0,	0.0,    0.0
};


//---------------------------------------------------------------------------


//Método de abertura dos arquivos de eventos.
void lerArquivos(AnsiString ArquivoDeEventos)
{
  //Declaração do Ponteiro do Arquivo de Eventos.
  FILE *PtArquivoDeEventos;
  const char *recebe_string;

  //Declaração da variável de status de abertura do arquivo.
  bool status = false;

  //Verifica se o arquivo existe antes de abrir.
  if (FileExists(ArquivoDeEventos))
  {
	// ShowMessage("Encontrou e conseguiu abrir o arquivo dentro do metodo");
	//Abertura do Arquivo de Padrões.
	FILE *arq_treinamento;

	int contNum = 0;

	// AnsiString APath = "padroes/"+name[i]+".txt";
	AnsiString APath = ArquivoDeEventos;
	PtArquivoDeEventos = fopen(APath.c_str() ,"rt");

	//Número de Amostras do Evento.
	fscanf(PtArquivoDeEventos, "%d\n", &NumeroDeAmostras);

	//Duração do Evento.
	fscanf(PtArquivoDeEventos, "%d\n", &Duracao);

	//Tipo do Evento.
	fscanf(PtArquivoDeEventos, "%c\n\n", &Tipo);

	//Redimensiona o vetor de amostras do evento.
	//Amostras.resize(NumeroDeAmostras);
	Amostras = new float[2048];
	float* Amostras2 = new float[2048];

	// Pula o cabecalho
    fseek ( PtArquivoDeEventos, 20, SEEK_SET );
	//Recebe as amostras do evento do arquivo.
	for (int a = 0; a < NumeroDeAmostras; a++)
	{
		 fscanf(PtArquivoDeEventos, "%lf\n", &Amostras[a]);
  //		fseek ( PtArquivoDeEventos, a , SEEK_SET );
//		fget(Amostras[a], 10, PtArquivoDeEventos);
	}

	//Fecha o Ponteiro do Arquivo de Padrões.
	fclose(PtArquivoDeEventos);

	//Sinaliza se o arquivo foi aberto corretamente.
	status = true;

    // Plota as amostrar no gráfico
	addAmostraGraph(NumeroDeAmostras, Amostras2);

  }
  else
  {
	status = false;
	// recebe_string = ExtractFileName(ArquivoDeEventos).c_str();
  	ShowMessage("Erro ao abrir o arquivo de Eventos");
  }

  //return (status);
}
//---------------------------------------------------------------------------

void addAmostraGraph(int NumeroDeAmostras, float* Amostras)
{
	if (testar)
	{
		//normAmostras(NumeroDeAmostras, Amostras);

		// Limpar dados do gráfico
		FmRna->Chart2->Series[0]->Clear();

		for (unsigned int a = 0; a < 512; a++)
		{
			FmRna->Chart2->Series[0]->AddY(Amostras[a]);
            ShowMessage(Amostras[a]);
		}

		FmRna->Chart2->Refresh();
		FmRna->ListBox1Click(FmRna);
	} else
	{
		// Limpar dados do gráfico
		FmRna->amostrasGraf->Series[0]->Clear();

		for (int a = 768; a < 1280; a++)
		{
			// Adicionar ao terceiro grafico
			FmRna->amostrasGraf->Series[0]->AddY(Amostras[a]);
		}
	}
}

float normAmostras(int NumeroDeAmostras, float* Amostras)
{
	// Recortar trecho do vetor de 512 amostras
	float min, max = Amostras[515];

	for ((i = 0); i < 512; i++) {

		if(Amostras[i] < min){
			min = (Amostras[i + 515]);
		}

		if(Amostras[i] > max){
			max = (Amostras[i + 515]);
		}

		p[i] = (Amostras[i + 515]);
	}


	// Para todas as amostras - Normalizar e passar para vetor utilizado pela rede neural
	for (int a = 0; a < 511; a++)
	{
		// Normalizar valores amostras para ficar entre -1 e 1
		// Possíveis formas de normalizar são:
			// Somar um numero a todas as amostrar para: 		Deslocar
			// Multiplicar ou dividir todas as amostras para:   Escalonar
//			if (Amostras != NULL)
//			{
//				Amostras[a] = ((Amostras[a] + 15) * 0.02);
//			}
  // x       - amostra do sinal a ser normalizado.
  // in_min  - Limite inferior do sinal de entrada.
  // in_max  - Limite superior do sinal de entrada.
  // out_min - Limite inferior do sinal de saída (normalizado).
  // out_max - Limite superior do sinal de saída (normalizado).

		 // p[a] = ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
		 // p[a] = (Amostras[a + 768] / 8);
		 if ((min > 0)) {
			 p[a] = (p[a] / max);
		 }else {
//			 p[a] = ((p[a] - min/2) / (max - min));
			p[a] = (p[a] / max);
         }

	}

        // Plota as amostrar - Normalizadas -  no gráfico
//	addAmostraGraph(NumeroDeAmostras, Amostras);
}


//---------------------------------------------------------------------------
__fastcall TFmRna::TFmRna(TComponent* Owner)
	: TForm(Owner)
{

}

//---------------------------------------------------------------------------

void __fastcall TFmRna::FormCreate(TObject *Sender)
{
	// Redimensiona o valor máximo do eixo x com o tamanho da tela desejada.
	Chart2->BottomAxis->Maximum = 250;

	// Expande o gráfico para comportar a quantidade de amostras contidas em max_tela.
	for (unsigned int a = 0; a < 250; a++)
	{
		Chart2->Series[0]->AddY(0);
	}

	// Atualiza o chart.
	Chart2->Refresh();

}
//---------------------------------------------------------------------------


void __fastcall TFmRna::Button1Click(TObject *Sender)
{
	// Limpa as séries do chart para nova plotagem.
	Chart1->Series[0]->Clear();
	Chart1->Series[1]->Clear();

	// Instanciação Declaração da Thread
	MyThread = new Thread(true); 		// cria a thread suspensa, sem executar ainda.
	// MyThread->FreeOnTerminate = True;   // destroi a thread automaticamente depois qu efinalizou.
	MyThread->Priority = tpNormal;  		// configura com prioridade mais baixa que a normal.
	MyThread->Resume();					// faz com que a thread seja executada.
}

//---------------------------------------------------------------------------

void __fastcall TFmRna::Button2Click(TObject *Sender)
{
	// Verificação da instância da thread de atualização do gráfico.
	if (MyThread != NULL)
	{
		MyThread->Terminate();
		Sleep(500);

		delete MyThread;    // se usar (MyThread->FreeOnTerminate = True;) não precisa deletar.
		MyThread = NULL;

		FmRna->Memo1->Lines->Add("finalizou a thread e deletou a instância.");
	}
}

//---------------------------------------------------------------------------

// Declaração da thread de processamento

__fastcall Thread::Thread(bool CreateSuspended)
				: TThread(CreateSuspended)
{
	FmRna->Memo1->Lines->Add("iniciou a thread...");
}

//---------------------------------------------------------------------------

void __fastcall Thread::Execute()
{

    /* initialize random weights: */
    srand (time(NULL));

	padroes = 4;                   // Número de padrões a treinar.
	funcao = 0;                 	// Função Logística(0).
	taxa_aprendizado = 0.001;    	// Taxa de Aprendizado.
	precisao_da_randomizacao = 0.01; // Precisão da randomização (0.1, 0.01, 0.001)
	ERRO = 0.0001;              	// Erro mínimo aceitável da rede (se aplicável).
	MOMENTUM = 0.9;             	// Termo de momentum.
	epocas = 250;            		// Número máximo de épocas de treinamento.
	rnd = 0;                    	// Variável auxiliar para a randomização dos pesos.
	soma = 0;                   	// Variável auxiliar para a soma das sinapses.
	erro_medio_quadratico = 0;  	// Variável auxiliar do Erro médio quadrático.
	erro_quadratico = 0;        	// Variável auxiliar do erro quadrático.
	fim = 0;                    	// Variável de controle do final do treinamento.
	contador = 0;               	// Variável de controle do número de épocas.
	curva = 0.3;


	padroes_validacao = 3; 		// Número de padrões a validar.
	erro_medio_quadratico_validacao = 0;  	// Variável auxiliar do Erro médio quadrático ded validação.
	erro_quadratico_validacao = 0;  // Variável auxiliar do erro quadrático de validação.



	// Zera o vetor de pesos da camada de entrada da rede e da camada 1.
	for (j = 0; j < (cx * c1); j++)
	{
		w1[j] = 0.0;
	}

	// Zera o vetor de pesos da camada 1 e da camada 2.
	for (j = 0; j < (c1 * c2); j++)
	{
        w2[j] = 0.0;
	}

	// Zera os vetores envolvidos aos neurônios da camada 1.
    for (j = 0; j < c1; j++)
    {
        entrada_camada1[j] = 0.0;
        saida_camada1[j] = 0.0;
        erro_camada1[j] = 0.0;
    }

	// Zera os vetores envolvidos aos neurônios da camada 2.
    for (j = 0; j < c2; j++)
    {
        entrada_camada2[j] = 0.0;
        saida_camada2[j] = 0.0;
        erro_camada2[j] = 0.0;
    }

    // Randomização dos pesos para a camada 1.
    for (j = 0; j < (cx*c1); j++)
	{
		// 10000 = pesos 0.001, 1000 = pesos 0.01, 100 = pesos 0.1
		w1[j] = ((float)(random(200) - 100) * precisao_da_randomizacao) / 10;
	}

	// Randomização dos pesos para a camada 2.
	for (j = 0; j < (c1*c2); j++)
	{
		w2[j] = ((float)(random(200) - 100) * precisao_da_randomizacao) / 10;
	}



//-----------------------------------------------------------------------------//
//                 Gravação dos Pesos antes do treinamento                     //
//-----------------------------------------------------------------------------//



    // Declaração do arquivo que armazena os pesos iniciais da rede.
    FILE *fp;
	fp = fopen("pesos_rand.txt","wt");

	// Grava os pesos da camada 1.
	fprintf(fp,"Pesos Camada 1\n");
    for (j = 0; j < (cx * c1); j++)
    {
        fprintf(fp,"%f\n",w1[j]);
    }

    // Grava os pesos da camada 2.
	fprintf(fp,"Pesos Camada 2\n\n");
    for (j = 0; j < (c1 * c2); j++)
    {
        fprintf(fp,"%f\n",w2[j]);
    }

    // Fecha o ponteiro do arquivo.
    fclose(fp);


	// Declaração do arquivo que armazena os error de treinamento.
	fp = fopen("treinamento.txt","wt");

	// Grava os padrões de entrada utilizados no treinamento.
	fprintf(fp,"Padrões de Entrada Utilizados\n");
	for (i = 0; i < padroes; i++)
	{
		for (j = 0; j < cx; j++)
		{
			fprintf(fp,"%10.2f\t",p[j]);
		}
		fprintf(fp,"\n");
	}
	fprintf(fp,"\n");

	// Grava os padrões desejados utilizados no treinamento.
	fprintf(fp,"Padrões Desejados Utilizados\n");
	for (i = 0; i < padroes; i++)
	{
		for (j = 0; j < c2; j++)
		{
			fprintf(fp,"%10.2f\t",d[i][j]);
		}
		fprintf(fp,"\n");
	}
	fprintf(fp,"\n");

	// Informações dos parâmetros utilizados no treinamento.
	fprintf(fp,"Parametros de Configuração da Rede\n");
	fprintf(fp,"Padrões............: %d\n", 	padroes);
	fprintf(fp,"Função utilizada...: %d\n", 	funcao);
	fprintf(fp,"Taxa de Aprendizado: %10.4f\n", taxa_aprendizado);
	fprintf(fp,"Erro mínimo........: %10.4f\n", ERRO);
	fprintf(fp,"Momentum...........: %10.4f\n", MOMENTUM);
	fprintf(fp,"Épocas.............: %d\n\n", 	epocas);
	fprintf(fp,"Épocas de Treinamento - Erro de Propagação\n");


//-----------------------------------------------------------------------------//
//       Inicio do treinamento da rede (propagação dos padrões pela rede)      //
//-----------------------------------------------------------------------------//


	while(contador != epocas && !MyThread->Terminated)
	{
		contador++;

		//FmRna->Memo1->Lines->Add("padrões de treinamento");

		// Propaga os k padrões pela rede.
		for (k = 0; k < padroes; k++)
		{
			//Cálculo para camada C1.
			n = 0;
			for (j = 0; j < c1; j++)
			{
				soma = 0;
				for (i = 0; i < cx; i++)
				{
                    // rba - Fazer
					// Chama a função para ler os arquivos      ---     pode ser a função que o giovani passou
						// Grava temporario no vetor p[] com 2048 valor * apenas 1 linha
						  // a cada epoca chama a função denovo e só sobrescreve o vetor temporario ai ocupa menos memório
					carregarArquivo(i);
					soma += w1[n] * p[i];
					n += 1;
				}
				entrada_camada1[j] = soma;
				saida_camada1[j] = funcao_ativacao(entrada_camada1[j],funcao,curva);
			}

			//Cálculo para camada C2.
			n = 0;
			for (j = 0; j < c2; j++)
			{
				soma = 0;
				for (i = 0; i < c1; i++)
				{
					soma += w2[n] * saida_camada1[i];
					n += 1;
				}
				entrada_camada2[j] = soma;
				saida_camada2[j] = funcao_ativacao(entrada_camada2[j],funcao,curva);
			}



//-----------------------------------------------------------------------------//
//                      Cálculo do Erro Quadrático                             //
//-----------------------------------------------------------------------------//



			//Calculo do Erro Quadratico.
			erro_quadratico = 0;
			for(j = 0; j < c2; j++)
			{
				erro_quadratico += pow((d[k][j] - saida_camada2[j]),2);
			}

			//Acumula o Erro Quadratico (Criterio de Parada).
			erro_medio_quadratico += (0.5 * erro_quadratico);



//-----------------------------------------------------------------------------//
//                  Retropropagação do Erro Pela Rede                          //
//-----------------------------------------------------------------------------//



			//Calculo do erro para cada neurônio da camada 2.
			for (i = 0; i < c2; i++)
			{
				erro_camada2[i] = (d[k][i] - saida_camada2[i]) * derivada(entrada_camada2[i],funcao,curva);
			}

			//Atualizacao dos pesos para camada 2.
			for (i = 0; i < c1; i++)
			{
				n = 0;
				for (j = 0; j < c2; j++)
				{
					dw2[n + i] = taxa_aprendizado * saida_camada1[i] * erro_camada2[j] + (MOMENTUM * dw2[n + i]);
					w2[n + i] = w2[n + i] + dw2[n + i];
					n += c1;
				}
			}

			//Calculo do erro para cada neurônio da camada 1.
			for (i = 0; i < c1; i++)
			{
				n = 0;
				soma = 0;
				for (j = 0; j < c2; j++)
				{
					soma += (erro_camada2[j] * w2[n + i]);
					n += c1;
				}
				erro_camada1[i] = soma * derivada(entrada_camada1[i],funcao,curva);
			}

			//Atualizacao dos pesos para camada 1.
			for (i = 0; i < cx; i++)
			{
				n = 0;
				for (j = 0; j < c1; j++)
				{
					dw1[n + i] = taxa_aprendizado * p[i] * erro_camada1[j] + (MOMENTUM * dw1[n + i]);
					w1[n + i] = w1[n + i] + dw1[n + i];
					n += cx;
				}
			}


			//FmRna->Memo1->Lines->Add(k);


		}


//-----------------------------------------------------------------------------//
//          Cálculo do erro médio quadrático da época de treinamento           //
//-----------------------------------------------------------------------------//


		erro_medio_quadratico = (1.0 / padroes) * erro_medio_quadratico;
		printf("%d\t%10.12f\n", (int) contador, erro_medio_quadratico);
		fprintf(fp,"%d\t%10.12f\n", (int) contador, erro_medio_quadratico);



		// Plotagem dos dados sincronizados com a thread.
		//Synchronize(FmRna->AtualizaGrafico);
		//erro_medio_quadratico = 0;





//-----------------------------------------------------------------------------//
//                      VALIDAÇÃO CRUZADA                                      //
//-----------------------------------------------------------------------------//




		//FmRna->Memo1->Lines->Add("padrões de validação");

		// Propaga os k padrões de validação pela rede.
		for (k = 0; k < padroes_validacao; k++)
		{
			//Cálculo para camada C1.
			n = 0;
			for (j = 0; j < c1; j++)
			{
				soma = 0;
				for (i = 0; i < cx; i++)
				{
					soma += w1[n] * v[k][i];
					n += 1;
				}
				entrada_camada1[j] = soma;
				saida_camada1[j] = funcao_ativacao(entrada_camada1[j],funcao,curva);
			}

			//Cálculo para camada C2.
			n = 0;
			for (j = 0; j < c2; j++)
			{
				soma = 0;
				for (i = 0; i < c1; i++)
				{
					soma += w2[n] * saida_camada1[i];
					n += 1;
				}
				entrada_camada2[j] = soma;
				saida_camada2[j] = funcao_ativacao(entrada_camada2[j],funcao,curva);
			}



	//-----------------------------------------------------------------------------//
	//                      Cálculo do Erro Quadrático de validação                            //
	//-----------------------------------------------------------------------------//



			//Calculo do Erro Quadratico de validação.
			erro_quadratico_validacao = 0;
			for(j = 0; j < c2; j++)
			{
				erro_quadratico_validacao += pow((dv[k][j] - saida_camada2[j]),2);
			}

			//Acumula o Erro Quadratico de validação (Criterio de Parada).
			erro_medio_quadratico_validacao += (0.5 * erro_quadratico_validacao);


			//FmRna->Memo1->Lines->Add(k);

		}


		erro_medio_quadratico_validacao = (1.0 / padroes_validacao) * erro_medio_quadratico_validacao;

		// Plotagem dos dados sincronizados com a thread.
		Synchronize(FmRna->AtualizaGrafico);

		erro_medio_quadratico = 0;
		erro_medio_quadratico_validacao = 0;

	}   // Laço de controle de épocas.

	// Fecha o ponteiro do arquivo de erros de treinamento.
	fclose(fp);


//-----------------------------------------------------------------------------//
//                   Gravação dos Pesos Após treinamento                       //
//-----------------------------------------------------------------------------//


	// Declaração do arquivo que armazena os pesos de treinamento da rede.
	fp = fopen("pesos_treino.txt","wt");

	// Grava os pesos da camada 1.
	fprintf(fp,"\tPesos Camada 1\n");
	for (j = 0; j < (cx * c1); j++)
	{
		fprintf(fp,"\tw1[%d] = %f;\n",j, w1[j]);
	}

	// Grava os pesos da camada 2.
	fprintf(fp,"\n\tPesos Camada 2\n");
	for (j = 0; j < (c1 * c2); j++)
	{
		fprintf(fp,"\tw2[%d] = %f;\n",j, w2[j]);
	}

	// Fecha o ponteiro do arquivo dos pesos de treinamento da rede.
	fclose(fp);

}

//---------------------------------------------------------------------------

void __fastcall TFmRna::AtualizaGrafico()
{
	// Plota as amostras no gráfico.
	//FmRna->Memo1->Lines->Add("Erro treinamento");
	FmRna->Memo1->Lines->Add(FloatToStrF(erro_medio_quadratico,ffFixed,10,6));
	//FmRna->Memo1->Lines->Add("Erro validação");
	//FmRna->Memo1->Lines->Add(FloatToStrF(erro_medio_quadratico_validacao,ffFixed,10,6));

	FmRna->Chart1->Series[0]->AddY(erro_medio_quadratico);
	FmRna->Chart1->Series[1]->AddY(erro_medio_quadratico_validacao);

}

//---------------------------------------------------------------------------


void __fastcall TFmRna::ListBox1Click(TObject *Sender)
{


//-----------------------------------------------------------------------------//
//                   Teste da Rede Neural Após treinamento                     //
//-----------------------------------------------------------------------------//


	//Cálculo para camada C1.
	n = 0;
	for (j = 0; j < c1; j++)
	{
		soma = 0;
		for (i = 0; i < cx; i++)
		{
			soma += w1[n] * p[i];
			n += 1;
		}
		entrada_camada1[j] = soma;
		saida_camada1[j] = funcao_ativacao(entrada_camada1[j],funcao,curva);


		// Formata a saída em binário.
		if (saida_camada1[j] < 0.5)
		{
			saidas_formatadas_c1[j] = 0;
		}else{
			saidas_formatadas_c1[j] = 1;
		}


	}

	//Cálculo para camada C2.
	n = 0;
	for (j = 0; j < c2; j++)
	{
		soma = 0;
		for (i = 0; i < c1; i++)
		{
			soma += w2[n] * saida_camada1[i];
			n += 1;
		}
		entrada_camada2[j] = soma;
		saida_camada2[j] = funcao_ativacao(entrada_camada2[j],funcao,curva);


		// Formata a saída em binário.
		if (saida_camada2[j] < 0.5)
		{
			saidas_formatadas_c2[j] = 0;
		}else{
			saidas_formatadas_c2[j] = 1;
		}

	}


	// Formatação dos neurônios da camada 1

	// rba {Todo rba - Ajustar  condições para dois neuronios 00 01 10 11 por camada de saida}
	// TODO 1  -cImportant :Do
	if (saidas_formatadas_c1[0] > 0.5)
		bit00->Brush->Color = clRed;
	else
		bit00->Brush->Color = clWhite;

	if (saidas_formatadas_c1[1] > 0.5)
		bit01->Brush->Color = clRed;
	else
		bit01->Brush->Color = clWhite;

	if (saidas_formatadas_c1[2] > 0.5)
		bit10->Brush->Color = clRed;
	else
		bit10->Brush->Color = clWhite;

	if (saidas_formatadas_c1[3] > 0.5)
		bit11->Brush->Color = clRed;
	else
		bit11->Brush->Color = clWhite;

}
//---------------------------------------------------------------------------

void __fastcall TFmRna::ListBox2Click(TObject *Sender)
{

	for (unsigned int a = 0; a < 550; a++)
	{
		Chart2->Series[0]->YValues->Value[a] = v[0][a];
	}

	Chart2->Refresh();



//-----------------------------------------------------------------------------//
//                   Teste da Rede Neural Após treinamento                     //
//-----------------------------------------------------------------------------//


	//Cálculo para camada C1.
	n = 0;
	for (j = 0; j < c1; j++)
	{
		soma = 0;
		for (i = 0; i < cx; i++)
		{
			soma += w1[n] * v[0][i];
			n += 1;
		}
		entrada_camada1[j] = soma;
		saida_camada1[j] = funcao_ativacao(entrada_camada1[j],funcao,curva);


		// Formata a saída em binário.
		if (saida_camada1[j] < 0.5)
		{
			saidas_formatadas_c1[j] = 0;
		}else{
			saidas_formatadas_c1[j] = 1;
		}


	}

	//Cálculo para camada C2.
	n = 0;
	for (j = 0; j < c2; j++)
	{
		soma = 0;
		for (i = 0; i < c1; i++)
		{
			soma += w2[n] * saida_camada1[i];
			n += 1;
		}
		entrada_camada2[j] = soma;
		saida_camada2[j] = funcao_ativacao(entrada_camada2[j],funcao,curva);


		// Formata a saída em binário.
		if (saida_camada2[j] < 0.5)
		{
			saidas_formatadas_c2[j] = 0;
		}else{
			saidas_formatadas_c2[j] = 1;
		}

	}
}
//---------------------------------------------------------------------------

void __fastcall TFmRna::btnAbrirArquivoClick(TObject *Sender)
{
//    ProcurarArquivo
    testar = true;
	ProcurarArquivo = new TOpenDialog(this);
	ProcurarArquivo->Title="Open a New File";
	ProcurarArquivo->Execute();

	amostrasGraf->Series[0]->Clear();
	ShowMessage("Arquivo selecionado:  " + ExtractFileName(ProcurarArquivo->FileName));
	lerArquivos("C:/Users/Ninguem/Desktop/Sistemas Inteligentes/RNA2/padroes/" +ExtractFileName(ProcurarArquivo->FileName));
}

void carregarArquivo(int x)
{
	// lerArquivos("C:/Users/Ninguem/Desktop/Sistemas Inteligentes/RNA2/padroes/" +ExtractFileName("EspículaOnda_PacH3_T10-Pz_003407_2048_96ms.pdr"));

	AnsiString seqarquivo[10]  = {"1", "2", "3", "4","5", "6", "7", "8", "9", "10"};
	if (x > 9) {
	        x = 0;
	}
	lerArquivos("C:/Users/Ninguem/Desktop/Sistemas Inteligentes/RNA2/padroesnomemudado/espicula" + seqarquivo[x] + ".pdr");
}
//---------------------------------------------------------------------------


void __fastcall TFmRna::btnNormClick(TObject *Sender)
{
	if (!((NumeroDeAmostras > 0) and (padroes > 0)))
	{
        ShowMessage("Primeiro deve carregar um arquivo e treinar rede");
        return;
	}
	normAmostras(NumeroDeAmostras, Amostras);
}
//---------------------------------------------------------------------------


