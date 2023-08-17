#!c:\Perl64\bin\perl.exe
#================================================================
# Fichero server_sock
#================================================================
#
#  SERVIABERTIS   -  Des. & Exp. Software.   (Derechos reservados)
#  ---------------------------------------
#
#  FICHERO: senvio
#
#  DESCRIPCION:
#    Script en Perl que permite ejecutar comandos a distancia
#
#
#
#  FECHA CREACION: 20-09-2007
#
#  AUTOR DISENYO: Adrian Casini
#
#  PROGRAMADOR:  Adrian Casini
#
#  MODIFICATIONES:
#
#     |   FECHA    |   AUTOR   |  RAZON                                   |
#     ---------------------------------------------------------------------
#     | DD-MM-YYYY |        |                   |
#     ---------------------------------------------------------------------


#------- Initialization & Gral Variables ---------
($NAME, $THISDIR) = ($0 =~ m#^(.*)[/|\\]([^/|\\]+)$#o) ? ($2, $1) : ($0, '.');
$|  = 1;

$port = 5556;
$AF_INET = 2;
$SOCK_STREAM = 1;
$sockaddr = 'S n a4 x8';
($name, $aliases, $proto) = getprotobyname('tcp');
if ($port !~ /^\d+$/) {
   ($name, $aliases, $port) = getservbyport($port, 'tcp');
}

$this = pack($sockaddr, $AF_INET, $port, "\0\0\0\0");
select(NS); $| = 1; select(stdout);
socket(S, $AF_INET, $SOCK_STREAM, $proto) || print ("Can't call socket: $!");
bind(S, $this) || print ("Can't bind socket: $this: $!");
listen(S, 20) || print ("Can't connect socket: $!");
select(S); $| = 1; select(stdout);

for ( ; ; )
{
   unless ($addr = accept(NS,S))
   {
      print ("Error  ");
   }
   ($af, $port, $inetaddr) = unpack($sockaddr, $addr);
   @inetaddr = unpack('C4', $inetaddr);

   while(<NS>)
   {
      chop($cmd = $_);
      &process_cmd($cmd);
      print NS "::EOD::\n";
   }
   close(NS);


   print "jhjhjdjhss\n";
}

sub process_cmd
{
   my ($cmd) = @_;
   print "$cmd\n";
#   @list = `ls`;
#   foreach $var (@list)
#   {
#      print NS "$var";
#   }
}










