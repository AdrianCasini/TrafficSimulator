#!/usr/bin/perl.exe
#================================================================
# Fichero route2route.pl
#================================================================
#  FICHERO: route2route.pl for simulator 
#  DESCRIPCION:
#  FECHA CREACION: 20-07-2021
#  AUTOR DISENYO: Another perl/tk masterpiece of Adrian Casini
#  PROGRAMADOR:   Adrian Casini
#================================================================

use IO::File;

$DIR_INIT="";

$file = "@ARGV[0]";

if (@ARGV[0] eq "-f")
{

	$file_in = @ARGV[1];
	open (FILE, "< @ARGV[1]");
	($dir, $dum)= split/:/,@ARGV[1];
	$DIR_INIT="${dir}:/TrafficSimulator/Etc/generated_files/route";
}
else
{
	open (FILE, "< ./$file");
	$DIR_INIT=".";
}

if (@ARGV[2] eq "-n")
{
	$file_name = @ARGV[3];
}
else
{
	$file_name = "xx";
}


@lines = <FILE>; 
#chop (@lines);
close (FILE);

$BLOCK=90;

`rm $DIR_INIT/route_block_*`;
`rm $DIR_INIT/route_file_result*`;
`rm $DIR_INIT/route_file_routed*`;

$index = 0;
$line_counter=1;
$count_file=0;
$count_txt= sprintf("%04d",$count_file);
open (FILE, "> $DIR_INIT/route_block_${count_txt}${file_name}.txt");
$count_file++;
$NUM=scalar(@lines);

for ($i = 0; $i < $NUM; $i++)
{
	#chop ($lines[$i]);
	($lat, $lon, $dum)=split/,/, $lines[$i];
	$str="$lon,$lat";

	if ($index == $BLOCK)
	{
		$index=0;
		print FILE "$str";
		close(FILE);

		$count_txt= sprintf("%04d",$count_file);
		open (FILE, "> $DIR_INIT/route_block_${count_txt}${file_name}.txt");
		$count_file++;
	}
	else
	{
		print FILE "$str;";
		$index++;
	}

	$line_counter++;
}
close(FILE);

$radius_str=10;
for ($i=0;$i < $NUM-1; $i++)
{
	join (";", "$radius_str",10)
}


@files=();
@files = `ls -t $DIR_INIT/`;
chop(@files);
$count=0;

foreach $file (@files)
{
	if ($file =~ m/route_block_/)
	{
		@line = ();
		open (FILE, "< $DIR_INIT/$file");
		@line = <FILE>; 
		chop (@line);
		close (FILE);
		@dum=();
		@dum = split /;/, $line[0];
		$rads = scalar(@dum);
		$rad_line = "";
		$waypoints = "";
		for ($i = 0; $i < $rads; $i++)
		{
			$rad_line = join("",$rad_line, "8;" );
		}
		chop($rad_line);
		for ($i = 0; $i < $rads; $i++)
		{
			$waypoints = join("",$waypoints, "0;" );
		}
		chop($waypoints);
		chop($waypoints);
		$tip=$rads-1;
		$waypoints = join("",$waypoints, "$tip" );

		`curl 'http://router.project-osrm.org/route/v1/driving/$line[0]?skip_waypoints=false&snapping=default&alternatives=true&overview=full&geometries=geojson&annotations=true&continue_straight=default&steps=false' >> $DIR_INIT/route_file_result_${file_name}.txt`;
		#`curl 'http://router.project-osrm.org/route/v1/driving/$line[0]?waypoints=$waypoints&radiuses=$rad_line&skip_waypoints=false&snapping=default&alternatives=true&overview=full&geometries=geojson&annotations=true&continue_straight=default&steps=false' >> $DIR_INIT/route_file_result.txt`;
		#`curl 'http://router.project-osrm.org/route/v1/car/$line[0]?alternatives=false&overview=full&geometries=polyline&annotations=true&continue_straight=default&steps=false' >> route_file_result.txt`;
		#`curl 'http://router.project-osrm.org/trip/v1/driving/$line[0]?overview=full&geometries=geojson&annotations=true&bearing=10' >> route_file_result.txt`;
		#`curl 'http://router.project-osrm.org/trip/v1/driving/$line[0]?source=first&destination=last&roundtrip=false&overview=full&geometries=geojson' >> route_file_result.txt`;
		
		$count++;
	}
}
#   }

@lines=();  
open (FILE, "< $DIR_INIT/route_file_result_${file_name}.txt");
@lines = <FILE>; 
chop (@lines);
close (FILE);

foreach $line (@lines)
{
	(@items)=split/\"/, $line;
}

$count=0;
$coordenadas="";
$catched=0;
foreach $item (@items)
{
	if ($item =~ m/coordinates/)
	{
		$coordenadas = join ("", $coordenadas, @items[$count+1]);
	}
	$count++;
}

$coordenadas =~ s/\[//g;
$coordenadas =~ s/]//g;
$coordenadas =~ s/://g;

$count;
@lonlat = split /,/, $coordenadas;

open (FILE_OUT, "+> $DIR_INIT/route_file_routed_${file_name}.txt");

for ($i=0; $i < scalar(@lonlat); $i++)
{
	print FILE_OUT "$lonlat[$i+1],$lonlat[$i]\n";
	$i++;
}
close(FILE_OUT);

