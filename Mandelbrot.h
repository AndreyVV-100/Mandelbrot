#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <immintrin.h>

const double MOUSE_WHEEL_SENSITIVITY = 0.05; 

const size_t WIDTH  = 1920;
const size_t HEIGHT = 1080;
const size_t MAX_CHECK = 256;
const double R2 = 100;

const unsigned int BLACK = 0xFF000000;

const __m256d FULL_COLORED = _mm256_cmp_pd (_mm256_set1_pd (0), _mm256_set1_pd (0), _CMP_EQ_OQ);
const __m256d MUL_2  = _mm256_set1_pd (2.0);
const __m256d R_NEED = _mm256_set1_pd (100);

struct MyImage
{
    sf::Texture texture;
    sf::Sprite  set;

    double scale = 0.23;
    double x_shift = -0.3, y_shift = 0; 

    MyImage ()
    {
        texture.create (WIDTH, HEIGHT);
        set.setTextureRect (sf::IntRect (0, 0, WIDTH, HEIGHT));
        set.setTexture (texture, false);
    }
};

struct Fps
{
    sf::Font  font;
    sf::Text  text;
    sf::Clock clock;
    sf::Time  time = clock.getElapsedTime();

    double time_prev = time.asSeconds();
    double time_now  = 0;
    double time_last_out = 0;

    const double FPS_DELAY = 0.15;

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
        time = clock.getElapsedTime();
        time_now = time.asSeconds(); // ToDo: rdtsc
        if (time_now - time_last_out > FPS_DELAY)
        {
            sprintf (str + 6, "%.2lf\n", 1 / (time_now - time_prev));
            text.setString (str);
            time_last_out = time_now;
        }
        time_prev = time_now;
        // printf ("%llu\n", _rdtsc);
    }
};

void RenderImage (const MyImage& img, unsigned int* pixels);
