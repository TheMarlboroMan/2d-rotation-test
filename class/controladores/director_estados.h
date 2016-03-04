#ifndef DIRECTOR_ESTADOS
#define DIRECTOR_ESTADOS

#include <memory>

#include <herramientas/log_base/log_base.h>

#include "../framework/director_estados_interface.h"

#include "../app/framework_impl/app_config.h"

#include "estados_controladores.h"
#include "principal.h"


namespace App
{

class Director_estados:
	public DFramework::Director_estados_interface
{
	public:

							Director_estados(DFramework::Kernel& kernel, App::App_config& config, DLibH::Log_base&);

	virtual void					preparar_cambio_estado(int deseado, int actual);

	private:

	void						preparar_video(DFramework::Kernel& kernel);
	void						registrar_controladores();

	App::App_config					config;
	DLibH::Log_base&				log;

	std::unique_ptr<Controlador_principal>		controlador_principal;
};

}
#endif
