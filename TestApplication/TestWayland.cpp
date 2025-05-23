/***********************************************************************************************************************
 *                                   Copyright 2025 Marcos Sánchez Torrent (@malium)                                   *
 *                                               All Rights Reserved                                                   *
 **********************************************************************************************************************/

#include "../GreaperCore/Public/CorePrerequisites.hpp"

#if PLT_LINUX
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>
#include <wayland-client.h>
#include "../External/wayland-protocols/xdg-shell-protocol.h"
#include "../External/wayland-protocols/xdg-decoration-unstable-v1-protocol.h"
#include <map>
#include <string>

static wl_display* gwl_display = nullptr;
static wl_registry* gwl_registry = nullptr;
static wl_shm* gwl_shm = nullptr;
static wl_compositor* gwl_compositor = nullptr;

static xdg_wm_base* gxdg_wm_base = nullptr;
static wl_surface* gwl_surface = nullptr;
static xdg_surface* gxdg_surface = nullptr;
static xdg_toplevel* gxdg_toplevel = nullptr;

struct zxdg_decoration_manager_v1;
static zxdg_decoration_manager_v1* gzxdg_decoration_manager_v1 = nullptr;
struct zxdg_toplevel_decoration_v1;
static zxdg_toplevel_decoration_v1* gzxdg_toplevel_decoration_v1 = nullptr;

static void wl_buffer_release(void *data, wl_buffer *wl_buffer)
{
	printf("DestroyBuffer\n");
	/* Sent by the compositor when it's no longer using this buffer */
	wl_buffer_destroy(wl_buffer);
}

static wl_buffer_listener wlbuffer_listener { &wl_buffer_release };

static void randname(char *buf)
{
	timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	long r = ts.tv_nsec;
	for (int i = 0; i < 6; ++i) {
		buf[i] = 'A'+(r&15)+(r&16)*2;
		r >>= 5;
	}
}

static int create_shm_file(void)
{
	int retries = 100;
	do {
		char name[] = "/wl_shm-XXXXXX";
		randname(name + sizeof(name) - 7);
		--retries;
		int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
		if (fd >= 0) {
			shm_unlink(name);
			return fd;
		}
	} while (retries > 0 && errno == EEXIST);
	return -1;
}

static int allocate_shm_file(size_t size)
{
	int fd = create_shm_file();
	if (fd < 0)
		return -1;
	int ret;
	do {
		ret = ftruncate(fd, size);
	} while (ret < 0 && errno == EINTR);
	if (ret < 0) {
		close(fd);
		return -1;
	}
	return fd;
}

static wl_buffer* draw_frame()
{
	const int width = 640, height = 480;
	int stride = width * 4;
	int size = stride * height;

	int fd = allocate_shm_file(size);
	if (fd == -1) {
		return nullptr;
	}

	uint32_t *data = static_cast<uint32_t*>(mmap(nullptr, size,
			PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
	if (data == MAP_FAILED) {
		close(fd);
		return nullptr;
	}

	auto* wlpool = wl_shm_create_pool(gwl_shm, fd, size);
	auto* wlbuffer = wl_shm_pool_create_buffer(wlpool, 0,
			width, height, stride, WL_SHM_FORMAT_XRGB8888);
	wl_shm_pool_destroy(wlpool);
	close(fd);

	/* Draw checkerboxed background */
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			if ((x + y / 8 * 8) % 16 < 8)
				data[y * width + x] = 0xFF666666;
			else
				data[y * width + x] = 0xFFEEEEEE;
		}
	}

	munmap(data, size);
	wl_buffer_add_listener(wlbuffer, &wlbuffer_listener, nullptr);
	return wlbuffer;
}

static void xdg_surface_configure(void *data, xdg_surface *xdg_surface, uint32_t serial)
{
	printf("Configure\n");
	xdg_surface_ack_configure(xdg_surface, serial);

	auto* buffer = draw_frame();
	wl_surface_attach(gwl_surface, buffer, 0, 0);
	wl_surface_commit(gwl_surface);
}

static xdg_surface_listener xdgsurface_listener { &xdg_surface_configure };

static void xdg_wm_base_ping(void *data, xdg_wm_base *xdg_wm_base, uint32_t serial)
{
	printf("Ping-Pong\n");
	xdg_wm_base_pong(xdg_wm_base, serial);
}

static xdg_wm_base_listener wm_listener { &xdg_wm_base_ping };

struct wlinterface
{
	std::string name;
	uint32_t version;
};

static std::map<uint32_t, wlinterface> gwlinterfaces{};

static void registry_global(void *data, wl_registry *wl_registry,
		uint32_t name, const char *interface, uint32_t version)
{
	gwlinterfaces.insert_or_assign(name, wlinterface{std::string{interface}, version});
	printf("Registry %d: %s v%d\n", name, interface, version);
	if (strcmp(interface, wl_shm_interface.name) == 0) {
		gwl_shm = static_cast<wl_shm*>(wl_registry_bind(
				wl_registry, name, &wl_shm_interface, 1));
	}
	else if (strcmp(interface, wl_compositor_interface.name) == 0) {
		gwl_compositor = static_cast<wl_compositor*>(wl_registry_bind(
				wl_registry, name, &wl_compositor_interface, 4));
	}
	else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
		gxdg_wm_base = static_cast<xdg_wm_base*>(wl_registry_bind(
				wl_registry, name, &xdg_wm_base_interface, 1));
		xdg_wm_base_add_listener(gxdg_wm_base,
				&wm_listener, nullptr);
	}
	else if (strcmp(interface, zxdg_decoration_manager_v1_interface.name) == 0){
		gzxdg_decoration_manager_v1 = static_cast<zxdg_decoration_manager_v1*>(
			wl_registry_bind(
				wl_registry, name, &zxdg_decoration_manager_v1_interface, 1
			));
	}
}

static void registry_global_remove(void *data, wl_registry *wl_registry, uint32_t name)
{
	printf("RegistryRemove\n");
}

static wl_registry_listener wl_reg_listener { &registry_global, &registry_global_remove };

void WaylandTest()
{
	gwl_display = wl_display_connect(nullptr);

	gwl_registry = wl_display_get_registry(gwl_display);

	wl_registry_add_listener(gwl_registry, &wl_reg_listener, nullptr);

	wl_display_roundtrip(gwl_display);

	gwl_surface = wl_compositor_create_surface(gwl_compositor);

	gxdg_surface = xdg_wm_base_get_xdg_surface(gxdg_wm_base, gwl_surface);

	xdg_surface_add_listener(gxdg_surface, &xdgsurface_listener, nullptr);

	gxdg_toplevel = xdg_surface_get_toplevel(gxdg_surface);

	gzxdg_toplevel_decoration_v1 = zxdg_decoration_manager_v1_get_toplevel_decoration(gzxdg_decoration_manager_v1, gxdg_toplevel);

	zxdg_toplevel_decoration_v1_set_mode(gzxdg_toplevel_decoration_v1, ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE);

	xdg_toplevel_set_title(gxdg_toplevel, "Greaper Wayland Test");
	
	wl_surface_commit(gwl_surface);

	while(wl_display_dispatch(gwl_display) != -1)
	{

	}

	wl_display_disconnect(gwl_display);
}

#else

void WaylandTest()
{
	printf("Test only available on Linux platforms running Wayland.");
}

#endif