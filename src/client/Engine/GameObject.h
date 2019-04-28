
#include <SFML/Graphics.hpp>
#include "../../configuration/config.h"

namespace cli
{

class TextureDrawer
{
 public:

    TextureDrawer(uint32_t type, sf::Packet &pack);

    void set_state_form_packet(sf::Packet &pack);

    void draw(sf::RenderWindow &window);

    void set_position(sf::Vector2f &pos);

    void set_position(sf::Vector2f &&pos);

    sf::Vector2f get_position() const;

 private:

    uint32_t _type;
    sf::Vector2f _position;
    sf::RectangleShape _texture;

};

}
