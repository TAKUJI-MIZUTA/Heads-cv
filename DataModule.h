// ---------------------------------------------------------------------------

#ifndef DataModuleH
#define DataModuleH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
// 2021.06.18 FireDACへの移行 MOD_S
//#include <Db.hpp>
//#include <DBTables.hpp>
#include <DB.hpp>
#include <FireDAC.Comp.Client.hpp>
#include <FireDAC.Comp.DataSet.hpp>
#include <FireDAC.Comp.UI.hpp>
#include <FireDAC.DApt.hpp>
#include <FireDAC.DApt.Intf.hpp>
#include <FireDAC.DatS.hpp>
#include <FireDAC.Phys.hpp>
#include <FireDAC.Phys.Intf.hpp>
#include <FireDAC.Phys.MSAcc.hpp>
#include <FireDAC.Phys.MSAccDef.hpp>
#include <FireDAC.Phys.ODBCBase.hpp>
#include <FireDAC.Stan.Async.hpp>
#include <FireDAC.Stan.Def.hpp>
#include <FireDAC.Stan.Error.hpp>
#include <FireDAC.Stan.Intf.hpp>
#include <FireDAC.Stan.Option.hpp>
#include <FireDAC.Stan.Param.hpp>
#include <FireDAC.Stan.Pool.hpp>
#include <FireDAC.UI.Intf.hpp>
#include <FireDAC.VCLUI.Wait.hpp>
// 2021.06.18 FireDACへの移行 MOD_E

// ---------------------------------------------------------------------------
class TDataModule1 : public TDataModule {
__published: // IDE 管理のコンポーネント

	// 2021.06.18 FireDACへの移行 MOD_S
	//TDatabase *Database_DNO;
	//TQuery *Query_MAT;
    //TDatabase *Database_MAT;
	//TDatabase *Database_INS;
	TFDConnection *Database_DNO;
	TFDQuery *Query_MAT;
	TFDConnection *Database_MAT;
	TFDConnection *Database_INS;
	TFDManager *FDManagerDN;
	TFDGUIxWaitCursor *FDGUIxWaitCursor1;
	TFDPhysMSAccessDriverLink *FDPhysMSAccessDriverLink1;
	// 2021.06.18 FireDACへの移行 MOD_E

private: // ユーザー宣言
        public : // ユーザー宣言

    __fastcall TDataModule1(TComponent* Owner);
};

// ---------------------------------------------------------------------------
extern PACKAGE TDataModule1 *DataModule1;
// ---------------------------------------------------------------------------
#endif
