#include "Mandelbrot.h"

int main()
{
    unsigned int pixels[HEIGHT][WIDTH] = {};

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Mandelbrot", sf::Style::Fullscreen);     
    MyImage img;
    Fps fps;

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                    // ToDo: Think, why shift / scale on const not working

                    case sf::Keyboard::Add:
                        img.scale   *= 1.1;
                        break;

                    case sf::Keyboard::Subtract:
                        img.scale   /= 1.1;
                        break;

                    case sf::Keyboard::A:
                    case sf::Keyboard::Left:
                        img.x_shift -= 0.1;
                        break;

                    case sf::Keyboard::D:
                    case sf::Keyboard::Right:
                        img.x_shift += 0.1;
                        break;

                    case sf::Keyboard::W:
                    case sf::Keyboard::Up:
                        img.y_shift -= 0.1;
                        break;

                    case sf::Keyboard::S:
                    case sf::Keyboard::Down:
                        img.y_shift += 0.1;
                        break;

                    case sf::Keyboard::Escape:
                        window.close();
                        printf ("OK...\n");
                        return 0;
                }
            }

            if (event.type == sf::Event::MouseWheelMoved)
                img.scale   *= 1 + MOUSE_WHEEL_SENSITIVITY * event.mouseWheel.delta;

            if (event.type == sf::Event::Closed)
            {
                window.close();
                return 0;
            }
        }

        RenderImage (img, *pixels);
        img.texture.update ((uint8_t*) *pixels);

        fps.Renew();

        window.clear();
        window.draw (img.set);
        window.draw (fps.text);
        window.display();
    }
}

void RenderImage (const MyImage& img, unsigned int* pixels)
{
    size_t pixels_pos = 0;

    for (size_t y = 0; y < HEIGHT; y++)
    {
        double Im_num = ((double) y / WIDTH - 0.5 * HEIGHT / WIDTH) / img.scale + img.y_shift;

        for (size_t x = 0; x < WIDTH; x += 4)
        {  
            // ToDo: No copypaste

            __m256d Re = _mm256_set_pd (((double) x / WIDTH - 0.5) / img.scale + img.x_shift,
                                  ((double) (x + 1) / WIDTH - 0.5) / img.scale + img.x_shift,
                                  ((double) (x + 2) / WIDTH - 0.5) / img.scale + img.x_shift,
                                  ((double) (x + 3) / WIDTH - 0.5) / img.scale + img.x_shift);
            __m256d Re0 = Re;
            __m256d Im = _mm256_set1_pd (Im_num);
            __m256d Im0 = Im;

            __m256d colored = _mm256_set1_pd (0);

            pixels_pos += 4;

            for (int i_pixel = 0; i_pixel < 4; i_pixel++) // All Black
                *(pixels + pixels_pos + i_pixel) = BLACK;

            __m256d Re_2 = _mm256_mul_pd (Re, Re);
            __m256d Im_2 = _mm256_mul_pd (Im, Im);

            for (size_t  n = 0; n < MAX_CHECK && !_mm256_testc_pd (colored, FULL_COLORED); n++)
            {
                Im = _mm256_fmadd_pd (MUL_2, _mm256_mul_pd (Re, Im), Im0);
                Re = _mm256_add_pd (_mm256_sub_pd (Re_2, Im_2), Re0);

                Re_2 = _mm256_mul_pd (Re, Re);
                Im_2 = _mm256_mul_pd (Im, Im);

                __m256d cmp = _mm256_cmp_pd (_mm256_add_pd (Re_2, Im_2), R_NEED, _CMP_GT_OQ);

                cmp = _mm256_andnot_pd (colored, cmp);

                for (int i_cmp = 0; i_cmp < 4; i_cmp++)
                    if (*((long long*) &cmp + i_cmp))
                        pixels[pixels_pos + (3 - i_cmp)] = BLACK + 0x10000 * (255 - 3 * n) + 0x100 * (128 + 15 * n) + 7 * n;

                colored = _mm256_or_pd (colored, cmp);
            }
        }
    }  

    return;
}
