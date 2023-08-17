
int CryptoStarixGenerarFicheroClaves(char *pathFicheroClaves,char *semilla);
int CryptoStarixObtenerClaveSEGU(char *pathFicheroClaves,int index,char *buffer,int szbuffer);
int CryptoStarixDesencriptarPAN(char *pathFicheroClaves,int index,char *cryptedTDES,char *cryptedPAN,char *PAN);

int CryptoStarixEncriptarTDES(char *pathFicheroClaves,int index,unsigned char *claveTDES,char *buffer,int szbuffer);
int CryptoStarixEncriptarPAN(unsigned char *claveTDES,char *PAN,char *buffer,int szbuffer);

