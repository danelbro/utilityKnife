#include "utl_Application.hpp"
#include "utl_StageManager.hpp"
#include "utl_Box.hpp"
#include <cstdint>
#include <string>
#include "utl_SDLInterface.hpp"
#include <SDL3/SDL_main.h>
#include <utility>

class EditorApp : public utl::Application {
public:
	EditorApp() = default;
	EditorApp(utl::AppConfig config);
	~EditorApp();

	void trigger_stage_change(const std::string& new_stage) override final;

	const std::string& title() const override final {
		return m_title;
	}
	virtual const std::string& version() const override final {
		return m_version;
	}
	virtual const std::string& identifier() const override final {
		return m_identifier;
	}
	virtual utl::Box& screen() override final { return m_screen; }
	virtual uint32_t flags() override final {
		return m_flags;
	}
	virtual uint32_t windowFlags() override final {
		return m_windowFlags;
	}
	virtual utl::Window& window() override final { return m_context.window; }
	virtual utl::Renderer& renderer() override final {
		return m_context.renderer;
	}
	virtual utl::StageManager& stageManager() override final {
		return m_stageManager;
	}
private:
	std::string m_title{};
	std::string m_version{};
	std::string m_identifier{};
	utl::Box m_screen{};
	uint32_t m_flags{};
	uint32_t m_windowFlags{};
	utl::WindowWithRenderer m_context{};
	utl::StageManager m_stageManager{ this };
};

EditorApp::EditorApp(utl::AppConfig config)
	: m_title{ config.title }, m_version{ config.version }, m_identifier{ config.identifier },
	m_screen{ config.screenSpace.w, config.screenSpace.h }, m_flags{ config.flags }, m_windowFlags{ config.windowFlags },
	m_context{ utl::create_window_with_renderer(m_title, m_screen.w, m_screen.h, m_flags) },
	m_stageManager{ this }
{
	utl::init_SDL(m_title, m_version, m_identifier, m_flags);
	m_context.renderer.setVSync(1);
	LOG("Set vSync");
	LOG("Created EditorApp");
}

EditorApp::~EditorApp()
{
	utl::quit_sdl();
	LOG("Destroyed EditorApp");
}

void EditorApp::trigger_stage_change([[maybe_unused]] const std::string& new_stage) {}

int main(int, char **)
{
	utl::AppConfig config{ "UtilityKnife Editor", "0.0.1", "com.utilityKnife.editor",
							{1280, 720}, SDL_INIT_VIDEO | SDL_INIT_AUDIO, 0 };
	EditorApp editor{config};
	editor.run();
	return 0;
}