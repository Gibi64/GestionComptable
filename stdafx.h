// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

#pragma once

#include "targetver.h"
#include "DatabaseRecordSet.h"
#define WIN32_LEAN_AND_MEAN             // Exclure les en-têtes Windows rarement utilisés
// Fichiers d'en-tête Windows :
#include <windows.h>

// Fichiers d'en-tête C RunTime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "GestionComptableApp.h" 
__declspec(dllimport) void Initialize(char * szIP);
__declspec(dllimport) int ConnectDataBase(wchar_t * ConnectionString);
__declspec(dllimport)  int SQLRequest(int ClientNumber, wchar_t * Request, CDataBaseRecordSet*pRecordSet);

// TODO: faites référence ici aux en-têtes supplémentaires nécessaires au programme
