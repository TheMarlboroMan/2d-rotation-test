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

	typedef DLibH::Poligono_2d<double>	tpoligono;

	struct 
	{
		tpoligono			poligono;
		double				angulo;
		float				vel;
	}jugador;

	struct disparo
	{
		tpoligono			poligono;
		double				angulo;
		float				tiempo;
	};

	void					rotar(float delta, int dir);
	void					mover(float delta);
	void					mover_disparo(disparo&, float);
	void					disparar();

	DLibH::Log_base&			log;

	std::vector<tpoligono>			poligonos;
	std::vector<disparo>			disparos;
	double  				zoom;
};

}

#endif
