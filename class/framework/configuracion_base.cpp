#include "configuracion_base.h"
#include <map>
#include <source/string_utilidades.h>

/*
Al crear el objeto aún no podemos cargar la información de los valores por
defecto (métodos virtuales) así que no podremos intentar acceder a ningún
valor hasta haber cargado la configuración.
*/

using namespace DFramework;

Configuracion_base::Configuracion_base(const std::string& ruta)
try
	:token(Herramientas_proyecto::parsear_dnot(ruta)),
	ruta_fichero(ruta)
{

}

catch(std::runtime_error& e)
{
	throw Configuracion_base_no_fichero_exception(e.what());
}

void Configuracion_base::grabar()
{
	using namespace Herramientas_proyecto;
	Dnot_token_opciones_serializador os;
	os.tabular_profundidad=true;
	std::ofstream f(ruta_fichero);
	f<<token.serializar(os);
}

/**
* Localiza un token nombrado siguiendo una cadena sencilla: config:video:tam_pantalla
* localizaría el token en el objeto "config" de la raiz, seguido del objeto 
* "video" contenido en "config" y "tam_pantalla" contenido en "video".
*/

const Herramientas_proyecto::Dnot_token& Configuracion_base::token_por_ruta(const std::string& c) const
{
	using namespace Herramientas_proyecto;
	const Dnot_token * p=&token;
	auto v=explotar(c, ':');
	for(const auto& clave : v) 
	{
		try
		{
			p=&p->acc_tokens().at(clave);
		}
		catch(std::exception& e)
		{
			throw std::runtime_error("Imposible localizar clave "+clave+" en ruta "+c);
		}
	}
	return *p;
}

Herramientas_proyecto::Dnot_token& Configuracion_base::token_por_ruta(const std::string& c)
{
	using namespace Herramientas_proyecto;
	Dnot_token * p=&token;
	auto v=explotar(c, ':');
	for(const auto& clave : v) p=&p->acc_tokens()[clave];
	return *p;
}
