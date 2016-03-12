#include "principal.h"

#include <string>

#include <class/lector_txt.h>

#include "../app/framework_impl/input.h"
#include "estados_controladores.h"

#ifdef WINCOMPIL
/* Localización del parche mingw32... Esto debería estar en otro lado, supongo. */
#include <herramientas/herramientas/herramientas.h>
#endif

using namespace App;

Controlador_principal::Controlador_principal(DLibH::Log_base& log)
	:log(log), 
		fuente_akashi("data/fuentes/Akashi.ttf", 16), 
		zoom(0.5), 
		xcam(0), 
		ycam(0)
{
	jugador.angulo=90.0;
	jugador.poligono.insertar_vertice({10.0, 20.0});
	jugador.poligono.insertar_vertice({20.0, 0.0});
	jugador.poligono.insertar_vertice({0.0, 0.0});
	jugador.poligono.cerrar();
	jugador.poligono.mut_centro({10.0, 10.0});
	jugador.vel=0.0f;

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

		if(input.es_input_down(Input::click_i))
		{
			auto pos_raton=input.acc_posicion_raton();
			nuevo_punto(punto_desde_pos_pantalla(pos_raton.x, pos_raton.y));
		}
		else if(input.es_input_down(Input::click_d))
		{
			cerrar_poligono();
		}

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

		if(input.es_input_pulsado(Input::zoom_mas)) 
		{
			zoom+=(double)delta;
		}
		else if(input.es_input_pulsado(Input::zoom_menos)) 
		{
			zoom-=(double)delta;
			if(zoom < 0.10) zoom=0.10;
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
			jugador.vel+=delta;
			if(jugador.vel > 3.0f) jugador.vel=3.0f;
		}
		else if(input.es_input_pulsado(Input::abajo))
		{
			jugador.vel-=delta * 5.0f;
			if(jugador.vel < -1.0f) jugador.vel=-1.0f;
		}
		else if(jugador.vel)
		{
			float vel=fabs(jugador.vel)-delta;
			if(vel < 0.0f) vel=0.0f;
			jugador.vel=vel* (jugador.vel > 0.f ? 1.0 : -1.0);
		}

		if(jugador.vel)
		{
			mover(delta);
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

	auto transformar=[](const DLibH::Punto_2d<double>& pt, int nx, int ny, double zoom)
	{
		double x=(pt.x - nx) * zoom;
		double y=(-pt.y + ny) * zoom;

		return DLibH::Punto_2d<double>(x, y);
	};

	//TODO: Quizás extender la librería LibDanSDL2 con SDL2_gfx????

	auto dibujar_poligono=[transformar](const tpoligono& poligono, DLibV::Pantalla& pantalla, int ex, int ey, double zoom)
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
				auto pt1=transformar(p1, ex, ey, zoom);
				auto pt2=transformar(p2, ex, ey, zoom);

				DLibV::Representacion_primitiva_linea l(pt1.x, pt1.y, pt2.x, pt2.y, 0, 255, 0);
				l.volcar(pantalla);
			}

			++iteracion;
		}

		//El último...
		p1=p2;
		p2=poligono.vertice(0);

		auto pt1=transformar(p1, ex, ey, zoom);
		auto pt2=transformar(p2, ex, ey, zoom);
		DLibV::Representacion_primitiva_linea l(pt1.x, pt1.y, pt2.x, pt2.y, 0, 255, 0);
		l.volcar(pantalla);

		//El centro.
		auto ptc=transformar(poligono.acc_centro(), ex, ey, zoom);
		DLibV::Representacion_primitiva_puntos centro(ptc.x, ptc.y, 255, 0, 0);
		centro.volcar(pantalla);
	};

	xcam=(jugador.poligono.acc_centro().x) - (320 / zoom), 
	ycam=(jugador.poligono.acc_centro().y) + (200 / zoom);

	dibujar_poligono(jugador.poligono, pantalla, xcam, ycam, zoom);

	for(const auto& p : poligonos) dibujar_poligono(p, pantalla, xcam, ycam, zoom);
	for(const auto& d : disparos) dibujar_poligono(d.poligono, pantalla, xcam, ycam, zoom);

	//TODO: Mejor usar otra función.
	if(poligono_construccion.size() > 2) 
	{
		dibujar_poligono(poligono_construccion, pantalla, xcam, ycam, zoom);
	}

	std::string texto=std::to_string(jugador.poligono.acc_centro().x)+","+std::to_string(jugador.poligono.acc_centro().y);
	DLibV::Representacion_TTF txt(fuente_akashi, {255, 255, 255, 255}, texto);
	txt.ir_a(16, 16);
	txt.volcar(pantalla);

	std::vector<DLibV::Representacion_primitiva_poligono::punto> puntos{
		{10,10}, {40, 20}, {50, 30}, {30, 40}};

	DLibV::Representacion_primitiva_poligono poli(puntos, 255, 255, 128);
	poli.volcar(pantalla);
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

void Controlador_principal::mover(float delta)
{
	const double factor_movimiento=jugador.vel;
	DLibH::Vector_2d<double> v=vector_unidad_para_angulo_cartesiano(jugador.angulo);
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

DLibH::Punto_2d<double>	Controlador_principal::punto_desde_pos_pantalla(int x, int y)
{
	double px=xcam+(x/zoom);
	double py=ycam-(y/zoom);
	return DLibH::Punto_2d<double>{px, py};
}

void Controlador_principal::nuevo_punto(DLibH::Punto_2d<double> p)
{
	poligono_construccion.insertar_vertice(p);
}

void Controlador_principal::cerrar_poligono()
{
	if(poligono_construccion.size() == 3)
	{
		poligono_construccion.cerrar();
		poligonos.push_back(poligono_construccion);
	}
	else if(poligono_construccion.size() > 3 && !poligono_construccion.es_concavo())
	{
		poligono_construccion.cerrar();
		poligonos.push_back(poligono_construccion);
	}

	poligono_construccion=Poligono_2d<double>{};
}
