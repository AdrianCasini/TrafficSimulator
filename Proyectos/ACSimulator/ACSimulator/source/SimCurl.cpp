#include "SimCurl.h"

//------------------------------------------------------------------------------
SimCurl::SimCurl()
{
	
}

//------------------------------------------------------------------------------
SimCurl::~SimCurl()
{

}

//------------------------------------------------------------------------------
void SimCurl::json_parser_buff(char* ptrc, SJsonHandler& handler)
{
	Reader reader;
	StringStream ss(ptrc);
	reader.Parse(ss, handler);
}

//------------------------------------------------------------------------------
void SimCurl::json_parser(Cadena cadjson, SJsonHandler& handler)
{
	Reader reader;
	StringStream ss(cadjson.getCadena());
	reader.Parse(ss, handler);
}

//------------------------------------------------------------------------------
bool SimCurl::curl_get(Cadena& cadURL, Cadena& cadHeader, SCurl_Buffer& curl_buff)
{
	CURL* curl;
	CURLcode res;
	struct curl_slist* headers = NULL;
	bool result = false;

	curl = curl_easy_init();

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
		curl_easy_setopt(curl, CURLOPT_URL, cadURL.getCadena());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_crl_resp);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curl_buff);
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_response);
		headers = curl_slist_append(headers, cadHeader.getCadena());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		const char* data = "";
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

		res = curl_easy_perform(curl);
		if (res == CURLE_OK) result = true;
		else
		{
			cout << "CURL: FAIL" << endl;
			result = false;
		}

	}

	curl_easy_cleanup(curl);

	return result;

}

//------------------------------------------------------------------------------
size_t SimCurl::write_crl_resp(void* ptr, size_t size, size_t nmemb, void* data)
{
	//cout << "Simulator::write_crl_resp" << endl;
	SByteType buffer[1 + CURL_MAX_WRITE_SIZE];
	SCurl_Buffer* curl_buff  = (SCurl_Buffer*)data;

	if (data != NULL)
	{
		if ((size * nmemb) >= (1 + CURL_MAX_WRITE_SIZE)) { cout << "write_crl_resp|Error: Fuera de Rango" << endl; return 0; }
		memcpy(buffer, ptr, size * nmemb);
		buffer[size * nmemb] = 0;
		//curl_buff->m_lines.formCadena("%s", buffer);
		//cout << curl_buff->m_lines.getCadena() << endl;

		memcpy(curl_buff->m_buff + curl_buff->m_indx, ptr, size * nmemb);
		curl_buff->m_indx = curl_buff->m_indx + size * nmemb;
	}

	return (size * nmemb);
}

//------------------------------------------------------------------------------
size_t SimCurl::write_response(void* ptr, size_t size, size_t nmemb, void* data)
{
	//cout << "Simulator::write_response" << endl;
	SByteType buffer[1 + CURL_MAX_WRITE_SIZE];
	if ((size * nmemb) >= (1 + CURL_MAX_WRITE_SIZE))
	{
		cout << "write_response: Error: Fuera de Rango" << endl;
		return 0;
	}
	memcpy(buffer, ptr, size * nmemb);
	buffer[size * nmemb] = 0;
	return (size * nmemb);
}

//------------------------------------------------------------------------------
size_t SimCurl::write_file(void* ptr, size_t size, size_t nmemb, void* data)
{
	//cout << "Simulator::write_file" << endl;
	FILE* writehere = (FILE*)data;
	return fwrite(ptr, size, nmemb, writehere);
}
