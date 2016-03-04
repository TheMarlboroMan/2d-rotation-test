#include <libDan2.h>
#include <defDanSDL.h>
#include "class/framework/kernel.h"
#include "class/app/framework_impl/kernel_config.h"
#include "class/app/framework_impl/app_config.h"
#include "class/controladores/director_estados.h"

//Declaración del log de aplicación en espacio global.
DLibH::Log_base LOG;

int iniciar_app(Herramientas_proyecto::Controlador_argumentos&);

int main(int argc, char ** argv)
{
	//Inicializar control de logs.
	LOG.inicializar("logs/info.log");
	LOG.activar();
	DLibH::Log_motor::arrancar("logs/motor_log.log");

	//Inicializar control de argumentos.
	Herramientas_proyecto::Controlador_argumentos CARG(argc, argv);
	bool res=iniciar_app(CARG);

	DLibH::Log_motor::finalizar();
	return res;
}

int iniciar_app(Herramientas_proyecto::Controlador_argumentos& CARG)
{
	using namespace App;

	DLibH::Log_base log_app("logs/app_log.log");
	log_app<<"Iniciando proceso principal..."<<std::endl;

	//Intentar inicializar sin cargar aún la SDL...
	try
	{
		log_app<<"Inicializando configuración aplicación..."<<std::endl;
		App_config config;

		//Si la inicialización ha tenido éxito podemos arrancar la SDL y el Kernel.
		log_app<<"Inicializando configuración kernel..."<<std::endl;
		Kernel_config kernel_config;

		log_app<<"Inicializando SDL2..."<<std::endl;	
		if(!DLibH::Herramientas_SDL::iniciar_SDL(SDL_INIT_VIDEO | SDL_INIT_TIMER))
		{
			throw std::runtime_error("No ha sido posible inicializar la librería SD2L");
		}
		
		log_app<<"Inicializando kernel..."<<std::endl;
		DFramework::Kernel kernel(CARG, kernel_config, config);
		App::Director_estados APP(kernel, config, log_app);
		APP.iniciar(kernel);

		log_app<<"Finalizando proceso principal."<<std::endl;
	}
	catch(std::exception& e)
	{
		log_app<<"Ha ocurrido un error "<<e.what()<<std::endl;
		std::cout<<"Abandonando aplicación debido a error : "<<e.what()<<std::endl;

		log_app<<"Finalizando SDL2..."<<std::endl;
		DLibH::Herramientas_SDL::apagar_SDL();

		return 1;
	}

	log_app<<"Finalizando SDL2..."<<std::endl;
	DLibH::Herramientas_SDL::apagar_SDL();
	return 0;
}
