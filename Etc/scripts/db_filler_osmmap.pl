#!c:\Perl64\bin\perl.exe

#================================================================
# Fichero maptodatbase
#================================================================
#  FICHERO: maptodatbase for simulator_db 
#  DESCRIPCION:
#  FECHA CREACION: 15-10-2019
#  AUTOR DISENYO: Another perl/tk masterpiece of Adrian Casini
#  PROGRAMADOR:   Adrian Casini
#================================================================



#------- Initialization ---------
use DBI;
use DBD::Pg qw(:pg_types);

$HOST='localhost';
`unset LANG >> /dev/null`;

$DBH=&db_connect();
open (FILER, "< F:/OPENGL/Proyectos/ACSimulator/ACSimulator/maps/spain-latest.osm.xml");


$node_inside = 0;
$way_inside  = 0;
$relation_inside=0;
$count_line=0;


#In 13 000 000 000 is in the beginning of Ways
#seek(FILER, 16000000000,0 );#en bytes
#seek(FILER,  12300000000,0 );#en bytes


$count_line_loop=0;
while ( ! eof(FILER) ) 
{
   $count_line++;
   $count_line_loop++;

   defined( $line = readline FILER ) or die "readline failed: $!";
   chop($line);

   #print "[$line]\n";
   if ($count_line_loop > 10000)
   {   
	  print "[$line]\n";
      print  ">>>>>>>>>>>>>>>>>>$count_line\n"; 
	  $count_line_loop=0;
   }


   	#--------------------------------------------------------------------------------------------------------------------------
	#                                           NODES
	#--------------------------------------------------------------------------------------------------------------------------
	#<node id="13850114" lat="41.7654267" lon="2.7315251" version="5" timestamp="2017-08-21T10:52:48Z" changeset="51300106" uid="293845" user="felixi"/>
	#<node id="13850124" lat="41.6016155" lon="2.3345586" version="5" timestamp="2017-08-21T15:56:56Z" changeset="51310700" uid="293845" user="felixi"/>
	#<node id="13850126" lat="41.7292443" lon="2.6284813" version="2" timestamp="2008-11-12T21:08:27Z" changeset="738702" uid="2488" user="Alban">
	#	<tag k="created_by" v="JOSM"/>
	#</node>
	#--------------------------------------------------------------------------------------------------------------------------


	if (($line =~ m/<node/) && ($line !~ m/member/))
	{
		$node_inside=1;
		$way_inside=0;
		$relation_inside=0;

		$line=~s/^\s*//g;
		($dum,$nid,$ver,$dum, $lat, $lon, @dum)= split /\s/,$line;

		$nid =~ s/(id=|\")//g;
		$lat =~ s/(lat=|\")//g;
		$lon =~ s/(lon=|\")//g;
		$lon =~ s/\///g;
		$lon =~ s/>//g;
		$ver =~ s/(version=|\")//g;

		#print  FILEW "<node id=\"$nid\" lat=\"$lat\" lon=\"$lon\" version=\"$ver\">\n";

		my $str="INSERT INTO Node_pos (nid, ver, lon, lat) VALUES ($nid,$ver,$lon,$lat)";
		#print "$str\n";

		my $sth = $DBH->prepare($str);
		$sth->execute();  

	}
	elsif (($line =~ m/<tag/) && ($node_inside))
	{
		$line=~s/^\s*//g;
		#<tag k="created_by" v="JOSM"/>
		my ($dum,$key_tag,$dum,$value,@dum)= split /\"/,$line;
		#$key_tag  =~ s/(k=|\")//g;
		#$value  =~ s/(v=|\"|\/>)//g;


		$key_tag  =~ s/\'/\\'/g;
		$value  =~ s/\'/\\'/g;

		my @dum=();
		@dum=split //, $key_tag;
		if (scalar (@dum) >= 200)
		{  
			$key_tag = substr $key_tag, 0,199;
		}

		my @dum=();
		@dum=split //, $value;
		if (scalar (@dum) >= 200)
		{  
			$value = substr $value, 0,199;
		}

		#print  FILEW "   <tag k=\"$key_tag\" v=\"$value\" >\n";

		my $str="INSERT INTO Node_tags (nid, ver, key_tag, value) VALUES ($nid, $ver, \'$key_tag\' ,\'$value\')";
		#print "$str\n";
		my $sth = $DBH->prepare($str);
		$sth->execute();  

	}
	elsif ($line =~ m/<\/node/) 
	{
		$way_inside=0;
		$node_inside=0;
		$relation_inside=0;
		#print FILEW "</node>\n";
	} 
	#--------------------------------------------------------------------------------------------------------------------------
	#                                           WAYS
	#--------------------------------------------------------------------------------------------------------------------------
	#        <way id="42386059" version="4" timestamp="2018-03-29T19:45:42Z" changeset="57643956" uid="581863" user="pedrobv">
	#          <nd ref="4519406632"/>
	#          <nd ref="309993539"/>
	#          <nd ref="309993569"/>
	#          <tag k="highway" v="tertiary"/>
	#          <tag k="lanes" v="2"/>
	#          <tag k="ref" v="GIV-5341"/>
	#        </way>
	#--------------------------------------------------------------------------------------------------------------------------

	elsif (($line =~ m/<way/) && ($line !~ m/member/))
	{
		print "------------------------------------------------------\n";
		print "---------------  $line    ---------------\n";
		exit;

		$way_inside=1;
		$node_inside=0;
		$relation_inside=0;

		$line=~s/^\s*//g;
		($dum,$wid,$ver,@dum)= split /\s/,$line;

		$wid  =~ s/(id=|\")//g;
		$ver  =~ s/(version=|\")//g;

		#print FILEW  "<way id=\"$wid\" version=\"$ver\">\n";

	}
	elsif (($line =~ m/<nd ref/) && ($way_inside))
	{
		$line=~s/^\s*//g;
		my ($dum,$node,@dum)= split /\s/,$line;
		$node  =~ s/(ref=|\"|\/>)//g;
		push (@${wid}, $node);
		$count_node++;
 
		#print FILEW  "   <nd ref=\"$node\" >\n";

	}
	elsif (($line =~ m/<tag/) && ($way_inside))
	{
		$line=~s/^\s*//g;
		#<tag k="ref" v="GIV  5341"/>
		my ($dum,$key_tag,$dum,$value,@dum)= split /\"/,$line;
		#$key_tag  =~ s/(k=|\")//g;
		#$value  =~ s/(v=|\"|\/>)//g;

		$key_tag  =~ s/\'/\\'/g;
		$value  =~ s/\'/\\'/g;

		my @dum=();
		@dum=split //, $key_tag;
		if (scalar (@dum) >= 200)
		{  
			$key_tag = substr $key_tag, 0,199;
		}

		my @dum=();
		@dum=split //, $value;
		if (scalar (@dum) >= 200)
		{  
			$value = substr $value, 0,199;
		}

		$hash_way_tag{"$wid"}{$key_tag}=$value;
      
		#print FILEW "   <tag k=\"$key_tag\" v=\"$value\" >\n";
	}
	elsif ($line =~ m/<\/way/) 
	{
		$way_inside=0;
		$node_inside=0;
		$relation_inside=0;
		#print FILEW "</way>\n";

      
		my $str = "INSERT INTO Ways (wid, ver, nfirst, nlast) VALUES ($wid,$ver, ${${wid}}[0] ,${${wid}}[$count_node-1])";
		#print "$str\n";
		my $sth = $DBH->prepare($str);
		$sth->execute(); 


		my $contador=0;
		foreach $nid (@${wid})
		{
			my $str = "INSERT INTO Way_nodes (wid, ver, norder, nid) VALUES ($wid,$ver, $contador,$nid)";
			#print "$str\n";
			my $sth = $DBH->prepare($str);
			$sth->execute(); 

			my $w_ver = $ver;
			my $str = "INSERT INTO Node_ways (nid,wid,w_ver,w_norder) VALUES ($nid,$wid,$w_ver,$contador)";
			#print "$str\n";
			my $sth = $DBH->prepare($str);
			$sth->execute(); 

			$contador++;
		}

		foreach $key_tag (keys %{$hash_way_tag{$wid}})
		{
			my $str = "INSERT INTO Way_tags (wid, ver, key_tag, value) VALUES ($wid, $ver,\'$key_tag\', \'$hash_way_tag{$wid}{$key_tag}\')";
			#print "$str\n";
			my $sth = $DBH->prepare($str);
			$sth->execute(); 
		}

		%hash_way_tag=();
		@${wid}=();
		$count_node=0;
	}
	#--------------------------------------------------------------------------------------------------------------------------
	#                                           RELATIONS
	#--------------------------------------------------------------------------------------------------------------------------
	#<relation id="8643" version="586" timestamp="2020-02-13T04:18:31Z" changeset="80933065" uid="90780" user="Verdy_p">
	# 	<member type="node" ref="1251610256" role="label"/>
	#	<member type="node" ref="65442261" role="admin_centre"/>
	#	<member type="way" ref="50344406" role="outer"/>
	#	<member type="way" ref="23301599" role="outer"/>
	#	<member type="way" ref="209513139" role="outer"/>
	#	<member type="way" ref="131749302" role="outer"/>
	#	<tag k="name:lt" v="Ryt Pirnai"/>
	#	<tag k="type" v="boundary"/>
	#	<tag k="wikidata" v="Q12709"/>
	#</relation>
	#--------------------------------------------------------------------------------------------------------------------------

	elsif ($line =~ m/<relation/) 
	{
		$way_inside=0;
		$node_inside=0;
		$relation_inside=1;
	  
		$line=~s/^\s*//g;
		($dum,$rid,$ver,@dum)= split /\s/,$line;

		$rid  =~ s/(id=|\")//g;
		$ver  =~ s/(version=|\")//g;
	}
	elsif (($line =~ m/<member/)  && ($relation_inside))
	{
		$line=~s/^\s*//g;
		my ($dum,$type,$dum,$ref,$dum,$role,@dum)= split /\"/,$line;
		my $txt  = "$type-$ref-$role";
		push (@${rid}, $txt);
	}
	elsif (($line =~ m/<tag/) && ($relation_inside))
	{

		$line=~s/^\s*//g;
		#<tag k="wikidata" v="Q12709"/>
		my ($dum,$key_tag,$dum,$value,@dum)= split /\"/,$line;

		$key_tag  =~ s/\'/\\'/g;
		$value  =~ s/\'/\\'/g;

		my @dum=();
		@dum=split //, $key_tag;
		if (scalar (@dum) >= 200)
		{  
			$key_tag = substr $key_tag, 0,199;
		}

		my @dum=();
		@dum=split //, $value;
		if (scalar (@dum) >= 200)
		{  
			$value = substr $value, 0,199;
		}

		$hash_relation_tag{"$rid"}{$key_tag}=$value;
      
		#print FILEW "   <tag k=\"$key_tag\" v=\"$value\" >\n";

	}
	elsif ($line =~ m/<\/relation/) 
	{
		$way_inside=0;
		$node_inside=0;
		$relation_inside=0;

		my $str = "INSERT INTO Relations (rid, ver) VALUES ($rid,$ver)";
		#print "$str\n";
		my $sth = $DBH->prepare($str);
		$sth->execute(); 

		my $norder=0;
		foreach $txt (@${rid})
		{
			($type,$ref,$role)=split /-/, $txt;
			#if ($role eq "") {$role="no-existe";}
			my $str = "INSERT INTO Relation_members (rid, ver, ref, norder, type, role) VALUES ($rid,$ver,$ref,$norder,\'$type\',\'$role\')";
			#print "$str\n";
			my $sth = $DBH->prepare($str);
			$sth->execute();   
			$norder++;
		}

		foreach $key_tag (keys %{$hash_relation_tag{$rid}})
		{
			my $str = "INSERT INTO Relation_tags (rid, ver, key_tag, value) VALUES ($rid, $ver,\'$key_tag\', \'$hash_relation_tag{$rid}{$key_tag}\')";
			#print "$str\n";
			my $sth = $DBH->prepare($str);
			$sth->execute(); 
		}

		%hash_relation_tag=();
		@${rid}=();

	}
}

close (FILER);
#close (FILEW);
&db_disconnect($DBH);


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


