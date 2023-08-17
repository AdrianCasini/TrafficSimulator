#ifndef _INTERFAZHOST
#define _INTERFAZHOST

#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <ctype.h>

#include <openssl/rsa.h>
#include <openssl/rand.h>
#include <openssl/des.h>

/**
 * <PRE>
 *
 *     SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
 *     --------------------------------------------------------------
 *
 * FICHERO: interfazhost.h
 *
 * DESCRIPCION:
 *
 *
 * FECHA CREACION: 13-11-2007
 *
 * AUTOR DISENYO:
 *
 * PROGRAMADOR: Toni Bassas Serramia
 *
 * MODIFICATIONES:
 *
 *    |   FECHA    |   AUTOR   |  RAZON                                   |
 *    ---------------------------------------------------------------------
 *    | DD-MM-YYYY |           |                                          |
 *    ---------------------------------------------------------------------
 * </PRE>
*/

#define CRYPTO_IFZ_HOST_FILE_NUM_KEYS          100

#define CRYPTO_IFZ_HOST_OK                       0
#define CRYPTO_IFZ_HOST_ERROR                    1

#define CRYPTO_IFZ_HOST_ERROR_FILE_EXISTS        2
#define CRYPTO_IFZ_HOST_ERROR_FILE_CREATE        3
#define CRYPTO_IFZ_HOST_ERROR_FILE_SEED          4

#define CRYPTO_IFZ_HOST_ERROR_FILE_NOEXISTS      2
#define CRYPTO_IFZ_HOST_ERROR_FILE_OPEN          3
#define CRYPTO_IFZ_HOST_ERROR_KEY_INDEX          4

#define CRYPTO_IFZ_HOST_ERROR_TDES               5
#define CRYPTO_IFZ_HOST_ERROR_PAN                6

#define CRYPTO_IFZ_HOST_MAX_SEED_SIZE           511

#define _TEST_CRYPTO_IFZ_HOST

struct _ifz_host_key {
   unsigned char m[64];
   unsigned char e[64];
   unsigned char d[64];
   int szm;
   int sze;
   int szd;
};

#define CRYPTO_IFZ_HOST_FILE_SIZE sizeof(struct _ifz_host_key)*CRYPTO_IFZ_HOST_FILE_NUM_KEYS

int CryptoStarixGenerarFicheroClaves(char *pathFicheroClaves,char *semilla);
int CryptoStarixObtenerClaveSEGU(char *pathFicheroClaves,int index,char *buffer,int szbuffer);
int CryptoStarixDesencriptarPAN(char *pathFicheroClaves,int index,char *cryptedTDES,char *cryptedPAN,char *PAN);

void _CryptoStarixBinToHex(char *line,unsigned char *pbin,int szbin);

#ifdef _TEST_CRYPTO_IFZ_HOST

int CryptoStarixEncriptarTDES(char *pathFicheroClaves,int index,unsigned char *claveTDES,char *buffer,int szbuffer);
int CryptoStarixEncriptarPAN(unsigned char *claveTDES,char *PAN,char *buffer,int szbuffer);

#endif

#endif
