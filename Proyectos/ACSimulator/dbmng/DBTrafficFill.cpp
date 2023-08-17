#include "DBTrafficFill.h"


//------------------------------------------------------------------------------
DBTrafficFill::DBTrafficFill()
{
	mpctrl = NULL;
}

//------------------------------------------------------------------------------
DBTrafficFill::~DBTrafficFill()
{
	mdb.desconectar();
}

//------------------------------------------------------------------------------
void DBTrafficFill::init(Control* pctrl)
{
	
	mpctrl = pctrl;
	mprocessed = 0;
	mprocessed_old = 0;

	if (mdb.conectar(SIM_DB_HOST, SIM_DB_PORT, mpctrl->mdb_name, SIM_DB_USER, "", SIM_DB_TIMEOUT) != true) { return; }
}

//------------------------------------------------------------------------------
void DBTrafficFill::execute()
{
	//Tipo de Mensaje; Version; Numero de Estacion; Numero de Via; Sentido; Tipo de Via; Momento       ; Estado Actual; Modo Actual; Tabulacion Cobrador; Clase; Categoria Tarifaria; Importe Propio; Importe Ajeno; "Moneda "; Decimales; IVA ; Forma de Pago Final; Simulacion de Paso; Contenido Tarjeta u OBE   ; Tabulacion Inicial; Forma de Pago Inicial; Tipo Ticket Entrada; Version Ticket Entrada; Numero Secuencial de Ticket; Estacion de Entrada; Via de Entrada; Momento de Entrada; Unidad Año Entrada; Numero de M5; Tiempo de Servicio; Tiempo de Transaccion; Resultado Captura SRM; Matricula SRM
	//TO             ; 06     ; 0102              ; 05           ; S      ; CA         ; 20170101001814; AB           ; 2000       ; T2                 ; 02   ; 1                  ; 000030        ; 000000       ; EU       ; 2        ; 2100; 06                 ; N                 ; "3085072131828031=200831  "; "  "             ; "  "                 ; O                  ; " "                   ; "    "                     ; 0104               ; 01            ; 0010016           ; 7                 ; "       "   ; 000045            ; 000074               ; 5                    ; "2940JJP"

	Cadena tipomsj, sentido, tipovia, estadoactual, modoactual, tabulacioncob, moneda, simulasionpaso, contenidotarjobe, tabulacioninicial, tipoticketentrada, matriculasrm;
	int version, numeroestacion, numerovia, clase, categoriatarif, decimales, iva, formapagofinal, formapagoinicial, versionticketentrada, numerosecuencialticket, estacionentrada, viaentrada, momentoentrada, unidadanyoentrada, numerom5, tiemposervicio, tiempotransaccion, resulatdocapturasrm;
	float importepropio, importeajeno;
	//long long int momento;
	Cadena momento;
	long long int traffic_count_ok = 0, traffic_count_nok = 0;
	
	/*CREATE TABLE Transitos
	(
		tipomsj                      VARCHAR(2),
		version                      INTEGER DEFAULT 0,
		numeroestacion               INTEGER DEFAULT 0,
		numerovia                    INTEGER DEFAULT 0,
		sentido                      VARCHAR(1),
		tipovia                      VARCHAR(2),
		momento                      TIMESTAMP,
		estadoactual                 VARCHAR(2),
		modoactual                   VARCHAR(4),
		tabulacioncob                VARCHAR(2),
		clase                        INTEGER DEFAULT 0,
		categoriatarif               INTEGER DEFAULT 0,
		importepropio                REAL,
		importeajeno                 REAL,
		moneda                       VARCHAR(2),
		decimales                    INTEGER DEFAULT 0,
		iva                          INTEGER DEFAULT 0,
		formapagofinal               INTEGER DEFAULT 0,
		simulasionpaso               VARCHAR(1),
		contenidotarjobe             VARCHAR(100),
		tabulacioninicial            VARCHAR(2),
		formapagoinicial             INTEGER DEFAULT 0,
		tipoticketentrada            VARCHAR(1),
		versionticketentrada         INTEGER DEFAULT 0,
		numerosecuencialticket       INTEGER DEFAULT 0,
		estacionentrada              INTEGER DEFAULT 0,
		viaentrada                   INTEGER DEFAULT 0,
		momentoentrada               INTEGER DEFAULT 0,
		unidadanyoentrada            INTEGER DEFAULT 0,
		numerom5                     INTEGER DEFAULT 0,
		tiemposervicio               INTEGER DEFAULT 0,
		tiempotransaccion            INTEGER DEFAULT 0,
		resulatdocapturasrm          INTEGER DEFAULT 0,
		matriculasrm                 VARCHAR(20),

		PRIMARY KEY(tipomsj, numeroestacion, numerovia, momento)
	);*/


	string line, substr;
	vector<Cadena> vtr;
	vector<Cadena>::const_iterator it;
	Cadena linec, cadquery, cad;
	const char* pc = NULL;
	int timestp;
	int line_long, line_long_trim = 0;

	ifstream f(mpctrl->mtr_source.getCadena());
	if (!f.is_open())
	{
		cout << "Error opening file:[" << mpctrl->mfilemap.getCadena() << "]" << endl;
		return;
	}

	f.seekg(mpctrl->mfseek, ios::beg);
	f.seekg(0, ios::beg);


	while (getline(f, line))
	{
		line_long = linec.getLong();
		mprocessed = mprocessed + line_long;
		linec = line.c_str();
		linec.lTrim();
		linec.rTrim();
		line_long_trim = linec.getLong();
		pc = linec.getCadena();

		if ((*pc == 'T') && 
			(
				(*(pc + 1) == 'O') ||
				(*(pc + 1) == 'V') ||
				(*(pc + 1) == 'R') ||
				(*(pc + 1) == 'L') ||
				(*(pc + 1) == 'I')
			)
		)
		{
			//cout << "[" << linec.c_str() << "]" << endl;
			stringstream ss(linec.getCadena());
			vtr.clear();
			while (ss.good())
			{
				getline(ss, substr, ';');
				cad = substr.c_str();
				cad.rTrim();
				cad.lTrim();
				vtr.push_back(cad);
			}


			tipomsj        = vtr[0].getCadena(); 
			version        = atoi(vtr[1].getCadena());
			numeroestacion = atoi(vtr[2].getCadena());
			numerovia      = atoi(vtr[3].getCadena());
			sentido        = vtr[4].getCadena();
			tipovia        = vtr[5].getCadena();
			momento        = vtr[6].getCadena();//atoll(vtr[6].getCadena());
			estadoactual   = vtr[7].getCadena();
			modoactual     = vtr[8].getCadena();
			tabulacioncob  = vtr[9].getCadena();
			clase          = atoi(vtr[10].getCadena());
			categoriatarif = atoi(vtr[11].getCadena());
			importepropio  = atof(vtr[12].getCadena());
			importeajeno   = atof(vtr[13].getCadena());
			moneda         = vtr[14].getCadena();
			decimales      = atoi(vtr[15].getCadena());
			iva            = atoi(vtr[16].getCadena());
			formapagofinal = atoi(vtr[17].getCadena());
			simulasionpaso         = vtr[18].getCadena();
			contenidotarjobe = vtr[19].getCadena(); 
			contenidotarjobe.borraCaracter(1, ' '); 
			contenidotarjobe.borraCaracter(contenidotarjobe.getLong(), ' '); 
			contenidotarjobe.lTrim(); 
			contenidotarjobe.rTrim();
			tabulacioninicial      = vtr[20].getCadena();
			tabulacioninicial.borraCaracter(1, ' ');
			tabulacioninicial.borraCaracter(tabulacioninicial.getLong(), ' ');
			tabulacioninicial.lTrim();
			tabulacioninicial.rTrim();
			formapagoinicial       = atoi(vtr[21].getCadena());
			tipoticketentrada      = vtr[22].getCadena();
			tipoticketentrada.borraCaracter(1, ' ');
			tipoticketentrada.borraCaracter(tipoticketentrada.getLong(), ' ');
			tipoticketentrada.lTrim();
			tipoticketentrada.rTrim();
			versionticketentrada   = atoi(vtr[23].getCadena());
			numerosecuencialticket = atoi(vtr[24].getCadena());
			estacionentrada        = atoi(vtr[25].getCadena());
			viaentrada             = atoi(vtr[26].getCadena());
			momentoentrada         = atoi(vtr[27].getCadena());
			unidadanyoentrada      = atoi(vtr[28].getCadena());
			numerom5               = atoi(vtr[29].getCadena());
			tiemposervicio         = atoi(vtr[30].getCadena());
			tiempotransaccion      = atoi(vtr[31].getCadena());
			resulatdocapturasrm    = atoi(vtr[32].getCadena());
			matriculasrm           = vtr[33].getCadena();
			matriculasrm.borraCaracter(1, ' ');
			matriculasrm.borraCaracter(matriculasrm.getLong(), ' ');
			matriculasrm.lTrim();
			matriculasrm.rTrim();

			/*
			cout << "[" << tipomsj.getCadena() << "]" << endl;
			cout << version << endl;
			cout << numeroestacion << endl;
			cout << numerovia << endl;
			cout << sentido.getCadena() << endl;
			cout << tipovia.getCadena() << endl;
			cout << momento.getCadena() << endl;
			cout << estadoactual.getCadena() << endl;
			cout << modoactual.getCadena() << endl;
			cout << tabulacioncob.getCadena() << endl;
			cout << clase << endl;
			cout << categoriatarif << endl;
			cout << importepropio << endl;
			cout << importeajeno << endl;
			cout << moneda.getCadena() << endl;
			cout << decimales << endl;
			cout << iva << endl;
			cout << formapagofinal << endl;
			cout << simulasionpaso.getCadena() << endl;
			cout << "[" << contenidotarjobe.getCadena() << "]" << endl;
			cout << tabulacioninicial.getCadena() << endl;
			cout << formapagoinicial << endl;
			cout << tipoticketentrada.getCadena() << endl;
			cout << versionticketentrada << endl;
			cout << numerosecuencialticket << endl;
			cout << estacionentrada << endl;
			cout << viaentrada << endl;
			cout << momentoentrada << endl;
			cout << unidadanyoentrada << endl;
			cout << numerom5 << endl;
			cout << tiemposervicio << endl;
			cout << tiempotransaccion << endl;
			cout << resulatdocapturasrm << endl;
			cout << matriculasrm.getCadena() << endl;
			*/

			cadquery.formCadena("INSERT INTO Transitos (tipomsj,version,numeroestacion,numerovia,sentido,tipovia,momento,estadoactual,modoactual, tabulacioncob, clase, categoriatarif, importepropio, importeajeno,\
                                 moneda, decimales, iva, formapagofinal, simulasionpaso, contenidotarjobe, tabulacioninicial, formapagoinicial, tipoticketentrada, versionticketentrada,\
                                 numerosecuencialticket, estacionentrada,viaentrada, momentoentrada, unidadanyoentrada, numerom5, tiemposervicio, tiempotransaccion, resulatdocapturasrm, matriculasrm)\
                                VALUES\
                                ('%s',%d, %d, %d,'%s','%s','%s','%s','%s','%s',%d,%d,%f,%f,\
                                 '%s',%d, %d, %d,'%s','%s','%s',%d,'%s',%d,\
                                 %d,%d,%d,%d,%d,%d,%d,%d,%d,'%s')",
				                 tipomsj.getCadena(),version,numeroestacion,numerovia,sentido.getCadena(),tipovia.getCadena(),momento.getCadena(),estadoactual.getCadena(),modoactual.getCadena(),tabulacioncob.getCadena(),clase,categoriatarif,importepropio,importeajeno,
                                 moneda.getCadena(),decimales,iva,formapagofinal,simulasionpaso.getCadena(),contenidotarjobe.getCadena(),tabulacioninicial.getCadena(),formapagoinicial,tipoticketentrada.getCadena(),versionticketentrada,
                                 numerosecuencialticket,estacionentrada,viaentrada,momentoentrada,unidadanyoentrada,numerom5,tiemposervicio,tiempotransaccion,resulatdocapturasrm,matriculasrm.getCadena());


			if (!mdb.ejecutarConsulta(cadquery)) 
			{
				//cout << cadquery.getCadena() << endl;
				cout << "-ntr";
				traffic_count_nok++;
			}
			else
			{
				traffic_count_ok++;
			}
		}


		if (mprocessed > mprocessed_old + 1000000)
		{
			mprocessed_old = mprocessed;
			cout << endl;
			cout << "Bytes read:[" << mprocessed << "]" << endl;
			cout << "Traffic messages processed ok:[" << traffic_count_ok << "]" << endl;
			cout << "Traffic messages processed nk:[" << traffic_count_nok << "]" << endl;
		}

		/*if (mpctrl->mbfseek == true)
		{
			if (mprocessed > 1000000000 + 20000)
			{
				cout << "  Finel mprocessed bytes:[" << mprocessed << "]" << endl;
				f.close();
				return;
			}
		}*/
	}


	f.close();

}

//------------------------------------------------------------------------------
int DBTrafficFill::get_timestamp_num(Cadena& cadtimestamp)
{
	vector<Cadena> vtime;
	string line, substr;
	int timestp;

	cadtimestamp.rTrim();
	cadtimestamp.borraCaracter(11, ' ');
	cadtimestamp.borraCaracter(20, '\0');
	cadtimestamp.ReemplazarCadena("-", ":");
	cadtimestamp.ReemplazarCadena("-", ":");
	cadtimestamp.ReemplazarCadena(" ", ":");
	stringstream ss(cadtimestamp.getCadena());
	vtime.clear();

	while (ss.good()) { getline(ss, substr, ':'); vtime.push_back(substr.c_str()); }
	cadtimestamp = vtime[0] + vtime[1] + vtime[2];// +vtime[3] + vtime[4] + vtime[5];
	timestp = cadtimestamp.toUDWordType();
	return timestp;
}

