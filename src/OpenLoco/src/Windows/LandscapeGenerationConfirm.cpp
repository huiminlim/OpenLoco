#include "Drawing/SoftwareDrawingEngine.h"
#include "Graphics/Colour.h"
#include "Graphics/Gfx.h"
#include "Input.h"
#include "Localisation/StringIds.h"
#include "Objects/InterfaceSkinObject.h"
#include "Objects/ObjectManager.h"
#include "Scenario.h"
#include "Ui/WindowManager.h"
#include "Widget.h"
#include <OpenLoco/Interop/Interop.hpp>

using namespace OpenLoco::Interop;

namespace OpenLoco::Ui::Windows::LandscapeGenerationConfirm
{
    static constexpr Ui::Size kWindowSize = { 280, 92 };

    enum widx
    {
        panel = 0,
        caption = 1,
        close_button = 2,
        button_ok = 3,
        button_cancel = 4,
    };

    static Widget widgets[] = {
        makeWidget({ 0, 0 }, { 280, 92 }, WidgetType::panel, WindowColour::primary),
        makeWidget({ 1, 1 }, { 278, 13 }, WidgetType::caption_22, WindowColour::primary),
        makeWidget({ 267, 2 }, { 11, 11 }, WidgetType::button, WindowColour::primary, StringIds::close_window_cross, StringIds::tooltip_close_window),
        makeWidget({ 20, 77 }, { 100, 12 }, WidgetType::button, WindowColour::primary, StringIds::label_ok),
        makeWidget({ 160, 77 }, { 100, 12 }, WidgetType::button, WindowColour::primary, StringIds::label_button_cancel),
        widgetEnd()
    };

    static WindowEventList events;

    // 0x004C18A5
    static void draw(Window& window, Gfx::RenderTarget* rt)
    {
        auto& drawingCtx = Gfx::getDrawingEngine().getDrawingContext();

        window.draw(rt);

        static loco_global<StringId, 0x0112C826> _commonFormatArgs;
        StringId prompt = window.var_846 == 0 ? StringIds::prompt_confirm_generate_landscape : StringIds::prompt_confirm_random_landscape;
        *_commonFormatArgs = prompt;

        auto origin = Ui::Point(window.x + (window.width / 2), window.y + 41);
        drawingCtx.drawStringCentredWrapped(*rt, origin, window.width, Colour::black, StringIds::wcolour2_stringid, (const char*)&*_commonFormatArgs);
    }

    // 0x004C18E4
    static void onMouseUp(Window& window, WidgetIndex_t widgetIndex)
    {
        switch (widgetIndex)
        {
            case widx::close_button:
            case widx::button_cancel:
                WindowManager::close(&window);
                break;

            case widx::button_ok:
                uint32_t status = window.var_846;
                WindowManager::close(&window);

                if (status == 0)
                    Scenario::generateLandscape();
                else
                    Scenario::eraseLandscape();
                break;
        }
    }

    static void init_events()
    {
        events.draw = draw;
        events.onMouseUp = onMouseUp;
    }

    // 0x004C180C
    Window* open(int32_t promptType)
    {
        auto window = WindowManager::bringToFront(WindowType::landscapeGenerationConfirm, 0);
        if (window == nullptr)
        {
            window = WindowManager::createWindowCentred(WindowType::landscapeGenerationConfirm, kWindowSize, WindowFlags::none, &events);
            window->widgets = widgets;
            window->enabledWidgets = (1 << widx::close_button) | (1 << widx::button_ok) | (1 << widx::button_cancel);
            window->initScrollWidgets();
            window->setColour(WindowColour::primary, AdvancedColour(Colour::mutedDarkRed).translucent());
            window->setColour(WindowColour::secondary, AdvancedColour(Colour::mutedDarkRed).translucent());
            window->flags |= WindowFlags::transparent;

            // TODO(avgeffen): only needs to be called once.
            init_events();
        }

        window->var_846 = promptType;
        if (promptType == 0)
            window->widgets[widx::caption].text = StringIds::title_generate_new_landscape;
        else
            window->widgets[widx::caption].text = StringIds::title_random_landscape_option;

        return window;
    }
}
