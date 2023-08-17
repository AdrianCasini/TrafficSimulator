#!/usr/bin/perl.exe

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

@files_veh1 = `ls -rt trip_1_*`;
chop(@files_veh1);



foreach $file (@files_veh1)
{
				print "\n\n---->>>>>>>>>>>>$file >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
	@lines = ();
	open (FILE, "< ./$file");
	@lines = <FILE>; 
	chop (@lines);
	close (FILE);

	#trip_0_638_113102.txt
	($dum, $dum, $seg, $dum) = split/_/, $file;

	for ($i = 1; $i <= 100; $i++)
	{
		open (FILE_OUT, "> ./tripsame_${i}_${seg}_113102.txt");

		foreach $line (@lines)
		{

			#"version": "1.0",
			#"clientReference" : "Driver LT00001",
			#"psn" : "99999900000001",
			#"vehicleReference" : "VIN_LT00001",
			#"lpn" : "LT00001",
			#"lpCountry" : "E",
			#"transmissionEventId" : "1",

			$line_aux = $line;

			$count= sprintf("%05d",$i);
			my $straux;
			my $timeaux;
			if ($line_aux =~ m/clientReference/)
			{
				$straux = join("", "Driver LT", $count);
				$line_aux =~ s/Driver LT00001/$straux/;
			}
			elsif ($line_aux =~ m/psn/)
			{
				$straux = join("", "999999000", $count);
				$line_aux =~ s/99999900000001/$straux/;
			}
			elsif ($line_aux =~ m/vehicleReference/)
			{
				$straux = join("", "VIN_LT", $count);
				$line_aux =~ s/VIN_LT00001/$straux/;
			}
			elsif ($line =~ m/lpn/)
			{
				$straux = join("", "LT", $count);
				$line_aux =~ s/LT00001/$straux/;
			}
			elsif ($line_aux =~ m/recordTime/)
			{
				#"recordTime" : "1622198888",
				my ($dum, $dum, $dum, $timeaux, $dum)=split/"/, $line_aux;
				print "----$timeaux\n";
				$timeaux = $timeaux + 5 * 86400 + 4 * 3600;
				$line_aux = "                         \"recordTime\" : \"$timeaux\",";
			}
			elsif ($line_aux =~ m/transmissionTime/)
			{
				
				#"transmissionTime" : "1622198888",
				my ($dum, $dum, $dum, $timeaux, $dum)=split/"/, $line_aux;
				print "---->>>>>>>>>>>>$timeaux\n";
				$timeaux = $timeaux + 5 * 86400 + 4 * 3600;
				$line_aux = "         \"transmissionTime\" : \"$timeaux\",";
			}


		



			print FILE_OUT "$line_aux\n";
			
		}
					print FILE_OUT "}";
		close (FILE_OUT);
	}
}






