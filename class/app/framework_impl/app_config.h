#ifndef APP_CONFIGURACION_H
#define APP_CONFIGURACION_H

#include "../../framework/configuracion_base.h"

#include <iostream>

namespace App
{

class App_config:
	public DFramework::Configuracion_base
{
	////////////////////////////////
	// Interface pública.

	public:

	int acc_w_logica_pantalla() const {return token_por_ruta(CLAVE_W_LOGICA_PANTALLA);}
	int acc_h_logica_pantalla() const {return token_por_ruta(CLAVE_H_LOGICA_PANTALLA);}
	int acc_w_fisica_pantalla() const {return token_por_ruta(CLAVE_W_FISICA_PANTALLA);}
	int acc_h_fisica_pantalla() const {return token_por_ruta(CLAVE_H_FISICA_PANTALLA);}

	void mut_w_logica_pantalla(int p_valor) {configurar(CLAVE_W_LOGICA_PANTALLA, p_valor);}
	void mut_h_logica_pantalla(int p_valor) {configurar(CLAVE_H_LOGICA_PANTALLA, p_valor);}
	void mut_w_fisica_pantalla(int p_valor) {configurar(CLAVE_W_FISICA_PANTALLA, p_valor);}
	void mut_h_fisica_pantalla(int p_valor) {configurar(CLAVE_H_FISICA_PANTALLA, p_valor);}

	App_config();

	private:
	
	////////////////////////////////////
	// Definiciones...

	static const std::string CLAVE_W_LOGICA_PANTALLA;
	static const std::string CLAVE_H_LOGICA_PANTALLA;
	static const std::string CLAVE_W_FISICA_PANTALLA;
	static const std::string CLAVE_H_FISICA_PANTALLA;

	////////////////////////////////////
	// Implementacion...

	protected: 

	std::string obtener_clave_version_archivo() const {return "config:meta:v";}
	std::string obtener_version_archivo() const {return "1";}
	std::string obtener_clave_modo_pantalla() const {return "config:video:modo_pantalla";}
	std::string obtener_clave_modo_hardware() const {return "config:video:modo_hardware";}
	std::string obtener_clave_pantalla_doble_buffer() const {return "config:video:doble_buffer";}
	std::string obtener_clave_pantalla_anyformat() const {return "config:video:pantalla_anyformat";}
	std::string obtener_clave_volumen_audio() const {return "config:audio:volumen_audio";}
	std::string obtener_clave_volumen_musica() const {return "config:audio:volumen_musica";}
	std::string obtener_clave_audio_ratio() const {return "config:audio:audio_ratio";}
	std::string obtener_clave_audio_buffers() const {return "config:audio:audio_buffers";}
	std::string obtener_clave_audio_salidas() const {return "config:audio:audio_salidas";}
	std::string obtener_clave_audio_canales() const {return "config:audio:audio_canales";}

	///////////////////////////////////
	// Propiedades.

	private:

	std::string obtener_ruta_archivo() const {return "data/config/configuracion.dnot";}
};

}
#endif
