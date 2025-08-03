#include "core/InputMap.h"

InputMap::InputMap()
{
	bindKey(SDLK_ESCAPE, ExitGame);

    bindKey(SDLK_w, MoveForward);
    bindKey(SDLK_s, MoveBackward);
    bindKey(SDLK_a, MoveLeft);
    bindKey(SDLK_d, MoveRight);
    bindKey(SDLK_1, Ability1);
    bindKey(SDLK_2, Ability2);
    bindKey(SDLK_3, Ability3);
    bindKey(SDLK_4, Ability4);
    bindKey(SDLK_5, Ability5);
}

void InputMap::bindKey(SDL_Keycode key, Action action, StateType state)
{
	m_keyBindings[key].push_back({ action, state });
}

void InputMap::processEvent(const SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
		SDL_Keycode key = event.key.keysym.sym;
		m_keyStates[key] = (event.type == SDL_KEYDOWN);

        auto it = m_keyBindings.find(key);
        if (it != m_keyBindings.end()) {
            for (const auto& binding : it->second) {
                if ((event.type == SDL_KEYDOWN && binding.state == Pressed) ||
                    (event.type == SDL_KEYUP && binding.state == Released)) {

                    m_activeEvents.push_back({ binding.action, binding.state });
                }
            }
        }
    }
}

void InputMap::update() {
    // 处理所有活跃事件
    for (const auto& event : m_activeEvents) {
        if (event.state == Pressed) {
            // 调用按下监听器
            for (auto& callback : m_pressedListeners[event.action]) {
                callback();
            }
        }
        else if (event.state == Released) {
            // 调用释放监听器
            for (auto& callback : m_releasedListeners[event.action]) {
                callback();
            }
        }
    }
    m_activeEvents.clear();
}

bool InputMap::isActionHeld(Action action) const
{
    for (const auto& [key, bindings] : m_keyBindings) {
        for (const auto& binding : bindings) {
            if (binding.action == action) {
                auto it = m_keyStates.find(key);
                if (it != m_keyStates.end() && it->second) {
                    return true;
                }
            }
        }
    }
    return false;
}

void InputMap::addActionListener(Action action, std::function<void()> callback, StateType state)
{

    if (state == Pressed) {
        m_pressedListeners[action].push_back(callback);
    }
    else if (state == Released) {
        m_releasedListeners[action].push_back(callback);
    }
}