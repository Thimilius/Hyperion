#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/assets/texture_atlas_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    // NOTE: Currently we are assuming a single channel. So no support for RGB or RGBA yet.
    template<typename K, typename V>
    class TextureAtlasPacker final {
    public:
        TextureAtlasPacker(uint32 width, uint32 height, uint32 padding, uint32 row_size) {
            m_width = width;
            m_height = height;
            m_padding = padding;
            m_row_size = row_size;

            m_current_column = 0;
            m_current_row = 0;

            m_pixel_data.Resize(width * height);
            m_pixel_buffer = m_pixel_data.GetData();
        }
    public:
        bool AddElement(K key, const V &value, uint32 element_width, uint32 element_height, byte *element_buffer) {
            // Make sure we have enough space in the texture atlas to put the new element into it.
            if ((m_current_column + element_width + m_padding) >= m_width) {
                m_current_column = 0;
                m_current_row += m_row_size + m_padding;

                if ((m_current_row + element_height + m_padding) >= m_height) {
                    HYP_LOG_WARN("Engine", "Texture atlas is to small to fit in a new character!");
                    return false;
                }
            }

            TextureAtlasElement<V> element;
            element.payload = value;

            // Create the uvs that point to the corresponding location inside the texture atlas.
            // We have to remember that the origin of the font atlas is at the top left corner just like a regular image.
            // This means the y-coordinate (v) of the uvs starts at 1 and goes down instead of starting at 0 and going up.
            {
                float32 u = static_cast<float32>(m_current_column);
                float32 v = static_cast<float32>(m_height - m_current_row);

                float32 left = u / m_width;
                float32 right = (u + element_width) / m_width;
                float32 top = v / m_height;
                float32 bottom = (v - element_height) / m_height;

                element.uv_top_left = Vector2(left, top);
                element.uv_top_right = Vector2(right, top);
                element.uv_bottom_right = Vector2(right, bottom);
                element.uv_bottom_left = Vector2(left, bottom);
            }

            // Now we can put the actual element buffer into the texture atlas.
            // At this point we know we have enough space and we should therefore not overflow the texture atlas buffer.
            uint32 texture_data_index = m_current_column + (m_current_row * m_width);
            for (uint32 y = 0; y < element_height; y++) {
                for (uint32 x = 0; x < element_width; x++) {
                    m_pixel_buffer[texture_data_index + x] = element_buffer[x + (y * element_width)];
                }
                texture_data_index += m_width;
            }
            m_current_column += element_width + m_padding;

            HYP_ASSERT(m_elements.Find(key) == m_elements.end());
            m_elements.Insert(key, element);
            return true;
        }

        TextureAtlas<K, V> *CreateAtlas() {
            Rendering::Texture2DParameters parameters;
            parameters.width = m_width;
            parameters.height = m_height;
            parameters.format = Rendering::TextureFormat::R8;
            Texture2D *texture = AssetManager::CreateTexture2D(parameters, m_pixel_data);
            TextureAtlas<K, V> *texture_atlas = AssetManager::CreateTextureAtlas(texture, m_elements);
            return texture_atlas;
        }
    private:
        uint32 m_width;
        uint32 m_height;
        uint32 m_padding;
        uint32 m_row_size;

        uint32 m_current_column;
        uint32 m_current_row;

        Array<byte> m_pixel_data;
        byte *m_pixel_buffer;

        Map<K, TextureAtlasElement<V>> m_elements;
    private:
        friend class Hyperion::AssetManager;
    };

}