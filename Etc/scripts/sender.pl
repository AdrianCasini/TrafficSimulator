#!/usr/bin/perl.exe

#================================================================
# Fichero sender.pl
#================================================================
#  FICHERO: sender.pl for simulator 
#  DESCRIPCION:
#  FECHA CREACION: 16-04-2021
#  AUTOR DISENYO:  Adrian Casini
#  PROGRAMADOR:   Adrian Casini
#================================================================

use IO::File;

@files = `ls -rt Processed/`;
chop(@files);

foreach $file (@files)
{
	if ($file =~ m/trip/)
	{
		#print "---[$file]----\n";
		#---[trip_0_267_117119.txt]----
		($dum, $index, $seg, $tripcode) = split /_/, $file;
		$seg_fmt= sprintf("%06d",$seg);
		$tripcode_fmt= sprintf("%08d",$tripcode);
		$index_fmt= sprintf("%06d",$index);
		$str = join("__", $seg_fmt, $index_fmt, $tripcode_fmt);
		$hash_files_2_send{$str}= "Processed/$file";
	}

}
print "\n\n";



$now = &ahora();
open(FILE_OUT,  "> ./SENDER_${now}.txt");

foreach $key (sort keys %hash_files_2_send)
{

	$now = &ahora();
	#PRO
	`curl -v --location --request POST 'https://s15996gji8.execute-api.eu-west-1.amazonaws.com/prod/eurotoll/upload' --header 'Content-Type: application/json' --header 'Authorization: Basic ZXVyb3RvbGxfc2VydmljZV9wcm9kOlRMbGZHcFdHNDlyNkNyYXo=' -d "\@$hash_files_2_send{$key}"`;

	#PRE
	#`curl -v --location --request POST 'https://9x6da9qbdg.execute-api.eu-west-1.amazonaws.com/test/eurotoll/upload' --header 'Content-Type: application/json' --header 'Authorization: Basic YWJlcnRpc1Rlc3Q6c2VjcmV0UGFzc3dvcmQxMjM0' -d "\@$hash_files_2_send{$key}"`;
	

	#TEST
	#`curl -v --location --request POST 'https://f0eov8ph41.execute-api.eu-west-1.amazonaws.com/test/eurotoll/upload' --header 'Content-Type: application/json' --header 'Authorization: Basic ZXVyb3RvbGxfc2VydmljZV90ZXN0OmdOQ054eXVJckVZQ2Ixakk=' -d "\@$hash_files_2_send{$key}"`;
	

	print "\n\n";
	print "\n===================================================================  \n";
	print ">>>[$hash_files_2_send{$key}]----$key----\n";
	print "\n\n";

	print FILE_OUT "$key - [$hash_files_2_send{$key}] - $now\n";

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



