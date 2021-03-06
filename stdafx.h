// stdafx.h�: fichier Include pour les fichiers Include syst�me standard,
// ou les fichiers Include sp�cifiques aux projets qui sont utilis�s fr�quemment,
// et sont rarement modifi�s
//

#pragma once

#include "targetver.h"
#include "DatabaseRecordSet.h"
#define WIN32_LEAN_AND_MEAN             // Exclure les en-t�tes Windows rarement utilis�s
// Fichiers d'en-t�te Windows�:
#include <windows.h>

// Fichiers d'en-t�te C RunTime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "GestionComptableApp.h" 
__declspec(dllimport) void Initialize(char * szIP);
__declspec(dllimport) int ConnectDataBase(wchar_t * ConnectionString);
__declspec(dllimport)  int SQLRequest(int ClientNumber, wchar_t * Request, CDataBaseRecordSet*pRecordSet);

// TODO: faites r�f�rence ici aux en-t�tes suppl�mentaires n�cessaires au programme
