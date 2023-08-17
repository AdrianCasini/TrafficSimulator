#!/usr/bin/perl.exe

#================================================================
#  FICHERO: sender_simulator_files.pl for simulator 
#  DESCRIPCION:
#  FECHA CREACION: 19-05-2022
#  AUTOR DISENYO:  Adrian Casini
#  PROGRAMADOR:   Adrian Casini
#================================================================

use IO::File;

$DIR_INIT="";
$ACTIVE = 0;
$FILES_READY = 0;
$SEND = 1;
$file = "@ARGV[0]";

if (@ARGV[0] eq "-f")
{

	$file_in = @ARGV[1];
	open (FILE, "< @ARGV[1]");
	($dir, $dum)= split/:/,@ARGV[1];
	$DIR_INIT="${dir}:/TrafficSimulator/Test/interfaces_pxy/simulator";
}
else
{
	open (FILE, "< ./$file");
	$DIR_INIT=".";
}


open(FILE_OUT,  "> $DIR_INIT/out/SENDER_${now}.txt");

while ($SEND)
{
	$SEND = 0;
	$ACTIVE = 0;
	$FILES_READY= 0;
	@files_ready=();
	@file_active=();
	print "--------Looking for neew files----------------------\n";
	@file_active = `ls -rt $DIR_INIT/ACTIVE_SENDING*`;
	chop(@file_active);
	foreach $file (@file_active) 	{if ($file =~ m/ACTIVE_SENDING/) {$ACTIVE = 1;}}
	@files_ready = `ls -rt $DIR_INIT/*ready* 2>/dev/nul`;
	chop(@files_ready);

	$FILES_READY = (scalar(@files_ready) > 0);

	$SEND = ($ACTIVE || $FILES_READY);


	if ($SEND)
	{
		foreach $file (@files_ready)
		{
			#$now = &ahora();
			`curl --location --request POST 'https://freeflow.autopistashub.com/simulator/simulator' -H 'Content-Type: application/json' -H 'X-Api-Key: UbP1XJUUpq2h20tb9NbNy1rP8ClEzq2naDpHycdx' -d \@$file`;
		    #print FILE_OUT "$file - $now\n";
			print "sending [$file]\n";
			`mv  $file $DIR_INIT/out/`;
			print "---------------------------------\n";
		}
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



