
#include "Handler.h"

int main()
{
    sf::RenderTexture renderTexture;
    if (!renderTexture.create(SCREEN_WIDTH, SCREEN_HEIGHT)) {
        return EXIT_FAILURE;
    }


    Handler handler = Handler(BALL_COUNT, BALL_RADIUS);
    
    


    int t = FRAME_COUNT;
    std::cout << "Start\n";

    while(t--) {
        
        
        renderTexture.clear(sf::Color::Black);

      
        int buff = SUBFRAMES;



        while(buff--) handler.update_structured((FRAME_TIME)/SUBFRAMES);
        if (t % SINGLE_SPAWN_FRAME_INTERVAL == 0 ) {
            handler.singleSpawner(sf::Vector2f(SCREEN_WIDTH / 2-200, SCREEN_HEIGHT - 100.0f) ,g_VALUE , sf::Vector2f(40,0));
            handler.singleSpawner(sf::Vector2f(SCREEN_WIDTH / 2-100, SCREEN_HEIGHT - 100.0f), g_VALUE, sf::Vector2f(-40, 0));
            handler.singleSpawner(sf::Vector2f(SCREEN_WIDTH / 2+100, SCREEN_HEIGHT - 100.0f), g_VALUE, sf::Vector2f(40, 0));
            handler.singleSpawner(sf::Vector2f(SCREEN_WIDTH / 2+200, SCREEN_HEIGHT - 100.0f), g_VALUE, sf::Vector2f(-40, 0));

        }

        handler.draw(renderTexture);
        
        sf::Image screenshot = renderTexture.getTexture().copyToImage();
        std::string filename = "video\\frame" + std::to_string(FRAME_COUNT-t) + ".png"; // Change the filename format as needed
        screenshot.saveToFile(filename);
        std::cout << "Frame: "<<FRAME_COUNT-t<<"\r";
    }
    std::cout << "Fin";
    return 0;
}