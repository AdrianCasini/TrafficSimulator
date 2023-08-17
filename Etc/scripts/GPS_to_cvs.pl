#!/usr/bin/perl.exe
#

#================================================================
# Fichero perf_files_gen
#================================================================
#  FICHERO: perf_files_gen for simulator 
#  DESCRIPCION:
#  FECHA CREACION: 16-04-2021
#  AUTOR DISENYO: Another perl/tk masterpiece of Adrian Casini
#  PROGRAMADOR:   Adrian Casini
#================================================================

use IO::File;


$file = "@ARGV[0]";
$NN = "@ARGV[1]";

open (FILE, "< ./$file");
@lines = <FILE>; 
chop (@lines);
close (FILE);

$now = &ahora();

open(FILE_OUT,  "> ./gps_${NN}_${now}.txt");

foreach $line (@lines)
{
	if ($line =~ m/veh$NN#/)
	{
		#veh0#1621240038#17#10#41.198848#1.638866#58#4#30#-858993460-858993460
		($index_veh, $time_epoch, $time_segStart, $time_seg_veh, $lat, $lon, $distance, $angle, $speed, $recorrido) = split /#/, $line;

		print FILE_OUT "$lat,$lon,$time_epoch\n";
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
