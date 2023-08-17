#ifndef _SIMCURL_H
#define _SIMCURL_H

#include "Simulator.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace std;
using namespace glm;
using namespace rapidjson;

//------------------------------------------------------------------------------
struct SJsonHandler
{
	Cadena cad;
	Cadena* array_item;
	int count_item;

	bool Null() { cad.formCadena("null");    array_item[count_item] = cad;	count_item++; return true; }
	bool Bool(bool b) { cad.formCadena("%d", b);   array_item[count_item] = cad;	count_item++; return true; }
	bool Int(int i) { cad.formCadena("%d", i);   array_item[count_item] = cad;	count_item++; return true; }
	bool Uint(unsigned u) { cad.formCadena("%d", u);   array_item[count_item] = cad;	count_item++; return true; }
	bool Int64(int64_t i) { cad.formCadena("%llf", i); array_item[count_item] = cad;	count_item++; return true; }
	bool Uint64(uint64_t u) { cad.formCadena("%llf", u); array_item[count_item] = cad;	count_item++; return true; }
	bool Double(double d) { cad.formCadena("%lf", d);  array_item[count_item] = cad;	count_item++; return true; }
	bool RawNumber(const char* str, SizeType length, bool copy) { cad.formCadena("%s", str); array_item[count_item] = cad;	count_item++; return true; }
	bool String(const char* str, SizeType length, bool copy) { cad.formCadena("%s", str); array_item[count_item] = cad;	count_item++; return true; }
	bool StartObject() { cad.formCadena("startobject"); array_item[count_item] = cad;	count_item++; return true; }
	bool Key(const char* str, SizeType length, bool copy) { cad.formCadena("%s", str); array_item[count_item] = cad;	count_item++; return true; }
	bool EndObject(SizeType memberCount) { cad.formCadena("endobject"); array_item[count_item] = cad;	count_item++; return true; }
	bool StartArray() { cad.formCadena("startarray"); array_item[count_item] = cad;	count_item++; return true; }
	bool EndArray(SizeType elementCount) { cad.formCadena("endarray"); array_item[count_item] = cad;	count_item++; return true; }

};

//------------------------------------------------------------------------------
class SimCurl
{
	public:
		
		struct SCurl_Buffer
		{
			int    m_indx;
			Cadena m_lines;
			char*  m_buff;

		};

	

	public:

		SimCurl();
		~SimCurl();

		//-----------Funciones Curl------------------------------------------
		bool   curl_get(Cadena& cadURL, Cadena& cadHeader, SCurl_Buffer& curl_buff);
		static size_t write_response(void* ptr, size_t size, size_t nmemb, void* data);
		static size_t write_file(void* ptr, size_t size, size_t nmemb, void* data);
		static size_t write_crl_resp(void* ptr, size_t size, size_t nmemb, void* data);

		//-----------Funciones Json------------------------------------------
		static void json_parser(Cadena cadjson, SJsonHandler& handler);
		static void json_parser_buff(char* ptrc, SJsonHandler& handler);


};

#endif

