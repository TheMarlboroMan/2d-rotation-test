#include "director_estados.h"

#include <algorithm>

using namespace App;

extern DLibH::Log_base LOG;

Director_estados::Director_estados(DFramework::Kernel& kernel, App::App_config& c, DLibH::Log_base& log)
	:Director_estados_interface(t_estados::principal, std::function<bool(int)>([](int v){return v >= principal && v < principal;})),
	config(c), log(log)
{
	preparar_video(kernel);
	registrar_controladores();
}

void Director_estados::preparar_video(DFramework::Kernel& kernel)
{
	auto& pantalla=kernel.acc_pantalla();

	int wf=config.acc_w_fisica_pantalla(), 
		hf=config.acc_h_fisica_pantalla(),
		wl=config.acc_w_logica_pantalla(),
		hl=config.acc_h_logica_pantalla();

	pantalla.inicializar(wf, hf);
	pantalla.establecer_medidas_logicas(wl, hl);
	pantalla.establecer_modo_ventana(config.acc_modo_pantalla());
}

void Director_estados::registrar_controladores()
{
	controlador_principal.reset(new Controlador_principal(log));
	registrar_controlador(t_estados::principal, *controlador_principal);
}

void Director_estados::preparar_cambio_estado(int deseado, int actual)
{
	switch(deseado)
	{
		case t_estados::principal: 
		break;
	}
}
