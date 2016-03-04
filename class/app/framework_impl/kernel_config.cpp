#include "kernel_config.h"
#include <SDL2/SDL.h>
#include "input.h"

using namespace App;

std::vector<std::string> Kernel_config::obtener_entradas_audio() const
{
	return obtener_entradas_desde_ruta(std::string("data/recursos/audio.txt"));
}

std::vector<std::string> Kernel_config::obtener_entradas_musica() const
{
	return obtener_entradas_desde_ruta(std::string("data/recursos/musica.txt"));
}

std::vector<std::string> Kernel_config::obtener_entradas_texturas() const
{
	return obtener_entradas_desde_ruta(std::string("data/recursos/texturas.txt"));
}

std::vector<std::string> Kernel_config::obtener_entradas_superficies() const
{
	return obtener_entradas_desde_ruta(std::string("data/recursos/superficies.txt"));
}

DFramework::Info_ventana Kernel_config::obtener_info_ventana() const
{
	return DFramework::Info_ventana{600, 200, 600, 200, "2D TESTING...", true};
}

std::vector<DFramework::Par_input> Kernel_config::obtener_pares_input() const
{
	using namespace DFramework;
	std::vector<Par_input> res{
		Par_input{Par_input::tipos::teclado, Input::escape, SDL_SCANCODE_ESCAPE},
		Par_input{Par_input::tipos::teclado, Input::aceptar, SDL_SCANCODE_SPACE},
		Par_input{Par_input::tipos::teclado, Input::aceptar, SDL_SCANCODE_RETURN},
		Par_input{Par_input::tipos::teclado, Input::izquierda, SDL_SCANCODE_LEFT},
		Par_input{Par_input::tipos::teclado, Input::derecha, SDL_SCANCODE_RIGHT},
		Par_input{Par_input::tipos::teclado, Input::arriba, SDL_SCANCODE_UP},
		Par_input{Par_input::tipos::teclado, Input::abajo, SDL_SCANCODE_DOWN},
		Par_input{Par_input::tipos::teclado, Input::pag_siguiente, SDL_SCANCODE_PAGEDOWN},
		Par_input{Par_input::tipos::teclado, Input::pag_anterior, SDL_SCANCODE_PAGEUP}};

	return res;
}
