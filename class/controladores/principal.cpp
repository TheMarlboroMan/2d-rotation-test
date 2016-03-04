#include "principal.h"

#include <string>

#include "../app/framework_impl/input.h"
#include "estados_controladores.h"

#ifdef WINCOMPIL
/* Localización del parche mingw32... Esto debería estar en otro lado, supongo. */
#include <herramientas/herramientas/herramientas.h>
#endif

using namespace App;

Controlador_principal::Controlador_principal(DLibH::Log_base& log)
	:log(log)
{

	poligonos.push_back(tpoligono{ {{1.0, 2.0}, {2.0, 0.0}, {0.0, 0.0}}, {1.0, 1.0} });

}

void  Controlador_principal::preloop(DFramework::Input& input, float delta)
{

}

void  Controlador_principal::loop(DFramework::Input& input, float delta)
{
	if(input.es_senal_salida())
	{
		abandonar_aplicacion();
	}
	else
	{
		
	}
}

void  Controlador_principal::postloop(DFramework::Input& input, float delta)
{

}

void  Controlador_principal::dibujar(DLibV::Pantalla& pantalla)
{
	pantalla.limpiar(0, 0, 0, 255);
	//TODO: Situar eje...

	//TODO: Dibujar polígonos cojoneros...
}

void  Controlador_principal::despertar()
{
	log<<"Despertando controlador principal"<<std::endl;
}

void  Controlador_principal::dormir()
{
	log<<"Durmiendo controlador principal"<<std::endl;
}

bool Controlador_principal::es_posible_abandonar_estado() const
{
	return true;
}
