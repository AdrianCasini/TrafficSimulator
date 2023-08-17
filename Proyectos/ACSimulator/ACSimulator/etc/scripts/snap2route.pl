#!/usr/bin/perl.exe

#================================================================
# Fichero snap2route.pl
#================================================================
#  FICHERO: snap2route.pl for simulator 
#  DESCRIPCION:
#  FECHA CREACION: 16-04-2021
#  AUTOR DISENYO: Another perl masterpiece of Adrian Casini
#  PROGRAMADOR:   Adrian Casini
#================================================================

push @INC, "/usr/lib/perl5/site_perl/5.26/i686-cygwin-threads-64int"; 
push @INC, "/usr/share/perl5/site_perl/5.26"; 
push @INC, "/usr/lib/perl5/vendor_perl/5.26/i686-cygwin-threads-64int"; 
push @INC, "/usr/share/perl5/vendor_perl/5.26";
push @INC, "/usr/lib/perl5/5.26/i686-cygwin-threads-64int"; 
push @INC, "/usr/share/perl5/5.26";
push @INC, "/usr/share/perl5";

#------- Initialization & Gral Variables ---------
($NAME, $THISDIR) = ($0 =~ m#^(.*)[/|\\]([^/|\\]+)$#o) ? ($2, $1) : ($0, '.');
$| = 1;


$SYNOPSIS = "$NAME [-h] [-d] [-s]
Este programa permite obtener los nodos de un recorrido, usando el sevicio de OSM de match";

$ARGUMENTS = "
Opciones:
  -f   Fichero con los puntos GPS de entrada.
  -n   Define el fichero de salida para intervalos de tiempo
  -b   Define cantidad de puntos por bloque enviado al servicoo OSM. Maximo 100.
  -h   Muestra esta ayuda
";


$DESCRIPTION = "
*****************************************************************************************
$NAME permite obtener los nodos de un recorrido, usando el sevicio de OSM de match
*****************************************************************************************
\n\n";


use IO::File;
require "newgetopt.pl";

$rc = &NGetOpt("h", "f=s", "b=s","n=s" );
if (!$opt_h && !$opt_f &&  !$opt_n &&  !$opt_b)
{ 
	print "Usar:\n$SYNOPSIS\n$ARGUMENTS\n"; exit;
}
if ($opt_h) { print "Usar:\n$SYNOPSIS\n$ARGUMENTS\n$DESCRIPTION\n"; exit;}
if ($opt_b) 
{ 
	$BLOCK=$opt_b;
	if ($opt_b > 100) 
	{ 
		print "La cantidad por bloque debe ser <= 100\n"; exit;
	}
}
if ($opt_f) 
{ 
	$FILE_IN = $opt_f;
	open (FILE_READ, "< $FILE_IN");
	($dir, $dum)= split/:/,$FILE_IN;
	$DIR_INIT="${dir}:/TrafficSimulator/Test/OSM_temp/snap";
	$DIR_TRACE="${dir}:/TrafficSimulator/Test/trace";

	@lines = <FILE_READ>; 
	close (FILE_READ);
	chop (@lines);
}
else
{ 
	print "Usar:\n$SYNOPSIS\n$ARGUMENTS\n"; exit;
}

if ($opt_n) 
{ 
	$FILE_TAG = $opt_n;
}

`rm $DIR_INIT/snptr_block_* 2>/dev/null`;
`rm $DIR_INIT/snap_lonlat_total* 2>/dev/null`;
`rm $DIR_INIT/snap_file_result* 2>/dev/null`;

#---------------------------------------------------------------------------
# Here we split the total amount of GPS point in $BLOCK gps points per file. 
# Then we use this files to call the OSM service
#---------------------------------------------------------------------------
$index = 0;
$line_counter=1;
$count_file=0;
$count_txt= sprintf("%04d",$count_file);
open (FILE_BLOCK, "> $DIR_INIT/snptr_block_gps_${count_txt}.txt");
open (FILE_BLOCK_TIME, "> $DIR_INIT/snptr_block_time_${count_txt}.txt");
$count_file++;
$NUM=scalar(@lines);
print "[$NUM]\n";
foreach $line (@lines)
{

    $line =~ s/\./__/g;
    $line =~ s/,/_/g;
	$line =~ s/;/_/g;
    $line =~ s/\W//g;
	$line =~ s/__/./g;
    $line =~ s/_/,/g;
	
	($lat_, $lon_, $timeepoch)=split/,/, $line;
	if ($lat_ > 20)
	{
		$lat = $lat_;
		$lon = $lon_;
	}
	else
	{
		$lon = $lat_;
		$lat = $lon_;
	}

	$str="$lon,$lat";
	#print "[$index]\n";

	if ($index == $BLOCK)
	{
		$index=0;
		print FILE_BLOCK "$str";
		print FILE_BLOCK_TIME "$timeepoch\n";
		close(FILE_BLOCK);
		close(FILE_BLOCK_TIME);

		$count_txt= sprintf("%04d",$count_file);

		open (FILE_BLOCK, "> $DIR_INIT/snptr_block_gps_${count_txt}.txt");
		open (FILE_BLOCK_TIME, "> $DIR_INIT/snptr_block_time_${count_txt}.txt");
		$count_file++;
	}
	else
	{
		print FILE_BLOCK "$str;";
		print FILE_BLOCK_TIME "$timeepoch;\n";
		$index++;
	}


	$line_counter++;
}
close(FILE_BLOCK);
close(FILE_BLOCK_TIME);

#---------------------------------------------------------------------------
# Here we take every block file to call the OSM service
# We append every OSM service result in the same file
#---------------------------------------------------------------------------
@files=();
@files = `ls -t $DIR_INIT`;
chop(@files);

foreach $file (@files)
{
	if ($file =~ m/snptr_block_gps/)
	{
		@line = ();
		open (FILE, "< $DIR_INIT/$file");
		@line = <FILE>; 
		chop (@line);
		close (FILE);

		$file =~ s/snptr_block_gps/snptr_block_time/g;
		@timeepoch = ();
		open (FILE, "< ./$file");
		@timeepoch = <FILE>; 
		chop (@timeepoch);
		close (FILE);
		$time_temp = $timeepoch;


		@dum=();
		@dum = split /;/, $line[0];
		$rads = scalar(@dum);
		$rad_line = "";
		$approaches = "";
		$timestr = "";
		for ($i = 0; $i < $rads; $i++)
		{
			$rad_line = join("",$rad_line, "8;" );
			$time_temp = $time_temp + 30;
			$timestr = join("",$timestr, "$time_temp;" );
		}
		chop($rad_line);
		chop($timestr);

		$EXECUTE = "curl 'http://router.project-osrm.org/match/v1/car/$line[0]?radiuses=$rad_line&annotations=true&tidy=true&geometries=geojson&generate_hints=false' 2>&1 >> $DIR_INIT/snap_file_result_${FILE_TAG}.txt";
		print "$EXECUTE\n";
	    print "-------------------------------------------------------------------------\n";
		$result = `$EXECUTE`;
		if ($result =~ m/resolve host/)
		{
			print "ERROR:  No existe conexion con internet\n";
			exit 3;
		}
		open (FILE_RESULT, "< $DIR_INIT/snap_file_result_${FILE_TAG}.txt");
	    print FILE_RESULT "\n-------------------------------------------------------------------------\n\n";
		close (FILE_RESULT);


		#$EXECUTE = "curl 'http://router.project-osrm.org/match/v1/car/$line[0]?timestamps=$timestr&radiuses=$rad_line&tidy=true&generate_hints=false' >> $DIR_INIT/snptr_file_result.txt";
		#`curl 'http://router.project-osrm.org/match/v1/car/$line[0]?timestamps=$timestr&radiuses=$rad_line&tidy=true&generate_hints=false' >> $DIR_INIT/snptr_file_result.txt`;
		#`curl 'http://router.project-osrm.org/route/v1/driving/$line[0]?' >> $DIR_INIT/snptr_file_result.txt`;
		#print "http://router.project-osrm.org/match/v1/car/$line[0]?timestamps=$timestr&radiuses=$rad_line&tidy=true&generate_hints=false&gaps=ignore\n";
	}
}

#--------------------------------------------------------------------------------------------------
# Here we take the whole result file and parse it, in order to extract all the lons and lats
#--------------------------------------------------------------------------------------------------
@lines=();
open (FILE, "< $DIR_INIT/snptr_file_result_${FILE_TAG}.txt");
@lines = <FILE>; 
chop (@lines);
close (FILE);

foreach $line (@lines)
{
	(@items)=split/,/, $line;
}

$count=0;
foreach $item (@items)
{
	if ($item =~ m/location/) 
	{
		$lon = $item;
		$lon =~ s/\"//g;
		$lon =~ s/location//g;
		$lon =~ s/\[//g;
		$lon =~ s/://g;

		$lat = @items[$count+1];
		$lat =~ s/\"//g;
		$lat =~ s/location//g;
		($lat, @dum) = split /]/, $lat;

		push(@latlon, "$lat,$lon");
		push(@lonlat, "$lon,$lat");
	
	}
	$count++;
}

$file_out = "$DIR_INIT/snap_lonlat_total_${FILE_TAG}.txt";

open (FILE, "> $file_out");
foreach $item (@latlon)
{
	print FILE "$item\n";
}
close(FILE);

`cp $file_out $DIR_TRACE/${FILE_TAG}_snapped.txt`;

