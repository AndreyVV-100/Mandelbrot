#include "Mandelbrot.h"

int main()
{
    unsigned int pixels[HEIGHT][WIDTH] = {};

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Mandelbrot", sf::Style::Fullscreen);     
    MyImage img;
    
    RenderImage (img, (char*) *pixels);

    img.texture.update ((uint8_t*) *pixels);

    while (window.isOpen())
    {
        sf::Event event;

        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Escape))
            window.close();

        // ToDo: Think, why shift / scale on const not working
        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Add))
        {
            img.scale   *= 1.1;
            img.x_shift *= 1.1;
            img.y_shift *= 1.1;
        }

        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Subtract))
        {
            img.scale   /= 1.1;
            img.x_shift /= 1.1;
            img.y_shift /= 1.1;
        }

        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Left))
            img.x_shift -= 0.1;

        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Right))
            img.x_shift += 0.1;

        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Up))
            img.y_shift -= 0.1;

        if (sf::Keyboard::isKeyPressed (sf::Keyboard::Down))
            img.y_shift += 0.1;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        RenderImage (img, (char*) *pixels);
        img.texture.update ((uint8_t*) *pixels);

        window.clear();
        window.draw (img.set);
        window.display();
    }
}

Complex operator + (const Complex& a, const Complex& b)
{
    return {a.Re + b.Re, a.Im + b.Im};
}

void RenderImage (const MyImage& img, char* pixels)
{
    for (size_t y = 0; y < HEIGHT; y++)
    {
        for (size_t x = 0; x < WIDTH; x++)
        {
            Complex z0 = {((double) x / WIDTH + img.x_shift) / img.scale, ((double) y / WIDTH + img.y_shift) / img.scale};
            Complex z = z0;

            size_t  n = 0;
            size_t pixels_pos = 4 * (y*WIDTH + x);

            for (n; n < MAX_CHECK && z.abs2() < R2; n++)
                z = z.pow2() + z0;

            if (n == MAX_CHECK)
                *((unsigned int*)(pixels + pixels_pos)) = BLACK;
            else 
            {
                pixels[pixels_pos]     = 7 * n;         // Red
                pixels[pixels_pos + 1] = 128 + 15 * n;   // Green
                pixels[pixels_pos + 2] = 255 - 3 * n;  // Blue
                pixels[pixels_pos + 3] = 0xFF;          // Alpha
            }
        }
    }  

    return;
}

// R = 5 * n, G = n, B = 255 - 12 * n 
