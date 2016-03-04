#ifndef CONTROLADOR_PRINCIPAL_H
#define CONTROLADOR_PRINCIPAL_H

#include <vector>

#include <herramientas/poligono_2d/poligono_2d.h>
#include <herramientas/log_base/log_base.h>

#include "../framework/controlador_interface.h"

namespace App
{

class Controlador_principal:
	public DFramework::Controlador_interface
{
	public:

					Controlador_principal(DLibH::Log_base&);

	virtual void 			preloop(DFramework::Input& input, float delta);
	virtual void 			loop(DFramework::Input& input, float delta);
	virtual void 			postloop(DFramework::Input& input, float delta);
	virtual void 			dibujar(DLibV::Pantalla& pantalla);
	virtual void 			despertar();
	virtual void 			dormir();
	virtual bool			es_posible_abandonar_estado() const;

	private:

	void					rotar(float delta, int dir);
	void					mover(float delta, int dir);

	typedef DLibH::Poligono_2d<double>	tpoligono;

	DLibH::Log_base&			log;

	struct 
	{
		tpoligono			poligono;
		double				angulo;
	}jugador;

	std::vector<tpoligono>			poligonos;
};

}

#endif
