#pragma once

#include <cstdint>
#include <string>
#include <utility>

#include <utl_Application.hpp>
#include <utl_StageManager.hpp>
#include <utl_Box.hpp>
#include <utl_SDLInterface.hpp>

class EditorApp : public utl::Application {
public:
	EditorApp() = default;
	EditorApp(utl::AppConfig config);
	~EditorApp();

	void trigger_stage_change(const std::string& new_stage) final;

	const std::string& title() const final {
		return m_title;
	}
	virtual const std::string& version() const final {
		return m_version;
	}
	virtual const std::string& identifier() const final {
		return m_identifier;
	}
	virtual utl::Box& screen() final { return m_screen; }
	virtual uint32_t flags() final {
		return m_flags;
	}
	virtual uint32_t windowFlags() final {
		return m_windowFlags;
	}
	virtual utl::Window& window() final { return m_context.window; }
	virtual utl::Renderer& renderer() final {
		return m_context.renderer;
	}
	virtual utl::StageManager& stageManager() final {
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