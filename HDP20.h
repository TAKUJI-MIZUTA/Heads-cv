// ---------------------------------------------------------------------------

#ifndef HDP20H
#define HDP20H
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include <Filectrl.hpp>

// ---------------------------------------------------------------------------
class TfrmHDP020 : public TForm {
__published: // IDE �Ǘ��̃R���|�[�l���g
    void __fastcall FormCreate(TObject *Sender);

private: // ���[�U�[�錾
    void __fastcall Heads_Parts_Main(void);
    bool __fastcall Log_File_SizeCheck(void); // ���O�t�@�C���@�T�C�Y�`�F�b�N
    bool __fastcall Connect_DataBase(void); // �ް��ް��ڑ�����
    bool __fastcall Get_Sakuban(void); // ��Ԏ擾
    bool __fastcall OutPut_Folder_Check(AnsiString Check_Pass); // �o�͐�̫�������
    bool __fastcall Read_CodeMasta_Data(void); // �K�i����,�ƊE����,�h���F �Ǎ�

    // EXCEL�p
    short __fastcall CallExcel(bool showflg); // Excel�N������
    void __fastcall QuitExcel(void);

    int prvFrmLoadFlg;

    AnsiString P_SQLText;
    AnsiString P_Sakuban_Mode; // ��Ԏ��(S:�����ԍ��CM:���ϔԍ�)
    AnsiString P_UtiKbn; // �ŏo���敪

public: // ���[�U�[�錾

    __fastcall TfrmHDP020(TComponent* Owner);
};

// ---------------------------------------------------------------------------
extern PACKAGE TfrmHDP020 *frmHDP020;
// ---------------------------------------------------------------------------
#endif
