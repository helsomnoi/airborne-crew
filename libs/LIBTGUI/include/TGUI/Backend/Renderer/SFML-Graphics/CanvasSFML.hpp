/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2023 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef TGUI_CANVAS_SFML_HPP
#define TGUI_CANVAS_SFML_HPP

#include <TGUI/Backend/Renderer/SFML-Graphics/BackendTextureSFML.hpp>

#if !TGUI_BUILD_AS_CXX_MODULE
    #include <TGUI/Backend/Renderer/BackendRenderTarget.hpp>
    #include <TGUI/Widgets/ClickableWidget.hpp>
#endif

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TGUI_MODULE_EXPORT namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// CanvasSFML provides a way to directly render SFML contents on a widget
    ///
    /// When gui.draw() is called, all widgets are drawn at once. If you wish to have custom SFML rendering inbetween
    /// TGUI widgets (e.g. draw to the background of a child window) then you need to use a CanvasSFML widget.
    ///
    /// The canvas widget is essentially just a wrapper around sf::RenderTarget. You draw your SFML contents on top of the canvas
    /// instead of on the window. The canvas is then added to the gui between the widgets where you want the rendering to appear.
    ///
    /// You can redraw the contents of the canvas at any time, but make sure to always start by calling clear() and end with
    /// calling display().
    ///
    /// Example:
    /// @code
    /// sf::Sprite sprite;
    /// sf::Text text;
    ///
    /// auto canvas = tgui::CanvasSFML::create({400, 300});
    /// gui.add(canvas);
    ///
    /// canvas->clear();        // Clear the contents of the canvas
    /// canvas->draw(sprite);   // Draw an image to the canvas
    /// canvas->draw(text);     // Draw some text on the canvas
    /// canvas->display();      // Save what was drawn on the canvas
    /// @endcode
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TGUI_API CanvasSFML : public ClickableWidget
    {
    public:

        using Ptr = std::shared_ptr<CanvasSFML>; //!< Shared widget pointer
        using ConstPtr = std::shared_ptr<const CanvasSFML>; //!< Shared constant widget pointer

        static constexpr const char StaticWidgetType[] = "CanvasSFML"; //!< Type name of the widget


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// @brief Constructor
        /// @param typeName     Type of the widget
        /// @param initRenderer Should the renderer be initialized? Should be true unless a derived class initializes it.
        /// @see create
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        CanvasSFML(const char* typeName = StaticWidgetType, bool initRenderer = true);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Copy constructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        CanvasSFML(const CanvasSFML& copy);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Move constructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        CanvasSFML(CanvasSFML&& copy) noexcept;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Overload of copy assignment operator
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        CanvasSFML& operator= (const CanvasSFML& right);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Overload of move assignment operator
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        CanvasSFML& operator= (CanvasSFML&& right) noexcept;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Creates a new canvas widget
        ///
        /// @param size  Size of the canvas
        ///
        /// @return The new canvas
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD static CanvasSFML::Ptr create(const Layout2d& size = {"100%", "100%"});


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Makes a copy of another canvas
        ///
        /// @param canvas  The other canvas
        ///
        /// @return The new canvas
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD static CanvasSFML::Ptr copy(const CanvasSFML::ConstPtr& canvas);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the size of the widget
        ///
        /// @param size  The new size of the widget
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setSize(const Layout2d& size) override;
        using Widget::setSize;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Change the current active view
        ///
        /// The view is like a 2D camera, it controls which part of the 2D scene is visible, and how it is viewed in the canvas.
        /// The new view will affect everything that is drawn, until another view is set.
        /// The canvas keeps its own copy of the view, so it is not necessary to keep the original one alive after calling
        /// this function.
        /// To restore the original view of the target, you can pass the result of getDefaultView() to this function.
        ///
        /// @warning This view is reset when the size of the canvas is changed.
        ///
        /// @param view  New view to use
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setView(const sf::View& view);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Get the view currently in use in the canvas
        ///
        /// @return The view object that is currently used
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD const sf::View& getView() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Get the default view of the canvas
        ///
        /// The default view has the same size as the canvas.
        ///
        /// @return The default view of the canvas
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD const sf::View& getDefaultView() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Get the viewport of the currently applied view, applied to this canvas
        ///
        /// The viewport is defined in the view as a ratio, this function simply applies this ratio to the current dimensions
        /// of the canvas to calculate the pixels rectangle that the viewport actually covers in the canvas.
        ///
        /// @return Viewport rectangle, expressed in pixels
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD IntRect getViewport() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Clears the entire canvas with a single color
        ///
        /// This function is usually called once every frame, to clear the previous contents of the canvas.
        ///
        /// @param color Fill color to use to clear the canvas
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void clear(Color color = Color::Black);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Draws a drawable object to the canvas
        ///
        /// @param drawable Object to draw
        /// @param states   Render states to use for drawing
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void draw(const sf::Drawable& drawable, const sf::RenderStates& states = {});


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Draws primitives defined by an array of vertices
        ///
        /// @param vertices    Pointer to the vertices
        /// @param vertexCount Number of vertices in the array
        /// @param type        Type of primitives to draw
        /// @param states      Render states to use for drawing
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void draw(const sf::Vertex* vertices, std::size_t vertexCount,
                  sf::PrimitiveType type, const sf::RenderStates& states = {});


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Draws a TGUI sprite to the canvas
        ///
        /// @param sprite  Sprite to draw
        /// @param states  Render states to use for drawing
        ///
        /// @warning Only simple sprites can be drawn. There is currently no support for SVGs, shaders, clipping or rotation.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void draw(const Sprite& sprite, const RenderStates& states = {});


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Updates the contents of the canvas
        ///
        /// @warning Calling this function is mandatory at the end of rendering.
        ///          Not calling it may leave the texture in an undefined state.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void display();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Draw the widget to a render target
        ///
        /// @param target Render target to draw to
        /// @param states Current render states
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void draw(BackendRenderTarget& target, RenderStates states) const override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns whether the widget can gain focus
        /// @return Can the widget be focused?
        ///
        /// This function returns false for Canvas widgets.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD bool canGainFocus() const override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Access the internal render texture that the canvas widget uses to draw on
        ///
        /// @return Reference to the internal render texture
        ///
        /// @warning Don't call the create function on the returned render texture.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD sf::RenderTexture& getRenderTexture()
        {
            return m_renderTexture;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Makes a copy of the widget
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TGUI_NODISCARD Widget::Ptr clone() const override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        sf::RenderTexture m_renderTexture;
        Vector2u m_usedTextureSize;
    };
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_CANVAS_SFML_HPP
