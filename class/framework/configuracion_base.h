#ifndef PROYECTO_SDL2_BASE_CONFIGURACION_H
#define PROYECTO_SDL2_BASE_CONFIGURACION_H

#ifdef WINCOMPIL
/* Localización del parche mingw32... Esto debería estar en otro lado, supongo. */
#include <herramientas/herramientas/herramientas.h>
#endif

/*
* Esta clase lee y escribe en un archivo los datos de configuración de la 
* aplicación. Al leer los datos de configuración los almacena entre sus 
* propiedades. Podemos (y debemos) extenderla para la configuración de cada
* proyecto implementando todos los métodos virtuales puros que tiene.
*/

#include <class/dnot_parser.h>
#include <iostream>

namespace DFramework
{

class Configuracion_base_no_fichero_exception
	:public std::runtime_error
{
	public:
	Configuracion_base_no_fichero_exception(const std::string& ruta)
		:std::runtime_error("No se puede localizar el fichero "+ruta)
	{}
};

class Configuracion_base
{
	////////////////////////////////
	// Interface pública.

	public:

	int acc_modo_pantalla() const 		{return token_por_ruta(obtener_clave_modo_pantalla());}
	int acc_modo_hardware() const 		{return token_por_ruta(obtener_clave_modo_hardware());}
	int acc_pantalla_doble_buffer() const 	{return token_por_ruta(obtener_clave_pantalla_doble_buffer());}
	int acc_pantalla_anyformat() const 	{return token_por_ruta(obtener_clave_pantalla_anyformat());}
	int acc_volumen_audio() const		{return token_por_ruta(obtener_clave_volumen_audio());}
	int acc_volumen_musica() const		{return token_por_ruta(obtener_clave_volumen_musica());}
	int acc_audio_ratio() const		{return token_por_ruta(obtener_clave_audio_ratio());}
	int acc_audio_salidas() const		{return token_por_ruta(obtener_clave_audio_salidas());}
	int acc_audio_buffers() const		{return token_por_ruta(obtener_clave_audio_buffers());}
	int acc_audio_canales() const		{return token_por_ruta(obtener_clave_audio_canales());}
	int acc_version_archivo() const		{return token_por_ruta(obtener_clave_version_archivo());}

	void mut_modo_pantalla(int p_valor) 		{configurar(obtener_clave_modo_pantalla(), p_valor);}
	void mut_modo_hardware(int p_valor)		{configurar(obtener_clave_modo_hardware(), p_valor);}
	void mut_pantalla_doble_buffer(int p_valor)	{configurar(obtener_clave_pantalla_doble_buffer(), p_valor);}
	void mut_pantalla_anyformat(int p_valor)	{configurar(obtener_clave_pantalla_anyformat(), p_valor);}
	void mut_volumen_audio(int p_valor)		{configurar(obtener_clave_volumen_audio(), p_valor);}
	void mut_volumen_musica(int p_valor)		{configurar(obtener_clave_volumen_musica(), p_valor);}
	void mut_audio_ratio(int p_valor)		{configurar(obtener_clave_audio_ratio(), p_valor);}
	void mut_audio_salidas(int p_valor)		{configurar(obtener_clave_audio_salidas(), p_valor);}
	void mut_audio_buffers(int p_valor)		{configurar(obtener_clave_audio_buffers(), p_valor);}
	void mut_audio_canales(int p_valor)		{configurar(obtener_clave_audio_canales(), p_valor);}

	void grabar();

	Configuracion_base(const std::string& ruta);

	protected: 

	//Conversor de lo que sea a string...
	template <typename T>
	void 						configurar(const std::string& clave, T v)
	{
		auto& tok=token_por_ruta(clave);
		tok.asignar(v);
	}

	const Herramientas_proyecto::Dnot_token&	token_por_ruta(const std::string& c) const;
	Herramientas_proyecto::Dnot_token&		token_por_ruta(const std::string& c);

	////////////////////////////////////
	// A implementar por clases que la extiendan...

		//Estos simplemente devuelven claves de configuración.
	virtual std::string obtener_clave_version_archivo() const=0;
	virtual std::string obtener_version_archivo() const=0;
	virtual std::string obtener_clave_modo_pantalla() const=0;
	virtual std::string obtener_clave_modo_hardware() const=0;
	virtual std::string obtener_clave_pantalla_doble_buffer() const=0;
	virtual std::string obtener_clave_pantalla_anyformat() const=0;
	virtual std::string obtener_clave_volumen_audio() const=0;
	virtual std::string obtener_clave_volumen_musica() const=0;
	virtual std::string obtener_clave_audio_ratio() const=0;
	virtual std::string obtener_clave_audio_buffers() const=0;
	virtual std::string obtener_clave_audio_salidas() const=0;
	virtual std::string obtener_clave_audio_canales() const=0;

	///////////////////////////////////
	// Propiedades.

	private:	

	Herramientas_proyecto::Dnot_token		token;
	const std::string				ruta_fichero;
};

}
#endif
