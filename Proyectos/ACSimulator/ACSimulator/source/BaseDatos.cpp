
#include "BaseDatos.h"
#include "lang/throwable/ComunicacionExcepcion.h"
#include "lang/throwable/ParametroExcepcion.h"
#include "lang/throwable/RangoExcepcion.h"
#include "lang/throwable/NoAplicaExcepcion.h"
#include <stdarg.h>


const char* BaseDatos::HOST        = "localhost";
const int   BaseDatos::PUERTO      = 5432;
const char* BaseDatos::NOMBRE_BD   = "simulator_db";
const char* BaseDatos::USUARIO     = "adria";
const char* BaseDatos::CONTRASENYA = "";


//------------------------------------------------------------------------------
BaseDatos::BaseDatos(bool autoConectar, int timeout)
	: mDB(NULL), mResult(NULL), mNumBegins(0), mStatusTransaccion(true)
{
	if (autoConectar)
	{
		if (!conectar(HOST, PUERTO, NOMBRE_BD, USUARIO, CONTRASENYA, timeout))
			throw ComunicacionExcepcion(__AQUI__, "No se ha podido conectar a la base de datos");
	}

}

//------------------------------------------------------------------------------
BaseDatos::BaseDatos(const Cadena& host, int puerto, const Cadena& nombre,
	const Cadena& usuario, const Cadena& contrasenya, int timeout)
	: mDB(NULL), mResult(NULL)
{
	if (!conectar(host, puerto, nombre, usuario, contrasenya, timeout))
		throw ComunicacionExcepcion(__AQUI__, "No se ha podido conectar a la base de datos");
}

//------------------------------------------------------------------------------
BaseDatos::BaseDatos(const BaseDatos& db)
	: mDB(NULL), mResult(NULL), mNumBegins(0), mStatusTransaccion(true)
{
	copiar(db);
}

//------------------------------------------------------------------------------
BaseDatos::~BaseDatos()
{
	desconectar();
}

//------------------------------------------------------------------------------
void BaseDatos::operator = (const BaseDatos& db)
{
	desconectar();
	copiar(db);
}

//------------------------------------------------------------------------------
bool BaseDatos::conectar(int timeout)
{
	return conectar(HOST, PUERTO, NOMBRE_BD, USUARIO, CONTRASENYA, timeout);
}

//------------------------------------------------------------------------------
bool BaseDatos::conectar(const Cadena& host, int puerto, const Cadena& nombre,
	const Cadena& usuario, const Cadena& contrasenya, int timeout)
{
	if (this->mDB != NULL)
	{
		clearResult();
		PQfinish(this->mDB);
		std::cout << ".." << endl;
	}

	this->mCadenaConexion.formCadena("host=%s port=%u dbname=%s user=%s password=%s  connect_timeout=%u", host.getCadena(), puerto, nombre.getCadena(), usuario.getCadena(), contrasenya.getCadena(),  timeout);

	this->mDB = PQconnectdb(this->mCadenaConexion.getCadena());
	//this->mDB = PQconnectdb(cadConexion.getCadena());
	if (this->mDB == NULL || PQstatus(this->mDB) != CONNECTION_OK)
	{
		if (this->mDB != NULL)
		{
			std::cout << " Error:  " << PQerrorMessage(this->mDB) << endl;
			PQfinish(this->mDB);
		}
		std::cout << " No se ha podido conectar a la base de dato " << endl;

		//	"No se ha podido conectar a la base de datos %s:%u (%s)",
		//	host.getCadena(), puerto, nombre.getCadena());
		this->mDB = NULL;
		return false;
	}
	else
	{
		PQsetNoticeReceiver(this->mDB, &noticeReceiver, NULL);
		PQsetNoticeProcessor(this->mDB, &noticeProcessor, NULL);
		//std::cout << "Conexion establecida" << endl;
		return true;
	}
}

//------------------------------------------------------------------------------
void BaseDatos::desconectar()
{
	if (this->mDB != NULL)
	{
		clearResult();
		PQfinish(this->mDB);
		this->mDB = NULL;
		this->mNumBegins = 0;
		this->mStatusTransaccion = true;
	}
}

//------------------------------------------------------------------------------
void BaseDatos::begin()
{
	if (this->mNumBegins == 0)
		this->mStatusTransaccion = comando("BEGIN");
	this->mNumBegins++;
}

//------------------------------------------------------------------------------
bool BaseDatos::commit()
{
	bool ok;

	if (this->mNumBegins > 0)
		this->mNumBegins--;
	else
		throw NoAplicaExcepcion(__AQUI__, "No hay ninguna transaccion en curso");

	if (this->mNumBegins == 0)
	{
		if (this->mStatusTransaccion)
			this->mStatusTransaccion = comando("COMMIT");
		else
			comando("ROLLBACK");
	}

	ok = this->mStatusTransaccion;
	if (this->mNumBegins == 0)
		this->mStatusTransaccion = true;

	return ok;
}

//------------------------------------------------------------------------------
void BaseDatos::rollback()
{
	if (this->mNumBegins > 0)
		this->mNumBegins--;
	else
		throw NoAplicaExcepcion(__AQUI__, "No hay ninguna transaccion en curso");

	if (this->mNumBegins == 0)
	{
		comando("ROLLBACK");
		this->mStatusTransaccion = true;
	}
}

//------------------------------------------------------------------------------
bool BaseDatos::hayTransaccion() const
{
	return (this->mNumBegins > 0);
}

//------------------------------------------------------------------------------
void BaseDatos::actualizarEstadisticas(const Cadena& tabla)
{
	Cadena sql;

	sql.formCadena("VACUUM ANALYZE %s;", tabla.getCadena());
	comando(sql.getCadena());
}

//------------------------------------------------------------------------------
Cadena BaseDatos::sanearDato( const Cadena& dato)
{
	//int index = dato.getLong();
	//char buffer[index * 2 + 1];

	char buffer[100];

	PQescapeString(buffer, dato.getCadena(), dato.getLong());

	return Cadena(buffer);
}



//------------------------------------------------------------------------------
Cadena BaseDatos::booleano(bool valor)
{
	return (valor ? "TRUE" : "FALSE");
}


//------------------------------------------------------------------------------
Cadena BaseDatos::entero(int valor)
{
	return enteroOString(valor, "0", 0);
}
//------------------------------------------------------------------------------
Cadena BaseDatos::enteroONulo(int valor, int valorNulo)
{
	return enteroOString(valor, "NULL", valorNulo);
}
//------------------------------------------------------------------------------
Cadena BaseDatos::enteroOString(int valor, Cadena string, int valorNulo)
{
	if (valor == valorNulo)
		return string;
	else
	{
		char buffer[32];

		sprintf(buffer, "%d", valor);
		return buffer;
	}
}

//------------------------------------------------------------------------------
Cadena BaseDatos::caracter(char valor)
{
	return caracterOString(valor, " ");
}
//------------------------------------------------------------------------------
Cadena BaseDatos::caracterONulo(char valor, char valorNulo)
{
	return caracterOString(valor, "NULL", valorNulo);
}
//------------------------------------------------------------------------------
Cadena BaseDatos::caracterOString(char valor, Cadena string, char valorNulo)
{
	if (valor == valorNulo)
		return string;
	else
	{
		char buffer[4];
		sprintf(buffer, "'%c'", valor);
		return buffer;
	}
}


//------------------------------------------------------------------------------
Cadena BaseDatos::cadenaONulo(Cadena valor)
{
	if (valor.trim().isEmpty()) return "NULL";
	return cadena(valor);
}
//------------------------------------------------------------------------------
Cadena BaseDatos::cadena(Cadena valor)
{
	return Cadena().formCadena("'%s'", sanearDato(valor.trim()).getCadena());
}

//------------------------------------------------------------------------------
bool BaseDatos::prepararConsulta(const Cadena& nombre, const Cadena& sql)
{
	PGresult* res;

	res = PQprepare(this->mDB, nombre.getCadena(), sql.getCadena(), 0, NULL);

	return (checkResultado(res, sql.getCadena()));
}

//------------------------------------------------------------------------------
bool BaseDatos::ejecutarConsultaPreparada( Cadena& nombre, int nParams,	 Cadena params[])
{
	int i;
	//char* p[nParams];
	//int l[nParams];


    char* p[1000];
	int l[100];

	if (this->mNumBegins > 0 && !this->mStatusTransaccion)
		return false;

	for (i = 0; i < nParams; i++)
	{
		p[i] = (char*)params[i].getCadena();
		l[i] = params[i].getLong();
	}
	clearResult();
	this->mResult = PQexecPrepared(this->mDB, nombre.getCadena(), nParams, p, l, NULL, FORMATO_TEXTO);

	this->mStatusTransaccion = checkResultado(this->mResult, nombre.getCadena());

	return this->mStatusTransaccion;
}

//------------------------------------------------------------------------------
bool BaseDatos::ejecutarConsulta( Cadena& sql)
{
	if (this->mNumBegins > 0 && !this->mStatusTransaccion)
		return false;

	this->mStatusTransaccion = consulta(sql.getCadena());

	return this->mStatusTransaccion;
}

//------------------------------------------------------------------------------
bool BaseDatos::ejecutarConsulta( char* formato, ...)
{
	bool    ok;
	char*   sql;
	int     n, size;
	va_list argv;

	if (this->mNumBegins > 0 && !this->mStatusTransaccion)
		return false;

	n = 512;
	sql = NULL;
	do
	{
		va_start(argv, formato);
		size = (n + 1);
		sql = (char*)realloc(sql, size);
		n = vsnprintf(sql, size, formato, argv);
		va_end(argv);
	} while (n >= size);

	ok = false;
	if (sql != NULL)
	{
		try
		{
			ok = consulta(sql);
		}
		catch (Excepcion& e)
		{
			//STrz::excepcion(ConstTraza::T_TRAZA_BD, e);
		}
		free(sql);
	}
	this->mStatusTransaccion = ok;

	return ok;
}

//------------------------------------------------------------------------------
/*int BaseDatos::esperaNotificacion(const Cadena& notificacion, int timeout)
{
	int         sock;
	fd_set      input_mask;
	PGnotify   *notify;
	bool        notifyCapturada = false;
	struct timeval vtimeout;
	int rv;

	if (!comando(Cadena("LISTEN ") + notificacion))
	{
		//STrz::error(ConstTraza::T_TRAZA_BD, "Notify %s, in LISTEN\n ", notificacion.getCadena());
		return -1; //error
	}

	sock = PQsocket(mDB);
	if (sock < 0)
	{
		//STrz::error(ConstTraza::T_TRAZA_BD, "Notify %s, in SOCK\n ", notificacion.getCadena());
		return -1; // shouldn't happen
	}

	FD_ZERO(&input_mask);
	FD_SET(sock, &input_mask);

	vtimeout.tv_sec = timeout;
	vtimeout.tv_usec = 0;

	rv = select(sock + 1, &input_mask, NULL, NULL, &vtimeout);
	if (rv < 0)
	{ // shouldn't happen
		//STrz::error(ConstTraza::T_TRAZA_BD, "Notify %s, select() failed: %s\n ", notificacion.getCadena(), strerror(errno));
	}
	else if (rv == 0)
	{ //timeout
		//STrz::warning(ConstTraza::T_TRAZA_BD, "timeout Notify [%s]", notificacion.getCadena());
	}
	else
	{ // Now check for input
		PQconsumeInput(mDB);
		while (!notifyCapturada && (notify = PQnotifies(mDB)) != NULL)
		{
			//STrz::basic(ConstTraza::T_TRAZA_BD, "[Notify '%s'] ASYNC NOTIFY of '%s' received from backend PID %d\n",notificacion.getCadena(), notify->relname, notify->be_pid);
			notifyCapturada = (Cadena(notify->relname) == notificacion.getCadena());
			PQfreemem(notify);
		}
	}

	//comando (Cadena("UNLISTEN ") + notificacion);
	if (!comando(Cadena("UNLISTEN ") + notificacion))
	{
		//STrz::error(ConstTraza::T_TRAZA_BD, "Notify %s, in UNLISTEN\n ", notificacion.getCadena());
		return -1; //error
	}

	return rv == 1 ? (notifyCapturada ? 1 : 0) : rv;
}*/

//------------------------------------------------------------------------------
int BaseDatos::getNumRegistros() const
{
	if (this->mResult != NULL)
		return PQntuples(this->mResult);
	else
		return 0;
}

//------------------------------------------------------------------------------
int BaseDatos::getNumRegistrosAfectados() const
{
	if (this->mResult != NULL)
		return atoi(PQcmdTuples(this->mResult));
	else
		return 0;
}

//------------------------------------------------------------------------------
int BaseDatos::getNumCampos() const
{
	if (this->mResult != NULL)
		return PQnfields(this->mResult);
	else
		return 0;
}

//------------------------------------------------------------------------------
bool BaseDatos::existeCampo(const char* campo) const
{
	if (this->mResult != NULL)
		return (PQfnumber(this->mResult, campo) >= 0);
	else
		return false;
}

//------------------------------------------------------------------------------
const char* BaseDatos::getNombreCampo(int campo) const
{
	if (this->mResult != NULL)
		return PQfname(this->mResult, campo);
	else
		return "";
}

//------------------------------------------------------------------------------
const char* BaseDatos::getTipoCampo(int campo) const
{
	int type;

	type = PQftype(this->mResult, campo);
	switch (type) {
	case 16: //BOOLOID:    /* boolean, 'true'/'false' */
	case 21: //INT2OID:    /* -32 thousand to 32 thousand, 2-byte storage */
	case 23: //INT4OID:    /* -2 billion to 2 billion integer, 4-byte storage */
	case 20: //INT8OID:    /* ~18 digit integer, 8-byte storage */
		return "DB_INT";
		break;

	case 700: //FLOAT4OID:  /* single-precision floating point number, 4-byte storage */
		return "DB_FLOAT";
		break;

	case 701: //FLOAT8OID:  /* double-precision floating point number, 8-byte storage */
		return "DB_DOUBLE";
		break;

	case 1082: //DATEOID
	case 1083: //TIMEOID
	case 1114: //TIMESTAMPOID:   /* date and time */
	case 1184: //TIMESTAMPTZOID: /* date and time with time zone */
		return "DB_DATETIME";
		break;

	case 18: //CHAROID:    /* single character */
	case 25: //TEXTOID:    /* variable-length string, no limit specified */
	case 1042: //BPCHAROID:  /* char(length), blank-padded string, fixed storage length */
	case 1043: //VARCHAROID: /* varchar(length), non-blank-padded string, variable storage length */
		return "DB_STRING";
		break;

	case 17: //BYTEAOID: /* variable-length string, binary values escaped" */
		return "DB_BLOB";
		break;

	case 1560: //BITOID:    /* fixed-length bit string */
	case 1562: //VARBITOID: /* variable-length bit string */
		return "DB_BITMAP";
		break;

	default:
		return ("Unsupported column type with oid"); // %d\n", type);
	}
}

//------------------------------------------------------------------------------
int BaseDatos::getIndiceCampo(const char* campo) const
{
	int indice;

	if (this->mResult != NULL)
	{
		indice = PQfnumber(this->mResult, campo);
		if (indice < 0)
		{
			//STrz::warning(ConstTraza::T_TRAZA_BD, "No existe el campo '%s' en [%s]", campo, this->mSql.isEmpty() ? this->mSql.getCadena() : "query null");
		}
	}
	else
	{
		//STrz::warning(ConstTraza::T_TRAZA_BD, "El resultado de la ultima consulta esta vacio: [%s]", this->mSql.isEmpty() ? this->mSql.getCadena() : "query null");
		indice = -1;
	}

	return indice;
}

//------------------------------------------------------------------------------
bool BaseDatos::esNulo(const Cadena& campo, int registro) const
{
	return esNulo(getIndiceCampo(campo.getCadena()), registro);
}

//------------------------------------------------------------------------------
bool BaseDatos::esNulo(int campo, int registro) const
{
	return (PQgetisnull(this->mResult, registro, campo) != 0);
}

//------------------------------------------------------------------------------
void BaseDatos::mostrarDatos(const char* separador)
{
	PQprintOpt opciones;

	opciones.header = true;
	opciones.align = true;
	opciones.standard = false;
	opciones.html3 = false;
	opciones.expanded = false;
	opciones.pager = false;
	opciones.fieldSep = const_cast<char*> (separador);
	opciones.tableOpt = NULL;
	opciones.caption = NULL;
	opciones.fieldName = NULL;

	fprintf(stdout, "Mostrando %d registros:\n\n", getNumRegistros());
	if (getNumRegistros() > 0)
		PQprint(stdout, this->mResult, &opciones);
	fprintf(stdout, "\n");
}

//------------------------------------------------------------------------------
const char* BaseDatos::getString(const Cadena& campo, int registro) const
{
	return getString(getIndiceCampo(campo.getCadena()), registro);
}

//------------------------------------------------------------------------------
const char* BaseDatos::getString(int campo, int registro) const
{
	if (registro >= PQntuples(this->mResult))
		return NULL;
	else
		return PQgetvalue(this->mResult, registro, campo);
}

//------------------------------------------------------------------------------
char BaseDatos::getChar(const Cadena& campo, int registro) const
{
	return getString(getIndiceCampo(campo.getCadena()), registro)[0];
}

//------------------------------------------------------------------------------
char BaseDatos::getChar(int campo, int registro) const
{
	return getString(campo, registro)[0];
}

//------------------------------------------------------------------------------
int BaseDatos::getInt(const Cadena& campo, int registro) const
{
	try
	{
		return getInt(getIndiceCampo(campo.getCadena()), registro);
	}
	catch (Excepcion& e)
	{
		//STrz::warning(ConstTraza::T_TRAZA_BD, "Error obteniendo el campo '%s' del registro %d", campo.getCadena(), registro);
		throw;
	}
}

//------------------------------------------------------------------------------
int BaseDatos::getInt(int campo, int registro) const
{
	return atol(getValor(campo, registro));
}


//------------------------------------------------------------------------------
int BaseDatos::getLong(const Cadena& campo, int registro) const
{
	try
	{
		return getLong(getIndiceCampo(campo.getCadena()), registro);
	}
	catch (Excepcion& e)
	{
		//STrz::warning(ConstTraza::T_TRAZA_BD, "Error obteniendo el campo '%s' del registro %d", campo.getCadena(), registro);
		throw;
	}
}

//------------------------------------------------------------------------------
int BaseDatos::getLong(int campo, int registro) const
{
	return atoll(getValor(campo, registro));
}


//------------------------------------------------------------------------------
bool BaseDatos::getBool(const Cadena& campo, int registro) const
{
	try
	{
		return getBool(getIndiceCampo(campo.getCadena()), registro);
	}
	catch (Excepcion& e)
	{
		//STrz::warning(ConstTraza::T_TRAZA_BD, "Error obteniendo el campo '%s' del registro %d", campo.getCadena(), registro);
		throw;
	}
}

//------------------------------------------------------------------------------
bool BaseDatos::getBool(int campo, int registro) const
{
	return (getValor(campo, registro)[0] == VALOR_TRUE);
}

//------------------------------------------------------------------------------
double BaseDatos::getDouble(const Cadena& campo, int registro) const
{
	try
	{
		return getDouble(getIndiceCampo(campo.getCadena()), registro);
	}
	catch (Excepcion& e)
	{
		//STrz::warning(ConstTraza::T_TRAZA_BD, "Error obteniendo el campo '%s' del registro %d", campo.getCadena(), registro);
		throw;
	}
}

//------------------------------------------------------------------------------
double BaseDatos::getDouble(int campo, int registro) const
{
	return atof(getValor(campo, registro));
}

//------------------------------------------------------------------------------
Cadena BaseDatos::getUltimoError() const
{
	return mTextoUltimoError;
}

//------------------------------------------------------------------------------
bool BaseDatos::checkResultado(const PGresult* res, const char* cmd)
{
	ExecStatusType status;
	bool           error;

	error = false;
	if (res == NULL || PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		if (res == NULL)
		{
			//STrz::error(ConstTraza::T_TRAZA_BD, "PGresult = NULL [ Error desconocido ]");
			//STrz::error(ConstTraza::T_TRAZA_BD, PQerrorMessage(this->mDB));
			mTextoUltimoError = PQerrorMessage(this->mDB);
			error = true;
		}
		else if (PQstatus(this->mDB) == CONNECTION_BAD)
		{
			//STrz::warning(ConstTraza::T_TRAZA_BD, "Se ha perdido la conexion");
			//STrz::error(ConstTraza::T_TRAZA_BD, PQresultErrorMessage(res));
			//STrz::error(ConstTraza::T_TRAZA_BD, PQerrorMessage(this->mDB));

			PQreset(this->mDB);
			error = true;
		}
		else
		{
			status = PQresultStatus(res);
			if (status == PGRES_BAD_RESPONSE || status == PGRES_NONFATAL_ERROR ||
				status == PGRES_FATAL_ERROR)
			{
				//STrz::error(ConstTraza::T_TRAZA_BD, PQresultErrorMessage(res));
				//STrz::error(ConstTraza::T_TRAZA_BD, PQerrorMessage(this->mDB));
				mTextoUltimoError = PQerrorMessage(this->mDB);
				error = true;
			}
		}
	}

	if (error)
	{
		//STrz::debug(ConstTraza::T_TRAZA_BD, "Comando que genero el error : [");
		for (int i = 0; i < strlen(cmd); i += 100)
		{
			Cadena cad(&cmd[i], 100);
			//STrz::debug(ConstTraza::T_TRAZA_BD, cad);
		}
		//STrz::debug(ConstTraza::T_TRAZA_BD, "]");

		//STrz::debug(ConstTraza::T_TRAZA_BD, "Transaction Status %d", PQtransactionStatus(this->mDB));
		if (PQTRANS_UNKNOWN == PQtransactionStatus(this->mDB)) {
			PQreset(this->mDB);
		}
	} /*
	else {
	   STrz::debug (ConstTraza::T_TRAZA_BD, "Query [");
	   for (int i = 0; i < strlen (cmd); i += 100)
	   {
		  Cadena cad(&cmd[i], 100);
		  STrz::debug (ConstTraza::T_TRAZA_BD, cad);
	   }
	   STrz::debug (ConstTraza::T_TRAZA_BD, "]");
	}*/

	return (!error);
}

//------------------------------------------------------------------------------
bool BaseDatos::comando(const Cadena& sql)
{
	return comando(sql.getCadena());
}

//------------------------------------------------------------------------------
bool BaseDatos::comando(const char* sql)
{
	PGresult* res;
	bool      ok;

	res = PQexec(this->mDB, sql);
	ok = checkResultado(res, sql);
	PQclear(res);

	return ok;
}

//------------------------------------------------------------------------------
bool BaseDatos::consulta(const char* sql)
{
	clearResult();

	this->mSql.setCharArray(sql, (SWordType)strlen(sql));
	this->mResult = PQexec(this->mDB, sql);

	return (checkResultado(this->mResult, sql));
}

//------------------------------------------------------------------------------
void BaseDatos::clearResult()
{
	this->mSql.setEmpty();
	if (this->mResult != NULL)
		PQclear(this->mResult);
	this->mResult = NULL;
}

//------------------------------------------------------------------------------
const char* BaseDatos::getValor(int campo, int registro) const
{
	if (registro >= PQntuples(this->mResult))
	{
		throw RangoExcepcion(__AQUI__, "El numero de registro esta fuera de rango");
	}
	else if (esNulo(campo, registro))
	{
		//STrz::basic(ConstTraza::T_TRAZA_BD, "El campo '%s' del registro %d  es nulo", getNombreCampo(campo), registro);
		//STrz::basic(ConstTraza::T_TRAZA_BD, this->mSql.getCadena());
		throw ParametroExcepcion(__AQUI__, "Valor nulo");
	}
	else
	{
		return PQgetvalue(this->mResult, registro, campo);
	}
}

//------------------------------------------------------------------------------
void BaseDatos::copiar(const BaseDatos& db)
{
	this->mResult = NULL;
	this->mCadenaConexion = db.mCadenaConexion;
	this->mDB = NULL;

	this->mDB = PQconnectdb(this->mCadenaConexion.getCadena());
	if (this->mDB == NULL || PQstatus(this->mDB) != CONNECTION_OK)
		throw ComunicacionExcepcion(__AQUI__, "No se ha podido conectar a la base de datos");

	PQsetNoticeReceiver(this->mDB, noticeReceiver, NULL);
	PQsetNoticeProcessor(this->mDB, noticeProcessor, NULL);
}

//------------------------------------------------------------------------------
void BaseDatos::noticeReceiver(void *arg, const PGresult *res)
{
#ifdef _DEBUG
	noticeProcessor(arg, PQresultErrorMessage(res));
#endif
}

//------------------------------------------------------------------------------
void BaseDatos::noticeProcessor(void *arg, const char *message)
{
#ifdef _DEBUG
	//STrz::debug(ConstTraza::T_TRAZA_BD, message);
#endif
}
