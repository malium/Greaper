/***********************************************************************************************************************
 *                                   Copyright 2025 Marcos Sánchez Torrent (@malium)                                   *
 *                                               All Rights Reserved                                                   *
 **********************************************************************************************************************/

#include "../GreaperCore/Public/CorePrerequisites.hpp"
#include <iostream>

static int MainCode(void* hInstance, int argc, char** argv);

#if PLT_WINDOWS
#ifdef _CONSOLE
int main(int argc, char* argv[])
{
	void* hInstance = nullptr;
	hInstance = (void*)GetModuleHandleW(nullptr);
	return MainCode(hInstance, argc, argv);
}
#else /* !_CONSOLE */
INT WINAPI WinMain(HINSTANCE hInstance, UNUSED HINSTANCE hPrevInstance, LPSTR lpCmdLine, UNUSED int nShowCmd)
{
	int argc = 0;

	auto argv = greaper::UPtr<LPSTR>(CommandLineToArgvA(lpCmdLine, &argc), &FreeArgvA);
	if (argv == nullptr)
		return EXIT_FAILURE;

	return MainCode(hInstance, argc, argv.Get());
}
#endif 
#else /* !PLT_WINDOWS */
int main(int argc, char* argv[])
{
	void* hInstance = nullptr;
	return MainCode(hInstance, argc, argv);
}
#endif

ENUMERATION(TESTS, Greaper, Wayland, Math, ComplexType);

constexpr auto TestToRun = TESTS_t::Math;

extern void WaylandTest();
extern void MathTest();
extern void GreaperTest();
extern void ComplexTypeTest();

int MainCode(void* hInstance, int argc, char** argv)
{
	using namespace greaper;

	switch (TestToRun)
	{
	case TESTS_t::Wayland:
		WaylandTest();
		break;
	case TESTS_t::Greaper:
		GreaperTest();
		break;
	case TESTS_t::Math:
		MathTest();
		break;
	case TESTS_t::ComplexType:
		ComplexTypeTest();
		break;
	default:
		std::cout << "No test selected!" << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
