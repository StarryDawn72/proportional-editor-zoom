#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
using namespace geode::prelude;

static constexpr float zoomFactor = 1.1f;
static constexpr float minZoom = 0.1f;
static constexpr float maxZoom = 99999.0f;

void clampZoom(float& zoom)
{
    if (zoom > maxZoom)
        zoom = maxZoom;
    else if (zoom < minZoom)
        zoom = minZoom;
}

class $modify(EditorUI)
{

    #if !(defined(GEODE_IS_WINDOWS) || defined(GEODE_IS_IOS))
    void zoomGameLayer(bool zoomingIn)
    {
        auto modEnabled = Mod::get()->getSettingValue<bool>("enabled");
        if (!modEnabled)
        {
            EditorUI::zoomGameLayer(zoomingIn);
            return;
        }

        float currentScale = m_editorLayer->m_objectLayer->getScale();
        
        if (zoomingIn)
            currentScale *= zoomFactor;
        else
            currentScale /= zoomFactor;
        
        clampZoom(currentScale);
        updateZoom(currentScale);
    }

    #else

    void zoomIn(CCObject* sender)
    {
        auto modEnabled = Mod::get()->getSettingValue<bool>("enabled");
        if (!modEnabled)
        {
            EditorUI::zoomIn(sender);
            return;
        }

        float currentScale = m_editorLayer->m_objectLayer->getScale();
        
        currentScale *= zoomFactor;
        clampZoom(currentScale);
        updateZoom(currentScale);
    }

    void zoomOut(CCObject* sender)
    {
        auto modEnabled = Mod::get()->getSettingValue<bool>("enabled");
        if (!modEnabled)
        {
            EditorUI::zoomOut(sender);
            return;
        }

        float currentScale = m_editorLayer->m_objectLayer->getScale();
        
        currentScale /= zoomFactor;
        clampZoom(currentScale);
        updateZoom(currentScale);
    }

    #endif

    void scrollWheel(float y, float x)
    {
        auto modEnabled = Mod::get()->getSettingValue<bool>("enabled");
        if (!modEnabled)
        {
            EditorUI::scrollWheel(y, x);
            return;
        }

        auto director = cocos2d::CCDirector::sharedDirector();
        auto kb = director->getKeyboardDispatcher();
        
        // check if we are zooming
        bool isPlaying = this->m_editorLayer->m_playbackMode == PlaybackMode::Playing;
        bool isZooming = kb->getControlKeyPressed() || kb->getCommandKeyPressed();

        if (!isPlaying && isZooming)
        {

            bool zoomingIn = (y < 0.0f || x < 0.0f);
            bool zoomingOut = (y > 0.0f || x > 0.0f);

            float currentScale = m_editorLayer->m_objectLayer->getScale();
            
            if (zoomingIn)
                currentScale *= zoomFactor;
            else if (zoomingOut)
                currentScale /= zoomFactor;
        
            clampZoom(currentScale);
            updateZoom(currentScale);

            return;
        }

        // let the vanilla function handle panning
        EditorUI::scrollWheel(y, x);
    }
};