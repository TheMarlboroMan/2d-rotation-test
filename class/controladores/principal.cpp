#include "principal.h"

#include <string>
#include <def_video.h>

#include <class/lector_txt.h>

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
	jugador.angulo=90.0;
	jugador.poligono.insertar_vertice({10.0, 20.0});
	jugador.poligono.insertar_vertice({20.0, 0.0});
	jugador.poligono.insertar_vertice({0.0, 0.0});
	jugador.poligono.cerrar();
	jugador.poligono.mut_centro({10.0, 10.0});

	Herramientas_proyecto::Lector_txt L("data/app/data.dat", '#');
	std::string linea;
	const char separador_pieza='\t';
	const char separador_num=',';

	auto obtener_punto=[separador_num](const std::string& str)
	{
		std::vector<std::string> partes=DLibH::Herramientas::explotar(str, separador_num);
		return DLibH::Punto_2d<double>(std::stod(partes[0]), std::stod(partes[1]));
	};
		
	while(true)
	{
		linea=L.leer_linea();
		if(L.es_eof()) break;
		std::vector<std::string> valores=DLibH::Herramientas::explotar(linea, separador_pieza);

		std::vector<DLibH::Punto_2d<double>> puntos;
		for(size_t i=1; i<valores.size(); ++i) puntos.push_back(obtener_punto(valores[i]));
		poligonos.push_back(tpoligono(puntos, obtener_punto(valores[0])));
	}
}

void  Controlador_principal::preloop(DFramework::Input& input, float delta)
{

}

void  Controlador_principal::loop(DFramework::Input& input, float delta)
{
	if(input.es_senal_salida() || input.es_input_pulsado(Input::escape))
	{
		abandonar_aplicacion();
	}
	else
	{
		//Comprobar colisión de disparos... Si colisiona los liquidamos.
		for(auto& p : poligonos)
		{
			for(auto& d : disparos)
			{
				if(colision_poligono_SAT(p, d.poligono)) 
				{
					d.tiempo=0.0f;
					DLibH::Vector_2d<double> v=vector_unidad_para_angulo_cartesiano(d.angulo);					
					p.desplazar(DLibH::Punto_2d<double>{v.x, v.y});
					break;
				}
			}
		}

		//Movimiento de disparos y colisión...
		auto ini=std::begin(disparos);
		while(ini < std::end(disparos))
		{
			auto &d=*ini;
			mover_disparo(d, delta);
			d.tiempo-=delta;
			if(d.tiempo < 0.0f) ini=disparos.erase(ini);
			else ++ini;
		}

		//Copia del jugador, por si colisiona poder restaurarlo.
		auto copia_p=jugador.poligono;
		auto copia_a=jugador.angulo;

		if(input.es_input_pulsado(Input::izquierda))
		{
			rotar(delta, 1);
		}
		else if(input.es_input_pulsado(Input::derecha))
		{
			rotar(delta, -1);
		}
		if(input.es_input_pulsado(Input::arriba))
		{
			mover(delta, 1);
		}
		else if(input.es_input_pulsado(Input::abajo))
		{
			mover(delta, -1);
		}
		
		if(input.es_input_down(Input::espacio))
		{
			disparar();
		}

		//Colisión del jugador.
		for(const auto& p : poligonos)
		{
			if(colision_poligono_SAT(p, jugador.poligono))
			{
				jugador.angulo=copia_a;
				jugador.poligono=copia_p;
				break;
			}
		}


	}
}

void  Controlador_principal::postloop(DFramework::Input& input, float delta)
{

}

void  Controlador_principal::dibujar(DLibV::Pantalla& pantalla)
{
	pantalla.limpiar(0, 0, 0, 255);

	auto transformar=[](const DLibH::Punto_2d<double>& pt, int nx, int ny)
	{
		double x=pt.x + nx;
		double y=-pt.y + ny;

		return DLibH::Punto_2d<double>(x, y);
	};

	auto dibujar_poligono=[transformar](const tpoligono& poligono, DLibV::Pantalla& pantalla, int ex, int ey)
	{
		int iteracion=0;
		DLibH::Punto_2d<double> p1, p2;

		for(const auto& v : poligono.acc_vertices())
		{
			switch(iteracion)
			{
				case 0: p1=v; break;
				case 1: p2=v; break;
				default: p1=p2;	p2=v; break;
			}

			if(iteracion)
			{
				auto pt1=transformar(p1, ex, ey);
				auto pt2=transformar(p2, ex, ey);

				DLibV::Representacion_primitiva_linea l(pt1.x, pt1.y, pt2.x, pt2.y, 0, 255, 0);
				l.volcar(pantalla);
			}

			++iteracion;
		}

		//El último...
		p1=p2;
		p2=poligono.vertice(0);

		auto pt1=transformar(p1, ex, ey);
		auto pt2=transformar(p2, ex, ey);
		DLibV::Representacion_primitiva_linea l(pt1.x, pt1.y, pt2.x, pt2.y, 0, 255, 0);
		l.volcar(pantalla);

		//El centro.
		auto ptc=transformar(poligono.acc_centro(), ex, ey);
		DLibV::Representacion_primitiva_puntos centro(ptc.x, ptc.y, 255, 0, 0);
		centro.volcar(pantalla);
	};

	dibujar_poligono(jugador.poligono, pantalla, 0, 400);

	for(const auto& p : poligonos) dibujar_poligono(p, pantalla, 0, 400);
	for(const auto& d : disparos) dibujar_poligono(d.poligono, pantalla, 0, 400);
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

void Controlador_principal::rotar(float delta, int dir)
{
	const double factor_rotacion=90.0;

	double factor=((double)delta * factor_rotacion) * (double)dir;
	jugador.angulo+=factor;
	jugador.poligono.rotar(factor);
}

void Controlador_principal::mover(float delta, int dir)
{
	double angulo=dir > 0 ? jugador.angulo : jugador.angulo+180.0;
	const double factor_movimiento=100.0 * delta;

	DLibH::Vector_2d<double> v=vector_unidad_para_angulo_cartesiano(angulo);
	DLibH::Punto_2d<double> pd{v.x * factor_movimiento, v.y * factor_movimiento};
	jugador.poligono.desplazar(pd);
}

void Controlador_principal::mover_disparo(disparo& disparo, float delta)
{
	const double factor_movimiento=200.0 * delta;
	DLibH::Vector_2d<double> v=vector_unidad_para_angulo_cartesiano(disparo.angulo);
	DLibH::Punto_2d<double> pd{v.x * factor_movimiento, v.y * factor_movimiento};
	disparo.poligono.desplazar(pd);
	disparo.poligono.rotar(90.0 * delta);
}

void Controlador_principal::disparar()
{
	//Crear...
	disparo disp;
	disp.angulo=jugador.angulo;
	disp.tiempo=1.0f;

	//Calcular poligono...
	double distancia=20.0;
	DLibH::Vector_2d<double> v=vector_unidad_para_angulo_cartesiano(jugador.angulo);
	DLibH::Punto_2d<double> centro{v.x * distancia, v.y * distancia};
	DLibH::Punto_2d<double> centro_j=jugador.poligono.acc_centro();

	DLibH::Punto_2d<double> pt1{centro_j.x+centro.x, centro_j.y+centro.y+10.0};
	DLibH::Punto_2d<double> pt2{centro_j.x+centro.x-5.0, centro_j.y+centro.y};
	DLibH::Punto_2d<double> pt3{centro_j.x+centro.x+5.0, centro_j.y+centro.y};

	disp.poligono.mut_centro(centro);
	disp.poligono.insertar_vertice(pt1);
	disp.poligono.insertar_vertice(pt2);
	disp.poligono.insertar_vertice(pt3);
	disp.poligono.cerrar();
	disp.poligono.rotar(disp.angulo);

	disparos.push_back(disp);	
}

