
#include "Sistema.h"
#include "util/FicheroDatos.h"


float Sistema::getPorcentajeDiscoLibre(char *PathFile) {
   struct statvfs buf;

   float pfree;

   if (statvfs((char *) getenv(PathFile),&buf)==0) {

      pfree=buf.f_bfree;
      pfree/=buf.f_blocks;

      return (pfree*100);

   }

   return -1;

}

float Sistema::getPorcentajeDiscoUsado(char *PathFile) {
   return(contrario(getPorcentajeDiscoLibre(PathFile)));
}

float Sistema::getPorcentajeMemoriaLibre(void) {
#warning TODO: implementar en Solaris
   return -1.0f;
}

float Sistema::getPorcentajeMemoriaUsada(void) {
   return(contrario(getPorcentajeMemoriaLibre()));
}

float Sistema::getPorcentajeSwapLibre(void) {
#warning TODO: implementar en Solaris
   return -1.0f;
}

float Sistema::getPorcentajeSwapUsado(void) {
   return(contrario(getPorcentajeSwapLibre()));
}

bool Sistema::getCargaSistema(float *f1min,float *f5min,float *f15min)
{
#warning TODO: implementar en Solaris
// d_type==4 !!
// buscar equivalente S_ISREG...

//   n = scandir("/proc", &namelist, 0, 0);
//   if (n>0) {
//      for(i=0;i<n;i++) {
//         if (namelist[i]->d_type==4) {
//            if (atoi(namelist[i]->d_name)!=0) {
//               sprintf(path,"/proc/%s/task",namelist[i]->d_name);
//               m = scandir(path, &subnamelist, 0, 0);
//               if (m>0) {
//                  for(j=0;j<m;j++) {
//                     if (subnamelist[j]->d_type==4) {
//                        if (atoi(subnamelist[j]->d_name)!=0) {
//                           sprintf(path,"/proc/%s/task/%s/status",namelist[i]->d_name,subnamelist[j]->d_name);
//                           if ((fd=open(path, O_RDONLY) )>0) {
//                              lseek(fd, 0, SEEK_SET);
//                              if (read(fd,buffer,sizeof(buffer))>0) {
//                                 if (getParam(buffer,"State:\t",State,"%s")) {
//                                    if (strcmp(State,"R")==0) {
//                                       if (getParam(buffer,"SleepAVG:\t",&sleepAVG,"%d")) {
//                                          if (100-sleepAVG>Porcentaje) {
//                                             if (getParam(buffer,"Name:\t",procName,"%s")) {
//                                                STrz::basic(ConstTraza::T_TRAZA_MSJSISTEMA,"CPU!!! %02d%% - %s [ProcId:%s ThrId:%s]",100-sleepAVG,procName, namelist[i]->d_name,subnamelist[j]->d_name);
//                                             }
//                                          }
//                                       }
//                                    }
//                                 }
//                              }
//                              close(fd);
//                           }
//                        }
//                     }
//                     free(subnamelist[j]);
//                  }
//                  free(subnamelist);
//               }
//            }
//         }
//         free(namelist[i]);
//      }
//      free(namelist);
//   }
   return false;
}

float Sistema::contrario(float fPorcentaje) {
   if (fPorcentaje<0) return fPorcentaje;
   return(100-fPorcentaje);
}

bool Sistema::EsDir(struct dirent *namelist)
{
struct stat statbuf;

	if (stat(namelist->d_name, &statbuf) == -1)
		return false;

	return  (S_ISDIR(statbuf.st_mode));
}


SWordType Sistema::getPid(const Cadena& lNombreProceso)
{
	struct dirent** namelist;
	SByteType       lsbNombre[Cadena::LON_MAX_STRING];
	Cadena          lTemp, lProc;
	SDWordType      pid, res, i, len, n;

	res = -1;

#ifdef __sun
   char *aux = const_cast<char *> (lNombreProceso.getCadena());
   lProc = basename(aux);
#else
   lProc = basename((char*)lNombreProceso.getCadena());
#endif
   n = scandir("/proc", &namelist, 0, 0);
   memset (lsbNombre, 0, Cadena::LON_MAX_STRING);

   for(i=0;i<n;i++)
   {
//      if (namelist[i]->d_type == DT_DIR) //si es un directorio
	   if (EsDir(namelist[i]))
      {
         pid = atoi(namelist[i]->d_name);
         if ( (pid > 0) && (pid != getpid()) )
         {
            lTemp.formCadena ("/proc/%d/exe",pid);
            len = readlink (lTemp.getCadena (), lsbNombre, Cadena::LON_MAX_STRING);
            if (len > 0)
            {
               lsbNombre[len] = '\0';
               lTemp = basename(lsbNombre);
               if(lTemp == lProc)
                  res = pid;
            }
         }
      }
      free(namelist[i]);
   }

   if (n > 0)
      free(namelist);

   return(res);
}



const PSByteType Sistema::getNombreProceso (pid_t pid)
{
   static const int BUFSZ = 1024;

   static char nombre[BUFSZ + 1];

   ssize_t len;
   char path[32];

   sprintf (path, "/proc/%d/exe", pid);
   len = readlink (path, nombre, BUFSZ);
   if (len > 0)
   {
      nombre[len] = '\0';
      return basename (nombre);
   }
   else
   {
      return "???";
   }
}

pid_t Sistema::getPadreProceso (pid_t pid)
{
   int tmp;
   pid_t ppid;
   char path[512], tmpStr[128], tmpChar;
   FILE* f;
   bool ok;

   sprintf (path, "/proc/%d/stat", pid);
   f = fopen (path, "r");
   ok = (f != NULL && fscanf (f, "%d %s %c %d", &tmp, tmpStr, &tmpChar, &ppid) == 4);
   fclose (f);

   if (ok)
      return ppid;
   else
      return -1;
}
