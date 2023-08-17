#!c:\Perl64\bin\perl.exe

#================================================================
# Fichero db_filler_plus_text.pl
#================================================================
#  FICHERO: db_filler_plus_text for simulator_db 
#  DESCRIPCION:
#  FECHA CREACION: 22-05-2020
#  AUTOR DISENYO: Another perl/tk masterpiece of Adrian Casini
#  PROGRAMADOR:   Adrian Casini
#================================================================



#------- Initialization ---------
$SIM_EARTH_RADIUS_KM=6371.0;
$SIM_PI=3.14159265358979323846;
$SIM_LON_REF=2.31368;
$SIM_LAT_REF=41.5942;

use DBI;
use DBD::Pg qw(:pg_types);
$HOST='localhost';
`unset LANG >> /dev/null`;

$DBH=&db_connect();


open (FILER, "< ../etc/mapper_generator.dat");
seek(FILER, 0,0 );
$count_line=0;
while ( ! eof(FILER) ) 
{
   #$count_line++;
   defined( $line = readline FILER ) or die "readline failed: $!";
   chop($line);

	if (($line !~ m/#/) && ($line !~ m/^\s*$/))
	{
	   print "[$line]\n";
		$line =~~ s/\s*//g;
		my ($key_tag, $value, $dum) = split/;/, $line;
		$hash_map_items{"$key_tag#$value"} = 1;
	}
}
close(FILER);

print "-------------------------------------------------------------------------------\n";
foreach $var (keys %hash_map_items)
{
	($KEY, $VALUE) = split/#/, $var;
	print "---------------($KEY, $VALUE)-----------\n";
	`echo ($KEY, $VALUE)`;

	$KEY_T = $KEY; 
	$KEY_T =~ s/://g;

	$VALUE_T = $VALUE; 
	$VALUE_T =~ s/://g;

	$TABLE = "Text_nodes_${KEY_T}_${VALUE_T}";
	`echo $TABLE`;
	print ">>>> $TABLE \n";

	
	$consulta_a = "DROP TABLE $TABLE"; 
    $sth = $DBH->prepare($consulta_a);
	$sth->execute(); 
	$sth->finish();


	$consulta_0 = "CREATE TABLE $TABLE 
	(

	   nid        BIGINT DEFAULT 0,
	   ver        INTEGER DEFAULT 0,
	   name       VARCHAR(200),
	   xlon       DOUBLE PRECISION,
	   ylat       DOUBLE PRECISION,


	   PRIMARY KEY (nid,ver)

	);

	CREATE INDEX SIM_${TABLE}_indx ON $TABLE (xlon, ylat);";

	`echo $consulta_0`;
	print ">>>> $consulta_0 \n";
	$sth = $DBH->prepare($consulta_0);
	$sth->execute(); 
	$sth->finish();

	
	#----------------------------------------------------------------------
	$KEY_A = "'$KEY'";
    $VALUE_A = "'$VALUE'";
	$consulta_1 = "SELECT * FROM Node_tags WHERE key_tag = $KEY_A AND value = $VALUE_A ORDER BY ver ASC;";
	`echo $consulta_1`;
	print ">>>> $consulta_1 \n";

	$sth1 = $DBH->prepare($consulta_1);
	$sth1->execute();  
	$row1 = $sth1->rows;

	%hash_nid=();
	%hash_nid_text=();
	%hash_text_nodes=();

	foreach ($i=0;$i<$row1;$i++)
	{
		@dum=();
		@dum = $sth1->fetchrow_array();
		my $str1=join('#', @dum);
		($nid,$ver, $dum, $dum)=();
		($nid,$ver, $dum, $dum)=split /#/, $str1;

		$nid =~ s/\s*//g;
		$ver =~ s/\s*//g;
		$hash_nid{$nid}=$ver; #Me quedo con la ultima version
		#print "nid: $nid -- ver: $ver\n";

	}
	$sth1->finish();
	

	foreach $nid (keys %hash_nid)
	{
		$ver = $hash_nid{$nid};
		$consulta_2 = "SELECT nid, ver, value FROM Node_tags WHERE nid = $nid AND ver = $ver and key_tag = 'name'";
		$sth2 = $DBH->prepare($consulta_2);
		$sth2->execute();  
		$row2 = $sth2->rows;

		foreach ($i=0;$i<$row2;$i++)
		{
			@dum=();
			@dum = $sth2->fetchrow_array();
			my $str2=join('#', @dum);
			($nid, $ver, $name)=();
			($nid, $ver, $name)=split /#/, $str2;
			$hash_nid_text{$nid}=$name;
			#print "nid: $nid -- name: $name\n";
		}
		$sth2->finish();
	}

	#----------------------------------------------------------------------
	foreach $nid (keys %hash_nid)
	{
		$ver = $hash_nid{$nid};
		$consulta_2 = "SELECT nid, ver, lon, lat FROM Node_pos WHERE nid = $nid AND ver = $ver";
		$sth2 = $DBH->prepare($consulta_2);
		$sth2->execute();  
		$row2 = $sth2->rows;

		foreach ($i=0;$i<$row2;$i++)
		{
			@dum=();
			@dum = $sth2->fetchrow_array();
			my $str2=join('#', @dum);
			($nid, $ver, $lon, $lat)=();
			($nid, $ver, $lon, $lat)=split /#/, $str2;
			$xlon = &getXfromLon($lon);
			$ylat = &getYfromLat($lat);

			$hash_text_nodes{$nid}="$ver#$hash_nid_text{$nid}#$xlon#$ylat";

			print "nid: $nid -- datos: $ver#$hash_nid_text{$nid}#$xlon#$ylat\n";

		}
		$sth2->finish();
	}

	print "====================================================================\n";

	
	#----------------------------------------------------------------------
	foreach $nid (keys %hash_text_nodes)
	{
		($ver, $name, $xlon, $ylat)=split /#/, $hash_text_nodes{$nid};
		$consulta_3 = "INSERT INTO $TABLE (nid,ver,name,xlon,ylat) VALUES ($nid,$ver, \'$name\', $xlon, $ylat)";

	    print "$consulta_3\n";
		$sth3 = $DBH->prepare($consulta_3);
		$sth3->execute();  
		$sth3->finish();
	}
}

&db_disconnect($DBH);

#---------------------------------------------------------------------------------------
sub getXfromLon
{
	my ($lon)=@_;
	my $x = &distanceEarth($SIM_LAT_REF, $SIM_LON_REF, $SIM_LAT_REF, $lon);
	if ($SIM_LON_REF > $lon) {$x = -$x;}

	return $x;
}
#---------------------------------------------------------------------------------------
sub getYfromLat
{
	my ($lat)=@_;
	my $y = &distanceEarth($SIM_LAT_REF, $SIM_LON_REF, $lat, $SIM_LON_REF);
	if ($SIM_LAT_REF > $lat) {$y = -$y;}

	return $y;
}

#---------------------------------------------------------------------------------------
sub deg2rad
{
	my ($deg)=@_;
	return ($deg * $SIM_PI / 180);
}

#---------------------------------------------------------------------------------------
sub rad2deg
{
	my ($rad)=@_;
	return ($rad * 180 / $SIM_PI);
}

#---------------------------------------------------------------------------------------
sub distanceEarth
{
   my ($lat1d, $lon1d, $lat2d, $lon2d)=@_;

   $lat1r = deg2rad($lat1d);
   $lon1r = deg2rad($lon1d);
   $lat2r = deg2rad($lat2d);
   $lon2r = deg2rad($lon2d);
   $u = sin(($lat2r - $lat1r) / 2.0);
   $v = sin(($lon2r - $lon1r) / 2.0);

   $dum = sqrt($u * $u + cos($lat1r) * cos($lat2r) * $v * $v);

   return (2.0 * $SIM_EARTH_RADIUS_KM * atan2($dum, sqrt(1 - $dum ** 2)));

}

#---------------------------------------------------------------------------------------
sub db_connect
{
   my ($dbh)=@_;
   $dbh = DBI->connect("dbi:Pg:dbname=simulator_db;
      host=localhost;
      port=5432",
      'adria', 
      '');

   return $dbh;
}

#---------------------------------------------------------------------------------------
sub db_disconnect
{
   my ($dbh)=@_;
   $dbh->disconnect(); 
}


