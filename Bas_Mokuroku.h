//---------------------------------------------------------------------------

#ifndef Bas_MokurokuH
#define Bas_MokurokuH
//---------------------------------------------------------------------------
#endif

extern bool __fastcall Mokuroku(HeadsData* pcHeadsData);          // �}�ʖژ^
extern void __fastcall SyuturyokuZumen_Set_S(AnsiString s_UtiKbn, bool bMarine, HeadsData* pcHeadsData);
extern AnsiString __fastcall spc(int iNum);

// 2007.11.15 �~���[�d�l�Ή� �@�햼�̎擾�֐�
extern AnsiString __fastcall GetEquipmentName(AnsiString s_NameOrg, AnsiString sSpecCode);
// 2007.11.15

//extern void __fastcall SyuturyokuZumen_Set_S(char Pattern, AnsiString Lang);

// 2020.07.14 �ی�J�o�[���L�ǉ�_S
extern bool __fastcall Tokki_Code(AnsiString Key, AnsiString s_HEADS_DATA[], int kazu);
// 2020.07.14 �ی�J�o�[���L�ǉ�_E

// 2022.04.05 �A�q�D���ǉ�_S
extern AnsiString __fastcall Tokki_Zen(AnsiString Key, AnsiString s_HEADS_DATA[], int kazu);
// 2022.04.05 �A�q�D���ǉ�_E

