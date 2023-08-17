#!/usr/bin/perl.exe
#

#================================================================
# Fichero snap2route.pl
#================================================================
#  FICHERO: snap2route.pl for simulator 
#  DESCRIPCION:
#  FECHA CREACION: 16-04-2021
#  AUTOR DISENYO: Another perl masterpiece of Adrian Casini
#  PROGRAMADOR:   Adrian Casini
#================================================================

use IO::File;

$DIR_INIT="";
$BLOCK=90;

$file = "@ARGV[0]";

if (@ARGV[0] eq "-d")
{

	$file_in = @ARGV[1];
	print "$file_in\n";
	open (FILE_READ, "< @ARGV[1]");
	($dir, $dum)= split/:/, @ARGV[1];

	$DIR_INIT="${dir}:/TrafficSimulator/Etc/generated_files/snap";
	$DIR_TRACE="${dir}:/TrafficSimulator/Test/trace";
}
if (@ARGV[2] eq "-f")
{
	$file_in_clean = @ARGV[3];
	print "$file_in_clean\n";
	($dum, $dum, $dum, $dum,$file_in_clean ) = split /\\/, $file_in_clean;
	$file_in_clean =~ s/.txt//;
}
else
{
	open (FILE_READ, "< ./$file");
	$DIR_INIT=".";
}


`rm $DIR_INIT/snptr_block_* 2>/dev/null`;
`rm $DIR_INIT/snptr_file_result.txt 2>/dev/null`;
`rm $DIR_INIT/snap_lonlat_total.txt 2>/dev/null`;

@lines = <FILE_READ>; 
close (FILE_READ);
chop (@lines);


$index = 0;
$line_counter=1;
$count_file=0;
$count_txt= sprintf("%04d",$count_file);
open (FILE, "> $DIR_INIT/snptr_block_gps_${count_txt}.txt");
open (FILE_T, "> $DIR_INIT/snptr_block_time_${count_txt}.txt");
$count_file++;
$NUM=scalar(@lines);
foreach $line (@lines)
{
	#print "[$line]\n";

	($lat, $lon, $timeepoch)=split/,/, $line;
	$str="$lon,$lat";


	if ($index == $BLOCK)
	{
		$index=0;
		print FILE "$str";
		print FILE_T "$timeepoch\n";
		close(FILE);
		close(FILE_T);

		$count_txt= sprintf("%04d",$count_file);
		open (FILE, "> $DIR_INIT/snptr_block_gps_${count_txt}.txt");
		open (FILE_T, "> $DIR_INIT/snptr_block_time_${count_txt}.txt");
		$count_file++;
	}
	else
	{
		print FILE "$str;";
		print FILE_T "$timeepoch;\n";
		$index++;
	}


	$line_counter++;
}
close(FILE);
close(FILE_T);


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

		$EXECUTE = "curl 'http://router.project-osrm.org/match/v1/car/$line[0]?radiuses=$rad_line&tidy=true&generate_hints=false' >> $DIR_INIT/snptr_file_result.txt";
		#print "$EXECUTE\n";
		`$EXECUTE`;



		#$EXECUTE = "curl 'http://router.project-osrm.org/match/v1/car/$line[0]?timestamps=$timestr&radiuses=$rad_line&tidy=true&generate_hints=false' >> $DIR_INIT/snptr_file_result.txt";
		#`curl 'http://router.project-osrm.org/match/v1/car/$line[0]?timestamps=$timestr&radiuses=$rad_line&tidy=true&generate_hints=false' >> $DIR_INIT/snptr_file_result.txt`;
		#`curl 'http://router.project-osrm.org/route/v1/driving/$line[0]?' >> $DIR_INIT/snptr_file_result.txt`;
		#print "http://router.project-osrm.org/match/v1/car/$line[0]?timestamps=$timestr&radiuses=$rad_line&tidy=true&generate_hints=false&gaps=ignore\n";
	}
}


@lines=();
open (FILE, "< $DIR_INIT/snptr_file_result.txt");
@lines = <FILE>; 
chop (@lines);
close (FILE);

foreach $line (@lines)
{
	(@items)=split/,/, $line;
}

$count=0;
#$wrong=0;
foreach $item (@items)
{
	#if ($item =~ m/null/)
	#{
	#	$wrong=1;
	#}
	
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
		#chop($lat);

		push(@latlon, "$lat,$lon");
		push(@lonlat, "$lon,$lat");
	

	}

	$count++;
}

$file_out = "$DIR_INIT/snap_lonlat_total.txt";

open (FILE, "> $file_out");
foreach $item (@latlon)
{
	print FILE "$item\n";
}
close(FILE);

`cp $file_out $DIR_TRACE/${file_in_clean}_snapped.txt`;
