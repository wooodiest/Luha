#include "Luha.h"
#include "Core/EntryPoint.h"
#include "implot.h"
#include "ExampleLayer.h"

Luha::Application* Luha::CreateApplication(int argc, char** argv)
{
	Luha::ApplicationSpecification spec;
	spec.Name              = "Luha Application";
	spec.Window_Width      = 1600;
	spec.Window_Height     = 900;
	spec.Window_Min_Width  = 1280;
	spec.Window_Min_Height = 720;
	spec.Window_Max_Width  = 0;
	spec.Window_Max_Height = 0;
	spec.VSync             = true;
	spec.Window_Resizeable = true;
	spec.MenuBar           = true;
	spec.ColorThema        = Luha::AppColorTheme::Dark;
	spec.PlotThema         = Luha::PlotColorTheme::Auto;
	spec.PlotColor_Map     = Luha::PlotColorMap::Dark;
	spec.Font              = Luha::AppFont::Roboto;
	spec.FontSize          = 20.0f;

	Luha::Application* app = new Luha::Application(spec);
	app->PushLayer(new ExampleApp::ExampleLayer());

	return app;
}