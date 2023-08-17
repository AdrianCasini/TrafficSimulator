#!/usr/bin/perl.exe
#
#================================================================
# Gestor de Base de Datos
#================================================================
# Fichero mngdb
#================================================================
#  FICHERO: mngdb 
#  DESCRIPCION:
#  FECHA CREACION: 15-10-2019
#  AUTOR DISENYO: Another perl masterpiece of Adrian Casini
#  PROGRAMADOR:   Adrian Casini
#================================================================


#------- Initialization & Gral Variables ---------
($NAME, $THISDIR) = ($0 =~ m#^(.*)[/|\\]([^/|\\]+)$#o) ? ($2, $1) : ($0, '.');
$| = 1;


$SYNOPSIS = "$NAME [-h] [-d] [-s] [-r] [-x] [-v] [-z] [-i] [-c TIME_DIAS]  [-b|-a FICHERO] [-f DIR]
Este programa permite gestionar la base de datos para el sitema FREEFLOW";

$ARGUMENTS = "
Opciones:
 -b   Realiza un Backup de la base de datos en el fichero FICHERO
 -f   Definir el Directorio de Trabajo
 -a   Actualiza la Base datos con fichero FICHERO
 -d   Detener el servicio de Base de datos
 -s   Arrancar el servicio de Base de datos
 -r   Remueve y Recrea la Base de datos. Se copiaran los ficheros de
      configuracion de la misma. SE PERDERAN TODOS LOS DATOS DE LA MISMA!
 -x   Realiza un reset del Transaccion Log. Esto es util cuando
      ha habido una caida brusca del Servico de Base de datos (corte de luz, etc.)
      y no ha podido gestionar bien una transaccion, con lo cual reuqiere ser
      arrancado luego de un reset del xlog.
 -c   Esta opcion eliminara todos los registros anteriores a TIME_DIAS dias. 
      
  Mantenimiento:
      -v   Vacuum: Limpiar la base de datos
      -z   Vacuum: Analize
      -i   Reindex database
      
  -h   Muestra esta ayuda


";


$DESCRIPTION = "
**************************************************************************
$NAME le permite detener o arrancar el servicio de base de datos,
crear backups, recobrarlos o recrear la misma.
**************************************************************************
\n\n";

#$LD_LIBRARY_PATH="/usr/lib/cross:/opt/PostgreSQL/8.3/lib:/opt/PostgreSQL/8.3/pgAdmin3/lib";
#$PATH="/root/bin:/fflow/bin:/fflow/bin/utils:/usr/sbin:/bin:/usr/bin:/sbin:/opt/PostgreSQL/8.3/pgAdmin3/bin";
#`export LD_LIBRARY_PATH`;
#`export PATH`;


push @INC, "/usr/lib/perl5/site_perl/5.26/i686-cygwin-threads-64int"; 
push @INC, "/usr/share/perl5/site_perl/5.26"; 
push @INC, "/usr/lib/perl5/vendor_perl/5.26/i686-cygwin-threads-64int"; 
push @INC, "/usr/share/perl5/vendor_perl/5.26";
push @INC, "/usr/lib/perl5/5.26/i686-cygwin-threads-64int"; 
push @INC, "/usr/share/perl5/5.26";
push @INC, "/usr/share/perl5";

require "newgetopt.pl";


$rc = &NGetOpt("h", "b=s", "f=s","a=s", "d", "s", "r", "x", "c=s", "e", "v", "z", "i" );
if (!$opt_h && !$opt_b &&  !$opt_f &&  !$opt_a && !$opt_r && !$opt_x && !$opt_d && !$opt_s && !$opt_c && !$opt_v && !$opt_z && !$opt_i )
{ print "Usar:\n$SYNOPSIS\n$ARGUMENTS\n"; exit;}
if ($opt_h) { print "Usar:\n$SYNOPSIS\n$ARGUMENTS\n$DESCRIPTION\n"; exit;}



#--------------------------------------------------------
#   Directorio de Trabajo
#--------------------------------------------------------
$DIR_W="E";
if ($opt_f)
{
   $DIR_W=$opt_f;
}


$DIRDB        = "$DIR_W:/TrafficSimulator/BaseDatos/SIMULATOR_DB";
$CREATEFILEDB = "$DIR_W:/TrafficSimulator/Proyectos/ACSimulator/ACSimulator/dbase/crearbd.sql";
$DIRBIN       = "$DIR_W:/TrafficSimulator/PostgresSQL/bin";
$DBNAME       = "simulator_db";
$USER         = "adria";


$resp         = '';
`unset LANG >> /dev/null`;

$resp         = '';
`unset LANG >> /dev/null`;


#--------------------------------------------------------
#   Guardar un backup de la base de datos
#--------------------------------------------------------
if ($opt_b)
{
   $FICHEROBCK=$opt_b;
   `$DIRBIN/pg_dump -U $USER -d $DBNAME  >> $FICHEROBCK`;
   exit 0;
}


#--------------------------------------------------------
#   Recobrar un backup de la base de datos
#--------------------------------------------------------
if ($opt_a)
{
   $FICHEROBCK=$opt_a;
   if (-e  $FICHEROBCK)
   {
      `$DIRBIN/psql -U $USER -d $DBNAME  -c "DROP SCHEMA public CASCADE;CREATE SCHEMA public AUTHORIZATION $USER; GRANT ALL ON SCHEMA public TO $USER;"`;

      `$DIRBIN/psql -U $USER -d $DBNAME   < $FICHEROBCK`;
   }
   else
   {
      print "Error: el fichero:[$FICHEROBCK] no existe!\n\n";
   }
   exit 0;
}

#--------------------------------------------------------
#   Detener Servicio Base de Datos
#--------------------------------------------------------
if ($opt_d)
{
   &detener_db();
   exit 0;
}

#--------------------------------------------------------
#   Arrancando Servicio Base de Datos
#--------------------------------------------------------
if ($opt_s)
{
   &arrancar_db();
   exit 0;
}

#--------------------------------------------------------
#   Recrear Base de datos
#--------------------------------------------------------
if ($opt_r)
{
   print "\n\n#############################################\n";
   print "##  CUIDADO:                                 \n";
   print "##  Este procedimiento Borrara la Base de    \n";
   print "##  Datos $DBNAME y luego la recreara   \n";
   print "##  PERDIENDOSE TODOS LOS DATOS !!           \n";
   print "###############################################\n";

   while ($resp !~ m/^(s|n)$/)
   {
      print "\nEsta seguro de querer continuar? [Responda s/n]:";
      $resp = <STDIN>;chop($resp);
      if ($resp !~ m/^(s|n)$/)
      {
         print "\nDebe responder 's' o 'n'\n\n";
      }
      else
      {
         if ($resp eq 'n') {exit 0;}
         break;
      }
   }


   #--Parar servicio postgress si o si
   &detener_db();

   #--Remover el contenido del directorio--
   print "\nRemoviendo Directorio Base Datos\n";
   print "------------------------------------\n";
   `rm -fr $DIRDB/*`;

   # ---Iniciar DB
   print "\nInicializando Base de Datos\n";
   print "------------------------------------\n";
   $exec = "$DIRBIN/initdb -A trust -U $USER -D $DIRDB";
   print "$exec\n";
   `$exec`;
   print "------------------------------------\n";
   
   #--Recuperar bck de ficheros de configuracion db
   print "\nRecuperando ficheros de configuracion\n";
   print "------------------------------------\n";

   #`cp /cygdrive/c/OPENGL/Proyectos/postgresql.conf /cygdrive/c/BASEDATOS/OpenGLDB; chmod 755 /cygdrive/c/BASEDATOS/OpenGLDB/postgresql.conf`;
   #`cp /cygdrive/c/OPENGL/Proyectos/pg_hba.conf /cygdrive/c/BASEDATOS/OpenGLDB;  chmod 755 /cygdrive/c/BASEDATOS/OpenGLDB/pg_hba.conf`;

   #--Iniciar servicio postgress si o si
   print "\nIniciando servicio de Base de Datos\n";
   print "------------------------------------\n";
   $exec = "$DIRBIN/pg_ctl -U $USER -D $DIRDB start  >>/dev/null";
   print "$exec\n";
   `$exec`;
   print "------------------------------------\n";

   # ---Crear DB $DBNAME
   print "\nCreando Base de  datos $DBNAME\n";
   print "------------------------------------\n";
   $exec = "$DIRBIN/createdb -U $USER -e $DBNAME";
   print "$exec\n";
   `$exec`;
   print "------------------------------------\n";
   
   # ---Recrear tablas

   print "\nCreando Tablas en base de datos $DBNAME\n";
   print "------------------------------------\n";
   $exec = "$DIRBIN/psql -U $USER -d $DBNAME -f $CREATEFILEDB";
   print "$exec\n";
   `$exec`;
   print "------------------------------------\n";

   exit 0;
}

#--------------------------------------------------------
#   Reset del transaccion LOG
#--------------------------------------------------------
if ($opt_x)
{
   `$DIRBIN/pg_resetxlog -f  $DIRDB`;
   exit 0;
}



#--------------------------------------------------------
#   VACUUM
#--------------------------------------------------------
if ($opt_v)
{
   @dum=();
   @dum=`$DIRBIN/psql -d $DBNAME -U $USER -c "vacuum full;" 2>&1`;
   print "\n@dum\n";
}


#--------------------------------------------------------
#   VACUUM ANALIZE
#--------------------------------------------------------
if ($opt_z)
{
   @dum=();
   @dum=`$DIRBIN/psql -d $DBNAME -U $USER -c "vacuum analyze;" 2>&1`;
   print "\n@dum\n";
}

#--------------------------------------------------------
#   Reindexing all tables
#--------------------------------------------------------
if ($opt_i)
{
   @dum=();
   @dum=`$DIRBIN/psql -d $DBNAME -U $USER -c "reindex database $DBNAME;" 2>&1`;
   print "\n@dum\n";
}


#--------------------------------------------------------
#   Funciones
#--------------------------------------------------------
sub arrancar_db
{
   #--Iniciar servicio postgress si
   print "\nIniciando servicio de Base de Datos\n";
   print "------------------------------------\n";

   `$DIRBIN/pg_ctl -U $USER -D $DIRDB start >>/dev/null`;

   for ($i=0;$i<5;$i++) {print ".";sleep(1);}
}

#--------------------------------------------------------
sub detener_db
{
   #--Detener servicio postgress si o si
   print "\nDeteniendo servicio de Base de Datos\n";
   print "------------------------------------\n";

   `$DIRBIN/pg_ctl -U $USER -D $DIRDB  stop -m fast >>/dev/null`;
   
   #---Anyway-----
   @process=`ps -elf | grep postmaster | grep -v grep | awk '{print \$4}'`;
   chop(@process);
   if (scalar(@process) > 0) {foreach $proc (@process) {`kill -9 $proc`;}}
}

#---------------------------------------------------------------------------------------
sub ahora
{
   my ($fmt)=@_;
   my $ahora;

   my $year    = `date "+%Y"`;chop($year);    
   my $month   = `date "+%m"`;chop($month);  
   my $day     = `date "+%d"`;chop($day);  
   my $hour    = `date "+%H"`;chop($hour);  
   my $min     = `date "+%M"`;chop($min); 
   my $sec     = `date "+%S"`;chop($sec);

   if    ($fmt eq 'db')  {$ahora="$year-$month-$day $hour:$min:$sec";}
   elsif ($fmt eq 'web') {$ahora="$year/$month/$day $hour:$min:$sec";}
   elsif ($fmt eq '#')   {$ahora="$year#$month#$day#$hour#$min#$sec";}
   else                  {$ahora="$year$month$day$hour$min$sec";}

   return $ahora;
}

#---------------------------------------------------------------------------------------
sub get_fecha_atras
{
   my ($time_ahora, $dias_atras, $fmt)=@_;
   
   my ($year_ahora,$month_ahora,$day_ahora,$hour_ahora,$min_ahora,$sec_ahora)=split/#/,$time_ahora;

   my $day   = $day_ahora;
   my $month = $month_ahora;
   my $year  = $year_ahora;

   my $hour  = $hour_ahora;
   my $min   = $min_ahora;
   my $sec   = $sec_ahora;

   for ($i=0;$i<$dias_atras;$i++)
   {
      $day--;
      if ($day==0)
      {
         $month--;
         if ($month==0)
         {
            $year--;
            $month=12;
         }
         $day=&dias_del_mes($month,$year);
      }
   }

   if ($sec   =~ m/^\d$/) {$sec   = "0$sec";}
   if ($min   =~ m/^\d$/) {$min   = "0$min";}
   if ($hour  =~ m/^\d$/) {$hour  = "0$hour";}
   if ($day   =~ m/^\d$/) {$day   = "0$day";}
   if ($month =~ m/^\d$/) {$month = "0$month";}


   if ($fmt eq 'db')
   {
      $fecha_nueva="$year-$month-$day $hour:$min:$sec";
   }
   elsif ($fmt eq '#')
   {
      $fecha_nueva="$year#$month#$day#$hour#$min#$sec";
   }
   else 
   {
      $fecha_nueva="$year$month$day$hour$min$sec";
   }

   return $fecha_nueva;

}

#---------------------------------------------------------------------------------------
sub dias_del_mes
{
   my ($mes,$anio)=@_;

   if (($mes =~ m/1/) && ($mes !~ m/10|11/)) {$dias=31;}
   elsif (($mes =~ m/2/) && ($mes !~ m/12/))
   {
      $dias=28;
      if ($anio =~ m/2012|2016|2020|2024|2028|2032/) {$dias=29;}
   }
   elsif ($mes =~ m/3/)  {$dias=31;}
   elsif ($mes =~ m/4/)  {$dias=30;}
   elsif ($mes =~ m/5/)  {$dias=31;}
   elsif ($mes =~ m/6/)  {$dias=30;}
   elsif ($mes =~ m/7/)  {$dias=31;}
   elsif ($mes =~ m/8/)  {$dias=31;}
   elsif ($mes =~ m/9/)  {$dias=30;}
   elsif ($mes =~ m/10/) {$dias=31;}
   elsif ($mes =~ m/11/) {$dias=30;}
   elsif ($mes =~ m/12/) {$dias=31;}

   return $dias;
}
