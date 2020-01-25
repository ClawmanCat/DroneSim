#pragma once

#include "Core.h"
#include "IEntity.h"
#include "GameConstants.h"

// Not really an entity, but this saves us from having an extra base class just for this to derive from.
namespace DroneSim::Game {
    class EntityChar : public IEntity<EntityChar> {
    public:
        constexpr static std::string_view CHARSET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789:., ";
        constexpr static float KERNING = 36.0f;


        EntityChar(const Vec2f& where, char c) : IEntity(where, CharLookup(c)) {}

        constexpr static bool             MayChange(void)        { return true;                             }
        constexpr static Vec2f            GetSize(void)          { return { 35, 54 };                       }
        constexpr static u32              GetFrameCount(void)    { return CHARSET.length();                 }
        constexpr static std::string_view GetTexture(void)       { return "font";                           }


        void update(void) {
            // No need to update anything. Don't call superclass method.
        }


        void set(char c) { frame = CharLookup(c); }


        static std::vector<EntityChar> MakeText(Vec2f where, std::string_view text) {
            std::vector<EntityChar> result;
            
            for (char c : text) {
                result.push_back(EntityChar(where, c));
                where.x += KERNING;
            }

            return result;
        }
    private:
        static u32 CharLookup(char c) {
            for (u32 i = 0; i < CHARSET.length(); ++i) if (CHARSET[i] == c) return i;
            terminate("Attempt to get non-existent glyph from font.");
        }
    };
}