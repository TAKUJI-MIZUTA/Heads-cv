//---------------------------------------------------------------------------

#ifndef Bas_SeisekisyoH
#define Bas_SeisekisyoH
//---------------------------------------------------------------------------
#endif

extern bool __fastcall Seisekisyo(HeadsData* pcHeadsData);          // �������я��쐬
// 2022.04.22 �ψ��ێ����Ԏw��ǉ�_S
extern bool __fastcall Tokki_Code(AnsiString Key, AnsiString s_HEADS_DATA[], int kazu);
extern AnsiString __fastcall Tokki_Zen(AnsiString Key, AnsiString s_HEADS_DATA[], int kazu);
// 2022.04.22 �ψ��ێ����Ԏw��ǉ�_E

void __fastcall Get_Nossle_Value(AnsiString NossleNM1, AnsiString NossleNM2, AnsiString Tugite);

static AnsiString P_Nossle_Top;
static AnsiString P_Nossle_Bottom;




