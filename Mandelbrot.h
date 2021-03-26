#include <stdio.h>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

const size_t WIDTH  = 1920;
const size_t HEIGHT = 1080;
const size_t MAX_CHECK = 256;
const double R2 = 100;

const unsigned int BLACK = 0xFF000000;
const unsigned int TEMPL = 0xFF000000;
const unsigned int B_COL = 0x10000;
const unsigned int G_COL = 0x100;

struct Complex
{
    double Re = 0;
    double Im = 0;

    Complex pow2 ()
    {   
        return {Re * Re - Im * Im, 2 * Re * Im};
    }

    double abs2 ()
    {
        return Re * Re + Im * Im;
    }
};

Complex operator + (const Complex& a, const Complex& b)
{
    return {a.Re + b.Re, a.Im + b.Im};
}

struct MyImage
{
    sf::Texture texture;
    sf::Sprite  set;

    double scale = 0.23;
    // double x_shift = -0.7, y_shift = -0.5; 
    double x_shift = -0.6, y_shift = -0.27; 

    MyImage ()
    {
        texture.create (WIDTH, HEIGHT);
        set.setTextureRect (sf::IntRect (0, 0, WIDTH, HEIGHT));
        set.setTexture (texture, false);
    }
};

struct Fps
{
    sf::Font font;
    sf::Text text;

    double time_prev = clock();
    double time_now  = 0;
    double time_last_out = 0;

    const double FPS_DELAY = CLOCKS_PER_SEC / 5;

    char str[16] = "fps = 000.00";

    Fps()
    {
        if (!font.loadFromFile ("font.ttf"))
            return;

        text.setFont (font);
        text.setCharacterSize (24); 
        text.setFillColor (sf::Color::Red);
    }

    void Renew()
    {
        time_now = clock();
        if (time_now - time_last_out > FPS_DELAY)
        {
            sprintf (str + 6, "%.2lf\n", CLOCKS_PER_SEC / (time_now - time_prev));
            text.setString (str);
            time_last_out = time_now;
        }
        time_prev = time_now;
    }
};


void RenderImage (const MyImage& img, char* pixels);
