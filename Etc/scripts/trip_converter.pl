#!c:\Perl64\bin\perl.exe

#================================================================
# Fichero 
#================================================================
#  FICHERO: 
#  DESCRIPCION:
#  FECHA CREACION: 
#  AUTOR DISENYO: Another perl masterpiece of Adrian Casini
#  PROGRAMADOR:   Adrian Casini
#================================================================

use IO::File;

@files=`ls trip_*`;
chop(@files);

foreach $file (@files)
{
	print "$file\n";
}
exit;


foreach $file (@files)
{
	@lines=();
	open (FILE_IN, "< $file");
	@lines = <FILE_IN>;
	close(FILE_IN);
	chop (@lines);

	open (FILE_OUT, "> cvformat_${file}");

	#*HQ,830202000200000,V1,101633,A,4144.7676,N,00239.4531,E,049.7,000,050321,FFFFFFFF#

	foreach $line (@lines)
	{
		@dum=();
		my ($dum, $dum,$dum,$dum,$dum,$lat,$dum,$lon,$dum,$dum,$dum,$dum,$dum)=split/,/,$line;
		#print "$lat -- $lon\n";

		my ($lat_g, $lat_d) = split/\./,$lat;
		@dum = split//, $lat_g;
		$lat_g = join("", $dum[0], $dum[1]);

		$lat_d_1=join("", $dum[2], $dum[3]);

		$lat_d = $lat_d / 10000.0;
		$lat_d = ($lat_d + $lat_d_1) / 60.0;
		$lat = $lat_g +  $lat_d;
	

		my ($lon_g, $lon_d) = split/\./,$lon;
		@dum = split//, $lon_g;
		$lon_g = join("", $dum[0], $dum[1], $dum[2]);
		$lon_d_1=join("", $dum[3], $dum[4]);

		$lon_d = $lon_d / 10000.0;
		$lon_d = ($lon_d + $lon_d_1) / 60.0;
		$lon = $lon_g +  $lon_d;
	

		print FILE_OUT "$lat,$lon\n";
	}
	close(FILE_OUT);

}

