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
    // Fun��o Log�stica
    /*
                    1
      y(n) = ---------------
             1 + exp(-a.net)
    */

        return( 1.0 / (1.0 + exp(-a * net)) );

    }
    else
    {
    // Fun��o Tangente Hiperb�lica
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
	// Derivada da Fun��o Log�stica
    /*
                    1                       1
      y(n) = --------------- * ( 1 - --------------- )
			 1 - exp(-a.net)         1 - exp(-a.net)
     */

        return( (1.0 / (1.0 + exp(-a * net))) * (1.0 - (1.0 / (1.0 + exp(-a * net)))) );

    }
    else
    {
    // Derivada da Fun��o Tangente Hiperb�lica
    /*
                   exp(a.net) - exp(-a.net)
      y(n) = 1 - ( ------------------------ )�
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
const int c1 = 1;          // Camada Intermedi�ria.
const int c2 = 2;           // Camada de Sa�da. /// 4 Op��es 2 bits 00 01 10 11

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

 // Padr�es de Entrada da Rede.
float p[520] = {0};

// Valores desejaos dos padr�es ao final do treinamento.
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
// Valores desejados dos padr�es ao final do treinamento.
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


//M�todo de abertura dos arquivos de eventos.
void lerArquivos(AnsiString ArquivoDeEventos)
{
  //Declara��o do Ponteiro do Arquivo de Eventos.
  FILE *PtArquivoDeEventos;
  const char *recebe_string;

  //Declara��o da vari�vel de status de abertura do arquivo.
  bool status = false;

  //Verifica se o arquivo existe antes de abrir.
  if (FileExists(ArquivoDeEventos))
  {
	// ShowMessage("Encontrou e conseguiu abrir o arquivo dentro do metodo");
	//Abertura do Arquivo de Padr�es.
	FILE *arq_treinamento;

	int contNum = 0;

	// AnsiString APath = "padroes/"+name[i]+".txt";
	AnsiString APath = ArquivoDeEventos;
	PtArquivoDeEventos = fopen(APath.c_str() ,"rt");

	//N�mero de Amostras do Evento.
	fscanf(PtArquivoDeEventos, "%d\n", &NumeroDeAmostras);

	//Dura��o do Evento.
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

	//Fecha o Ponteiro do Arquivo de Padr�es.
	fclose(PtArquivoDeEventos);

	//Sinaliza se o arquivo foi aberto corretamente.
	status = true;

    // Plota as amostrar no gr�fico
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

		// Limpar dados do gr�fico
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
		// Limpar dados do gr�fico
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
		// Poss�veis formas de normalizar s�o:
			// Somar um numero a todas as amostrar para: 		Deslocar
			// Multiplicar ou dividir todas as amostras para:   Escalonar
//			if (Amostras != NULL)
//			{
//				Amostras[a] = ((Amostras[a] + 15) * 0.02);
//			}
  // x       - amostra do sinal a ser normalizado.
  // in_min  - Limite inferior do sinal de entrada.
  // in_max  - Limite superior do sinal de entrada.
  // out_min - Limite inferior do sinal de sa�da (normalizado).
  // out_max - Limite superior do sinal de sa�da (normalizado).

		 // p[a] = ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
		 // p[a] = (Amostras[a + 768] / 8);
		 if ((min > 0)) {
			 p[a] = (p[a] / max);
		 }else {
//			 p[a] = ((p[a] - min/2) / (max - min));
			p[a] = (p[a] / max);
         }

	}

        // Plota as amostrar - Normalizadas -  no gr�fico
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
	// Redimensiona o valor m�ximo do eixo x com o tamanho da tela desejada.
	Chart2->BottomAxis->Maximum = 250;

	// Expande o gr�fico para comportar a quantidade de amostras contidas em max_tela.
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
	// Limpa as s�ries do chart para nova plotagem.
	Chart1->Series[0]->Clear();
	Chart1->Series[1]->Clear();

	// Instancia��o Declara��o da Thread
	MyThread = new Thread(true); 		// cria a thread suspensa, sem executar ainda.
	// MyThread->FreeOnTerminate = True;   // destroi a thread automaticamente depois qu efinalizou.
	MyThread->Priority = tpNormal;  		// configura com prioridade mais baixa que a normal.
	MyThread->Resume();					// faz com que a thread seja executada.
}

//---------------------------------------------------------------------------

void __fastcall TFmRna::Button2Click(TObject *Sender)
{
	// Verifica��o da inst�ncia da thread de atualiza��o do gr�fico.
	if (MyThread != NULL)
	{
		MyThread->Terminate();
		Sleep(500);

		delete MyThread;    // se usar (MyThread->FreeOnTerminate = True;) n�o precisa deletar.
		MyThread = NULL;

		FmRna->Memo1->Lines->Add("finalizou a thread e deletou a inst�ncia.");
	}
}

//---------------------------------------------------------------------------

// Declara��o da thread de processamento

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

	padroes = 4;                   // N�mero de padr�es a treinar.
	funcao = 0;                 	// Fun��o Log�stica(0).
	taxa_aprendizado = 0.001;    	// Taxa de Aprendizado.
	precisao_da_randomizacao = 0.01; // Precis�o da randomiza��o (0.1, 0.01, 0.001)
	ERRO = 0.0001;              	// Erro m�nimo aceit�vel da rede (se aplic�vel).
	MOMENTUM = 0.9;             	// Termo de momentum.
	epocas = 250;            		// N�mero m�ximo de �pocas de treinamento.
	rnd = 0;                    	// Vari�vel auxiliar para a randomiza��o dos pesos.
	soma = 0;                   	// Vari�vel auxiliar para a soma das sinapses.
	erro_medio_quadratico = 0;  	// Vari�vel auxiliar do Erro m�dio quadr�tico.
	erro_quadratico = 0;        	// Vari�vel auxiliar do erro quadr�tico.
	fim = 0;                    	// Vari�vel de controle do final do treinamento.
	contador = 0;               	// Vari�vel de controle do n�mero de �pocas.
	curva = 0.3;


	padroes_validacao = 3; 		// N�mero de padr�es a validar.
	erro_medio_quadratico_validacao = 0;  	// Vari�vel auxiliar do Erro m�dio quadr�tico ded valida��o.
	erro_quadratico_validacao = 0;  // Vari�vel auxiliar do erro quadr�tico de valida��o.



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

	// Zera os vetores envolvidos aos neur�nios da camada 1.
    for (j = 0; j < c1; j++)
    {
        entrada_camada1[j] = 0.0;
        saida_camada1[j] = 0.0;
        erro_camada1[j] = 0.0;
    }

	// Zera os vetores envolvidos aos neur�nios da camada 2.
    for (j = 0; j < c2; j++)
    {
        entrada_camada2[j] = 0.0;
        saida_camada2[j] = 0.0;
        erro_camada2[j] = 0.0;
    }

    // Randomiza��o dos pesos para a camada 1.
    for (j = 0; j < (cx*c1); j++)
	{
		// 10000 = pesos 0.001, 1000 = pesos 0.01, 100 = pesos 0.1
		w1[j] = ((float)(random(200) - 100) * precisao_da_randomizacao) / 10;
	}

	// Randomiza��o dos pesos para a camada 2.
	for (j = 0; j < (c1*c2); j++)
	{
		w2[j] = ((float)(random(200) - 100) * precisao_da_randomizacao) / 10;
	}



//-----------------------------------------------------------------------------//
//                 Grava��o dos Pesos antes do treinamento                     //
//-----------------------------------------------------------------------------//



    // Declara��o do arquivo que armazena os pesos iniciais da rede.
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


	// Declara��o do arquivo que armazena os error de treinamento.
	fp = fopen("treinamento.txt","wt");

	// Grava os padr�es de entrada utilizados no treinamento.
	fprintf(fp,"Padr�es de Entrada Utilizados\n");
	for (i = 0; i < padroes; i++)
	{
		for (j = 0; j < cx; j++)
		{
			fprintf(fp,"%10.2f\t",p[j]);
		}
		fprintf(fp,"\n");
	}
	fprintf(fp,"\n");

	// Grava os padr�es desejados utilizados no treinamento.
	fprintf(fp,"Padr�es Desejados Utilizados\n");
	for (i = 0; i < padroes; i++)
	{
		for (j = 0; j < c2; j++)
		{
			fprintf(fp,"%10.2f\t",d[i][j]);
		}
		fprintf(fp,"\n");
	}
	fprintf(fp,"\n");

	// Informa��es dos par�metros utilizados no treinamento.
	fprintf(fp,"Parametros de Configura��o da Rede\n");
	fprintf(fp,"Padr�es............: %d\n", 	padroes);
	fprintf(fp,"Fun��o utilizada...: %d\n", 	funcao);
	fprintf(fp,"Taxa de Aprendizado: %10.4f\n", taxa_aprendizado);
	fprintf(fp,"Erro m�nimo........: %10.4f\n", ERRO);
	fprintf(fp,"Momentum...........: %10.4f\n", MOMENTUM);
	fprintf(fp,"�pocas.............: %d\n\n", 	epocas);
	fprintf(fp,"�pocas de Treinamento - Erro de Propaga��o\n");


//-----------------------------------------------------------------------------//
//       Inicio do treinamento da rede (propaga��o dos padr�es pela rede)      //
//-----------------------------------------------------------------------------//


	while(contador != epocas && !MyThread->Terminated)
	{
		contador++;

		//FmRna->Memo1->Lines->Add("padr�es de treinamento");

		// Propaga os k padr�es pela rede.
		for (k = 0; k < padroes; k++)
		{
			//C�lculo para camada C1.
			n = 0;
			for (j = 0; j < c1; j++)
			{
				soma = 0;
				for (i = 0; i < cx; i++)
				{
                    // rba - Fazer
					// Chama a fun��o para ler os arquivos      ---     pode ser a fun��o que o giovani passou
						// Grava temporario no vetor p[] com 2048 valor * apenas 1 linha
						  // a cada epoca chama a fun��o denovo e s� sobrescreve o vetor temporario ai ocupa menos mem�rio
					carregarArquivo(i);
					soma += w1[n] * p[i];
					n += 1;
				}
				entrada_camada1[j] = soma;
				saida_camada1[j] = funcao_ativacao(entrada_camada1[j],funcao,curva);
			}

			//C�lculo para camada C2.
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
//                      C�lculo do Erro Quadr�tico                             //
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
//                  Retropropaga��o do Erro Pela Rede                          //
//-----------------------------------------------------------------------------//



			//Calculo do erro para cada neur�nio da camada 2.
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

			//Calculo do erro para cada neur�nio da camada 1.
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
//          C�lculo do erro m�dio quadr�tico da �poca de treinamento           //
//-----------------------------------------------------------------------------//


		erro_medio_quadratico = (1.0 / padroes) * erro_medio_quadratico;
		printf("%d\t%10.12f\n", (int) contador, erro_medio_quadratico);
		fprintf(fp,"%d\t%10.12f\n", (int) contador, erro_medio_quadratico);



		// Plotagem dos dados sincronizados com a thread.
		//Synchronize(FmRna->AtualizaGrafico);
		//erro_medio_quadratico = 0;





//-----------------------------------------------------------------------------//
//                      VALIDA��O CRUZADA                                      //
//-----------------------------------------------------------------------------//




		//FmRna->Memo1->Lines->Add("padr�es de valida��o");

		// Propaga os k padr�es de valida��o pela rede.
		for (k = 0; k < padroes_validacao; k++)
		{
			//C�lculo para camada C1.
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

			//C�lculo para camada C2.
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
	//                      C�lculo do Erro Quadr�tico de valida��o                            //
	//-----------------------------------------------------------------------------//



			//Calculo do Erro Quadratico de valida��o.
			erro_quadratico_validacao = 0;
			for(j = 0; j < c2; j++)
			{
				erro_quadratico_validacao += pow((dv[k][j] - saida_camada2[j]),2);
			}

			//Acumula o Erro Quadratico de valida��o (Criterio de Parada).
			erro_medio_quadratico_validacao += (0.5 * erro_quadratico_validacao);


			//FmRna->Memo1->Lines->Add(k);

		}


		erro_medio_quadratico_validacao = (1.0 / padroes_validacao) * erro_medio_quadratico_validacao;

		// Plotagem dos dados sincronizados com a thread.
		Synchronize(FmRna->AtualizaGrafico);

		erro_medio_quadratico = 0;
		erro_medio_quadratico_validacao = 0;

	}   // La�o de controle de �pocas.

	// Fecha o ponteiro do arquivo de erros de treinamento.
	fclose(fp);


//-----------------------------------------------------------------------------//
//                   Grava��o dos Pesos Ap�s treinamento                       //
//-----------------------------------------------------------------------------//


	// Declara��o do arquivo que armazena os pesos de treinamento da rede.
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
	// Plota as amostras no gr�fico.
	//FmRna->Memo1->Lines->Add("Erro treinamento");
	FmRna->Memo1->Lines->Add(FloatToStrF(erro_medio_quadratico,ffFixed,10,6));
	//FmRna->Memo1->Lines->Add("Erro valida��o");
	//FmRna->Memo1->Lines->Add(FloatToStrF(erro_medio_quadratico_validacao,ffFixed,10,6));

	FmRna->Chart1->Series[0]->AddY(erro_medio_quadratico);
	FmRna->Chart1->Series[1]->AddY(erro_medio_quadratico_validacao);

}

//---------------------------------------------------------------------------


void __fastcall TFmRna::ListBox1Click(TObject *Sender)
{


//-----------------------------------------------------------------------------//
//                   Teste da Rede Neural Ap�s treinamento                     //
//-----------------------------------------------------------------------------//


	//C�lculo para camada C1.
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


		// Formata a sa�da em bin�rio.
		if (saida_camada1[j] < 0.5)
		{
			saidas_formatadas_c1[j] = 0;
		}else{
			saidas_formatadas_c1[j] = 1;
		}


	}

	//C�lculo para camada C2.
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


		// Formata a sa�da em bin�rio.
		if (saida_camada2[j] < 0.5)
		{
			saidas_formatadas_c2[j] = 0;
		}else{
			saidas_formatadas_c2[j] = 1;
		}

	}


	// Formata��o dos neur�nios da camada 1

	// rba {Todo rba - Ajustar  condi��es para dois neuronios 00 01 10 11 por camada de saida}
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
//                   Teste da Rede Neural Ap�s treinamento                     //
//-----------------------------------------------------------------------------//


	//C�lculo para camada C1.
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


		// Formata a sa�da em bin�rio.
		if (saida_camada1[j] < 0.5)
		{
			saidas_formatadas_c1[j] = 0;
		}else{
			saidas_formatadas_c1[j] = 1;
		}


	}

	//C�lculo para camada C2.
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


		// Formata a sa�da em bin�rio.
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
	// lerArquivos("C:/Users/Ninguem/Desktop/Sistemas Inteligentes/RNA2/padroes/" +ExtractFileName("Esp�culaOnda_PacH3_T10-Pz_003407_2048_96ms.pdr"));

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


