#ifndef _BaseDatos_H
#define _BaseDatos_H

#include "lang/Cadena.h"
#include <libpq-fe.h>
#include <map>
#include <limits.h>
//#pragma comment(lib, "libpq.lib")


 /**
 \warning Ninguna de las funciones de esta clase es reentrante

 Codigo de ejemplo:
 \code
 try
 {
	BaseDatos db("localhost", 5432, "prueba", "usuario", "1234");

 # Sin preparar la consulta
	db.begin ();
	db.ejecutarConsulta ("DELETE FROM test");
	for (i = 1; i <= N_REGISTROS; i++)
	{
	   sql.formCadena ("INSERT INTO test VALUES (%d, 'STR%09d', %0.4f, %s)",
		  i, rand (), (float)rand (), rand () & 0x1 ? "TRUE" : "FALSE");
	   db.ejecutarConsulta (sql);
	}
	db.commit ();

 # Preparando la consulta
	db.prepararConsulta ("prep",
	   "INSERT INTO test VALUES ($1, $2, $3, $4)");
	db.begin ();
	db.ejecutarConsulta ("DELETE FROM test");
	for (i = 1; i <= N_REGISTROS; i++)
	{
	   params[0].formCadena ("%d", i);
	   params[1].formCadena ("'STR%09d'", rand ());
	   params[2].formCadena ("%0.4f", (float)rand ());
	   params[3] = (rand () & 0x1 ? "TRUE" : "FALSE");
	   db.ejecutarConsultaPreparada ("prep", 4, params);
	}
	db.commit ();

 # Consultar datos
	db.ejecutarConsulta ("SELECT * FROM test");
	for (i = 0; i < db.getNumRegistros (); i++)
	{
	   printf (" %02d: %d,\t%s,\t%0.2f,\t%c\n", i,
		  db.getInt ("c_int32", i),
		  db.getString ("c_str", i),
		  db.getDouble ("c_float", i),
		  db.getBool ("c_bool", i) ? 'S' : 'N');
	}
 }
 catch (Excepcion& e)
 {
	e.ImprimirStack ();
 }
 \endcode
 */
class BaseDatos
{
public:




	static const int VALOR_ENTERO_NULO = INT_MAX;
	static const char VALOR_CARACTER_NULO = SCHAR_MAX;


	BaseDatos(bool autoConectar = false, int timeout = 5);


	BaseDatos(const Cadena& host, int puerto, const Cadena& nombre, const Cadena& usuario, const Cadena& contrasenya, int timeout = 5);

	BaseDatos(const BaseDatos& db);

	~BaseDatos();

	void operator = (const BaseDatos& db);

	bool conectar(int timeout = 5);

	bool conectar(const Cadena& host, int puerto, const Cadena& nombre,	const Cadena& usuario, const Cadena& contrasenya, int timeout = 5);


	void desconectar();


	void begin();

	bool commit();

	void rollback();

	bool hayTransaccion() const;


	void actualizarEstadisticas(const Cadena& tabla = "");


	 Cadena sanearDato(const Cadena& dato);


	//static Cadena fechaHora(const FechaHora& fecha, bool soloFecha = false);


	 Cadena booleano(bool valor);

	 Cadena entero(int valor);
	 Cadena enteroONulo(int valor, int valorNulo = VALOR_ENTERO_NULO);
	 Cadena enteroOString(int valor, Cadena string, int valorNulo = VALOR_ENTERO_NULO);


	 Cadena caracter(char valor);
	 Cadena caracterONulo(char valor, char valorNulo = VALOR_CARACTER_NULO);
	 Cadena caracterOString(char valor, Cadena string, char valorNulo = VALOR_CARACTER_NULO);


	 Cadena cadenaONulo(Cadena valor);
	 Cadena cadena(Cadena valor);



	//int esperaNotificacion(const Cadena& notificacion, int timeout);


	bool prepararConsulta(const Cadena& nombre, const Cadena& sql);

	bool ejecutarConsultaPreparada( Cadena& nombre, int nParams,  Cadena params[]);


	bool ejecutarConsulta( char* formato, ...);
	bool ejecutarConsulta(Cadena& sql);

	int getNumRegistros() const;

	int getNumRegistrosAfectados() const;

	int getNumCampos() const;

	const char* getTipoCampo(int campo) const;

	bool existeCampo(const char* campo) const;

	const char* getNombreCampo(int campo) const;

	int getIndiceCampo(const char* campo) const;

	bool esNulo(const Cadena& campo, int registro) const;

	bool esNulo(int campo, int registro) const;

	void mostrarDatos(const char* separador = "\t");

	const char* getString(const Cadena& campo, int registro) const;

	const char* getString(int campo, int registro) const;

	char getChar(const Cadena& campo, int registro) const;

	char getChar(int campo, int registro) const;

	int getInt(const Cadena& campo, int registro) const;

	int getInt(int campo, int registro) const;


	int getLong(const Cadena& campo, int registro) const;

	int getLong(int campo, int registro) const;





	bool getBool(const Cadena& campo, int registro) const;

	bool getBool(int campo, int registro) const;

	double getDouble(const Cadena& campo, int registro) const;

	double getDouble(int campo, int registro) const;



	Cadena getUltimoError() const;

private:

	 const int FORMATO_TEXTO = 0;
	 const int FORMATO_BINARIO = 1;

	 const char VALOR_TRUE = 't';
	 const char VALOR_FALSE = 'f';

	//Datos de conexion
	 static const char* HOST;
	 static const int   PUERTO;
	 static const char* NOMBRE_BD;
	 static const char* USUARIO;
	 static const char* CONTRASENYA;

	bool checkResultado(const PGresult* res, const char* cmd);
	bool comando(const char* sql);
	bool comando(const Cadena& sql);

	bool consulta(const char* sql);
	void clearResult();
	const char* getValor(int campo, int registro) const;
	void copiar(const BaseDatos& db);

	static void noticeReceiver(void *arg, const PGresult *res);
    static void noticeProcessor(void *arg, const char *message);

	PGconn*    mDB;
	PGresult*  mResult;
	Cadena     mSql;
	Cadena     mCadenaConexion;
	Cadena     mTextoUltimoError;
	int        mNumBegins;
	bool       mStatusTransaccion;
};

#endif
#pragma once
