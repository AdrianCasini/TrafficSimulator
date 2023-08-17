#!c:\Perl64\bin\perl.exe

#================================================================
# Fichero db_filer
#================================================================
#  FICHERO: db_filler for simulator_db 
#  DESCRIPCION:
#  FECHA CREACION: 15-10-2019
#  AUTOR DISENYO: Another perl/tk masterpiece of Adrian Casini
#  PROGRAMADOR:   Adrian Casini
#================================================================

#------- Initialization ---------
use DBI;
use DBD::Pg qw(:pg_types);

$HOST='localhost';
$USER='adria';
$PORT='5432';

#----------The File contains 68300683 lines -----------------
open (FILE, "<  ../etc/InfoAP7");
#open (FILE, "<  /drives/e/OPENGL/Proyectos/ACSimulator/ACSimulator/etc/InfoAP7");
seek(FILE, 0, 0);#en bytes
$DBH=&db_connect();


print "--------------------------------------\n";
$count   = 0;
$count_g = 0;
while ( ! eof(FILE) ) 
{
   defined( $line = readline FILE ) or die "readline failed: $!";
   $line =~ s/\"//g;
   chop($line);
   #print "$line\n";

   if ($count > 100000)
   {
      print "$count_g\n";
	  $count=0;
   }
   $count++;
   $count_g++;

   ($tipomsj,$version, $numeroestacion, $numerovia, $sentido, $tipovia, $momento, $estadoactual, $modoactual, $tabulacioncob, $clase, $categoriatarif, $importepropio, $importeajeno, $moneda, $decimales, $iva, $formapagofinal, $simulasionpaso, $contenidotarjobe, $tabulacioninicial, $formapagoinicial, $tipoticketentrada, $versionticketentrada, $numerosecuencialticket, $estacionentrada,$viaentrada, $momentoentrada, $unidadanyoentrada, $numerom5, $tiemposervicio, $tiempotransaccion, $resulatdocapturasrm, $matriculasrm ) = ();
   ($tipomsj,$version, $numeroestacion, $numerovia, $sentido, $tipovia, $momento, $estadoactual, $modoactual, $tabulacioncob, $clase, $categoriatarif, $importepropio, $importeajeno, $moneda, $decimales, $iva, $formapagofinal, $simulasionpaso, $contenidotarjobe, $tabulacioninicial, $formapagoinicial, $tipoticketentrada, $versionticketentrada, $numerosecuencialticket, $estacionentrada,$viaentrada, $momentoentrada, $unidadanyoentrada, $numerom5, $tiemposervicio, $tiempotransaccion, $resulatdocapturasrm, $matriculasrm )= split /;/, $line;
   #print "($tipomsj,$version, $numeroestacion, $numerovia, $sentido, $tipovia, $momento, $estadoactual, $modoactual, $tabulacioncob, $clase, $categoriatarif, $importepropio, $importeajeno, $moneda, $decimales, $iva, $formapagofinal, $simulasionpaso, $contenidotarjobe, $tabulacioninicial, $formapagoinicial, $tipoticketentrada, $versionticketentrada, $numerosecuencialticket, $estacionentrada,$viaentrada, $momentoentrada, $unidadanyoentrada, $numerom5, $tiemposervicio, $tiempotransaccion, $resulatdocapturasrm, $matriculasrm )\n";

   @date=();
   (@date)=split//,$momento;
   $anyo  = "$date[0]$date[1]$date[2]$date[3]";
   $mes   = "$date[4]$date[5]";
   $dia   = "$date[6]$date[7]";
   $hora  = "$date[8]$date[9]";
   $min   = "$date[10]$date[11]";
   $seg   = "$date[12]$date[13]";  

   $importe = $importe/100;

   $momento = "$anyo/$mes/$dia $hora:$min:$seg";



   if ($formapagoinicial !~ m/\d/) 
   {
      $formapagoinicial = 0;
   }
   if ($versionticketentrada !~ m/\d/) 
   {
      $versionticketentrada = 0;
   }
   if ($numerosecuencialticket !~ m/\d/) 
   {
      $numerosecuencialticket = 0;
   }
   if ($estacionentrada !~ m/\d/) 
   {
      $estacionentrada = 0;
   }
   if ($viaentrada !~ m/\d/) 
   {
      $viaentrada = 0;
   }
   if ($momentoentrada !~ m/\d/) 
   {
      $momentoentrada = 0;
   }
   if ($unidadanyoentrada !~ m/\d/) 
   {
      $unidadanyoentrada = 0;
   }
   if ($numerom5 !~ m/\d/) 
   {
      $numerom5 = 0;
   }

   if ($tiemposervicio !~ m/\d/) 
   {
      $tiemposervicio = 0;
   }
   if ($tiempotransaccion !~ m/\d/) 
   {
      $tiempotransaccion = 0;
   }
   if ($resulatdocapturasrm !~ m/\d/) 
   {
      $resulatdocapturasrm = 0;
   }

   my $sth = $DBH->prepare("INSERT INTO Transitos (tipomsj,version, numeroestacion, numerovia, sentido, tipovia, momento, estadoactual, modoactual, tabulacioncob, clase, categoriatarif, importepropio, importeajeno, moneda, decimales, iva, formapagofinal, simulasionpaso, contenidotarjobe, tabulacioninicial, formapagoinicial, tipoticketentrada, versionticketentrada, numerosecuencialticket, estacionentrada,viaentrada, momentoentrada, unidadanyoentrada, numerom5, tiemposervicio, tiempotransaccion, resulatdocapturasrm, matriculasrm) values (\'$tipomsj\',\'$version\', \'$numeroestacion\', \'$numerovia\', \'$sentido\', \'$tipovia\', \'$momento\', \'$estadoactual\', \'$modoactual\', \'$tabulacioncob\', \'$clase\', \'$categoriatarif\', \'$importepropio\', \'$importeajeno\', \'$moneda\', \'$decimales\', \'$iva\', \'$formapagofinal\', \'$simulasionpaso\', \'$contenidotarjobe\', \'$tabulacioninicial\', \'$formapagoinicial\', \'$tipoticketentrada\', \'$versionticketentrada\', \'$numerosecuencialticket\', \'$estacionentrada\',\'$viaentrada\', \'$momentoentrada\', \'$unidadanyoentrada\', \'$numerom5\', \'$tiemposervicio\', \'$tiempotransaccion\', \'$resulatdocapturasrm\', \'$matriculasrm\') ");
   $sth->execute();  
   #print "$PSQL -c INSERT INTO Transitos (tipomsj,version, numeroestacion, numerovia, sentido, tipovia, momento, estadoactual, modoactual, tabulacioncob, clase, categoriatarif, importepropio, importeajeno, moneda, decimales, iva, formapagofinal, simulasionpaso, contenidotarjobe, tabulacioninicial, formapagoinicial, tipoticketentrada, versionticketentrada, numerosecuencialticket, estacionentrada,viaentrada, momentoentrada, unidadanyoentrada, numerom5, tiemposervicio, tiempotransaccion, resulatdocapturasrm, matriculasrm) values (\'$tipomsj\',\'$version\', \'$numeroestacion\', \'$numerovia\', \'$sentido\', \'$tipovia\', \'$momento\', \'$estadoactual\', \'$modoactual\', \'$tabulacioncob\', \'$clase\', \'$categoriatarif\', \'$importepropio\', \'$importeajeno\', \'$moneda\', \'$decimales\', \'$iva\', \'$formapagofinal\', \'$simulasionpaso\', \'$contenidotarjobe\', \'$tabulacioninicial\', \'$formapagoinicial\', \'$tipoticketentrada\', \'$versionticketentrada\', \'$numerosecuencialticket\', \'$estacionentrada\',\'$viaentrada\', \'$momentoentrada\', \'$unidadanyoentrada\', \'$numerom5\', \'$tiemposervicio\', \'$tiempotransaccion\', \'$resulatdocapturasrm\', \'$matriculasrm\');";
   $sth->finish();


}


close (FILE);



#---------------------------------------------------------------------------------------
sub db_connect
{
   my ($dbh)=@_;
   $dbh = DBI->connect("dbi:Pg:dbname=simulator_db;
      host=$HOST;
      port=$PORT",
      $USER, 
      '');

   return $dbh;
}

#---------------------------------------------------------------------------------------
sub db_disconnect
{
   my ($dbh)=@_;
   $dbh->disconnect(); 
}
