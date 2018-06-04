#include "StdAfx.h"
#include <dbosnap2.h>
#include <dbobjptr2.h>
#include <dbindex.h>
#include <dbboiler.h>
#include <dbents.h>
#include <core_rxmfcapi.h>
#include <acedCmdNF.h>
#include <dbloftedsurf.h>
#include <dbSubD.h>
#include <rxmfcapi.h>
#include <dbViewRepBlockReference.h>
#include <dbViewBorder.h>
#include <AdApplicationFrame.h>
#include <acedads.h>
#include <acdbimageutils.h>
#include <Unknwn.h>    
#include <windows.h>
#include <gdiplus.h>
#include <AcDbPointCloudDefEx.h>
#include <AcDbPointCloudEx.h>
#include <AcPointCloudExtractedCylinder.h>
#include <dynprops.h>
#include <brent.h>
#include <brbrep.h>
#include <memory>
#include "eoktest.h"
#include "AcDbAssocPersSubentIdPE.h"


using namespace Gdiplus;

#pragma comment (lib,"Gdiplus.lib")

#define CALC_EPSILON  10E-5
static int YORN = 0;
#define M_PI       3.14159265358979323846   // pi
#define eOkDialog(arg) \
((arg == Acad::eOk) ? true : Mutter(__FILE__, __LINE__, arg))

//-----------------Markus Kraus----------------//
inline bool Mutter(char *file, int line, Acad::ErrorStatus arg)
	{
	ACHAR buf[1024];
	wsprintf(buf, _T("!%s@%d: %s"), file, line, acadErrorStatusText(arg));
	acedAlert(buf);
	return false;
	}

// Utility functions
////////////////////////////////////////////////////////////////////////////////
using namespace std;

static void closeOrDeleteDbObj(AcDbObject *pObj) { if (pObj->objectId().isNull()) delete pObj; else pObj->close(); }
template <class T> struct unique_db_ptr : public unique_ptr<T, void(*)(AcDbObject*)>
{
	unique_db_ptr<T>(T*& t) : unique_ptr<T, void(*)(AcDbObject*)>(t, closeOrDeleteDbObj) { }
};


static bool
addToDb(AcDbEntity *pEnt, AcDbDatabase *pDb)
{
	unique_db_ptr<AcDbEntity> ent(pEnt);
	AcDbBlockTable *pBt;
	if (eOk != pDb->getBlockTable(pBt, kForRead)) return false;
	unique_db_ptr<AcDbBlockTable> bt(pBt);
	AcDbBlockTableRecord *pMs;
	if (eOk != pBt->getAt(ACDB_MODEL_SPACE, pMs, kForWrite)) return false;
	return eOk == unique_db_ptr<AcDbBlockTableRecord>(pMs)->appendAcDbEntity(ent.get());
}
//-----------------------//

static void usage_test()
{
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();
	if (nullptr == pDb) return;

	AcDbLinetypeTable *pLtt;
	if (eOk != pDb->getLinetypeTable(pLtt, kForRead)) return;
	unique_db_ptr<AcDbLinetypeTable> ltt(pLtt);

	AcDbObjectId lttId;
	if (eOk != ltt->getAt(L"2|GAS_LINE", lttId)) return;

	acutPrintf(L"DWG database = <%x>\nlttId database = <%x>\n",
		pDb, lttId.database());

	AcDbLayerTable *pLt;
	if (eOk != pDb->getLayerTable(pLt, kForRead)) return;
	unique_db_ptr<AcDbLayerTable> lt(pLt);

	AcDbLayerTableRecord *pLtr;
	if (eOk != pLt->getAt(L"2|2_LayerA", pLtr, kForWrite)) return;
	unique_db_ptr<AcDbLayerTableRecord> ltr(pLtr);

	acutPrintf(L"LTR database: <%x>\n", ltr->database());
	ltr->setLinetypeObjectId(lttId);
}

void solidFillet()
{
	//select a Solid entity

	ads_point pt;
	ads_name entName;
	if (RTNORM != acedEntSel(_T(""), entName, pt)) return;
	AcDbObjectId entId = AcDbObjectId::kNull;
	if (!eOkVerify(acdbGetObjectId(entId, entName))) return;
	AcDbObjectPointer <AcDb3dSolid> pEntity(entId, AcDb::kForWrite);
	if (!eOkVerify(pEntity.openStatus())) return;

	//AcDbSubentId* edgeId = new AcDbSubentId(AcDb::kNullSubentType,)
	// Get the Protocol extension associated with the
	// cube .
		// This will be used to fetch the edge information.
	AcDbAssocPersSubentIdPE* const pAssocPerSubId= AcDbAssocPersSubentIdPE::cast(pEntity->queryX(AcDbAssocPersSubentIdPE::desc()));
	AcArray<AcDbSubentId> edgeIds;
	AcArray<AcDbSubentId*> pEdgeIds;
	pAssocPerSubId->getAllSubentities(pEntity,AcDb::kEdgeSubentType, edgeIds);
	AcGeDoubleArray raidus;
	AcGeDoubleArray startSetBack, endSetBack;
	double setBackvalue = 5.0;
	for (auto sid : edgeIds)
	{
		raidus.append(setBackvalue);
		startSetBack.append(setBackvalue);
		endSetBack.append(setBackvalue);
		AcDbSubentId* psubEntId = new AcDbSubentId(sid);
		pEdgeIds.append(psubEntId);
	}

	if(!eOkVerify(pEntity->filletEdges(pEdgeIds, raidus, startSetBack, endSetBack))) return;
	//House cleaning
	if (pEdgeIds.physicalLength() > 0)
	{
		for (auto pid : pEdgeIds) {
			delete pid;
			pid = nullptr;
		}
	}
}





//**************************************************************
extern "C" AcRx::AppRetCode acrxEntryPoint(AcRx::AppMsgCode msg, void *pkt)
{
	switch(msg)
	{
		case AcRx::kInitAppMsg:
			acrxDynamicLinker->unlockApplication(pkt);
			acrxDynamicLinker->registerAppMDIAware(pkt);
			/*Load commands*/
			acrxBuildClassHierarchy();/*use this for derived classes*/					
			acedRegCmds->addCommand(_T("TestCmd"), _T("TSOLFILLET"), _T("TSOLFILLET"), ACRX_CMD_MODAL, solidFillet);
			
			break;
		case AcRx::kUnloadAppMsg:
			/*unload commands*/
			acedRegCmds->removeGroup(_T("TestCmd"));

			break;
		default:
			break;
	}
	return AcRx::kRetOK;
}
