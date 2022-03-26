//---------------------------------------------------------------------------

#ifndef URnaH
#define URnaH

//---------------------------------------------------------------------------

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Dialogs.hpp>

//---------------------------------------------------------------------------

class TFmRna : public TForm
{
	__published:	// IDE-managed Components
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TChart *Chart1;
	TFastLineSeries *Series1;
	TButton *Button1;
	TButton *Button2;
	TTabSheet *TabSheet2;
	TChart *Chart2;
	TFastLineSeries *Series2;
	TMemo *Memo1;
	TShape *bit00;
	TShape *bit01;
	TShape *bit10;
	TShape *bit11;
	TLabel *Label2;
	TFastLineSeries *Series3;
	TFastLineSeries *Series4;
	TPanel *Panel1;
	TOpenDialog *ProcurarArquivo;
	TLabel *Label1;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TButton *btnAbrirArquivo;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall AtualizaGrafico();
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ListBox1Click(TObject *Sender);
	void __fastcall ListBox2Click(TObject *Sender);
	void __fastcall btnAbrirArquivoClick(TObject *Sender);
	void __fastcall btnNormClick(TObject *Sender);


	private:	// User declarations
	public:		// User declarations
		__fastcall TFmRna(TComponent* Owner);
};

//---------------------------------------------------------------------------

class Thread : public TThread
{
	private:
	protected:
		void __fastcall Execute();

	public:
		__fastcall Thread(bool CreateSuspended);
};

//---------------------------------------------------------------------------
extern PACKAGE TFmRna *FmRna;
//---------------------------------------------------------------------------
#endif
