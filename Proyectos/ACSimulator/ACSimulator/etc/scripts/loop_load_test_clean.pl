#!/usr/bin/perl.exe

#================================================================
# Fichero 
#================================================================
#  FICHERO:  
#  DESCRIPCION:
#  FECHA CREACION: 15-09-2021
#  AUTOR DISENYO: Adrian Casini
#  PROGRAMADOR:   Adrian Casini
#================================================================
use IO::File;
$file  = "@ARGV[0]";
$now = &ahora();
$LOGFILE = IO::File->new();
$LOGFILE->open( "./log_${now}.txt",'>');
$log_file = "log_${now}.txt";
close($log_file);

for ($index_veh = 1; $index_veh < 7001; $index_veh++)
{
	print "$index_veh\n";
	`/drives/d/TrafficSimulator/Proyectos/ACSimulator/ACSimulator/etc/load_test_generator_clean.pl $file $index_veh $log_file`;
}


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
