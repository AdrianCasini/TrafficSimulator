#!/usr/bin/perl.exe
#

#================================================================
# Fichero move_veh_identifier.pl
#================================================================
#  FICHERO: move_veh_identifier for simulator 
#  DESCRIPCION:
#  FECHA CREACION: 16-09-2021
#  AUTOR DISENYO: Another perl/tk masterpiece of Adrian Casini
#  PROGRAMADOR:   Adrian Casini
#================================================================

use IO::File;


$FILE_NAME             = "@ARGV[0]";
$DELTA_VEH             = "@ARGV[1]";
$DELTA_TIME_EPOCH      = "@ARGV[2]";
$RECORRIDO             = "@ARGV[3]";
$RECORRIDO_DEF         = 0;

if ($DELTA_VEH !~ m/\d+/)
{
	print "\n\n We need DELTA_VEH in natural numbers\n\n";
	exit;
}

if ($DELTA_TIME_EPOCH !~ m/\d+/)
{
	print "\n\n We need DELTA_TIME_EPOCH in natural numbers (seconds delta from time epoch)\n\n";
	exit;
}

if ($RECORRIDO =~ m/^(\s)*$/)
{
	$RECORRIDO_DEF=0;
	print "\n\n $RECORRIDO= Default\n\n";
}
else
{
	$RECORRIDO_DEF=1;
}



open (FILE, "< ./$FILE_NAME");
@lines = <FILE>; 
chop (@lines);
close (FILE);

$now = &ahora();

open(FILE_OUT,  "> ./moved_${FILE_NAME}_${DELTA_VEH}_${now}.txt");

foreach $line (@lines)
{
	if ($line =~ m/veh/)
	{
		#veh0#1621240038#17#10#41.198848#1.638866#58#4#30#-858993460-858993460
		($index_veh, $time_epoch, $time_segStart, $time_seg_veh, $lat, $lon, $distance, $angle, $speed, $recorrido) = split /#/, $line;
		$index_veh =~ s/veh//g;
		$index_veh = $index_veh + $DELTA_VEH;
		$index_veh = "veh$index_veh";


		$time_epoch = $time_epoch + $DELTA_TIME_EPOCH;
		if ($RECORRIDO_DEF)
		{
			$recorrido = $RECORRIDO;
		}

		print FILE_OUT "$index_veh#$time_epoch#$time_segStart#$time_seg_veh#$lat#$lon#$distance#$angle#$speed#$recorrido\n";
	}
}

close(FILE_OUT);


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
